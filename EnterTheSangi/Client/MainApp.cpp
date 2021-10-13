#include "framework.h"
#include "MainApp.h"

MainApp::MainApp()
{
}

MainApp::~MainApp()
{
}

HRESULT MainApp::Ready_MainApp()
{
	if (FAILED(Ready_Default(DeviceMgr::MODE_WIN, g_nWinCX, g_nWinCY)))
		return E_FAIL;

	//ShowCursor(false);

	return NOERROR;
}

int MainApp::Update_MainApp(float TimeDelta)
{
	return 0;
}

HRESULT MainApp::Render_MainApp()
{
	if (!m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphic_Device->BeginScene();

	//if (FAILED(m_pRenderer->Render_ObjectList()))
	//	return E_FAIL;

	//m_pManagement->Render_Management();

	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);

	return NOERROR;
}

HRESULT MainApp::Ready_Default(DeviceMgr::WINMODE eMode, const UINT& iSizeX, const UINT& iSizeY)
{
	if (FAILED(DeviceMgr::GetInstance()->Ready_GraphicDev(g_hWnd, eMode, iSizeX, iSizeY, &m_pGraphic_Device)))
		return E_FAIL;

	return NOERROR;
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
	DeviceMgr::GetInstance()->DestroyInstance();
}
