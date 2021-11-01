#pragma once
#include "DeviceMgr.h"
#include "Player.h"

class Renderer;
class DeviceMgr;
class MainApp
{
public:
	explicit MainApp();
	~MainApp();

public:
	HRESULT Ready_MainApp();
	int Update_MainApp(const float TimeDelta);
	HRESULT Render_MainApp();

public:
	HRESULT Ready_Default(DeviceMgr::WINMODE eMode, const UINT& iSizeX, const UINT& iSizeY);
	HRESULT Ready_Shader();

private:
	Renderer* m_pRenderer = nullptr;
	Player* m_pPlayer = nullptr;

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:
	static MainApp* Create();
	virtual void Free();
};

