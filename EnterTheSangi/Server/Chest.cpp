#include "Chest.h"

OBJECT::TYPE CChest::m_object_type = OBJECT::CHEST;
float CChest::m_height_hf;
float CChest::m_width_hf;

CChest::CChest()
{

};

CChest::~CChest()
{

};

float CChest::vGetHeightHf()
{
	return m_height_hf;
}; 

float CChest::vGetWidthHf()
{
	return m_width_hf;
};  

OBJECT::TYPE CChest::vGetObjectType()
{
	return m_object_type;
};