#pragma once
#include "Component.h"

class ResourceMgr;
class Animation : public Component
{
public:
	explicit Animation(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Animation();

public:
	virtual HRESULT Ready_Component();

private:
	ResourceMgr* m_pResourceMgr = nullptr;

public:
	static Animation* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

