#pragma once
class Shader;
class ShaderMgr
{
	DECLARE_SINGLETON(ShaderMgr)

private:
	ShaderMgr();
	~ShaderMgr();

public:
	HRESULT Add_Shader(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pTag, const TCHAR* pShaderFilePath);
	Shader* Get_ShaderReference(const TCHAR* pTag);

private:
	Shader* Find_Shader(const TCHAR* pTag);

private:
	map<const TCHAR*, Shader*> m_mapShader;

public:
	void Free();
};

