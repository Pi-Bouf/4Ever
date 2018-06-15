#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif		

#pragma warning( disable : 4786 4530 4503 4552)


#define WIN32_LEAN_AND_MEAN
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include <TNetLib.h>
#include "CryptographyExt.h"
