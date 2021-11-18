#pragma once
#include "GameObject.h"

class Animation;
class Player : public GameObject
{
public:
	explicit Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Player();

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	static Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

