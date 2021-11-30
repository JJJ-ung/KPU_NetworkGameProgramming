#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	explicit Bullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Bullet();

public:
	virtual HRESULT Ready_GameObject(int iType, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fSide, float fSize, float fAngle, float fDuration, float fSpeed);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	int m_iType = -1;
	float m_fTime = 0.f;
	float m_fSpeed = 1.f;
	float m_fDuration = 0.f;
	float m_fSide = 1.f;
	float m_fSize = 0.f;
	float m_fAngle = 0.f;
	D3DXVECTOR3 m_vDir{ 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vCenter{ 0.f, 0.f, 0.f };

public:
	static Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device, int iType, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fSide, float fSize, float fAngle, float fDuration, float fSpeed);
	virtual void Free();
};

