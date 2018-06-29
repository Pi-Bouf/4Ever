#pragma once

class CTTitleSlot;
class CTTitleItem;

class CTTitleSlot : public CTCtrlListSlot
{
public :	// Member Data.

	TButton*					m_pCheckBtn;

public :

	CTTitleSlot();
	virtual ~CTTitleSlot();

	virtual	void				Select( BOOL bSel );
	virtual void				ShowComponent( BOOL bShow );

public :	// Member Function.

	BOOL						OnLButtonDown( CPoint pt );

};

class CTTitleItem : public CTCtrlListItem
{
public:
	BYTE		m_bItemType;

	LPTPORTAL	m_pTPortal;
	DWORD		m_dwRealPrice;

	CString		m_strRegion;

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);

public:
	CTTitleItem() : m_bItemType(0), m_pTPortal(NULL), m_dwRealPrice(0) {}
	virtual ~CTTitleItem();
};

class CTTitleDlg : public CTClientUIBase
{

public:
	TList*		m_pList;
	TList*		m_pList2;
	TList*		m_pList3;

	CTCtrlList*		m_pList_c;

	TComponent *m_pFocus;
	TComponent *m_pDescription;

	static TButton		*s_pCheck;
	static CPoint		s_ptCheck_Offset;

	CPoint		m_ptCheckBoxOffset;
	CPoint		m_ptBasePointCategory;

	TButton*	m_pBtn;
	TButton*	m_pCheckBox[9];

	//BOOL		m_bNeedResetPos;

public:
	virtual void AddLists();
	virtual void ClearLists();
	virtual void AddStringL1(CString strRequirement, CString strTitle, WORD wTitleID);
	virtual void ResetPos();

public:
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void SwitchFocus(TComponent *pCandidate);
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTTitleDlg( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual ~CTTitleDlg();
};