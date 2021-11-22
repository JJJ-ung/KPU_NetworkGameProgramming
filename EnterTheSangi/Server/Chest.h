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
	static OBJECT::TYPE  m_object_type;
	static float m_height_hf;
	static float m_width_hf;
};