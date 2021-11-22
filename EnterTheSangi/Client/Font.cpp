#include "framework.h"
#include "Font.h"
#include "Texture.h"

Font::Font(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Font::~Font()
{
	Free();
}

HRESULT Font::Ready_GameObject(string strName, float fSize, bool align)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Font");
	if (!m_pShader) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"UI", L"Font");
	if (!m_pTexture) return E_FAIL;

	m_bAlign = align;
	m_fSize = fSize;
	m_strLine = strName;
	Convert_Line(strName);

	return GameObject::Ready_GameObject();
}

INT Font::Update_GameObject(float time_delta)
{
	return GameObject::Update_GameObject(time_delta);
}

INT Font::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT Font::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_fSize, m_fSize, m_fSize);
	for(int i = 0; i < m_vecIndex.size(); ++i)
	{
		if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture", m_vecIndex[i])))
			return E_FAIL;

		D3DXVECTOR3 vPos = { m_fSpace * m_fSize * i + m_vPos.x, m_vPos.y, 0.f };
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		matWorld = matScale * matTrans;
		pEffect->SetMatrix("g_matWorld", &matWorld);

		D3DXMATRIX		matTmp;
		m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
		pEffect->SetMatrix("g_matView", &matTmp);

		m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
		pEffect->SetMatrix("g_matProj", &matTmp);

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		if (FAILED(m_pTexture->Draw_Sprite(m_vecIndex[i])))
			return E_FAIL;

		pEffect->EndPass();
		pEffect->End();
	}

	return GameObject::Render_GameObject();
}

HRESULT Font::Convert_Line(string strLine)
{
	int idx = -1;
	char c = ' ';
	m_vecIndex.clear();
	m_vecIndex.reserve(strLine.length());
	for(int i = 0; i < strLine.length(); ++i)
	{
		c = strLine[i];

		if (c > 64 && c < 91)
			idx = c - 65;
		if (c > 96 && c < 123)
			idx = c - 97;

		if(idx < 0 || idx > 25)
		{
			idx = -1;
			continue;
		}
		m_vecIndex.push_back(idx);
	}

	m_vAlign = { 0.f, 0.f, 0.f };
	if(m_bAlign)
	{
		float fLen = m_strLine.length() * m_fSpace * m_fSize * 0.5f * -1.f;
		m_vAlign = { fLen, 0.f, 0.f };
	}

	m_strLine = strLine;

	return NOERROR;
}

HRESULT Font::Update_Position(D3DXVECTOR3 vPos, D3DXVECTOR3 vOffset)
{
	m_vPos = vPos + vOffset + m_vAlign;

	return NOERROR;
}

Font* Font::Create(LPDIRECT3DDEVICE9 pGraphicDev, string strName, float fSize, bool align)
{
	Font* pInstance = new Font(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(strName, fSize, align)))
		SafeDelete(pInstance);
	return pInstance;
}

void Font::Free()
{
	GameObject::Free();
}
