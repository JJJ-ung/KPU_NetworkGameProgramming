#pragma once
#include "NetworkPlayer.h"
#include "Scene.h"

class Scene_Ending : public Scene
{
public:
	explicit Scene_Ending(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Ending();

public:
	virtual HRESULT Ready_Scene(int iWin);
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

public:
	static Scene_Ending* Create(LPDIRECT3DDEVICE9 pGraphic_Device, int iWin);
	virtual void Free();
};

