#pragma once
#include "Component.h"

class Texture : public Component
{
public:
	explicit Texture(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Texture();

public:
	IDirect3DBaseTexture9* Get_Texture(UINT iInt) { return m_vecTexture[iInt]; }

public:
	virtual HRESULT Ready_Component(const TCHAR* pPath, const UINT& iCnt);

public:
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, UINT iIndex = 0);
	UINT Get_FrameCount();

private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;

public:
	static Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const UINT& iCnt = 1);
	virtual void Free(void);
};

