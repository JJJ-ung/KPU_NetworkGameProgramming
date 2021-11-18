#pragma once
class Texture;
class Animation;
class ResourceMgr
{
	DECLARE_SINGLETON(ResourceMgr)

private:
	ResourceMgr();
	~ResourceMgr();

public:
	HRESULT Ready_ResourceMgr(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Add_TexturesFromFile(OBJECT::TYPE eObjType, const TCHAR* pPath);
	HRESULT Add_Texture(OBJECT::TYPE eObjType, const TCHAR* pState, const TCHAR* pPath, UINT iCnt);

public:
	Texture* Find_Texture(OBJECT::TYPE eObjType, const TCHAR* pState);

private:
	void Free();

private:
	LPDIRECT3DDEVICE9 m_pDevice;
	map< wstring, Texture*> m_mapTextures[OBJECT::END];
	typedef 	map< wstring, Texture*> MAPTEX;
};

