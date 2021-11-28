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

// For Server

struct player_info_for_packet
{
	char            id;
	STATE::TYPE     state;
	svector2        position;
	short			look;
	DIR::DIR        direction;
	WEAPON::TYPE    weapon;
	char            health;
	bool            is_shooting;
	float           cool_time;
}typedef player_info;

struct bullet_info_for_packet
{
	char            id; // bullet id
	svector2        position;
	svector2        look;
}typedef bullet_info;

struct item_info_for_packet
{
	char            id; // item id
	svector2        position;
	svector2        look;
}typedef item_info;