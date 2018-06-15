class CTServer : public CTLoginSession
{
public:
	DWORD m_dwID;
	LPTSVRTEMP m_pSVRTEMP;	
	DWORD m_dwRecvTick;
	DWORD m_dwSendTick;

public:
	CTServer();
	~CTServer();

public:	
	void SendSM_VALIDMAPSESSION_REQ(DWORD dwUserID);
};