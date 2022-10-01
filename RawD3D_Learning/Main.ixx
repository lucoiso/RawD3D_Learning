module;

#pragma once
#include <Windows.h>

export module Main;

import D3DWindow;

int WINAPI WinMain(const HINSTANCE hInstance, [[maybe_unused]] const HINSTANCE hPrevInstance, [[maybe_unused]] const LPSTR lpCmdLine, const int nShowCmd)
{
	const auto MainWindow = new D3DWindow(800u, 600u, L"D3D Learning Window");
	MainWindow->RegisterBaseWindowClass(hInstance);

	return MainWindow->CreateBaseWindow(hInstance, nShowCmd);
}