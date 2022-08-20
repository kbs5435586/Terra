
// Tool.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//


#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Tool.h"
#include "MainFrm.h"

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainApp.h"
#include "System.h"
#include "SecondWnd.h"
#include "UnitTool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolApp

BEGIN_MESSAGE_MAP(CToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CToolApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CToolApp ����

CToolApp::CToolApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Tool.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CToolApp ��ü�Դϴ�.

CToolApp theApp;
HINSTANCE g_hInst;
HWND g_hWnd;
bool	IsMouseCheck=false;
bool	m_IsCheckListBox = false;
bool	IsEditMode = true;
TOOL_MODE	eMode=MODE_END;
_tchar*		m_pEdit_Mode;
_tchar*		m_gLayerTag = L"";
_bool		m_IsMouseClicked = false;
_bool		IsWireFrame = false;
_bool		IsPlay = false;
// CToolApp �ʱ�

BOOL CToolApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


								// ���� ���α׷��� ���� ���ø��� ����մϴ�.  ���� ���ø���
								//  ����, ������ â �� �� ������ ���� ������ �մϴ�.

	g_hInst = m_hInstance;

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CToolDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CToolApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CToolApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

														// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


int CToolApp::Run()
{
	pMainApp = CMainApp::Create();

	if (nullptr == pMainApp)
		return FALSE;

	CSystem*			pSystem = CSystem::GetInstance();
	if (nullptr == pSystem)
		return FALSE;

	pSystem->AddRef();

	// Timer�� �߰��Ѵ�.
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

	// �⺻ �޽��� �����Դϴ�.

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
		if (nullptr == pMainApp)
			return FALSE;

		_float		fTimeDelta_Default = pSystem->Get_TimeDelta(L"Timer_Default");

		if (true == pSystem->Permit_Call(L"Frame_60", fTimeDelta_Default))
		{
			_float		fTimeDelta_60 = pSystem->Get_TimeDelta(L"Timer_60");
			pMainApp->Update_MainApp(fTimeDelta_60);
			pMainApp->Render_MainApp();
		}
	}

	_ulong		dwRefCnt = 0;

	Safe_Release(pSystem);

	if (dwRefCnt = Safe_Release(pMainApp))
		_MSG_BOX("CMainApp Release Failed");

	return CWinAppEx::Run();
}


BOOL CToolApp::OnIdle(LONG lCount)
{


	return CWinAppEx::OnIdle(lCount);
}
