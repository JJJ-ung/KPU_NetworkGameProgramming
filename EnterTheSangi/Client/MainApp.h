#pragma once

class Renderer;
class GameMgr;
class ShaderMgr;
class ResourceMgr;
class MainApp
{
public:
	explicit MainApp();
	~MainApp();

public:
	HRESULT Ready_MainApp();
	int Update_MainApp(const float TimeDelta);
	HRESULT Render_MainApp();

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

private:
	Renderer* m_pRenderer = nullptr;
	GameMgr* m_pGameMgr = nullptr;
	ShaderMgr* m_pShaderMgr = nullptr;
	ResourceMgr* m_pResourceMgr = nullptr;

public:
	static MainApp* Create();
	virtual void Free();
};

