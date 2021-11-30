#include "framework.h"
#include "Scene_Customize.h"
#include "InputMgr.h"
#include "Scene_Test.h"
#include "PostCard.h"
#include "CustomPlayer.h"
#include "Mouse.h"
#include "NetworkMgr.h"

Scene_Customize::Scene_Customize(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Scene(pGraphic_Device)
{
}

Scene_Customize::~Scene_Customize()
{
	Free();
}

HRESULT Scene_Customize::Ready_Scene(sc_packet_login_ok tLogin)
{
	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) 
		return E_FAIL;

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (!m_pNetworkMgr) 
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, m_pPostCard[tLogin.id] = PostCard::Create(m_pGraphic_Device, tLogin.id, true, m_pGameMgr->Get_ClientPlayerName(), tLogin.body_color, tLogin.cloth_color))))
		return E_FAIL;
	m_pGameMgr->Get_ClientInfos()[tLogin.id] = CLIENT(true, tLogin.id, m_pGameMgr->Get_ClientPlayerName(), tLogin.cloth_color, tLogin.body_color, D3DXVECTOR3(0.f, 0.f, 0.f));

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, Mouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	return Scene::Ready_Scene();
}

int Scene_Customize::Update_Scene(float time_delta)
{
	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Customize::Render_Scene()
{
	if(m_bSceneChange)
	{
		m_pGameMgr->Clear_Scene();
		Scene_Test* pScene = Scene_Test::Create(m_pGraphic_Device);
		if (FAILED(m_pGameMgr->Set_CurrScene(pScene)))
		{
			cout << "Failed To Change Scene" << endl;
			return E_FAIL;
		}
	}

	return Scene::Render_Scene();
}

HRESULT Scene_Customize::Update_PlayerColor(sc_packet_change_color* tRecv)
{
	if (!tRecv) return E_FAIL;

	if (tRecv->type != SC_PACKET_CHANGE_COLOR)
		return E_FAIL;

	if (!m_pPostCard[tRecv->id])
		return E_FAIL;

	if (!m_pPostCard[tRecv->id]->Get_Player())
		return E_FAIL;

	m_pPostCard[tRecv->id]->Get_Player()->Get_CustomInfo().vBody = tRecv->body_color;
	m_pPostCard[tRecv->id]->Get_Player()->Get_CustomInfo().vCloth = tRecv->cloth_color;

	m_pGameMgr->Get_ClientInfos()[tRecv->id].custom = m_pPostCard[tRecv->id]->Get_Player()->Get_CustomInfo();

	return NOERROR;
}

HRESULT Scene_Customize::Add_OtherPlayer(sc_packet_login_other_client* tRecv)
{
	if (!tRecv) return E_FAIL;

	if (tRecv->type != SC_PACKET_LOGIN_OTHER_CLIENT)
		return E_FAIL;

	if (m_pPostCard[tRecv->id])
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, m_pPostCard[tRecv->id] = PostCard::Create(m_pGraphic_Device, tRecv->id, false, tRecv->name, tRecv->body_color, tRecv->cloth_color))))
		return E_FAIL;
	m_pGameMgr->Get_ClientInfos()[tRecv->id] = CLIENT(false, tRecv->id, tRecv->name, tRecv->cloth_color, tRecv->body_color, D3DXVECTOR3(0.f, 0.f, 0.f));

	return m_pPostCard[tRecv->id]->Setup_Ready(tRecv->is_ready);
}

HRESULT Scene_Customize::Update_PlayerReady(sc_packet_ready* tRecv)
{
	if (!tRecv) return E_FAIL;

	if (tRecv->type != SC_PACKET_READY)
		return E_FAIL;

	if (!m_pPostCard[tRecv->id])
		return E_FAIL;

	return m_pPostCard[tRecv->id]->Setup_Ready(tRecv->is_ready);
}

HRESULT Scene_Customize::Setup_Recv(char c, void* recv)
{
	if (!recv) return E_FAIL;

	switch (c)
	{
	case SC_PACKET_CHANGE_COLOR:
		if (FAILED(Update_PlayerColor((sc_packet_change_color*)recv)))
			return E_FAIL;
		break;
	case SC_PACKET_LOGIN_OTHER_CLIENT:
		if (FAILED(Add_OtherPlayer((sc_packet_login_other_client*)recv)))
			return E_FAIL;
		break;
	case SC_PACKET_READY:
		if (FAILED(Update_PlayerReady((sc_packet_ready*)recv)))
			return E_FAIL;
		break;
	case SC_PACKET_ALL_READY:
		m_bSceneChange = true;
		break;
	default:
		break;
	}

	return Scene::Setup_Recv(c, recv);
}

Scene_Customize* Scene_Customize::Create(LPDIRECT3DDEVICE9 pGraphic_Device, sc_packet_login_ok tLogin)
{
	Scene_Customize* pInstance = new Scene_Customize(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene(tLogin)))
	{
		MSG_BOX("Failed To Create Scene_Customize Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Scene_Customize::Free()
{
	cout << "Customize Thread Closed" << endl;
	Scene::Free();
}
