#include "framework.h"
#include "PostCard.h"
#include "NetworkMgr.h"

#include "StaticSprite.h"
#include "Texture.h"
#include "CustomPlayer.h"
#include "ColorButton.h"

PostCard::PostCard(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

PostCard::~PostCard()
{
	Free();
}

HRESULT PostCard::Ready_GameObject(UINT iPlayerID, bool bLocalPlayer)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"UI", L"CustomizeWnd");
	if (!m_pTexture) return E_FAIL;

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (!m_pNetworkMgr) return E_FAIL;

	D3DXVECTOR3 vEye = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, -1.f);
	D3DXVECTOR3 vAt = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, 0.f);
	D3DXVECTOR3 vUP = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUP);

	m_iPlayerID = iPlayerID;

	m_vCenter = D3DXVECTOR3(m_pTexture->Get_TextureInfo().Width * 0.5f, m_pTexture->Get_TextureInfo().Height * 0.5f, 0.f);

	m_vPosition = D3DXVECTOR3(420.f * (int)(iPlayerID - 1), 720.f, 0.f);

	if(!bLocalPlayer)
	{
		// check is spawned
		m_vPosition.y = 0.f;
		m_bAnimation = false;
		if (FAILED(m_pGameMgr->Add_GameObject((OBJECT::TYPE)5, CustomPlayer::Create(m_pDevice, m_iPlayerID))))
			return E_FAIL;
		if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, ColorButton::Create(m_pDevice, m_iPlayerID, false))))
			return E_FAIL;
	}

	return GameObject::Ready_GameObject();
}

INT PostCard::Update_GameObject(float time_delta)
{
	if(m_bAnimation)
	{
		if (m_vPosition.y > 0.f)
		{
			m_fSpeed -= m_fAccel * time_delta;
			m_vPosition += D3DXVECTOR3(0.f, time_delta * m_fSpeed, 0.f);
		}

		if (abs(m_vPosition.y) < 20.f)
		{
			if (FAILED(m_pGameMgr->Add_GameObject((OBJECT::TYPE)5, m_pCustomPlayer = CustomPlayer::Create(m_pDevice, m_iPlayerID))))
				return E_FAIL;
			if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pColorButton = ColorButton::Create(m_pDevice, m_iPlayerID, true))))
				return E_FAIL;
			m_vPosition.y = 0.f;
			m_bAnimation = false;
		}
	}

	if(m_pColorButton && m_pCustomPlayer)
	{
		// Color
		m_pColorButton->Update_Collision(m_pCustomPlayer);
	}

	if(m_pInputMgr->KeyDown(KEY_ENTER))
	{
		
	}

	return GameObject::Update_GameObject(time_delta);
}

INT PostCard::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT PostCard::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.4f, 2.4f, 2.4f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	pEffect->SetMatrix("g_matView", &m_matView);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pTexture->Draw_Sprite(0, &m_vCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

PostCard* PostCard::Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iPlayerID, bool bLocalPlayer)
{
	PostCard* pInstance = new PostCard(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(iPlayerID, bLocalPlayer)))
		SafeDelete(pInstance);
	return pInstance;
}

void PostCard::Free()
{
	GameObject::Free();
}