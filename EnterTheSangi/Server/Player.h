#pragma once
#include <d3dx9.h>
#include "Protocol.h"
#include "GameObject.h"

class CPlayer:CGameObject
{
public:
	explicit CPlayer();
	~CPlayer();

	D3DXVECTOR4 GetBodyColor();
	D3DXVECTOR4 GetClothColor();
	char        GetHealth();

	void        SetBodyColor(D3DXVECTOR4 body_color);
	void        SetClothColor(D3DXVECTOR4 cloth_color);
	void        SetHealth(char health);

	void        ChangeInvincibleMode();
	bool        IsInvincibleMode();

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