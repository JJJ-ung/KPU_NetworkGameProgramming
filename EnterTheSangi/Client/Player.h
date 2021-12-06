#pragma once
#include "GameObject.h"
#include "PlayerState.h"

class Font;
class Health;
class Weapon;
class Animation;
class Player : public GameObject
{
public:
	explicit Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Player();

public:
	int& Get_Health() { return m_iHealth; }
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
	INT Update_Networking();
	INT Recv_Networking(char c, void* p);
	INT Update_Movement(float TimeDelta);

public:
	bool Update_Animation(float TimeDelta);
	HRESULT Change_Animation(wstring strTag);
	STATE::DIR Compute_Direction();
	wstring Direction_Tag(wstring strTag);

public:
	virtual HRESULT Change_Weapon(sc_packet_change_weapon t);

protected:
	HRESULT Ready_AnimationInfo();

protected:
	HRESULT Ready_Collision();
	INT Update_Collision(float TimeDelta);
	HRESULT Render_Rect(RECT t, D3DXMATRIX view, D3DXMATRIX proj);

protected:
	int m_iHealth = 10;

protected:
	float m_fSide = 1.f;
	float m_fSpeed = 1.f;
	STATE::TYPE m_eState;
	D3DXVECTOR3 m_vDirection{ 0.f, 0.f, 0.f };
	D3DXMATRIX m_matWorld;

protected:
	CLIENT m_tClientInfo = {};
	Font* m_pNameTag = nullptr;
	Weapon* m_pWeapon = nullptr;
	Animation* m_pCurrAnimation = nullptr;
	StateMachine<Player>* m_pStateMachine = nullptr;

protected:
	Health* m_pHealth = nullptr;
	bool m_bDead = false;

protected:
	RECT m_tCollRect = {};
	vector<RECT> m_vecColl;
	bool m_bRenderCollision = false;

protected:
	typedef map<wstring, Animation*> MAPANI;
	map<wstring, Animation*> m_mapAnimations;

	ID3DXLine* m_pLine;

public:
	static Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CLIENT t);
	virtual void Free();
};

