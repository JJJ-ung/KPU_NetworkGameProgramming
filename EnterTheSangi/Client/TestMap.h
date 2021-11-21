#pragma once
#include "GameObject.h"

class TestMap : public GameObject
{
public:
	explicit TestMap(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~TestMap();

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	static TestMap* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

