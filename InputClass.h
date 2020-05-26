#pragma once

const unsigned int MAX_KEYS = 256;

/// <summary>
/// ������� Ű���� �Է��� ó���ϴ� Ŭ����.
/// SystemClass:MessageHandler �Լ����� �Է��� �޴´�.
/// �Է� ��ü�� Ű������ ��� Ű�� ���� ���¸� ������ �ִ�(?)
/// </summary>
class InputClass
{
public:
	void Initalize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[MAX_KEYS];

public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();
};

