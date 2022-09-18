module;

#pragma once
#include <Windows.h>

export module D3DButton;

export import D3DControl;

export class D3DButton : public D3DControl
{
public:
	D3DButton() = delete;
		
	D3DButton(const HWND& InParentWindow, const int& InId, const std::wstring& InButtonText, const int& InPosX, const int& InPosY, const int& InWidth, const int& InHeight)
		: D3DControl(L"Button", InParentWindow, InId, InButtonText, InPosX, InPosY, InWidth, InHeight, BS_PUSHBUTTON)
	{
	}

	D3DButton(const HWND& InParentWindow, const int& InId) : D3DButton(InParentWindow, InId, std::wstring(), 0, 0, 0, 0)
	{
	}
};