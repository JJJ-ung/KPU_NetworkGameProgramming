#pragma once

typedef struct tagTimer
{
public:
	FLOAT Compute_Time();
public:
	LARGE_INTEGER	CurTime;
	LARGE_INTEGER	OldTime;
	LARGE_INTEGER	CpuTick;
public:
	FLOAT TimeAcc;
	FLOAT RenderAcc;
	INT RenderCall;
}TIMER;

class TimerMgr
{
	DECLARE_SINGLETON(TimerMgr)

private:
	explicit	TimerMgr();
	~TimerMgr();

public:
	enum ID { CLINET_DEFAULT, CLIENT, NETWORK_DEFAULT, NETWORKING, END };

public:
	bool Frame_Limit(ID eTimer, float fLimit);
	float Update_Timer(ID eTimer);
	void Print_FrameRate();

private:
	TIMER m_arrTimers[END];
	TCHAR m_szFPS[MAX_PATH] = L"";
	INT m_iRender[END] = {};
};

