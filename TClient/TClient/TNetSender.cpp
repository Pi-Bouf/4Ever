#include "stdafx.h"
#include "TClient.h"
#include "TClientWnd.h"

void CTClientWnd::SendCS_LOGIN_REQ( CString fixedUserID, CString fixedPassword, DWORD dwSiteCode)
{
	CPacket vMSG;

	WORD wVersion = TVERSION;
	CString Zombie3 = 0;
	CString Zombie2 = 0;
	CString Zombie1 = 0;

	INT64 llChecksum;
	INT64 key = 0x336c3aebf71a8b08;

	llChecksum = wVersion * 2 - 500;

	INT64 dwIndex = llChecksum % sizeof(INT64);
	INT64 dwBody = llChecksum / sizeof(INT64);

	for(DWORD i = 0; i < dwIndex; i++)
	{
		llChecksum ^= dwBody;
		llChecksum += key;
	}

	vMSG.SetID(CS_LOGIN_REQ)
		<< wVersion
		<< Zombie3
		<< fixedPassword
		<< Zombie1
		<< Zombie2
		<< fixedUserID
		<< theApp.m_dlCheckFile
		<< llChecksum;
		//<< strProccessorID;

	if( CTNationOption::MODIFY_DIRECTLOGIN )
		vMSG << dwSiteCode;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_GROUPLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GROUPLIST_REQ);
	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_CHARLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_CHARLIST_REQ)
		<< m_pTGROUP->m_bGroupID;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_CREATECHAR_REQ( BYTE bGroupID,
										 CString strNAME,
										 BYTE bSlotID,
										 BYTE bClass,
										 BYTE bRace,
										 BYTE bContry,
										 BYTE bSex,
										 BYTE bHair,
										 BYTE bFace,
										 BYTE bBody,
										 BYTE bPants,
										 BYTE bHand,
										 BYTE bFoot,
										 BYTE bLevelOption)
{
	CPacket vMSG;

	vMSG.SetID(CS_CREATECHAR_REQ)
		<< bGroupID
		<< strNAME
		<< bSlotID
		<< bClass
		<< bRace
		<< bContry
		<< bSex
		<< bHair
		<< bFace
		<< bBody
		<< bPants
		<< bHand
		<< bFoot
		<< bLevelOption;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_DELCHAR_REQ( BYTE bGroupID,
									  CString strPasswd,
									  DWORD dwCharID)
{
	CPacket vMSG;

	vMSG.SetID(CS_DELCHAR_REQ)
		<< bGroupID
		<< strPasswd
		<< dwCharID;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_START_REQ( BYTE bGroupID,
								    BYTE bChannel,
									DWORD dwCharID)
{
	CPacket vMSG;

	vMSG.SetID(CS_START_REQ)
		<< bGroupID
		<< bChannel
		<< dwCharID;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_CHANNELLIST_REQ(BYTE bGroupID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHANNELLIST_REQ)
		<< bGroupID;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_AGREEMENT_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_AGREEMENT_REQ);

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_HOTSEND_REQ(INT64 dlValue, BYTE bAll)
{
	CPacket vMSG;

	vMSG.SetID(CS_HOTSEND_REQ)
		<< dlValue
		<< bAll;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_VETERAN_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_VETERAN_REQ);

	m_pTNet->Say(&vMSG);
}





