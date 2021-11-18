#pragma once
#include "GameObject.h"

class CPlayer : public GameObject
{
public:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

