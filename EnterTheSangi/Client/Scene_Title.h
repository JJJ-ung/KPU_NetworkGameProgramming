#pragma once
#include "Scene.h"

class AnimatedSprite;
class Scene_Title : public Scene
{
public:
	explicit Scene_Title(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Title();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

private:
	AnimatedSprite* m_pAnimation = nullptr;

public:
	static Scene_Title* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
