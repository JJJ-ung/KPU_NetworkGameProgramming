#pragma once
#include "GameObject.h"

class Texture;
class Font : public GameObject
{
public:
	explicit Font(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Font();

public:
	string& Get_Line() { return m_strLine; }
	D3DXVECTOR3& Get_Pos() { return m_vPos; }
	D3DXVECTOR4& Get_Color() { return m_vColor; }
	D3DXMATRIX& Get_ViewMatrix() { return m_matView; }

public:
	virtual HRESULT Ready_GameObject(string strName, float fSize, bool align, bool view = false);
	virtual INT Update_GameObject(float TimeDelta);
	virtual INT LateUpdate_GameObject(float TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT Update_Position(D3DXVECTOR3 vPos, D3DXVECTOR3 vOffset = { 0.f, 0.f, 0.f });
	HRESULT Convert_Line(string strLine);

private:
	string m_strLine = "";
	vector<UINT> m_vecIndex;
	Texture* m_pTexture = nullptr;
	D3DXMATRIX m_matView = {};

private:
	const float m_fSpace = 23.f;
	float m_fSize = 1.f;
	bool m_bAlign = true;
	bool m_bViewMat = true;
	D3DXVECTOR3 m_vPos{ 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vAlign{ 0.f, 0.f, 0.f };
	D3DXVECTOR4 m_vColor = { 1.f, 1.f, 1.f, 0.f };

public:
	static Font* Create(LPDIRECT3DDEVICE9 pGraphicDev, string strName, float fSize = 1.f, bool align = true, bool view = false);
	virtual void Free(void);
};

