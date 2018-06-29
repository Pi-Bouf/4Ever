#pragma once


class CTMainUI : public CTClientUIBase
{
public:
	static LPTEXTURESET m_pNOTIFYMAIL;

public:
#ifdef NEW_IF
	TComponent *m_pTMAINBTN_NEW[TMAINBUTTON_COUNT];
#else
	TComponent *m_pTMAINBTN[TMAINBUTTON_COUNT];
	TComponent *m_pTMSGBTN[TMSGBUTTON_COUNT];
	TComponent *m_pTTCASHMENU[TCASHMENU_COUNT];
	TComponent *m_pTPARTYITEM;
#endif
	TComponent *m_pTCHAPTERMSG;
	TComponent *m_pTREGIONMSG;
	TComponent *m_pTQUESTMSG;
	TButton *m_pLotBtn;
	TComponent *m_pLotBtnBlink;
#ifdef NEW_IF
	TComponent *m_pLotBlink;
#else
	TButton    *m_pMESSENGER;
	TButton    *m_pMAIL;
	TComponent *m_pSoulForm;
#endif
	BOOL m_bNotifyNewMsg;
	BOOL m_bNotifyNewMail;
	BOOL m_bNotifyNewCashCabinet;
	BYTE m_bInvenSlot;
	BYTE m_bMainMenu;
	BYTE m_bCashMenu;
	CTClientUIBase* m_pCENTER;
	CTClientUIBase* m_pRIGHT;
	DWORD m_dwNotifyMailTick;
	D3DXVECTOR2 m_vNotifyMailStart;
	D3DXVECTOR2 m_vNotifyMailEnd;
#ifdef NEW_IF
#else
	TButton* m_pCashShopButton;
	TButton* m_pCabinetButton;
#endif

public:
	static BYTE MakeSlotID(TMAINSLOTID_TYPE eSlotType, BYTE bSlotNumber);
	static TMAINSLOTID_TYPE ToSlotType(BYTE bSlotID); 
	static BYTE ToSlotNumber(BYTE bSlotID);

public:
	void ResetCashMenu();
	void ResetChapterMSG( CString& strTEXT);
	void ResetRegionMSG( CString& strTEXT);
	void ResetQuestMSG( CString& strTITLE, CString& strRESULT);
	void CheckMSGCTRL();
	void SetNewMessengerMsg(BOOL bNew)	{ m_bNotifyNewMsg = bNew; }
	void SetNewMail(BOOL bNew)			{ m_bNotifyNewMail = bNew; if( bNew ) m_dwNotifyMailTick = NOTIFYMAIL_TICK; }
	void SetNewCashCabinet(BOOL bNew) { m_bNotifyNewCashCabinet = bNew; }
	void AdjustOtherCompByTREECALLSLOT( BOOL bShow );

public:
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual BOOL CanWithItemUI();
	virtual void ResetPosition();
	virtual void DefaultPosition( CPoint* vBASIS, BOOL bRestore );
	virtual void SwitchFocus(TComponent *pCandidate);
	virtual HRESULT Render( DWORD dwTickCount );

protected:
	ITDetailInfoPtr GetTInfoKey_MainBtn(const CPoint& point, INT nID);
	ITDetailInfoPtr GetTInfoKey_MsgBtn(const CPoint& point, INT nID);

public:
	CTMainUI( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser, CTClientChar *pHost);
	virtual ~CTMainUI();
};
