#pragma once
#include "Scene.h"

class PostCard;
class Scene_Customize : public Scene
{
public:
	explicit Scene_Customize(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Customize();

public:
	virtual HRESULT Ready_Scene(sc_packet_login_ok tLogin);
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

public:
	HRESULT Update_PlayerColor(sc_packet_change_color* tRecv);
	HRESULT Add_OtherPlayer(sc_packet_login_other_client* tRecv);
	HRESULT Update_PlayerReady(sc_packet_ready* tRecv);
	HRESULT Update_PlayerInfo(sc_packet_game_state* tRecv);

public:
	virtual HRESULT Setup_Recv(char type, void* pRecv);

private:
	PostCard* m_pPostCard[3] = {};

	bool m_bReady = false;
	bool m_bSceneChange = false;

public:
	static Scene_Customize* Create(LPDIRECT3DDEVICE9 pGraphic_Device, sc_packet_login_ok tLogin);
	virtual void Free();
};
