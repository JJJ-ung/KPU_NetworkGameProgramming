#pragma once
#include "GameObject.h"
#include "PlayerState.h"

class Font;
class Animation;
class Player : public GameObject
{
public:
	explicit Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Player();

public:
	float& Get_Speed() { return m_fSpeed; }
	STATE::TYPE& Get_State() { return m_eState; }
	D3DXVECTOR3& Get_Dir() { return m_vDirection; }
	StateMachine<Player>* Get_StateMachine() { return m_pStateMachine; }

public:
	virtual HRESULT Ready_GameObject(CLIENT t);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual INT Update_Networking();

public:
	bool Update_Animation(float TimeDelta);
	HRESULT Change_Animation(wstring strTag);
	STATE::DIR Compute_Direction();
	wstring Direction_Tag(wstring strTag);

protected:
	HRESULT Ready_AnimationInfo();

protected:
	float m_fSide = 1.f;
	float m_fSpeed = 1.f;
	STATE::TYPE m_eState;
	D3DXVECTOR3 m_vPosition{0.f, 0.f, 0.f};
	D3DXVECTOR3 m_vDirection{ 0.f, 0.f, 0.f };

protected:
	CLIENT m_tClientInfo = {};
	Font* m_pNameTag = nullptr;
	Animation* m_pCurrAnimation = nullptr;
	StateMachine<Player>* m_pStateMachine = nullptr;

protected:
	typedef map<wstring, Animation*> MAPANI;
	map<wstring, Animation*> m_mapAnimations;

public:
	static Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CLIENT t);
	virtual void Free();
};

