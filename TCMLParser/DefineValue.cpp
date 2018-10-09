#include "stdafx.h"
#include "DefineValue.h"

DefineValue::DefineValue(string name, string hex)
{
	this->name = CString(name.c_str());
	this->hex = hex;
	this->realValue = std::stoi(this->hex, 0, 16);
}

DefineValue::~DefineValue()
{
}

int DefineValue::getRealValue()
{
	return this->realValue;
}

CString DefineValue::getName()
{
	return this->name;
}

string DefineValue::getHex()
{
	return this->hex;
}
