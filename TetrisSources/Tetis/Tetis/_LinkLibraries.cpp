#include "stdafx.h"

#include "Game/_Link.h"

#pragma comment(lib, "opengl32")


#ifndef _STATIC//will need glew32d.dll or glew32.dll
#ifdef _DEBUG
#pragma comment(lib, "Debug\\Win32\\glew32d")
#else
#pragma comment(lib, "Release\\Win32\\glew32")
#endif
#else//static libs
#ifdef _DEBUG
#pragma comment(lib, "Debug\\Win32\\glew32sd")
#else
#pragma comment(lib, "Release\\Win32\\glew32s")
#endif
#endif