#pragma once
#include "Scene.h"

class Loading;
class AnimatedSprite;
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
	float m_fTime = 0.f;
	Loading* m_pLoading = nullptr;
	AnimatedSprite* m_pAnimation = nullptr;

public:
	static Scene_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

