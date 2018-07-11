#pragma once

class CTRaidNewDlg : public CTClientUIBase
{
	
public:
	CTRaidNewDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTRaidNewDlg();

public:
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
};
