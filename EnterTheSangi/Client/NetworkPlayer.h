//#pragma once
//#include "Player.h"
//
//class NetworkPlayer : public Player
//{
//public:
//	explicit NetworkPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
//	virtual ~NetworkPlayer();
//
//public:
//	virtual HRESULT Ready_GameObject(UINT iPlayerIdx, const TCHAR* pName);
//	virtual INT Update_GameObject(float TimeDelta);
//	virtual INT LateUpdate_GameObject(float TimeDelta);
//	virtual HRESULT Render_GameObject();
//
//private:
//	UINT m_iPlayerIdx = -1;
//
//public:
//	static NetworkPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iPlayerIdx, const TCHAR* pName);
//	virtual void Free();
//};
//
