#include "framework.h"
#include "Component.h"

Component::Component(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
}

Component::~Component()
{
}

HRESULT Component::Ready_Component()
{
	return NOERROR;
}

INT Component::Update_Component(float TimeDelta)
{
	return 0;
}

INT Component::LateUpdate_Component(float TimeDelta)
{
	return 0;
}

HRESULT Component::Render_Component()
{
	return NOERROR;
}

void Component::Free()
{
}
