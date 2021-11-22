#include "framework.h"
#include "Scene_Customize.h"

#include "AnimatedSprite.h"
#include "InputMgr.h"
#include "Scene_Test.h"
#include "PostCard.h"
#include "CustomPlayer.h"
#include "NetworkMgr.h"

Scene_Customize::Scene_Customize(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Scene(pGraphic_Device)
{
}

Scene_Customize::~Scene_Customize()
{
	Free();
}

HRESULT Scene_Customize::Ready_Scene()
{
	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) 
		return E_FAIL;

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (!m_pNetworkMgr) 
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 0, true))))
		return E_FAIL;
	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 1, false))))
		return E_FAIL;
	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 2, false))))
		return E_FAIL;

	return Scene::Ready_Scene();
}

int Scene_Customize::Update_Scene(float time_delta)
{


	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Customize::Render_Scene()
{
	return Scene::Render_Scene();
}

Scene_Customize* Scene_Customize::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Scene_Customize* pInstance = new Scene_Customize(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create Scene_Customize Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Scene_Customize::Free()
{
	Scene::Free();
}
