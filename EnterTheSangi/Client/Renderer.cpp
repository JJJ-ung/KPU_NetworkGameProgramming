#include "framework.h"
#include "Renderer.h"

#include "Shader.h"
#include "GameObject.h"

Renderer::Renderer(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pTarget_Manager = TargetMgr::GetInstance();
}

Renderer::~Renderer()
{
	Free();
}

HRESULT Renderer::Ready_Renderer()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3DVIEWPORT9		ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	//*********************************************************************************************************************************************************************************************
	if (FAILED(m_pTarget_Manager->Add_Target(m_pDevice, L"Target_Diffuse", ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	//*********************************************************************************************************************************************************************************************

	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VTXVIEWPORTTEX) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXVIEWPORTTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = D3DXVECTOR4(0.0f - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertices[1].vPosition = D3DXVECTOR4(ViewPort.Width - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertices[2].vPosition = D3DXVECTOR4(ViewPort.Width - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertices[3].vPosition = D3DXVECTOR4(0.0f - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();


	if (FAILED(m_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	INDEX16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();


#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Diffuse", 0.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
#endif

	return NOERROR;
}

HRESULT Renderer::Add_RenderList(RENDERGROUP eGroup, GameObject* pObj)
{
	if (nullptr == pObj ||
		RENDER_END <= eGroup)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pObj);

	return NOERROR;
}

HRESULT Renderer::Render_Objects()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

#ifdef _DEBUG
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Deferred");
#endif

	return NOERROR;
}

HRESULT Renderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
	}
	m_RenderList[RENDER_PRIORITY].clear();

	return NOERROR;
}

HRESULT Renderer::Render_NonAlpha()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Deferred")))
		return E_FAIL;

	// Begin();
	// 백버퍼를 빼자.
	// 디퓨즈, 노멀타겟을 셋팅하자.

	// 그린다.
	for (auto& pGameObject : m_RenderList[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
	}
	m_RenderList[RENDER_NONALPHA].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Deferred")))
		return E_FAIL;
	// End();
	// 디퓨즈와 노멀타겟을 회수.
	// 백버퍼를 셋팅하자.

	return NOERROR;
}

HRESULT Renderer::Render_Alpha()
{
	return NOERROR;
}

HRESULT Renderer::Render_UI()
{
	return NOERROR;
}

HRESULT Renderer::Render_Test()
{
	return NOERROR;
}

Renderer* Renderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Renderer* pInstance = new Renderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		MSG_BOX("Failed To Create CRenderer Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Renderer::Free()
{
	for (size_t i = 0; i < RENDER_END; ++i)
		m_RenderList[i].clear();

	SafeDelete(m_pVB);
	SafeDelete(m_pIB);

	SafeDelete(m_pTest);
}
