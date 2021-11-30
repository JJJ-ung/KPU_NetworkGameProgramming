#pragma once
#include "GameObject.h"

typedef struct tagButtonInfo
{
	D3DXVECTOR3 pos;
	D3DXVECTOR4 color;
	RECT collrect;
}COLORINFO;

class NetworkMgr;
class CustomPlayer;
class ColorButton : public GameObject
{
public:
	explicit ColorButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~ColorButton();

public:
	bool& Get_Active() { return m_bActive; }

public:
	virtual HRESULT Ready_GameObject(UINT iPlayerID, bool bActive);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Update_Collision(CustomPlayer* pPlayer);

private:
	NetworkMgr* m_pNetworkMgr = nullptr;

private:
	bool m_bActive = true;
	UINT m_iPlayerID = 0;
	D3DXMATRIX m_matView = {};
	D3DXVECTOR3 m_vCenter = {};

private:
	COLORINFO m_vBodyColors[3] = {};
	COLORINFO m_vClothColors[3] = {};

public:
	static ColorButton* Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iPlayerID, bool bActive);
	virtual void Free();
};

