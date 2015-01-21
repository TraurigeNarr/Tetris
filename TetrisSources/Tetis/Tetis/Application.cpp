#include "stdafx.h"

#include "Application.h"

#include "OpenGLRenderer.h"

#include <Windows.h>
#include <time.h>

namespace
	{

	void RegisterApplicationClass(HINSTANCE ih_instance, WNDPROC i_wnd_proc, LPCWSTR i_class_name)
		{
		// Register Class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = i_wnd_proc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = ih_instance;
		wcex.hIcon = 0;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = 0;
		wcex.lpszClassName = i_class_name;
		wcex.hIconSm = 0;

		RegisterClassEx(&wcex);
		}

	LARGE_INTEGER liFrequency;
	SDK::uint64 GetAbsoluteMS()
		{
		LARGE_INTEGER liCounter;
		QueryPerformanceCounter(&liCounter);
		return static_cast<SDK::uint64>(((double)(liCounter.QuadPart)) / (double)liFrequency.QuadPart * 1000.);
		}


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{

		switch (message)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			}

		return DefWindowProc(hWnd, message, wParam, lParam);
		}

	}


namespace TetrisGame
	{
	
	const int WINDOWED_STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	int CONTENT_WIDTH = 640;
	int CONTENT_HEIGHT = 960;

	// global instance of Application
	Application appInstance;

	Application::Application()
		: m_working(false)
		, mh_instance(nullptr)
		, mp_renderer(nullptr)
		{}

	Application::~Application()
		{}

	bool Application::CreateWin32Window(HINSTANCE ih_instance)
		{
		this->mh_instance = ih_instance;
		return true;
		}

	void Application::Initialize(HINSTANCE ih_instance)
		{
		const bool windowCreated = CreateWin32Window(ih_instance);
		if (!windowCreated)
			throw std::runtime_error("Window cannot be created.");

		//single instance check
		TCHAR fileName[MAX_PATH];
		GetModuleFileName(NULL, fileName, MAX_PATH);
		fileName[MAX_PATH - 1] = 0; //string can be not null-terminated on winXP
		for (int i = 0; i < MAX_PATH; ++i)
			{
			if (fileName[i] == L'\\') //symbol \ is not allowed in CreateMutex mutex name
				fileName[i] = ' ';
			}

		SetLastError(0);

		LPCWSTR class_name = L"Tetris";
		RegisterApplicationClass(ih_instance, (WNDPROC)WndProc, class_name);

		// calculate client size
		RECT clientSize;
		clientSize.top = 0;
		clientSize.left = 0;
		clientSize.right = CONTENT_WIDTH;
		clientSize.bottom = CONTENT_HEIGHT;

		ULONG style = WINDOWED_STYLE | WS_CLIPCHILDREN;
		AdjustWindowRect(&clientSize, style, FALSE);

		int realWidth = clientSize.right - clientSize.left;
		int realHeight = clientSize.bottom - clientSize.top;

		int windowLeft = -10000;
		int windowTop = -10000;

		// create window
		mh_window = CreateWindow(class_name, L"", style, windowLeft, windowTop,
			realWidth, realHeight, NULL, NULL, mh_instance, NULL);

		ShowWindow(mh_window, SW_SHOW);
		UpdateWindow(mh_window);

		// fix ugly ATI driver bugs. Thanks to ariaci (Taken from Irrlight).
		MoveWindow(mh_window, windowLeft, windowTop, realWidth, realHeight, TRUE);
		SetWindowText(mh_window, class_name);

		// move window
		clientSize.top = 0;
		clientSize.left = 0;
		clientSize.right = CONTENT_WIDTH;
		clientSize.bottom = CONTENT_HEIGHT;

		AdjustWindowRect(&clientSize, style, FALSE);

		realWidth = clientSize.right - clientSize.left;
		realHeight = clientSize.bottom - clientSize.top;

		windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
		windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;
		MoveWindow(mh_window, windowLeft, windowTop, realWidth, realHeight, TRUE);

		// initialize renderer
		mp_renderer.reset(new OpenGLRenderer(mh_window, IRect()));
		mp_renderer->Reshape();

		RAWINPUTDEVICE Rid[2];
		// Keyboard
		Rid[0].usUsagePage = 0x01;
		Rid[0].usUsage = 0x06;
		Rid[0].dwFlags = 0;
		Rid[0].hwndTarget = NULL;
		// Mouse
		Rid[1].usUsagePage = 0x01;
		Rid[1].usUsage = 0x02;
		Rid[1].dwFlags = 0;
		Rid[1].hwndTarget = NULL;

		if (RegisterRawInputDevices(Rid, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
			throw std::runtime_error("Cannot register input devices");

		// initialize timer
		QueryPerformanceFrequency(&liFrequency);
		}

	void Application::Start()
		{
		m_working = true;

		MSG msg;
		while (m_working)
			{
			// process messages
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
				SDK::uint64 start_time = GetAbsoluteMS();
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
					{
					/*ApplicationCore * appCore = Core::Instance()->GetApplicationCore();
					if (appCore && appCore->OnQuit())
					{
					exit(0);
					}
					else
					{
					m_working = true;
					}*/
					m_working = false;
					}

				mp_renderer->SetClearColor(Color(200, 0, 0, 150));
				mp_renderer->RenderScene();
				
				SDK::uint64 elapsed_time = (GetAbsoluteMS() - start_time);
				int sleep_ms = 1;
				
				const int FPS = 60;
				const int frame_time = 1000 / FPS;
				sleep_ms = static_cast<int>(frame_time - elapsed_time);
				if (sleep_ms < 1)
					sleep_ms = 1;
				//Sleep(sleep_ms);
				} // peek message cycle
			} // main cycle
		}

	void Application::Release()
		{
		mp_renderer->Release();
		mp_renderer.reset();
		}

	void Application::RequestShutdown()
		{
		m_working = false;
		}

	} // TetrisGame