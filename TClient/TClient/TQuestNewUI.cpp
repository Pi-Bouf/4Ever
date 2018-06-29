#include "stdafx.h"
#include "Resource.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "TQuestNewUI.h"

CTQuestNewUI::CTQuestNewUI( TComponent* pParent, LP_FRAMEDESC pDesc, CTClientChar* pHost)
: CTClientUIBase( pParent, pDesc), m_pHost(pHost)
{

}

CTQuestNewUI::~CTQuestNewUI(void)
{

}
