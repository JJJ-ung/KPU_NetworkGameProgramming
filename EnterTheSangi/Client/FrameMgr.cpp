#include "framework.h"
#include "FrameMgr.h"

IMPLEMENT_SINGLETON(FrameMgr)

FrameMgr::FrameMgr()
{
}

FrameMgr::~FrameMgr()
{
	Release();
}

FLOAT FrameMgr::UpdateTime()
{
	m_fTimeDelta = m_tTimer.ComputeTime();

	++m_iRenderCall;
	m_fRenderAcc += m_fTimeDelta;

	if (m_fRenderAcc >= 1.f)
	{
		wsprintf(m_szFPS, L"Client | FPS:%d", m_iRenderCall);
		m_iRenderCall = 0;
		m_fRenderAcc = 0.f;
	}
	SetWindowText(g_hWnd, m_szFPS);

	return m_fTimeDelta;
}

BOOL FrameMgr::FrameLimit(FLOAT fLimit)
{
	m_fTimeAcc += m_tFrameTimer.ComputeTime();

	if ((1.f / fLimit) < m_fTimeAcc)
	{
		m_fTimeAcc = 0.f;
		return TRUE;
	}

	return FALSE;
}

VOID FrameMgr::Release()
{
	return VOID();
}

FLOAT tagTimer::ComputeTime()
{
	QueryPerformanceCounter(&CurTime);

	if (CurTime.QuadPart - OldTime.QuadPart > CpuTick.QuadPart)
	{
		QueryPerformanceCounter(&CurTime);
		QueryPerformanceCounter(&OldTime);
		QueryPerformanceFrequency(&CpuTick);
	}

	const FLOAT fResult = static_cast<FLOAT>(CurTime.QuadPart - OldTime.QuadPart) / CpuTick.QuadPart;

	OldTime.QuadPart = CurTime.QuadPart;

	return fResult;
}
