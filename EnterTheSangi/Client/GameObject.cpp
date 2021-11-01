#include "framework.h"
#include "GameObject.h"

GameObject::GameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
}

GameObject::~GameObject()
{
	Free();
}

HRESULT GameObject::Ready_GameObject()
{
	return NOERROR;
}

INT GameObject::Update_GameObject(float TimeDelta)
{
	return 0;
}

INT GameObject::LateUpdate_GameObject(float TimeDelta)
{
	return 0;
}

HRESULT GameObject::Render_GameObject()
{
	return NOERROR;
}

HRESULT GameObject::Add_Component(const TCHAR* pComponentTag, Component* pCom)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	m_Components.insert(COMPONENTS::value_type(pComponentTag, pCom));

	return NOERROR;
}

Component* GameObject::Find_Component(const TCHAR* pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), TagFinder(pComponentTag));
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void GameObject::Free()
{
	for (auto& Pair : m_Components)
		SafeDelete(Pair.second);
	m_Components.clear();
}
