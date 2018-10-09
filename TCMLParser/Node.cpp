#include "stdafx.h"
#include "Node.h"


Node::Node(COMPINST vComp, int frameLevel, vector<NodeAttribute> attributes)
{
	this->vComp = vComp;
	this->frameLevel = frameLevel;
	this->attributes = attributes;
	this->typeText = GetNodeType(vComp.m_bType);
}

Node::Node()
{
}

Node::~Node()
{
}

CString Node::GetNodeType(BYTE type)
{
	CString typeString;

#pragma region TYPES

	switch (type)
	{
	case 0:
		typeString = "COMPONENT";
		break;
	case 1:
		typeString = "BUTTON";
		break;
	case 2:
		typeString = "SCROLL";
		break;
	case 3:
		typeString = "EDIT";
		break;
	case 4:
		typeString = "COMBO";
		break;
	case 5:
		typeString = "LIST";
		break;
	case 6:
		typeString = "GAUGE";
		break;
	case 8:
		typeString = "TABCTRL";
		break;
	case 9:
		typeString = "FRAME";
		break;
	case 10:
		typeString = "IMAGELIST";
		break;
	case 11:
		typeString = "METER";
		break;
	case 12:
		typeString = "HTTPCTRL";
		break;
	}

#pragma endregion

	return typeString;
}

void Node::AddChild(Node child)
{
	childs.push_back(child);
}

CString Node::GetAllString()
{
	CString result;

#pragma region ELEMENT

#pragma region TABULATION

	for (int i = 0; i < frameLevel; i++) {
		result += "\t";
	}

#pragma endregion

	result += "<" + this->typeText;

	// Attributes
	for (int i = 0; i < attributes.size(); i++) {
		NodeAttribute currentAttribute = attributes[i];

		if (currentAttribute.GetValue() != "LIKE_NULL" && currentAttribute.GetValue() != "") {
			result += " ";
			result += currentAttribute.GetName();
			if (currentAttribute.GetName() == "POSX" || 
				currentAttribute.GetName() == "POSY" || 
				currentAttribute.GetName() == "HEIGHT" || 
				currentAttribute.GetName() == "WIDTH" || 
				currentAttribute.GetName() == "HMARGINE" ||
				currentAttribute.GetName() == "VMARGINE" ||
				currentAttribute.GetName() == "COLOR") {
				result += "=";
				result += currentAttribute.GetValue();
			}
			else {
				result += "=\"";
				result += currentAttribute.GetValue();
				result += "\"";
			}
			
		}
	}

	result += ">\n";

	// Childs
	for (int i = 0; i < childs.size(); i++) {
		Node currentNode = childs[i];

		result += currentNode.GetAllString();
	}

#pragma region TABULATION

	for (int i = 0; i < frameLevel; i++) {
		result += "\t";
	}

#pragma endregion

	result += "</" + this->typeText + ">\n";
	if (frameLevel == 0) {
		result += "\n";
	}

#pragma endregion

	return result;
}

