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
	D3DXVECTOR3& Get_PlayerPos() { return m_vPlayerPos; }
	string& Get_ClientPlayerName() { return m_strClientPlayerName; }
	vector<CLIENT>& Get_ClientInfos() { return m_vecClients; }

public:
	HRESULT Set_CurrScene(Scene* pScene);
	HRESULT Add_GameObject(OBJECT::TYPE eType, GameObject* pObj);
	HRESULT Delete_GameObject(OBJECT::TYPE eType, char iObjID);
	GameObject* Get_GameObject(OBJECT::TYPE eType, char iObjID);

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
	int Update_Networking();
	int Recv_Networking(char c, void* p);

public:
	HRESULT Clear_Scene();
	HRESULT Clear_ObjectList(OBJECT::TYPE eType);

public:
	HRESULT Setup_Recv(char type, void* pRecv);

private:
	Scene* m_pCurrScene = nullptr;

private:
	typedef list<GameObject*> OBJLST;
	OBJLST m_lstObj[OBJECT::END];
	OBJLST m_lstPrototype[OBJECT::END];

private:
	vector<CLIENT> m_vecClients;
	string m_strClientPlayerName = "";
	D3DXVECTOR3 m_vPlayerPos{ 0.f, 0.f, 0.f };

private:
	void Free();
};

