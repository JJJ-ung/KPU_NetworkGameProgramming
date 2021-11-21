#pragma once
#include "Scene.h"

class Scene_Test : public Scene
{
public:
	explicit Scene_Test(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Test();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

public:
	static Scene_Test* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
