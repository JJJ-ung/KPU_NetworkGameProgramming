#pragma once
#include "GameObject.h"

class Player;
class MainMap : public GameObject
{
public:
	explicit MainMap(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~MainMap();

public:
	virtual HRESULT Ready_GameObject(Player* pLocalPlayer);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	Player* m_pPlayer = nullptr;
	D3DXVECTOR3 m_vCenter = { 0.f, 0.f, 0.f };

private:
	vector<RECT> m_vecCollisionRects;

public:
	static MainMap* Create(LPDIRECT3DDEVICE9 pGraphic_Device, Player* pLocalPlayer);
	virtual void Free();
};

