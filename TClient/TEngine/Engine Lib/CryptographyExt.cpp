/* This file is part of Crypt Library Demo application developed by Mihai MOGA.

Image Converter is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

Image Converter is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Crypt Library Demo.  If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

// CryptographyExt.cpp : Demo for Microsoft's Crypt Library functions.
// Author: Stefan-Mihai MOGA, e-mail: contact@mihaimoga.com, phone: +40745497982

#include "stdafx.h"
#include "CryptographyExt.h"

#define SECURITY_WIN32
#include <Security.h>
#pragma comment(lib, "secur32")

#include <nb30.h>
#pragma comment(lib, "netapi32")

#include <wincrypt.h>
#pragma comment(lib, "crypt32")
#pragma comment(lib, "advapi32")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void TraceLastError(LPCTSTR lpszLibrary, LPCTSTR lpszOperation, DWORD dwLastError)
{
	//Display a message and the last error in the TRACE. 
	LPVOID lpszErrorBuffer = NULL;
	CString	strLastError;

	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpszErrorBuffer,
		0,
		NULL);

	strLastError.Format(_T("[%s] %s: %s\n"), lpszLibrary, lpszOperation, lpszErrorBuffer);

	// free alocated buffer by FormatMessage
	LocalFree(lpszErrorBuffer); 

	//Display the last error.
	OutputDebugString(strLastError);
}

CString GetComputerID()
{
	CString strComputerID;
	DWORD dwLength = MAX_STR_BUFFER;
	TCHAR lpszComputer[MAX_STR_BUFFER] = { 0 };
	if (GetComputerNameEx(ComputerNameDnsFullyQualified, lpszComputer, &dwLength))
	{
		lpszComputer[dwLength] = 0;
		strComputerID = lpszComputer;
	}
	else
	{
		if (GetComputerName(lpszComputer, &dwLength))
		{
			lpszComputer[dwLength] = 0;
			strComputerID = lpszComputer;
		}
		else
		{
			strComputerID =  _T("MihaiMoga");
		}
	}
	return strComputerID;
}

BOOL EncryptBuffer(ALG_ID nAlgorithm, LPBYTE lpszOutputBuffer, DWORD& dwOutputLength, LPBYTE lpszInputBuffer, DWORD dwInputLength, LPBYTE lpszSecretKey, DWORD dwSecretKey)
{
	BOOL retVal = FALSE;
	DWORD dwHowManyBytes = dwInputLength;

	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	::CopyMemory(lpszOutputBuffer, lpszInputBuffer, dwHowManyBytes);

	if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		if (CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash))
		{
			if (CryptHashData(hCryptHash, lpszSecretKey, dwSecretKey, 0))
			{
				if (CryptDeriveKey(hCryptProv, nAlgorithm, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey))
				{
					if (CryptEncrypt(hCryptKey, NULL, TRUE, 0, lpszOutputBuffer, &dwHowManyBytes, dwOutputLength))
					{
						dwOutputLength = dwHowManyBytes;
						retVal = TRUE;
					}
					else
					{
						TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptEncrypt"), GetLastError());
					}
					CryptDestroyKey(hCryptKey);
				}
				else
				{
					TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptDeriveKey"), GetLastError());
				}
			}
			else
			{
				TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptHashData"), GetLastError());
			}
			CryptDestroyHash(hCryptHash);
		}
		else
		{
			TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptCreateHash"), GetLastError());
		}
		CryptReleaseContext(hCryptProv, 0);
	}
	else
	{
		TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptAcquireContext"), GetLastError());
	}

	return retVal;
}


BOOL DecryptBuffer(ALG_ID nAlgorithm, LPBYTE lpszOutputBuffer, DWORD& dwOutputLength, LPBYTE lpszInputBuffer, DWORD dwInputLength, LPBYTE lpszSecretKey, DWORD dwSecretKey)
{
	BOOL retVal = FALSE;
	DWORD dwHowManyBytes = dwInputLength;

	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	::CopyMemory(lpszOutputBuffer, lpszInputBuffer, dwHowManyBytes);

	if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		if (CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash))
		{
			if (CryptHashData(hCryptHash, lpszSecretKey, dwSecretKey, 0))
			{
				if (CryptDeriveKey(hCryptProv, nAlgorithm, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey))
				{
					if (CryptDecrypt(hCryptKey, NULL, TRUE, 0, lpszOutputBuffer, &dwHowManyBytes))
					{
						dwOutputLength = dwHowManyBytes;
						retVal = TRUE;
					}
					else
					{
						TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptDecrypt"), GetLastError());
					}
					CryptDestroyKey(hCryptKey);
				}
				else
				{
					TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptDeriveKey"), GetLastError());
				}
			}
			else
			{
				TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptHashData"), GetLastError());
			}
			CryptDestroyHash(hCryptHash);
		}
		else
		{
			TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptCreateHash"), GetLastError());
		}
		CryptReleaseContext(hCryptProv, 0);
	}
	else
	{
		TraceLastError(CRYPT_LIBRARY_NAME, _T("CryptAcquireContext"), GetLastError());
	}

	return retVal;
}

