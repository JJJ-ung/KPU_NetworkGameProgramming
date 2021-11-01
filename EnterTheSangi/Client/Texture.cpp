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

HRESULT Texture::Ready_Component(const TCHAR* pPath, const UINT& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9* pTexture = nullptr;

	for (UINT i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		wsprintf(szFileName, pPath, i);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
			return E_FAIL;

		m_vecTexture.push_back(pTexture);
	}

	return NOERROR;
}

HRESULT Texture::SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, UINT iIndex)
{
	if (m_vecTexture.size() <= iIndex ||
		nullptr == m_vecTexture[iIndex])
		return E_FAIL;

	return pEffect->SetTexture(pConstantName, m_vecTexture[iIndex]);
}

Texture* Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const UINT& iCnt)
{
	Texture* pInstance = new Texture(pGraphicDev);

	if (FAILED(pInstance->Ready_Component(pPath, iCnt)))
	{
		MSG_BOX("Failed To Create CTexture Instance");
		SafeDelete(pInstance);
	}
	return pInstance;
}

void Texture::Free()
{
	size_t iContainerSize = m_vecTexture.size();

	for (size_t i = 0; i < iContainerSize; ++i)
		SafeDelete(m_vecTexture[i]);

	m_vecTexture.clear();

	Component::Free();
}
