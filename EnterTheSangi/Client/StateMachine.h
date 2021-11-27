#pragma once
#include "Component.h"
#include "framework.h"

template<class T>
class State
{
public:
	virtual ~State() = default;
public:
	virtual void Enter(T* pOwner) { m_pOwner = pOwner; }
	virtual INT Update(float TimeDelta) { return 0; }
	virtual void Exit() {};
protected:
	T* m_pOwner;
};

template<class T>
class StateMachine : public Component
{
public:
	explicit StateMachine() :Component(nullptr)	{}
	virtual ~StateMachine()	{ Free();	}

public:
	virtual HRESULT Ready_Component(T* pOwner, State<T>* pStartState)
	{
		if (!pOwner) return E_FAIL;
		if (!pStartState) return E_FAIL;

		m_pOwner = pOwner;
		m_pCurrentState = pStartState;
		m_pCurrentState->Enter(pOwner);

		return NOERROR;
	}

	virtual INT Update_Component(float TimeDelta)
	{
		return m_pCurrentState->Update(TimeDelta);
	}

public:
	HRESULT Change_State(State<T>* pState)
	{
		if (!pState) return E_FAIL;

		m_pCurrentState->Exit();
		m_pCurrentState = pState;
		m_pCurrentState->Enter(m_pOwner);

		return NOERROR;
	}

private:
	T* m_pOwner = nullptr;
	State<T>* m_pCurrentState = nullptr;

public:
	static StateMachine<T>* Create(T* pOwner, State<T>* pStartState)
	{
		StateMachine<T>* pInstance = new StateMachine<T>;
		if (FAILED(pInstance->Ready_Component(pOwner, pStartState)))
			SafeDelete(pInstance);
		return pInstance;
	}
};

