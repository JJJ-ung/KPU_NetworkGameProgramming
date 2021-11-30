#pragma once
#define MAX_VKEY 0xff

const DWORD KEY_W = 0x00000001;
const DWORD KEY_S = 0x00000002;
const DWORD KEY_A = 0x00000004;
const DWORD KEY_D = 0x00000008;

const DWORD KEY_SPACE = 0x00000010;
const DWORD KEY_SHIFT = 0x00000020;
const DWORD KEY_CTRL = 0x00000040;

const DWORD KEY_LBUTTON = 0x00000080;
const DWORD KEY_RBUTTON = 0x00000100;

const DWORD KEY_ENTER = 0x00000200;

const DWORD KEY_F1 = 0x00000400;
const DWORD KEY_F2 = 0x00000800;
const DWORD KEY_F3 = 0x00001000;

const DWORD KEY_Q = 0x00002000;
const DWORD KEY_E = 0x00004000;

class InputMgr
{
	DECLARE_SINGLETON(InputMgr)

public:
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_WHEEL, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_WHEEL, DIMS_END };

public:
	bool& Get_TextMode() { return m_bTextMode; }
	string& Get_Text() { return m_strText; }
	float& Get_Angle() { return m_fAngle; }
	const float Get_Angle_Abs() { return abs(m_fAngle); }
	const float Get_Angle_Radian() { return D3DXToRadian(m_fAngle); }
	D3DXVECTOR3& Get_MousePoint() { return m_vMousePoint; }

private:
	InputMgr();
	~InputMgr();

public:
	_byte	GetKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte	GetMouseState(MOUSEKEYSTATE eMouse) { return m_tMouseState.rgbButtons[eMouse]; }
	LONG	GetMouseMove(MOUSEMOVESTATE eMouseState) { return *(((LONG*)&m_tMouseState) + eMouseState); }

public:
	BOOL KeyDown(DWORD dwCurKey);
	BOOL KeyUp(DWORD dwCurKey);
	BOOL KeyPressing(DWORD dwCurKey);
	BOOL KeyCombined(DWORD dwFirstKey, DWORD dwSecondKey);

public:
	HRESULT Init_InputDev();
	void	 Update_Key();
	void Update_Text(WPARAM message);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte								m_byKeyState[256];
	DIMOUSESTATE			m_tMouseState;

private:
	bool									m_bTextMode = false;
	string								m_strText = "";

private:
	float								m_fAngle = 0.f;
	POINT								m_tMousePoint = {};
	D3DXVECTOR3				m_vMousePoint = {};

public:
	ULONG m_dwCurKey = 0;
	ULONG m_dwKeyUp = 0;
	ULONG m_dwKeyDown = 0;

private:
	void Free();
};

