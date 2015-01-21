#ifndef __TETRISAPPLICATION_H__
#define __TETRISAPPLICATION_H__

namespace TetrisGame
	{
	
	class Application
		{
		private:
			bool m_working;

		// win specific
		private:
			HINSTANCE mh_instance;
			HWND			mh_window;

			HDC hDC;
			HGLRC hRC;
			HANDLE hMutex;

		private:
			bool CreateWin32Window(HINSTANCE hInstance);

		public:
			Application();
			~Application();

			void Initialize(HINSTANCE ih_instance);
			void Release();
			void Start();
			void RequestShutdown();

			HINSTANCE GetInstance() const;
			HWND GetWindow() const;
		};

	extern Application appInstance;

	} // TetrisGame

#endif