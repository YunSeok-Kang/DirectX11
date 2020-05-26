#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass&)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initalize()
{
	// Initalize all the keys to being released and not pressed.
	for (int index = 0; index < MAX_KEYS; ++index)
	{
		m_keys[index] = false;
	}
}

void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}


