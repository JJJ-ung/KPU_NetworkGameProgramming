#pragma once
#include "GameObject.h"

class Texture;
class StaticSprite : public GameObject
{
public:
	explicit StaticSprite(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~StaticSprite();

public:
	float& Get_Alpha() { return m_fAlpha; }
	D3DXVECTOR3& Get_Scale() { return m_vScale; }
	D3DXVECTOR3& Get_Position() { return m_vPosition; }

public:
	virtual HRESULT Ready_GameObject(const TCHAR* pObjTag, const TCHAR* pStateTag, D3DXVECTOR3 vPos, D3DXVECTOR3 vScale);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	float m_fAlpha = 0.f;
	Texture* m_pTexture = nullptr;
	D3DXVECTOR3 m_vScale{ 1.f, 1.f, 1.f };
	D3DXVECTOR3 m_vCenter{ 0.f, 0.f, 0.f };

public:
	static StaticSprite* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pObjTag, const TCHAR* pStateTag, D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f }, D3DXVECTOR3 vScale = { 1.f, 1.f, 1.f });
	virtual void Free();
};

