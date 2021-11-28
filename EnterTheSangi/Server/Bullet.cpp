#include "Bullet.h"

CBullet::CBullet()
{

};

CBullet::~CBullet()
{

};

void CBullet::SetType(char type)
{
	m_type = type;
}

void CBullet::SetID(char id)
{
	m_id = id;
}


void CBullet::SetState(OBJECT_STATE::STATE state)
{
	m_state = state;
}

char CBullet::GetType()
{
	return m_type;
}

char CBullet::GetID()
{
	return m_id;
}

OBJECT_STATE::STATE CBullet::GetState()
{
	return m_state;
}

float CBullet::vGetHeightHf()
{
	return 0.f; //temp
}; 

float CBullet::vGetWidthHf()
{
	return 0.f; //temp
};

OBJECT::TYPE  CBullet::vGetObjectType() const
{
	return m_object_type;
};

void CBullet::StateLock()
{
	m_state_lock.lock();
}

void CBullet::StateUnlock()
{
	m_state_lock.unlock();
}