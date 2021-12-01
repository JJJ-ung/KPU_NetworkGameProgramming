#pragma once
#include "Component.h"

class Texture;
class Animation : public Component
{
public:
	explicit Animation(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Animation();

public:
	bool& Get_Stop() { return m_bStop; }
	D3DXVECTOR3& Get_Center() { return m_vCenter; }
	Texture* Get_Texture() { return m_pTexture; }

public:
	virtual HRESULT Ready_Component(Texture* pTexture, float fSpeed, bool bSetCenter, float fStartFrame);
	virtual INT Update_Component(float TimeDelta);
	bool Update_Animation(float TimeDelta, bool bStop = false);

public:
	HRESULT Set_Texture(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT Draw_Sprite();

public:
	void Reset_Animation();

private:
	Texture* m_pTexture = nullptr;

private:
	bool m_bStop = false;
	float m_fFrame = 0.f;
	float m_fMaxFrame = 0.f;
	float m_fFrameSpeed = 1.f;
	D3DXVECTOR3 m_vCenter = {};

public:
	static Animation* Create(LPDIRECT3DDEVICE9 pGraphicDev, Texture* pTexture, float fSpeed, bool bSetCenter = true, float fStartFrame = 0.f);
	virtual void Free(void);
};

