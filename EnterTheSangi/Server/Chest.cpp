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

OBJECT_STATE::STATE CChest::GetState()
{
	return m_state;
}

srect CChest::GetCollisionBox()
{
	return { {m_position.x - 0 , m_position.y - 0  },{m_position.x + 0, m_position.y + 0  } }; //temp
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