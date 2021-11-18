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

OBJECT_CODE  CGameObject::GetObjectCode()
{
	return m_object_code;
}

D3DXVECTOR2 CGameObject::GetPosition()
{
	return m_position;
};

D3DXVECTOR2 CGameObject::GetLook()
{
	return m_look;
};

