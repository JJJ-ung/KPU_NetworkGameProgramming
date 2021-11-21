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

	vEye = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, -0.1f);
	vAt = D3DXVECTOR3(1280.f * -0.5f, 720.f * -0.5f, 0.f);
	vUP = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUP);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
	m_matView *= matScale;

	return NOERROR;
}

INT Camera::Update_GameObject(float TimeDelta)
{
	if (GetAsyncKeyState(VK_RIGHT))
	{
		vEye += D3DXVECTOR3(1.f, 0.f, 0.f);
		vAt += D3DXVECTOR3(1.f, 0.f, 0.f);
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		vEye += D3DXVECTOR3(-1.f, 0.f, 0.f);
		vAt += D3DXVECTOR3(-1.f, 0.f, 0.f);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		vEye += D3DXVECTOR3(0.f, 1.f, 0.f);
		vAt += D3DXVECTOR3(0.f, 1.f, 0.f);
	}
	if (GetAsyncKeyState(VK_UP))
	{
		vEye += D3DXVECTOR3(0.f, -1.f, 0.f);
		vAt += D3DXVECTOR3(0.f, -1.f, 0.f);
	}

	return 0;
}

INT Camera::LateUpdate_GameObject(float TimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUP);
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
