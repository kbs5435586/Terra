#pragma once
const unsigned int g_iBackCX = 1920;
const unsigned int g_iBackCY = 1200;
#define		MAX_STR		256

#define NO_COPY(CLASSNAME)									\
	private:												\
	CLASSNAME(const CLASSNAME&);							\
	CLASSNAME& operator = (const CLASSNAME&);				

#define _DECLARE_SINGLETON(CLASSNAME)						\
	NO_COPY(CLASSNAME)										\
	private:												\
	static CLASSNAME*	m_pInstance;						\
	public:													\
	static CLASSNAME*	GetInstance( void );				\
	static unsigned long DestroyInstance( void );					

#define _IMPLEMENT_SINGLETON(CLASSNAME)						\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;				\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
		if(NULL == m_pInstance) {							\
			m_pInstance = new CLASSNAME;					\
		}													\
		return m_pInstance;									\
	}														\
	unsigned long CLASSNAME::DestroyInstance( void ) {		\
		unsigned long dwRefCnt = 0;							\
		if(NULL != m_pInstance)	{							\
			dwRefCnt = m_pInstance->Release();				\
			if(0 == dwRefCnt) m_pInstance = NULL;			\
		}													\
		return dwRefCnt;									\
	}

#define _MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("Error"), MB_OK);


const DWORD KEY_LBUTTON = 0x00000001;		// 0000 0001
const DWORD KEY_RBUTTON = 0x00000002;		// 0000 0010
const DWORD KEY_RETURN = 0x00000004;		// 0000 0100
const DWORD KEY_SPACE = 0x00000008;		// 0000 1000
const DWORD KEY_SHIFT = 0x00000010;		// 0001 0000
const DWORD KEY_UP = 0x00000020;		// 0010 0000
const DWORD KEY_DOWN = 0x00000040;		// 0100 0000
const DWORD KEY_LEFT = 0x00000080;		// 1000 0000
const DWORD KEY_RIGHT = 0x00000100;		// 0001 0000 0000

const DWORD KEY_F1 = 0x00000200;
const DWORD KEY_F2 = 0x00000400;
const DWORD KEY_F3 = 0x00000800;

const DWORD KEY_U = 0x00001000;
