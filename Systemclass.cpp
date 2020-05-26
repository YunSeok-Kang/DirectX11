#include "Systemclass.h"

// 객체 포인터들을 nullptr로 초기화.
// *중요: 이 객체들의 초기화 실패 시 Shutdown 함수가 나중에 이 객체들을 해제하려고 하며 문제 발생.
// 릴리즈에서는 초기화를 안 하면 빌드에 실패하는 경우도 있음.
Systemclass::Systemclass() : m_Input(nullptr), m_Graphics(nullptr)
{
}

// 비어있는 복사 생성자
Systemclass::Systemclass(const Systemclass&)
{
}

// 비어있는 소멸자
Systemclass::~Systemclass()
{
	/* 소멸자에서 객체의 소멸작업을 하지 않는다.
	 * 대신 나중에 정의할 Shutdown 함수에서 소멸 작업을 모두 한다.

	 * 소멸자가 불린다는 확신이 없어서이다.

	 * ExitThread() 같은 윈도우 함수는 클래스 소멸자를 부르지 않는 것으로 알려져 있다.
	 * 이때문에 메모리 누수가 생길 수도 있다.
	*/
}

/// <summary>
/// 이 애플리케이션의 모든 설정 작업을 진행.
/// * 우선 우리 애플리케이션에서 사용할 윈도우 창을 만드는 InitalizeWindows() 함수를 호출.
/// * 사용자 입력과 화면에 나타날 그래픽을 처리할 입력, 그래픽 오브젝트들도 생성하고 초기화.
/// </summary>
/// <returns></returns>
bool Systemclass::Initalize()
{
	// Initalize the width and height of the screen to zero before sending
	//	the variables into the function.
	int screenWidth(0);
	int screenHeight(0);
	bool result = false;

	// Initalize the windows API.
	InitalizeWindows(screenWidth, screenHeight);

	// Create the input object. This object will be used to handle reading the keyboard input from user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initalize the input object
	m_Input->Initalize();

	// Create the graphics object. This Object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initalize the graphics object.
	result = m_Graphics->Initalize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void Systemclass::Shutdown()
{
	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	// Shutdown the window.
	ShutdownWindows();
}

/// <summary>
/// 루프를 돌면서 애플리케이션이 종료될 때까지 애플리케이션의 작업을 함.
/// 루프를 한 번 돌 때마다 호출하는 Frame 함수에서 애플리케이션을 작업. (기억하고 있어야 할 매우 중요한 개념!)
/// </summary>
void Systemclass::Run()
{
	MSG msg;
	bool done(false);
	bool result(false);

	// Initalize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

/// <summary>
/// 애플리케이션의 모든 작업을 처리.
/// 입력 객체(InputClass)가 사용자가 escape를 눌렀는지 체크
/// 만약 종료하지 않을 시 그 프레임의 그래픽 작업을 할 그래픽 객체의 프레임 함수를 호출
/// </summary>
/// <returns></returns>
bool Systemclass::Frame()
{
	bool result(false);

	// Check if the user pressed escape and wants to exit the application
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

/// <summary>
/// MessageHandler 함수로 시스템 메시지를 전달.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="umsg"></param>
/// <param name="wparam"></param>
/// <param name="lparam"></param>
/// <returns></returns>
LRESULT Systemclass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released sent it to input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of then.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
	return LRESULT();
}

/// <summary>
/// 출력을 위한 윈도우를 만들기위한 코드가 있는 곳.
/// 애플리케이션에서 사용하기 위해 호출 한 함수에게 screenWidth와 screenHeight를 반환.
/// 기본 설정을 이용해 테두리가 없는 평범한 윈도우 창을 만든다.
/// 전역 변수 FULL_SCREEN에 의해 전체화면으로 만들지 작은 창으로 만들지 결정.
/// graphicsclass.h 파일의 FULL_SCREEN 값 선언해서 창 크기 변경 가능.
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
void Systemclass::InitalizeWindows(int&, int&)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine"; // 이건 또 뭐냐

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display setting to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = 0;
		posY = 0;
	}
	else
	{
		// If windowed then set it to 800 x 600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight,
		NULL, NULL, m_hInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
}

/// <summary>
/// 화면의 설정을 다시 정상적으로 되돌리고 윈도우와 관련된 핸들들을 해제.
/// </summary>
void Systemclass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = nullptr;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = nullptr;

	// Release the pointer to this class.
	ApplicationHandle = nullptr;
}

/// <summary>
/// 윈도우가 메시지를 전달하는 함수.
/// InitalizeWindows 함수에서 윈도우를 초기화할 때 이 함수의 이름을 알려줌.
/// SystemClass 내부에 정의된 MessageHandler 함수에 모든 메시지들을 보내기 때문에 이 클래스 파일에 이 함수를 정의함.
/// 이렇게 하면 메시지 보내는 걸 기능적으로 System 클래스에 연결되게 하고 코드를 간결하게 유지할 수 있다.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="umessage"></param>
/// <param name="wparam"></param>
/// <param name="lparam"></param>
/// <returns></returns>
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
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

		// All other message pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}