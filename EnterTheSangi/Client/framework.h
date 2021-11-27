#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// For DirectX
#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

// For Winsock
#include <winsock2.h>
#include "Protocol.h"
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <process.h>
#include <chrono>

using namespace std;
using namespace chrono;
//
//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#ifndef DBG_NEW 
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//#endif  // _DEBUG  
//
//#include <dxgidebug.h>
//
//#endif

#include "Define.h"
#include "Enum.h"
#include "Function.h"
#include "Struct.h"

#pragma warning(disable: 26495)

#ifdef _DEBUG
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
#endif

//#define TEST
