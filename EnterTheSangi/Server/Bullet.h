#pragma once 
#include "GameObject.h"
#include "Protocol.h"
#include "Enum.h"

class CBullet :public CGameObject
{
public:
	CBullet();

	~CBullet();

	virtual float         vGetHeightHf();   //  return half height
	virtual float         vGetWidthHf();    //  return half width
	virtual OBJECT::TYPE  vGetObjectType()const;
private:
	char                  m_type; //ÃÑ¾Ë Á¾·ù

	const static OBJECT::TYPE   m_object_type = OBJECT::BULLET;
};