// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <time.h>
#include <atlstr.h>

#pragma warning(push)
#pragma warning(disable	: 4267)
#pragma warning(disable	: 4244)
#pragma warning(disable	: 4099)
#pragma warning(disable : 4251)
#pragma warning(disable : 4996)



#include "d3dx9.h"
#include "d3d9.h"

#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"

#include <list>
#include <vector>
#include <map>
#include <algorithm>


// For.CRTDBG
#ifdef _DEBUG 
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h> 

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG  
using namespace std;


#include "Define.h"
#include "Funtion.h"
#include "Funtor.h"
#include "Macro.h"
#include "Enum.h"
#include "Struct.h"
#include "Typedef.h"
#include "Constant.h"
#include "Extern.h"
