#include "stdafx.h"
#include "Nodes.h"


Nodes::Nodes()
{
}


Nodes::~Nodes()
{
}

void Nodes::Add(Node node)
{
	nodes.push_back(node);
}

CString Nodes::GetAllStrings()
{
	CString result;

	for (int i = 0; i < nodes.size(); i++) {
		Node current = nodes[i];
		result += current.GetAllString();
	}

	return result;
}

void Nodes::Clear()
{
	nodes.clear();
}
