#pragma once

// 이게 뭔데? framework.h 내에도 있음.
#define WIN32_LEAN_AND_MEAN

#include "windows.h"

// MY CLASS INCLUDES
#include "InputClass.h"
#include "GraphicsClass.h"

class Systemclass
{
public:
	bool Initalize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitalizeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;

public:
	Systemclass();
	Systemclass(const Systemclass&);
	~Systemclass();
};

// FUNCTION PROTOTYPES
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS
static Systemclass* ApplicationHandle = 0;

