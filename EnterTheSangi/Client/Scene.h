#pragma once
#include "GameMgr.h"

class InputMgr;
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
	bool m_bChangeScene = false;

protected:
	InputMgr*							m_pInputMgr = nullptr;
	GameMgr*							m_pGameMgr = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

public:
	virtual void Free();
};

