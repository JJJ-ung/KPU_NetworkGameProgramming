#pragma once
#include <d3dx9.h>
#include "Enum.h"
#include "Struct.h"


class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	void                 SetPosition(svector2 pos);
	void                 SetLook(short look);

	svector2          GetPosition();
	short          GetLook();

	virtual float        vGetHeightHf() =0;   // return half height
	virtual float        vGetWidthHf() = 0;    // return half width
	virtual OBJECT::TYPE vGetObjectType() const = 0; // return object type

	bool operator<(const CGameObject& robject) const ;
	bool operator==(const CGameObject& robject)const ;
protected:
	svector2          m_position;
	short			  m_look;

private:

};