#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "GraphicsClass.h"
#include "ConfigClass.h"

class SystemClass
{
public:
	SystemClass(const ConfigClass& config);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	bool InitializeWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

private:
	const ConfigClass& m_config;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	GraphicsClass* m_pGraphics;
};

