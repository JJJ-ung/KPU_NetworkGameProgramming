#include "framework.h"
#include "Bullet.h"
#include "Shader.h"
#include "Texture.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"

Bullet::Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Bullet::~Bullet()
{
}

HRESULT Bullet::Ready_GameObject(sc_packet_put_bullet tInfo)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"Weapon", L"Bullet");
	if (!m_pTexture) return E_FAIL;

	if (tInfo.bullet_type > 13)
		return E_FAIL;
	m_iType = tInfo.bullet_type;

	m_vDir = { (float)tInfo.direction.x * 0.00001f, (float)tInfo.direction.y * 0.00001f, 0.f };
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_vPosition = { (float)tInfo.position.x, (float)tInfo.position.y, 0.f };

	auto t = m_pResourceMgr->Find_WeaponData(m_iType);

	m_fSize = t.size;
	m_fDuration = t.duration;
	m_fSpeed = t.bulletspeed;

	m_fAngle = tInfo.angle;
	m_fSide = 1.f;
	if (m_fAngle > 0)
		m_fSide = -1.f;

	m_vCenter = D3DXVECTOR3(m_pTexture->Get_TextureInfo(m_iType).Width * 0.5f, m_pTexture->Get_TextureInfo(m_iType).Height * 0.5f, 0.f);

	return GameObject::Ready_GameObject();
}

INT Bullet::Update_GameObject(float TimeDelta)
{
	m_fTime += TimeDelta;

	if(m_fTime > m_fDuration)
	{
		// 총알 지속 시간 끝 삭제
		return OBJDEAD;
	}

	m_vPosition += m_vDir * m_fSpeed * TimeDelta;

	return GameObject::Update_GameObject(TimeDelta);
}

INT Bullet::LateUpdate_GameObject(float TimeDelta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(TimeDelta);
}

HRESULT Bullet::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX matScale, matRot, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_fSize * m_fSide, m_fSize, m_fSize);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(m_fAngle + 90.f * m_fSide));
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, 0.f);
	matWorld = matScale * matRot * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->SetFloat("g_fAlpha", 1.f);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pTexture->Draw_Sprite(m_iType, &m_vCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

Bullet* Bullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device, sc_packet_put_bullet tInfo)
{
	Bullet* pInstance = new Bullet(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(tInfo)))
		SafeDelete(pInstance);
	return pInstance;
}

void Bullet::Free()
{
	GameObject::Free();
}
