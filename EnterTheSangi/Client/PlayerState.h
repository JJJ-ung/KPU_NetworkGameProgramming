#pragma once
#include "StateMachine.h"

class Player;

class Player_Idle : public State<Player>
{
public:
	static Player_Idle* Instance();
	virtual void Enter(Player* pOwner);
	virtual INT Update(float TimeDelta);
	virtual void Exit();
};

class Player_Run : public State<Player>
{
public:
	static Player_Run* Instance();
	virtual void Enter(Player* pOwner);
	virtual INT Update(float TimeDelta);
	virtual void Exit();
};

class Player_Dodge : public State<Player>
{
public:
	static Player_Dodge* Instance();
	virtual void Enter(Player* pOwner);
	virtual INT Update(float TimeDelta);
	virtual void Exit();
};
