#include "framework.h"
#include "Target.h"

Target::Target(LPDIRECT3DDEVICE9 pDevice)
	:m_pGraphic_Device(pDevice)
{
}

Target::~Target()
{
	Free();
}

HRESULT Target::Ready_Target(UINT iSizeX, UINT iSizeY, D3DFORMAT eFormat, D3DXCOLOR Color)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(D3DXCreateTexture(m_pGraphic_Device, iSizeX, iSizeY, 1, D3DUSAGE_RENDERTARGET, eFormat, D3DPOOL_DEFAULT, &m_pTexture)))
		return E_FAIL;

	if (FAILED(m_pTexture->GetSurfaceLevel(0, &m_pSurface)))
		return E_FAIL;

	m_ClearColor = Color;

	return NOERROR;
}

HRESULT Target::SetUp_OnShader(LPD3DXEFFECT pEffect, D3DXHANDLE hHandle)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->SetTexture(hHandle, m_pTexture);

	return NOERROR;
}

HRESULT Target::SetUp_OnGraphicDev(UINT iIndex)
{
	m_pGraphic_Device->GetRenderTarget(iIndex, &m_pOldSurface);

	m_pGraphic_Device->SetRenderTarget(iIndex, m_pSurface);

	return NOERROR;
}

HRESULT Target::Release_OnGraphicDev(UINT iIndex)
{
	m_pGraphic_Device->SetRenderTarget(iIndex, m_pOldSurface);

	SafeRelease(m_pOldSurface);

	return NOERROR;
}

HRESULT Target::Clear_Target()
{
	m_pGraphic_Device->GetRenderTarget(0, &m_pOldSurface);

	m_pGraphic_Device->SetRenderTarget(0, m_pSurface);

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pGraphic_Device->SetRenderTarget(0, m_pOldSurface);

	SafeRelease(m_pOldSurface);

	return NOERROR;
}

HRESULT Target::Ready_Debug_Buffer(float fX, float fY, float fSizeX, float fSizeY)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXVIEWPORTTEX) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXVIEWPORTTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = D3DXVECTOR4(fX, fY, 0.f, 1.f);
	pVertices[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertices[1].vPosition = D3DXVECTOR4(fX + fSizeX, fY, 0.f, 1.f);
	pVertices[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertices[2].vPosition = D3DXVECTOR4(fX + fSizeX, fY + fSizeY, 0.f, 1.f);
	pVertices[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertices[3].vPosition = D3DXVECTOR4(fX, fY + fSizeY, 0.f, 1.f);
	pVertices[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	INDEX16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT Target::Render_Debug_Buffer()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, m_pTexture);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORTTEX));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	return NOERROR;
}

Target* Target::Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iSizeX, UINT iSizeY, D3DFORMAT eFormat, D3DXCOLOR Color)
{
	Target* pInstance = new Target(pGraphic_Device);

	if (FAILED(pInstance->Ready_Target(iSizeX, iSizeY, eFormat, Color)))
		SafeDelete(pInstance);

	return pInstance;
}

void Target::Free()
{
#ifdef _DEBUG
	SafeRelease(m_pVB);
	SafeRelease(m_pIB);
#endif

	SafeRelease(m_pSurface);
	SafeRelease(m_pTexture);
}
