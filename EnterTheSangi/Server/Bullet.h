#pragma once 
#include "GameObject.h"
#include "Protocol.h"
#include "Enum.h"

class CBullet :CGameObject
{
public:
	CBullet();
	~CBullet();

	float         vGetHeightHf();   //  return half height
	float         vGetWidthHf();    //  return half width
	OBJECT::TYPE  vGetObjectType();
private:
	char          m_type; //�Ѿ� ����

	static const OBJECT::TYPE  m_object_type = OBJECT::BULLET;
};