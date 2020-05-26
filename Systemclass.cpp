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

void Systemclass::InitalizeWindows(int&, int&)
{
}

void Systemclass::ShutdownWindows()
{
}


