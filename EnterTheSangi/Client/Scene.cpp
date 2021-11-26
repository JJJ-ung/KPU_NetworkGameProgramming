#include "framework.h"
#include "Scene.h"

Scene::Scene(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	m_pGameMgr = GameMgr::GetInstance();
}

Scene::~Scene()
{
	Free();
}

HRESULT Scene::Ready_Scene()
{
	return NOERROR;
}

int Scene::Update_Scene(float TimeDelta)
{
	return NOEVENT;
}

HRESULT Scene::Render_Scene()
{
	return NOERROR;
}

HRESULT Scene::Setup_Recv(char type, void* pRecv)
{
	return NOERROR;
}

void Scene::Free()
{
}
