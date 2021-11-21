#include "framework.h"
#include "Texture.h"

Texture::Texture(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Component(pGraphic_Device)
{
}

Texture::~Texture()
{
	Free();
}

HRESULT Texture::Ready_Component(LPD3DXSPRITE pSprite, const TCHAR* pPath, const UINT& iCnt)
{
	if (!pSprite) return E_FAIL;

	m_pSprite = pSprite;

	m_vecTexture.reserve(iCnt);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	TCHAR szFileName[128] = L"";

	for (UINT i = 0; i < iCnt; ++i)
	{

		wsprintf(szFileName, pPath, i);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
			return E_FAIL;

		m_vecTexture.push_back(pTexture);
	}

	D3DXGetImageInfoFromFile(szFileName, &m_tInfo);

	return NOERROR;
}

HRESULT Texture::Set_Texture(LPD3DXEFFECT pEffect, const char* pConstantName, UINT iIndex)
{
	if (m_vecTexture.size() <= iIndex || nullptr == m_vecTexture[iIndex])
		return E_FAIL;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	return pEffect->SetTexture(pConstantName, m_vecTexture[iIndex]);
}

HRESULT Texture::Draw_Sprite(UINT iIndex, const D3DXVECTOR3* pCenter, const RECT* pSrcRect, const D3DXVECTOR3* pPosition, D3DCOLOR Color)
{
	HRESULT hr =  m_pSprite->Draw(m_vecTexture[iIndex], pSrcRect, pCenter, pPosition, Color);

	m_pSprite->End();

	return hr;
}

Texture* Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXSPRITE pSprite, const TCHAR* pPath, const UINT& iCnt)
{
	Texture* pInstance = new Texture(pGraphicDev);

	if (FAILED(pInstance->Ready_Component(pSprite, pPath, iCnt)))
	{
		MSG_BOX("Failed To Create Texture Instance");
		SafeDelete(pInstance);
	}
	return pInstance;
}

void Texture::Free()
{
	size_t iContainerSize = m_vecTexture.size();

	for (size_t i = 0; i < iContainerSize; ++i)
		SafeRelease(m_vecTexture[i]);

	m_vecTexture.clear();

	Component::Free();
}
