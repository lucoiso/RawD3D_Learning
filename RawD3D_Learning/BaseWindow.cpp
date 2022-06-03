#include "BaseWindow.h"

HWND BaseWindow::m_WindowHandle = nullptr;

HRESULT BaseWindow::RegisterBaseWindowClass(const HINSTANCE WindowInstance)
{
	const HICON _hIcon = LoadIcon(WindowInstance, IDI_APPLICATION);

	const WNDCLASSEX _WindowClass
	{
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_DBLCLKS,
		.lpfnWndProc = WindowProcessing,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = WindowInstance,
		.hIcon = _hIcon,
		.hCursor = LoadCursor(nullptr, IDC_ARROW),
		.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
		.lpszMenuName = nullptr,
		.lpszClassName = m_ClassName.c_str(),
		.hIconSm = _hIcon
	};

	if (!RegisterClassEx(&_WindowClass))
	{
		MessageBox(nullptr, L"Class Registration Failed!", L"RawD3D_Learning", 0);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return 0;
}

HRESULT BaseWindow::CreateBaseWindow(const HINSTANCE WindowInstance, const int ShowCommand)
{
	m_WindowHandle = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		m_ClassName.c_str(),
		m_Title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_Width,
		m_Height,
		nullptr,
		nullptr,
		WindowInstance,
		nullptr);

	if (!m_WindowHandle)
	{
		MessageBox(nullptr, L"Window Creation Failed!", L"RawD3D_Learning", 0);
		return HRESULT_FROM_WIN32(GetLastError());
	}
	ShowWindow(m_WindowHandle, ShowCommand);
	UpdateWindow(m_WindowHandle);

	MSG _Message
	{
		.message = WM_NULL
	};

	while (GetMessage(&_Message, nullptr, 0, 0))
	{
		TranslateMessage(&_Message);
		DispatchMessage(&_Message);
	}

	return static_cast<HRESULT>(_Message.wParam);
}

LRESULT BaseWindow::WindowProcessing(const HWND WindowHandle, const UINT Message, const WPARAM Parameter,
                                     const LPARAM LongParameter)
{
	switch (Message)
	{
	case WM_PAINT:
		{
			// Update renderer
			PAINTSTRUCT _PaintStruct;
			BeginPaint(WindowHandle, &_PaintStruct);

			FillRect(_PaintStruct.hdc, &_PaintStruct.rcPaint, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
			const std::wstring _OutputText = L"Work in Progress";
			DrawText(_PaintStruct.hdc, _OutputText.c_str(), static_cast<int>(_OutputText.size()), &_PaintStruct.rcPaint,
			         DT_CENTER);

			EndPaint(WindowHandle, &_PaintStruct);
			break;
		}

	case WM_CLOSE:
		{
			if (MessageBox(WindowHandle, L"End application?", L"RawD3D_Learning", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(WindowHandle);
				return 0;
			}
			break;
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

	default: break;
	}

	return DefWindowProc(WindowHandle, Message, Parameter, LongParameter);
}
