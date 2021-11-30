#pragma once
#include "Component.h"

class Texture : public Component
{
public:
	explicit Texture(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Texture();

public:
	D3DXIMAGE_INFO Get_TextureInfo(int idx = 0) { return m_vecInfo[idx]; }
	UINT Get_TextureCnt() { return (UINT)m_vecTexture.size(); }
	LPDIRECT3DTEXTURE9 Get_Texture(UINT index) { return m_vecTexture[index]; }

public:
	virtual HRESULT Ready_Component(LPD3DXSPRITE pSprite, const TCHAR* pPath, const UINT& iCnt);

public:
	HRESULT Set_Texture(LPD3DXEFFECT pEffect, const char* pConstantName, UINT iIndex = 0);
	HRESULT Set_Transform(D3DXMATRIX& mat);
	HRESULT Draw_Sprite(UINT iIndex = 0, const D3DXVECTOR3* pCenter = nullptr, const RECT* pSrcRect = nullptr, const D3DXVECTOR3* pPosition = nullptr, D3DCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255));

private:
	LPD3DXSPRITE m_pSprite;
	D3DXIMAGE_INFO m_tInfo{};
	vector<LPDIRECT3DTEXTURE9>					m_vecTexture;
	typedef vector<LPDIRECT3DTEXTURE9>	VECTEXTURE;
	vector<D3DXIMAGE_INFO>							m_vecInfo;
	typedef vector<D3DXIMAGE_INFO>			VECTEXTUREINFO;

public:
	static Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXSPRITE pSprite, const TCHAR* pPath, const UINT& iCnt = 1);
	virtual void Free(void);
};

