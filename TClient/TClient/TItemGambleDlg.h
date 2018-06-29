#pragma once

class CTItemGambleDlg : public CTItemSlotBaseDlg
{
public:
	enum 
	{
		TARGET_SLOT,
		SLOT_COUNT
	};

	LPTOPENBYCASH m_pOpenByCash;
	void SetOpenByCash( LPTOPENBYCASH );
	void ClearOpenByCash();

public:
	virtual BOOL CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const;
	virtual BOOL SetItem(BYTE bSlotID, LPTITEM pItem, BYTE bInven=T_INVALID, BYTE bInvenSlot=T_INVALID);

public:
	CTItemGambleDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTItemGambleDlg();
};