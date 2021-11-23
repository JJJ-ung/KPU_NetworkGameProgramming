#pragma once 
#include "GameObject.h"
#include "Protocol.h"
#include "Enum.h"

class CBullet :CGameObject
{
public:
	CBullet();
	~CBullet();

	virtual float         vGetHeightHf();   //  return half height
	virtual float         vGetWidthHf();    //  return half width
	virtual OBJECT::TYPE  vGetObjectType();
private:
	char          m_type; //ÃÑ¾Ë Á¾·ù

	static OBJECT::TYPE  m_object_type;
};