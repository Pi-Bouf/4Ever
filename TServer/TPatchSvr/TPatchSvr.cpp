// TPatchSvr.cpp : WinMain�� �����Դϴ�.

#include "stdafx.h"
#include "TPatchSvr.h"
#include "TPatchSvrModule.h"

CTPatchSvrModule _AtlModule;

//extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
//                                LPTSTR /*lpCmdLine*/, int nShowCmd)
//{
//    return _AtlModule.WinMain(nShowCmd);
//}


void main()
{
	_AtlModule.StartServer();

	int i;
	cin >> i;
}


CTPatchSvrModule::CTPatchSvrModule() : TServerSystem("TPatch", "1.0.1")
{
	m_wPort = DEF_PATCHPORT;
	m_bNumWorker = 0;
	m_bServerID = 1;

	for( int i=0; i<MAX_THREAD; i++)
		m_hWorker[i] = NULL;
	m_hControl = NULL;

	m_hIocpControl = NULL;
	m_hIocpWork = NULL;

	m_mapTSESSION.clear();	

	m_accept = INVALID_SOCKET;
	m_bThreadRun = FALSE;
}

CTPatchSvrModule::~CTPatchSvrModule()
{
}

void CTPatchSvrModule::OnERROR( DWORD dwErrorCode)
{
}

DWORD CTPatchSvrModule::_ControlThread( LPVOID lpParam)
{
	CTPatchSvrModule *pModule = (CTPatchSvrModule *) lpParam;
	return pModule->ControlThread();
}

DWORD CTPatchSvrModule::_WorkThread( LPVOID lpParam)
{
	CTPatchSvrModule *pModule = (CTPatchSvrModule *) lpParam;
	return pModule->WorkThread();
}

DWORD CTPatchSvrModule::OnEnter()
{
	LogInfo("Load configuration...", 1);
	DWORD dwResult = LoadConfig();
	if (dwResult)
		return dwResult;

	LogInfo("Database initialization...", 1);
	dwResult = InitDB(&m_db);
	if (dwResult)
		return dwResult;

	LogInfo("Loading data...", 1);
	dwResult = LoadData(&m_db);
	if(dwResult)
		return dwResult;

	LogInfo("Create threads...", 1);
	dwResult = CreateThreads();
	if(dwResult)
		return dwResult;

	LogInfo("Init network...", 1);
	dwResult = InitNetwork();
	if(dwResult)
		return dwResult;

	if(!ResumeThreads())
		return EC_INITSERVICE_RESUMETHREAD;

	LogInfo("Ready !", 1);
	return 0;
}

void CTPatchSvrModule::OnExit()
{
	if( m_accept != INVALID_SOCKET )
		closesocket(m_accept);

	m_listen.Close();
	if(m_bThreadRun)
		ClearThread();

	DeleteCriticalSection(&m_csPC);
	UpdateData();
	m_db.Close();
	WSACleanup();
}

DWORD CTPatchSvrModule::LoadConfig()
{

	LoadStringFromIni("DSN", string("TGLOBAL_GSP"), &m_szDSN);
	LoadStringFromIni("DBUser", string("sa"), &m_szDBUserID);
	LoadStringFromIni("DBPasswd", string("1234"), &m_szDBPasswd);
	LoadIntFromIni("PatchPort", DEF_PATCHPORT, &m_wPort);
	LoadIntFromIni("ServerID", 1, &m_bServerID);

	DisplayIni();

	return EC_NOERROR;
}

DWORD CTPatchSvrModule::InitDB( CSqlDatabase *pDB)
{
	if(!pDB->Open( m_szDSN.c_str(), m_szDBUserID.c_str(), m_szDBPasswd.c_str()))
		return EC_INITSERVICE_DBOPENFAILED;

	if(!InitQueryTPatchSvr(pDB))
		return EC_INITSERVICE_PREPAREQUERY;

	return EC_NOERROR;
}

