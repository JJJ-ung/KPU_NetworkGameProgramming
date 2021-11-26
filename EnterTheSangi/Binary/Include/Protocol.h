#pragma once
#include <d3dx9.h>
#include <vector>

using namespace std;

constexpr const char* SERVER_IP = "220.94.221.20";
//constexpr const char* SERVER_IP = "121.139.87.66";
//constexpr const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 5000;
const int MAX_CLIENTS = 3;
//constexpr unsigned int MAX_BUF_SIZE = 256;
#define  MAX_NAME_SIZE 16
#define	 BUF_SIZE 256


// client to server packet
const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_CHANGE_COLOR = 2;
const char CS_PACKET_READY = 3;
const char CS_PACKET_TYPE_END = 4;

//server to client packet
const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_LOGIN_OTHER_CLIENT = 2;
const char SC_PACKET_REMOVE_OBJECT = 3;
const char SC_PACKET_CHANGE_COLOR = 4;
const char SC_PACKET_READY = 5;
const char SC_PACKET_ALL_READY = 6;
const char SC_PACKET_TYPE_END = 7;


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

struct sc_packet_all_ready // ��� �÷��̾ �غ� �Ϸ����� �˸�
{
	unsigned char	size;
	char			type;
	D3DXVECTOR2		position;	// �ʿ��� �ʱ� ��ġ�� ����? ���ִ°� ������?
};

struct sc_packet_game_state  //�� �����Ӹ��� ���� ��ü ���� ����
{
	unsigned char   size;
	char            type;

	//Player info
	//GameStatePlayer player[3];
	//vector<Item>    items;
	//vector<Bullet>  bullets;
};
