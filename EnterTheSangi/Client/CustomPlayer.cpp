#include "framework.h"
#include "CustomPlayer.h"

#include "Animation.h"
#include "ResourceMgr.h"
#include "Shader.h"
#include "ShaderMgr.h"
#include "Font.h"

CustomPlayer::CustomPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

CustomPlayer::~CustomPlayer()
{
	Free();
}

HRESULT CustomPlayer::Ready_GameObject(UINT iPlayerID, string strName, D3DXVECTOR3 vBody, D3DXVECTOR3 vCloth)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Player");
	if (!m_pShader) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	Texture* pTex = m_pResourceMgr->Find_Texture(L"Player", L"Turn");
	m_pAnimation = Animation::Create(m_pDevice, pTex, 8.f);
	if (!m_pAnimation) return E_FAIL;

	m_tCustomInfo.vBody = vBody;
	m_tCustomInfo.vCloth = vCloth;

	// iPlayerID로 이름 받아오기
	m_strName = strName;

	m_vPosition = D3DXVECTOR3(420.f * (int)(iPlayerID - 1), -130.f, 0.f);

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pNameTag = Font::Create(m_pDevice, strName, 1.f))))
		return E_FAIL;
	if (!m_pNameTag) return E_FAIL;
	m_pNameTag->Get_Color() = D3DXVECTOR4(0.f, 0.f, 0.f, 0.f);
	m_pNameTag->Update_Position(m_vPosition, D3DXVECTOR3(0.f, -150.f, 0.f));

	D3DXVECTOR3 vEye = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, -1.f);
	D3DXVECTOR3 vAt = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, 0.f);
	D3DXVECTOR3 vUP = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUP);

	m_pNameTag->Get_ViewMatrix() = m_matView;

	return GameObject::Ready_GameObject();
}

INT CustomPlayer::Update_GameObject(float time_delta)
{
	if (m_fAlpha < 1.f)
		m_fAlpha += time_delta * 3.f;

	m_pAnimation->Update_Component(time_delta);

	return GameObject::Update_GameObject(time_delta);
}

INT CustomPlayer::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT CustomPlayer::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pAnimation->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 5.f, 5.f, 5.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	pEffect->SetMatrix("g_matView", &m_matView);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	m_pShader->Set_Value("g_vCloth", &m_tCustomInfo.vCloth, sizeof(D3DXVECTOR3));
	m_pShader->Set_Value("g_vBody", &m_tCustomInfo.vBody, sizeof(D3DXVECTOR3));

	m_pShader->Set_Value("g_fAlpha", &m_fAlpha, sizeof(float));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pAnimation->Draw_Sprite()))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

CustomPlayer* CustomPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iPlayerID, string strName, D3DXVECTOR3 vBody, D3DXVECTOR3 vCloth)
{
	CustomPlayer* pInstance = new CustomPlayer(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(iPlayerID, strName, vBody, vCloth)))
		SafeDelete(pInstance);
	return pInstance;
}

void CustomPlayer::Free()
{
	SafeDelete(m_pAnimation);
	GameObject::Free();
}