DWORD CTPatchSvrModule::LoadData( CSqlDatabase *pDB)
{
	DEFINE_QUERY(pDB, CSPLoadService)
	query->m_bWorld = 0;
	query->m_bServiceGroup = 6;
	if(!query->Call())
		return EC_INITSERVICE_DBOPENFAILED;

	m_strFTP = "http://patch.4story.pw";
	UNDEFINE_QUERY();	

	//IP http://195.138.240.239/
	/*DEFINE_QUERY(pDB, CSPLoadService)
	query->m_bWorld = SVRGRP_NULL;
	query->m_bServiceGroup = SVRGRP_PREFTPSVR;
	if(!query->Call())
		return EC_INITSERVICE_DBOPENFAILED;

	m_strPreFTP = query->m_szIP;
	UNDEFINE_QUERY();*/

	DEFINE_QUERY(pDB, CSPLoadService)
	query->m_bWorld = 0;
	query->m_bServiceGroup = 2;
	if(!query->Call())
		return EC_INITSERVICE_DBOPENFAILED;

	m_addrLOGIN.sin_addr.s_addr = inet_addr(query->m_szIP);
	m_addrLOGIN.sin_port = query->m_wPort;
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTPatchSvrModule::CreateThreads()
{
	DWORD dwThread = 0;
	SYSTEM_INFO vINFO;

	m_hControl = CreateThread(
		NULL, 0,
		_ControlThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThread);

	if(!m_hControl)
		return EC_INITSERVICE_CREATETHREAD;

	GetSystemInfo(&vINFO);
	m_bNumWorker = (BYTE) (2 * vINFO.dwNumberOfProcessors);

	for( BYTE i=0; i<m_bNumWorker; i++)
	{
		m_hWorker[i] = CreateThread(
			NULL, 0,
			_WorkThread,
			(LPVOID) this,
			CREATE_SUSPENDED,
			&dwThread);

		if(!m_hWorker[i])
			return EC_INITSERVICE_CREATETHREAD;
	}

	return EC_NOERROR;
}

BYTE CTPatchSvrModule::ResumeThreads()
{
	if( ResumeThread(m_hControl) < 0 )
		return FALSE;

	for( BYTE i=0; i<m_bNumWorker; i++)
		if( ResumeThread(m_hWorker[i]) < 0 )
			return FALSE;
	m_bThreadRun = TRUE;

	return TRUE;
}

DWORD CTPatchSvrModule::InitNetwork()
{
	WSADATA wsaDATA;
	WORD wVersionRequested = MAKEWORD( 2, 2);

	int nERROR = WSAStartup( wVersionRequested, &wsaDATA);
	if(nERROR)
		return EC_INITSERVICE_SOCKLIBFAILED;

	if( LOBYTE(wsaDATA.wVersion) != 2 ||
		HIBYTE(wsaDATA.wVersion) != 2 )
		return EC_INITSERVICE_INVALIDSOCKLIB;

	m_hIocpControl = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL, 0, 0);

	if(!m_hIocpControl)
		return EC_INITSERVICE_CREATEIOCP;

	m_hIocpWork = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL, 0, 0);

	if(!m_hIocpWork)
		return EC_INITSERVICE_CREATEIOCP;

	if(!m_listen.Listen(m_wPort))
		return EC_INITSERVICE_LISTENFAILED;

	m_hIocpControl = CreateIoCompletionPort(
		(HANDLE) m_listen.m_sock,
		m_hIocpControl,
		COMP_ACCEPT, 0);

	if(!m_hIocpControl)
		return EC_INITSERVICE_CREATEIOCP;

	if(!WaitForConnect())
		return EC_INITSERVICE_WAITFORCONNECT;

	InitializeCriticalSectionAndSpinCount(&m_csPC, 4000);

	return EC_NOERROR;
}

BYTE CTPatchSvrModule::WaitForConnect()
{
	DWORD dwRead = 0;

	if(!CSession::CreateSocket(m_accept))
		return FALSE;

	if(!AcceptEx(
		m_listen.m_sock,
		m_accept,
		m_vAccept.GetBuffer(), 0,
		sizeof(SOCKADDR) + 16,
		sizeof(SOCKADDR) + 16,
		&dwRead,
		(LPOVERLAPPED) &m_ovAccept) &&
		WSAGetLastError() != ERROR_IO_PENDING )
		return FALSE;

	return TRUE;
}

