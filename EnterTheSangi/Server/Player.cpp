#pragma once
#include "Player.h"


CPlayer::CPlayer() {};

CPlayer::~CPlayer() {};

srect CPlayer::GetCollisionBox()
{
	return { {m_position.x - PLAYER_HEIGHT / 2, m_position.y},
		{m_position.x + PLAYER_HEIGHT / 2, m_position.y + PLAYER_HEIGHT } };
}

float CPlayer::vGetHeightHf()
{
	return PLAYER_HEIGHT/2;
}; 

float CPlayer::vGetWidthHf()
{
	return PLAYER_WIDTH/2;
};

OBJECT::TYPE CPlayer::vGetObjectType() const
{
	return m_object_type;
};

D3DXVECTOR3 CPlayer::GetBodyColor()
{
	return m_body_color;
};

D3DXVECTOR3 CPlayer::GetClothColor()
{
	return m_cloth_color;
};

float CPlayer::GetHealth()
{
	return m_health;
};

STATE::TYPE CPlayer::GetState()
{
	return m_state;
};

char CPlayer::GetWeapon()
{
	return m_weapon;
};

void CPlayer::SetBodyColor(D3DXVECTOR3 body_color)
{
	m_body_color = body_color;
};

void CPlayer::SetClothColor(D3DXVECTOR3 cloth_color)
{
	m_cloth_color = cloth_color;
};

void CPlayer::SetState(STATE::TYPE state)
{
	m_state = state;
};

void CPlayer::SetHealth(char health)
{
	m_health = health;
};

void CPlayer::SetWeapon(char weapon)
{
	m_weapon = weapon;
}

void CPlayer::ChangeHealth(char delta_health)
{

	m_health += delta_health;
	if (m_health < 0)
		m_health = 0;
	else if (m_health > PLAYER_MAX_HP)
		m_health = PLAYER_MAX_HP;
};

void CPlayer::ChangeInvincibleMode()
{
	m_is_invincible = !m_is_invincible;
};

void CPlayer::StateLock()
{
	m_state_lock.lock();
}

void CPlayer::StateUnlock()
{
	m_state_lock.unlock();
}