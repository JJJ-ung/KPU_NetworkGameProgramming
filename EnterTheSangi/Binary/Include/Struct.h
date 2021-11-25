#pragma once
#include "Enum.h"
// For Rendering / DirectX

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



// For Client

typedef struct tagCustomInfo
{
	D3DXVECTOR3 vCloth{};
	D3DXVECTOR3 vBody{};
}CUSTOMIZE;


// For Server

struct short_vector2_for_packet
{
	//원래 값의 1/10
	short x;
	short y;
}typedef svector2;

struct player_info_for_packet
{
	char            id;
	STATE::TYPE     state;
	svector2        position;
	svector2        look;
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