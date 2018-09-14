#pragma once

#include "TCMLParser.h"
#include "DefineValues.h"
#include "DefineValue.h"
#include "NodeAttribute.h"

class Node
{
public:
	Node(COMPINST vComp, int frameLevel, vector<NodeAttribute> attributes);
	Node();
	~Node();
	static Node FromFrameDesc(COMPINST vComp, DefineValues *defineValues, int frameLevel);
	void AddChild(Node child);

	CString GetAllString();

private:
	int frameLevel;
	COMPINST vComp;
	vector<NodeAttribute> attributes;
	vector<Node> childs;
	CString typeText;

	CString GetNodeType(BYTE type);
};

