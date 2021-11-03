#pragma once
#include "DeviceMgr.h"

class Renderer;
class GameMgr;
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
	HRESULT Ready_Resource();

private:
	Renderer* m_pRenderer = nullptr;
	GameMgr* m_pGameMgr = nullptr;

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:
	static MainApp* Create();
	virtual void Free();
};

