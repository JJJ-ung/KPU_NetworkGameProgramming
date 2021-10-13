#pragma once

class DeviceMgr
{
	DECLARE_SINGLETON(DeviceMgr)

public:
	enum WINMODE { MODE_FULL, MODE_WIN };

private:
	explicit	DeviceMgr();
	virtual		~DeviceMgr();

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev() { return m_pGraphicDev; }
	LPD3DXSPRITE				Get_Sprite() { return m_pSprite; }

public:
	HRESULT		Ready_GraphicDev(HWND hWnd,
																WINMODE eMode,
																const UINT& iSizeX,
																const UINT& iSizeY,
																LPDIRECT3DDEVICE9* ppGraphicDev);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End();

private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	LPD3DXSPRITE			m_pSprite = nullptr;

public:
	virtual void Free(void);
};

