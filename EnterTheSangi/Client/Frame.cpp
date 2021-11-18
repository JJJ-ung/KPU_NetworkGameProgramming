#include "framework.h"
#include "Frame.h"

#include "Animation.h"
#include "Texture.h"

Frame::Frame(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Component(pGraphic_Device)
{
}

Frame::~Frame()
{
}

HRESULT Frame::Ready_Component(Texture* pTexture, float fSpeed, float fStartFrame)
{
	if (!pTexture) return E_FAIL;
	m_pTexture = pTexture;

	m_fFrameCnt = pTexture->Get_FrameCount();
	m_fFrameSpeed = fSpeed;
	m_fFrameCurr = fStartFrame;

	return NOERROR;
}

INT Frame::Update_Component(float time_delta)
{
	return Component::Update_Component(time_delta);
}

INT Frame::LateUpdate_Component(float time_delta)
{
	m_fFrameCurr += time_delta * m_fFrameSpeed;
	if (m_fFrameCurr >= m_fFrameCnt)
		m_fFrameCurr = 0.f;

	return Component::LateUpdate_Component(time_delta);
}

HRESULT Frame::SetUp_Frame(LPD3DXEFFECT pEffect, const char* pConstantName)
{
	return m_pTexture->SetUp_OnShader(pEffect, pConstantName, (UINT)m_fFrameCurr);
}

HRESULT Frame::Reset_Frame()
{
	m_fFrameCurr = 0.f;
	return NOERROR;
}

Frame* Frame::Create(LPDIRECT3DDEVICE9 pGraphic_Device, Texture* pTexture, float fSpeed, float fStartFrame)
{
	Frame* pInstance = new Frame(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component(pTexture, fSpeed, fStartFrame)))
	{
		MSG_BOX("Failed To Create Frame Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Frame::Free()
{
	Component::Free();
}
