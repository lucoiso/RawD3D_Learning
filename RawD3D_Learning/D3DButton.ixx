module;

#pragma once
#include <Windows.h>
#include <string>
#include <functional>

export module D3DButton;
import BaseControl;

export class D3DButton : public BaseControl
{
public:
	D3DButton() = delete;
	D3DButton(HWND ParentWindow, const int Id, const std::wstring ButtonText, const int PosX, const int PosY, const int Width, const int Height)
	{
		ButtonHandle = CreateWindow(
			L"Button", ButtonText.c_str(),
			BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD,
			PosX, PosY, Width, Height,
			ParentWindow,
			reinterpret_cast<HMENU>(Id),
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(ParentWindow, GWLP_HINSTANCE)),
			nullptr);
	}

	void BindOnClicked(std::function<void(HWND)> InFunction)
	{
		Delegate = InFunction;
	}

	virtual void Click(HWND InstigatorWindow) override
	{
		if (Delegate)
		{
			Delegate(InstigatorWindow);
		}
		else
		{
			MessageBox(nullptr, L"Delegate not bound!", L"RawD3D_Learning", 0);
		}
	}

private:
	std::function<void(HWND)> Delegate;
	HWND ButtonHandle;
};