#include "framework.h"
#include "Player.h"
#include "Font.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"
#include "ShaderMgr.h"
#include "ResourceMgr.h"

Player::Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Player::~Player()
{
	Free();
}

HRESULT Player::Ready_GameObject(CLIENT t)
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

	if (FAILED(Ready_Collision()))
		return E_FAIL;

	m_tClientInfo = t;
	m_iObjectID = t.index;

	m_vPosition = t.startpos;
	m_pGameMgr->Get_PlayerPos() = m_vPosition;

	m_pStateMachine = StateMachine<Player>::Create(this, Player_Idle::Instance());
	if (!m_pStateMachine)
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, m_pNameTag = Font::Create(m_pDevice, t.name, 0.5f, true, true))))
		return E_FAIL;
	if (!m_pNameTag) return E_FAIL;
	m_pNameTag->Update_Position(m_vPosition, D3DXVECTOR3(0.f, 100.f, 0.f));

	D3DXMatrixIdentity(&m_matWorld);


	D3DXCreateLine(m_pDevice, &m_pLine);
	m_pLine->SetWidth(2);


	return GameObject::Ready_GameObject();
}

INT Player::Update_GameObject(float time_delta)
{
	m_pStateMachine->Update_Component(time_delta);

	m_pNameTag->Update_Position(m_vPosition, D3DXVECTOR3(0.f, -100.f, 0.f));

	//cout << m_vPosition.x << "/" << m_vPosition.y << "/" << m_vPosition.z << endl;

	D3DXMATRIX		matScale, matTrans;
	D3DXMatrixScaling(&matScale, 3.f * m_fSide, 3.f, 3.f);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matScale * matTrans;

	m_pGameMgr->Get_PlayerPos() = m_vPosition;

	if (m_pInputMgr->KeyDown(KEY_F1))
		m_bRenderCollision = !m_bRenderCollision;

	return GameObject::Update_GameObject(time_delta);
}

INT Player::LateUpdate_GameObject(float time_delta)
{
	if (m_pInputMgr->Get_Angle() < 0.f)
		m_fSide = -1.f;
	else
		m_fSide = 1.f;

	m_pRenderer->Add_RenderList(Renderer::RENDER_NONALPHA, this);

	return GameObject::LateUpdate_GameObject(time_delta);
}

HRESULT Player::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect) return E_FAIL;

	if (FAILED(m_pCurrAnimation->Set_Texture(pEffect, "g_BaseTexture")))
		return E_FAIL;

	pEffect->SetMatrix("g_matWorld", &m_matWorld);

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_TEXTURE0, &matView);
	pEffect->SetMatrix("g_matView", &matView);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pShader->Set_Value("g_vCloth", &m_tClientInfo.custom.vCloth, sizeof(D3DXVECTOR3));
	m_pShader->Set_Value("g_vBody", &m_tClientInfo.custom.vBody, sizeof(D3DXVECTOR3));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(m_pCurrAnimation->Draw_Sprite()))
		return E_FAIL;

	pEffect->EndPass();
	pEffect->End();

	if(m_bRenderCollision)
	{
		for (auto t : m_vecColl)
			Render_Rect(t, matView, matProj);

		Render_Rect(m_tCollRect, matView, matProj);
	}

	return GameObject::Render_GameObject();
}

INT Player::Update_Networking()
{
	m_pNetworkMgr->Send_PlayerInfo(m_eState, m_vPosition, m_pInputMgr->Get_Angle());

	return GameObject::Update_Networking();
}

INT Player::Recv_Networking(char c, void* p)
{
	return GameObject::Recv_Networking(c, p);
}

INT Player::Update_Movement(float TimeDelta)
{
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	m_vPosition += m_vDirection * m_fSpeed * TimeDelta;

	Update_Collision(TimeDelta);

	return 0;
}

bool Player::Update_Animation(float TimeDelta)
{
	return m_pCurrAnimation->Update_Animation(TimeDelta);
}

HRESULT Player::Change_Animation(wstring strTag)
{
	if (!m_mapAnimations[strTag])
		return E_FAIL;
	if(m_pCurrAnimation == m_mapAnimations[strTag])
		return NOERROR;

	m_pCurrAnimation = m_mapAnimations[strTag];
	m_pCurrAnimation->Reset_Animation();
	return NOERROR;
}

