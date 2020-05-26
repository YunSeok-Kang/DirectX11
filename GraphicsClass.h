#pragma once

#include "windows.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

/// <summary>
/// �ش� Ŭ������ �ý��� Ŭ������ ���� ��ü �� �ϳ�.
/// �� Ŭ���� �ȿ� �׷��� �ڵ���� ��������� ĸ��ȭ�ȴ�.
/// ���� �׷��� ���� ���� ������(���� ��� ȭ�� ������ ���� ���� ���� ������)�� �� ���⿡ ����.
/// </summary>
class GraphicsClass
{
public:
	bool Initalize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();
};

