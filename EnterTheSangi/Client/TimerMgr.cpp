#include "framework.h"
#include "TimerMgr.h"

FLOAT tagTimer::Compute_Time()
{
	QueryPerformanceCounter(&CurTime);

	if (CurTime.QuadPart - OldTime.QuadPart > CpuTick.QuadPart)
	{
		QueryPerformanceCounter(&CurTime);
		QueryPerformanceCounter(&OldTime);
		QueryPerformanceFrequency(&CpuTick);
	}

	FLOAT fResult = FLOAT(CurTime.QuadPart - OldTime.QuadPart) / CpuTick.QuadPart;

	OldTime.QuadPart = CurTime.QuadPart;

	return fResult;
}

//
IMPLEMENT_SINGLETON(TimerMgr)

TimerMgr::TimerMgr()
{
	for(int i = 0; i < END; ++i)
		ZeroMemory(&m_arrTimers[i], sizeof(TIMER));
}

TimerMgr::~TimerMgr()
{
}

bool TimerMgr::Frame_Limit(ID eTimer, float fLimit)
{
	m_arrTimers[eTimer].TimeAcc += m_arrTimers[eTimer].Compute_Time();

	if((1.f / fLimit) < m_arrTimers[eTimer].TimeAcc)
	{
		m_arrTimers[eTimer].TimeAcc = 0.f;
		return TRUE;
	}
	return FALSE;
}

float TimerMgr::Update_Timer(ID eTimer)
{
	float fTimeDelta = m_arrTimers[eTimer].Compute_Time();

	m_arrTimers[eTimer].RenderCall += 1;
	m_arrTimers[eTimer].RenderAcc += fTimeDelta;

	if(m_arrTimers[eTimer].RenderAcc >= 1.f)
	{
		m_iRender[eTimer] = m_arrTimers[eTimer].RenderCall;
		wsprintf(m_szFPS, L"Client %d fps | Server %d fps", m_iRender[CLINET_DEFAULT], m_iRender[NETWORK_DEFAULT]);
		m_arrTimers[eTimer].RenderCall = 0;
		m_arrTimers[eTimer].RenderAcc = 0.f;
	}
	SetWindowText(g_hWnd, m_szFPS);

	return fTimeDelta;
}

void TimerMgr::Print_FrameRate()
{
	SetWindowText(g_hWnd, m_szFPS);
	//cout << "Client" << m_arrTimers[CLIENT].RenderCall << endl;
	//cout << "Network" << m_arrTimers[NETWORKING].RenderCall << endl;
	//cout << "Client D" << m_arrTimers[CLINET_DEFAULT].RenderCall << endl;
	//cout << "Network D" << m_arrTimers[NETWORK_DEFAULT].RenderCall << endl;
	//cout << endl;
}

//
