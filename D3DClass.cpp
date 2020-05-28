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
/// DirectX 11�� Direct3D�� ���� ������ �ش� �Լ����� ����
/// </summary>
/// <param name="screenWidth"> SystemClass���� ������ â�� �ʺ� </param>
/// <param name="screenHeight"> SystemClass���� ������ â�� ���� </param>
/// <param name="vsync"></param>
/// <param name="hwnd"> â�� �ڵ�. Direct3D�� ������ ������� â�� �����ϱ� ���� �ʿ�. </param>
/// <param name="fullscreen"> â ��� or ��üȭ��. 3D ȯ�濡�� â�� ����� �� �ʿ��� ��(?)</param>
/// <param name="screenDepth"> 3D ȯ�濡�� â�� ����� �� �ʿ��� ��. </param>
/// <param name="screenNear"> 3D ȯ�濡�� â�� ����� �� �ʿ��� ��. </param>
/// <returns></returns>
bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	// Store the vsync setting.
	m_vsync_enabled = vsync;

	/* 2. ���� ī�� / ����� �ֻ��� ���ϱ� */
	/* 
		Direct3D�� �����ϱ� ���ؼ��� ���� ī�� / ������� �ֻ����� ���ؾ��Ѵ�. �� ��ǻ�͸��� ���� �ٸ� �� �־ �� ���� �˾Ƴ��� �Ѵ�.
		���ڿ� �и��� �˾Ƴ� �� DirecX�� �Ѱ� ������ �� ��Ȯ�� �ֻ����� ���Ѵ�.
		���� �� ������ ��ġ�� �ʰ� ����Ʈ ������ �ֻ����� �����ϸ�, �ֻ��� ���� �������� �ʴ� ��ǻ�Ϳ��� buffer flip ��ſ� blit ������� �����ؼ�
		���� ���Ͽ� ����� ��¿��� ������ ����.
	*/

	// Create a DirectX graphics interface factory.
	IDXGIFactory* factory;
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory); // (void**)���� ��ĩ�޴�. C++ ���ʸ� ������� ����.
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
	displayModeList = new DXGI_MODE_DESC[numModes]; // new ��ü[0] �ϸ� null ��ȯ?
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
	// When a match is found store the numerator and denominator of the refresh rate for that monitor. ����, �и�
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