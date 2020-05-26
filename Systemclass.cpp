#include "Systemclass.h"

// ��ü �����͵��� nullptr�� �ʱ�ȭ.
// *�߿�: �� ��ü���� �ʱ�ȭ ���� �� Shutdown �Լ��� ���߿� �� ��ü���� �����Ϸ��� �ϸ� ���� �߻�.
// ��������� �ʱ�ȭ�� �� �ϸ� ���忡 �����ϴ� ��쵵 ����.
Systemclass::Systemclass() : m_Input(nullptr), m_Graphics(nullptr)
{
}

// ����ִ� ���� ������
Systemclass::Systemclass(const Systemclass&)
{
}

// ����ִ� �Ҹ���
Systemclass::~Systemclass()
{
	/* �Ҹ��ڿ��� ��ü�� �Ҹ��۾��� ���� �ʴ´�.
	 * ��� ���߿� ������ Shutdown �Լ����� �Ҹ� �۾��� ��� �Ѵ�.

	 * �Ҹ��ڰ� �Ҹ��ٴ� Ȯ���� ����̴�.

	 * ExitThread() ���� ������ �Լ��� Ŭ���� �Ҹ��ڸ� �θ��� �ʴ� ������ �˷��� �ִ�.
	 * �̶����� �޸� ������ ���� ���� �ִ�.
	*/
}

/// <summary>
/// �� ���ø����̼��� ��� ���� �۾��� ����.
/// * �켱 �츮 ���ø����̼ǿ��� ����� ������ â�� ����� InitalizeWindows() �Լ��� ȣ��.
/// * ����� �Է°� ȭ�鿡 ��Ÿ�� �׷����� ó���� �Է�, �׷��� ������Ʈ�鵵 �����ϰ� �ʱ�ȭ.
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
/// ������ ���鼭 ���ø����̼��� ����� ������ ���ø����̼��� �۾��� ��.
/// ������ �� �� �� ������ ȣ���ϴ� Frame �Լ����� ���ø����̼��� �۾�. (����ϰ� �־�� �� �ſ� �߿��� ����!)
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
/// ���ø����̼��� ��� �۾��� ó��.
/// �Է� ��ü(InputClass)�� ����ڰ� escape�� �������� üũ
/// ���� �������� ���� �� �� �������� �׷��� �۾��� �� �׷��� ��ü�� ������ �Լ��� ȣ��
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
/// MessageHandler �Լ��� �ý��� �޽����� ����.
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
/// ����� ���� �����츦 ��������� �ڵ尡 �ִ� ��.
/// ���ø����̼ǿ��� ����ϱ� ���� ȣ�� �� �Լ����� screenWidth�� screenHeight�� ��ȯ.
/// �⺻ ������ �̿��� �׵θ��� ���� ����� ������ â�� �����.
/// ���� ���� FULL_SCREEN�� ���� ��üȭ������ ������ ���� â���� ������ ����.
/// graphicsclass.h ������ FULL_SCREEN �� �����ؼ� â ũ�� ���� ����.
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
	m_applicationName = L"Engine"; // �̰� �� ����

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
/// ȭ���� ������ �ٽ� ���������� �ǵ����� ������� ���õ� �ڵ���� ����.
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
/// �����찡 �޽����� �����ϴ� �Լ�.
/// InitalizeWindows �Լ����� �����츦 �ʱ�ȭ�� �� �� �Լ��� �̸��� �˷���.
/// SystemClass ���ο� ���ǵ� MessageHandler �Լ��� ��� �޽������� ������ ������ �� Ŭ���� ���Ͽ� �� �Լ��� ������.
/// �̷��� �ϸ� �޽��� ������ �� ��������� System Ŭ������ ����ǰ� �ϰ� �ڵ带 �����ϰ� ������ �� �ִ�.
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