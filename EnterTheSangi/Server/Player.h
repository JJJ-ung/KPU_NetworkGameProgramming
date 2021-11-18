#pragma once
#include <d3dx9.h>
#include "Protocol.h"
#include "GameObject.h"

class CPlayer:CGameObject
{
public:
	explicit CPlayer();
	~CPlayer();

private:
	STATE         m_e_state;
	DIR           m_e_direction;	
	char          m_health;
	bool          m_is_invincible;
	WEAPON        m_e_weapon;
	float         m_cool_time;

	D3DXVECTOR4   m_body_color;
	D3DXVECTOR4   m_cloth_color;
};