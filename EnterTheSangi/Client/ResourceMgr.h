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
	HRESULT Add_TexturesFromFile(const TCHAR* pTexSetTag, const TCHAR* pPath);
	HRESULT Add_Texture(const TCHAR* pTexSetTag, const TCHAR* pState, const TCHAR* pPath, UINT iCnt);

public:
	Texture* Find_Texture(const TCHAR* pTexSetTag, const TCHAR* pState);

private:
	LPD3DXSPRITE m_pSprite;
	LPDIRECT3DDEVICE9 m_pDevice;

private:
	typedef 	map< wstring, Texture*> MAPTEX;
	typedef 	map<wstring, MAPTEX> TEXSETS;

	map<wstring, MAPTEX> m_mapTexSets;

private:
	void Free();
};
