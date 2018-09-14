#pragma once

#define LIKE_NULL 4294967295

#include "DefineValue.h"
#include "UsedDefine.h"

class DefineValues
{
public:
	DefineValues();
	~DefineValues();

	void Add(string name, string hex);
	void Used(CString name, DWORD realValue);
	CString GetAllDefines();
	CString FindWithCriteria(DWORD value, CString wordLike, CString defaultPrefix);

private:
	vector<DefineValue> defineValues;
	vector<UsedDefine> usedDefines;
};

