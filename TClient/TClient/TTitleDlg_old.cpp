#include "stdafx.h"
#include "TTitleDlg.h"
#include "TClientGame.h"

CTTitleSlot::CTTitleSlot()
{
	m_pCheckBtn = NULL;
}

CTTitleSlot::~CTTitleSlot()
{

}

void CTTitleSlot::Select( BOOL bSel )
{
	static_cast< TButton* >( m_pBase )->Select( bSel );
}

void CTTitleSlot::ShowComponent( BOOL bShow )
{
	m_bShow = bShow;

	if( m_bShow == FALSE )
	{
		m_pCheckBtn->ShowComponent( FALSE );
		m_pCheckBtn->ShowComponent( FALSE );
	}
}

BOOL CTTitleSlot::OnLButtonDown( CPoint pt )
{
	if( m_pBase &&
		m_pItem &&
		m_pBase->HitTest( pt ) )
	{
		if( m_pCheckBtn &&
			m_pCheckBtn->HitTest( pt ) )
		{
			BOOL bCheck = m_pCheckBtn->IsStateDown();

			return TRUE;
		}
	}

	return FALSE;
}

//END SLOT

void CTTitleItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	CTTitleSlot* pTitleSlot = static_cast<CTTitleSlot*>(pSlot);

	pTitleSlot->m_pCheckBtn->ShowComponent( TRUE );
}
// ----------------------------------------------------------------------------------------------------
CTTitleItem::~CTTitleItem()
{
}

//END TITLEITEM

TButton*	CTTitleDlg::s_pCheck = NULL;
CPoint		CTTitleDlg::s_ptCheck_Offset = CPoint( 0, 0 );

CTTitleDlg::CTTitleDlg( TComponent *pParent, LP_FRAMEDESC pDesc )
:	CTClientUIBase( pParent, pDesc )
{
	m_pList = static_cast<TList*>( FindKid(ID_CTRLINST_TITLE_LIST9) );
	m_pList2 = static_cast<TList*>( FindKid(ID_CTRLINST_TITLE_LIST6) );
	m_pList3 = static_cast<TList*>( FindKid(ID_CTRLINST_TITLE_LIST14) );
	m_pBtn = static_cast<TButton *>(FindKid(ID_CTRLINST_TITLE_LIST13));
	m_pDescription = FindKid(ID_CTRLINST_TITLE_LIST10);

	static DWORD dwSlot[ 9 ] =
	{
		ID_CTRLINST_SLOT_TITLE1,
		ID_CTRLINST_SLOT_TITLE2,
		ID_CTRLINST_SLOT_TITLE3,
		ID_CTRLINST_SLOT_TITLE4,
		ID_CTRLINST_SLOT_TITLE5,
		ID_CTRLINST_SLOT_TITLE6,
		ID_CTRLINST_SLOT_TITLE7,
		ID_CTRLINST_SLOT_TITLE8,
		ID_CTRLINST_SLOT_TITLE9
	};
	
	CTTitleSlot* pSlot;
	CTCLSlotArray vSlots;

	for( int i=0; i<9; ++i )
	{
		pSlot = new CTTitleSlot;
		pSlot->m_pBase = FindKid( dwSlot[i] );
		vSlots.push_back( pSlot );
	}

	CTTitleSlot* pSlot0 = static_cast< CTTitleSlot* >( vSlots[0] );

	CPoint ptBase;
	pSlot0->m_pBase->GetComponentPos( &ptBase );
	CPoint ptTemp = CPoint( 0, 0 );

	CTTitleDlg::s_pCheck = static_cast< TButton* >( FindKid( ID_CTRLINST_CHECKBOX ) );
	CTTitleDlg::s_pCheck->GetComponentPos( &ptTemp );
	CTTitleDlg::s_ptCheck_Offset = ptTemp - ptBase;
	RemoveKid( CTTitleDlg::s_pCheck );

	for( int i=0; i<9; ++i )
	{
		pSlot = static_cast< CTTitleSlot* >( vSlots[i] );
		pSlot->m_pCheckBtn = new TButton( this, CTTitleDlg::s_pCheck->m_pDESC );

		pSlot->m_pCheckBtn->m_id = GetUniqueID();

		AddKid( pSlot->m_pCheckBtn );

		pSlot->m_pBase->GetComponentPos( &ptBase );
		pSlot->m_pCheckBtn->MoveComponent( ptBase + CTTitleDlg::s_ptCheck_Offset );
	}
	RemoveKid( m_pList->m_pScroll );
	m_pList_c = new CTCtrlList;
	m_pList_c->SetSlot( &vSlots );
	m_pList_c->SetupScroll(static_cast<TScroll*>(m_pList->m_pScroll), 1);

	m_pList_c->m_pScroll->m_id = m_pList->m_pScroll->m_id;

	delete m_pList->m_pScroll;

	AddKid(m_pList_c->m_pScroll);
}

CTTitleDlg::~CTTitleDlg()
{	
	m_pList->ClearText();
	delete m_pList;

	m_pList_c->ClearItem();
	delete m_pList_c;

	delete CTTitleDlg::s_pCheck;
	CTTitleDlg::s_pCheck = NULL;
}

BOOL CTTitleDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	//CTClientUIBase::DoMouseWheel(nFlags,zDelta,pt);
	m_pList_c->DoMouseWheel(nFlags,zDelta,pt);
	m_pList_c->NotifyUpdate();

	return TRUE;
}

