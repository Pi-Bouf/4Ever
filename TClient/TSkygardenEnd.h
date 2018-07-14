#pragma once

class CTSkygardenEnd : public CTClientUIBase
{
private :

	TImageList*		m_pWinList;


public :

	CTSkygardenEnd( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual ~CTSkygardenEnd();

	void ShowInterface(WORD wCountry);
};
