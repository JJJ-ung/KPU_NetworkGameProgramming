#pragma once
#include "Scene.h"

class Font;
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

public:
	virtual HRESULT Setup_Recv(char type, void* pRecv);

private:
	string m_strName = "";
	Font* m_pName = nullptr;
	AnimatedSprite* m_pAnimation = nullptr;
	bool m_bPressedOK = false;

public:
	static Scene_Title* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
