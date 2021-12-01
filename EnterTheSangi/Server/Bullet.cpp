#include "Bullet.h"

CBullet::CBullet()
{

};

CBullet::~CBullet()
{

};

void CBullet::SetBulletType(char bullet_type)
{
	m_bullet_type = bullet_type;
}

void CBullet::SetID(char id)
{
	m_id = id;
}


void CBullet::SetState(OBJECT_STATE::STATE state)
{
	m_state = state;
}

char CBullet::GetBulletType()
{
	return m_bullet_type;
}

char CBullet::GetID()
{
	return m_id;
}

OBJECT_STATE::STATE CBullet::GetState()
{
	return m_state;
}

srect CBullet::GetCollisionBox()
{
	return { {m_position.x - 0 , m_position.y - 0   },{m_position.x + 0, m_position.y + 0  } };//temp
};

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

void CBullet::SetDirection(D3DXVECTOR3 dir)
{
	m_direction = dir;
}

D3DXVECTOR3 CBullet::GetDirection()
{
	return m_direction;
}

void CBullet::SetBulletSpeed(float speed)
{
	m_fSpeed = speed;
}

float CBullet::GetBulletSpeed()
{
	return m_fSpeed;
}