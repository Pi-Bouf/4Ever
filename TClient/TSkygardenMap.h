#pragma once

#define	INDEX_DEFEND	(0)
#define	INDEX_ATTACK	(1)

class CTSkygardenMap : public CTClientUIBase
{
private :

	TImageList*		m_pTopCountry;
	TImageList*		m_pLeftCountry;
	TImageList*		m_pCenterCountry;
	TImageList*		m_pRightCountry;
	TImageList*		m_pBottomCountry;

	TComponent*		m_pDefendText;
	TComponent*		m_pAttackText;

	TComponent*		m_pFrame;

public:
	void ChangeLeftCountry(BYTE bCountry);
	void ChangeCenterCountry(BYTE bCountry);
	void ChangeRightCountry(BYTE bCountry);
	void LoadCountries(BYTE bDefendCountry, BYTE bLeftCountry, BYTE bCenterCountry, BYTE bRightCountry, BYTE bAttackCountry);

public :

	CTSkygardenMap( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual ~CTSkygardenMap();

	virtual BOOL	CanWithItemUI();
};