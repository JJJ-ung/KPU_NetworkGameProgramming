#include "GameObject.h"


CGameObject::CGameObject() {};
CGameObject::~CGameObject() {};

void CGameObject::SetPosition(D3DXVECTOR2 pos)
{
	m_position = pos;
};

void CGameObject::SetLook(D3DXVECTOR2 look)
{
	m_look = look;
};

D3DXVECTOR2 CGameObject::GetPosition()
{
	return m_position;
};

D3DXVECTOR2 CGameObject::GetLook()
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