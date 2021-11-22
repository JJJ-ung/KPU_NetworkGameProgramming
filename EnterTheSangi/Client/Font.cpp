#include "framework.h"
#include "Font.h"

Font::Font(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Component(pGraphic_Device)
{
}

Font::~Font()
{
	Free();
}

HRESULT Font::Ready_Component(wstring strLine)
{
	return NOERROR;
}

HRESULT Font::Render_Component()
{
	return Component::Render_Component();
}

HRESULT Font::Convert_Line(wstring strLine)
{
	int idx = -1;
	char c = ' ';
	m_vecIndex.clear();
	m_vecIndex.reserve(strLine.length());
	for(int i = 0; i < strLine.length(); ++i)
	{
		c = strLine[i];

		if (c > 65 && c < 91)
			idx = c - 65;
		if (c > 97 && c < 123)
			idx = c - 97;

		if(idx < 0 || idx > 25)
		{
			idx = -1;
			continue;
		}
		m_vecIndex.push_back(idx);
	}

	return NOERROR;
}

Font* Font::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strLine)
{
	return nullptr;
}

void Font::Free()
{
	Component::Free();
}
