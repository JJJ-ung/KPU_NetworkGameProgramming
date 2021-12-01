#pragma once
#include "GameObject.h"

class MainMap : public GameObject
{
public:
	explicit MainMap(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~MainMap();

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	D3DXVECTOR3 m_vCenter = { 0.f, 0.f, 0.f };

public:
	static MainMap* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

