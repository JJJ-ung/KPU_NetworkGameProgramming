#pragma once
#include "Player.h"

class NetworkPlayer : public Player
{
public:
	explicit NetworkPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~NetworkPlayer();

public:
	virtual HRESULT Ready_GameObject(CLIENT t);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual INT Recv_Networking();

public:
	static NetworkPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CLIENT t);
	virtual void Free();
};

