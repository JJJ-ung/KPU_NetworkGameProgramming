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
	void                 SetLook(svector2 look);

	svector2          GetPosition();
	svector2          GetLook();

	virtual float        vGetHeightHf() =0;   // return half height
	virtual float        vGetWidthHf() = 0;    // return half width
	virtual OBJECT::TYPE vGetObjectType() const = 0; // return object type

	bool operator<(const CGameObject& robject) const ;
	bool operator==(const CGameObject& robject)const ;
protected:
	svector2          m_position;
	svector2          m_look;

private:

};