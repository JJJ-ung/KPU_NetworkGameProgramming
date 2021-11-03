// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

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

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

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

#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")