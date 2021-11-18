#pragma once
#include <d3dx9.h>
#include "Protocol.h"

class CGameObject
{
public:
	CGameObject();
	~CGameObject();


protected:
	void SetPosition(D3DXVECTOR2 pos);
	void SetLook(D3DXVECTOR2 look);

	OBJECT_CODE GetObjectCode();
	D3DXVECTOR2 GetPosition();
	D3DXVECTOR2 GetLook();


protected:
	OBJECT_CODE   m_object_code;
	D3DXVECTOR2   m_position;
	D3DXVECTOR2   m_look;


private:

};