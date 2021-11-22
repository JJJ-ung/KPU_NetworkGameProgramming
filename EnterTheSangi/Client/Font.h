#pragma once
#include "Component.h"

class Texture;
class Font : public Component
{
public:
	explicit Font(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Font();

public:
	wstring& Get_Line() { return m_strLine; }

public:
	virtual HRESULT Ready_Component(wstring strLine);
	virtual HRESULT Render_Component();

private:
	HRESULT Convert_Line(wstring strLine);

public:
	wstring m_strLine = L"";
	vector<UINT> m_vecIndex;
	Texture* m_pTexture = nullptr;

public:
	static Font* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strLine = L"");
	virtual void Free(void);
};

