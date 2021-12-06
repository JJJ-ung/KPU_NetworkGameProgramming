#include "framework.h"
#include "Scene_Ending.h"
#include "EndingPlayer.h"

Scene_Ending::Scene_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Scene(pGraphic_Device)
{
}

Scene_Ending::~Scene_Ending()
{
	Free();
}

HRESULT Scene_Ending::Ready_Scene(int iWin)
{
	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (!m_pNetworkMgr)
		return E_FAIL;

	for(int i = 0; i < 3; ++i)
	{
		if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, EndingPlayer::Create(m_pGraphic_Device, i, i == iWin))))
			return E_FAIL;
	}

	return Scene::Ready_Scene();
}

int Scene_Ending::Update_Scene(float time_delta)
{
	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Ending::Render_Scene()
{
	return Scene::Render_Scene();
}

Scene_Ending* Scene_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device, int iWin)
{
	Scene_Ending* pInstance = new Scene_Ending(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene(iWin)))
	{
		MSG_BOX("Failed To Create Scene_Title Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Scene_Ending::Free()
{
	Scene::Free();
}
