module;
#include <Windows.h>
export module BaseControl;

export class BaseControl
{
public:
	BaseControl() = default;
	virtual void Click(HWND InstigatorWindow) = 0;
};