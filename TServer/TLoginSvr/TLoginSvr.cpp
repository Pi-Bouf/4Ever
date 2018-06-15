#include "stdafx.h"
#include "TLoginSvr.h"
#include "TLoginSvrModule.h"
#include <fstream>
#include <iostream>
#include "SimpleIni.h"

INT64 dwKey2  = 463737125;


void main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 14);
	cout << endl;
	cout << " #################################################################### " << endl;
	cout << " #         .o    .oooooo..o     .                                   # " << endl;
	cout << " #       .d88   d8P'    `Y8   .o8                                   # " << endl;
	cout << " #     .d'888   Y88bo.      .o888oo  .ooooo.  oooo d8b oooo    ooo  # " << endl;
	cout << " #   .d'  888    `\"Y8888o.    888   d88' `88b `888\"\"8P  `88.  .8'   # " << endl;
	cout << " #   88ooo888oo      `\"Y88b   888   888   888  888       `88..8'    # " << endl;
	cout << " #        888   oo     .d8P   888 . 888   888  888        `888'     # " << endl;
	cout << " #       o888o  8\"\"88888P'    \"888\" `Y8bod8P' d888b        .8'      # " << endl;
	SetConsoleTextAttribute(hConsole, 13);
	cout << " #       TLOGIN SERVER";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "                                 .o..P'       # " << endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout << " #       Version: 1.0.1";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "                                `Y8P'        # " << endl;
	cout << " #################################################################### " << endl << endl;

	CTLoginSvrModule ct = CTLoginSvrModule();
	ct.StartServer();

	int i = 0;
	cin >> i;
}

CTLoginSvrModule::CTLoginSvrModule()
{
	timeNow = time(0);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	LogInfo("Welcome on TLogin Server !");

	m_wPort		= DEF_LOGINPORT;
	m_bNumWorker= 0;
	m_bServerID = 0;
	m_dlCheckFile = 0;
	m_hExecFile = INVALID_HANDLE_VALUE;

	m_wLogServerPORT = 0;

	m_pDebugSocket = new CDebugSocket();

#ifdef DEF_UDPLOG
	m_pUdpSocket = new CUdpSocket();
#endif

	for( int i=0; i<MAX_THREAD; i++)
		m_hWorker[i] = NULL;
	m_hControl = NULL;

	m_hIocpControl = NULL;
	m_hIocpWork = NULL;

	m_mapTSESSION.clear();
	m_mapTUSER.clear();
	m_mapACTIVEUSER.clear();

	m_accept = INVALID_SOCKET;
	m_bThreadRun = FALSE;

	m_mapEVENT.clear();
	m_mapCurrentUser.clear();
	m_vVETERAN.clear();
}

CTLoginSvrModule::~CTLoginSvrModule()
{
	if(m_hExecFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hExecFile);
		m_hExecFile = INVALID_HANDLE_VALUE;
	}

#ifdef DEF_UDPLOG
	delete	m_pUdpSocket;
	m_pUdpSocket = NULL;
#endif

}

void CTLoginSvrModule::LogInfo(string text)
{
	tm *ltm = localtime(&timeNow);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] ==> " << text << endl;
}

void CTLoginSvrModule::LogError(string text)
{
	tm *ltm = localtime(&timeNow);
	SetConsoleTextAttribute(hConsole, 4);
	cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] ==> " << text << endl;
}

void CTLoginSvrModule::OnERROR( DWORD dwErrorCode)
{
}

DWORD CTLoginSvrModule::_ControlThread( LPVOID lpParam)
{
	CTLoginSvrModule *pModule = (CTLoginSvrModule *) lpParam;

	return pModule->ControlThread();
}

DWORD CTLoginSvrModule::_WorkThread( LPVOID lpParam)
{
	CTLoginSvrModule *pModule = (CTLoginSvrModule *) lpParam;
	return pModule->WorkThread();
}

LPTGROUP CTLoginSvrModule::FindGroup( LPMAPTGROUP pGROUP, BYTE bGroupID)
{
	MAPTGROUP::iterator finder = pGROUP->find(bGroupID);

	if( finder != pGROUP->end() )
		return (*finder).second;

	return NULL;
}

void CTLoginSvrModule::StartServer()
{
	DWORD dwResult = StartUp();

	if(dwResult)
	{
		OnERROR(dwResult);
	}
}

