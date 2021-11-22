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
	virtual HRESULT Ready_GameObject(UINT iPlayerID, bool bLocalPlayer);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	NetworkMgr* m_pNetworkMgr = nullptr;

private:
	D3DXVECTOR3 m_vPosition = {};
	D3DXVECTOR3 m_vCenter = {};
	D3DXMATRIX m_matView = {};

private:
	UINT m_iPlayerID = 0;
	float m_fSpeed = 100.f;
	float m_fAccel = 2000.f;
	bool m_bAnimation = true;

private:
	ColorButton* m_pColorButton = nullptr;
	CustomPlayer* m_pCustomPlayer = nullptr;

public:
	static PostCard* Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iPlayerID, bool bLocalPlayer);
	virtual void Free();
};