Node Node::FromFrameDesc(COMPINST vComp, DefineValues *defineValues, int frameLevel)
{
	vector<NodeAttribute> attributes;

#pragma region CommentedToDoList

	/* TO DO, BUT USELESS FOR THE MOMENT...
	case BLT: 
		if(0==strcmp(TCMLlval.string_value, "TCML_BLT_FIT"))
			TCMLCurrent->display = TCML_BLT_FIT;
		else if(0==strcmp(TCMLlval.string_value, "TCML_BLT_CENTER"))
			TCMLCurrent->display = TCML_BLT_CENTER;
		else if(0==strcmp(TCMLlval.string_value, "TCML_BLT_TILE"))
			TCMLCurrent->display = TCML_BLT_TILE;
		else
			return -2;
		break;
	//Combo
	case DROP: 		TCMLCurrent->ex.combo.drop = find_idef(TCMLlval.string_value);					break;
	case DOWNLIST:	TCMLCurrent->ex.combo.downlist  = find_idef(TCMLlval.string_value);				break;
	//METER
	case SUB:
		if(TCMLCurrent->ex.meter.sublevel == TCML_ID_NULL) 
			TCMLCurrent->ex.meter.sublevel = 0;
		if(TCMLCurrent->ex.meter.sublevel >= MAX_METER_SUBLEVEL) 
			return -2;
		TCMLCurrent->ex.meter.sub[TCMLCurrent->ex.meter.sublevel++] = find_idef(TCMLlval.string_value);	
		break;
	case SUPER:		TCMLCurrent->ex.meter.super = find_idef(TCMLlval.string_value);					break;
	case SUPERLEVEL:TCMLCurrent->ex.meter.superlevel = TCMLlval.integer_value;						break;
	*/

#pragma endregion

#pragma region dwID

	CString dwIDValue = "";
	if (frameLevel == 0) {
		dwIDValue += defineValues->FindWithCriteria(vComp.m_dwID, "ID_FRAME", "UNKNOWN_ID_FRAME");
	}
	else {
		dwIDValue += defineValues->FindWithCriteria(vComp.m_dwID, "ID_CTRLINST", "UNKNOWN_ID_CTRLINST");
	}
	attributes.push_back(NodeAttribute::FromNameAndValue("ID", dwIDValue));

#pragma endregion

#pragma region MENU
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_LCLICK] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("LCLICK", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_LCLICK], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_RCLICK] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("RCLICK", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_RCLICK], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_DBLCLICK] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("DBLCLICK", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_DBLCLICK], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_LINEUP] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("LINEUP", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_LINEUP], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_LINEDOWN] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("LINEDOWN", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_LINEDOWN], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_VSCROLL] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("VSCROLL", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_VSCROLL], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_HSCROLL] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("HSCROLL", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_HSCROLL], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_SEL_CHANGE] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("SEL_CHANGE", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_SEL_CHANGE], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_TAB_ACTIVE] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("TAB_ACTIVE", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_TAB_ACTIVE], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_ENTER] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("ENTER", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_ENTER], "GM_", "UNKNOWN_GM")));
	}
	if (vComp.m_vMENU[TNM_LCLICK] != LIKE_NULL && vComp.m_vMENU[TNM_ESC] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("ESC", defineValues->FindWithCriteria(vComp.m_vMENU[TNM_ESC], "GM_", "UNKNOWN_GM")));
	}
#pragma endregion

#pragma region MARGINS&POSITIONS
	if (vComp.m_nMargineH != LIKE_NULL && vComp.m_nMargineH != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("HMARGINE", CString(to_string(vComp.m_nMargineH).c_str())));
	}
	if (vComp.m_nMargineV != LIKE_NULL && vComp.m_nMargineV != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("VMARGINE", CString(to_string(vComp.m_nMargineV).c_str())));
	}
	if (vComp.m_nPosX != LIKE_NULL && vComp.m_nPosX != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("POSX", CString(to_string(vComp.m_nPosX).c_str())));
	}
	if (vComp.m_nPosY != LIKE_NULL && vComp.m_nPosY != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("POSY", CString(to_string(vComp.m_nPosY).c_str())));
	}
	if (vComp.m_nHeight != LIKE_NULL && vComp.m_nHeight != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("HEIGHT", CString(to_string(vComp.m_nHeight).c_str())));
	}
	if (vComp.m_nWidth != LIKE_NULL && vComp.m_nWidth != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("WIDTH", CString(to_string(vComp.m_nWidth).c_str())));
	}
#pragma endregion

#pragma region IMAGES
	if (vComp.m_dwImageID[0] != LIKE_NULL && vComp.m_dwImageID[0] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("DEFAULT", defineValues->FindWithCriteria(vComp.m_dwImageID[0], "ID_IMAGE", "ID_IMAGE")));
	}
	if (vComp.m_dwImageID[1] != LIKE_NULL && vComp.m_dwImageID[1] != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("DISABLE", defineValues->FindWithCriteria(vComp.m_dwImageID[1], "ID_IMAGE", "ID_IMAGE")));
	}
#pragma endregion

#pragma region ALIGNMENT
	if (vComp.m_bAlign != LIKE_NULL && vComp.m_bAlign != 0) {
		CString align;
		switch (vComp.m_bAlign)
		{
		case ALIGN_CUSTOM:
			align = "ALIGN_CUSTOM";
			break;
		case ALIGN_LEFT:
			align = "ALIGN_LEFT";
			break;
		case ALIGN_RIGHT:
			align = "ALIGN_RIGHT";
			break;
		case ALIGN_CENTER:
			align = "ALIGN_CENTER";
			break;
		default:
			align = "UNKNOWN_ALIGN";
		}

		attributes.push_back(NodeAttribute::FromNameAndValue("ALIGN", align));
	}
#pragma endregion

