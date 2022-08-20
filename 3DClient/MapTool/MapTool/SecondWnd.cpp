// SecondWnd.cpp: 구현 파일
//

#include "pch.h"
#include "MapTool.h"
#include "SecondWnd.h"
#include "UnitTool.h"
#include "EffectTool.h"
#include "UITool.h"
#include "NaviTool.h"
#include "CameraTool.h"

IMPLEMENT_DYNCREATE(CSecondWnd, CFormView)

CSecondWnd::CSecondWnd()
	: CFormView(IDD_SECONDWND)
{

}

CSecondWnd::~CSecondWnd()
{
	Safe_Delete(m_pUnitTool);
	Safe_Delete(m_pEffectTool);
	Safe_Delete(m_pUITool);
	Safe_Delete(m_pNaviTool);
	Safe_Delete(m_pCameraTool);

}

void CSecondWnd::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CSecondWnd, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSecondWnd::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CSecondWnd 진단

#ifdef _DEBUG
void CSecondWnd::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSecondWnd::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSecondWnd 메시지 처리기


void CSecondWnd::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_Tab.InsertItem(0, L"UnitTool");
	m_Tab.InsertItem(1, L"EffectTool");
	m_Tab.InsertItem(2, L"UITool");
	m_Tab.InsertItem(3, L"NaviTool");
	m_Tab.InsertItem(4, L"CameraTool");
	m_Tab.SetCurSel(0);

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_pUnitTool = new CUnitTool;
	m_pUnitTool->Create(IDD_UnitTool, &m_Tab);
	m_pUnitTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pUnitTool->ShowWindow(SW_SHOW);

	m_pEffectTool = new CEffectTool;
	m_pEffectTool->Create(IDD_EffectTool, &m_Tab);
	m_pEffectTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pEffectTool->ShowWindow(SW_HIDE);
	
	m_pUITool = new CUITool;
	m_pUITool->Create(IDD_UITool, &m_Tab);
	m_pUITool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pUITool->ShowWindow(SW_HIDE);


	m_pNaviTool = new CNaviTool;
	m_pNaviTool->Create(IDD_NaviTool, &m_Tab);
	m_pNaviTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pNaviTool->ShowWindow(SW_HIDE);

	m_pCameraTool = new CCameraTool;
	m_pCameraTool->Create(IDD_CameraTool, &m_Tab);
	m_pCameraTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pCameraTool->ShowWindow(SW_HIDE);

	eMode = TOOL_MODE::MODE_UNIT;
}


void CSecondWnd::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	switch (m_Tab.GetCurSel())
	{
	case 0:
		m_pUnitTool->ShowWindow(SW_SHOW);
		m_pEffectTool->ShowWindow(SW_HIDE);
		m_pUITool->ShowWindow(SW_HIDE);
		m_pNaviTool->ShowWindow(SW_HIDE);
		m_pCameraTool->ShowWindow(SW_HIDE);
		eMode = TOOL_MODE::MODE_UNIT;
		break;
	case 1:
		m_pUnitTool->ShowWindow(SW_HIDE);
		m_pEffectTool->ShowWindow(SW_SHOW);
		m_pUITool->ShowWindow(SW_HIDE);
		m_pNaviTool->ShowWindow(SW_HIDE);
		m_pCameraTool->ShowWindow(SW_HIDE);
		eMode = TOOL_MODE::MODE_EFFECT;
		break;
	case 2:
		m_pUnitTool->ShowWindow(SW_HIDE);
		m_pEffectTool->ShowWindow(SW_HIDE);
		m_pUITool->ShowWindow(SW_SHOW);
		m_pNaviTool->ShowWindow(SW_HIDE);
		m_pCameraTool->ShowWindow(SW_HIDE);
		eMode = TOOL_MODE::MODE_UI;
		break;
	case 3:
		m_pUnitTool->ShowWindow(SW_HIDE);
		m_pEffectTool->ShowWindow(SW_HIDE);
		m_pUITool->ShowWindow(SW_HIDE);
		m_pNaviTool->ShowWindow(SW_SHOW);
		m_pCameraTool->ShowWindow(SW_HIDE);
		eMode = TOOL_MODE::MODE_NAVIMESH;
		break;
	case 4:
		m_pUnitTool->ShowWindow(SW_HIDE);
		m_pEffectTool->ShowWindow(SW_HIDE);
		m_pUITool->ShowWindow(SW_HIDE);
		m_pNaviTool->ShowWindow(SW_HIDE);
		m_pCameraTool->ShowWindow(SW_SHOW);
		eMode = TOOL_MODE::MODE_CAMERA;
		break;
	}
	*pResult = 0;
}
