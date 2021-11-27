#include "framework.h"
#include "ShaderMgr.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(ShaderMgr)

ShaderMgr::ShaderMgr()
{
}

ShaderMgr::~ShaderMgr()
{
	Free();
}

HRESULT ShaderMgr::Add_Shader(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pTag, const TCHAR* pShaderFilePath)
{
	if (Find_Shader(pTag))
		return E_FAIL;

	Shader* pShader = Shader::Create(pGraphic_Device, pShaderFilePath);
	if (!pShader)
		return E_FAIL;

	pair<const TCHAR*, Shader*> p(pTag, pShader);
	m_mapShader.insert(p);

	return NOERROR;
}

Shader* ShaderMgr::Get_ShaderReference(const TCHAR* pTag)
{
	return Find_Shader(pTag);
}

Shader* ShaderMgr::Find_Shader(const TCHAR* pTag)
{
	auto iter = m_mapShader.find(pTag);
	if (iter == m_mapShader.end())
		return nullptr;
	return iter->second;
}

void ShaderMgr::Free()
{
	for_each(m_mapShader.begin(), m_mapShader.end(), [](pair<const TCHAR*, Shader*> p) { SafeDelete(p.second); });
}
