#include "framework.h"
#include "Scene_Customize.h"

#include "AnimatedSprite.h"
#include "InputMgr.h"
#include "Scene_Test.h"
#include "PostCard.h"
#include "CustomPlayer.h"
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

	//for(int i = 0; i < tLogin.id; ++i)
	//{
	//	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, m_pPostCard[i] = PostCard::Create(m_pGraphic_Device, i, false, ""))))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, m_pPostCard[tLogin.id] = PostCard::Create(m_pGraphic_Device, tLogin.id, true, m_pGameMgr->Get_ClientPlayerName()))))
		return E_FAIL;

	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Recv, this, 0, NULL);

	return Scene::Ready_Scene();
}

int Scene_Customize::Update_Scene(float time_delta)
{
	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Customize::Render_Scene()
{
	return Scene::Render_Scene();
}

HRESULT Scene_Customize::Update_PlayerColor(sc_packet_change_color* tRecv)
{
	if (!tRecv) return E_FAIL;

	if (tRecv->type != SC_PACKET_CHANGE_COLOR)
		return E_FAIL;

	if (!m_pPostCard[tRecv->id])
		return E_FAIL;

	m_pPostCard[tRecv->id]->Get_Player()->Get_CustomInfo().vBody = tRecv->body_color;
	m_pPostCard[tRecv->id]->Get_Player()->Get_CustomInfo().vCloth = tRecv->cloth_color;

	return NOERROR;
}

HRESULT Scene_Customize::Add_OtherPlayer(sc_packet_login_other_client* tRecv)
{
	if (!tRecv) return E_FAIL;

	if (tRecv->type != SC_PACKET_LOGIN_OTHER_CLIENT)
		return E_FAIL;

	if (m_pPostCard[tRecv->id])
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, m_pPostCard[tRecv->id] = PostCard::Create(m_pGraphic_Device, tRecv->id, false, tRecv->name))))
		return E_FAIL;

	return NOERROR;
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

unsigned Scene_Customize::Thread_Recv(void* pArg)
{
	Scene_Customize* pScene = (Scene_Customize*)pArg;

	EnterCriticalSection(pScene->Get_Crt());

	while(!pScene->m_bFinish)
	{
		void* p = malloc(BUF_SIZE);
		char c = pScene->m_pNetworkMgr->Recv_ServerInfo(p);
		switch (c)
		{
		case SC_PACKET_CHANGE_COLOR:
			if (FAILED(pScene->Update_PlayerColor((sc_packet_change_color*)p)))
				return E_FAIL;
			break;
		case SC_PACKET_LOGIN_OTHER_CLIENT:
			if (FAILED(pScene->Add_OtherPlayer((sc_packet_login_other_client*)p)))
				return E_FAIL;
			break;
		case SC_PACKET_READY:
			if (FAILED(pScene->Update_PlayerReady((sc_packet_ready*)p)))
				return E_FAIL;
			break;
		default:
			break;
		}
		delete p;
	}

	LeaveCriticalSection(pScene->Get_Crt());

	return 0;
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
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
	cout << "Customize Thread Closed" << endl;
	Scene::Free();
}
