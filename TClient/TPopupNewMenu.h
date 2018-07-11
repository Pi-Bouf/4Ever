#pragma once


class CTPopupNewMenuDlg : public CTClientUIBase
{
private:
	TImageList *m_pCatImg[TMENUNEW_COUNT];
	TComponent *m_pCatName[TMENUNEW_COUNT];
	TButton *m_pCatBtn[TMENUNEW_COUNT];
	TComponent *m_pFrameName;

	TScroll *m_pScroll;
	
	int m_nCurMode;

public:
	void ClearPopup();
	void ClearButton();
	void SetBlinkMail(BYTE bState);
	void SetBlinkCD(BYTE bState);
	CString GetVKString(enum TKEY_SET eKey);
	int GetCurMode();
	void AddSystem();
	void AddInven();
	void AddChar();
	void AddCommunity();
	void AddWar();
	void AddHelp();

public:
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);

public:
	CTPopupNewMenuDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTPopupNewMenuDlg();
};
