#pragma once
#include "Scene.h"
class NetworkMgr;
class PostCard;
class Scene_Customize : public Scene
{
public:
	explicit Scene_Customize(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Customize();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

private:
	PostCard* m_pPostCard[3] = {};
	NetworkMgr* m_pNetworkMgr = nullptr;

public:
	static Scene_Customize* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
