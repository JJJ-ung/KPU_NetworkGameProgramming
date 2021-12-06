#pragma once
#include "GameObject.h"

class Texture;
class Health : public GameObject
{
public:
	explicit Health(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Health();

public:
	int& Get_Health() { return m_iHealth; }

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	HRESULT Render_Heart(LPD3DXEFFECT pEffect, int iIdx, int iTex);

private:
	int m_iHealth = 0;
	Texture* m_pTexture = nullptr;
	D3DXVECTOR3 m_vCenter{ 0.f, 0.f, 0.f };

public:
	static Health* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

