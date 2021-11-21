#include "framework.h"
#include "Scene_Title.h"

#include "Camera.h"
#include "TestMap.h"

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
	cout << "Title" << endl;

	if (FAILED(m_pGameMgr->Set_PrototypesOnScene(OBJECT::PLAYER)))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::CAMERA, Camera::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::MAP, TestMap::Create(m_pGraphic_Device))))
		return E_FAIL;

	return Scene::Ready_Scene();
}

int Scene_Title::Update_Scene(float time_delta)
{
	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Title::Render_Scene()
{
	return Scene::Render_Scene();
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
	Scene::Free();
}
