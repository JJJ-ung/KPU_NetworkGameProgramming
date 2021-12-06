#include "framework.h"
#include "GameObject.h"

GameObject::GameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pDevice(pGraphic_Device)
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
	if (m_bDelete)
		return OBJDEAD;

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

INT GameObject::Update_Networking()
{
	return 0;
}

INT GameObject::Recv_Networking(char c, void* p)
{
	return 0;
}

D3DXMATRIX GameObject::Get_Transform(_D3DTRANSFORMSTATETYPE eType)
{
	D3DXMATRIX mat;
	m_pDevice->GetTransform(eType, &mat);
	return mat;
}

void GameObject::Free()
{
}
