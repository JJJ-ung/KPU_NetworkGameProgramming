#pragma once
#include "GameMgr.h"

class Scene
{
public:
	explicit Scene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

protected:
	GameMgr*							m_pGameMgr = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

public:
	virtual void Free();
};

