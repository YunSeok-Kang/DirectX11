#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initalize(int screenWidth, int screenHeight, HWND hwnd)
{
    return true;
}

void GraphicsClass::Shutdown()
{
}

bool GraphicsClass::Frame()
{
    return true;
}

bool GraphicsClass::Render()
{
    return true;
}