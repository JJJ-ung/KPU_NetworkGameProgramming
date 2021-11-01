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

protected:
	virtual void Free();
};

