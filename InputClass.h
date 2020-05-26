#pragma once

const unsigned int MAX_KEYS = 256;

/// <summary>
/// 사용자의 키보드 입력을 처리하는 클래스.
/// SystemClass:MessageHandler 함수에서 입력을 받는다.
/// 입력 객체는 키보드의 모든 키에 대한 상태를 가지고 있다(?)
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

