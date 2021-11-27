#include "framework.h"
#include "PlayerState.h"
#include "Player.h"
#include "InputMgr.h"

static InputMgr* g_pInputMgr = InputMgr::GetInstance();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(Player_Idle)

void Player_Idle::Enter(Player* owner)
{
	State<Player>::Enter(owner);
	m_pOwner->Get_Speed() = 0.f;
	cout << "Idle" << endl;
}

INT Player_Idle::Update(float time_delta)
{
	if(FAILED(m_pOwner->Change_Animation(m_pOwner->Direction_Tag(L"Idle"))))
		return -1;

	m_pOwner->Update_Animation(time_delta);

	if(g_pInputMgr->KeyPressing(KEY_W))
	{
		m_pOwner->Get_Dir().y = -1.f;
		m_pOwner->Get_StateMachine()->Change_State(Player_Run::Instance());
	}
	if (g_pInputMgr->KeyPressing(KEY_S))
	{
		m_pOwner->Get_Dir().y = 1.f;
		m_pOwner->Get_StateMachine()->Change_State(Player_Run::Instance());
	}
	if (g_pInputMgr->KeyPressing(KEY_D))
	{
		m_pOwner->Get_Dir().x = 1.f;
		m_pOwner->Get_StateMachine()->Change_State(Player_Run::Instance());
	}
	if (g_pInputMgr->KeyPressing(KEY_A))
	{
		m_pOwner->Get_Dir().x = -1.f;
		m_pOwner->Get_StateMachine()->Change_State(Player_Run::Instance());
	}

	return State<Player>::Update(time_delta);
}

void Player_Idle::Exit()
{
	State<Player>::Exit();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(Player_Run)

void Player_Run::Enter(Player* owner)
{
	State<Player>::Enter(owner);
	m_pOwner->Get_Speed() = 200.f;
	cout << "Run" << endl;
}

INT Player_Run::Update(float time_delta)
{
	m_pOwner->Change_Animation(m_pOwner->Direction_Tag(L"Run"));

	m_pOwner->Update_Animation(time_delta);

	if (g_pInputMgr->KeyPressing(KEY_W))
		m_pOwner->Get_Dir().y = -1.f;
	if (g_pInputMgr->KeyPressing(KEY_S))
		m_pOwner->Get_Dir().y = 1.f;
	if (g_pInputMgr->KeyPressing(KEY_D))
		m_pOwner->Get_Dir().x = 1.f;
	if (g_pInputMgr->KeyPressing(KEY_A))
		m_pOwner->Get_Dir().x = -1.f;

	if (g_pInputMgr->KeyUp(KEY_W))
		m_pOwner->Get_Dir().y = 0.f;
	if (g_pInputMgr->KeyUp(KEY_S))
		m_pOwner->Get_Dir().y = 0.f;
	if (g_pInputMgr->KeyUp(KEY_D))
		m_pOwner->Get_Dir().x = 0.f;
	if (g_pInputMgr->KeyUp(KEY_A))
		m_pOwner->Get_Dir().x = 0.f;

	if(g_pInputMgr->KeyDown(KEY_RBUTTON))
		m_pOwner->Get_StateMachine()->Change_State(Player_Dodge::Instance());

	if (!g_pInputMgr->KeyPressing(KEY_W) && !g_pInputMgr->KeyPressing(KEY_S)&&
		!g_pInputMgr->KeyPressing(KEY_D)&& !g_pInputMgr->KeyPressing(KEY_A))
		m_pOwner->Get_StateMachine()->Change_State(Player_Idle::Instance());

	return State<Player>::Update(time_delta);
}

void Player_Run::Exit()
{
	State<Player>::Exit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE(Player_Dodge)

void Player_Dodge::Enter(Player* owner)
{
	State<Player>::Enter(owner);
	m_pOwner->Get_Speed() = 400.f;
	D3DXVECTOR3 vDir = m_pOwner->Get_Dir();
	cout << vDir.x << "/" << vDir.y << endl;

	if(vDir.y < 0.f)
	{
		if(vDir.x < 0.f) m_pOwner->Change_Animation(L"Dodge_Back");
		else m_pOwner->Change_Animation(L"Dodge_BackSide");
	}
	else
	{
		if (vDir.x < 0.f) m_pOwner->Change_Animation(L"Dodge_Front");
		else m_pOwner->Change_Animation(L"Dodge_Side");
	}

	cout << "Dodge" << endl;
}

INT Player_Dodge::Update(float time_delta)
{
	if(m_pOwner->Update_Animation(time_delta))
		m_pOwner->Get_StateMachine()->Change_State(Player_Run::Instance());

	return State<Player>::Update(time_delta);
}

void Player_Dodge::Exit()
{
	m_pOwner->Get_Dir() = D3DXVECTOR3(0.f, 0.f, 0.f);
	State<Player>::Exit();
}



