#pragma once
#include "GameObject.h"

class Player;
class Texture;
class Weapon : public GameObject
{
public:
	explicit Weapon(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Weapon();

public:
	virtual HRESULT Ready_GameObject(Player* pOwner);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	Player* m_pOwner;
	Texture* m_pTexture = nullptr;


public:
	static Weapon* Create(LPDIRECT3DDEVICE9 pGraphic_Device, Player* pOwner);
	virtual void Free();
};