HRESULT CTLoginSvrModule::PostMessageLoop()
{
	OnExit();

	return 0;
}

DWORD CTLoginSvrModule::StartUp()
{
//	ULONG addr = inet_addr("79.110.88.47");

	LogInfo("Staring Up...");
	DWORD dwResult = LoadConfig();
	if(dwResult)
		return dwResult;

	dwResult = InitDB(&m_db);
	if(dwResult)
		return dwResult;

	dwResult = LoadDataLogin();
	if(dwResult)
		return dwResult;

	dwResult = CreateThreads();
	if(dwResult)
		return dwResult;

	dwResult = InitNetwork();
	if(dwResult)
		return dwResult;

	if(!ResumeThreads())
		return EC_INITSERVICE_RESUMETHREAD;

	ClearLoginUser();

	return 0;
}

void CTLoginSvrModule::ClearLoginUser()
{
	DEFINE_QUERY(&m_db, CSPClearLoginUser)
	query->Call();
	UNDEFINE_QUERY()
}

void CTLoginSvrModule::OnExit()
{
	if( m_accept != INVALID_SOCKET )
		closesocket(m_accept);

	m_listen.Close();
	if(m_bThreadRun)
		ClearThread();

	if(m_hExecFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hExecFile);
		m_hExecFile = INVALID_HANDLE_VALUE;
	}

	DeleteCriticalSection(&m_csLI);
	UpdateData();
	m_db.Close();
	WSACleanup();
}

/*
 *	LoadConfig()
 *
 *	- Get Server Config from registry
 */