DWORD CTPatchSvrModule::ControlThread()
{
	while(TRUE)
	{
		DWORD dwCompKey = COMP_NULL;
		DWORD dwIoBytes = 0;

		LPOVERLAPPED pOV = NULL;

		if(!GetQueuedCompletionStatus(
			m_hIocpControl,
			&dwIoBytes,
			&dwCompKey,
			&pOV, INFINITE) &&
			GetLastError() != WAIT_TIMEOUT )
		{
			switch(dwCompKey)
			{
			case COMP_ACCEPT	:
				if(!WaitForConnect())
					LogEvent(_T("WaitForConnect : Error"));

				break;
			}
		}
		else if(pOV)
		{
			switch(dwCompKey)
			{
			case COMP_ACCEPT	:
				if(!Accept())
					LogEvent(_T("Accept : Error"));

				if(!WaitForConnect())
					LogEvent(_T("WaitForConnect : Error"));

				break;

			case COMP_CLOSE		: OnCloseSession((CTPatchSession *) pOV); break;
			}
		}
		else if( dwCompKey == COMP_EXIT )
			return 0;
	}

	return 0;
}

void CTPatchSvrModule::OnCloseSession( CTPatchSession *pSession)
{
	SMART_LOCKCS(&m_csPC);
	MAPTSESSION::iterator itSESSION = m_mapTSESSION.find((DWORD_PTR) pSession);

	if( itSESSION == m_mapTSESSION.end() )
		return;

	m_mapTSESSION.erase(itSESSION);
	delete pSession;
}

BYTE CTPatchSvrModule::Accept()
{
	if(m_accept == INVALID_SOCKET)
	{
		m_vAccept.Clear();
		return FALSE;
	}

	CTPatchSession *pSession = new CTPatchSession();

	pSession->Open( m_accept, m_vAccept);
	pSession->m_Recv.ExpandIoBuffer(RECV_CLI_SIZE);

	m_accept = INVALID_SOCKET;
	m_vAccept.Clear();

	HANDLE hIocpWork = CreateIoCompletionPort(
		(HANDLE) pSession->m_sock,
		m_hIocpWork,
		COMP_SESSION, 0);

	SMART_LOCKCS(&m_csPC);

	m_mapTSESSION.insert( MAPTSESSION::value_type( (DWORD_PTR) pSession, pSession));
	pSession->m_dwTick = GetTickCount();	

	if( !hIocpWork || !pSession->WaitForMessage() )
	{
		m_mapTSESSION.erase((DWORD_PTR) pSession);
		pSession->Close();
		delete pSession;

		return FALSE;
	}

	return TRUE;
}

DWORD CTPatchSvrModule::WorkThread()
{
	CSqlDatabase db;

	DWORD dwResult = InitDB(&db);
	BOOL bRun = TRUE;

	if(dwResult)
	{
		OnERROR(dwResult);

		return 0;
	}

	while(bRun)
	{
		DWORD dwCompKey = COMP_NULL;
		DWORD dwIoBytes = 0;

		LPOVERLAPPED pOV = NULL;

		if(!GetQueuedCompletionStatus(
			m_hIocpWork,
			&dwIoBytes,
			&dwCompKey,
			&pOV, INFINITE) &&
			GetLastError() != WAIT_TIMEOUT )
		{
			switch(dwCompKey)
			{
			case COMP_SESSION	:
				if(pOV)
				{
					CTPatchSession *pUser = (CTPatchSession *) ((COverlappedEx *) pOV)->m_pOBJ;
					BYTE bTYPE = ((COverlappedEx *) pOV)->m_bTYPE;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	: OnInvalidSession(pUser); break;
						// ***** IOCP ���� �� �˾Ƴ��� ���� ù��° ���� (������ ���� ����) *****
						//
						// ������ ���� closesocket()�� ȣ���Ͽ� ������ ������ ����̸�
						// WSARecv()�� ȣ��� ���¿����� �� �ڵ�� ������
						// ��� ������ ���۷��̼��� ����� �����̱� ������
						// �� ���� �ڵ�� ���õ� ����Ÿ�� IOCPť�� �������� �ʴ�.
						// ���� �� �����忡���� �ش� ���ǿ� ���õ� �۾� ����� ���̻� �������� �ʱ� ������
						// �ٸ� �����尡 ����Ѵٸ� �� �������� ���� �����͸� �����ص� �����ϴ�.
						// ������ ���� �ϴµ� ���� ���� �����̹Ƿ� ��ü �ý��� �����
						// �������� ���� ����� ���������� ���� ������ �����Ű���� �����ϴ� ���� �����ϴ�.
					case TOV_SSN_SEND:
						OnSendComplete(pUser, 0);
					}
				}

				break;
			}
		}
		else if(pOV)
		{
			BYTE bTYPE = ((COverlappedEx *) pOV)->m_bTYPE;

			switch(dwCompKey)
			{
			case COMP_SESSION	:
				{
					CTPatchSession *pUser = (CTPatchSession *) ((COverlappedEx *) pOV)->m_pOBJ;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	:
						if(pUser)
							ProcessSession( &db, pUser, dwIoBytes);

						break;

					case TOV_SSN_SEND	:
						if(pUser)
							OnSendComplete( pUser, dwIoBytes);

						break;
					}
				}

				break;
			}
		}
		else
		{
			switch(dwCompKey)
			{
			case COMP_EXIT		:
				{
					PostQueuedCompletionStatus(
						m_hIocpWork, 0,
						COMP_EXIT, NULL);

					bRun = FALSE;
				}

				break;
			}
		}
	}

	db.Close();

	return 0;
}

