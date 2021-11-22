#pragma once
#include "GameObject.h"

class Font;
class Animation;
class Player : public GameObject
{
public:
	explicit Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Player();

public:
	void Set_CustomInfo(CUSTOMIZE tCustom) { m_tCustomInfo = tCustom; }

public:
	virtual HRESULT Ready_GameObject(string strName);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

protected:
	HRESULT Ready_AnimationInfo();

protected:
	string m_strName = "";
	D3DXVECTOR3 m_vPosition{0.f, 0.f, 0.f};

protected:
	Font* m_pNameTag = nullptr;
	Animation* m_pCurrAnimation = nullptr;
	CUSTOMIZE m_tCustomInfo{};

protected:
	typedef map<wstring, Animation*> MAPANI;
	map<wstring, Animation*> m_mapAnimations;

public:
	static Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device, string strName);
	virtual void Free();
};

