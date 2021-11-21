#include "framework.h"
#include "Scene_Loading.h"
#include "Loading.h"
#include "AnimatedSprite.h"
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

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pAnimation = AnimatedSprite::Create(m_pGraphic_Device, L"UI", L"Loading", 8.f))))
		return E_FAIL;

	m_pAnimation->Get_Scale() = D3DXVECTOR3(2.f, 2.f, 2.f);
	m_pAnimation->Get_Position() = D3DXVECTOR3(1024.f, 464.f, 0.f);

	return NOERROR;
}

int Scene_Loading::Update_Scene(float time_delta)
{
	if (!m_pLoading) return -1;

	m_fTime += time_delta;
	//cout << m_fTime << endl;

	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Loading::Render_Scene()
{
	//if (m_fTime >= 5.f)
	{
		m_fTime = 5.f;
		if (m_pLoading->Get_Finish())
		{
			// 로딩 종료
			m_pGameMgr->Clear_Scene();
			Scene_Title* pScene = Scene_Title::Create(m_pGraphic_Device);
			if (FAILED(m_pGameMgr->Set_CurrScene(pScene)))
				return -1;
		}
	}

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
	//cout << "Delete Enter" << endl;
	
	SafeDelete(m_pLoading);

	Scene::Free();
}
