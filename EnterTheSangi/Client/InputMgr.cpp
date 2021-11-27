#include "framework.h"
#include "InputMgr.h"

IMPLEMENT_SINGLETON(InputMgr)

InputMgr::InputMgr()
{
}

InputMgr::~InputMgr()
{
	Free();
}

BOOL InputMgr::KeyDown(DWORD dwCurKey)
{
	if (!(m_dwKeyDown & dwCurKey) && (m_dwCurKey & dwCurKey))
	{
		m_dwKeyDown |= dwCurKey;
		return true;
	}

	else if ((m_dwKeyDown & dwCurKey) && !(m_dwCurKey & dwCurKey))
	{
		m_dwKeyDown ^= dwCurKey;
		return false;
	}

	return false;
}

BOOL InputMgr::KeyUp(DWORD dwCurKey)
{
	if ((m_dwKeyUp & dwCurKey) && !(m_dwCurKey & dwCurKey))
	{
		m_dwKeyUp ^= dwCurKey;
		return true;
	}

	else if (!(m_dwKeyUp & dwCurKey) && (m_dwCurKey & dwCurKey))
	{
		m_dwKeyUp |= dwCurKey;
		return false;
	}

	return false;
}

BOOL InputMgr::KeyPressing(DWORD dwCurKey)
{
	if (m_dwCurKey & dwCurKey)
		return true;

	return false;
}

BOOL InputMgr::KeyCombined(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if (KeyDown(dwSecondKey) && KeyPressing(dwFirstKey))
		return true;

	return false;
}

HRESULT InputMgr::Init_InputDev()
{
	if (FAILED(DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	m_pKeyBoard->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pKeyBoard->Acquire();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);

	m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pMouse->Acquire();

	m_vMousePoint.z = 0.f;

	return NOERROR;
}

void InputMgr::Update_Key()
{
	m_dwCurKey = 0;

	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	GetCursorPos(&m_tMousePoint);
	ScreenToClient(g_hWnd, &m_tMousePoint);

	m_tMousePoint.x += LONG(1280.f * -0.5f);
	m_tMousePoint.y += LONG(720.f * -0.5f);

	m_vMousePoint.x = (float)m_tMousePoint.x;
	m_vMousePoint.y = (float)m_tMousePoint.y;

	m_fAngle = D3DXToDegree(atan2(m_tMousePoint.x, m_tMousePoint.y));

	if (GetKeyState(DIK_W) & 0x80)
		m_dwCurKey |= KEY_W;
	if (GetKeyState(DIK_S) & 0x80)
		m_dwCurKey |= KEY_S;
	if (GetKeyState(DIK_A) & 0x80)
		m_dwCurKey |= KEY_A;
	if (GetKeyState(DIK_D) & 0x80)
		m_dwCurKey |= KEY_D;
	if (GetKeyState(DIK_Q) & 0x80)
		m_dwCurKey |= KEY_Q;
	if (GetKeyState(DIK_E) & 0x80)
		m_dwCurKey |= KEY_E;
	if (GetKeyState(DIK_LSHIFT))
		m_dwCurKey |= KEY_SHIFT;
	if (GetKeyState(DIK_LCONTROL))
		m_dwCurKey |= KEY_CTRL;
	if (GetKeyState(DIK_SPACE))
		m_dwCurKey |= KEY_SPACE;

	if (GetMouseState(DIM_LB) & 0x80)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetMouseState(DIM_RB) & 0x80)
		m_dwCurKey |= KEY_RBUTTON;

	if (GetKeyState(DIK_RETURN) & 0x80)
		m_dwCurKey |= KEY_ENTER;

	if (GetKeyState(DIK_F1) & 0x80)
		m_dwCurKey |= KEY_F1;
	if (GetKeyState(DIK_F2) & 0x80)
		m_dwCurKey |= KEY_F2;
	if (GetKeyState(DIK_F3) & 0x80)
		m_dwCurKey |= KEY_F3;
}

void InputMgr::Update_Text(WPARAM message)
{
	if (!m_bTextMode) return;

	if (m_strText.length() == 16)
		return;

	if (message == VK_BACK)
	{
		if(!m_strText.empty())
			m_strText.pop_back();
	}

	if (message > 64 && message < 91)
		m_strText += (char)message;
}

void InputMgr::Free()
{
	m_pKeyBoard->Release();
	m_pMouse->Release();
	m_pInputSDK->Release();
}
