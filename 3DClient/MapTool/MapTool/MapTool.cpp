
// MapTool.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MapTool.h"
#include "MainFrm.h"

#include "MapToolDoc.h"
#include "MapToolView.h"


#include "MainApp.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolApp

BEGIN_MESSAGE_MAP(CMapToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMapToolApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CMapToolApp 생성

CMapToolApp::CMapToolApp() noexcept
{

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 애플리케이션을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MapTool.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CMapToolApp 개체입니다.

CMapToolApp theApp;
HINSTANCE	g_hInst;
HWND		g_hWnd;
HWND		g_MinihWnd;
_bool		IsMouse = false;
_bool		IsDeffered = false;
_bool		IsListBox_StaticMesh = false;
_bool		IsListBox_DynamicMesh = false;
_bool		IsCreative = false;
_bool		IsRotation_EffectMesh = false;
_bool		IsCameraMove = false;
_bool		IsDrag = false;
_float		fCameraSpeed = 10.f;
_float		fEffectMeshX = 0.f;
_float		fEffectMeshY = 0.f;
_float		fEffectMeshZ= 0.f;
_float		g_TimeDelta = 0.f;
int			TotlaPointIdx = 0;
_uint		g_iCameraMoveIdx = 0;
int			g_iEffectRotateNum = 0;
NAVI_MODE	g_eNaviMode = NAVI_MODE::NAVI_MODE_END;

TOOL_MODE	eMode = TOOL_MODE::MODE_END;
NAVI		eNavi = NAVI::NAVI_END;
// CMapToolApp 초기화


BOOL CMapToolApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	g_hInst = m_hInstance;
	// 애플리케이션의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMapToolDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CMapToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CMapToolApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CMapToolApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStop();
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON6, &CAboutDlg::OnBnClickedStop)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CMapToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMapToolApp 메시지 처리기





BOOL CMapToolApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinApp::OnIdle(lCount);
}


int CMapToolApp::Run()
{
	m_pMainApp = CMainApp::Create();

	if (nullptr == m_pMainApp)
		return FALSE;

	CSystem* pSystem = CSystem::GetInstance();
	if (nullptr == pSystem)
		return FALSE;

	pSystem->AddRef();

	// Timer를 추가한다.
	// For.Timer_Default
	if (FAILED(pSystem->Add_Timer(L"Timer_Default")))
		return FALSE;

	// For.Timer_60
	if (FAILED(pSystem->Add_Timer(L"Timer_60")))
		return FALSE;

	// For.Frame_60
	if (FAILED(pSystem->Add_Frame(L"Frame_60", 60.0f)))
		return FALSE;

	MSG msg;

	// 기본 메시지 루프입니다.

	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (WM_QUIT == msg.message || WM_CLOSE == msg.message)
				break;

			if (!PumpMessage())
			{
				::PostQuitMessage(WM_CLOSE);
				break;
			}

		}
		if (nullptr == m_pMainApp)
			return FALSE;

		_float		fTimeDelta_Default = pSystem->Get_TimeDelta(L"Timer_Default");

		if (true == pSystem->Permit_Call(L"Frame_60", fTimeDelta_Default))
		{
			_float		fTimeDelta_60 = pSystem->Get_TimeDelta(L"Timer_60");
			g_TimeDelta = fTimeDelta_60;
			m_pMainApp->Update_MainApp(fTimeDelta_60);
			m_pMainApp->Render_MainApp();
		}
	}

	_ulong		dwRefCnt = 0;

	Safe_Release(pSystem);

	if (dwRefCnt = Safe_Release(m_pMainApp))
		_MSG_BOX("CMainApp Release Failed");
	return CWinApp::Run();
}



void CAboutDlg::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
