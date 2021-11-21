#pragma once
#include "GameObject.h"

class Texture;
class Animation;
class Player : public GameObject
{
public:
	explicit Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Player();

public:
	void Set_CustomInfo(CUSTOMIZE tCustom) { m_tCustomInfo = tCustom; }

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	HRESULT Ready_AnimationInfo();

private:
	Animation* m_pCurrAnimation = nullptr;
	CUSTOMIZE m_tCustomInfo{};

private:
	typedef map<wstring, Animation*> MAPANI;
	map<wstring, Animation*> m_mapAnimations;

public:
	static Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

