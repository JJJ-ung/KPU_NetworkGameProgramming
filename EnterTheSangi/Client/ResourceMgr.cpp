#include "framework.h"
#include "ResourceMgr.h"
#include "Texture.h"

IMPLEMENT_SINGLETON(ResourceMgr)

ResourceMgr::ResourceMgr()
{
}

ResourceMgr::~ResourceMgr()
{
	Free();
}

HRESULT ResourceMgr::Ready_ResourceMgr(LPDIRECT3DDEVICE9 pDevice)
{
	if (!pDevice) return E_FAIL;
	m_pDevice = pDevice;
	return NOERROR;
}

HRESULT ResourceMgr::Add_TexturesFromFile(OBJECT::TYPE eObjType, const TCHAR* pPath)
{
	wifstream fin;

	fin.open(pPath);
	if (fin.fail()) return E_FAIL;

	UINT cnt;
	wstring tag, path;
	while(true)
	{
		fin >> tag >> path >> cnt;

		if (fin.eof()) break;

		if (FAILED(Add_Texture(eObjType, tag.c_str(), path.c_str(), cnt)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT ResourceMgr::Add_Texture(OBJECT::TYPE eObjType, const TCHAR* pState, const TCHAR* pPath, UINT iCnt)
{
	if (Find_Texture(eObjType, pState)) return E_FAIL;

	Texture* pTex = Texture::Create(m_pDevice, pPath, iCnt);
	if (!pTex)
		return E_FAIL;
	m_mapTextures[eObjType].insert(MAPTEX::value_type(pState, pTex));

	return NOERROR;
}

Texture* ResourceMgr::Find_Texture(OBJECT::TYPE eObjType, const TCHAR* pState)
{
	auto iter = m_mapTextures[eObjType].find(pState);
	if (iter == m_mapTextures[eObjType].end()) return nullptr;
	return iter->second;
}


void ResourceMgr::Free()
{

}
