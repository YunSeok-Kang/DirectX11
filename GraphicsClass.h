#pragma once

#include "windows.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

/// <summary>
/// 해당 클래스는 시스템 클래스가 만든 객체 중 하나.
/// 이 클래스 안에 그래픽 코드들이 기능적으로 캡슐화된다.
/// 또한 그래픽 관련 전역 변수들(예를 들어 화면 사이즈 변수 등의 전역 변수들)을 다 여기에 넣음.
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

