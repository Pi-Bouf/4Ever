#pragma once

class CTChatMenuNew : public CTClientUIBase
{
public:
	TComponent *m_pTitle;
	TEdit *m_pEditTitle;
	TComponent *m_pChatOption[TCHAT_MENU_COUNT];

	TButton *m_pNewChat;
	TButton *m_pClose;
	TButton *m_pCheckBox[TCHAT_MENU_COUNT];

	CTChatList *m_pHost;

	INT m_nIndex;
public:
	void Init(CString strTitle, DWORD dwChatFlag, INT nIndex);
	TEdit* GetCurEdit();
	void SetTitle();

protected:
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);

public:
	virtual void ShowComponent(BOOL bVisible = TRUE);

public:
	CTChatMenuNew( TComponent *pParent, LP_FRAMEDESC pDesc ,CTChatList *pHost );
	virtual ~CTChatMenuNew();
};
