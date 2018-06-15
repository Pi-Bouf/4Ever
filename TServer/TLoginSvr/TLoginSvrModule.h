#pragma once


#define ON_RECEIVE(p)							case p : return On##p( pDB, pGROUP, pUser, packet);

class CTLoginSvrModule
{
protected:
	MAPTUSER m_mapTSESSION;							// all session pool
	MAPTUSER m_mapTUSER;							// valid session pool
	MAPDWORD m_mapACTIVEUSER;						
	MAPDWORD m_mapCurrentUser;
	VVETERAN m_vVETERAN;
	INT64 m_dlCheckFile;
	HANDLE m_hExecFile;

	CRITICAL_SECTION m_csLI;

protected:
	time_t timeNow;
	const char * m_szDBUserID;
	const char * m_szDBPasswd;
	const char * m_szDSN;

	const char * m_bServerID;
	WORD m_wPort;
	BYTE  m_bNation;

	BYTE m_bNumWorker;
	BYTE m_bThreadRun;

	SOCKADDR_IN m_addrCtrl;

	HANDLE m_hWorker[MAX_THREAD];
	HANDLE m_hControl;

	HANDLE m_hIocpControl;
	HANDLE m_hIocpWork;
	SOCKET m_accept;

	CSqlDatabase m_db;
	CSession m_listen;

	COverlappedEx m_ovAccept;
	CPacket m_vAccept;
	
	MAPEVENTINFO m_mapEVENT;

	const char * m_szLogServerIP;
	int m_wLogServerPORT;

	CDebugSocket *m_pDebugSocket;

	HANDLE hConsole;

#ifdef DEF_UDPLOG
	CUdpSocket *m_pUdpSocket;
#endif

protected:

	LPTGROUP FindGroup(LPMAPTGROUP pGROUP, BYTE bGroupID);
	DWORD GetCurrentUser(BYTE bGroup);

	void LogInfo(string text);
	void LogError(string text);

	void OnSendComplete(CTUser *pUser, DWORD dwIoBytes);

	void ProcessSession(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, DWORD dwIoBytes);

	void OnInvalidSession( CTUser *pUser);				// 사용 금지
	void OnCloseSession( CTUser *pUser);				// 사용 금지
	void ClosingSession( CTUser *pUser);				// 사용 금지
	void CloseSession( CTUser *pUser);					// 세션을 종료하려면 이 함수를 호출

	void ClearThread();
	void UpdateData();

	DWORD InitDB(CSqlDatabase *pDB);
	DWORD LoadData(CSqlDatabase *pDB, LPMAPTGROUP pGROUP);
	DWORD LoadDataLogin();

	DWORD CreateThreads();
	BYTE ResumeThreads();

	DWORD LoadConfig();
	DWORD InitNetwork();

	BYTE WaitForConnect();
	BYTE Accept();

	DWORD OnReceive( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet, INT64 dwKey);

	DWORD ControlThread();
	DWORD WorkThread();

	DWORD GetCheckFilePoint(CTUser * pUser);
	BYTE CheckFile(CTUser * pUser, INT64 dlValue);
	void LogExecCheck(DWORD dwUserID, DWORD dwError, CString strAccount = NAME_NULL);

protected:
	virtual void OnERROR( DWORD dwErrorCode);

	DWORD StartUp();
	virtual void OnExit();

private:
	static DWORD WINAPI _ControlThread( LPVOID lpParam);
	static DWORD WINAPI _WorkThread( LPVOID lpParam);

protected:
	// 시스템 메세지 핸들러
	DWORD OnSM_QUITSERVICE_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);

protected:
	// Control Server Message
	DWORD OnCT_SERVICEMONITOR_ACK( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCT_SERVICEDATACLEAR_ACK(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCT_CTRLSVR_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCT_EVENTUPDATE_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCT_EVENTMSG_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);

	// CS message handler
	DWORD OnCS_CHANNELLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_GROUPLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_CHARLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_LOGIN_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	// 현승룡 CS_TESTLOGIN_REQ
	DWORD OnCS_TESTLOGIN_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	// 현승룡 CS_TESTVERSION_REQ
	DWORD OnCS_TESTVERSION_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);

	DWORD OnCS_CREATECHAR_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_DELCHAR_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_START_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_AGREEMENT_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_TERMINATE_REQ( CSqlDatabase * pDB, LPMAPTGROUP pGROUP, CTUser * pUser, CPacket& packet);
	DWORD OnCS_HOTSEND_REQ( CSqlDatabase * pDB, LPMAPTGROUP pGROUP, CTUser * pUser, CPacket& packet);
	DWORD OnCS_VETERAN_REQ( CSqlDatabase * pDB, LPMAPTGROUP pGROUP, CTUser * pUser, CPacket& packet);

public:
	CTLoginSvrModule();
	virtual ~CTLoginSvrModule();
	BYTE CheckCharName(LPCSTR str);
	void ClearLoginUser();

public:
	DECLARE_LIBID(LIBID_TLoginSvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TLOGINSVR, "{A9C0CF53-7D54-43D7-B01C-D604FB9DF809}")

	void StartServer();
	HRESULT PostMessageLoop();
};


extern CTLoginSvrModule _AtlModule;
