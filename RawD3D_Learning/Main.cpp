import D3DWindow;

#include <Windows.h>

int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, const LPSTR lpCmdLine, const int nShowCmd)
{
	const auto MainWindow = new D3DWindow();
	MainWindow->RegisterBaseWindowClass(hInstance);

	return MainWindow->CreateBaseWindow(hInstance, nShowCmd);
}
