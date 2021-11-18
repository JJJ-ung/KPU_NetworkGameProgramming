#pragma once
#include "Player.h"

D3DXVECTOR4 CPlayer::GetBodyColor()
{
	return m_body_color;
};

D3DXVECTOR4 CPlayer::GetClothColor()
{
	return m_cloth_color;
};

char CPlayer::GetHealth()
{
	return m_health;
}

void CPlayer::SetBodyColor(D3DXVECTOR4 body_color)
{
	m_body_color = body_color;
};

void CPlayer::SetClothColor(D3DXVECTOR4 cloth_color)
{
	m_cloth_color = cloth_color;
};

void CPlayer::SetHealth(char health)
{
	m_health = health;
}

void CPlayer::ChangeInvincibleMode()
{
	m_is_invincible = !m_is_invincible;
};

bool CPlayer::IsInvincibleMode()
{
	return m_is_invincible;
};

CPlayer::CPlayer() 
{
	m_object_code = E_PLAYER;
};

CPlayer::~CPlayer() {};