#include "Chest.h"

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

OBJECT::TYPE CChest::vGetObjectType() const
{
	return m_object_type;
};