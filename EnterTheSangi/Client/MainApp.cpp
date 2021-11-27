#include "framework.h"
#include "MainApp.h"

#include "DeviceMgr.h"
#include "GameMgr.h"
#include "Renderer.h"
#include "ShaderMgr.h"
#include "NetworkMgr.h"
#include "ResourceMgr.h"

#include "Scene_Loading.h"

#include "Texture.h"

MainApp::MainApp()
{
}

MainApp::~MainApp()
{
	Free();
}

HRESULT MainApp::Ready_MainApp()
{
	// 장치 초기화
	if (FAILED(DeviceMgr::GetInstance()->Ready_GraphicDev(g_hWnd, OPTION::MODE_WIN, g_nWinCX, g_nWinCY, &m_pGraphic_Device)))
		return E_FAIL;

	// 쉐이더 로드
	m_pShaderMgr = ShaderMgr::GetInstance();
	if (!m_pShaderMgr) return E_FAIL;

	if (FAILED(m_pShaderMgr->Add_Shader(m_pGraphic_Device, L"Player", L"../Binary/Shaders/Player.hlsl")))
		return E_FAIL;

	if (FAILED(m_pShaderMgr->Add_Shader(m_pGraphic_Device, L"Blend", L"../Binary/Shaders/Blend.hlsl")))
		return E_FAIL;

	if (FAILED(m_pShaderMgr->Add_Shader(m_pGraphic_Device, L"Default", L"../Binary/Shaders/Default.hlsl")))
		return E_FAIL;

	if (FAILED(m_pShaderMgr->Add_Shader(m_pGraphic_Device, L"Font", L"../Binary/Shaders/Font.hlsl")))
		return E_FAIL;

	// 매니저 초기화
	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	m_pRenderer = Renderer::GetInstance();
	if (FAILED(m_pRenderer->Ready_Renderer(m_pGraphic_Device)))
		return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (FAILED(m_pResourceMgr->Ready_ResourceMgr(m_pGraphic_Device, DeviceMgr::GetInstance()->Get_Sprite())))
		return E_FAIL;

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (FAILED(m_pNetworkMgr->Ready_WinSock()))
		int a = 0;
		//return E_FAIL;

	// 로딩용 이미지 로드
	if (FAILED(m_pResourceMgr->Add_Texture(L"UI", L"Loading", L"../Binary/Resources/Loading/Loading_%d.png", 17)))
		return E_FAIL;

	// 씬 세팅
	Scene_Loading* pScene = Scene_Loading::Create(m_pGraphic_Device);
	if (FAILED(m_pGameMgr->Set_CurrScene(pScene)))
		return E_FAIL;

	return NOERROR;
}

int MainApp::Update_MainApp(const float TimeDelta)
{
	return m_pGameMgr->Update_GameMgr(TimeDelta);
}

HRESULT MainApp::Render_MainApp()
{
	if (!m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 1.f, 0);
	m_pGraphic_Device->BeginScene();

	if (FAILED(m_pRenderer->Render_Objects()))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Render_GameMgr()))
		return E_FAIL;

	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);

	return NOERROR;
}

int MainApp::Update_Networking(const float TimeDelta)
{
	return m_pGameMgr->Update_Networking();
}

MainApp* MainApp::Create()
{
	MainApp* pMainApp = new MainApp;

	if (FAILED(pMainApp->Ready_MainApp()))
	{
		MSG_BOX("Failed To Create MainApp Instance");
		SafeDelete(pMainApp);
	}

	return pMainApp;
}

void MainApp::Free()
{
	Renderer::GetInstance()->DestroyInstance();
	ShaderMgr::GetInstance()->DestroyInstance();
	GameMgr::GetInstance()->DestroyInstance();
	ResourceMgr::GetInstance()->DestroyInstance();
	DeviceMgr::GetInstance()->DestroyInstance();
	NetworkMgr::GetInstance()->DestroyInstance();
}
