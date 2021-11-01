#pragma once
class Target
{
public:
	explicit Target(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Target();

public:
	HRESULT Ready_Target(UINT iSizeX, UINT iSizeY, D3DFORMAT eFormat, D3DXCOLOR Color);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE hHandle);
	HRESULT SetUp_OnGraphicDev(UINT iIndex);
	HRESULT Release_OnGraphicDev(UINT iIndex);
	HRESULT Clear_Target();

public:
	HRESULT Ready_Debug_Buffer(float fX, float fY, float fSizeX, float fSizeY);
	HRESULT Render_Debug_Buffer();

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9			m_pTexture = nullptr; // ≈ÿΩ∫√ƒ
	LPDIRECT3DSURFACE9			m_pSurface = nullptr; // ∑ª¥ı≈∏∞Ÿ
	LPDIRECT3DSURFACE9			m_pOldSurface = nullptr; // ¿¸ ∑ª¥ı≈∏∞Ÿ

private:
	D3DXCOLOR					m_ClearColor;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

public:
	static Target* Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iSizeX, UINT iSizeY, D3DFORMAT eFormat, D3DXCOLOR Color);
	void Free();
};

