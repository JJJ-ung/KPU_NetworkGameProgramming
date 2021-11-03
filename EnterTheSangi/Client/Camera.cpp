#include "framework.h"
#include "Camera.h"

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
	D3DXMatrixIdentity(&m_matProj);

	D3DXMatrixOrthoLH(&m_matProj, 1280.f / 720.f, 1.f, 0.f, 1000.f);

	D3DXVECTOR3 vEye = D3DXVECTOR3(0.f, 0.f, -1.f);
	D3DXVECTOR3 vAt = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vUP = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUP);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.15f, 0.15f, 0.15f);
	m_matView *= matScale;

    return NOERROR;
}

INT Camera::Update_GameObject(float TimeDelta)
{


    return 0;
}

INT Camera::LateUpdate_GameObject(float TimeDelta)
{
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

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
