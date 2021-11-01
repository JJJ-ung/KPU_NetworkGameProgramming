#pragma once
#include "Component.h"

class Shader : public Component
{
public:
	explicit Shader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Shader();

public:
	LPD3DXEFFECT	Get_EffectHandle() { return m_pEffect; }
	HRESULT Set_Value(D3DXHANDLE hHandle, void* pData, UINT iSize);

public:
	virtual HRESULT Ready_Component(const TCHAR* pShaderFilesPath);

private:
	LPD3DXEFFECT	m_pEffect = nullptr;
	LPD3DXBUFFER	m_pErrBuffer = nullptr;

public:
	static Shader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pShaderFilesPath);
	virtual void Free();
};

