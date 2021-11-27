#include "framework.h"
#include "Camera.h"
#include "InputMgr.h"

Camera::Camera(LPDIRECT3DDEVICE9 pGraphic_Device)
	:GameObject(pGraphic_Device)
{
}

Camera::~Camera()
{
}

HRESULT Camera::Ready_GameObject()
{
	D3DXMatrixIdentity(&m_matView);

	m_vAt = m_vAspect;
	m_vEye = m_vAt + m_vAspectEye;
	m_vUP = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUP);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
	m_matView *= matScale;

	m_pGameMgr = GameMgr::GetInstance();
	if (!m_pGameMgr) return E_FAIL;

	m_pInputMgr = InputMgr::GetInstance();
	if (!m_pInputMgr) return E_FAIL;

	return NOERROR;
}

INT Camera::Update_GameObject(float TimeDelta)
{
	m_vAt = m_pGameMgr->Get_PlayerPos()+ m_vAspect;
	m_vAt += m_pInputMgr->Get_MousePoint() * 0.2f;
	m_vEye = m_vAt + m_vAspectEye;

	return 0;
}

INT Camera::LateUpdate_GameObject(float TimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUP);
	m_pDevice->SetTransform(D3DTS_TEXTURE0, &m_matView);

	return 0;
}

HRESULT Camera::Render_GameObject()
{
	return NOERROR;
}

Camera* Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Camera* pInstance = new Camera(pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject()))
		SafeDelete(pInstance);
	return pInstance;
}

void Camera::Free()
{
}
