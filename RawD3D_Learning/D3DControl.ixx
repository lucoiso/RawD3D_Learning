module;

#include <Windows.h>
#include <string>
#include <functional>

export module D3DControl;

export class D3DControl
{
public:
	D3DControl() = default;
	D3DControl(HWND ParentWindow, const int Id, const std::wstring ClassName, const std::wstring ControlName, const int PosX, const int PosY, const int Width, const int Height, const DWORD Style)
	{
		ControlHandle = CreateWindow(
			ClassName.c_str(),
			ControlName.c_str(),
			Style,
			PosX, PosY,
			Width, Height,
			ParentWindow,
			reinterpret_cast<HMENU>(Id),
			reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(ParentWindow, GWLP_HINSTANCE)),
			nullptr);
	}

	void BindOnClicked(std::function<void(HWND)> InFunction)
	{
		Delegate = InFunction;
	}

	virtual void Click(HWND InstigatorWindow)
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

protected:
	std::function<void(HWND)> Delegate;
	HWND ControlHandle;
};