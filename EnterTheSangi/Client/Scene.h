#pragma once
#include "GameMgr.h"

class InputMgr;
class NetworkMgr;
class Scene
{
public:
	explicit Scene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

public:
	virtual HRESULT Setup_Recv(char type, void* pRecv);

protected:
	bool m_bChangeScene = false;

protected:
	InputMgr*							m_pInputMgr = nullptr;
	GameMgr*							m_pGameMgr = nullptr;
	NetworkMgr*					m_pNetworkMgr = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

public:
	virtual void Free();
};

