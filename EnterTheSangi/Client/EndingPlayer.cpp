#include "framework.h"
#include "EndingPlayer.h"
#include "Font.h"
#include "Shader.h"
#include "Animation.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"

EndingPlayer::EndingPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

EndingPlayer::~EndingPlayer()
{
	Free();
}

HRESULT EndingPlayer::Ready_GameObject(int iID, bool bWin)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	if(bWin)
		m_pAnimation = Animation::Create(m_pDevice, m_pResourceMgr->Find_Texture(L"Player", L"Happy"), 10.f, true);
	else
		m_pAnimation = Animation::Create(m_pDevice, m_pResourceMgr->Find_Texture(L"Player", L"Idle_Front"), 6.f, true);
	if (!m_pAnimation) return E_FAIL;

	m_tInfo = m_pGameMgr->Get_ClientInfos()[iID];

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pFont = Font::Create(m_pDevice, m_tInfo.name, 0.5f, true, true))))
		return E_FAIL;
	if (!m_pFont) return E_FAIL;
	m_pFont->Update_Position(m_vPosition, D3DXVECTOR3(0.f, 100.f, 0.f));

	D3DXVECTOR3 vEye = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, -1.f);
	D3DXVECTOR3 vAt = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, 0.f);
	D3DXVECTOR3 vUP = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUP);

	m_vPosition.x = (iID - 1) * 300.f;

	return GameObject::Ready_GameObject();
}

INT EndingPlayer::Update_GameObject(float time_delta)
{
	m_pAnimation->Update_Component(time_delta);

	return GameObject::Update_GameObject(time_delta);
}

INT EndingPlayer::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT EndingPlayer::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pAnimation->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	pEffect->SetMatrix("g_matView", &m_matView);

	D3DXMATRIX		matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pShader->Set_Value("g_vCloth", &m_tInfo.custom.vCloth, sizeof(D3DXVECTOR3));
	m_pShader->Set_Value("g_vBody", &m_tInfo.custom.vBody, sizeof(D3DXVECTOR3));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pAnimation->Draw_Sprite()))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

EndingPlayer* EndingPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, int iID, bool bWin)
{
	EndingPlayer* pInstance = new EndingPlayer(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(iID, bWin)))
		SafeDelete(pInstance);
	return pInstance;
}

void EndingPlayer::Free()
{
	SafeDelete(m_pAnimation);
	GameObject::Free();
}
