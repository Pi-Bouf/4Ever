#pragma once
class NodeAttribute
{
public:
	NodeAttribute(CString name, CString value);
	~NodeAttribute();

	CString GetName();
	CString GetValue();
	static NodeAttribute FromNameAndValue(CString name, CString value);

private:
	CString name;
	CString value;
};

