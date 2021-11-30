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

HRESULT ResourceMgr::Ready_ResourceMgr(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE pSprite)
{
	if (!pDevice) return E_FAIL;
	m_pDevice = pDevice;

	if (!pSprite) return E_FAIL;
	m_pSprite = pSprite;

	return NOERROR;
}

HRESULT ResourceMgr::Add_TexturesFromFile(const TCHAR* pTexSetTag, const TCHAR* pPath)
{
	wifstream fin;

	fin.open(pPath);
	if (fin.fail())
		return E_FAIL;

	UINT cnt;
	wstring tag, path;
	while (true)
	{
		fin >> tag >> path >> cnt;

		if (fin.eof()) break;

		if (FAILED(Add_Texture(pTexSetTag, tag.c_str(), path.c_str(), cnt)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT ResourceMgr::Add_Texture(const TCHAR* pTexSetTag, const TCHAR* pState, const TCHAR* pPath, UINT iCnt)
{
	auto iter_texset = m_mapTexSets.find(pTexSetTag);
	if (iter_texset == m_mapTexSets.end())
	{
		MAPTEX mapTmp;
		m_mapTexSets.insert(TEXSETS::value_type(pTexSetTag, mapTmp));
	}

	Texture* pTex = Texture::Create(m_pDevice, m_pSprite, pPath, iCnt);
	if (!pTex) return E_FAIL;

	m_mapTexSets[pTexSetTag].insert(MAPTEX::value_type(pState, pTex));

	return NOERROR;
}

Texture* ResourceMgr::Find_Texture(const TCHAR* pTexSetTag, const TCHAR* pState)
{
	auto iter = m_mapTexSets.find(pTexSetTag);
	if (iter == m_mapTexSets.end())
		return nullptr;

	auto iter2 = iter->second.find(pState);
	if (iter2 == iter->second.end())
		return nullptr;

	return iter2->second;
}

HRESULT ResourceMgr::Add_WeaponData(const TCHAR* pPath)
{
	wifstream fin;

	fin.open(pPath);
	if (fin.fail())
		return E_FAIL;

	while (true)
	{
		WEAPON t;
		fin >> t.type >> t.damage >> t.shotspeed >> t.bulletspeed >> t.duration >> t.bulletoffset.x >> t.bulletoffset.y >> t.size;
		if (fin.eof()) break;
		m_mapWeaponData.insert(WEAPONDATA::value_type(t.type, t));
	}

	return NOERROR;
}

WEAPON ResourceMgr::Find_WeaponData(int iType)
{
	auto iter = m_mapWeaponData.find(iType);
	if (iter == m_mapWeaponData.end())
		return WEAPON();
	return iter->second;
}

void ResourceMgr::Free()
{
	for(auto iter_texset : m_mapTexSets)
	{
		for(auto iter_tex : iter_texset.second)
			SafeDelete(iter_tex.second);
		iter_texset.second.clear();
	}
	m_mapTexSets.clear();
}
