#include "BaseWindow.h"

int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, const LPSTR lpCmdLine, const int nShowCmd)
{
	const auto MainWindow = new BaseWindow();
	MainWindow->RegisterBaseWindowClass(hInstance);

	return MainWindow->CreateBaseWindow(hInstance, nShowCmd);
}
