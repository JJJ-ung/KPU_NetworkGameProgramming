#include "framework.h"
#include "TargetMgr.h"

IMPLEMENT_SINGLETON(TargetMgr)

TargetMgr::TargetMgr()
{
}

TargetMgr::~TargetMgr()
{
	Free();
}

HRESULT TargetMgr::Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pTargetTag, UINT iWidth, UINT iHeight,
	D3DFORMAT eFormat, D3DXCOLOR Color)
{
	Target* pTarget = Find_Target(pTargetTag);

	if (nullptr != pTarget)
		return E_FAIL;

	pTarget = Target::Create(pGraphic_Device, iWidth, iHeight, eFormat, Color);
	if (nullptr == pTarget)
		return E_FAIL;

	m_Targets.insert(TARGETS::value_type(pTargetTag, pTarget));

	return NOERROR;
}

HRESULT TargetMgr::Add_MRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag)
{
	Target* pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;

	list<Target*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<Target*>		MRTList;

		MRTList.push_back(pTarget);

		m_MRTs.insert(MRTS::value_type(pMRTTag, MRTList));
	}
	else
		pMRTList->push_back(pTarget);

	return NOERROR;
}

HRESULT TargetMgr::Begin_MRT(const TCHAR* pMRTTag)
{
	list<Target*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;


	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
			pTarget->Clear_Target();
	}

	UINT	iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		// 디퓨즈 : 백버퍼를 빼고, 디퓨즈를 셋팅하자.
		// 노말 : 널을 빼고, 노멀을 셋팅하ㅏㅈ.
		if (nullptr != pTarget)
			pTarget->SetUp_OnGraphicDev(iIndex++);
	}

	return NOERROR;
}

HRESULT TargetMgr::End_MRT(const TCHAR* pMRTTag)
{
	list<Target*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	UINT	iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		// 디퓨즈 : 백버퍼를 빼고, 디퓨즈를 셋팅하자.
		// 노말 : 널을 빼고, 노멀을 셋팅하ㅏㅈ.
		if (nullptr != pTarget)
			pTarget->Release_OnGraphicDev(iIndex++);
	}

	return NOERROR;
}

HRESULT TargetMgr::SetUp_OnShader(LPD3DXEFFECT pEffect, const TCHAR* pTargetTag, D3DXHANDLE hHandle)
{
	Target* pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->SetUp_OnShader(pEffect, hHandle);;
}

HRESULT TargetMgr::Ready_Debug_Buffer(const TCHAR* pTargetTag, float fX, float fY, float fSizeX, float fSizeY)
{
	Target* pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;


	return pTarget->Ready_Debug_Buffer(fX, fY, fSizeX, fSizeY);
}

HRESULT TargetMgr::Render_Debug_Buffer(const TCHAR* pMRTTag)
{
	if (!m_bRenderDebugBuff)
		return NOERROR;

	list<Target*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pTarget : *pMRTList)
		pTarget->Render_Debug_Buffer();

	return NOERROR;
}

Target* TargetMgr::Find_Target(const TCHAR* pTargetTag)
{
	auto	iter = find_if(m_Targets.begin(), m_Targets.end(), TagFinder(pTargetTag));
	if (iter == m_Targets.end())
		return nullptr;

	return iter->second;
}

list<Target*>* TargetMgr::Find_MRT(const TCHAR* pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), TagFinder(pMRTTag));
	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void TargetMgr::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pTarget : Pair.second)
			SafeDelete(pTarget);
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_Targets)
	{
		SafeDelete(Pair.second);
	}
	m_Targets.clear();
}
