#pragma once
#include "DeviceMgr.h"

class DeviceMgr;
class MainApp
{
public:
	explicit MainApp();
	~MainApp();

public:
	HRESULT Ready_MainApp();
	int Update_MainApp(float TimeDelta);
	HRESULT Render_MainApp();

public:
	HRESULT Ready_Default(DeviceMgr::WINMODE eMode, const UINT& iSizeX, const UINT& iSizeY);

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:
	static MainApp* Create();
	virtual void Free();
};

