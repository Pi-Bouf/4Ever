// SSHandler.cpp : S/S Protocol Handler �Լ��� �����Դϴ�.

#include "stdafx.h"
#include "TLoginSvr.h"
#include "TLoginSvrModule.h"
#include <iostream>

DWORD CTLoginSvrModule::OnSM_QUITSERVICE_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	LogEvent(_T("SM_QUITSERVICE_REQ detected !!"));

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cout << "GROS BORDEL" << endl;

	return EC_NOERROR;
}