void CTTitleDlg::OnLButtonDown( UINT nFlags, CPoint pt )
{		
	if(!m_pList3->HitTest(pt) && !m_pBtn->HitTest(pt))
		m_pList3->ShowComponent(FALSE);

	m_pBtn->SetPushButton();

	if( m_pBtn->HitTest(pt) )
		{
			if(m_pList3->m_bVisible == FALSE)
				m_pList3->ShowComponent(TRUE);
			else
				m_pList3->ShowComponent(FALSE);
		}

	if(m_pList3->HitTest(pt))
	{
		TListItem* pHITITEM = m_pList3->GetHitItem( pt );

		if( pHITITEM && pHITITEM != NULL)
		{
			MAPTTITLEGROUPTEMP::iterator itTTITLEGROUP;
			MAPTTITLETEMP::iterator itTTITLE;

			m_pList->RemoveAll();

			DWORD dwID = m_pList3->GetHitItemData(pt);

			for( itTTITLE = CTChart::m_mapTTITLETEMP.begin(); itTTITLE != CTChart::m_mapTTITLETEMP.end(); itTTITLE++)
			{
				if((*itTTITLE).second->m_bKind == dwID || dwID == NULL)
				{
					AddStringL1((*itTTITLE).second->m_strRequirement, (*itTTITLE).second->m_strTitle, (*itTTITLE).second->m_wTitleID);
							
					LPTTITLEGROUP pTitleGroup = CTChart::FindTTITLEGROUPTEMP(dwID);
					m_pBtn->m_strText = pTitleGroup->m_strCaption;
				}
			}
			m_pList3->ShowComponent(FALSE);
		}

	}

	if(m_pList->HitTest(pt))
	{
		TListItem* pHITITEM = m_pList->GetHitItem( pt );

		if( pHITITEM && pHITITEM != NULL)
		{
			DWORD dwID = m_pList->GetHitItemData(pt);
			LPTTITLE pTitle = CTChart::FindTTITLETEMP(dwID);

			if(pTitle)
				m_pDescription->m_strText = pTitle->m_strDescription;
		}

	}
	CTClientUIBase::OnLButtonDown( nFlags, pt);
}

void CTTitleDlg::SwitchFocus(TComponent *pCandidate)
{
	if( m_pFocus == pCandidate )
		return;

	if( m_pFocus )
		m_pFocus->SetFocus(FALSE);

	if( pCandidate )
		pCandidate->SetFocus(TRUE);

	m_pFocus = pCandidate;
}

HRESULT CTTitleDlg::Render( DWORD dwTickCount )
{
	//m_pList_c->SetupScroll(static_cast<TScroll*>(m_pList->m_pScroll), 1);
	m_pList_c->Update();

	return CTClientUIBase::Render( dwTickCount );;
}

void CTTitleDlg::AddLists()
{
	/*ClearLists();
	m_pDescription->m_strText.Empty();
	m_pList3->ShowComponent(FALSE);

	MAPTTITLETEMP::iterator itTTITLE;
	MAPTTITLEGROUPTEMP::iterator itTTITLEGROUP;

	for( itTTITLE = CTChart::m_mapTTITLETEMP.begin(); itTTITLE != CTChart::m_mapTTITLETEMP.end(); itTTITLE++)
	{
		int nLine = m_pList->AddString((*itTTITLE).second->m_strTitle, 1);

		m_pList->SetItemString(nLine, 2, (*itTTITLE).second->m_strRequirement);
		m_pList->SetItemData(nLine, 1, (*itTTITLE).second->m_wTitleID);
		m_pList->SetItemData(nLine, 2, (*itTTITLE).second->m_wTitleID);

	}

	for( itTTITLEGROUP = CTChart::m_mapTTITLEGROUPTEMP.begin(); itTTITLEGROUP != CTChart::m_mapTTITLEGROUPTEMP.end(); itTTITLEGROUP++)
	{
		if((*itTTITLEGROUP).second->m_bID == 0)
			m_pBtn->m_strText = (*itTTITLEGROUP).second->m_strCaption;

		int nLine = m_pList3->AddString((*itTTITLEGROUP).second->m_strCaption);
		m_pList3->SetItemData(nLine, 0, (*itTTITLEGROUP).second->m_bID);
	}*/

	for(int i = 0; i < 20; i++)
	{
		CTTitleItem* pItem = new CTTitleItem();
		m_pList_c->AddItem( pItem );
	}

//	m_bNeedResetPos = TRUE;
}

void CTTitleDlg::ClearLists()
{
	m_pList->RemoveAll();
	m_pList2->RemoveAll();
	m_pList3->RemoveAll();
}

void CTTitleDlg::AddStringL1(CString strRequirement, CString strTitle, WORD wTitleID)
{
	int nLine = m_pList->AddString(strTitle, 1);

	m_pList->SetItemString(nLine, 2, strRequirement);
	m_pList->SetItemData(nLine, 1, wTitleID);
	m_pList->SetItemData(nLine, 2, wTitleID);
}

void CTTitleDlg::ResetPos()
{
	MAPTTITLETEMP::iterator itTTITLE;

	/*for( itTTITLE = CTChart::m_mapTTITLETEMP.begin(); itTTITLE != CTChart::m_mapTTITLETEMP.end(); itTTITLE++)
		m_pCheck->MoveComponent( m_ptBasePointCategory + m_ptCheckBoxOffset );*/

//	m_bNeedResetPos = FALSE;
}
