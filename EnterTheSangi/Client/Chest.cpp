#include "framework.h"
#include "Chest.h"
#include "Shader.h"
#include "Texture.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"

Chest::Chest(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Chest::~Chest()
{
	Free();
}

HRESULT Chest::Ready_GameObject(sc_packet_put_chest tInfo)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"Weapon", L"Chest");
	if (!m_pTexture) return E_FAIL;

	m_pWeaponTexture = m_pResourceMgr->Find_Texture(L"Weapon", L"Weapon");
	if (!m_pWeaponTexture) return E_FAIL;

	m_iObjectID = tInfo.chest_id;
	m_iWeaponID = tInfo.weapon_id;
	m_vPosition = { (float)tInfo.position.x, (float)tInfo.position.y, 0.f };

	m_vCenter = D3DXVECTOR3(m_pTexture->Get_TextureInfo().Width * 0.5f, m_pTexture->Get_TextureInfo().Height, 0.f);

	return GameObject::Ready_GameObject();
}

INT Chest::Update_GameObject(float TimeDelta)
{
	m_fWeaponAngle += TimeDelta * 5.f;
	if (m_fWeaponAngle > 360.f)
		m_fWeaponAngle = 0.f;

	m_vWeaponPosition.y = sinf(m_fWeaponAngle) * 10.f;

	if (m_pInputMgr->KeyDown(KEY_SHIFT))
	{
		m_iWeaponID += 1;
		if (m_iWeaponID > 13)
			m_iWeaponID = 0;
	}

	return GameObject::Update_GameObject(TimeDelta);
}

INT Chest::LateUpdate_GameObject(float TimeDelta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(TimeDelta);
}

HRESULT Chest::Render_GameObject()
{
	if (FAILED(Render_ChestTop()))
		return E_FAIL;

	if (FAILED(Render_Weapon()))
		return E_FAIL;

	if (FAILED(Render_ChestBottom()))
		return E_FAIL;

	return GameObject::Render_GameObject();
}

HRESULT Chest::Render_ChestBottom()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->SetFloat("g_fAlpha", 1.f);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pTexture->Draw_Sprite(0, &m_vCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

HRESULT Chest::Render_ChestTop()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->SetFloat("g_fAlpha", 1.f);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pTexture->Draw_Sprite(1, &m_vCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

HRESULT Chest::Render_Weapon()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pWeaponTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x - 20.f, m_vPosition.y + m_vWeaponPosition.y - 70.f, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->SetFloat("g_fAlpha", 1.f);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pWeaponTexture->Draw_Sprite(m_iWeaponID, &m_vWeaponCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

Chest* Chest::Create(LPDIRECT3DDEVICE9 pGraphic_Device, sc_packet_put_chest tInfo)
{
	Chest* pInstance = new Chest(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(tInfo)))
		SafeDelete(pInstance);
	return pInstance;
}

void Chest::Free()
{
	GameObject::Free();
}
