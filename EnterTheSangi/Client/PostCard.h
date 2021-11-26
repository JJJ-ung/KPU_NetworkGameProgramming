#pragma once
#include "GameObject.h"

class NetworkMgr;
class ColorButton;
class CustomPlayer;
class PostCard : public GameObject
{
public:
	explicit PostCard(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~PostCard();

public:
	CustomPlayer* Get_Player() { return m_pCustomPlayer; }
	D3DXVECTOR3& Get_Position() { return m_vPosition; }

public:
	virtual HRESULT Ready_GameObject(UINT iPlayerID, bool bLocalPlayer, string strName, D3DXVECTOR3 vBody, D3DXVECTOR3 vCloth);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Setup_Ready(bool bReady);

private:
	bool m_bLocal = false;
	bool m_bReady = false;
	Texture* m_pReadyTexture = nullptr;
	NetworkMgr* m_pNetworkMgr = nullptr;

private:
	D3DXVECTOR3 m_vPosition = {};
	D3DXVECTOR3 m_vCenter = {};
	D3DXMATRIX m_matView = {};
	D3DXVECTOR3 m_vColor[2] = {};

private:
	UINT m_iPlayerID = 0;
	float m_fSpeed = 100.f;
	float m_fAccel = 2000.f;
	bool m_bAnimation = true;
	string m_strName = "";

private:
	ColorButton* m_pColorButton = nullptr;
	CustomPlayer* m_pCustomPlayer = nullptr;

public:
	static PostCard* Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iPlayerID, bool bLocalPlayer, string strName, D3DXVECTOR3 vBody, D3DXVECTOR3 vCloth);
	virtual void Free();
};

