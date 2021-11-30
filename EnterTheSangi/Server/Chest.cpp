#include "Chest.h"

float CChest::m_height_hf;
float CChest::m_width_hf;

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

float CChest::vGetHeightHf()
{
	return m_height_hf;
}; 

float CChest::vGetWidthHf()
{
	return m_width_hf;
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