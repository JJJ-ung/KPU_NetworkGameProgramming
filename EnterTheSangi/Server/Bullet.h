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
	void				  SetDirection(D3DXVECTOR3 dir);
	void				  SetBulletSpeed(float speed);
	void				  SetBulletPosition(D3DXVECTOR3 pos);

	float			      GetBulletSpeed();
	D3DXVECTOR3			  GetDirection();
	D3DXVECTOR3			  GetBulletPosition();
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
	float				  m_fSpeed;
	OBJECT_STATE::STATE   m_state;
	mutex                 m_state_lock;
	D3DXVECTOR3			  m_direction;
	D3DXVECTOR3			  m_bullet_pos;
	const static OBJECT::TYPE m_object_type = OBJECT::BULLET;

	
};

