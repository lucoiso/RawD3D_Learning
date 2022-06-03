#pragma once

#include <string>
#include <Windows.h>

class BaseWindow
{
public:
	BaseWindow() = default;

	BaseWindow(const int Height, const int Width, const std::wstring Title, const std::wstring ClassName)
		: m_Height(Height), m_Width(Width), m_Title(Title), m_ClassName(ClassName)
	{
	}

	virtual HRESULT RegisterBaseWindowClass(HINSTANCE WindowInstance);
	virtual HRESULT CreateBaseWindow(HINSTANCE WindowInstance, int ShowCommand);

	static HWND GetWindowHandle() { return m_WindowHandle; }

	static LRESULT WindowProcessing(HWND WindowHandle, UINT Message, WPARAM Parameter, LPARAM LongParameter);

protected:
	~BaseWindow() = default;

private:
	int m_Height = 800;
	int m_Width = 800;
	std::wstring m_Title = L"RawD3D_Learning: Window";
	std::wstring m_ClassName = L"RawD3D_Learning: Class";

	static HWND m_WindowHandle;
};
