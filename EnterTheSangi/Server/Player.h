#pragma once
#include <d3dx9.h>
#include "Protocol.h"
#include "GameObject.h"
#include "Enum.h"
#include "ObjectStatus.h"

class CPlayer:public CGameObject
{
public:
	CPlayer();
	~CPlayer();
		 
	D3DXVECTOR3          GetBodyColor();
	D3DXVECTOR3          GetClothColor();
	float                GetHealth();

	virtual float        vGetHeightHf();   //  return half height
	virtual float        vGetWidthHf();    //  return half width
	virtual OBJECT::TYPE vGetObjectType() const; // return object type
	 
	void                 SetBodyColor(D3DXVECTOR3 body_color);
	void                 SetClothColor(D3DXVECTOR3 cloth_color);
	void                 SetHealth(float health);         // set player.health to health
	void                 ChangeHealth(float delta_health); // add delta_health to player.health
	void                 ChangeInvincibleMode();

private:
	STATE::TYPE          m_state;
	DIR::DIR             m_direction;
	char                 m_health;
	bool                 m_is_invincible;
	WEAPON::TYPE         m_weapon;
	float                m_cool_time;

	D3DXVECTOR3          m_body_color;
	D3DXVECTOR3          m_cloth_color;

	const static OBJECT::TYPE 
		                 m_object_type= OBJECT::PLAYER;
	static float         m_height_hf;
	static float         m_width_hf;
};