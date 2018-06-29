#include "stdafx.h"
#include "TChatMenuNew.h"
#include "TClientGame.h"

#ifdef NEW_IF

CTChatMenuNew::CTChatMenuNew( TComponent *pParent, LP_FRAMEDESC pDesc, CTChatList* pHost )
:	CTClientUIBase( pParent, pDesc )
{
	m_pHost = pHost;

	DWORD dwCheckBox[] = {
		ID_CTRLINST_CHATMENU_CHECKBOX0,
		ID_CTRLINST_CHATMENU_CHECKBOX1,
		ID_CTRLINST_CHATMENU_CHECKBOX2,
		ID_CTRLINST_CHATMENU_CHECKBOX3,
		ID_CTRLINST_CHATMENU_CHECKBOX4,
		ID_CTRLINST_CHATMENU_CHECKBOX5,
		ID_CTRLINST_CHATMENU_CHECKBOX6,
		ID_CTRLINST_CHATMENU_CHECKBOX7,
		ID_CTRLINST_CHATMENU_CHECKBOX8,
		ID_CTRLINST_CHATMENU_CHECKBOX9
	};

	DWORD dwChatOption[] = {
		ID_CTRLINST_CHATMENU_CHATOPT0,
		ID_CTRLINST_CHATMENU_CHATOPT1,
		ID_CTRLINST_CHATMENU_CHATOPT2,
		ID_CTRLINST_CHATMENU_CHATOPT3,
		ID_CTRLINST_CHATMENU_CHATOPT4,
		ID_CTRLINST_CHATMENU_CHATOPT5,
		ID_CTRLINST_CHATMENU_CHATOPT6,
		ID_CTRLINST_CHATMENU_CHATOPT7,
		ID_CTRLINST_CHATMENU_CHATOPT8,
		ID_CTRLINST_CHATMENU_CHATOPT9
	};

	for(int i=0; i<TCHAT_MENU_COUNT;i++)
	{
		m_pCheckBox[i] = (TButton*) FindKid(dwCheckBox[i]);
		m_pChatOption[i] = FindKid(dwChatOption[i]);
	}

	m_pTitle = FindKid(ID_CTRLINST_TITLE);
	m_pNewChat = (TButton*) FindKid(ID_CTRLINST_CHATMENU_NEWCH);
	m_pClose = (TButton*) FindKid(ID_CTRLINST_CHATMENU_CLOSE);

	CString strTitle;
	m_pEditTitle = (TEdit*)FindKid(ID_CTRLINST_CHATMENU_EDIT);
	m_pEditTitle->ClearText();
	m_pEditTitle->SetFocus(FALSE);

	m_nIndex = NULL;
}

CTChatMenuNew::~CTChatMenuNew()
{	

}

void CTChatMenuNew::Init(CString strTitle, DWORD dwChatFlag, INT nIndex)
{
	m_nIndex = nIndex;

	static const DWORD dwChatOption[] = 
	{
		TSTR_CHAT_TAB_ALL,
		TSTR_CHAT_TAB_NORMAL,
		TSTR_CHAT_TAB_WHISPER,
		TSTR_CHAT_TAB_PARTY,
		TSTR_CHAT_TAB_FORCE,
		TSTR_CHAT_TAB_GUILD,
		TSTR_CHAT_TACTICS,
		TSTR_CHAT_TAB_RESULT,
		TSTR_CHAT_WORLD,
		TSTR_CHATNEW_SYSTEM,
	};

	if(dwChatFlag & TCHAT_FLAG_WHISPER)
		m_pCheckBox[TCHAT_MENU_WHISPER]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_WHISPER]->Select(FALSE);

	if(dwChatFlag & TCHAT_FLAG_NEAR)
		m_pCheckBox[TCHAT_MENU_NORMAL]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_NORMAL]->Select(FALSE);

	if(dwChatFlag & TCHAT_FLAG_MAP)
		m_pCheckBox[TCHAT_MENU_ALL]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_ALL]->Select(FALSE);

	if(dwChatFlag & TCHAT_FLAG_PARTY)
		m_pCheckBox[TCHAT_MENU_GROUP]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_GROUP]->Select(FALSE);

	if(dwChatFlag & TCHAT_FLAG_GUILD)
		m_pCheckBox[TCHAT_MENU_GUILD]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_GUILD]->Select(FALSE);

	if(dwChatFlag & TCHAT_FLAG_GUILD)
		m_pCheckBox[TCHAT_MENU_GUILD]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_GUILD]->Select(FALSE);

	if(dwChatFlag & TCHAT_FLAG_INFO)
		m_pCheckBox[TCHAT_MENU_RESULT]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_RESULT]->Select(FALSE);

	if(dwChatFlag & TCHAT_FLAG_FORCE)
		m_pCheckBox[TCHAT_MENU_RAID]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_RAID]->Select(FALSE);

	if(dwChatFlag & TCHAT_FLAG_TACTICS)
		m_pCheckBox[TCHAT_MENU_MERCENARY]->Select(TRUE);
	else
		m_pCheckBox[TCHAT_MENU_MERCENARY]->Select(FALSE);

	for(int i=0; i<TCHAT_MENU_COUNT;i++)
	{
		m_pChatOption[i]->m_strText = CTChart::LoadString( (TSTRING) dwChatOption[i]);

		if(i > TCHAT_MENU_RESULT)
		{
			m_pCheckBox[i]->ShowComponent(FALSE);
			RemoveKid(m_pCheckBox[i]);
		}

		m_pCheckBox[i]->EnableComponent(FALSE);
	}

	if(m_pHost->IsOuter())
		m_pNewChat->m_strText = CTChart::LoadString( (TSTRING) TSTR_CHATNEW_DELCHAT);
	else
		m_pNewChat->m_strText = CTChart::LoadString( (TSTRING) TSTR_CHATNEW_CREATECHAT);


	m_pTitle->m_strText = strTitle;
	m_pHost->m_pTab->m_strText = strTitle;
	m_pEditTitle->ResetTextSetting();
	m_pClose->m_strText = CTChart::LoadString( TSTR_CLOSE );
}

