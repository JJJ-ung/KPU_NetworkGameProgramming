#pragma once
#include "GameObject.h"
class Camera : public GameObject
{
public:
	explicit Camera(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Camera();

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	D3DXMATRIX m_matProj;
	D3DXMATRIX m_matView;

public:
	static Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

