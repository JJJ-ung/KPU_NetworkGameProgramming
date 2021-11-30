#include "framework.h"
#include "Scene_Test.h"

#include "Player.h"
#include "Mouse.h"
#include "Camera.h"
#include "TestMap.h"
#include "NetworkPlayer.h"
#include "Weapon.h"

Scene_Test::Scene_Test(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Scene(pGraphic_Device)
{
}

Scene_Test::~Scene_Test()
{
	Free();
}

HRESULT Scene_Test::Ready_Scene()
{
	cout << "Test" << endl;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	Player* p = nullptr;

#ifdef TEST
	CLIENT t(true, 0, m_pGameMgr->Get_ClientPlayerName(), 
		D3DXVECTOR3(rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f), 
		D3DXVECTOR3(rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f), D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pGameMgr->Get_ClientInfos()[0] = t;
	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, p = Player::Create(m_pGraphic_Device, m_pGameMgr->Get_ClientInfos()[0]))))
		return E_FAIL;
#else
	for(auto t : m_pGameMgr->Get_ClientInfos())
	{
		if(t.islocal)
		{
			if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, p = Player::Create(m_pGraphic_Device, t))))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::NETWORK_PLAYER, NetworkPlayer::Create(m_pGraphic_Device, t))))
				return E_FAIL;
		}
	}
#endif

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::CAMERA, Camera::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::MAP, TestMap::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, Mouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::WEAPON, Weapon::Create(m_pGraphic_Device, p, 0))))
		return E_FAIL;

	cout << "TestEnd" << endl;

	return Scene::Ready_Scene();
}

int Scene_Test::Update_Scene(float time_delta)
{
	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Test::Render_Scene()
{
	return Scene::Render_Scene();
}

HRESULT Scene_Test::Setup_Recv(char c, void* recv)
{
	m_pGameMgr->Recv_Networking(c, recv);
	return Scene::Setup_Recv(c, recv);
}

Scene_Test* Scene_Test::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Scene_Test* pInstance = new Scene_Test(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create Scene_Title Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Scene_Test::Free()
{
	Scene::Free();
}
