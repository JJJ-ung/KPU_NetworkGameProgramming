#pragma once
#include "Renderer.h"
#include "InputMgr.h"
#include "GameMgr.h"
#include "NetworkMgr.h"
#include "ResourceMgr.h"

class Buffer;
class Shader;
class Texture;
class GameObject
{
public:
	explicit GameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~GameObject();

public:
	D3DXVECTOR3& Get_Position() { return m_vPosition; }
	virtual float Get_Y() { return m_vPosition.y; }

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual INT Update_Networking();
	virtual INT Recv_Networking(char c, void* p);

protected:
	D3DXMATRIX Get_Transform(_D3DTRANSFORMSTATETYPE eType);

protected:
	D3DXVECTOR3 m_vPosition{ 0.f, 0.f, 0.f };

protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	Renderer*							m_pRenderer = nullptr;
	Shader*								m_pShader = nullptr;
	Texture*								m_pTexture = nullptr;
	InputMgr*							m_pInputMgr = nullptr;
	GameMgr*							m_pGameMgr = nullptr;
	NetworkMgr*					m_pNetworkMgr = nullptr;
	ResourceMgr*					m_pResourceMgr = nullptr;

public:
	virtual void Free();
};
