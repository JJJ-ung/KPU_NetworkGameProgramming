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
	bool& Get_Delete() { return m_bDeleteWeapon; }
	float& Get_Angle() { return m_fAngle; }
	float& Get_Side() { return m_fSide; }

public:
	virtual HRESULT Ready_GameObject(Player* pOwner, int iType, bool bNetwork);
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
	float m_fTime = 0.f;
	bool m_bShoot = true;
	bool m_bNetwork = false;
	bool m_bDeleteWeapon = false;

public:
	static Weapon* Create(LPDIRECT3DDEVICE9 pGraphic_Device, Player* pOwner, int iType, bool bNetwork);
	virtual void Free();
};

