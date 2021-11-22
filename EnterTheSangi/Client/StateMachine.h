//#pragma once
//#include "Component.h"
//
//template<class T>
//class State
//{
//public:
//	State(T*);
//	~State();
//public:
//	virtual void Enter() PURE;
//	virtual void Update(float TimeDelta) PURE;
//	virtual void Exit() PURE;
//private:
//	T* pOwner;
//};
//
//template<class T>
//class StateMachine : public Component
//{
//public:
//	explicit StateMachine(LPDIRECT3DDEVICE9 pGraphic_Device);
//	virtual ~StateMachine();
//
//public:
//	virtual HRESULT Ready_Component(T* pOwner);
//	virtual INT Update_Component(float TimeDelta);
//
//public:
//	HRESULT Change_State(State<T>* pState);
//
//private:
//	T* m_pOwner;
//	State<T>* m_pCurrentState;
//
//private:
//	virtual void Free();
//};
//
