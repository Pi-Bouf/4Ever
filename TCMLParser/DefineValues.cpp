#include "stdafx.h"
#include "DefineValues.h"

#include <iostream>
#include <algorithm>
#include <vector>


DefineValues::DefineValues()
{
}


DefineValues::~DefineValues()
{
}

void DefineValues::Add(string name, string hex)
{
	DefineValue defValueToAdd(name, hex);
	defineValues.push_back(defValueToAdd);
}

void DefineValues::Used(CString name, DWORD realValue) {
	UsedDefine usedDefineToAdd(name, realValue);

	std::vector<UsedDefine>::iterator it = std::find(usedDefines.begin(), usedDefines.end(), usedDefineToAdd);
	if (!(it != usedDefines.end())) {
		usedDefines.push_back(usedDefineToAdd);
	}
}

CString DefineValues::GetAllDefines()
{
	CString result;

	for (int i = 0; i < usedDefines.size(); i++) {
		UsedDefine currentUsedDefine = usedDefines[i];

		result += currentUsedDefine.formattedDefine() + "\n";
	}

	return result;
}

CString DefineValues::FindWithCriteria(DWORD value, CString wordLike, CString defaultPrefix)
{
	if (value != LIKE_NULL) {
		for (int i = 0; i < defineValues.size(); i++)
		{
			DefineValue currentDefValue = defineValues.at(i);
			if (currentDefValue.getRealValue() == value && currentDefValue.getName().Find(wordLike) != -1) {
				Used(currentDefValue.getName(), currentDefValue.getRealValue());
				return currentDefValue.getName();
			}
		}
	}
	else {
		return CString("LIKE_NULL");
	}

	CString unknownResult;
	unknownResult.Format("%s_%d", defaultPrefix, value);

	Used(unknownResult, value);

	return unknownResult;
}
