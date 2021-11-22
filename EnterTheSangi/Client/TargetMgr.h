#pragma once
#include "Target.h"

class TargetMgr
{
	DECLARE_SINGLETON(TargetMgr)

public:
	explicit TargetMgr();
	virtual ~TargetMgr();

public:
	HRESULT Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pTargetTag, UINT iWidth, UINT iHeight, D3DFORMAT eFormat, D3DXCOLOR Color);
	HRESULT Add_MRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag);
	HRESULT Begin_MRT(const TCHAR* pMRTTag);
	HRESULT End_MRT(const TCHAR* pMRTTag);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const TCHAR* pTargetTag, D3DXHANDLE hHandle);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(const TCHAR* pTargetTag, float fX, float fY, float fSizeX, float fSizeY);
	HRESULT Render_Debug_Buffer(const TCHAR* pMRTTag);
#endif

private:
	unordered_map<const TCHAR*, Target*>			m_Targets;
	typedef unordered_map<const TCHAR*, Target*>	TARGETS;

private:
	unordered_map<const TCHAR*, list<Target*>>			m_MRTs;
	typedef unordered_map<const TCHAR*, list<Target*>>	MRTS;

private:
	Target* Find_Target(const TCHAR* pTargetTag);
	list<Target*>* Find_MRT(const TCHAR* pMRTTag);

public:
	virtual void Free();
};

