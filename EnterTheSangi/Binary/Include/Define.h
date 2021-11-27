#pragma once

const unsigned short	g_nWinCX = 1280;
const unsigned short	g_nWinCY = 720;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

typedef unsigned char		_ubyte;
typedef signed char			_byte;

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

#define DECLARE_STATE(STATENAME, OWNER)                     \
NO_COPY(STATENAME)                              \
public:                                             \
   static   STATENAME* Instance();                        \

#define IMPLEMENT_STATE(STATENAME)                        \
STATENAME* STATENAME::Instance()                        \
{                                                \
   static STATENAME instance;                           \
   return &instance;                                 \
}

#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif  // _DEBUG  

#include <dxgidebug.h>

#endif
