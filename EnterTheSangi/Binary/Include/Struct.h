#pragma once
#include <string>
#include <math.h>
#include "Enum.h"

// For Rendering / DirectX

using namespace std;

typedef struct tagVertex_ViewPort_Texture
{
	D3DXVECTOR4		vPosition;
	D3DXVECTOR2		vTexUV;
}VTXVIEWPORTTEX;

typedef struct tagVertex_Texture
{
	D3DXVECTOR3		vPosition;
	D3DXVECTOR2		vTexUV;
}VTXTEX;

typedef struct tagIndex16
{
	unsigned short	_0, _1, _2;
}INDEX16;

typedef struct tagIndex32
{
	unsigned long	_0, _1, _2;
}INDEX32;



struct svector2
{
	short x;
	short y;
	
	svector2() {};
	svector2(svector2& svector)
	{
		this->x = svector.x;
		this->y = svector.y;
	}
	svector2(const svector2& svector)
	{
		this->x = svector.x;
		this->y = svector.y;
	}
	svector2(short _x, short _y)
	{
		this->x = _x;
		this->y = _y;
	}

	svector2 operator-(const svector2& rhs)
	{
		return svector2{this->x - rhs.x, this->y - rhs.y};
	}
};

struct fvector2
{
	float x;
	float y;

	fvector2() { x = 0.f; y = 0.f; };
	fvector2(svector2 svector)
	{
		this->x = (float)svector.x;
		this->y = (float)svector.y;
	}
	fvector2(const fvector2& fvector)
	{
		this->x = fvector.x;
		this->y = fvector.y;
	}
	fvector2(fvector2& fvector)
	{
		this->x = fvector.x;
		this->y = fvector.y;
	}
	fvector2(float _x, float _y)
	{
		this->x = _x;
		this->y = _y;
	}

	fvector2 GetUnitVector()
	{
		fvector2 unit_vector;
		float size = sqrt(pow(this->x, 2) + pow(this->y, 2));
		unit_vector.x = this->x / size;
		unit_vector.y = this->y / size;
		return unit_vector;
	}

	float AbsDotVector(fvector2& vector)
	{
		return abs(this->x * vector.x + this->y * vector.y);
	}

	fvector2 operator-(fvector2& rhs)
	{	
		return fvector2{ this->x - rhs.x,this->y - rhs.y };
	}
};

struct srect
{
	svector2 pos_1; //좌상단
	svector2 pos_2; //우하단
};
// For Client

typedef struct tagCustomInfo
{
public:
	tagCustomInfo() {}
	tagCustomInfo(D3DXVECTOR3 cloth, D3DXVECTOR3 body)
		:vCloth(cloth), vBody(body) {}
	D3DXVECTOR3 vCloth{};
	D3DXVECTOR3 vBody{};
}CUSTOMIZE;

typedef struct tagClientInfo
{
public:
	tagClientInfo() { }
	tagClientInfo(bool local, UINT idx, string str, D3DXVECTOR3 cloth, D3DXVECTOR3 body, D3DXVECTOR3 pos)
		:islocal(local), index(idx), name(str), custom(cloth, body), startpos(pos)	{}
	bool islocal = true;
	UINT index = 0;
	string name = "";
	CUSTOMIZE custom{};
	D3DXVECTOR3 startpos{};
}CLIENT;

typedef struct tagWeaponInfo
{
public:
	tagWeaponInfo() {}
	tagWeaponInfo(int iType, int iDamage, float fShotSpeed, float fBulletSpeed, float fDuration)
		: type(iType), damage(iDamage), shotspeed(fShotSpeed), bulletspeed(fBulletSpeed), duration(fDuration) {}
	int type = -1;
	int damage = 0;
	float shotspeed = 1.f;
	float bulletspeed = 1.f;
	float duration = 0.f;
	D3DXVECTOR2 bulletoffset = { 0.f, 0.f };
	float size = 1.f;
}WEAPON;

// For Server

struct player_info_for_packet
{
	char				    state;
	svector2				position;
	short					look;
	char					weapon;
	char					health;
	bool						is_shooting;
}typedef player_info;

struct bullet_info_for_packet
{
	char				id; // bullet id
	svector2        position;
	svector2        look;
}typedef bullet_info;

struct item_info_for_packet
{
	char				id; // item id
	svector2        position;
	svector2        look;
}typedef item_info;