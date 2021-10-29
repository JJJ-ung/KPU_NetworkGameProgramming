#pragma once

template <typename T>
void SafeDelete(T& pointer)
{
	if (NULL != pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}

template <typename T>
void SafeRelease(T& pointer)
{
	if (NULL != pointer)
	{
		pointer->Release();
		pointer = NULL;
	}
}

template <typename T>
void SafeDelete_Array(T& pointer)
{
	if (NULL != pointer)
	{
		delete[] pointer;
		pointer = NULL;
	}
}

class CStrCmp
{
public:
	CStrCmp() {};
	CStrCmp(const TCHAR* pString)
		:m_pString(pString)
	{}
	~CStrCmp() {};
public:
	template<typename T>
	bool operator()(T& rPair)
	{
		return !lstrcmp(m_pString, rPair.first);
	}
private:
	const TCHAR* m_pString = nullptr;
};