STATE::DIR Player::Compute_Direction()
{
	float f = m_pInputMgr->Get_Angle_Abs();

	if (f < 45.f)
		return STATE::FRONT;
	else if (f < 90.f)
		return STATE::SIDE;
	else if (f < 135.f)
		return STATE::BACKSIDE;
	else
		return STATE::BACK;

	return STATE::END;
}

wstring Player::Direction_Tag(wstring strTag)
{
	wstring out = strTag;
	float f = m_pInputMgr->Get_Angle_Abs();
	if (f < 45.f)
		out += L"_Front";
	else if (f < 90.f)
		out += L"_Side";
	else if (f < 135.f)
		out += L"_BackSide";
	else
		out += L"_Back";
	return out;
}

HRESULT Player::Ready_AnimationInfo()
{
	Texture* pTexture = nullptr;
	Animation* pAnimation = nullptr;

	wifstream fin;

	fin.open(L"../Binary/Data/Anim_Player.txt");
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

		pAnimation->Get_Center().y = (float)pAnimation->Get_Texture()->Get_TextureInfo().Height;

		m_mapAnimations.insert(MAPANI::value_type(tag, pAnimation));
	}

	return NOERROR;
}

HRESULT Player::Ready_Collision()
{
	wifstream fin;

	fin.open(L"../Binary/Data/Collision.txt");
	if (fin.fail()) return E_FAIL;

	RECT t;
	while (true)
	{
		fin >> t.left >> t.right >> t.top >> t.bottom;

		if (fin.eof()) break;

		t.left *= 4;
		t.right *= 4;
		t.top *= 4;
		t.bottom *= 4;

		m_vecColl.push_back(t);
	}

	return NOERROR;
}

INT Player::Update_Collision(float TimeDelta)
{
	m_tCollRect.right			= LONG(m_vPosition.x + 24.f);
	m_tCollRect.left			= LONG(m_vPosition.x - 24.f);
	m_tCollRect.top			= LONG(m_vPosition.y - 20.f);
	m_tCollRect.bottom	= LONG(m_vPosition.y);

	RECT tIntersect = {};
	for(auto t : m_vecColl)
	{
		if(IntersectRect(&tIntersect, &m_tCollRect, &t))
		{
			LONG width = tIntersect.right - tIntersect.left;
			LONG height = tIntersect.bottom - tIntersect.top;

			if (width > height)
			{
				LONG CollCenter = t.bottom - (t.bottom - t.top) * 0.5;
				LONG PlayerCenter = m_tCollRect.bottom - (m_tCollRect.bottom - m_tCollRect.top) * 0.5;
				if (CollCenter < PlayerCenter) // 충돌체가 더 위에 있음
					m_vPosition.y += height;
				else
					m_vPosition.y -= height;
			}
			//오른쪽 왼쪽 충돌
			else
			{
				LONG CollCenter = t.right - (t.right - t.left) * 0.5;
				LONG PlayerCenter = m_tCollRect.right - (m_tCollRect.right - m_tCollRect.left) * 0.5;
				if (CollCenter < PlayerCenter) // 충돌체가 더 왼쪽에 있음
					m_vPosition.x += width;
				else
					m_vPosition.x -= width;
			}
		}
	}

	return 0;
}

HRESULT Player::Render_Rect(RECT t, D3DXMATRIX view, D3DXMATRIX proj)
{
	D3DXVECTOR3 vLst[] =
	{
		{ (float)t.left,		(float)t.top,		0.f },
		{ (float)t.right,	(float)t.top,		0.f },
		{ (float)t.right,	(float)t.bottom,0.f },
		{ (float)t.left,		(float)t.bottom,0.f },
		{ (float)t.left,		(float)t.top,		0.f }
	};
	D3DXMATRIX		matTrans;
	matTrans = view * proj;

	m_pLine->Begin();
	//m_pLine->Draw(vLst, 5, D3DCOLOR_XRGB(255, 0, 0));
	m_pLine->DrawTransform(vLst, 5, &matTrans, D3DCOLOR_XRGB(255, 0, 0));
	m_pLine->End();

	return NOERROR;
}

Player* Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CLIENT t)
{
	Player* pInstance = new Player(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject(t)))
		SafeDelete(pInstance);
	return pInstance;
}

void Player::Free()
{
	for(auto iter : m_mapAnimations)
		SafeDelete(iter.second);
	m_mapAnimations.clear();
	SafeDelete(m_pStateMachine);
	GameObject::Free();
}
