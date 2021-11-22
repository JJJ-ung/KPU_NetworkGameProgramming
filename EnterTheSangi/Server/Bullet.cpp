#include "Bullet.h"

OBJECT::TYPE  CBullet::m_object_type = OBJECT::BULLET;

CBullet::CBullet()
{

};

CBullet::~CBullet()
{

};


float CBullet::vGetHeightHf()
{
	return 0.f; //temp
}; 

float CBullet::vGetWidthHf()
{
	return 0.f; //temp
};

OBJECT::TYPE  CBullet::vGetObjectType()
{
	return m_object_type;
};