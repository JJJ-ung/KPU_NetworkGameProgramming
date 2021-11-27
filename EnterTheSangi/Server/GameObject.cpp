#include "GameObject.h"


CGameObject::CGameObject() {};
CGameObject::~CGameObject() {};

void CGameObject::SetPosition(svector2 pos)
{
	m_position = pos;
};

void CGameObject::SetLook(svector2 look)
{
	m_look = look;
};

svector2 CGameObject::GetPosition()
{
	return m_position;
};

svector2 CGameObject::GetLook()
{
	return m_look;
};

bool CGameObject::operator<(const CGameObject& robject) const
{
	if (vGetObjectType() < robject.vGetObjectType()) return true;
	else return false;
}

bool CGameObject::operator==(const CGameObject& robject)const
{
	if (vGetObjectType() == robject.vGetObjectType()) return true;
	else return false;
}