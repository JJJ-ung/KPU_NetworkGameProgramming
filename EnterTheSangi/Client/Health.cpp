#include "framework.h"
#include "Health.h"
#include "Shader.h"
#include "Texture.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"

Health::Health(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Health::~Health()
{
	Free();
}

HRESULT Health::Ready_GameObject()
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"UI", L"Health");
	if (!m_pTexture) return E_FAIL;

	return GameObject::Ready_GameObject();
}

INT Health::Update_GameObject(float TimeDelta)
{
	return GameObject::Update_GameObject(TimeDelta);
}

INT Health::LateUpdate_GameObject(float TimeDelta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_UI, this);

	return GameObject::LateUpdate_GameObject(TimeDelta);
}

HRESULT Health::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	int iFull = m_iHealth / 2;
	int iHalf = m_iHealth % 2;
	int iEmpty = 5 - (iFull + iHalf);

	int iCnt = 0;
	for (int i = 0; i < iFull; ++i)
	{
		Render_Heart(pEffect, iCnt, 2);
		++iCnt;
	}
	for (int i = 0; i < iHalf; ++i)
	{
		Render_Heart(pEffect, iCnt, 1);
		++iCnt;
	}
	for (int i = 0; i < iEmpty; ++i)
	{
		Render_Heart(pEffect, iCnt, 0);
		++iCnt;
	}

	return GameObject::Render_GameObject();
}

HRESULT Health::Render_Heart(LPD3DXEFFECT pEffect, int iIdx, int iTex)
{
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x + 20.f + iIdx * 50.f, m_vPosition.y + 20.f, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_VIEW, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pTexture->Draw_Sprite(iTex)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

Health* Health::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Health* pInstance = new Health(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject()))
		SafeDelete(pInstance);
	return pInstance;
}

void Health::Free()
{
}
