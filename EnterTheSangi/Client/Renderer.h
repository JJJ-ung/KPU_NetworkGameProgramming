#pragma once
#include "TargetMgr.h"

class Shader;
class TargetMgr;
class GameObject;
class Renderer
{
	DECLARE_SINGLETON(Renderer)
	
private:
	Renderer();
	~Renderer();

public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

public:
	HRESULT Ready_Renderer(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Add_RenderList(RENDERGROUP eGroup, GameObject* pObj);
	HRESULT Render_Objects();

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

private:
	HRESULT Render_Blend();

private:
	HRESULT Setup_Buffer(D3DVIEWPORT9		ViewPort);
	HRESULT Setup_Target(D3DVIEWPORT9		ViewPort);

	//ForTest ************************************************************************************************
private:
	HRESULT Render_Test();
	//********************************************************************************************************

private:
	LPDIRECT3DDEVICE9 m_pDevice;

private:
	list<GameObject*>					m_RenderList[RENDER_END];
	typedef list<GameObject*>	RENDERLIST;

private:
	TargetMgr* m_pTarget_Manager = nullptr;
	Shader* m_pShader_Blend = nullptr;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

public:
	static Renderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
