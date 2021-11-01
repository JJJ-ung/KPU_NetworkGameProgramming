#include "framework.h"
#include "Buffer.h"

Buffer::Buffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Component(pGraphic_Device)
{
}

Buffer::~Buffer()
{
	Free();
}

HRESULT Buffer::Ready_Component()
{
	m_dwVtxCnt = 4;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTXTEX);

	m_IdxFmt = D3DFMT_INDEX16;

	m_dwIdxSize = sizeof(INDEX16);

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPosition = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT Buffer::Render_Buffer()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_dwVtxSize)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetFVF(m_dwVtxFVF)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetIndices(m_pIB)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt)))
		return E_FAIL;

	return NOERROR;
}

Buffer* Buffer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Buffer* pInstance = new Buffer(pDevice);

	if (FAILED(pInstance->Ready_Component()))
		SafeDelete(pInstance);

	return pInstance;
}

void Buffer::Free()
{
	SafeDelete(m_pVB);
	SafeDelete(m_pIB);
	Component::Free();
}
