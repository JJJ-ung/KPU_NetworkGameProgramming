#pragma once
#include <d3dx9.h>

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	void SetPosition(D3DXVECTOR2 pos);
	void SetLook(D3DXVECTOR2 look);

	D3DXVECTOR2 GetPosition();
	D3DXVECTOR2 GetLook();

protected:
	D3DXVECTOR2   m_position;
	D3DXVECTOR2   m_look;


private:

};