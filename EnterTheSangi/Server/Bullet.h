#pragma once 
#include <mutex>
#include "GameObject.h"
#include "Protocol.h"
#include "Enum.h"



class CBullet :public CGameObject
{
public:
	CBullet();
	~CBullet();

	void                  SetType(char type);
	void                  SetID(char id);
	void                  SetState(OBJECT_STATE::STATE state);

	char                  GetType();
	char                  GetID();
	OBJECT_STATE::STATE   GetState();
	virtual float         vGetHeightHf();   //  return half height
	virtual float         vGetWidthHf();    //  return half width
	virtual OBJECT::TYPE  vGetObjectType()const;

	void                  StateLock();
	void                  StateUnlock();
private:
	char                  m_type; //ÃÑ¾Ë Á¾·ù
	char                  m_id;   
	OBJECT_STATE::STATE   m_state;
	mutex                 m_state_lock;
	const static OBJECT::TYPE m_object_type = OBJECT::BULLET;
};