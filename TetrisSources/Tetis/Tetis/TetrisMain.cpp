// MonkeyProject_OgreBasics.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Application.h"

#include <GameCore/Core.h>


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	try
	{
		SDK::Core::Run(std::make_unique<TetrisGame::ApplicationDelegateImpl>());
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	return 0;
}
