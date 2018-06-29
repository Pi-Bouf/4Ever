#include "stdafx.h"
#include "TClientGame.h"

CTRankingNewMain::CTRankingNewMain( TComponent *pParent, LP_FRAMEDESC pDesc )
:	CTFrameGroupBase( pParent, pDesc, ID_CTRLINST_RANKING_INNERFRAME )
{
	TButton *pClose = (TButton *)FindKid(ID_CTRLINST_BTN_CLOSE);
	pClose->m_menu[TNM_LCLICK] = GM_CLOSE_UI;
}

CTRankingNewMain::~CTRankingNewMain()
{

}

ITDetailInfoPtr	CTRankingNewMain::GetTInfoKey( const CPoint& pt )
{
	ITDetailInfoPtr pInfo;

	if( !IsVisible() )
		return pInfo;

	pInfo = GetInnerFrame(0)->GetTInfoKey( pt );
	return pInfo;
}

void CTRankingNewMain::ShowComponent( BOOL bVisible )
{
	CTRankingNewDlg* pFrame = static_cast< CTRankingNewDlg* >( GetInnerFrame( 0 ) );
	pFrame->ShowComponent(bVisible);

	CTFrameGroupBase::ShowComponent( bVisible );
}