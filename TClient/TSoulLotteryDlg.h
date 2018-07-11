#pragma once

class CTSoulLotteryDlg : public CTClientUIBase
{
public:
	TComponent *m_pNumber1;
	TComponent *m_pNumber2;
	TComponent *m_pNumber3;
	TComponent *m_pNumber4;

	TComponent *m_pGold;
	TComponent *m_pSilver;
	TComponent *m_pBronze;

	TButton *m_pLuckyBtn;

	BYTE m_bRenderNumber;
	DWORD m_dwRenderTime;

	BYTE m_bClickStaus;

public:
	void Init(DWORD dwNumber1,
			  DWORD dwNumber2,
			  DWORD dwNumber3,
			  DWORD dwNumber4,
			  DWORD dwMoney);

	void SetMainNumber(DWORD dwNumber);

public:
	virtual HRESULT Render( DWORD dwTickCount );

public :
	CTSoulLotteryDlg( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual ~CTSoulLotteryDlg();
};
