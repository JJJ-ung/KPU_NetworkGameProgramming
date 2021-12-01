#pragma once
#include "struct.h"
#include <d3dx9.h>
#include <vector>

using namespace std;

//constexpr const char* SERVER_IP = "220.94.221.20";
constexpr const char* SERVER_IP = "121.139.87.66";
//constexpr const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 5000;
const int MAX_CLIENTS = 3;
const int MAX_BULLETS = 50;
const int MAX_CHESTS = 10;
const int MAX_MAP_RECT = 5;
const int MAX_WEAPON = 14;

//constexpr unsigned int MAX_BUF_SIZE = 256;
#define  MAX_NAME_SIZE 16
#define  BUF_SIZE 1024


// client to server packet
const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_CHANGE_COLOR = 2;
const char CS_PACKET_READY = 3;
const char CS_PACKET_PLAYER_INFO = 4;
const char CS_PACKET_SHOOT_BULLET = 5;

const char CS_PACKET_TYPE_END = 6;

//server to client packet
const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_LOGIN_OTHER_CLIENT = 2;
const char SC_PACKET_REMOVE_OBJECT = 3;
const char SC_PACKET_CHANGE_COLOR = 4;
const char SC_PACKET_READY = 5;
const char SC_PACKET_ALL_READY = 6;
const char SC_PACKET_GAME_STATE = 7;
const char SC_PACKET_PUT_BULLET = 8;
const char SC_PACKET_REMOVE_BULLET = 9;
const char SC_PACKET_PUT_CHEST = 10;
const char SC_PACKET_MOVE_CHEST = 11;
const char SC_PACKET_REMOVE_CHEST = 12;
const char SC_PACKET_CHANGE_WEAPON = 13;

const char SC_PACKET_TYPE_END = 14;


//--------------------------------------------------
//         client to server packet 
//--------------------------------------------------
struct cs_packet_login
{
	unsigned char  size;
	char           type;
	char           name[MAX_NAME_SIZE];
};

struct cs_packet_change_color
{
	unsigned char  size;
	char           type;
	D3DXVECTOR3    body_color;
	D3DXVECTOR3    cloth_color;
};

struct cs_packet_ready
{
	unsigned char  size;
	char           type;
	bool           is_ready;
};

struct cs_packet_player_info
{
	unsigned char  size;
	char           type;
	char		   m_state;
	svector2       m_position;
	short		   m_look;
};

struct cs_packet_shoot_bullet
{
	unsigned char  size;
	char           type;
	char           bullet_type;
	short		   angle;
	svector2       position;
	svector2	   direction;
};


//--------------------------------------------------
//         server to client packet 
//--------------------------------------------------
struct sc_packet_login_ok
{
	unsigned char  size;
	char           type;
	char           id;
	D3DXVECTOR3    body_color;   //커스터마이징 초기값
	D3DXVECTOR3    cloth_color;  //커스터마이징 초기값
	bool           is_ready;
};

struct sc_packet_login_other_client
{
	unsigned char  size;
	char           type;
	char           id;
	char           name[MAX_NAME_SIZE];
	D3DXVECTOR3    body_color;
	D3DXVECTOR3    cloth_color;
	bool           is_ready;
};

struct sc_packet_remove_object
{
	unsigned char  size;
	char           type;
	char           id;
};

struct sc_packet_change_color
{
	unsigned char  size;
	char           type;
	char           id;
	D3DXVECTOR3    body_color;
	D3DXVECTOR3    cloth_color;
};

struct sc_packet_ready    //준비 설정, 해제 둘 다 해당 패킷 사용
{
	unsigned char  size;
	char           type;
	char           id;
	bool           is_ready;
};

struct sc_packet_all_ready // 모든 플레이어가 준비 완료임을 알림
{
	unsigned char   size;
	char            type;
};

struct sc_packet_game_state  //매 프레임마다 게임 전체 정보 전달
{
	unsigned char  size;
	char           type;
	player_info_for_packet player[3];
	//아이템 정보 추가 필요

};

struct sc_packet_put_bullet
{
	unsigned char  size;
	char           type;
	char           bullet_type;
	char           bullet_id;
	short		   angle;
	svector2       position;
	svector2	   direction;
};

struct sc_packet_remove_bullet
{
	unsigned char  size;
	char           type;
	char           bullet_id;
};

struct sc_packet_put_chest
{
	unsigned char  size;
	char           type;
	char           chest_id;
	char           weapon_id;
	svector2       position;
};

struct sc_packet_move_chest
{
	unsigned char  size;
	char           type;
	char           chest_id;
	char           weapon_id;
	svector2       position;
};

struct sc_packet_remove_chest
{
	unsigned char  size;
	char           type;
	char           chest_id;
};

struct sc_packet_change_weapon
{
	unsigned char  size;
	char           type;
	char           id;
	char           weapon_id;
};