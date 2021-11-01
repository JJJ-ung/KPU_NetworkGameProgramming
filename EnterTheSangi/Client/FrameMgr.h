#pragma once

typedef struct tagTimer
{
public:
	FLOAT ComputeTime();
	LARGE_INTEGER	CurTime;
	LARGE_INTEGER	OldTime;
	LARGE_INTEGER	CpuTick;
}TIMER;

class FrameMgr
{
	DECLARE_SINGLETON(FrameMgr)

private:
	FrameMgr();
	~FrameMgr();

public:
	FLOAT			UpdateTime();
	BOOL				FrameLimit(FLOAT fLimit);

private:
	TIMER				m_tTimer = {};
	TIMER				m_tFrameTimer = {};

private:
	FLOAT			m_fTimeDelta = 0.f;
	FLOAT			m_fTimeAcc = 0.f;
	FLOAT			m_fRenderAcc = 0.f;
	INT					m_iRenderCall = 0;
	TCHAR			m_szFPS[MAX_PATH] = L"";

private:
	VOID				Release();
};

