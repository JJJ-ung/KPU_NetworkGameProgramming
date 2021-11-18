#pragma once
#include "Component.h"

class Texture;
class ResourceMgr;
class Frame : public Component
{
public:
	explicit Frame(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Frame();

public:
	virtual HRESULT Ready_Component(Texture* pTexture, float fSpeed, float fStartFrame);
	virtual INT Update_Component(float TimeDelta);
	virtual INT LateUpdate_Component(float TimeDelta);

public:
	HRESULT SetUp_Frame(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT Reset_Frame();

private:
	Texture* m_pTexture = nullptr;
	ResourceMgr* m_pResourceMgr = nullptr;

private:
	float m_fFrameCnt = 0.f;
	float m_fFrameCurr = 0.f;
	float m_fFrameSpeed = 0.f;

public:
	static Frame* Create(LPDIRECT3DDEVICE9 pGraphic_Device, Texture* pTexture, float fSpeed, float fStartFrame = 0.f);
	virtual void Free();
};
