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
	virtual HRESULT Ready_GameObject(Player* pOwner, int iType);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	Player* m_pOwner;
	Texture* m_pTexture = nullptr;
	WEAPON m_tWeaponInfo{};
	D3DXVECTOR3 m_vCenter{ 0.f, 0.f, 0.f };

private:
	//float m_fRotation = 0.f;
	//D3DXVECTOR3 m_vOffset{ 0.f, 0.f, 0.f };

	float m_fSide = 1.f;
	float m_fAngle = 0.f;
	D3DXVECTOR3 m_vDir{ 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vParent{ 0.f, 0.f, 0.f };

public:
	static Weapon* Create(LPDIRECT3DDEVICE9 pGraphic_Device, Player* pOwner, int iType);
	virtual void Free();
};

