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

BOOL CDebugSocket::Initialize(char *szIPAddr,int nPort)
{
	m_SendSock = socket(AF_INET, SOCK_STREAM, 0);

	if(m_SendSock == INVALID_SOCKET)
		return FALSE;

	memset(&m_siAddr, 0, sizeof(SOCKADDR_IN));
	
	m_siAddr.sin_family = AF_INET;
	m_siAddr.sin_port = htons(nPort);
	m_siAddr.sin_addr.s_addr = inet_addr(szIPAddr);

	int nConnect;
	nConnect = connect(m_SendSock, (SOCKADDR *)&m_siAddr, sizeof(SOCKADDR));

	if(nConnect != SOCKET_ERROR)
	{
		char *szData = "TMAP is online!";
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

void CDebugSocket::LogLogin(char* szIP, CString strName)
{
	char szData[256];
	CString strData;
	strData.Format("TMAP: " + strName + " logged in! (IP-Address: %s)", szIP);
	lstrcpy(szData, LPCSTR(strData));

	SendToTerminal(szData);
}

void CDebugSocket::LogInvalidPacket(char* szIP)
{
	char szData[256];
	CString strData;
	strData.Format("TMAP: Invalid Packet logged! IP-Address: %s", szIP);
	lstrcpy(szData, LPCSTR(strData));

	SendToTerminal(szData);
}

void CDebugSocket::LogNonVaildPacket(char* szIP, WORD wPacketID)
{
	char szData[256];
	CString strData;
	strData.Format("TMAP: Packet logged! Packet ID: %d", wPacketID);
	lstrcpy(szData, LPCSTR(strData));

	SendToTerminal(szData);
}