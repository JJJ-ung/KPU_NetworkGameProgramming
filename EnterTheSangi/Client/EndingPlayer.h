#pragma once
#include "GameObject.h"

class Font;
class Animation;
class EndingPlayer : public GameObject
{
public:
	explicit EndingPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~EndingPlayer();

public:
	virtual HRESULT Ready_GameObject(int iID, bool bWin);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CLIENT m_tInfo = {};
	Font* m_pFont = nullptr;
	D3DXMATRIX m_matView;
	Animation* m_pAnimation = nullptr;

public:
	static EndingPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device, int iID, bool bWin);
	virtual void Free();
};

