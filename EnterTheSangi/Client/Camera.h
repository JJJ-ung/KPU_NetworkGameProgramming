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
	D3DXMATRIX m_matView;
	D3DXVECTOR3 m_vEye = D3DXVECTOR3(0.f, 0.f, -1.f);
	D3DXVECTOR3 m_vAt = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vUP = D3DXVECTOR3(0.f, 1.f, 0.f);

private:
	const D3DXVECTOR3 m_vAspect = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, 0.f);
	const D3DXVECTOR3 m_vAspectEye = D3DXVECTOR3(0.f, 0.f, -1.f);

public:
	static Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