#pragma region FONTS
	if (vComp.m_dwFontID != LIKE_NULL && vComp.m_dwFontID != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("FONT", defineValues->FindWithCriteria(vComp.m_dwFontID, "ID_FONT", "UNKNOWN_ID_FONT")));
	}
	if (vComp.m_dwTooltipID != LIKE_NULL && vComp.m_dwTooltipID != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("TIPFONT", defineValues->FindWithCriteria(vComp.m_dwTooltipID, "ID_FONT", "UNKNOWN_ID_FONT")));
	}
#pragma endregion

#pragma region COLOR
	if (vComp.m_dwCOLOR != LIKE_NULL && vComp.m_dwCOLOR != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("COLOR", CString(to_string(vComp.m_dwCOLOR).c_str())));
	}
#pragma endregion

#pragma region BUTTON
	if (vComp.m_vEX.button.up != LIKE_NULL && vComp.m_vEX.button.up != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("UP", defineValues->FindWithCriteria(vComp.m_vEX.button.up, "ID_CTRL", "UNKNOWN_ID_CTRL")));
	}
	if (vComp.m_vEX.button.down != LIKE_NULL && vComp.m_vEX.button.down != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("DOWN", defineValues->FindWithCriteria(vComp.m_vEX.button.down, "ID_CTRL", "UNKNOWN_ID_CTRL")));
	}
	if (vComp.m_vEX.button.hover != LIKE_NULL && vComp.m_vEX.button.hover != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("HOVER", defineValues->FindWithCriteria(vComp.m_vEX.button.hover, "ID_CTRL", "UNKNOWN_ID_CTRL")));
	}
	if (vComp.m_dwStyle != LIKE_NULL && vComp.m_dwStyle == TBS_STATE_BUTTON) {
		attributes.push_back(NodeAttribute::FromNameAndValue("STATEBUTTON", CString(to_string(1).c_str())));
	}
#pragma endregion

#pragma region SCROLL
	if (vComp.m_vEX.scroll.ul != LIKE_NULL && vComp.m_vEX.scroll.ul != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("UL", defineValues->FindWithCriteria(vComp.m_vEX.scroll.ul, "ID_CTRL", "UNKNOWN_ID_CTRL")));
	}
	if (vComp.m_vEX.scroll.dr != LIKE_NULL && vComp.m_vEX.scroll.dr != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("DR", defineValues->FindWithCriteria(vComp.m_vEX.scroll.dr, "ID_CTRL", "UNKNOWN_ID_CTRL")));
	}
	if (vComp.m_vEX.scroll.bar != LIKE_NULL && vComp.m_vEX.scroll.bar != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("BAR", defineValues->FindWithCriteria(vComp.m_vEX.scroll.bar, "ID_CTRL", "UNKNOWN_ID_CTRL")));
	}
#pragma endregion

#pragma region EDIT
	if (vComp.m_vEX.edit.caret != LIKE_NULL && vComp.m_vEX.edit.caret != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("CARET", defineValues->FindWithCriteria(vComp.m_vEX.edit.caret, "ID_CTRL", "UNKNOWN_ID_CTRL")));
	}
	if (vComp.m_dwStyle != LIKE_NULL && vComp.m_dwStyle == TES_PASSWORD) {
		attributes.push_back(NodeAttribute::FromNameAndValue("PASSWORD", CString(to_string(1).c_str())));
	}
#pragma endregion

#pragma region ITEMS
	if (vComp.m_vEX.list.vs != LIKE_NULL && vComp.m_vEX.list.vs != 0) {
		attributes.push_back(NodeAttribute::FromNameAndValue("VERTICAL", defineValues->FindWithCriteria(vComp.m_vEX.list.vs, "ID_CTRL", "UNKNOWN_ID_CTRL")));
	}
	for (int i = 0; i < MAX_LIST_COLUMN; i++) {
		if (vComp.m_vEX.list.item[i] != LIKE_NULL && vComp.m_vEX.list.item[i] != 0) {
			attributes.push_back(NodeAttribute::FromNameAndValue("ITEM", defineValues->FindWithCriteria(vComp.m_vEX.list.item[i], "ID_CTRL", "UNKNOWN_ID_CTRL")));
		}
	}
#pragma endregion

#pragma region TEXT
	if (vComp.m_strText != LIKE_NULL) {
		attributes.push_back(NodeAttribute::FromNameAndValue("TEXT", CString(vComp.m_strText)));
	}
#pragma endregion

#pragma region TOOLTIP
	if (vComp.m_strTooltip != LIKE_NULL) {
		attributes.push_back(NodeAttribute::FromNameAndValue("TOOLTIP", CString(vComp.m_strTooltip)));
	}
#pragma endregion

	return Node(vComp, frameLevel, attributes);
}
