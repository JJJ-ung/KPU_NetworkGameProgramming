#pragma once
#include "Scene.h"

class Loading;
class Scene_Loading : public Scene
{
public:
	explicit Scene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Loading();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

private:
	Loading* m_pLoading = nullptr;

public:
	static Scene_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

