#pragma once

class Scene;
class GameObject;
class GameMgr
{
	DECLARE_SINGLETON(GameMgr)

private:
	GameMgr();
	~GameMgr();

public:
	HRESULT Set_CurrScene(Scene* pScene);
	HRESULT Add_GameObject(OBJECT::TYPE eType, GameObject* pObj);

public:
	HRESULT Set_PrototypesOnScene(OBJECT::TYPE eType);
	HRESULT Add_Prototype(OBJECT::TYPE eType, GameObject* pObj);

public:
	int Update_GameMgr(float TimeDelta);
	HRESULT Render_GameMgr();

public:
	HRESULT Update_GameObject(float TimeDelta);
	HRESULT LateUpdate_GameObject(float TimeDelta);

public:
	HRESULT Clear_ObjectList(OBJECT::TYPE eType);

private:
	Scene* m_pCurrScene = nullptr;

private:
	typedef list<GameObject*> OBJLST;
	OBJLST m_lstObj[OBJECT::END];
	OBJLST m_lstPrototype[OBJECT::END];

private:
	void Free();
};

