#include "framework.h"
#include "GameMgr.h"
#include "GameObject.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(GameMgr)

GameMgr::GameMgr()
{
	for (int i = 0; i < 3; ++i)
		m_vecClients.emplace_back(CLIENT());
}

GameMgr::~GameMgr()
{
	Free();
}

HRESULT GameMgr::Set_CurrScene(Scene* pScene)
{
	if (!pScene) return E_FAIL;

	if (m_pCurrScene == pScene) return E_FAIL;

	SafeDelete(m_pCurrScene);

	if(!m_pCurrScene)
		m_pCurrScene = pScene;

	return NOERROR;
}

int GameMgr::Update_GameMgr(float TimeDelta)
{
	if (!m_pCurrScene) return -1;

	if (FAILED(Update_GameObject(TimeDelta)))
		return -1;

	if (FAILED(LateUpdate_GameObject(TimeDelta)))
		return -1;

	return m_pCurrScene->Update_Scene(TimeDelta);
}

HRESULT GameMgr::Render_GameMgr()
{
	if (!m_pCurrScene) return NOERROR;

	return m_pCurrScene->Render_Scene();
}

HRESULT GameMgr::Add_GameObject(OBJECT::TYPE eType, GameObject* pObj)
{
	if (eType == OBJECT::TYPE::END) return E_FAIL;
	if (!pObj) return E_FAIL;

	m_lstObj[eType].emplace_back(pObj);
	return NOERROR;
}

HRESULT GameMgr::Set_PrototypesOnScene(OBJECT::TYPE eType)
{
	if (eType == OBJECT::TYPE::END) return E_FAIL;
	if (!m_lstObj[eType].empty()) 
		return E_FAIL;

	copy(m_lstPrototype[eType].begin(), m_lstPrototype[eType].end(), back_inserter(m_lstObj[eType]));
	m_lstPrototype[eType].clear();

	return NOERROR;
}

HRESULT GameMgr::Add_Prototype(OBJECT::TYPE eType, GameObject* pObj)
{
	if (eType == OBJECT::TYPE::END) return E_FAIL;
	if (!pObj) return E_FAIL;

	m_lstPrototype[eType].emplace_back(pObj);
	return NOERROR;
}

HRESULT GameMgr::Update_GameObject(float TimeDelta)
{
	for (int i = 0; i < OBJECT::END; ++i)
	{
		auto iter = m_lstObj[i].begin();
		for (; iter != m_lstObj[i].end(); )
		{
			if ((*iter)->Update_GameObject(TimeDelta) == OBJDEAD)
			{
				SafeDelete(*iter);
				iter = m_lstObj[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return NOERROR;
}

HRESULT GameMgr::LateUpdate_GameObject(float TimeDelta)
{
	for (int i = 0; i < OBJECT::END; ++i)
	{
		auto iter = m_lstObj[i].begin();
		for (; iter != m_lstObj[i].end(); )
		{
			if ((*iter)->LateUpdate_GameObject(TimeDelta) == OBJDEAD)
			{
				SafeDelete(*iter);
				iter = m_lstObj[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return NOERROR;
}

int GameMgr::Update_Networking()
{
	for (int i = 0; i < OBJECT::END; ++i)
	{
		auto iter = m_lstObj[i].begin();
		for (; iter != m_lstObj[i].end(); )
		{
			if ((*iter)->Update_Networking() == -1)
				return -1;
			else
				++iter;
		}
	}

	return 0;
}

int GameMgr::Recv_Networking(char c, void* p)
{
	for (int i = 0; i < OBJECT::END; ++i)
	{
		auto iter = m_lstObj[i].begin();
		for (; iter != m_lstObj[i].end(); )
		{
			if ((*iter)->Recv_Networking(c, p) == -1)
				return -1;
			else
				++iter;
		}
	}

	return 0;
}

HRESULT GameMgr::Clear_Scene()
{
	cout << "Clear" << endl;
	for (int i = 0; i < OBJECT::TYPE::END; ++i)
		Clear_ObjectList((OBJECT::TYPE)i);

	return NOERROR;
}

HRESULT GameMgr::Clear_ObjectList(OBJECT::TYPE eType)
{
	for_each(m_lstObj[eType].begin(), m_lstObj[eType].end(), [](GameObject* p) { SafeDelete(p); });
	m_lstObj[eType].clear();

	return NOERROR;
}

HRESULT GameMgr::Setup_Recv(char type, void* pRecv)
{
	if (!m_pCurrScene)
		return E_FAIL;

	if (FAILED(Recv_Networking(type, pRecv)))
		return E_FAIL;

	return m_pCurrScene->Setup_Recv(type, pRecv);
}

void GameMgr::Free()
{
	SafeDelete(m_pCurrScene);
	for (int i = 0; i < OBJECT::TYPE::END; ++i)
		Clear_ObjectList((OBJECT::TYPE)i);
	for (int i = 0; i < OBJECT::TYPE::END; ++i)
	{
		for_each(m_lstPrototype[i].begin(), m_lstPrototype[i].end(), [](GameObject* p) { SafeDelete(p); });
		m_lstPrototype[i].clear();
	}

}
