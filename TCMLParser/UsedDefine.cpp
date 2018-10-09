#include "stdafx.h"
#include "UsedDefine.h"


UsedDefine::UsedDefine(CString name, DWORD realValue)
{
	this->name = name;
	this->realValue = realValue;
}

CString UsedDefine::formattedDefine()
{
	CString result;
	result.Format("\t<DEFINE %s = %d>", this->name, this->realValue);

	return result;
}

int UsedDefine::operator==(UsedDefine u1) {
	if ((this->name == u1.name) && (this->realValue == u1.realValue)) return 1;
	else return 0;
}

UsedDefine::~UsedDefine()
{
}
