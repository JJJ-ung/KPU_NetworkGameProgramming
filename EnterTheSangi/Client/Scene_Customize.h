#pragma once
#include "Scene.h"

class PostCard;
class NetworkMgr;
class Scene_Customize : public Scene
{
public:
	explicit Scene_Customize(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Customize();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

public:
	HRESULT Update_PlayerColor(sc_packet_change_color tRecv);

public:
	CRITICAL_SECTION* Get_Crt(void) { return &m_Crt; }
	BOOL Get_Finish(void) const { return m_bFinish; }
	static unsigned int CALLBACK Thread_Recv(void* pArg);

private:
	PostCard* m_pPostCard[3] = {};
	NetworkMgr* m_pNetworkMgr = nullptr;

private:
	HANDLE								m_hThread;
	CRITICAL_SECTION			m_Crt;
	BOOL									m_bFinish = false;

public:
	static Scene_Customize* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
