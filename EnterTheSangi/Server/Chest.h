#pragma once 
#include "GameObject.h"
#include "Protocol.h"
#include "Enum.h"

class CChest :CGameObject
{
public:
	CChest();
	~CChest();

	float              vGetHeightHf();   //  return half height
	float              vGetWidthHf();    //  return half width
	OBJECT::TYPE       vGetObjectType(); // return object type

private:
	static const OBJECT::TYPE  m_object_type = OBJECT::CHEST;
	static const float m_height_hf;
	static const float m_width_hf;
};