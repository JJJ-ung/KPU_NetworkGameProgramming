#include "framework.h"
#include "NetworkPlayer.h"

#include "Font.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"
#include "Weapon.h"

NetworkPlayer::NetworkPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Player(pGraphic_Device)
{
}

NetworkPlayer::~NetworkPlayer()
{
	Free();
}

HRESULT NetworkPlayer::Ready_GameObject(CLIENT t)
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

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (!m_pNetworkMgr) return E_FAIL;

	if (FAILED(Ready_AnimationInfo()))
		return E_FAIL;

	m_tClientInfo = t;
	m_iObjectID = t.index;

	m_vPosition = t.startpos;

	m_eState = STATE::IDLE;

	Change_Animation(L"Idle_Front");

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pNameTag = Font::Create(m_pDevice, t.name, 0.5f, true, true))))
		return E_FAIL;
	if (!m_pNameTag) return E_FAIL;
	m_pNameTag->Update_Position(m_vPosition, D3DXVECTOR3(0.f, -100.f, 0.f));

	return NOERROR;
}

INT NetworkPlayer::Update_GameObject(float time_delta)
{
	m_pCurrAnimation->Update_Component(time_delta);

	m_pNameTag->Update_Position(m_vPosition, D3DXVECTOR3(0.f, -100.f, 0.f));

	if (m_iHealth == 0)
	{
		if (m_pWeapon)
		{
			m_pWeapon->Get_Delete() = true;
			m_pWeapon = nullptr;
		}
	}

	return GameObject::Update_GameObject(time_delta);
}

INT NetworkPlayer::LateUpdate_GameObject(float time_delta)
{
	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT NetworkPlayer::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pCurrAnimation->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	D3DXMATRIX		matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 3.f * m_fSide, 3.f, 3.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	matWorld = matScale * matTrans;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	D3DXMATRIX		matTmp;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	m_pShader->Set_Value("g_vCloth", &m_tClientInfo.custom.vCloth, sizeof(D3DXVECTOR3));
	m_pShader->Set_Value("g_vBody", &m_tClientInfo.custom.vBody, sizeof(D3DXVECTOR3));

	if (m_bDead)
		pEffect->SetFloat("g_fAlpha", 0.5f);
	else
		pEffect->SetFloat("g_fAlpha", 1.f);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pCurrAnimation->Draw_Sprite()))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	return GameObject::Render_GameObject();
}

HRESULT NetworkPlayer::Update_Weapon(float fLook)
{
	if (!m_pWeapon)
		return NOERROR;

	m_pWeapon->Get_Angle() = fLook * -1.f;

	return NOERROR;
}

HRESULT NetworkPlayer::Change_Weapon(sc_packet_change_weapon t)
{
	if (m_pWeapon)
	{
		m_pWeapon->Get_Delete() = true;
		m_pWeapon = nullptr;
	}

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::WEAPON, m_pWeapon = Weapon::Create(m_pDevice, this, t.weapon_id, true))))
		return E_FAIL;
	if (!m_pWeapon) return E_FAIL;

	return NOERROR;
}

INT NetworkPlayer::Update_Networking()
{
	return 0;
}

INT NetworkPlayer::Recv_Networking(char c, void* p)
{
	// ???? ?? ???? ????
	if (c != SC_PACKET_GAME_STATE)
		return 0;

	sc_packet_game_state t;
	memcpy(&t, p, sizeof(sc_packet_game_state));

	player_info_for_packet player = t.player[m_tClientInfo.index];

	wstring strAnimation = L"";

	m_vPosition.x = player.position.x;
	m_vPosition.y = player.position.y;

	if (player.state == (char)STATE::IDLE)
		strAnimation = L"Idle_";
	if (player.state == (char)STATE::RUN)
		strAnimation = L"Run_";
	if (player.state == (char)STATE::DODGE)
		strAnimation = L"Dodge_";

	if (abs(player.look) < 45)
		strAnimation += L"Front";
	else if (abs(player.look) < 90)
		strAnimation += L"Side";
	else if (abs(player.look) < 135)
		strAnimation += L"BackSide";
	else
		strAnimation += L"Back";

	if(player.look < 0)
		m_fSide = -1.f;
	else
		m_fSide = 1.f;

	Change_Animation(strAnimation);

	m_iHealth = player.health;
	if (m_iHealth == 0)
	{
		m_bDead = true;
		m_tClientInfo.custom.vBody = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		m_tClientInfo.custom.vCloth = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	}

	Update_Weapon(player.look);

	return 0;
}

NetworkPlayer* NetworkPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CLIENT t)
{
	NetworkPlayer* pInstance = new NetworkPlayer(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(t)))
		SafeDelete(pInstance);
	return pInstance;
}

void NetworkPlayer::Free()
{
	Player::Free();
}
