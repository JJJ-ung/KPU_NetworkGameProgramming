#include "framework.h"
#include "StaticSprite.h"

#include "Texture.h"
#include "ResourceMgr.h"

StaticSprite::StaticSprite(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

StaticSprite::~StaticSprite()
{
}

HRESULT StaticSprite::Ready_GameObject(const TCHAR* pObjTag, const TCHAR* pStateTag, D3DXVECTOR3 vPos,
	D3DXVECTOR3 vScale)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(pObjTag, pStateTag);
	if (!m_pTexture) return E_FAIL;

	m_vPos = vPos;
	m_vScale = vScale;

	m_vCenter = D3DXVECTOR3(m_pTexture->Get_TextureInfo().Width * 0.5f, m_pTexture->Get_TextureInfo().Height * 0.5f, 0.f);

	return GameObject::Ready_GameObject();
}

INT StaticSprite::Update_GameObject(float time_delta)
{
	if(m_fAlpha < 1.f)
		m_fAlpha += time_delta * 3.f;

	return GameObject::Update_GameObject(time_delta);
}

INT StaticSprite::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT StaticSprite::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_VIEW, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	m_pShader->Set_Value("g_fAlpha", &m_fAlpha, sizeof(float));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pTexture->Draw_Sprite(0, &m_vCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

StaticSprite* StaticSprite::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pObjTag, const TCHAR* pStateTag,
	D3DXVECTOR3 vPos, D3DXVECTOR3 vScale)
{
	StaticSprite* pInstance = new StaticSprite(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(pObjTag, pStateTag, vPos, vScale)))
		SafeDelete(pInstance);
	return pInstance;
}

void StaticSprite::Free()
{
	SafeDelete(m_pTexture);
	GameObject::Free();
}
