#pragma once 
#include "GameObject.h"
#include "Protocol.h"
#include "Enum.h"

class CChest :public CGameObject
{
public:
	CChest();
	~CChest();

	virtual float              vGetHeightHf();   //  return half height
	virtual float              vGetWidthHf();    //  return half width
	virtual OBJECT::TYPE       vGetObjectType()const; // return object type

private:
	static float               m_height_hf;
	static float               m_width_hf;
	const static OBJECT::TYPE  m_object_type = OBJECT::CHEST;
};