#pragma once

#include "Node.h"

class Nodes
{
public:
	Nodes();
	~Nodes();

	void Add(Node node);
	CString GetAllStrings();
	void Clear();

private:
	vector<Node> nodes;
};

