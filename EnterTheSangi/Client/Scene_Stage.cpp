#include "framework.h"
#include "Scene_Stage.h"

#include "Player.h"
#include "Mouse.h"
#include "Camera.h"
#include "MainMap.h"
#include "NetworkPlayer.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Chest.h"

#include "Scene_Ending.h"

Scene_Stage::Scene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Scene(pGraphic_Device)
{
}

Scene_Stage::~Scene_Stage()
{
	Free();
}

HRESULT Scene_Stage::Ready_Scene()
{
	//cout << "Stage" << endl;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	return Scene::Ready_Scene();
}

int Scene_Stage::Update_Scene(float time_delta)
{
	return Scene::Update_Scene(time_delta);
}

HRESULT Scene_Stage::Render_Scene()
{
	if(m_bSceneChange)
	{
		m_pGameMgr->Clear_Scene();
		Scene_Ending* pScene = Scene_Ending::Create(m_pGraphic_Device, m_iWinner);
		if (FAILED(m_pGameMgr->Set_CurrScene(pScene)))
		{
			cout << "Failed To Change Scene" << endl;
			return E_FAIL;
		}
	}

	return Scene::Render_Scene();
}

HRESULT Scene_Stage::Setup_Recv(char c, void* recv)
{
	if(c == SC_PACKET_GAME_STATE)
	{
		if(!m_bSetupScene)
		{
			if (FAILED(Setup_Scene((sc_packet_game_state *)recv)))
				return E_FAIL;
			m_bSetupScene = true;
		}
		m_pGameMgr->Recv_Networking(c, recv);
	}

	// �߰� ����
	if(c == SC_PACKET_PUT_BULLET)
	{
		sc_packet_put_bullet t = {};
		memcpy(&t, recv, sizeof(sc_packet_put_bullet));
		cout << "Add " << t.bullet_id << endl;
		if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::BULLET, Bullet::Create(m_pGraphic_Device, t))))
			return E_FAIL;
	}
	if (c == SC_PACKET_PUT_CHEST)
	{
		cout << "add chest" << endl;
		sc_packet_put_chest t = {};
		memcpy(&t, recv, sizeof(sc_packet_put_chest));
		if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::CHEST, Chest::Create(m_pGraphic_Device, t))))
			return E_FAIL;
	}
	if (c == SC_PACKET_MOVE_CHEST)
	{
		sc_packet_move_chest t = {};
		memcpy(&t, recv, sizeof(sc_packet_move_chest));
		Chest* p = (Chest*)m_pGameMgr->Get_GameObject(OBJECT::CHEST, t.chest_id);
		if (!p) return E_FAIL;
		if(FAILED(p->Move_Chest(t))) return E_FAIL;
	}

	// ���� ����
	if (c == SC_PACKET_REMOVE_BULLET)
	{
		sc_packet_remove_bullet t = {};
		memcpy(&t, recv, sizeof(sc_packet_remove_bullet));
		cout << "Delete " << t.bullet_id << endl;
		if (FAILED(m_pGameMgr->Delete_GameObject(OBJECT::BULLET, t.bullet_id)))
			return E_FAIL;
	}
	if (c == SC_PACKET_REMOVE_CHEST)
	{
		sc_packet_remove_chest t = {};
		memcpy(&t, recv, sizeof(sc_packet_remove_chest));
		if (FAILED(m_pGameMgr->Delete_GameObject(OBJECT::CHEST, t.chest_id)))
			return E_FAIL;
	}
	if (c == SC_PACKET_CHANGE_WEAPON)
	{
		sc_packet_change_weapon t = {};
		memcpy(&t, recv, sizeof(sc_packet_change_weapon));
		Player* p = (Player*)m_pGameMgr->Get_GameObject(OBJECT::PLAYER, t.id);
		if (!p) return E_FAIL;
		if(FAILED(p->Change_Weapon(t))) return E_FAIL;
	}
	if (c == SC_PACKET_GAME_END)
	{
		sc_packet_game_end t = {};
		memcpy(&t, recv, sizeof(sc_packet_game_end));
		m_iWinner = t.winner_id;
		m_bSceneChange = true;
	}

	return Scene::Setup_Recv(c, recv);
}

HRESULT Scene_Stage::Setup_Scene(sc_packet_game_state* recv)
{
	for (int i = 0; i < 3; ++i)
	{
		player_info_for_packet t = recv->player[i];
		m_pGameMgr->Get_ClientInfos()[i].startpos = D3DXVECTOR3(t.position.x, t.position.y, 0.f);
	}

	Player* p = nullptr;
	for (auto t : m_pGameMgr->Get_ClientInfos())
	{
		if (t.islocal)
		{
			if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, p = Player::Create(m_pGraphic_Device, t))))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::PLAYER, NetworkPlayer::Create(m_pGraphic_Device, t))))
				return E_FAIL;
		}
	}

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::CAMERA, Camera::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::MAP, MainMap::Create(m_pGraphic_Device, p))))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObject(OBJECT::UI, Mouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

Scene_Stage* Scene_Stage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Scene_Stage* pInstance = new Scene_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create Scene_Title Instance");
		SafeDelete(pInstance);
	}

	return pInstance;
}

void Scene_Stage::Free()
{
	Scene::Free();
}
