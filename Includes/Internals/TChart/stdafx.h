// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

struct DXGI_JPEG_DC_HUFFMAN_TABLE
{
	typedef int CodeCounts[12];
	typedef int CodeValues[12];
};

struct DXGI_JPEG_AC_HUFFMAN_TABLE
{
	typedef int CodeCounts[16];
	typedef int CodeValues[162];
};

struct DXGI_JPEG_QUANTIZATION_TABLE
{
	typedef int  Elements[64];
};

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

#pragma warning( disable : 4312)


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxsock.h>


#include <T3D.h>
#include <NetCode.h>

#include "TChartType.h"