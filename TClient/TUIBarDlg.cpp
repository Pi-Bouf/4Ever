#include "StdAfx.h"
#include "TUIBarDlg.h"
#include "TClientGame.h"
#include "Resource.h"
#define ID_BUTTON_EX (0x000065D7)
#define ID_BUTTON_N1 (0x000065CF)
#define ID_BUTTON_N2 (0x000065D0)
#define ID_BUTTON_N3 (0x000065D1)
#define ID_BUTTON_N4 (0x000065D2)
#define ID_BUTTON_N5 (0x000065D3)
#define ID_BUTTON_N6 (0x000065D4)
#define ID_BUTTON_N7 (0x000065D5)
#define ID_BUTTON_N8 (0x000065D6)
#define ID_BUTTON_N9 (0x0000680F)
#define ID_IMAGE_USELESS (0x0000680E)


CTUIBarDlg::CTUIBarDlg( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser )
	: CTClientUIBase(pParent, pDesc)
{

	static DWORD dwHiden[8] =
	{
    ID_BUTTON_N1,
	ID_BUTTON_N2,
	ID_BUTTON_N3,
	ID_BUTTON_N4,
	ID_BUTTON_N5,
	ID_BUTTON_N6,
	ID_BUTTON_N7,
	ID_BUTTON_N8
	//ID_BUTTON_N9
	};

	for(int i =0;i<8;++i) // i++ = 10% swag, ++i = 150% swag
	m_pButIdx[i] = static_cast<TButton*>(FindKid(dwHiden[i]));

	m_pButEx = static_cast<TButton*>(FindKid(ID_BUTTON_EX));
	m_pBut1 = static_cast<TButton*>(FindKid(ID_BUTTON_N1));
	m_pBut2 = static_cast<TButton*>(FindKid(ID_BUTTON_N2));
	m_pBut3 = static_cast<TButton*>(FindKid(ID_BUTTON_N3));
	m_pBut4 = static_cast<TButton*>(FindKid(ID_BUTTON_N4));
	m_pBut5 = static_cast<TButton*>(FindKid(ID_BUTTON_N5));
	m_pBut6 = static_cast<TButton*>(FindKid(ID_BUTTON_N6));
	m_pBut7 = static_cast<TButton*>(FindKid(ID_BUTTON_N7));
	m_pBut8 = static_cast<TButton*>(FindKid(ID_BUTTON_N8));

} // I have a question ! This source is from you ?

CTUIBarDlg::~CTUIBarDlg()
{

}
HRESULT CTUIBarDlg::Render(DWORD dwTickCount)
{
	if(bShownAll && bShownFade)
	ShowComponent(TRUE);
	else
    ShowComponent(FALSE);

	TComponent* m_pUseless = FindKid(ID_IMAGE_USELESS);
	m_pUseless->ShowComponent(FALSE);
	if(!bShownBar)
	{
		for(int i=0;i<8;i++)
			m_pButIdx[i]->ShowComponent(FALSE);
	}
	else
	{
		for(int i=0;i<8;i++)
			m_pButIdx[i]->ShowComponent(TRUE);
	}

	CPoint pt = CTClientUIBase::m_vBasis[TBASISPOINT_CENTER_TOP];
#ifdef NEW_IF
	pt.x -= 135;
	pt.y += 57;
#else
	pt.x -= 135;
	pt.y += 32;
#endif
	MoveComponent(pt);
	return CTClientUIBase::Render(dwTickCount);
}

ITDetailInfoPtr CTUIBarDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;
	
	//Støed button
	if(m_pButEx && m_pButEx->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pButEx->GetComponentRect(&rt);
		m_pButEx->ComponentToScreen(&rt);
		strTitle.Format("Quickbar +\\-"); 
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc, 
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	//Button 1
	if(m_pBut1 && m_pBut1->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pBut1->GetComponentRect(&rt);
		m_pBut1->ComponentToScreen(&rt);
		strTitle.Format("Reinforce Equipment"); 
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc, 
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	//Button 2
	if(m_pBut2 && m_pBut2->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pBut2->GetComponentRect(&rt);
		m_pBut2->ComponentToScreen(&rt);
		strTitle.Format("Refinement"); 
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc, 
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	//Button 3
	if(m_pBut3 && m_pBut3->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pBut3->GetComponentRect(&rt);
		m_pBut3->ComponentToScreen(&rt);
		strTitle.Format("Repair"); 
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc, 
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	//Button 4
	if(m_pBut4 && m_pBut4->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pBut4->GetComponentRect(&rt);
		m_pBut4->ComponentToScreen(&rt);
		strTitle.Format("Deposit"); 
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc,
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	//Button 5
	if(m_pBut5 && m_pBut5->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pBut5->GetComponentRect(&rt);
		m_pBut5->ComponentToScreen(&rt);
		strTitle.Format("Item Auction"); 
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc, 
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	//Button 6
	if(m_pBut6 && m_pBut6->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pBut6->GetComponentRect(&rt);
		m_pBut6->ComponentToScreen(&rt);
		strTitle.Format("Consumer Goods Trades"); 
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc, 
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	//Button 7
	if(m_pBut7 && m_pBut7->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pBut7->GetComponentRect(&rt);
		m_pBut7->ComponentToScreen(&rt);
		strTitle.Format("Mystic");
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc, 
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	//Button 8
	if(m_pBut8 && m_pBut8->HitTest(point))
	{
		CRect rt;
		CString strTitle;
		CString strTitleDesc;
		m_pBut8->GetComponentRect(&rt);
		m_pBut8->ComponentToScreen(&rt);
		strTitle.Format("Battle"); 
		strTitleDesc.Format(""); 
		pInfo = CTDetailInfoManager::NewDefTooltipInst(
			strTitle, 
			strTitleDesc, 
			rt);

		pInfo->SetDir(FALSE, FALSE, FALSE);

		return pInfo;

	}

	return pInfo;
}

BOOL CTUIBarDlg::CanWithItemUI()
{
	return TRUE;
}