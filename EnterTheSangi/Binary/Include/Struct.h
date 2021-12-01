#pragma once
#include <string>
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