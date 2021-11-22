#pragma once
#include "Player.h"

OBJECT::TYPE CPlayer::m_object_type = OBJECT::PLAYER;
float CPlayer::m_height_hf;
float CPlayer::m_width_hf;

CPlayer::CPlayer() {};

CPlayer::~CPlayer() {};


float CPlayer::vGetHeightHf()
{
	return m_height_hf;
}; 

float CPlayer::vGetWidthHf()
{
	return m_width_hf;
};

OBJECT::TYPE CPlayer::vGetObjectType()
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

void CPlayer::SetBodyColor(D3DXVECTOR3 body_color)
{
	m_body_color = body_color;
};

void CPlayer::SetClothColor(D3DXVECTOR3 cloth_color)
{
	m_cloth_color = cloth_color;
};

void CPlayer::SetHealth(float health)
{
	m_health = health;
};

void CPlayer::ChangeHealth(float delta_health)
{

	m_health += delta_health;
	if (m_health < 0)
		m_health = 0;
	else if (m_health > 100)
		m_health = 100;		
};

void CPlayer::ChangeInvincibleMode()
{
	m_is_invincible = !m_is_invincible;
};