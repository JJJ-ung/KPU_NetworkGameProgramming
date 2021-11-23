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

	//if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 0, true))))
	//	return E_FAIL;
	//if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 1, false))))
	//	return E_FAIL;
	//if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 2, false))))
	//	return E_FAIL;

	//for(auto p : m_pGameMgr->Get_ClientInfo())
	//{
	//	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, p.id, false))))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, m_pPostCard[tLogin.id] = PostCard::Create(m_pGraphic_Device, tLogin.id, true))))
		return E_FAIL;

	for(int i = 0; i < 3; ++i)
	{
		if(i == tLogin.id)
			continue;
		if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, m_pPostCard[i] = PostCard::Create(m_pGraphic_Device, i, false))))
			return E_FAIL;
	}

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

HRESULT Scene_Customize::Update_PlayerColor(sc_packet_change_color tRecv)
{
	if (tRecv.type != SC_PACKET_CHANGE_COLOR)
		return E_FAIL;

	int id = tRecv.id;
	D3DXVECTOR3 t = tRecv.body_color;

	m_pPostCard[id]->Get_Player()->Get_CustomInfo().vBody = tRecv.body_color;
	m_pPostCard[id]->Get_Player()->Get_CustomInfo().vCloth = tRecv.cloth_color;

	return NOERROR;
}

HRESULT Scene_Customize::Add_OtherPlayer(int id)
{
	return NOERROR;
}

unsigned Scene_Customize::Thread_Recv(void* pArg)
{
	Scene_Customize* pScene = (Scene_Customize*)pArg;

	EnterCriticalSection(pScene->Get_Crt());

	while(!pScene->m_bFinish)
	{
		// 쓰레드에서 할거
		cout << "***" << endl;

		//
		void* p = malloc(1024);
		char c = pScene->m_pNetworkMgr->Recv_ServerInfo(p);

		if(c == SC_PACKET_CHANGE_COLOR)
		{
			sc_packet_change_color t;
			memcpy(&t, p, sizeof(sc_packet_login_ok));
			delete p;
			pScene->Update_PlayerColor(t);
			cout << "Changed Color" << endl;
		}

		//if (c == SC_PACKET_LOGIN_OTHER_CLIENT)
		//{
		//	sc_packet_login_other_client t;
		//	memcpy(&t, p, sizeof(sc_packet_login_other_client));
		//	delete p;
		//	if (FAILED(pScene->m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(pScene->m_pGraphic_Device, t.id, false))))
		//		return E_FAIL;
		//	cout << "sdfh" << endl;
		//}
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
