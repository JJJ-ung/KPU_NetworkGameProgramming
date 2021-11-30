#pragma once
class Texture;
class ResourceMgr
{
	DECLARE_SINGLETON(ResourceMgr)

private:
	ResourceMgr();
	~ResourceMgr();

public:
	HRESULT Ready_ResourceMgr(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE pSprite);

public:
	HRESULT Add_TexturesFromFile(const TCHAR* pTexSetTag, const TCHAR* pPath);
	HRESULT Add_Texture(const TCHAR* pTexSetTag, const TCHAR* pState, const TCHAR* pPath, UINT iCnt);
	Texture* Find_Texture(const TCHAR* pTexSetTag, const TCHAR* pState);

public:
	HRESULT Add_WeaponData(const TCHAR* pPath);
	WEAPON Find_WeaponData(int iType);

private:
	LPD3DXSPRITE m_pSprite;
	LPDIRECT3DDEVICE9 m_pDevice;

private:
	typedef 	map< wstring, Texture*> MAPTEX;
	typedef 	map<wstring, MAPTEX> TEXSETS;
	typedef map<char, WEAPON> WEAPONDATA;

	map<wstring, MAPTEX> m_mapTexSets;
	map<int, WEAPON> m_mapWeaponData;

private:
	void Free();
};
