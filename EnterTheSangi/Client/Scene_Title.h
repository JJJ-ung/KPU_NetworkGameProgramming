#pragma once
#include "Scene.h"

class Font;
class AnimatedSprite;
class NetworkMgr;
class Scene_Title : public Scene
{
public:
	explicit Scene_Title(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Title();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

public:
	CRITICAL_SECTION* Get_Crt(void) { return &m_Crt; }
	BOOL Get_Finish(void) const { return m_bFinish; }
	static unsigned int CALLBACK Thread_Recv(void* pArg);

private:
	string m_strName = "";
	Font* m_pName = nullptr;
	AnimatedSprite* m_pAnimation = nullptr;
	NetworkMgr* m_pNetworkMgr = nullptr;

private:
	HANDLE								m_hThread;
	CRITICAL_SECTION			m_Crt;
	BOOL									m_bFinish = false;

public:
	static Scene_Title* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
