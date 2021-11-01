#include "framework.h"
#include "Shader.h"

Shader::Shader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: Component(pGraphic_Device)
{
}

Shader::~Shader()
{
	Free();
}

HRESULT Shader::Set_Value(D3DXHANDLE hHandle, void* pData, UINT iSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetValue(hHandle, pData, iSize);

	return NOERROR;
}

HRESULT Shader::Ready_Component(const TCHAR* pShaderFilesPath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, pShaderFilesPath, nullptr, nullptr, 0, nullptr, &m_pEffect, &m_pErrBuffer)))
		return E_FAIL;

	return NOERROR;
}

Shader* Shader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pShaderFilesPath)
{
	Shader* pInstance = new Shader(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component(pShaderFilesPath)))
	{
		MSG_BOX("Failed To Create Shader Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Shader::Free()
{
	SafeDelete(m_pErrBuffer);
	SafeDelete(m_pEffect);

	Component::Free();
}