void CTPatchSvrModule::ClosingSession( CTPatchSession *pUser)
{
	// pUser�� ���� ��Ŷó���� �Ϸ�Ǵ� ������ �˸�
	// pUser�� ���� ������ ���۷��̼��� �Ϸ�� ���� Ȯ���� ȣ�� �Ͽ��� ��.

	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_CLOSE,
		(LPOVERLAPPED) pUser);
}

void CTPatchSvrModule::CloseSession( CTPatchSession *pUser)
{
	pUser->CloseSession();
}

void CTPatchSvrModule::OnSendComplete( CTPatchSession *pUser, DWORD dwIoBytes)
{
	if(pUser->SendComplete(dwIoBytes))
		ClosingSession(pUser);
}

void CTPatchSvrModule::ProcessSession( CSqlDatabase *pDB, CTPatchSession *pUser, DWORD dwIoBytes)
{
	BOOL bContinue = TRUE;

	if(!pUser->Read(dwIoBytes))
	{
		// ***** IOCP ���� �� �˾Ƴ��� ���� �ι�° ���� (Ŭ���̾�Ʈ�� ���� ����) *****
		//
		// Ŭ���̾�Ʈ�� ���� closesocket()�� ȣ���Ͽ� ������ ������ ����̸�
		// WSARecv()�� ȣ��� ���¿����� �� �ڵ�� ������
		// ��� ������ ���۷��̼��� ���� �Ǿ��ٰ� �� �� ����.
		// ���� ���Ŀ� �� �����忡�� �� ���ǰ� ���õ� �۾������ ���� �� �� �����Ƿ�
		// ���⼭ ���� �����͸� �����ϸ� ������ �ٿ�� �� �ִ�.
		// Receive�� ���õ� ������ ���۷��̼��� Ȯ���� ���� �Ǿ����Ƿ�
		// Send�� ���õ� ������ ���۷��̼��� ����Ǿ�������
		// Ȯ���� �� �ٸ� �������� ���� ���� ���������� ��ġ�� ������ ���� �ؾ� �Ѵ�.
		OnInvalidSession(pUser);
		return;
	}

	while(bContinue)
	{
		DWORD dwResult = pUser->CheckMessage();

		switch(dwResult)
		{
		//case PACKET_INVALID		:
		case PACKET_INCOMPLETE	: bContinue = FALSE; break;
		case PACKET_COMPLETE	:
			{
				DWORD dwResult = OnReceive( pDB, pUser);

				if(dwResult)
				{
					OnInvalidSession(pUser);
					OnERROR(dwResult);

					return;
				}

				pUser->Flush();
			}

			break;

		default					: OnInvalidSession(pUser); return;
			// ***** IOCP ���� �� �˾Ƴ��� ���� ����° ���� (�� �������� ���� ����) *****
			//
			// Ŭ���̾�Ʈ�� ��Ŷ�� �����ؼ� �����ٰų� ��Ʈ�p ������ ���� ������ �� �������� ���°� �� ����̸�
			// WSARecv()�� ȣ��� ���¿����� �� �ڵ�� ������
			// ��� ������ ���۷��̼��� ���� �Ǿ��ٰ� �� �� ����.
			// ���� ���Ŀ� �� �����忡�� �� ���ǰ� ���õ� �۾������ ���� �� �� �����Ƿ�
			// ���⼭ ���� �����͸� �����ϸ� ������ �ٿ�� �� �ִ�.
			// Receive�� ���õ� ������ ���۷��̼��� Ȯ���� ���� �Ǿ����Ƿ�
			// Send�� ���õ� ������ ���۷��̼��� ����Ǿ�������
			// Ȯ���� �� �ٸ� �������� ���� ���� ���������� ��ġ�� ������ ���� �ؾ� �Ѵ�.
			//
			// *** �������� �ʴ� ��� ***
			// Ȥ�ó� ���⼭ closesocket()�� ȣ�� �Ͽ�
			// ������ �������� ���μ����� ������ �õ��� ���� �ʴ� ���� ����.
			// closesocket()�� ȣ���ص� WSARecv()�� ȣ����� ���� �����̱� ������
			// ������ �������� ���μ����� ���� ���Ѵ�. ���� WSARecv()�� ����
			// ȣ���ϰ� �ٷ� closesocket()�� ȣ���ϸ� ���μ������� ������ ���� �� ���� ������
			// �� �������� ������ ������� �׷� �׼��� �ϴ� ���� �����ϴ�.
		}
	}

	if(!pUser->WaitForMessage())
	{
		// ***** IOCP ���� �� �˾Ƴ��� ���� �׹�° ���� (�� �������� ���� ����) *****
		//
		// ��Ʈ�p ������ ���� ������ �� �������� ���¿��� WSARecv()�Լ� ȣ���� ������ ����̸�
		// WSARecv()�� ȣ��� ���¿����� �� �ڵ�� ������
		// ��� ������ ���۷��̼��� ���� �Ǿ��ٰ� �� �� ����.
		// ���� ���Ŀ� �� �����忡�� �� ���ǰ� ���õ� �۾������ ���� �� �� �����Ƿ�
		// ���⼭ ���� �����͸� �����ϸ� ������ �ٿ�� �� �ִ�.
		// Receive�� ���õ� ������ ���۷��̼��� Ȯ���� ���� �Ǿ����Ƿ�
		// Send�� ���õ� ������ ���۷��̼��� ����Ǿ�������
		// Ȯ���� �� �ٸ� �������� ���� ���� ���������� ��ġ�� ������ ���� �ؾ� �Ѵ�.
		OnInvalidSession(pUser);
	}
}

