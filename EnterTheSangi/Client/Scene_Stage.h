#pragma once
#include "Scene.h"

class Scene_Stage : public Scene
{
public:
	explicit Scene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Scene_Stage();

public:
	virtual HRESULT Ready_Scene();
	virtual int Update_Scene(float TimeDelta);
	virtual HRESULT Render_Scene();

public:
	virtual HRESULT Setup_Recv(char type, void* pRecv);

private:
	HRESULT Setup_Scene(sc_packet_game_state* recv);

private:
	int m_iWinner = -1;
	bool m_bSceneChange = false;
	bool m_bSetupScene = false;

public:
	static Scene_Stage* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};
