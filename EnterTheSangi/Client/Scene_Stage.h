#pragma once
#include "Scene.h"

class Scene_Stage : public Scene
{
public:
	explicit Scene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Stage();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

public:
	virtual HRESULT Setup_Recv(char type, void* pRecv);

public:
	static Scene_Stage* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
