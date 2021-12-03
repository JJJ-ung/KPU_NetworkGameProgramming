#pragma once
#include <d3dx9.h>
#include <mutex>
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
	STATE::TYPE          GetState();
	char                 GetWeapon();

	virtual srect        GetCollisionBox();
	virtual float        vGetHeightHf();   //  return half height
	virtual float        vGetWidthHf();    //  return half width
	virtual OBJECT::TYPE vGetObjectType() const; // return object type
	 
	void                 SetBodyColor(D3DXVECTOR3 body_color);
	void                 SetClothColor(D3DXVECTOR3 cloth_color);
	void                 SetState(STATE::TYPE state);
	void                 SetHealth(char health);         // set player.health to health
	void                 SetWeapon(char weapon);
	void                 ChangeHealth(char delta_health); // add delta_health to player.health
	void                 ChangeInvincibleMode();

	void                 StateLock();
	void                 StateUnlock();
private:
	STATE::TYPE          m_state;
	mutex                m_state_lock;
	DIR::DIR             m_direction;
	char                 m_health;
	bool                 m_is_invincible;
	char                 m_weapon;

	D3DXVECTOR3          m_body_color;
	D3DXVECTOR3          m_cloth_color;	

	const static OBJECT::TYPE
		m_object_type = OBJECT::PLAYER;
};