#include "D3DClass.h"

D3DClass::D3DClass() : m_vsync_enabled(false), m_videoCardMemory(0), m_swapChain(nullptr), m_device(nullptr), m_deviceContext(nullptr),
m_renderTargetView(nullptr), m_depthStencilBuffer(nullptr), m_depthStencilState(nullptr), m_depthStencilView(nullptr), m_rasterState(nullptr)
{
	memset(m_videoCardDescription, 0, sizeof(m_videoCardDescription));
}

D3DClass::D3DClass(const D3DClass&)
{
}

D3DClass::~D3DClass()
{
}

/// <summary>
/// DirectX 11의 Direct3D에 대한 설정을 해당 함수에서 진행
/// </summary>
/// <param name="screenWidth"> SystemClass에서 생성한 창의 너비 </param>
/// <param name="screenHeight"> SystemClass에서 생성한 창의 높이 </param>
/// <param name="vsync"></param>
/// <param name="hwnd"> 창의 핸들. Direct3D가 이전에 만들었던 창에 접근하기 위해 필요. </param>
/// <param name="fullscreen"> 창 모드 or 전체화면. 3D 환경에서 창에 출력할 때 필요한 값(?)</param>
/// <param name="screenDepth"> 3D 환경에서 창에 출력할 때 필요한 값. </param>
/// <param name="screenNear"> 3D 환경에서 창에 출력할 때 필요한 값. </param>
/// <returns></returns>
bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	// Store the vsync setting.
	m_vsync_enabled = vsync;

	/* 2. 비디오 카드 / 모니터 주사율 구하기 */
	/* 
		Direct3D를 설정하기 위해서는 비디오 카드 / 모니터의 주사율을 구해야한다. 각 컴퓨터마다 값이 다를 수 있어서 그 값을 알아내야 한다.
		분자와 분모값을 알아낸 뒤 DirecX에 넘겨 설정할 때 정확한 주사율을 구한다.
		만약 이 과정을 거치지 않고 디폴트 값으로 주사율을 설정하면, 주사율 값이 존재하지 않는 컴퓨터에선 buffer flip 대신에 blit 방식으로 실행해서
		성능 저하와 디버그 출력에서 에러가 난다.
	*/

	// Create a DirectX graphics interface factory.
	IDXGIFactory* factory;
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory); // (void**)에서 멈칫햇다. C++ 기초를 까먹으면 쓰나.
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	IDXGIAdapter* adapter;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (moniter).
	IDXGIOutput* adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNIFORM display format for the adapter output (moniter).
	unsigned int numModes;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this moniter / video card combination.
	DXGI_MODE_DESC* displayModeList = nullptr;
	displayModeList = new DXGI_MODE_DESC[numModes]; // new 객체[0] 하면 null 반환?
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor. 분자, 분모
	unsigned int numerator, denominator;
	for (int index = 0; index < numModes; ++index)
	{
		if (displayModeList[index].Width == (unsigned int)screenWidth && displayModeList[index].Height == (unsigned int)screenHeight)
		{
			numerator = displayModeList[index].RefreshRate.Numerator;
			denominator = displayModeList[index].RefreshRate.Denominator;
		}
	}

	return false;
}