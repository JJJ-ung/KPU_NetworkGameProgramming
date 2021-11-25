#pragma once
#include <d3dx9.h>
#include "Enum.h"



class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	void                 SetPosition(D3DXVECTOR2 pos);
	void                 SetLook(D3DXVECTOR2 look);

	D3DXVECTOR2          GetPosition();
	D3DXVECTOR2          GetLook();

	virtual float        vGetHeightHf() =0;   // return half height
	virtual float        vGetWidthHf() = 0;    // return half width
	virtual OBJECT::TYPE vGetObjectType() const = 0; // return object type

	bool operator<(const CGameObject& robject) const ;
	bool operator==(const CGameObject& robject)const ;
protected:
	D3DXVECTOR2          m_position;
	D3DXVECTOR2          m_look;

private:

};