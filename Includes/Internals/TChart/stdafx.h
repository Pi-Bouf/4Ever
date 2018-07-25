// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
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

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

#pragma warning( disable : 4312)


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxsock.h>


#include <T3D.h>
#include <NetCode.h>

#include "TChartType.h"