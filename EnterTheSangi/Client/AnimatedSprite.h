#pragma once
#include "GameObject.h"

class Animation;
class AnimatedSprite : public GameObject
{
public:
	explicit AnimatedSprite(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~AnimatedSprite();

public:
	float& Get_Alpha() { return m_fAlpha; }
	D3DXVECTOR3& Get_Scale() { return m_vScale; }
	D3DXVECTOR3& Get_Position() { return m_vPos; }

public:
	virtual HRESULT Ready_GameObject(const TCHAR* pObjTag, const TCHAR* pStateTag, float fSpeed);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	float m_fAlpha = 0.f;
	Animation* m_pAnimation = nullptr;
	D3DXVECTOR3 m_vScale{1.f, 1.f, 1.f};
	D3DXVECTOR3 m_vPos{ 0.f, 0.f, 0.f };

public:
	static AnimatedSprite* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pObjTag, const TCHAR* pStateTag, float fSpeed);
	virtual void Free();
};

