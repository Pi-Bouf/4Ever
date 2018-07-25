#include "Stdafx.h"
#include "TGameSettingFrame.h"
#include "TClientGame.h"



LPBYTE CTGameSettingFrame::TOPTIONPTR[OPTION_COUNT] =
{
	&CTClientGame::m_vTOPTION.m_bNpcNAME,
	&CTClientGame::m_vTOPTION.m_bPcNAME,
	&CTClientGame::m_vTOPTION.m_bMonNAME,
	&CTClientGame::m_vTOPTION.m_bAutoHelp,
	&CTClientGame::m_vTOPTION.m_bTalkBox,
	&CTClientGame::m_vTOPTION.m_bDenyWhisper,
	&CTClientGame::m_vTOPTION.m_bDenyCommunity,
	&CTClientGame::m_vTOPTION.m_bHUD,
	&CTClientGame::m_vTOPTION.m_bContinualChat,
	&CTClientGame::m_vTOPTION.m_bMouseClickMove,
	&CTClientGame::m_vTOPTION.m_bAutoTargeting,
	&CTClientGame::m_vTOPTION.m_bUIBar
};

DWORD CTGameSettingFrame::TNAME[OPTION_COUNT] = 
{
	TSTR_GAMEOPTION_SHOW_NPC_NAME, //NPC �̸� ǥ��
	TSTR_GAMEOPTION_SHOW_PLAYER_NAME, //ĳ���� �̸� ǥ��
	TSTR_GAMEOPTION_SHOW_MONSTER_NAME, //���� �̸� ǥ��
	TSTR_GAMEOPTION_AUTO_HELP, //�ڵ� ����
	TSTR_GAMEOPTION_SHOW_TALK_BOX, //��ǳ�� ���̱�
	TSTR_GAMEOPTION_DENY_WHISPER, //�Ӹ� �ź�
	TSTR_GAMEOPTION_DENY_COMMUNITY, //Ŀ�´�Ƽ �ź�
	TSTR_GAMEOPTION_SHOW_HUD, //HUD ���̱�
	TSTR_GAMEOPTION_CONTINUOUS_CHAT, //���� ä��
	TSTR_GAMEOPTION_MOUSE_CLICK_MOVE, //���콺 Ŭ�� �̵�
	TSTR_GAMEOPTION_AUTO_TARGETING, //�ڵ� Ÿ���� ���
	TSTR_GAMEOPTION_UIBAR
};

// ===============================================================================
CTGameSettingFrame::CTGameSettingFrame(TComponent *pParent, LP_FRAMEDESC pDesc)
	: ITInnerFrame(pParent, pDesc, TSETTING_FRAME_GAME)
{
	m_pTLIST = (TList*) FindKid( ID_CTRLINST_LIST );

	for( int i=0 ; i < OPTION_COUNT ; ++i )
	{
		int nLine = m_pTLIST->AddString( CTChart::LoadString( (TSTRING) CTGameSettingFrame::TNAME[i] ) );
		m_pTLIST->SetItemData( nLine, 0, i );
	}

	CPoint ptBasisPos;
	m_pTLIST->GetComponentPos(&ptBasisPos);
    
	TButton* pTCHECK = (TButton*) FindKid( ID_CTRLINST_CHECK );
	RemoveKid(pTCHECK);

	CPoint ptOffset;
	pTCHECK->GetComponentPos(&ptOffset);
	ptOffset = ptOffset - ptBasisPos;
	int n = m_pTLIST->GetMaxItemHeight();
	int nTotal = m_pTLIST->GetItemPerPage();
	m_pTCHECK = new TButton*[nTotal];

	for( int i=0 ; i < nTotal ; ++i )
	{
		m_pTCHECK[ i ] = new TButton( this, pTCHECK->m_pDESC );
		m_pTCHECK[ i ]->m_id = GetUniqueID();
		m_pTCHECK[ i ]->SetStateButton();
		m_pTCHECK[ i ]->MoveComponent( ptBasisPos + ptOffset );
		AddKid( m_pTCHECK[ i ] );
		ptOffset.y += n;
	}

	delete pTCHECK;

	UpdateUI();
}
// -------------------------------------------------------------------------------
CTGameSettingFrame::~CTGameSettingFrame()
{
	if( m_pTCHECK )
		delete [] m_pTCHECK;
}
// ===============================================================================
void CTGameSettingFrame::UpdateUI()
{
	for( INT i=0 ; i < m_pTLIST->GetItemPerPage() ; ++i )
	{
		BOOL bValid = ( m_pTLIST->GetTop() + i < m_pTLIST->GetItemCount() );
		if( bValid )
		{
			m_pTCHECK[ i ]->Select( *(CTGameSettingFrame::TOPTIONPTR[ m_pTLIST->GetTop()+i ]) );
			RemoveKid(m_pTCHECK[ i ] );
			AddKid(m_pTCHECK[ i ] );
			m_pTCHECK[ i ]->ShowComponent(TRUE);
		}
		else
		{
			m_pTCHECK[ i ]->ShowComponent(FALSE);
		}
	}
}
// ===============================================================================
void CTGameSettingFrame::OnLButtonDown( UINT nFlags, CPoint pt)
{
	ITInnerFrame::OnLButtonDown(nFlags, pt);

	if( IsVisible() )
	{
		for( int i=0 ; i < m_pTLIST->GetItemPerPage() ; ++i )
		{
			BOOL bChecked = m_pTCHECK[ i ]->IsStateDown();

			BOOL bValid = ( m_pTLIST->GetTop() + i < m_pTLIST->GetItemCount() );
			if( bValid )
			{
				int index = m_pTLIST->GetTop() + i;
				BOOL bChecked = m_pTCHECK[i]->IsStateDown() ? TRUE : FALSE;
				BOOL bCurrent = *(CTGameSettingFrame::TOPTIONPTR[ index ]);

				if( bChecked != bCurrent && index == 0 ) // ���ǥ�� ����
				{
					CTGaugePannel* pTGAUGE = static_cast<CTGaugePannel*>( CTClientGame::GetInstance()->GetFrame( TFRAME_GAUGE ) );
					if( bChecked  )
						pTGAUGE->AttachCaution15();
					else
						pTGAUGE->DettachCaution15();
				}

				*(CTGameSettingFrame::TOPTIONPTR[ index ]) = bChecked;
			}
		}
	}
}
// ===============================================================================

HRESULT CTGameSettingFrame::Render(DWORD dwTickCount)
{
	if( IsVisible() )
	{
		UpdateUI();
	}

	return ITInnerFrame::Render(dwTickCount);
}