void CTPatchSvrModule::OnInvalidSession( CTPatchSession *pUser)
{
	if(pUser->OnInvalidSession())
		ClosingSession(pUser);
}

void CTPatchSvrModule::ClearThread()
{
	PostQueuedCompletionStatus(
		m_hIocpWork, 0,
		COMP_EXIT, NULL);

	WaitForMultipleObjects(
		m_bNumWorker,
		m_hWorker,
		TRUE, INFINITE);

	for( BYTE i=0; i<m_bNumWorker; i++)
		CloseHandle(m_hWorker[i]);

	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_EXIT, NULL);

	WaitForSingleObject( m_hControl, INFINITE);
	CloseHandle(m_hControl);

	CloseHandle(m_hIocpControl);
	CloseHandle(m_hIocpWork);
}

void CTPatchSvrModule::UpdateData()
{
	m_mapTSESSION.clear();	
}

DWORD CTPatchSvrModule::OnReceive( CSqlDatabase *pDB, CTPatchSession *pSession)
{
	switch(pSession->m_Recv.GetID())
	{
		ON_RECEIVE(CT_SERVICEMONITOR_ACK)
		ON_RECEIVE(CT_PATCH_REQ)
		ON_RECEIVE(CT_PATCHSTART_REQ)
		ON_RECEIVE(CT_SERVICEDATACLEAR_ACK)
		ON_RECEIVE(CT_CTRLSVR_REQ)
		ON_RECEIVE(CT_NEWPATCH_REQ)
		ON_RECEIVE(CT_CHANGEIF_REQ)
		ON_RECEIVE(CT_PREPATCH_REQ)
		ON_RECEIVE(CT_PREPATCHCOMPLETE_REQ)
	}

	return EC_SESSION_INVALIDMSG;
}