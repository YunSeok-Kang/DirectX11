#pragma once

/* LINKING 링크할 라이브러리 명시 */
// DirectX 11에서 3D 그래픽을 설정하고 그리는 것에 대한 Direct3D 기능을 가지고 있음.
#pragma comment(lib, "d3d11.lib")

// 컴퓨터의 하드웨어를 조작해 모니터의 주사율이나 사용하는 비디오 카드 등에 대한 정보를 얻을 수 있는 툴.
#pragma comment(lib, "dxgi.lib")

// 셰이더를 컴파일하는 기능들 포함.
#pragma comment(lib, "d3dcompiler.lib")

#include "d3d11.h"
#include "directxmath.h"
using namespace DirectX;

/// <summary>
/// Direct3D 시스템의 기능들을 다룰 클래스.
/// 모든 그래픽 관련 클래스는 GraphicClass 안에 캡슐화될 것이기에, GraphicsClass안이 최적의 위치.
/// </summary>
class D3DClass
{
public:
	// Initalize, Shutdown 함수를 가진다. 
	// DirectX 초기화를 공부하는 과정에서 집중적으로 공부할 녀석들임.
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
	// View Matrix가 없는 이유 -> 나중에 카메라 클래스 안에 넣을 것이기 때문.

public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();
};

