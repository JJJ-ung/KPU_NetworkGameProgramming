#pragma once
class Component
{
public:
	explicit Component(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Component();

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

public:
	virtual HRESULT Ready_Component();
	virtual INT Update_Component(float TimeDelta);
	virtual INT LateUpdate_Component(float TimeDelta);
	virtual HRESULT Render_Component();

protected:
	virtual void Free();
};

