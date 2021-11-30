#include "framework.h"
#include "Weapon.h"
#include "Player.h"
#include "Bullet.h"
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

float Weapon::Get_Y()
{
	float y = m_vPosition.y;
	if (abs(m_fAngle) > 90.f)
		y -= 10.f;
	else
		y += 10.f;

	return y;
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

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	m_tWeaponInfo = m_pResourceMgr->Find_WeaponData(iType);
	if (m_tWeaponInfo.type == -1)
		return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"Weapon", L"Weapon");
	if (!m_pTexture) return E_FAIL;

	m_vCenter = D3DXVECTOR3(18.f, m_pTexture->Get_TextureInfo().Height * 0.5f, 0.f);

	D3DXMatrixIdentity(&m_matWorld);

	return GameObject::Ready_GameObject();
}

INT Weapon::Update_GameObject(float time_delta)
{
	Update_Rendering(time_delta);

	if(!m_bShoot)
	{
		m_fTime += time_delta;
		if (m_fTime > m_tWeaponInfo.shotspeed)
		{
			m_fTime = 0.f;
			m_bShoot = true;
		}
	}

	if (m_pInputMgr->KeyDown(KEY_LBUTTON))
		Shoot_Bullet(time_delta);

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

	pEffect->SetMatrix("g_matWorld", &m_matWorld);

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

INT Weapon::Update_Rendering(float TimeDelta)
{
#ifdef TEST
	if (m_pInputMgr->KeyDown(KEY_ENTER))
	{
		m_tWeaponInfo.type += 1;
		if (m_tWeaponInfo.type > 13)
			m_tWeaponInfo.type = 0;
		m_tWeaponInfo = m_pResourceMgr->Find_WeaponData(m_tWeaponInfo.type);
		if (m_tWeaponInfo.type == -1)
			return E_FAIL;
	}
#endif

	m_fAngle = m_pInputMgr->Get_Angle() * -1.f;
	if (m_pInputMgr->Get_Angle() < 0.f)
		m_fSide = -1.f;
	else
		m_fSide = 1.f;

	m_vPosition = m_pOwner->Get_Position() + D3DXVECTOR3(18.f * m_fSide, 35.f, 0.f);

	D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	D3DXMatrixScaling(&matFlip, m_fSide, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(90.f));
	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matParent, m_vPosition.x, m_vPosition.y, 0.f);
	m_matWorld = matScale * matRot * matFlip * matRev * matParent;

	return 0;
}

INT Weapon::Shoot_Bullet(float TimeDelta)
{
	if (!m_bShoot)
		return 0;

	m_bShoot = false;

	m_vBulletPos = { m_tWeaponInfo.bulletoffset.x * 3.f, m_tWeaponInfo.bulletoffset.y * -3.f, 0.f };
	matBullet = matRot * matFlip * matRev * matParent;

	D3DXVECTOR4 vTmp;
	D3DXVec3Transform(&vTmp, &m_vBulletPos, &matBullet);

	m_vBulletPos.x = vTmp.x;
	m_vBulletPos.y = vTmp.y;

	m_vBulletDir = m_pInputMgr->Get_MousePoint() - m_vBulletPos;
	D3DXVec3Normalize(&m_vBulletDir, &m_vBulletDir);

	m_pGameMgr->Add_GameObject(OBJECT::BULLET,
		Bullet::Create(m_pDevice, m_tWeaponInfo.type,
			m_vBulletPos, m_vBulletDir,
			m_fSide, m_tWeaponInfo.size, m_fAngle,
			m_tWeaponInfo.duration, m_tWeaponInfo.bulletspeed));

	return 0;
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
