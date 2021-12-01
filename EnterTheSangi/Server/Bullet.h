#pragma once 
#include <mutex>
#include <vector>
#include "GameObject.h"
#include "Protocol.h"
#include "Enum.h"


class CBullet :public CGameObject
{
public:
	CBullet();
	~CBullet();

	void                  SetBulletType(char bullet_type);
	void                  SetID(char id);
	void                  SetState(OBJECT_STATE::STATE state);

	char                  GetBulletType();
	char                  GetID();
	OBJECT_STATE::STATE   GetState();

	virtual srect         GetCollisionBox();
	virtual float         vGetHeightHf();   //  return half height
	virtual float         vGetWidthHf();    //  return half width
	virtual OBJECT::TYPE  vGetObjectType()const;

	void                  StateLock();
	void                  StateUnlock();
private:
	char                  m_bullet_type; //ÃÑ¾Ë Á¾·ù
	char                  m_id;   
	OBJECT_STATE::STATE   m_state;
	mutex                 m_state_lock;
	const static OBJECT::TYPE m_object_type = OBJECT::BULLET;

	
};

