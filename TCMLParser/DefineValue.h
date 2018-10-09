#pragma once

#include <string>

class DefineValue
{
public:
	DefineValue(string name, string hex);
	~DefineValue();
	int getRealValue();
	CString getName();
	string getHex();

private:
	CString name;
	string hex;
	int realValue;
};

