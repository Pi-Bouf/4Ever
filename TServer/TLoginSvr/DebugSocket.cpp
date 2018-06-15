#include "StdAfx.h"

CDebugSocket::CDebugSocket()
{
	InitializeCriticalSection(&m_DebugLock);
}

CDebugSocket::~CDebugSocket()
{
	CLOSESOCKET(m_SendSock);
	DeleteCriticalSection(&m_DebugLock);
}

BOOL CDebugSocket::Initialize(char *pIPAddr,int pPort)
{
	m_SendSock = socket(AF_INET, SOCK_STREAM, 0);

	if(m_SendSock == INVALID_SOCKET)
		return FALSE;

	memset(&m_siAddr, 0, sizeof(SOCKADDR_IN));
	
	m_siAddr.sin_family = AF_INET;
	m_siAddr.sin_port = htons(pPort);
	m_siAddr.sin_addr.s_addr = inet_addr(pIPAddr);

	int nConnect;
	nConnect = connect(m_SendSock, (SOCKADDR *)&m_siAddr, sizeof(SOCKADDR));

	if(nConnect != SOCKET_ERROR)
	{
		char *szData = "TLOGIN is online!";
		send(m_SendSock, szData, INT(strlen(szData)), 0);
	}

	return TRUE;
}

void CDebugSocket::SendToTerminal(char *szData)
{	
	EnterCriticalSection(&m_DebugLock);

		send(m_SendSock, szData, INT(strlen(szData)), 0);

	LeaveCriticalSection(&m_DebugLock);
}

void CDebugSocket::LogLogin(CTUser *pUser, DWORD dwUserID)
{
	char szData[256];
	CString strData;
	strData.Format("TLOGIN: " + pUser->m_strUserID + " (User ID: %d) logged in! (IP-Address: %s)", dwUserID, inet_ntoa(pUser->m_addr.sin_addr));
	lstrcpy(szData, LPCSTR(strData));

	//SendToTerminal(szData);
}

void CDebugSocket::LogInvalidPacket(char* szIP)
{
	char szData[256];
	CString strData;
	strData.Format("TLOGIN: Invalid Packet logged! IP-Address: %s", szIP);
	lstrcpy(szData, LPCSTR(strData));

	//SendToTerminal(szData);
}