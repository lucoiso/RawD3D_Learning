module;

#pragma once
#include <Windows.h>
#include <dwmapi.h>
#include <string>
#include <map>
#include <memory>

export module D3DWindow;

import D3DControl;
import D3DButton;

static std::map<int, std::unique_ptr<D3DControl>> Controls;

export class D3DWindow
{
public:
	D3DWindow() = default;

	D3DWindow(const int Width, const int Height, const std::wstring Title, const std::wstring ClassName)
		: m_Width(Width), m_Height(Height), m_Title(Title), m_ClassName(ClassName)
	{
	}

protected:
	~D3DWindow() = default;

private:
	int m_Height = 800;
	int m_Width = 800;
	std::wstring m_Title = L"RawD3D_Learning: Window";
	std::wstring m_ClassName = L"RawD3D_Learning: Class";

	HWND m_WindowHandle = nullptr;

public:
	virtual HRESULT RegisterBaseWindowClass(HINSTANCE WindowInstance)
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

	virtual HRESULT CreateBaseWindow(HINSTANCE WindowInstance, int ShowCommand)
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

		auto SetDWMAttribute = [](HWND& WindowHandle, const DWMWINDOWATTRIBUTE Attribute, const auto Value)
		{
			DwmSetWindowAttribute(WindowHandle, Attribute, &Value, sizeof(Value));
		};

		const BOOL bEnable = true;
		SetDWMAttribute(m_WindowHandle, DWMWA_USE_HOSTBACKDROPBRUSH, bEnable);
		SetDWMAttribute(m_WindowHandle, DWMWA_USE_IMMERSIVE_DARK_MODE, bEnable);

		DWM_BLURBEHIND BlurProperties
		{
			.dwFlags = DWM_BB_ENABLE,
			.fEnable = true,
			.hRgnBlur = nullptr,
			.fTransitionOnMaximized = true
		};
		DwmEnableBlurBehindWindow(m_WindowHandle, &BlurProperties);

		if (auto NewButton = CreateNewControl<D3DButton>(5, 5, 128, 32, L"Testing Button"))
		{
			NewButton->BindOnClicked([](HWND WindowHandle)
				{
					MessageBox(nullptr, L"Button Clicked!", L"RawD3D_Learning", 0);
				});
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

	template<class ControlTy>
	ControlTy* CreateNewControl(const int PosX, const int PosY, const int Width, const int Height, const std::wstring ControlText)
	{
		auto _NewControl = new ControlTy(m_WindowHandle, 1, ControlText.c_str(), PosX, PosY, Width, Height);
		Controls.insert(std::pair<int, std::unique_ptr<D3DControl>>(1, _NewControl));

		return _NewControl;
	}

	static LRESULT WindowProcessing(HWND WindowHandle, UINT Message, WPARAM Parameter, LPARAM LongParameter)
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
			DrawText(_PaintStruct.hdc, _OutputText.c_str(), static_cast<int>(_OutputText.length()), &_PaintStruct.rcPaint,
				DT_CENTER);

			EndPaint(WindowHandle, &_PaintStruct);
			break;
		}

		case WM_COMMAND:
		{
			if (HIWORD(Parameter) == BN_CLICKED)
			{
				Controls[LOWORD(Parameter)]->Click(WindowHandle);
			}
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
};