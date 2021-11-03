#include "framework.h"
#include "GameMgr.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(GameMgr)

GameMgr::GameMgr()
{
}

GameMgr::~GameMgr()
{
	Free();
}

HRESULT GameMgr::Add_GameObject(OBJECT::TYPE eType, GameObject* pObj)
{
	if (eType == OBJECT::TYPE::END) return E_FAIL;
	if (!pObj) return E_FAIL;

	m_lstObj[eType].emplace_back(pObj);
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

HRESULT GameMgr::Clear_ObjectList(OBJECT::TYPE eType)
{
	for_each(m_lstObj[eType].begin(), m_lstObj[eType].end(), [](GameObject* p) { SafeDelete(p); });
	m_lstObj[eType].clear();

	return NOERROR;
}

void GameMgr::Free()
{
	for (int i = 0; i < OBJECT::TYPE::END; ++i)
		Clear_ObjectList((OBJECT::TYPE)i);
}
