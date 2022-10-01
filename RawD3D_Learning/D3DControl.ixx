module;

#pragma once
#include <Windows.h>

export module D3DControl;

export import std.core;

export class D3DControl
{
public:
	D3DControl() = delete;

	D3DControl(const std::wstring& InClassName, const HWND& InParentWindow, const unsigned int& InId) 
		: m_ClassName(InClassName), m_Parent(InParentWindow), m_ControlId(InId)
	{
		std::wstringstream OutDbg;
		OutDbg << L"Constructing control with id " << m_ControlId << std::endl;
		OutputDebugStringW(OutDbg.str().c_str());
	};

	D3DControl(const std::wstring& InClassName, const HWND& InParentWindow, const unsigned int& InId, const std::wstring& InControlText, const int& InPosX, const int& InPosY, const unsigned int& InWidth, const unsigned int& InHeight, const DWORD& InStyle) 
		: m_ClassName(InClassName), m_Parent(InParentWindow), m_ControlId(InId), m_ControlText(InControlText), m_PosX(InPosX), m_PosY(InPosY), m_Width(InWidth), m_Height(InHeight), m_Style(InStyle | WS_VISIBLE | WS_CHILD)
	{
		std::wstringstream OutDbg;
		OutDbg << L"Constructing control with id " << m_ControlId << std::endl;
		OutputDebugStringW(OutDbg.str().c_str());
	}

	~D3DControl()
	{
		std::wstringstream OutDbg;
		OutDbg << L"Destructing control with id " << m_ControlId << std::endl;
		OutputDebugStringW(OutDbg.str().c_str());
	}

	bool DestroyControl()
	{
		const bool Output = DestroyWindow(m_ControlHandle);
		
		if (Output)
		{
			this->~D3DControl();
		}
		
		return Output;
	}
	
	const unsigned int GetControlId() const
	{
		return m_ControlId;
	}

	const HWND GetControlHandle() const
	{
		return m_ControlHandle;
	}

	void BindInteraction(const std::function<void(const HWND&)>& InFunction)
	{
		m_Delegate = InFunction;
	}

	virtual void InvokeInteraction(const HWND& InstigatorWindow)
	{
		if (m_Delegate)
		{
			m_Delegate(InstigatorWindow);
		}
		else
		{
			MessageBox(nullptr, L"Delegate not bound!", L"RawD3D_Learning", 0);
		}
	}
	
	void SetControlText(const std::wstring& InText)
	{
		const BOOL Result = SetWindowTextW(m_ControlHandle, InText.c_str());
		
		if (Result)
		{
			m_ControlText = InText;
		}
	}

	void SetPosition(const int& InPosX, const int& InPosY)
	{
		const BOOL Result = MoveWindow(m_ControlHandle, InPosX, InPosY, m_Width, m_Height, true);

		if (Result)
		{
			m_PosX = InPosX;
			m_PosY = InPosY;
		}		
	}

	void SetRelativePosition(const HWND& RelativeTo, const int& InPosX, const int& InPosY)
	{
		WINDOWINFO ControlInfo;
		if (!GetWindowInfo(RelativeTo, &ControlInfo))
		{
			MessageBox(nullptr, L"Failed to get window info!", L"RawD3D_Learning", 0);
			return;
		}

		const int New_PosX = InPosX + ControlInfo.rcClient.left;
		const int New_PosY = InPosY + ControlInfo.rcClient.top;

		const BOOL Result = MoveWindow(m_ControlHandle, New_PosX, New_PosY, m_Width, m_Height, true);

		if (Result)
		{
			m_PosX = New_PosX;
			m_PosY = New_PosY;
		}

		UpdateWindow(m_ControlHandle);
	}

	void SetSize(const unsigned int& InWidth, const unsigned int& InHeight)
	{
		const BOOL Result = MoveWindow(m_ControlHandle, m_PosX, m_PosY, InWidth, InHeight, true);

		if (Result)
		{
			m_Width = InWidth;
			m_Height = InHeight;
		}
	}

	void SetStyle(const DWORD& InStyle)
	{
		m_Style = InStyle;
	}

	void SetParent(const HWND& InParent)
	{
		m_Parent = InParent;
	}
	
	virtual bool InitializeControl()
	{
		m_ControlHandle = CreateWindowEx(WS_EX_NOINHERITLAYOUT,
			m_ClassName.c_str(),
			m_ControlText.c_str(),
			m_Style,
			m_PosX, m_PosY,
			m_Width, m_Height,
			m_Parent,
			reinterpret_cast<HMENU>(m_ControlId),
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(m_Parent, GWLP_HINSTANCE)),
			nullptr);

		if (!m_ControlHandle)
		{
			std::wstringstream OutDbg;
			OutDbg << L"Failed to create control with id " << m_ControlId << ". Error: " << GetLastError() << std::endl;
			OutputDebugStringW(OutDbg.str().c_str());

			this->~D3DControl();

			return false;
		}

		return true;
	}
		
protected:
	const std::wstring m_ClassName = L"D3DControl";

	std::function<void(HWND)> m_Delegate;
	HWND m_ControlHandle;

	unsigned int m_ControlId;
	std::wstring m_ControlText;
	HWND m_Parent;

	unsigned int m_Width;
	unsigned int m_Height;
	int m_PosX; 
	int m_PosY;
	DWORD m_Style;
};

export template<typename T> concept IsD3DControl = std::is_base_of_v<D3DControl, T>;