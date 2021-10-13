#include "framework.h"
#include "DeviceMgr.h"

IMPLEMENT_SINGLETON(DeviceMgr)

DeviceMgr::DeviceMgr(void)
{
}

DeviceMgr::~DeviceMgr(void)
{
	Free();
}

HRESULT DeviceMgr::Ready_GraphicDev(HWND hWnd, WINMODE eMode, const UINT& iSizeX, const UINT& iSizeY, LPDIRECT3DDEVICE9* ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pSDK)
		return E_FAIL;

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
		return E_FAIL;

	ULONG		uFlag = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		uFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		uFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;


	D3DPRESENT_PARAMETERS			Present_Parameter;
	ZeroMemory(&Present_Parameter, sizeof(D3DPRESENT_PARAMETERS));

	Present_Parameter.BackBufferWidth = iSizeX;
	Present_Parameter.BackBufferHeight = iSizeY;
	Present_Parameter.BackBufferFormat = D3DFMT_A8R8G8B8;
	Present_Parameter.BackBufferCount = 1;

	Present_Parameter.MultiSampleType = D3DMULTISAMPLE_NONE;
	Present_Parameter.MultiSampleQuality = 0;

	Present_Parameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Present_Parameter.hDeviceWindow = hWnd;

	Present_Parameter.Windowed = eMode;
	Present_Parameter.EnableAutoDepthStencil = TRUE;
	Present_Parameter.AutoDepthStencilFormat = D3DFMT_D24S8;

	Present_Parameter.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	Present_Parameter.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, uFlag, &Present_Parameter, &m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
		return E_FAIL;

	if (nullptr != ppGraphicDev)
		*ppGraphicDev = m_pGraphicDev;

	return NOERROR;
}

void DeviceMgr::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		Color,
		1.f,
		0);

	m_pGraphicDev->BeginScene();
}

void DeviceMgr::Render_End(void)
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
}

void DeviceMgr::Free(void)
{
	SafeRelease(m_pGraphicDev);
	SafeRelease(m_pSDK);
}
