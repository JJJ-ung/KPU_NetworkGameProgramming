#include "framework.h"
#include "Animation.h"

#include "Frame.h"
#include "Texture.h"
#include "ResourceMgr.h"

Animation::Animation(LPDIRECT3DDEVICE9 pGraphic_Device)
	:Component(pGraphic_Device)
{
}

Animation::~Animation()
{
}

HRESULT Animation::Ready_Component(const TCHAR* pPath, OBJECT::TYPE eType)
{
	m_pResourceMgr = ResourceMgr::GetInstance();

	wifstream fin;

	fin.open(pPath);
	if (fin.fail()) return E_FAIL;

	wstring tag;
	float speed, start;
	while (true)
	{
		fin >> tag >> speed >> start;

		if (fin.eof()) break;

		Texture* pTex = nullptr;
		pTex = m_pResourceMgr->Find_Texture(eType, tag.c_str());
		if (!pTex)
			return E_FAIL;

		Frame* pFrame = Frame::Create(m_pGraphic_Device, pTex, speed, start);
		if (!pFrame)
			return E_FAIL;

		m_mapFrame.insert(MAPFRAME::value_type(tag, pFrame));

		m_pCurrFrame = pFrame;
	}

	return NOERROR;
}

INT Animation::Update_Component(float time_delta)
{
	return Component::Update_Component(time_delta);
}

INT Animation::LateUpdate_Component(float time_delta)
{
	return Component::LateUpdate_Component(time_delta);
}

Animation* Animation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const TCHAR* pPath, OBJECT::TYPE eType)
{
	return nullptr;
}

void Animation::Free()
{
	Component::Free();
}
