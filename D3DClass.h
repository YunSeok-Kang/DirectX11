#pragma once

/* LINKING ��ũ�� ���̺귯�� ��� */
// DirectX 11���� 3D �׷����� �����ϰ� �׸��� �Ϳ� ���� Direct3D ����� ������ ����.
#pragma comment(lib, "d3d11.lib")

// ��ǻ���� �ϵ��� ������ ������� �ֻ����̳� ����ϴ� ���� ī�� � ���� ������ ���� �� �ִ� ��.
#pragma comment(lib, "dxgi.lib")

// ���̴��� �������ϴ� ��ɵ� ����.
#pragma comment(lib, "d3dcompiler.lib")

#include "d3d11.h"
#include "directxmath.h"
using namespace DirectX;

/// <summary>
/// Direct3D �ý����� ��ɵ��� �ٷ� Ŭ����.
/// ��� �׷��� ���� Ŭ������ GraphicClass �ȿ� ĸ��ȭ�� ���̱⿡, GraphicsClass���� ������ ��ġ.
/// </summary>
class D3DClass
{
public:
	// Initalize, Shutdown �Լ��� ������. 
	// DirectX �ʱ�ȭ�� �����ϴ� �������� ���������� ������ �༮����.
	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D10RasterizerState* m_rasterState;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	// View Matrix�� ���� ���� -> ���߿� ī�޶� Ŭ���� �ȿ� ���� ���̱� ����.

public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();
};

