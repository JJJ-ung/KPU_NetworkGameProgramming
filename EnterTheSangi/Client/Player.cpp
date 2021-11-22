#include "framework.h"
#include "Player.h"

#include "Animation.h"
#include "ResourceMgr.h"
#include "Texture.h"
#include "DeviceMgr.h"
#include "Font.h"

Player::Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Player::~Player()
{
	Free();
}

HRESULT Player::Ready_GameObject(string strName)
{
	m_pRenderer = Renderer::GetInstance();
	if (!m_pRenderer) return E_FAIL;

	m_pResourceMgr = ResourceMgr::GetInstance();
	if (!m_pResourceMgr) return E_FAIL;

	m_pShader = ShaderMgr::GetInstance()->Get_ShaderReference(L"Player");
	if (!m_pShader) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	if (FAILED(Ready_AnimationInfo()))
		return E_FAIL;

	m_pCurrAnimation = m_mapAnimations[L"Idle_Front"];

	m_tCustomInfo.vBody = D3DXVECTOR3(rand() % 5 * 0.2f, rand() % 5 * 0.2f, rand() % 5 * 0.2f);
	m_tCustomInfo.vCloth = D3DXVECTOR3(rand() % 5 * 0.2f, rand() % 5 * 0.2f, rand() % 5 * 0.2f);

	m_strName = strName;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pNameTag = Font::Create(m_pDevice, m_strName, 0.5f))))
		return E_FAIL;
	if (!m_pNameTag) return E_FAIL;
	m_pNameTag->Update_Position(m_vPosition, D3DXVECTOR3(0.f, 48.f, 0.f));

	return GameObject::Ready_GameObject();
}

INT Player::Update_GameObject(float time_delta)
{
	if (m_pInputMgr->KeyPressing(KEY_W))
		m_vPosition += D3DXVECTOR3(0.f, time_delta * -200.f, 0.f);
	if (m_pInputMgr->KeyPressing(KEY_S))
		m_vPosition += D3DXVECTOR3(0.f, time_delta * 200.f, 0.f);
	if (m_pInputMgr->KeyPressing(KEY_A))
		m_vPosition += D3DXVECTOR3(time_delta * -200.f, 0.f, 0.f);
	if (m_pInputMgr->KeyPressing(KEY_D))
		m_vPosition += D3DXVECTOR3(time_delta * 200.f, 0.f, 0.f);

	m_pGameMgr->Get_PlayerPos() = m_vPosition;

	m_pCurrAnimation->Update_Component(time_delta);

	m_pNameTag->Update_Position(m_vPosition, D3DXVECTOR3(0.f, -48.f, 0.f));

	return GameObject::Update_GameObject(time_delta);
}

INT Player::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT Player::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pCurrAnimation->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	m_pShader->Set_Value("g_vCloth", &m_tCustomInfo.vCloth, sizeof(D3DXVECTOR3));
	m_pShader->Set_Value("g_vBody", &m_tCustomInfo.vBody, sizeof(D3DXVECTOR3));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pCurrAnimation->Draw_Sprite()))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

HRESULT Player::Ready_AnimationInfo()
{
	Texture* pTexture = nullptr;
	Animation* pAnimation = nullptr;

	wifstream fin;

	fin.open(L"../Binary/Data/Animation/Anim_Player.txt");
	if (fin.fail()) return E_FAIL;

	UINT speed, center, start;
	wstring tag;
	while (true)
	{
		fin >> tag >> speed >> center >> start;

		if (fin.eof()) break;

		pTexture = m_pResourceMgr->Find_Texture(L"Player", tag.c_str());
		pAnimation = Animation::Create(m_pDevice, pTexture, (float)speed, (bool)center, (float)start);
		if (!pAnimation) return E_FAIL;

		m_mapAnimations.insert(MAPANI::value_type(tag, pAnimation));
	}

	return NOERROR;
}

Player* Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device, string strName)
{
	Player* pInstance = new Player(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(strName)))
		SafeDelete(pInstance);
	return pInstance;
}

void Player::Free()
{
	for(auto iter : m_mapAnimations)
		SafeDelete(iter.second);
	m_mapAnimations.clear();

	GameObject::Free();
}
