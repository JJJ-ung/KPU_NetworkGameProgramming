#pragma once
#include "Component.h"
#include "Renderer.h"
#include "InputMgr.h"
#include "GameMgr.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"

class Buffer;
class Texture;
class GameObject
{
public:
	explicit GameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~GameObject();

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

protected:
	D3DXMATRIX Get_Transform(_D3DTRANSFORMSTATETYPE eType);

protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	Renderer*							m_pRenderer = nullptr;
	Shader*								m_pShader = nullptr;
	Texture*								m_pTexture = nullptr;
	InputMgr*							m_pInputMgr = nullptr;
	GameMgr*							m_pGameMgr = nullptr;
	ResourceMgr*					m_pResourceMgr = nullptr;

protected:
	unordered_map<const TCHAR*, Component*>			m_Components;
	typedef unordered_map<const TCHAR*, Component*>	COMPONENTS;

protected:
	HRESULT Add_Component(const TCHAR* pComponentTag, Component* pCom);
	Component* Find_Component(const TCHAR* pComponentTag);

public:
	virtual void Free();
};
