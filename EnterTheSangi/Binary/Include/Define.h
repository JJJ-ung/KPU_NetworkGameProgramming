#pragma once

const unsigned short	g_nWinCX = 1280;
const unsigned short	g_nWinCY = 720;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

class GameObj;
typedef list<GameObj*>			OBJLIST;
typedef OBJLIST::iterator			OBJITER;

#define  NOEVENT 0
#define  OBJDEAD 1 

#define NO_COPY(ClassName)						\
private:										\
	ClassName(const ClassName& Obj);			\
	ClassName& operator=(const ClassName& Obj);

#define DECLARE_SINGLETON(ClassName)		\
		NO_COPY(ClassName)					\
public:										\
	static ClassName* GetInstance()			\
	{										\
		if(nullptr == m_pInstance)			\
		{									\
			m_pInstance = new ClassName;	\
		}									\
		return m_pInstance;					\
	}										\
	void DestroyInstance()					\
	{										\
		if(m_pInstance)						\
		{									\
			delete m_pInstance;				\
			m_pInstance = nullptr;			\
		}									\
	}										\
private:									\
	static ClassName*	m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)		\
ClassName* ClassName::m_pInstance = nullptr;

#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK)