DWORD CTLoginSvrModule::LoadConfig()
{
	LogInfo("Load config from TLogin.ini file");

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(".\\TLogin.ini");

	m_szDBPasswd = ini.GetValue("TLoginConfig", "DBPasswd", "1234");
	m_szDBUserID = ini.GetValue("TLoginConfig", "DBUser", "sa");
	m_szDSN = ini.GetValue("TLoginConfig", "DSN", "TGLOBAL_GSP");
	m_bServerID = ini.GetValue("TLoginConfig", "ServerID", "1");
	m_wPort = ini.GetLongValue("TLoginConfig", "LoginPort", 5336);
	m_szLogServerIP = ini.GetValue("TLoginConfig", "LogIP", "127.0.0.1");
	m_wLogServerPORT = stoi(ini.GetValue("TLoginConfig", "LogPort", "7000"));

	LogInfo("Data from TLogin.ini:");
	SetConsoleTextAttribute(hConsole, 11);
	cout << "  ###################################" << endl;
	cout << "  # + DSN:       ";
	SetConsoleTextAttribute(hConsole, 7);
	cout << m_szDSN << endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout << "  # + DBUser:    ";
	SetConsoleTextAttribute(hConsole, 7);
	cout << m_szDBUserID << endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout << "  # + DBPasswd:  ";
	SetConsoleTextAttribute(hConsole, 7);
	cout << m_szDBPasswd << endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout << "  # + LoginPort: ";
	SetConsoleTextAttribute(hConsole, 7);
	cout << m_wPort << endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout << "  # + ServerID:  ";
	SetConsoleTextAttribute(hConsole, 7);
	cout << m_bServerID << endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout << "  # + LogIP:     ";
	SetConsoleTextAttribute(hConsole, 7);
	cout << m_szLogServerIP << endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout << "  # + LogPort:   ";
	SetConsoleTextAttribute(hConsole, 7);
	cout << m_wLogServerPORT << endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout << "  ###################################" << endl;

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::InitDB( CSqlDatabase *pDB)
{
	LogInfo("Database connection...");
	if (!pDB->Open(m_szDSN, m_szDBUserID, m_szDBPasswd)) {
		LogError("Connection error ! Check your DSN, DBUser and DBPasswd !");
		cout << endl;
		LogError("1: Start the 'SQL Server' service");
		LogError("2: check your ODBC configuration (64 bits AND 32 bits)");
		LogError("3: check if you have... a database in your server ? :)");
		return EC_INITSERVICE_DBOPENFAILED;
	}

	if (!InitQueryTLoginSvr(pDB)) {
		LogError("Connection error ! Can't init query on database connection !");
		return EC_INITSERVICE_PREPAREQUERY;
	}

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::LoadDataLogin()
{
	DEFINE_QUERY(&m_db, CSPLoadService)
	query->m_bWorld = SVRGRP_NULL;
	query->m_bServiceGroup = SVRGRP_CTLSVR;
	if(!query->Call())
		return EC_INITSERVICE_DBOPENFAILED;

	m_addrCtrl.sin_addr.s_addr = inet_addr(query->m_szIP);
	m_addrCtrl.sin_port = query->m_wPort;
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CSPGetNation)
	if(!query->Call())
		return EC_INITSERVICE_NATION;
	m_bNation = query->m_bNation;
	if(!m_bNation)
		return EC_INITSERVICE_NATION;
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLUserCount)
	if(query->Open())
	{
		while(query->Fetch())
			m_mapCurrentUser.insert(MAPDWORD::value_type(query->m_bGroup, query->m_dwCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLVeteranChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			TVETERAN Veteran;

			Veteran.m_bOption = query->m_bID;
			Veteran.m_bLevel = query->m_bLevel;

			m_vVETERAN.push_back(Veteran);
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	TCHAR szFN[ONE_KBYTE];

	GetModuleFileName(NULL, szFN, ONE_KBYTE);
	char *pSlash = ::strrchr(szFN, '\\');
	if( pSlash )
		::lstrcpy(pSlash + 1, "\\TClient.exe");

	m_hExecFile = CreateFile(
		szFN,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_RANDOM_ACCESS,
		NULL);

	if(m_hExecFile != INVALID_HANDLE_VALUE)
	{
		INT64 dlValue = 0;
		DWORD dwNumberOfBytesRead = 0;

		BYTE bResult = ReadFile(m_hExecFile, (LPVOID)&dlValue, sizeof(INT64), &dwNumberOfBytesRead, NULL);
		while(bResult && dwNumberOfBytesRead)
		{
			m_dlCheckFile ^= dlValue;
			bResult = ReadFile(m_hExecFile, (LPVOID)&dlValue, sizeof(INT64), &dwNumberOfBytesRead, NULL);
		}
	}

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::LoadData( CSqlDatabase *pDB, LPMAPTGROUP pGROUP)
{
	DEFINE_QUERY( pDB, CTBLGroup)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTGROUP pGroup = new TGROUP();

			pGroup->m_strDBUser = query->m_szDBUser;
			pGroup->m_strPasswd = query->m_szPasswd;
			pGroup->m_strNAME = query->m_szNAME;
			pGroup->m_strDSN = query->m_szDSN;
			pGroup->m_bID = query->m_bGroupID;
			pGroup->m_bType = query->m_bType;
			pGROUP->insert( MAPTGROUP::value_type( pGroup->m_bID, pGroup));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	MAPTGROUP::iterator it;
	for( it = pGROUP->begin(); it != pGROUP->end(); it++)
	{
		LPTGROUP pGroup = (*it).second;

		if(!pGroup->m_db.Open( pGroup->m_strDSN, pGroup->m_strDBUser, pGroup->m_strPasswd))
			return EC_INITSERVICE_DBOPENFAILED;

		if(!InitQueryTGameSvr(&pGroup->m_db))
			return EC_INITSERVICE_PREPAREQUERY;
	}

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::CreateThreads()
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

BYTE CTLoginSvrModule::ResumeThreads()
{
	if( ResumeThread(m_hControl) < 0 )
		return FALSE;

	for( BYTE i=0; i<m_bNumWorker; i++)
		if( ResumeThread(m_hWorker[i]) < 0 )
			return FALSE;
	m_bThreadRun = TRUE;

	return TRUE;
}

/*
 *	InitNetwork()
 */
DWORD CTLoginSvrModule::InitNetwork()
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

	InitializeCriticalSectionAndSpinCount(&m_csLI, 4000);

	char * m_szLogServerIPTemp = const_cast<char*>(m_szLogServerIP);
	if( !m_pDebugSocket->Initialize(m_szLogServerIPTemp, m_wLogServerPORT) )
	{
		LogInfo("DebugSocket could not be initializised!");

		return EC_INITSERVICE_UDPSOCKETFAILED;
	}

	//	Initialize UDP
#ifdef DEF_UDPLOG

	if( !m_pUdpSocket->Initialize(m_szLogServerIPTemp, m_wLogServerPORT) )
	{
		LogInfo("Fail then Initialize UDP ");

		return EC_INITSERVICE_UDPSOCKETFAILED;
	}

#endif

	return EC_NOERROR;
}

BYTE CTLoginSvrModule::WaitForConnect()
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

DWORD CTLoginSvrModule::ControlThread()
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
					LogInfo(_T("WaitForConnect : Error"));

				break;
			}
		}
		else if(pOV)
		{
			switch(dwCompKey)
			{
			case COMP_ACCEPT	:
				if(!Accept())
					LogInfo(_T("Accept : Error"));

				if(!WaitForConnect())
					LogInfo(_T("WaitForConnect : Error"));

				break;

			case COMP_CLOSE		: OnCloseSession((CTUser *) pOV); break;
			}
		}
		else if( dwCompKey == COMP_EXIT )
			return 0;
	}

	return 0;
}

/*
 *	OnCloseSession
 */
void CTLoginSvrModule::OnCloseSession(CTUser *pUser)
{
	SMART_LOCKCS(&m_csLI);

		MAPTUSER::iterator itSESSION = m_mapTSESSION.find((DWORD_PTR) pUser);

		if( itSESSION == m_mapTSESSION.end() )
			return;

		MAPTUSER::iterator itUSER = m_mapTUSER.find(pUser->m_dwID);
		if( itUSER != m_mapTUSER.end() )
			m_mapTUSER.erase(itUSER);

		if(pUser->m_bLogout)
		{
			DEFINE_QUERY( &m_db, CSPLogout)
			
				query->m_dwID = pUser->m_dwID;
				query->m_dwCharID = 0;
				query->m_bLevel = 0;
				query->m_dwExp = 0;
				query->Call();

			UNDEFINE_QUERY()
		}

		m_mapTSESSION.erase(itSESSION);

		delete pUser;
}

BYTE CTLoginSvrModule::Accept()
{
#ifndef _DEBUG
	if(IsDebuggerPresent())
		return FALSE;
#endif

	if(m_accept == INVALID_SOCKET)
	{
		m_vAccept.Clear();
		return FALSE;
	}

	BYTE bError = FALSE;
	CTUser *pUser = new CTUser();

	pUser->Open( m_accept, m_vAccept);
	pUser->m_Recv.ExpandIoBuffer(RECV_CLI_SIZE);

	if( pUser->m_addr.sin_addr.s_addr == m_addrCtrl.sin_addr.s_addr )
		pUser->m_bSessionType = SESSION_SERVER;
	else
	{
		pUser->m_bUseCrypt = TRUE;

/*#ifndef _DEBUG
		SOCKADDR_IN *pAddr = (SOCKADDR_IN *) (m_vAccept.GetBuffer() + 10);
		if(pAddr->sin_addr.s_addr << 8 != 0x5F6E4F00 && pAddr->sin_addr.s_addr << 8 != 0xAFFDCE00)
			switch(pAddr->sin_addr.s_addr)
			{
			case 1358472636:
				break;
			default:
				bError = TRUE;
				break;
			}
#endif*/
	}

	m_accept = INVALID_SOCKET;
	m_vAccept.Clear();

	if(bError)
	{
		pUser->Close();
		delete pUser;

		return FALSE;
	}

/*
	if( pUser->m_bSessionType == SESSION_CLIENT &&
		MAKEWORD(pUser->m_addr.sin_zero[1], pUser->m_addr.sin_zero[0]) != TVERSION)
	{
		delete pUser;
		return FALSE;
	}
*/
	HANDLE hIocpWork = CreateIoCompletionPort(
		(HANDLE) pUser->m_sock,
		m_hIocpWork,
		COMP_SESSION, 0);

	SMART_LOCKCS(&m_csLI)

	if( !hIocpWork || !pUser->WaitForMessage() )
	{
		pUser->Close();
		delete pUser;

		return FALSE;
	}

	pUser->m_dwAcceptTick = GetTickCount();

	ATLTRACE2( "SESSION INSERT %s, %d\n", inet_ntoa(pUser->m_addr.sin_addr), m_mapTSESSION.size());
	m_mapTSESSION.insert( MAPTUSER::value_type( (DWORD_PTR) pUser, pUser));

	return TRUE;
}

DWORD CTLoginSvrModule::WorkThread()
{
	MAPTGROUP mapGROUP;
	CSqlDatabase db;

	DWORD dwResult = InitDB(&db);
	BOOL bRun = TRUE;

	if(dwResult)
	{
		OnERROR(dwResult);

		LogInfo(_T("Game ODBC Connect Error"));
		return 0;
	}

	dwResult = LoadData( &db, &mapGROUP);
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
					CTUser *pUser = (CTUser *) ((COverlappedEx *) pOV)->m_pOBJ;
					BYTE bTYPE = ((COverlappedEx *) pOV)->m_bTYPE;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	: OnInvalidSession(pUser); break;
						// ***** IOCP 사용법 중 알아내기 힘든 첫번째 구문 (서버측 세션 종료) *****
						//
						// 서버가 먼저 closesocket()을 호출하여 세션을 종료한 경우이며
						// WSARecv()가 호출된 상태에서만 이 코드로 들어오며
						// 모든 오버랩 오퍼래이션이 종료된 상태이기 때문에
						// 이 소켓 핸들과 관련된 데이타는 IOCP큐에 남아있지 않다.
						// 따라서 이 스레드에서는 해당 세션에 관련된 작업 명령을 더이상 수행하지 않기 때문에
						// 다른 스레드가 허락한다면 이 구문에서 세션 포인터를 삭제해도 무방하다.
						// 세션을 삭제 하는데 가장 좋은 지점이므로 전체 시스템 설계시
						// 정상적인 세션 종료는 서버측에서 먼저 세션을 종료시키도록 설계하는 것이 안전하다.
					case TOV_SSN_SEND:
						OnSendComplete(pUser, 0);
						break;
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
					CTUser *pUser = (CTUser *) ((COverlappedEx *) pOV)->m_pOBJ;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	:
						if(pUser)
							ProcessSession( &db, &mapGROUP, pUser, dwIoBytes);


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

	MAPTGROUP::iterator it;
	for( it = mapGROUP.begin(); it != mapGROUP.end(); it++)
		delete (*it).second;

	mapGROUP.clear();
	db.Close();

	return 0;
}

void CTLoginSvrModule::ClosingSession( CTUser *pUser)
{
	// pUser에 대한 패킷처리가 완료되는 시점을 알림
	// pUser에 대한 오버랩 오퍼래이션이 완료된 것을 확인후 호출 하여야 함.

	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_CLOSE,
		(LPOVERLAPPED) pUser);
}

void CTLoginSvrModule::CloseSession( CTUser *pUser)
{
	pUser->CloseSession();
}

void CTLoginSvrModule::OnSendComplete( CTUser *pUser, DWORD dwIoBytes)
{
	if(pUser->SendComplete(dwIoBytes))
		ClosingSession(pUser);
}

void CTLoginSvrModule::ProcessSession( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, DWORD dwIoBytes)
{
	BOOL bContinue = TRUE;

	if(!pUser->Read(dwIoBytes))
	{
		// ***** IOCP 사용법 중 알아내기 힘든 두번째 구문 (클라이언트측 세션 종료) *****
		//
		// 클라이언트가 먼저 closesocket()을 호출하여 세션을 종료한 경우이며
		// WSARecv()가 호출된 상태에서만 이 코드로 들어오며
		// 모든 오버랩 오퍼래이션이 종료 되었다고 볼 수 없다.
		// 따라서 이후에 이 스레드에서 이 세션과 관련된 작업명령이 실행 될 수 있으므로
		// 여기서 세션 포인터를 삭제하면 서버가 다운될 수 있다.
		// Receive와 관련된 오버랩 오퍼레이션은 확실히 종료 되었으므로
		// Send와 관련된 오버랩 오퍼레이션이 종료되었는지를
		// 확인한 후 다른 스레드의 세션 삭제 수락과정을 거치고 세션을 삭제 해야 한다.
		OnInvalidSession(pUser);
		return;
	}

	while(bContinue)
	{
		DWORD dwResult = pUser->CheckMessage();
		WORD wMsgID = pUser->m_Recv.GetID();

		if(pUser)
		{
			if(pUser->m_bSessionType == SESSION_CLIENT)
			{
				if(pUser->m_bLock && wMsgID != CS_LOGIN_REQ)
				{
					//m_pDebugSocket->LogNonVaildPacket(inet_ntoa(pPlayer->m_addr.sin_addr), wMsgID);
					dwResult = PACKET_INVALID;
				}
			}
		} 

		switch(dwResult)
		{
		case PACKET_INCOMPLETE	: bContinue = FALSE; break;
		case PACKET_COMPLETE	:
			{
				DWORD dwResult = OnReceive( pDB, pGROUP, pUser, pUser->m_Recv, dwKey2);

				if(dwResult)
				{
					OnInvalidSession(pUser);
					OnERROR(dwResult);

					return;
				}

				pUser->Flush();
			}

			break;

		default					: 
			
			m_pDebugSocket->LogInvalidPacket(inet_ntoa(pUser->m_addr.sin_addr));

			OnInvalidSession(pUser);
			return;
			// ***** IOCP 사용법 중 알아내기 힘든 세번째 구문 (비 정상적인 세션 종료) *****
			//
			// 클라이언트가 패킷을 변조해서 보낸다거나 네트웤 오류로 인해 세션이 비 정상적인 상태가 된 경우이며
			// WSARecv()가 호출된 상태에서만 이 코드로 들어오며
			// 모든 오버랩 오퍼래이션이 종료 되었다고 볼 수 없다.
			// 따라서 이후에 이 스레드에서 이 세션과 관련된 작업명령이 실행 될 수 있으므로
			// 여기서 세션 포인터를 삭제하면 서버가 다운될 수 있다.
			// Receive와 관련된 오버랩 오퍼레이션은 확실히 종료 되었으므로
			// Send와 관련된 오버랩 오퍼레이션이 종료되었는지를
			// 확인한 후 다른 스레드의 세션 삭제 수락과정을 거치고 세션을 삭제 해야 한다.
			//
			// *** 권장하지 않는 편법 ***
			// 혹시나 여기서 closesocket()을 호출 하여
			// 서버측 세션종료 프로세스로 들어가려는 시도는 하지 않는 것이 좋다.
			// closesocket()을 호출해도 WSARecv()가 호출되지 않은 상태이기 때문에
			// 서버측 세션종료 프로세스로 들어가지 못한다. 만약 WSARecv()를 먼저
			// 호출하고 바로 closesocket()을 호출하면 프로세스로의 진입은 가능 할 수도 있으나
			// 비 정상적인 세션을 대상으로 그런 액션을 하는 것은 위험하다.
		}
	}

	if(!pUser->WaitForMessage())
	{
		// ***** IOCP 사용법 중 알아내기 힘든 네번째 구문 (비 정상적인 세션 종료) *****
		//
		// 네트웤 오류로 인해 세션이 비 정상적인 상태에서 WSARecv()함수 호출이 실패한 경우이며
		// WSARecv()가 호출된 상태에서만 이 코드로 들어오며
		// 모든 오버랩 오퍼래이션이 종료 되었다고 볼 수 없다.
		// 따라서 이후에 이 스레드에서 이 세션과 관련된 작업명령이 실행 될 수 있으므로
		// 여기서 세션 포인터를 삭제하면 서버가 다운될 수 있다.
		// Receive와 관련된 오버랩 오퍼레이션은 확실히 종료 되었으므로
		// Send와 관련된 오버랩 오퍼레이션이 종료되었는지를
		// 확인한 후 다른 스레드의 세션 삭제 수락과정을 거치고 세션을 삭제 해야 한다.
		OnInvalidSession(pUser);
	}
}

void CTLoginSvrModule::OnInvalidSession( CTUser *pUser)
{
	if(pUser->OnInvalidSession())
		ClosingSession(pUser);
}

void CTLoginSvrModule::ClearThread()
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

void CTLoginSvrModule::UpdateData()
{
	MAPTUSER::iterator itUSER;

	for( itUSER = m_mapTSESSION.begin(); itUSER != m_mapTSESSION.end(); itUSER++)
	{
		if((*itUSER).second->m_bLogout)
		{
			DEFINE_QUERY( &m_db, CSPLogout)

				query->m_dwID = (*itUSER).second->m_dwID;
				query->m_dwCharID = 0;
				query->m_bLevel = 0;
				query->m_dwExp = 0;
				query->Call();

			UNDEFINE_QUERY()
		}

		delete (*itUSER).second;
	}

	m_mapTSESSION.clear();
	m_mapTUSER.clear();
	m_mapACTIVEUSER.clear();
}

DWORD CTLoginSvrModule::OnReceive( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet, INT64 dwKey)
{
	if(packet.GetSize() == MAX_PACKET_SIZE)
	{

		LogInfo("Overflow Message " + packet.GetID());
		return EC_SESSION_INVALIDMSG;
	}

	switch(packet.GetID())
	{
	if(pUser && pUser->m_bSessionType == SESSION_SERVER)
	{
		// Control Server Message
		ON_RECEIVE(CT_SERVICEMONITOR_ACK)
		ON_RECEIVE(CT_SERVICEDATACLEAR_ACK)
		ON_RECEIVE(CT_CTRLSVR_REQ)
		ON_RECEIVE(CT_EVENTUPDATE_REQ)
		ON_RECEIVE(CT_EVENTMSG_REQ)

		//AI Thread
		ON_RECEIVE(SM_QUITSERVICE_REQ)
	}
	
	ON_RECEIVE(CS_LOGIN_REQ)
	ON_RECEIVE(CS_AGREEMENT_REQ)
	ON_RECEIVE(CS_CHANNELLIST_REQ)
	ON_RECEIVE(CS_GROUPLIST_REQ)
	ON_RECEIVE(CS_CHARLIST_REQ)
	ON_RECEIVE(CS_CREATECHAR_REQ)
	ON_RECEIVE(CS_DELCHAR_REQ)
	ON_RECEIVE(CS_START_REQ)
	ON_RECEIVE(CS_TESTLOGIN_REQ)	// 현승룡 CS_TESTLOGIN_REQ
	ON_RECEIVE(CS_TESTVERSION_REQ)	// 현승룡 CS_TESTVERSION_REQ
	ON_RECEIVE(CS_TERMINATE_REQ)
	ON_RECEIVE(CS_HOTSEND_REQ)
	ON_RECEIVE(CS_VETERAN_REQ)
	}

	return EC_SESSION_INVALIDMSG;
}

DWORD CTLoginSvrModule::GetCurrentUser(BYTE bGroup)
{
	MAPDWORD::iterator it = m_mapCurrentUser.find(bGroup);
	if(it!=m_mapCurrentUser.end())
		return (*it).second;

	return 0;
}

DWORD CTLoginSvrModule::GetCheckFilePoint(CTUser * pUser)
{
	if(m_hExecFile == INVALID_HANDLE_VALUE)
		return 0;

	DWORD dwValue;
	DWORD dwSize;
	dwSize = GetFileSize(m_hExecFile, NULL);
	dwValue = TRand(dwSize);
	pUser->m_qCheckPoint.push(dwValue);
	return dwValue;
}

BYTE CTLoginSvrModule::CheckFile(CTUser * pUser, INT64 dlValue)
{
	if(m_hExecFile == INVALID_HANDLE_VALUE)
		return TRUE;

	if(pUser->m_qCheckPoint.empty())
		return FALSE;

	DWORD dwOffset = pUser->m_qCheckPoint.front();
	pUser->m_qCheckPoint.pop();

	INT64 dlBuff;
	DWORD dwNumberOfBytesRead;
	DWORD dwRet = SetFilePointer(m_hExecFile, dwOffset, NULL, FILE_BEGIN);
	if(dwRet != INVALID_SET_FILE_POINTER)
	{
		BYTE bRet = ReadFile(m_hExecFile, (LPVOID)&dlBuff, sizeof(INT64), &dwNumberOfBytesRead, NULL);
		if(bRet && dwNumberOfBytesRead)
		{
			if(dlValue != dlBuff)
				return FALSE;
		}
	}

	return TRUE;
}

void CTLoginSvrModule::LogExecCheck(DWORD dwUserID, DWORD dwError, CString strAccount)
{
	static CString strFileName = NAME_NULL;
	if(strFileName.IsEmpty())
	{
		TCHAR szFN[ONE_KBYTE];
		GetModuleFileName(NULL, szFN, ONE_KBYTE); 
		char *pSlash = ::strrchr(szFN, '\\');
		if( pSlash )
			::lstrcpy(pSlash + 1, "LoginSvr.log");

		strFileName = szFN;
	}

	FILE * file;
	file = fopen(strFileName, "a+");
	if(!file)
		file = fopen(strFileName, "w+");

	if(!file)
		return;

	CString log;
	CTime time = CTime::GetCurrentTime();

	log.Format("%s	USER_ID=%d(%s)	CODE=%d\n",time.Format("%c"), dwUserID, strAccount, dwError);
	fwrite(log, sizeof(char), lstrlen(log), file);
	fclose(file);
}

BYTE CTLoginSvrModule::CheckCharName(LPCSTR str)
{
	int l = int(strlen(str));
	if(0 > l || l > 16) return FALSE;

	unsigned char* p = (unsigned char *)str;
	unsigned char* e = (unsigned char *)(str+l);

	while(p < e)
	{
		switch(m_bNation)
		{
		case NATION_GERMAN:
			{
				if('0' <= *p && *p <= '9') 
					++p;				
				else if('a' <= *p && *p <= 'z') 
					++p;	
				else if('A' <= *p && *p <= 'Z') 
					++p;
				else if( (*p == 220) ||
						(*p == 214) ||
						(*p == 196) ||
						(*p == 252) ||
						(*p == 246) ||
						(*p == 228) ||
						(*p == 223) )
					++p;
				else 
					return FALSE;
			}
			break;
		case NATION_TAIWAN:
			{
				while(p < e)
				{
					if( (0xA1 <= *p) && (*p <= 0xF9) && (p+1 < e)  )
					{
						if ( (0xA1 <= *p && *p <= 0xC5) || (0xC9 <= *p && *p <= 0xF8) )
						{
							if ( (0x40 <= *(p+1) && *(p+1) <= 0x7E) || (0xA1 <= *(p+1) && *(p+1) <= 0xFE) )
								p += 2;
							else
								return FALSE;
						}
						else if ( *p == 0xC6 )
						{
							if ( 0x40 <= *(p+1) && *(p+1) <= 0x7E )
								p += 2;
							else 
								return FALSE;
						}
						else if ( *p == 0xF9 )
						{
							if ( (0x40 <= *(p+1) && *(p+1) <= 0x7E) || (0xA1 <= *(p+1) && *(p+1) <= 0xDC) )
								p += 2;
							else
								return FALSE;
						}

					}
					else
					{
						if('0' <= *p && *p <= '9') 
							++p;				
						else if('a' <= *p && *p <= 'z') 
							++p;	
						else if('A' <= *p && *p <= 'Z') 
							++p;	
						else 
							return FALSE;
					}
				}
			}
			break;
		case NATION_US:
			{
				if('0' <= *p && *p <= '9') 
					++p;				
				else if('a' <= *p && *p <= 'z') 
					++p;	
				else if('A' <= *p && *p <= 'Z') 
					++p;
				else
					return FALSE;
			}
			break;
		case NATION_JAPAN:
			{
				if( 0xA6 <= *p && *p <= 0xDD )
				{
					p += 1;
					continue;
				}
				else if(p+1 < e && 0x80 <= *p)
				{
					if( *p == 0x81 && *(p+1) == 0x5B )
					{
						p += 2;
					}
					else if( *p == 0x82 )
					{
						p += 2;
					}
					else if( *p == 0x83 )
					{
						if( 0x40 <= *(p+1) && *(p+1) <= 0x96 )
							p += 2;
						else
							return FALSE;
					}
					else if( 0x88 <= *p && *p <= 0xFC )
					{
						p += 2;
					}
					else
						return FALSE;
				}
				else
				{
					if('0' <= *p && *p <= '9') 
						++p;				
					else if('a' <= *p && *p <= 'z') 
						++p;	
					else if('A' <= *p && *p <= 'Z') 
						++p;	
					else 
						return FALSE;
				}
			}
			break;
		case NATION_KOREA:
			{
				if(p+1 < e && 0x80 <= *p)
				{
					if(0xB0 <= *p && *p <= 0xC8 && 0xA1 <= *(p+1) && *(p+1) <= 0xFE)
						p += 2;	
					else
						return FALSE;
				}
				else
				{
					if('0' <= *p && *p <= '9') 
						++p;				
					else if('a' <= *p && *p <= 'z') 
						++p;	
					else if('A' <= *p && *p <= 'Z') 
						++p;	
					else 
						return FALSE;
				} 
			}
			break;
		case NATION_RUSSIA:
			{
				while(p < e)
				{
					if('0' <= *p && *p <= '9') 
						++p;				
					else if('a' <= *p && *p <= 'z') 
						++p;	
					else if('A' <= *p && *p <= 'Z') 
						++p;	
					else if ( 0xC0 <= *p && *p <= 0xFF )
						++p;
					else if ( 0xA8 == *p || 0xB8 == *p )
						++p;
					else 
						return FALSE;
				}
			}
			break;
		default:
			return FALSE;
		}
	}

	return TRUE;
}
