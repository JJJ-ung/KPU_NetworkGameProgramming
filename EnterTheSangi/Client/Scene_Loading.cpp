#include "framework.h"
#include "Scene_Loading.h"
#include "Loading.h"

#include "Scene_Title.h"

Scene_Loading::Scene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Scene(pGraphic_Device)
{
}

Scene_Loading::~Scene_Loading()
{
	Free();
}

HRESULT Scene_Loading::Ready_Scene()
{
	m_pLoading = Loading::Create(m_pGraphic_Device);
	if (!m_pLoading) return E_FAIL;

	return NOERROR;
}

int Scene_Loading::Update_Scene(float time_delta)
{
	if (!m_pLoading) return -1;

	cout << m_pLoading->Get_Finish() << endl;

	if (m_pLoading->Get_Finish())
	{
		// 로딩 종료
		Scene_Title* pScene = Scene_Title::Create(m_pGraphic_Device);
		if (FAILED(m_pGameMgr->Set_CurrScene(pScene)))
			return -1;
	}

	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Loading::Render_Scene()
{
	return Scene::Render_Scene();
}

Scene_Loading* Scene_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Scene_Loading* pInstance = new Scene_Loading(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create Scene_Loading Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Scene_Loading::Free()
{
	SafeDelete(m_pLoading);

	Scene::Free();
}
