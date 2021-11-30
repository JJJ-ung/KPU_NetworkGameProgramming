#pragma once
#include "GameObject.h"

class Texture;
class Mouse : public GameObject
{
public:
	explicit Mouse(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Mouse();

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	Texture* m_pTexture = nullptr;
	D3DXVECTOR3 m_vScale{ 2.f, 2.f, 2.f };
	D3DXVECTOR3 m_vCenter{ 0.f, 0.f, 0.f };

public:
	static Mouse* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

