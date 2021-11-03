#include "framework.h"
#include "Player.h"

#include "Texture.h"
#include "Buffer.h"

#include "DeviceMgr.h"

Player::Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Player::~Player()
{
}

HRESULT Player::Ready_GameObject()
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = Texture::Create(m_pDevice, L"../Binary/Resources/Player/Idle_Front/Idle_Front_%d.png", 3);
	if (!m_pTexture) return E_FAIL;

	m_pBuffer = Buffer::Create(m_pDevice);
	if (!m_pBuffer) return E_FAIL;

	return NOERROR;
}

INT Player::Update_GameObject(float TimeDelta)
{
	return 0;
}

INT Player::LateUpdate_GameObject(float TimeDelta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_PRIORITY, this);

	return 0;
}

HRESULT Player::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	D3DXMATRIX		matTmp;

	D3DXMATRIX		matScale;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	pEffect->SetMatrix("g_matWorld", &matScale);
	//pEffect->SetMatrix("g_matWorld", D3DXMatrixIdentity(&matScale));

	m_pDevice->GetTransform(D3DTS_VIEW, &matTmp);
	//pEffect->SetMatrix("g_matView", D3DXMatrixIdentity(&matTmp));
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);
	//pEffect->SetMatrix("g_matProj", D3DXMatrixIdentity(&matTmp));

	if (FAILED(m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 0)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBuffer->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

Player* Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Player* pInstance = new Player(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject()))
		SafeDelete(pInstance);
	return pInstance;
}

void Player::Free()
{
}
