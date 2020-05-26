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


