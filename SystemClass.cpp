#include "SystemClass.h"

#include <map>
#include <ios>
#include <ostream>
#include <iostream>
#include <strsafe.h>

void ErrorExit(LPCTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	std::cout << std::hex << "[WndProc] hwnd: " << hwnd << "; umsg: " << umessage << "; wp: " << wparam << "; lp: " << lparam << std::dec << std::endl;
	switch (umessage)
	{
	case WM_NCCREATE:
	{
		CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lparam;
		SystemClass* pSystemClass = (SystemClass*)(pCreateStruct->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pSystemClass);
		return pSystemClass->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	
	// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		SystemClass* pSystemClass = (SystemClass*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pSystemClass != 0)
			return pSystemClass->MessageHandler(hwnd, umessage, wparam, lparam);
		else
			return 0;
	}
	}
}

SystemClass::SystemClass(const ConfigClass& config) : m_applicationName(L"Engine"), m_config(config), m_hinstance((HINSTANCE)INVALID_HANDLE_VALUE), m_hwnd((HWND)INVALID_HANDLE_VALUE), m_pGraphics(NULL)
{
}

SystemClass::~SystemClass()
{
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) {
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_pGraphics = new GraphicsClass(m_config);
	if (!m_pGraphics->Initialize(screenWidth, screenHeight, m_hwnd))
		return false;

	return true;
}

void SystemClass::Shutdown()
{
	// Release the graphics object.
	if (m_pGraphics)
	{
		m_pGraphics->Shutdown();
		delete m_pGraphics;
		m_pGraphics = NULL;
	}

	// Shutdown the window.
	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	while (true)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT || !Frame())
			break;
	}

	return;
}

bool SystemClass::Frame()
{
	if (!m_pGraphics->Frame())
		return false;

	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	int displayWidth = GetSystemMetrics(SM_CXSCREEN);
	int displayHeight = GetSystemMetrics(SM_CYSCREEN);

	screenWidth = displayWidth >> 1;
	screenHeight = displayHeight >> 1;

	// Place the window in the middle of the screen.
	posX = (displayWidth - screenWidth) / 2;
	posY = (displayHeight - screenHeight) / 2;

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, (LPVOID)this);
	if (m_hwnd == NULL) {
		ErrorExit(TEXT("CreateWindowEx"));
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	return;
}

