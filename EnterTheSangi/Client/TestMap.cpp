#include "framework.h"
#include "TestMap.h"

#include "Texture.h"

TestMap::TestMap(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

TestMap::~TestMap()
{
}

HRESULT TestMap::Ready_GameObject()
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"Test", L"Map");
	if (!m_pTexture) return E_FAIL;

	return GameObject::Ready_GameObject();
}

INT TestMap::Update_GameObject(float time_delta)
{
	return GameObject::Update_GameObject(time_delta);
}

INT TestMap::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_PRIORITY, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT TestMap::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale;
	D3DXMatrixScaling(&matScale, 4.f, 4.f, 4.f);
	pEffect->SetMatrix("g_matWorld", &matScale);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	D3DXVECTOR3 vCenter = {m_pTexture->Get_TextureInfo().Width * 0.5f, m_pTexture->Get_TextureInfo().Height * 0.5f, 0.f};
	if (FAILED(m_pTexture->Draw_Sprite(0, &vCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

TestMap* TestMap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	TestMap* pInstance = new TestMap(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject()))
		SafeDelete(pInstance);
	return pInstance;
}

void TestMap::Free()
{
	GameObject::Free();
}
