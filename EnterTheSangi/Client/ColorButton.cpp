#include "framework.h"
#include "ColorButton.h"
#include "Texture.h"
#include "CustomPlayer.h"
#include "NetworkMgr.h"
#include "Shader.h"
#include "ShaderMgr.h"

ColorButton::ColorButton(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

ColorButton::~ColorButton()
{
}

HRESULT ColorButton::Ready_GameObject(UINT iPlayerID, bool bActive)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Font");
	if (!m_pShader) return E_FAIL;

	m_pTexture = m_pResourceMgr->Find_Texture(L"UI", L"ColorIcon");
	if (!m_pTexture) return E_FAIL;

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (!m_pNetworkMgr) return E_FAIL;

	D3DXVECTOR3 vEye = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, -1.f);
	D3DXVECTOR3 vAt = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, 0.f);
	D3DXVECTOR3 vUP = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUP);

	m_iPlayerID = iPlayerID;
	m_vCenter = D3DXVECTOR3(m_pTexture->Get_TextureInfo().Width * 0.5f, m_pTexture->Get_TextureInfo().Height * 0.5f, 0.f);

	m_vPosition = D3DXVECTOR3(420.f * (int)(iPlayerID - 1), 185.f, 0.f);

	for(int i = 0; i < 3; ++i)
	{
		m_vBodyColors[i].color = D3DXVECTOR4(rand() % 5 * 0.2f, rand() % 5 * 0.2f, rand() % 5 * 0.2f, 0.f);
		m_vBodyColors[i].pos = m_vPosition + D3DXVECTOR3((i - 1) * 70.f, -25.f, 0.f);
		m_vBodyColors[i].collrect.top = (LONG)m_vBodyColors[i].pos.y - 20;
		m_vBodyColors[i].collrect.bottom = (LONG)m_vBodyColors[i].pos.y + 20;
		m_vBodyColors[i].collrect.right = (LONG)m_vBodyColors[i].pos.x + 20;
		m_vBodyColors[i].collrect.left = (LONG)m_vBodyColors[i].pos.x - 20;

		m_vClothColors[i].color = D3DXVECTOR4(rand() % 5 * 0.2f, rand() % 5 * 0.2f, rand() % 5 * 0.2f, 0.f);
		m_vClothColors[i].pos = m_vPosition + D3DXVECTOR3((i - 1) * 70.f, 25.f, 0.f);
		m_vClothColors[i].collrect.top = (LONG)m_vClothColors[i].pos.y - 20;
		m_vClothColors[i].collrect.bottom = (LONG)m_vClothColors[i].pos.y + 20;
		m_vClothColors[i].collrect.right = (LONG)m_vClothColors[i].pos.x + 20;
		m_vClothColors[i].collrect.left = (LONG)m_vClothColors[i].pos.x - 20;
	}

	m_bActive = bActive;

	return GameObject::Ready_GameObject();
}

INT ColorButton::Update_GameObject(float time_delta)
{
	if(m_pInputMgr->KeyDown(KEY_SPACE))
	{
		for (int i = 0; i < 3; ++i)
		{
			m_vBodyColors[i].color = D3DXVECTOR4(rand() % 5 * 0.2f, rand() % 5 * 0.2f, rand() % 5 * 0.2f, 0.f);
			m_vClothColors[i].color = D3DXVECTOR4(rand() % 5 * 0.2f, rand() % 5 * 0.2f, rand() % 5 * 0.2f, 0.f);
		}
	}

	return GameObject::Update_GameObject(time_delta);
}

INT ColorButton::LateUpdate_GameObject(float time_delta)
{
	if(m_bActive)
		m_pRenderer->Add_RenderList(Renderer::RENDER_UI, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT ColorButton::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;
	
	D3DXMATRIX		matTmp;
	D3DXMATRIX		matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 5.f, 5.f, 5.f);

	for (int i = 0; i < 3; ++i)
	{
		if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
			return E_FAIL;

		D3DXMatrixTranslation(&matTrans, m_vBodyColors[i].pos.x, m_vBodyColors[i].pos.y, m_vBodyColors[i].pos.z);
		matWorld = matScale * matTrans;
		pEffect->SetMatrix("g_matWorld", &matWorld);

		pEffect->SetMatrix("g_matView", &m_matView);

		m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
		pEffect->SetMatrix("g_matProj", &matTmp);

		m_pShader->Set_Value("g_vColor", &m_vBodyColors[i].color, sizeof(D3DXVECTOR4));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		if (FAILED(m_pTexture->Draw_Sprite(0, &m_vCenter)))
			return E_FAIL;

		pEffect->EndPass();
		pEffect->End();

		//

		if (FAILED(m_pTexture->Set_Texture(pEffect, "g_BaseTexture")))
			return E_FAIL;

		D3DXMatrixTranslation(&matTrans, m_vClothColors[i].pos.x, m_vClothColors[i].pos.y, m_vClothColors[i].pos.z);
		matWorld = matScale * matTrans;
		pEffect->SetMatrix("g_matWorld", &matWorld);

		pEffect->SetMatrix("g_matView", &m_matView);

		m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
		pEffect->SetMatrix("g_matProj", &matTmp);

		m_pShader->Set_Value("g_vColor", &m_vClothColors[i].color, sizeof(D3DXVECTOR4));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		if (FAILED(m_pTexture->Draw_Sprite(0, &m_vCenter)))
			return E_FAIL;

		pEffect->EndPass();
		pEffect->End();
	}

	return GameObject::Render_GameObject();
}

HRESULT ColorButton::Update_Collision(CustomPlayer* pPlayer)
{
	if (!pPlayer) return E_FAIL;

	if (!m_bActive) return NOERROR;

	if (!m_pInputMgr->KeyDown(KEY_LBUTTON))
		return NOERROR;

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x += LONG(1280.f * -0.5f);
	pt.y += LONG(720.f * -0.5f);

	cs_packet_change_color tSend{};
	for (int i = 0; i < 3; ++i)
	{
		if (PtInRect(&m_vBodyColors[i].collrect, pt))
		{
			//cout << "Body" << i << endl;
			m_pNetworkMgr->Send_ColorInfo(D3DXVECTOR3(m_vBodyColors[i].color.x, m_vBodyColors[i].color.y, m_vBodyColors[i].color.z), pPlayer->Get_CustomInfo().vCloth);

		}
		if (PtInRect(&m_vClothColors[i].collrect, pt))
		{
			//cout << "Cloth" << i << endl;
			m_pNetworkMgr->Send_ColorInfo(pPlayer->Get_CustomInfo().vBody, D3DXVECTOR3(m_vClothColors[i].color.x, m_vClothColors[i].color.y, m_vClothColors[i].color.z));
		}
	}

	return NOERROR;
}

ColorButton* ColorButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device, UINT iPlayerID, bool bActive)
{
	ColorButton* pInstance = new ColorButton(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(iPlayerID, bActive)))
		SafeDelete(pInstance);
	return pInstance;
}

void ColorButton::Free()
{
	GameObject::Free();
}
