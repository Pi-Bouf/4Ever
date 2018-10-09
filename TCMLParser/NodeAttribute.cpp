#include "stdafx.h"
#include "NodeAttribute.h"

NodeAttribute::NodeAttribute(CString name, CString value)
{
	this->name = name;
	this->value = value;
}

NodeAttribute::~NodeAttribute()
{
}

CString NodeAttribute::GetName()
{
	return name;
}

CString NodeAttribute::GetValue()
{
	return value;
}

NodeAttribute NodeAttribute::FromNameAndValue(CString name, CString value)
{
	return NodeAttribute(name, value);
}
