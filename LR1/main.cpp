#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <utility>


WNDCLASSEX configure_window_characteristics(const HINSTANCE&);
HWND configure_handle(const WNDCLASSEX&, const std::pair<int, int>&, const std::pair<int, int>&, const LPCWSTR&);


int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	WNDCLASSEX window_characteristics{ configure_window_characteristics(hInstance) };

	if (!RegisterClassEx(&window_characteristics)) 
		return EXIT_FAILURE;

	const HWND window_handle{ configure_handle(window_characteristics, {100, 100}, {500, 500}, L"First window!")};

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


WNDCLASSEX configure_window_characteristics(const HINSTANCE &hInstance)
{
	WNDCLASSEX response{ sizeof(WNDCLASSEX) };

	response.cbClsExtra = response.cbWndExtra = 0; // additional extraction of memory

	response.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH));
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
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};

	return response;
}

HWND configure_handle(const WNDCLASSEX& characteristics, 
	const std::pair<int, int> &coordinates, 
	const std::pair<int, int> &dimensions, 
	const LPCWSTR &title)
{
	return CreateWindow(
		characteristics.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		coordinates.first,
		coordinates.second,
		dimensions.first,
		dimensions.second,
		nullptr,
		nullptr,
		characteristics.hInstance,
		nullptr
	);
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