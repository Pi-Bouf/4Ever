#pragma once
class UsedDefine
{
public:
	UsedDefine(CString name, DWORD realValue);
	CString formattedDefine();
	int operator==(UsedDefine u1);
	~UsedDefine();

private:
	CString name;
	DWORD realValue;
};

