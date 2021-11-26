#include "framework.h"
#include "Scene_Title.h"
#include "Scene_Customize.h"
#include "InputMgr.h"
#include "NetworkMgr.h"
#include "StaticSprite.h"
#include "AnimatedSprite.h"
#include "Font.h"
#include "NetworkMgr.h"

Scene_Title::Scene_Title(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Scene(pGraphic_Device)
{
}

Scene_Title::~Scene_Title()
{
	Free();
}

HRESULT Scene_Title::Ready_Scene()
{
	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (!m_pNetworkMgr)
		return E_FAIL;

	m_pInputMgr->Get_TextMode() = true;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pAnimation = AnimatedSprite::Create(m_pGraphic_Device, L"UI", L"Title", 20.f))))
		return E_FAIL;

	m_pAnimation->Get_Scale() = D3DXVECTOR3(0.63f, 0.63f, 0.63f);
	m_pAnimation->Get_Position() = D3DXVECTOR3(0.f, 0.f, 0.f);

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, StaticSprite::Create(m_pGraphic_Device, L"UI", L"Logo", D3DXVECTOR3(640.f, 260.f, 0.f), D3DXVECTOR3(1.3f, 1.3f, 1.3f)))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, StaticSprite::Create(m_pGraphic_Device, L"UI", L"Login", D3DXVECTOR3(150.f, 700.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f)))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pName = Font::Create(m_pGraphic_Device, "", 0.7f, false))))
		return E_FAIL;
	m_pName->Update_Position(D3DXVECTOR3(80.f, 620.f, 0.f));

//	cout << "TitleEnd" << endl;

	return Scene::Ready_Scene();
}

int Scene_Title::Update_Scene(float time_delta)
{
	if(m_strName != m_pInputMgr->Get_Text())
	{
		m_strName = m_pInputMgr->Get_Text();
		m_pName->Convert_Line(m_strName);
	}

	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Title::Render_Scene()
{
	if(m_pInputMgr->KeyDown(KEY_ENTER))
	{
		if (m_bPressedOK)
			return NOERROR;

		if (m_strName.length() < 2)
			return NOERROR;

		m_pGameMgr->Get_ClientPlayerName() = m_strName;
		m_pNetworkMgr->Send_LoginInfo(m_strName.c_str());

		m_bPressedOK = true;
	}

	return Scene::Render_Scene(); 
}

HRESULT Scene_Title::Setup_Recv(char c, void* recv)
{
	if(c == SC_PACKET_LOGIN_OK)
	{
		m_pGameMgr->Clear_Scene();
		sc_packet_login_ok login;
		memcpy(&login, recv, sizeof(sc_packet_login_ok));
		Scene_Customize* pScene = Scene_Customize::Create(m_pGraphic_Device, login);
		if (FAILED(m_pGameMgr->Set_CurrScene(pScene)))
		{
			cout << "Failed To Change Scene" << endl;
			return E_FAIL;
		}
	}

	return Scene::Setup_Recv(c, recv);
}

Scene_Title* Scene_Title::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Scene_Title* pInstance = new Scene_Title(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create Scene_Title Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Scene_Title::Free()
{
	m_pInputMgr->Get_TextMode() = false;

	Scene::Free();
}
