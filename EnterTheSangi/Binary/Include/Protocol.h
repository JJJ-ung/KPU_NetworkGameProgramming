#pragma once
#include <d3dx9.h>
#include "Struct.h"
#include "Enum.h"

using namespace std;

constexpr const char* SERVER_IP = "220.94.221.20";
const int SERVER_PORT = 5000;
const int MAX_CLIENTS = 3;
const int MAX_BULLETS = 30;
const int MAX_ITEMS = 10;
//constexpr unsigned int MAX_BUF_SIZE = 256;
#define  MAX_NAME_SIZE 16
#define	 BUF_SIZE 512



// client to server packet
const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_CHANGE_COLOR = 2;
const char CS_PACKET_READY = 3;
const char CS_PACKET_GAME_STATE = 4;

//server to client packet
const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_LOGIN_OTHER_CLIENT = 2;
const char SC_PACKET_REMOVE_OBJECT = 3;
const char SC_PACKET_CHANGE_COLOR = 4;
const char SC_PACKET_READY = 5;
const char SC_PACKET_GAME_STATE = 6;

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

struct cs_packet_game_state  //�� �����Ӹ��� ���� ��ü ���� ����
{
	unsigned char   size;
	char            type;
	STATE::TYPE     state;
	D3DXVECTOR2     position;
	D3DXVECTOR2     look;
	bool            is_shooting; // �������� ������� �ƴϴ� �÷��̾ ������̸� �ش� �Ѿ� ������Ű�� ���� (��� ���� 2������ �̻� ���� �ʿ�)
};



//--------------------------------------------------
//         server to client packet 
//--------------------------------------------------
struct sc_packet_login_ok
{
	unsigned char  size;
	char           type;
	char           id;          
	D3DXVECTOR3    body_color;   //Ŀ���͸���¡ �ʱⰪ
	D3DXVECTOR3    cloth_color;  //Ŀ���͸���¡ �ʱⰪ
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

struct sc_packet_ready    //�غ� ����, ���� �� �� �ش� ��Ŷ ���
{
	unsigned char  size;
	char           type;
	char           id;
	bool           is_ready;
};

struct sc_packet_game_start // 3���� ��� �غ� ���� ������ ����
{
	unsigned char   size;
	char            type;
};

struct sc_packet_game_state  //�� �����Ӹ��� ���� ��ü ���� ����
{
	unsigned char   size;
	char            type;

	player_info     players[MAX_CLIENTS];

	char            bullet_num;
	bullet_info     bullets[MAX_BULLETS];

	char            item_num;
	item_info       items[MAX_ITEMS];
};
