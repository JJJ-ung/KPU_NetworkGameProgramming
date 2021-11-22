#pragma once
#include <d3dx9.h>
#include <vector>

using namespace std;

constexpr const char* SERVER_IP = "255.255.255.255";
const int SERVER_PORT = 5000;
const int MAX_CLIENTS = 3;
constexpr unsigned int MAX_BUF_SIZE = 256;
#define  MAX_NAME_SIZE 16
#define	 BUF_SIZE 512

enum WEAPON
{


};
struct Weapon
{
	WEAPON      e_weapon;
	float       weapon_length;
};

enum BULLET
{


};
struct Bullet
{

	//클라 화이팅

};

enum DIR { DIR_LT, DIR_L, DIR_LB, DIR_CT, DIR_C, DIR_CB, DIR_RT, DIR_R, DIR_RB };
enum STATE { IDLE, RUN, DODGE, DEAD, HAPPY };

struct Item
{
	//id 필요한가?
	D3DXVECTOR2   v_position;
	WEAPON        e_weapon;     //무기 종류 (아이템에서 무기외에 다른 것도 나오게 하려면 수정 필요)
};

//struct Player  //player 정보
//{
//	char          id;
//	char          name[MAX_NAME_SIZE];
//	D3DXVECTOR4   body_color;
//	D3DXVECTOR4   cloth_color;
//
//	D3DXVECTOR2   v_position;
//	D3DXVECTOR2   v_look;
//	DIR           e_direction;
//	STATE         e_state;
//	char          health;
//	bool          is_invincible;
//	WEAPON        e_weapon;
//	float         cool_time;
//};

struct GameStatePlayer   //game_state 전송에 필요없는 데이터 제외한 구조체
{
	char          id;

	D3DXVECTOR2   v_position;
	D3DXVECTOR2   v_look;
	DIR           e_direction;
	STATE         e_state;
	char          health;
	bool          is_invincible;
	WEAPON        e_weapon;
	float         cool_time;
};


const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_CHANGE_COLOR = 2;
const char CS_PACKET_READY = 3;

const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_PUT_OBJECT = 2;
const char SC_PACKET_REMOVE_OBJECT = 3;
const char SC_PACKET_CHANGE_COLOR = 4;
const char SC_PACKET_READY = 5;
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

struct sc_packet_put_object
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

struct sc_packet_ready
{
	unsigned char  size;
	char           type;
	char           id;
	bool           is_ready;
};

struct sc_packet_game_start // 3명이 모두 준비를 마쳐 게임을 시작
{
	unsigned char  size;
	char           type;
};

struct sc_packet_game_state  //매 프레임마다 게임 전체 정보 전달
{
	unsigned char   size;
	char            type;

	//Player info
	GameStatePlayer player[3];
	vector<Item>    items;
	vector<Bullet>  bullets;
};
