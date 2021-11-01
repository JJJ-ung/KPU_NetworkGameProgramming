#pragma once

class Component;
class GameObject
{
public:
	explicit GameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~GameObject();

public:
	virtual HRESULT Ready_GameObject();
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

protected:
	unordered_map<const TCHAR*, Component*>			m_Components;
	typedef unordered_map<const TCHAR*, Component*>	COMPONENTS;

protected:
	HRESULT Add_Component(const TCHAR* pComponentTag, Component* pCom);
	Component* Find_Component(const TCHAR* pComponentTag);

public:
	virtual void Free();
};
