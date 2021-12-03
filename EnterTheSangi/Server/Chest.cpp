#pragma once
#include "Chest.h"

CChest::CChest()
{

};

CChest::~CChest()
{

};

char CChest::GetID()
{
	return m_id;
}

char CChest::GetWeaponID()
{
	return m_weapon_id;
}

OBJECT_STATE::STATE CChest::GetState()
{
	return m_state;
}

srect CChest::GetCollisionBox()
{
	return { {m_position.x - CHEST_WIDTH/2 , m_position.y - CHEST_HEIGHT / 2  },{m_position.x + CHEST_WIDTH / 2, m_position.y + CHEST_HEIGHT/2  } }; //temp
};


float CChest::vGetHeightHf()
{
	return CHEST_HEIGHT/2;
}; 

float CChest::vGetWidthHf()
{
	return CHEST_WIDTH/2;
};  

OBJECT::TYPE CChest::vGetObjectType() const
{
	return m_object_type;
};

void CChest::SetID(char id)
{
	m_id = id;
}

void CChest::SetState(OBJECT_STATE::STATE state)
{
	m_state = state;
}

void CChest::SetWeaponID(char weapon_id)
{
	m_weapon_id = weapon_id;
}