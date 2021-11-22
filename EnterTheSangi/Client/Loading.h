#pragma once

class Loading
{
public:
	explicit Loading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Loading();

public:
	CRITICAL_SECTION* Get_Crt(void) { return &m_Crt; }
	BOOL								Get_Finish(void) const { return m_bFinish; }

public:
	HRESULT Ready_Loading();

public:
	HRESULT Load_Resources();
	HRESULT Load_Objects();
	HRESULT Load_MapInfo();

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

private:
	HANDLE								m_hThread;
	CRITICAL_SECTION			m_Crt;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	BOOL									m_bFinish;

public:
	static Loading* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();
};

