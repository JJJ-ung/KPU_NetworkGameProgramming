#include "framework.h"
#include "Loading.h"

#include "ResourceMgr.h"
#include "GameMgr.h"

#include "Player.h"

Loading::Loading(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev), m_bFinish(false)
{
}

Loading::~Loading()
{
	Free();
}

HRESULT Loading::Ready_Loading()
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	cout << "Loading Ready" << endl;

	return NOERROR;
}

HRESULT Loading::Load_Resources()
{
	ResourceMgr* pResourceMgr = ResourceMgr::GetInstance();

	HRESULT hr = pResourceMgr->Add_TexturesFromFile(L"Player", L"../Binary/Data/Texture/Texture_Player.txt");
	if (FAILED(hr)) return E_FAIL;

	hr = pResourceMgr->Add_TexturesFromFile(L"UI", L"../Binary/Data/Texture/Texture_UI.txt");
	if (FAILED(hr)) return E_FAIL;

	hr = pResourceMgr->Add_TexturesFromFile(L"Weapon", L"../Binary/Data/Texture/Texture_Weapon.txt");
	if (FAILED(hr)) return E_FAIL;

	hr = pResourceMgr->Add_Texture(L"Test", L"Map", L"../Binary/Resources/test0.png", 1);
	if (FAILED(hr)) return E_FAIL;

	return NOERROR;
}

HRESULT Loading::Load_Objects()
{
	return NOERROR;
}

HRESULT Loading::Load_MapInfo()
{
	return NOERROR;
}

unsigned int Loading::Thread_Main(void* pArg)
{
	Loading* pLoading = (Loading*)pArg;

	EnterCriticalSection(pLoading->Get_Crt());

	if (FAILED(pLoading->Load_Resources()))
	{
		cout << "Resource Load Failed" << endl;
		LeaveCriticalSection(pLoading->Get_Crt());
		return -1;
	}
	cout << "Resource Loaded" << endl;

	if (FAILED(pLoading->Load_Objects())) 
	{
		cout << "Object Load Failed" << endl;
		LeaveCriticalSection(pLoading->Get_Crt());
		return -1;
	}
	cout << "Objects Loaded" << endl;

	if (FAILED(pLoading->Load_MapInfo()))
	{
		cout << "Map Load Failed" << endl;
		LeaveCriticalSection(pLoading->Get_Crt());
		return -1;
	}
	cout << "Map Loaded" << endl;

	pLoading->m_bFinish = true;

	LeaveCriticalSection(pLoading->Get_Crt());

	return 0;
}

Loading* Loading::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Loading* pInstance = new Loading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading()))
		SafeDelete(pInstance);

	return pInstance;
}

void Loading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
	cout << "Loading Thread Closed" << endl;
}
