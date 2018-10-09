#pragma once

class CTUIBarDlg : public CTClientUIBase
{
private :




public :
	TButton* m_pBut1;
	TButton* m_pBut2;
	TButton* m_pBut3;
	TButton* m_pBut4;
	TButton* m_pBut5;
	TButton* m_pBut6;
	TButton* m_pBut7;
	TButton* m_pBut8;
	TButton* m_pBut9;
	TButton* m_pButEx;
	TButton* m_pButIdx[9];
	BYTE bShownBar;
	BYTE bShownAll;
	BOOL CanWithItemUI();
	BYTE bShownFade;
	CTUIBarDlg( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser );
	virtual ~CTUIBarDlg();
	HRESULT Render(DWORD dwTickCount);
	BOOL m_bShowScene;
	CTClientUIBase *m_pUIBar;
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
};
