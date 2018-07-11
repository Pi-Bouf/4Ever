#include "StdAfx.h"
#include "TSkygardenEnd.h"
#include "TClientGame.h"

CTSkygardenEnd::CTSkygardenEnd( TComponent *pParent, LP_FRAMEDESC pDesc )
	: CTClientUIBase(pParent, pDesc)
{
	m_pWinList = static_cast<TImageList*>(FindKid(ID_CTRLINST_SKYGARDEN_WIN));
}

CTSkygardenEnd::~CTSkygardenEnd()
{

}

void CTSkygardenEnd::ShowInterface(WORD wCountry)
{
	m_pWinList->SetCurImage(wCountry);

	ShowComponent( TRUE );
	EnableComponent( TRUE );
}

