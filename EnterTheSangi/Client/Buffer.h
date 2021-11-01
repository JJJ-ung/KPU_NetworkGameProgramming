#pragma once
#include "Component.h"

class Buffer : public Component
{
public:
	explicit Buffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Buffer();

public:
	virtual HRESULT Ready_Component();
	virtual HRESULT Render_Buffer();

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
	ULONG		m_dwVtxSize = 0;
	ULONG 		m_dwVtxCnt = 0;
	ULONG 		m_dwTriCnt = 0;
	ULONG 		m_dwVtxFVF = 0;
	ULONG 		m_dwIdxSize = 0;
	D3DFORMAT	m_IdxFmt;

public:
	static Buffer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();
};

