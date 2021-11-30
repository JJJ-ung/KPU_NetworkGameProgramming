#pragma once
#include "GameObject.h"

class Font;
class PostCard;
class Animation;
class CustomPlayer : public GameObject
{
public:
	explicit CustomPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CustomPlayer();

public:
	CUSTOMIZE& Get_CustomInfo() { return m_tCustomInfo; }
	void Set_CustomInfo(CUSTOMIZE tCustom) { m_tCustomInfo = tCustom; }

public:
	virtual HRESULT Ready_GameObject(UINT iPlayerID, string strName, D3DXVECTOR3 vBody, D3DXVECTOR3 vCloth);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	float m_fAlpha = 0.f;
	string m_strName = "";
	D3DXMATRIX m_matView = {};

private:
	Font* m_pNameTag = nullptr;
	Animation* m_pAnimation = nullptr;
	CUSTOMIZE m_tCustomInfo{};

public:
	static CustomPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iPlayerID, string strName, D3DXVECTOR3 vBody, D3DXVECTOR3 vCloth);
	virtual void Free();
};

