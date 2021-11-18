#pragma once
#include "Component.h"

class Frame;
class ResourceMgr;
class Animation : public Component
{
public:
	explicit Animation(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Animation();

public:
	virtual HRESULT Ready_Component(const TCHAR* pPath, OBJECT::TYPE eType);
	virtual INT Update_Component(float TimeDelta);
	virtual INT LateUpdate_Component(float TimeDelta);

private:
	Frame* m_pCurrFrame = nullptr;
	map<wstring, Frame*> m_mapFrame;
	typedef map<wstring, Frame*> MAPFRAME;

private:
	ResourceMgr* m_pResourceMgr = nullptr;

public:
	static Animation* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pPath, OBJECT::TYPE eType);
	virtual void Free();
};
