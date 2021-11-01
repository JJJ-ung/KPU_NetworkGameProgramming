#pragma once
#include "TargetMgr.h"

class Shader;
class TargetMgr;
class GameObject;
class Renderer
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

public:
	explicit Renderer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Renderer();

public:
	HRESULT Ready_Renderer();
	HRESULT Add_RenderList(RENDERGROUP eGroup, GameObject* pObj);
	HRESULT Render_Objects();

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

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
	Shader* m_pTest = nullptr;
	//CShader* m_pShader_LightAcc = nullptr;
	//CShader* m_pShader_Blend = nullptr;
	//CShader* m_pShader_Blur = nullptr;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

public:
	static Renderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

