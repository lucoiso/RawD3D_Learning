module;

#pragma once
#include <Windows.h>
#include <dwmapi.h>
#include <type_traits>

export module D3DWindow;

import std.memory;

import D3DButton;

static std::map<unsigned int, std::unique_ptr<D3DControl>> Controls;

export class D3DWindow : public D3DControl
{
public:
	D3DWindow() = delete;

	D3DWindow(const unsigned int& InWidth, const unsigned int& InHeight, const std::wstring& InWindowTitle)
		: D3DControl(L"D3DWindow", nullptr, 0u, InWindowTitle, 0, 0, InWidth, InHeight, WS_OVERLAPPEDWINDOW)
	{
	}

protected:
	~D3DWindow() = default;

	virtual bool InitializeControl() override
	{
		OutputDebugStringW(L"Window control doesn't use this class to initialize because we need the instance to register. Look for RegisterBaseWindowClass & CreateBaseWindow instead.");
		return false;
	}

public:
	virtual HRESULT RegisterBaseWindowClass(const HINSTANCE& InWindowInstance)
	{
		const HICON hIcon = LoadIcon(InWindowInstance, IDI_APPLICATION);

		const WNDCLASSEX WindowClass
		{
			.cbSize = sizeof(WNDCLASSEX),
			.style = CS_DBLCLKS,
			.lpfnWndProc = WindowProcessing,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = InWindowInstance,
			.hIcon = hIcon,
			.hCursor = LoadCursor(nullptr, IDC_ARROW),
			.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
			.lpszMenuName = nullptr,
			.lpszClassName = m_ClassName.c_str(),
			.hIconSm = hIcon
		};

		if (!RegisterClassEx(&WindowClass))
		{
			MessageBox(nullptr, L"Class Registration Failed!", L"RawD3D_Learning", 0);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		return 0;
	}

	virtual HRESULT CreateBaseWindow(const HINSTANCE& InWindowInstance, const int& InShowCommand)
	{
		m_ControlHandle = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,
			m_ClassName.c_str(),
			m_ControlText.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_Width,
			m_Height,
			nullptr,
			nullptr,
			InWindowInstance,
			nullptr);

		if (!m_ControlHandle)
		{
			MessageBox(nullptr, L"Window Creation Failed!", L"RawD3D_Learning", 0);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		const auto SetDWMAttribute = [](HWND& InWindowHandle, const DWMWINDOWATTRIBUTE& InAttribute, const auto& InValue)
		{
			DwmSetWindowAttribute(InWindowHandle, InAttribute, &InValue, sizeof(InValue));
		};

		const BOOL bEnable = true;
		SetDWMAttribute(m_ControlHandle, DWMWA_USE_HOSTBACKDROPBRUSH, bEnable);
		SetDWMAttribute(m_ControlHandle, DWMWA_USE_IMMERSIVE_DARK_MODE, bEnable);
		SetDWMAttribute(m_ControlHandle, DWMWA_SYSTEMBACKDROP_TYPE, DWM_SYSTEMBACKDROP_TYPE::DWMSBT_AUTO);

		if (auto NewButton = CreateNewControl<D3DButton>(4, 4, 128, 32, L"Open dialog"))
		{
			NewButton->BindInteraction([](const HWND& InWindowHandle)
				{
					MessageBox(InWindowHandle, L"Button Clicked!", L"RawD3D_Learning", 0);
				});
		}

		if (auto NewButton = CreateNewControl<D3DButton>(4, 36, 216, 32, L"Destroy the other button"))
		{
			if (Controls[1u])
			{
				NewButton->BindInteraction([&](const HWND& InWindowHandle)
					{
						if (Controls[1u] && Controls[1u]->DestroyControl())
						{
							MessageBox(InWindowHandle, L"Control destroyed! :)", L"RawD3D_Learning", 0);
							Controls.erase(1u);

							NewButton->SetPosition(5, 5);
						}
						else
						{
							MessageBox(InWindowHandle, L"Control already destroyed! :)", L"RawD3D_Learning", 0);
						}
					});
			}
			else
			{
				MessageBox(m_ControlHandle, L"Invalid control: Id 1", L"RawD3D_Learning", 0);
			}
		}

		ShowWindow(m_ControlHandle, InShowCommand);
		UpdateWindow(m_ControlHandle);

		MSG Message
		{
			.message = WM_NULL
		};

		while (GetMessage(&Message, nullptr, 0, 0))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		return static_cast<HRESULT>(Message.wParam);
	}

	template<class ControlTy> requires IsD3DControl<ControlTy>
	constexpr ControlTy* CreateNewControl(const int& InPosX, const int& InPosY, const unsigned int& InWidth, const unsigned int& InHeight, const std::wstring& InControlText)
	{
		const unsigned int ControlID = static_cast<unsigned int>(Controls.size() + 1u);
		auto NewControl = new ControlTy(m_ControlHandle, ControlID, InControlText.c_str(), InPosX, InPosY, InWidth, InHeight);

		if (NewControl->InitializeControl())
		{
			Controls.insert(std::make_pair(ControlID, NewControl));
			return NewControl;
		}

		return nullptr;
	}

	static LRESULT WindowProcessing(HWND InWindowHandle, UINT InMessage, WPARAM InParameter, LPARAM InLongParameter)
	{
		switch (InMessage)
		{
		case WM_PAINT:
		{
			// Update renderer
			
			break;
		}

		case WM_COMMAND:
		{
			if (HIWORD(InParameter) == BN_CLICKED)
			{
				Controls[LOWORD(InParameter)]->InvokeInteraction(InWindowHandle);
			}
			break;
		}

		case WM_CLOSE:
		{
			if (MessageBox(InWindowHandle, L"End application?", L"RawD3D_Learning", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(InWindowHandle);
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

		return DefWindowProc(InWindowHandle, InMessage, InParameter, InLongParameter);
	}
};