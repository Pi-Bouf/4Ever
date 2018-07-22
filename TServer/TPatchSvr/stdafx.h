// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef STRICT
#define STRICT
#endif

// �Ʒ� ������ �÷����� �켱�ϴ� �÷����� ������� �ؾ� �Ѵٸ� ���� ���Ǹ� �����Ͻʽÿ�.
// �ٸ� �÷����� ���Ǵ� �ش� ���� �ֽ� ������ MSDN�� �����Ͻʽÿ�.
#ifndef WINVER				// Windows 95 �� Windows NT 4 �̻󿡼��� ����� ����� �� �ֽ��ϴ�.
#define WINVER 0x0400		// �� ���� Windows 98 �� Windows 2000 �̻��� ������� �ϴ� �� ������ ������ �����Ͻʽÿ�.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 �̻󿡼��� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINNT 0x0500	// �� ���� Windows 2000 �̻��� ������� �ϴ� �� ������ ������ �����Ͻʽÿ�.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 �̻󿡼��� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINDOWS 0x0410 // �� ���� Windows Me �̻��� ������� �ϴ� �� ������ ������ �����Ͻʽÿ�.
#endif

#ifndef _WIN32_IE			// IE 4.0 �̻󿡼��� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_IE 0x0400	// �� ���� IE 5.0 �̻��� ������� �ϴ� �� ������ ������ �����Ͻʽÿ�.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// ATL�� �Ϲ����̸� �밳 �����ϰ� ���õǴ� �Ϻ� ��� �޽��� ������ �����մϴ�.
#define _ATL_NO_COM_SUPPORT
#define _ATL_ALL_WARNINGS

#include "resource.h"

#include "AtlTime.h"

#include "TNetLib.h"
#include "NetCode.h"
#include "TPatchType.h"
#include "CTProtocol.h"
#include "DBAccess.h"
