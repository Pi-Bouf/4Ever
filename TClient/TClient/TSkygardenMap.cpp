#include "StdAfx.h"
#include "TSkygardenMap.h"
#include "TClientGame.h"

CTSkygardenMap::CTSkygardenMap( TComponent *pParent, LP_FRAMEDESC pDesc )
	: CTClientUIBase(pParent, pDesc)
{
	m_pTopCountry = static_cast<TImageList*>(FindKid(ID_CTRLINST_TOP));
	m_pLeftCountry = static_cast<TImageList*>(FindKid(ID_CTRLINST_LEFT));
	m_pCenterCountry = static_cast<TImageList*>(FindKid(ID_CTRLINST_CENTER));
	m_pRightCountry = static_cast<TImageList*>(FindKid(ID_CTRLINST_RIGHT));
	m_pBottomCountry = static_cast<TImageList*>(FindKid(ID_CTRLINST_BOTTOM));

	m_pDefendText = FindKid(ID_CTRLINST_SKYGARDEN_DEFEND);
	m_pAttackText = FindKid(ID_CTRLINST_SKYGARDEN_ATTACK);
	m_pFrame = FindKid(ID_CTRLINST_FRAME);
	RemoveKid(m_pFrame);

	ShowComponent( FALSE );
	EnableComponent( FALSE );
}

CTSkygardenMap::~CTSkygardenMap()
{

}

BOOL CTSkygardenMap::CanWithItemUI()
{
	return TRUE;
}

void CTSkygardenMap::ChangeLeftCountry(BYTE bCountry)
{
	int nIndex = INDEX_DEFEND;

	if(bCountry == CAMP_ATTACK)
		nIndex = INDEX_ATTACK;

	m_pLeftCountry->SetCurImage(nIndex);
}

void CTSkygardenMap::ChangeCenterCountry(BYTE bCountry)
{
	int nIndex = INDEX_DEFEND;

	if(bCountry == CAMP_ATTACK)
		nIndex = INDEX_ATTACK;

	m_pCenterCountry->SetCurImage(nIndex);
}

void CTSkygardenMap::ChangeRightCountry(BYTE bCountry)
{
	int nIndex = INDEX_DEFEND;

	if(bCountry == CAMP_ATTACK)
		nIndex = INDEX_ATTACK;

	m_pRightCountry->SetCurImage(nIndex);
}

void CTSkygardenMap::LoadCountries(BYTE bDefendCountry, BYTE bLeftCountry, BYTE bCenterCountry, BYTE bRightCountry, BYTE bAttackCountry)
{
	m_pTopCountry->SetCurImage(0);

	int nLeftIndex = INDEX_DEFEND;

	if(bLeftCountry == CAMP_ATTACK)
		nLeftIndex = INDEX_ATTACK;

	m_pLeftCountry->SetCurImage(nLeftIndex);

	int nCenterIndex = INDEX_DEFEND;

	if(bCenterCountry == CAMP_ATTACK)
		nCenterIndex = INDEX_ATTACK;

	m_pCenterCountry->SetCurImage(nCenterIndex);

	int nRightIndex = INDEX_DEFEND;

	if(bRightCountry == CAMP_ATTACK)
		nRightIndex = INDEX_ATTACK;

	m_pRightCountry->SetCurImage(nRightIndex);

	m_pBottomCountry->SetCurImage(1);

	m_pDefendText->m_strText = CTChart::LoadString( (TSTRING)CTClientGame::m_vTCOUNTRYSTR[ bDefendCountry ] );
	m_pAttackText->m_strText = CTChart::LoadString( (TSTRING)CTClientGame::m_vTCOUNTRYSTR[ bAttackCountry ] );
}


