#pragma once
#include "GameObject.h"

class Player;
class Weapon : public GameObject
{
public:
	explicit Weapon(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Weapon();

public:
	virtual float Get_Y();

public:
	virtual HRESULT Ready_GameObject(Player* pOwner, int iType);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	INT Update_Rendering(float TimeDelta);
	INT Shoot_Bullet(float TimeDelta);

private:
	Player* m_pOwner;
	WEAPON m_tWeaponInfo{};
	D3DXVECTOR3 m_vCenter{ 0.f, 0.f, 0.f };

private:
	D3DXVECTOR3 m_vBulletDir{ 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vBulletPos{ 0.f, 0.f, 0.f };

private:
	float m_fSide = 1.f;
	float m_fAngle = 0.f;

private:
	D3DXMATRIX m_matWorld;
	D3DXMATRIX matScale, matRot, matRev, matParent, matFlip, matBullet;

private:
	bool m_bShoot = true;
	float m_fTime = 0.f;

public:
	static Weapon* Create(LPDIRECT3DDEVICE9 pGraphic_Device, Player* pOwner, int iType);
	virtual void Free();
};