void CTChatMenuNew::OnLButtonDown(UINT nFlags, CPoint pt)
{
	for(int i = 0; i < TCHAT_MENU_COUNT; i++)
	{
		if(m_pCheckBox[i]->HitTest(pt) && m_pCheckBox[i]->IsVisible())
		{
			m_pCheckBox[i]->EnableComponent(TRUE);

			if(m_pCheckBox[i]->IsStateDown())
				m_pCheckBox[i]->Select(FALSE);
			else
				m_pCheckBox[i]->Select(TRUE);

			DWORD dwFlag = 0;
			if(m_pCheckBox[TCHAT_MENU_WHISPER]->IsStateDown())
				dwFlag = dwFlag | TCHAT_FLAG_WHISPER;
			if(m_pCheckBox[TCHAT_MENU_NORMAL]->IsStateDown())
				dwFlag = dwFlag | TCHAT_FLAG_NEAR;
			if(m_pCheckBox[TCHAT_MENU_ALL]->IsStateDown())
				dwFlag = dwFlag | TCHAT_FLAG_MAP;
			if(m_pCheckBox[TCHAT_MENU_GROUP]->IsStateDown())
				dwFlag = dwFlag | TCHAT_FLAG_PARTY;
			if(m_pCheckBox[TCHAT_MENU_GUILD]->IsStateDown())
				dwFlag = dwFlag | TCHAT_FLAG_GUILD;
			if(m_pCheckBox[TCHAT_MENU_RESULT]->IsStateDown())
				dwFlag = dwFlag | TCHAT_FLAG_INFO;
			if(m_pCheckBox[TCHAT_MENU_RAID]->IsStateDown())
				dwFlag = dwFlag | TCHAT_FLAG_FORCE;
			if(m_pCheckBox[TCHAT_MENU_MERCENARY]->IsStateDown())
				dwFlag = dwFlag | TCHAT_FLAG_TACTICS;

			m_pHost->SetChatFlag(dwFlag);


            m_pCheckBox[i]->EnableComponent(FALSE);
		}
	}

	if(m_pEditTitle->HitTest(pt) && m_pEditTitle->IsVisible())
	{
		if(m_pHost->m_pHost->IsChatEditON())
			return;

		m_pEditTitle->ClearText();
		m_pTitle->m_strText.Empty();
		m_pEditTitle->SetFocus(TRUE);
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
TEdit* CTChatMenuNew::GetCurEdit()
{
	if( !IsVisible() )
		return NULL;

	if( m_pEditTitle->IsVisible() && m_pEditTitle->GetFocus() )
		return m_pEditTitle;

	return NULL;
}

void CTChatMenuNew::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if(m_pClose->HitTest(pt) && m_pClose->IsVisible())
		ShowComponent(FALSE);

	if(m_pNewChat->HitTest(pt) && m_pNewChat->IsVisible())
	{
		if(m_pHost->IsOuter())	
			m_pHost->m_pHost->DeleteList(m_pHost);
		else
			m_pHost->m_pHost->AddList(TCHAT_FLAG_ALL_CHAT);

		return;
	}

	CTClientUIBase::OnLButtonUp(nFlags,pt);
}

void CTChatMenuNew::ShowComponent(BOOL bVisible)
{
	m_pEditTitle->SetFocus(FALSE);

	CTClientUIBase::ShowComponent(bVisible);
}

void CTChatMenuNew::SetTitle()
{
	CString strTitle = m_pEditTitle->m_strText;

	m_pTitle->m_strText = strTitle;
	m_pHost->m_strTitle = strTitle;
	m_pHost->m_pTab->m_strText = strTitle;

	if(!m_pHost->IsOuter())
		m_pHost->m_pHost->SetInnerTextTab(strTitle);

	m_pEditTitle->ClearText();
	m_pEditTitle->SetFocus(FALSE);
}
#endif