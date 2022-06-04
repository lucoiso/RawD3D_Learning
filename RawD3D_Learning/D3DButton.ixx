module;

#pragma once
#include <Windows.h>

export module D3DButton;

import D3DControl;
import std.core;

export class D3DButton : public D3DControl
{
public:
	D3DButton() = delete;
	D3DButton(HWND ParentWindow, const int Id, const std::wstring ButtonText, const int PosX, const int PosY, const int Width, const int Height)
		: D3DControl(ParentWindow, Id, L"Button", ButtonText, PosX, PosY, Width, Height, BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD)
	{
	}
};