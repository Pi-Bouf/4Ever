#pragma once

class CTQuestNewUI : public CTClientUIBase
{
public:

	CTQuestNewUI( TComponent* pParent, LP_FRAMEDESC pDesc, CTClientChar* pHost);
	virtual ~CTQuestNewUI(void);

public:
	CTClientObjBase *m_pHost;

};