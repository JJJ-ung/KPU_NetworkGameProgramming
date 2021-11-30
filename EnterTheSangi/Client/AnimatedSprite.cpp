#include "framework.h"
#include "AnimatedSprite.h"

#include "Shader.h"
#include "Animation.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"

AnimatedSprite::AnimatedSprite(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

AnimatedSprite::~AnimatedSprite()
{
	Free();
}

HRESULT AnimatedSprite::Ready_GameObject(const TCHAR* pObjTag, const TCHAR* pStateTag, float fSpeed)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pAnimation = Animation::Create(m_pDevice, m_pResourceMgr->Find_Texture(pObjTag, pStateTag), fSpeed, false);
	if (!m_pAnimation) return E_FAIL;

	return GameObject::Ready_GameObject();
}

INT AnimatedSprite::Update_GameObject(float time_delta)
{
	if (m_fAlpha < 1.f)
		m_fAlpha += time_delta * 3.f;
	m_pAnimation->Update_Component(time_delta);

	return GameObject::Update_GameObject(time_delta);
}

INT AnimatedSprite::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT AnimatedSprite::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pAnimation->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
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

	if (FAILED(m_pAnimation->Draw_Sprite()))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

AnimatedSprite* AnimatedSprite::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pObjTag, const TCHAR* pStateTag,
	float fSpeed)
{
	AnimatedSprite* pInstance = new AnimatedSprite(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(pObjTag, pStateTag, fSpeed)))
		SafeDelete(pInstance);
	return pInstance;
}

void AnimatedSprite::Free()
{
	SafeDelete(m_pAnimation);
	GameObject::Free();
}
