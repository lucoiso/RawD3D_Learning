module;

#pragma once
#include <Windows.h>

export module Main;

import D3DWindow;

int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, const LPSTR lpCmdLine, const int nShowCmd)
{
	const auto MainWindow = new D3DWindow();
	MainWindow->RegisterBaseWindowClass(hInstance);

	return MainWindow->CreateBaseWindow(hInstance, nShowCmd);
}