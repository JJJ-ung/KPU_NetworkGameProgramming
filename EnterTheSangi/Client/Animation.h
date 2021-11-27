#pragma once
#include "Component.h"

class Texture;
class Animation : public Component
{
public:
	explicit Animation(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Animation();

public:
	virtual HRESULT Ready_Component(Texture* pTexture, float fSpeed, bool bSetCenter, float fStartFrame);
	virtual INT Update_Component(float TimeDelta);
	bool Update_Animation(float TimeDelta);

public:
	HRESULT Set_Texture(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT Draw_Sprite();

public:
	void Reset_Animation();

private:
	Texture* m_pTexture = nullptr;

private:
	float m_fFrame = 0.f;
	float m_fMaxFrame = 0.f;
	float m_fFrameSpeed = 1.f;
	D3DXVECTOR3 m_vCenter = {};

public:
	static Animation* Create(LPDIRECT3DDEVICE9 pGraphicDev, Texture* pTexture, float fSpeed, bool bSetCenter = true, float fStartFrame = 0.f);
	virtual void Free(void);
};

