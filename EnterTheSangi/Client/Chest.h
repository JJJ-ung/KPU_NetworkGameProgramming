#pragma once
#include "GameObject.h"

class Texture;
class Chest : public GameObject
{
public:
	explicit Chest(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Chest();

public:
	virtual HRESULT Ready_GameObject(sc_packet_put_chest tInfo);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	HRESULT Render_ChestBottom();
	HRESULT Render_ChestTop();
	HRESULT Render_Weapon();

private:
	float m_fWeaponAngle = 0.f;
	int m_iWeaponID = -1;
	Texture* m_pWeaponTexture = nullptr;
	D3DXVECTOR3 m_vCenter{ 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vWeaponCenter{ 16.f, 16.f, 0.f };
	D3DXVECTOR3 m_vWeaponPosition{ 0.f, 0.f, 0.f };

public:
	static Chest* Create(LPDIRECT3DDEVICE9 pGraphic_Device, sc_packet_put_chest tInfo);
	virtual void Free();
};

