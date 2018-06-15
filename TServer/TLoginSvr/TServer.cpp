#include "StdAfx.h"


CTServer::CTServer()
{
	m_dwID = 0;
	m_pSVRTEMP = NULL;	
	m_dwRecvTick = 0;
	m_dwSendTick = 0;
}

CTServer::~CTServer()
{
}

void CTServer::SendSM_VALIDMAPSESSION_REQ(DWORD dwUserID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(SM_VALIDMAPSESSION_REQ)
		<< dwUserID;

	Say(pMsg);
}