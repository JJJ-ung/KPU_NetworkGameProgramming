#include "framework.h"
#include "Weapon.h"

#include "Player.h"
#include "Shader.h"
#include "Texture.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"

Weapon::Weapon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Weapon::~Weapon()
{
}

HRESULT Weapon::Ready_GameObject(Player* pOwner, int iType)
{
	if (!pOwner)
		return E_FAIL;
	m_pOwner = pOwner;

	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_tWeaponInfo = m_pResourceMgr->Find_WeaponData(iType);
	if (m_tWeaponInfo.type == -1)
		return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"Weapon", L"Weapon");
	if (!m_pTexture) return E_FAIL;

	m_vCenter = D3DXVECTOR3(m_pTexture->Get_TextureInfo().Width * 0.5f, m_pTexture->Get_TextureInfo().Height * 0.5f, 0.f);

	return GameObject::Ready_GameObject();
}

INT Weapon::Update_GameObject(float time_delta)
{
	//m_vParent = m_pOwner->Get_Position()/* + D3DXVECTOR3(0.f, 20.f, 0.f)*/;
	//m_fRotation = m_pInputMgr->Get_Angle();
	//m_vPosition = D3DXVECTOR3(30.f, 0.f, 0.f);
	//if (m_fRotation < 0.f)
	//{
	//	m_fSide = -1.f;
	//	//m_vPosition = D3DXVECTOR3(-30.f, 0.f, 0.f);
	//}
	//else
	//{
	//	m_fSide = 1.f;
	//	//m_vPosition = D3DXVECTOR3(30.f, 0.f, 0.f);
	//}

	m_vParent = m_pOwner->Get_Position();
	m_vDir = m_pInputMgr->Get_MousePoint();
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	D3DXVECTOR3 v = D3DXVECTOR3(0.f, 1.f, 0.f);
	m_fAngle = D3DXVec3Dot(&v, &m_vDir);
	if (m_pInputMgr->Get_Angle() < 0.f)
	{
		m_fSide = -1.f;
		m_vPosition = D3DXVECTOR3(-20.f, 20.f, 0.f);
	}
	else
	{
		m_fSide = 1.f;
		m_vPosition = D3DXVECTOR3(20.f, 20.f, 0.f);
	}

	cout << D3DXToDegree(m_fAngle) << endl;

	return GameObject::Update_GameObject(time_delta);
}

INT Weapon::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT Weapon::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	//D3DXMATRIX		matScale, matRot, matTrans, matParent, matFlip, matRev, matWorld;
	//D3DXMatrixRotationZ(&matRot, D3DXToRadian(-90.f * m_fSide));
	//D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fRotation));
	//D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	////D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	//D3DXMatrixTranslation(&matTrans, 20.f, 0.f, 0.f);
	//D3DXMatrixTranslation(&matParent, m_vParent.x, m_vParent.y, m_vParent.z);
	//D3DXMatrixScaling(&matFlip, m_fSide, 1.f, 1.f);
	//matWorld = matScale * matRot * matTrans * matFlip * matRev* matParent;

	D3DXMATRIX matScale, matRot, matTrans, matParent, matFlip, matWorld;
	D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	D3DXMatrixScaling(&matFlip, m_fSide, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRot, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixTranslation(&matParent, m_vParent.x, m_vParent.y, m_vParent.z);
	matWorld = matScale * matRot * matFlip * matTrans * matParent;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->SetFloat("g_fAlpha", 1.f);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pTexture->Draw_Sprite(m_tWeaponInfo.type, &m_vCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

Weapon* Weapon::Create(LPDIRECT3DDEVICE9 pGraphic_Device, Player* pOwner, int iType)
{
	Weapon* pInstance = new Weapon(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(pOwner, iType)))
		SafeDelete(pInstance);
	return pInstance;
}

void Weapon::Free()
{
	GameObject::Free();
}
