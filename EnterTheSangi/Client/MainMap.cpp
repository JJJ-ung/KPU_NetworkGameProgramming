#include "framework.h"
#include "MainMap.h"

#include "Texture.h"
#include "Shader.h"
#include "GameMgr.h"
#include "ShaderMgr.h"

MainMap::MainMap(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

MainMap::~MainMap()
{
	Free();
}

HRESULT MainMap::Ready_GameObject()
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Default");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"Map", L"Main");
	if (!m_pTexture) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	m_vCenter = { m_pTexture->Get_TextureInfo().Width * 0.5f, m_pTexture->Get_TextureInfo().Height * 0.5f, 0.f };

	return GameObject::Ready_GameObject();
}

INT MainMap::Update_GameObject(float time_delta)
{
	if(m_pInputMgr->KeyDown(KEY_SHIFT))
	{
		D3DXVECTOR3 v = m_pGameMgr->Get_PlayerPos();
		v += m_pInputMgr->Get_MousePoint();
		cout << v.x << ", " << v.y << endl;
	}

	return GameObject::Update_GameObject(time_delta);
}

INT MainMap::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_PRIORITY, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT MainMap::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale;
	D3DXMatrixScaling(&matScale, 4.f, 4.f, 4.f);
	pEffect->SetMatrix("g_matWorld", &matScale);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pTexture->Draw_Sprite(0, &m_vCenter)))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

MainMap* MainMap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	MainMap* pInstance = new MainMap(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject()))
		SafeDelete(pInstance);
	return pInstance;
}

void MainMap::Free()
{
	GameObject::Free();
}
