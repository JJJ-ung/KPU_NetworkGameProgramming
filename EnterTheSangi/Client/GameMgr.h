#pragma once
class GameObject;
class GameMgr
{
	DECLARE_SINGLETON(GameMgr)

private:
	GameMgr();
	~GameMgr();

public:
	HRESULT Add_GameObject(OBJECT::TYPE eType, GameObject* pObj);
	HRESULT Update_GameObject(float TimeDelta);
	HRESULT LateUpdate_GameObject(float TimeDelta);

public:
	HRESULT Clear_ObjectList(OBJECT::TYPE eType);

private:
	void Free();

private:
	typedef list<GameObject*> OBJLST;
	OBJLST m_lstObj[OBJECT::END];
};

