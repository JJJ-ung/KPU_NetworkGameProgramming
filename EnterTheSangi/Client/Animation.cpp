#include "framework.h"
#include "Animation.h"

#include "Texture.h"

Animation::Animation(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Component(pGraphic_Device)
{
}

Animation::~Animation()
{
}

HRESULT Animation::Ready_Component(Texture* pTexture, float fSpeed, bool bSetCenter, float fStartFrame)
{
	if (!pTexture) return E_FAIL;

	m_pTexture = pTexture;

	m_fFrame = fStartFrame;
	m_fMaxFrame = (float)pTexture->Get_TextureCnt();

	m_fFrameSpeed = fSpeed;

	if(bSetCenter)
	{
		D3DXIMAGE_INFO tInfo = pTexture->Get_TextureInfo();
		m_vCenter = D3DXVECTOR3(tInfo.Width * 0.5f, tInfo.Height * 0.5f, 0.f);
	}

	return Component::Ready_Component();
}

INT Animation::Update_Component(float time_delta)
{
	m_fFrame += time_delta * m_fFrameSpeed;
	if (m_fFrame >= m_fMaxFrame)
		m_fFrame = 0.f;

	return Component::Update_Component(time_delta);
}

INT Animation::LateUpdate_Component(float time_delta)
{
	return Component::LateUpdate_Component(time_delta);
}

HRESULT Animation::Set_Texture(LPD3DXEFFECT pEffect, const char* pConstantName)
{
	return m_pTexture->Set_Texture(pEffect, pConstantName, (UINT)m_fFrame);
}

HRESULT Animation::Draw_Sprite()
{
	return m_pTexture->Draw_Sprite((UINT)m_fFrame, &m_vCenter);
}

void Animation::Reset_Animation()
{
	m_fFrame = 0.f;
}

Animation* Animation::Create(LPDIRECT3DDEVICE9 pGraphicDev, Texture* pTexture, float fSpeed, bool bSetCenter, float fStartFrame)
{
	Animation* pInstance = new Animation(pGraphicDev);

	if (FAILED(pInstance->Ready_Component(pTexture, fSpeed, bSetCenter, fStartFrame)))
	{
		MSG_BOX("Failed To Create Animation Instance");
		SafeDelete(pInstance);
	}
	return pInstance;
}

void Animation::Free()
{
	Component::Free();
}
