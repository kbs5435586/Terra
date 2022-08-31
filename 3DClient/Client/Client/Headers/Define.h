#pragma once

const unsigned int g_iBackCX = 1920;
const unsigned int g_iBackCY = 1080;

#define		MAX_STR		256

#define		DEAD_OBJ	1
#define		NO_EVENT	0

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif