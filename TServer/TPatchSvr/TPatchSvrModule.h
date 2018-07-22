#pragma once

#define ON_RECEIVE(p)							case p : return On##p( pDB, pSession);

#include <TServerSystem.h>


class CTPatchSvrModule : public TServerSystem
{
protected:
	MAPTSESSION m_mapTSESSION;							// all session pool
	CRITICAL_SECTION m_csPC;	

protected:
	string m_szDBUserID;
	string m_szDBPasswd;
	string m_szDSN;
	int m_bServerID;
	int m_wPort;

	BYTE m_bNumWorker;
	BYTE m_bThreadRun;

	CString m_strFTP;
	CString m_strPreFTP;
	SOCKADDR_IN m_addrLOGIN;

	HANDLE m_hWorker[MAX_THREAD];
	HANDLE m_hControl;

	HANDLE m_hIocpControl;
	HANDLE m_hIocpWork;
	SOCKET m_accept;

	CSqlDatabase m_db;
	CSession m_listen;

	COverlappedEx m_ovAccept;
	CPacket m_vAccept;

protected:
	void OnSendComplete( CTPatchSession *pSession, DWORD dwIoBytes);
	void ProcessSession(
		CSqlDatabase *pDB,
		CTPatchSession * pSession,
		DWORD dwIoBytes);

	void OnInvalidSession( CTPatchSession * pSession);			// ��� ����
	void OnCloseSession( CTPatchSession * pSession);				// ��� ����
	void ClosingSession( CTPatchSession * pSession);				// ��� ����
	void CloseSession( CTPatchSession * pSession);				// ������ �����Ϸ��� �� �Լ��� ȣ��

	void ClearThread();
	void UpdateData();

	DWORD InitDB( CSqlDatabase *pDB);
	DWORD LoadData(CSqlDatabase *pDB);

	DWORD CreateThreads();
	BYTE ResumeThreads();

	DWORD LoadConfig();
	DWORD InitNetwork();

	BYTE WaitForConnect();
	BYTE Accept();

	DWORD OnReceive(
		CSqlDatabase *pDB,
		CTPatchSession * pSession);

	DWORD ControlThread();
	DWORD WorkThread();

protected:
	virtual void OnERROR( DWORD dwErrorCode);

	virtual DWORD OnEnter();
	virtual void OnExit();

private:
	static DWORD WINAPI _ControlThread( LPVOID lpParam);
	static DWORD WINAPI _WorkThread( LPVOID lpParam);

protected:
	// �ý��� �޼��� �ڵ鷯
	DWORD OnSM_QUITSERVICE_REQ( CSqlDatabase *pDB, CTPatchSession * pSession);

protected:
	// Patch Server Message
	DWORD OnCT_SERVICEMONITOR_ACK( CSqlDatabase *pDB, CTPatchSession * pSession );
	DWORD OnCT_PATCH_REQ(CSqlDatabase * pDB, CTPatchSession * pSession);
	DWORD OnCT_NEWPATCH_REQ(CSqlDatabase * pDB, CTPatchSession * pSession);
	DWORD OnCT_CHANGEIF_REQ(CSqlDatabase * pDB, CTPatchSession * pSession);
	DWORD OnCT_PREPATCH_REQ(CSqlDatabase * pDB, CTPatchSession * pSession);
	DWORD OnCT_PATCHSTART_REQ(CSqlDatabase * pDB, CTPatchSession * pSession);
	DWORD OnCT_SERVICEDATACLEAR_ACK(CSqlDatabase * pDB, CTPatchSession * pSession);
	DWORD OnCT_CTRLSVR_REQ(CSqlDatabase * pDB, CTPatchSession * pSession);
	DWORD OnCT_PREPATCHCOMPLETE_REQ(CSqlDatabase * pDB, CTPatchSession * pSession);

public:
	CTPatchSvrModule();
	virtual ~CTPatchSvrModule();

public:
	DECLARE_LIBID(LIBID_TPatchSvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TPATCHSVR, "{A9C0CF53-7D54-43D7-B01C-D604FB9DF809}")
};

extern CTPatchSvrModule _AtlModule;
