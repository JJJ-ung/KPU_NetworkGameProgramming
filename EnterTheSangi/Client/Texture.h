#pragma once
#include "Component.h"

class Texture : public Component
{
public:
	explicit Texture(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Texture();

public:
	virtual HRESULT Ready_Component(const TCHAR* pPath, const UINT& iCnt);

public:
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, UINT iIndex = 0);

private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;

public:
	static Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const UINT& iCnt = 1);
	virtual void Free(void);
};

