#include "framework.h"
#include "Scene_Test.h"

#include "Camera.h"
#include "TestMap.h"

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

	if (FAILED(m_pGameMgr->Set_PrototypesOnScene(OBJECT::PLAYER)))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::CAMERA, Camera::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::MAP, TestMap::Create(m_pGraphic_Device))))
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
