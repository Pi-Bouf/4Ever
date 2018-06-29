#include "StdAfx.h"
#include "TSoulLotteryDlg.h"
#include "TClientGame.h"

CTSoulLotteryDlg::CTSoulLotteryDlg( TComponent *pParent, LP_FRAMEDESC pDesc )
	: CTClientUIBase(pParent, pDesc)
{
	m_pNumber1 = FindKid(ID_CTRLINST_SOULOT_NUM1);
	m_pNumber2 = FindKid(ID_CTRLINST_SOULOT_NUM2);
	m_pNumber3 = FindKid(ID_CTRLINST_SOULOT_NUM3);
	m_pNumber4 = FindKid(ID_CTRLINST_SOULOT_NUM4);
	m_pLuckyBtn = (TButton*) FindKid(ID_CTRLINST_SOULOT_BTN);
	m_pGold = FindKid(ID_CTRLINST_RUNE);
	m_pSilver = FindKid(ID_CTRLINST_LUNA);
	m_pBronze = FindKid(ID_CTRLINST_CRON);

	m_bRenderNumber = FALSE;
	m_dwRenderTime = NULL;
	m_bClickStaus = CLICK_LOTTERY_NONE;
}

CTSoulLotteryDlg::~CTSoulLotteryDlg()
{

}

void CTSoulLotteryDlg::Init(DWORD dwNumber1,
							DWORD dwNumber2,
							DWORD dwNumber3,
							DWORD dwNumber4,
							DWORD dwMoney)
{
	m_pNumber1->m_strText = CTChart::Format( TSTR_SOULLOT_BTN_PLACEHOLDER, dwNumber1);
	m_pNumber2->m_strText = CTChart::Format( TSTR_SOULLOT_BTN_PLACEHOLDER, dwNumber2);
	m_pNumber3->m_strText = CTChart::Format( TSTR_SOULLOT_BTN_PLACEHOLDER, dwNumber3);
	m_pNumber4->m_strText = CTChart::Format( TSTR_SOULLOT_BTN_PLACEHOLDER, dwNumber4);
	m_pLuckyBtn->m_strText = CTChart::LoadString(TSTR_SOULLOT_BTN_FILLER);

	m_pGold->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwMoney / TRUNE_DIVIDER);
	m_pSilver->m_strText = CTChart::Format( TSTR_FMT_NUMBER, (dwMoney % TRUNE_DIVIDER) / TLUNA_DIVIDER);
	m_pBronze->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwMoney % TLUNA_DIVIDER);

	m_bRenderNumber = FALSE;
	m_dwRenderTime = 1500;
	m_bClickStaus = CLICK_LOTTERY_NONE;
}

void CTSoulLotteryDlg::SetMainNumber(DWORD dwNumber)
{
	m_pLuckyBtn->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwNumber);
}

HRESULT CTSoulLotteryDlg::Render( DWORD dwTickCount )
{	
	if(m_bRenderNumber)
	{
		m_dwRenderTime -= min(m_dwRenderTime, dwTickCount);

		if(m_dwRenderTime != 0)
		{
			DWORD dwRand = rand() % 30 + 1;
			m_pLuckyBtn->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwRand);
		}
		else
		{
			if(m_bClickStaus == CLICK_LOTTERY_START)
				m_bClickStaus = CLICK_LOTTERY_END;

			DWORD dwRand = rand() % 30 + 1;
			m_pLuckyBtn->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwRand);
		}
	}

	if(m_bClickStaus == CLICK_LOTTERY_END)
	{
		CTClientSession *pSession = CTClientGame::GetInstance()->GetSession();

		if(pSession)
			pSession->SendCS_SOULLOTTERY_TRIGGER_REQ();

		m_bClickStaus = CLICK_LOTTERY_NONE;
	}

	return CTClientUIBase::Render(dwTickCount);
}