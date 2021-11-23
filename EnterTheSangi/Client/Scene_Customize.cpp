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

HRESULT Scene_Customize::Ready_Scene()
{
	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) 
		return E_FAIL;

	m_pNetworkMgr = NetworkMgr::GetInstance();
	if (!m_pNetworkMgr) 
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 0, true))))
		return E_FAIL;
	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 1, false))))
		return E_FAIL;
	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, PostCard::Create(m_pGraphic_Device, 2, false))))
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

unsigned Scene_Customize::Thread_Recv(void* pArg)
{
	Scene_Customize* pScene = (Scene_Customize*)pArg;

	EnterCriticalSection(pScene->Get_Crt());

	while(!pScene->m_bFinish)
	{
		// 쓰레드에서 할거
		cout << "***" << endl;

		char c = ' ';
		void* p = pScene->m_pNetworkMgr->Recv_ServerInfo(c);

		if(c == SC_PACKET_CHANGE_COLOR)
		{
			cout << "Change Color" << endl;
		}
	}

	LeaveCriticalSection(pScene->Get_Crt());

	return 0;
}

Scene_Customize* Scene_Customize::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Scene_Customize* pInstance = new Scene_Customize(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
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
