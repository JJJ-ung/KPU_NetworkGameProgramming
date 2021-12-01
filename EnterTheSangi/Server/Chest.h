#pragma once 
#include "GameObject.h"
#include "Protocol.h"
#include "Enum.h"
#include "ObjectStatus.h"

class CChest :public CGameObject
{
public:
	CChest();
	~CChest();

	char                       GetID();
	char                       GetWeaponID();
	OBJECT_STATE::STATE        GetState();	

	virtual srect              GetCollisionBox();
	virtual float              vGetHeightHf();   //  return half height
	virtual float              vGetWidthHf();    //  return half width
	virtual OBJECT::TYPE       vGetObjectType()const; // return object type

	void                       SetID(char id);
	void                       SetState(OBJECT_STATE::STATE state);
	void                       SetWeaponID(char weapon_id);

private:
	char                       m_id;
	char                       m_weapon_id;
	OBJECT_STATE::STATE        m_state;
	const static OBJECT::TYPE  m_object_type = OBJECT::CHEST;
};