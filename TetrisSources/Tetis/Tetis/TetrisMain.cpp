// MonkeyProject_OgreBasics.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Application.h"

#include <Windows.h>

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
	{
	try
		{
		TetrisGame::appInstance.Initialize(hInst);
		TetrisGame::appInstance.Start();
		TetrisGame::appInstance.Release();
		}
	catch (std::exception& e)
		{
		MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}
	return 0;
	}
