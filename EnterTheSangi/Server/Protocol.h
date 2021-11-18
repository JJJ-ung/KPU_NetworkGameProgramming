#pragma once
#include <d3dx9.h>
#include <vector>

using namespace std;

constexpr const char* SERVER_IP = "255.255.255.255";
constexpr int SERVER_PORT = 5000;
constexpr int MAX_CLIENTS = 3;

#define  MAX_NAME_SIZE 16

struct sc_other_client_info //로그인시 혹은 타 플레이어 로그인시 다른 클라이언트 접속 정보
{
	unsigned char size;
	char        type;
	int           id;
	char          name[MAX_NAME_SIZE];
	D3DXVECTOR4   body_color;
	D3DXVECTOR4   cloth_color;
	bool          is_ready;
};

struct cs_change_color
{
	unsigned char size;
	char        type;
	int           id;
	D3DXVECTOR4   body_color;
	D3DXVECTOR4   cloth_color;
};

struct sc_change_color
{
	unsigned char size;
	char        type;
	int           id;
	D3DXVECTOR4   body_color;
	D3DXVECTOR4   cloth_color;
};

struct cs_ready
{
	unsigned char size;
	char        type;
	int           id;
	bool          is_ready;
};

struct sc_ready
{
	unsigned char size;
	char        type;
	int           id;
	bool          is_ready;
};

struct sc_game_start // 3명이 모두 준비를 마쳐 게임을 시작
{
	unsigned char size;
	char        type;


};

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

struct Player  //player 정보
{
	char          id;
	char          name[MAX_NAME_SIZE];
	D3DXVECTOR4   body_color;
	D3DXVECTOR4   cloth_color;

	D3DXVECTOR2   v_position;
	D3DXVECTOR2   v_look;
	DIR           e_direction;
	STATE         e_state;
	char          health;
	bool          is_invincible;
	WEAPON        e_weapon;
	float         cool_time;
};

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

struct sc_game_state  //매 프레임마다 게임 전체 정보 전달
{
	unsigned char size;
	char        type;

	//Player info
	GameStatePlayer player[3];
	vector<Item>    items;
	vector<Bullet>  bullets;
};
