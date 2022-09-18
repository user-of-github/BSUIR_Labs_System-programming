// https://learn.microsoft.com/en-us/windows/win32/controls/cookbook-overview
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <utility>
#include "./constants.h"


WNDCLASSEX configure_window_characteristics(const HINSTANCE&);


int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	WNDCLASSEX   window_characteristics{ configure_window_characteristics(hInstance) };

	if (!RegisterClassEx(&window_characteristics))
		return EXIT_FAILURE;

	const HWND window_handle{ CreateWindow(
		window_characteristics.lpszClassName,
		kMainWindowTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX,
		kMainWindowX, kMainWindowY, kMainWindowWidth, kMainWindowHeight,
		nullptr, nullptr,
		window_characteristics.hInstance,
		nullptr
	) };

	const HWND button_handle{ CreateWindow(
		kButtonSetSizeClass,
		kButtonSetSizeTitle,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		kButtonSetSizeX, kButtonSetSizeY, kButtonSetSizeWidth, kButtonSetSizeHeight,
		window_handle,	reinterpret_cast<HMENU>(kButtonSetSizeId), nullptr, nullptr
	) };

	if (window_handle == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;

	ShowWindow(window_handle, nCmdShow);
	UpdateWindow(window_handle);

	MSG message{};

	while (GetMessage(&message, nullptr, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return static_cast<int>(message.wParam);
}


WNDCLASSEX configure_window_characteristics(const HINSTANCE& hInstance)
{
	WNDCLASSEX response{ sizeof(WNDCLASSEX) };

	response.cbClsExtra = response.cbWndExtra = 0; // additional extraction of memory

	response.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	response.hCursor = LoadCursor(nullptr, IDC_ARROW);
	response.hIcon = response.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	response.hInstance = hInstance;

	response.lpszClassName = L"ApplicationClass";
	response.lpszMenuName = nullptr;
	response.style = CS_VREDRAW | CS_HREDRAW;

	response.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;

		case WM_SHOWWINDOW:
		{
			MessageBox(hWnd, L"Welcome to WinAPI !\nThe program will launch now !", L"Some message for you", NULL);
		}
		return 0;

		case WM_SIZE:
		{
			const std::size_t new_width{ LOWORD(lParam) };
			const std::size_t new_height{ HIWORD(lParam) };
		}
		return 0;
			
		case WM_COMMAND:
		{
			const auto id_pressed{ LOWORD(wParam) };
			
			switch (id_pressed)
			{
			case kButtonSetSizeId:
				MessageBox(hWnd, kMessageBoxButtonSetSizeClickedText, nullptr, NULL);
				SetWindowPos(
					hWnd, 
					nullptr, 
					kMainWindowX, kMainWindowY, kMainWindowWidth, kMainWindowHeight, 
					SWP_DRAWFRAME
				);
				break;
			}
		}
		return 0;
		}
			
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};

	return response;
}


/*
ABOUT wWinMain:

1. HINSTANCE hInstance = > adress in memory of beginning of executed module
2. HINSTANCE => not used (only in 16bit)
3. PWSTR szCmdLine => arguments
4. int nCmdShow => window show method (minimized or smth like that)

wWinMain => entry point
*/

/*
ABOUT HWND window handle:

pointer to place for information about window (descriptor, kernel object)
*/


/*
Events are driven with window messages.
Everything is a window (button, window, scrollbar...)

*/