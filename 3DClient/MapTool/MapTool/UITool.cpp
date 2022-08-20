// UITool.cpp: 구현 파일
//

#include "pch.h"
#include "MapTool.h"
#include "UITool.h"


// CUITool

IMPLEMENT_DYNCREATE(CUITool, CFormView)

CUITool::CUITool()
	: CDialogEx(IDD_UITool)
{

}

CUITool::~CUITool()
{
}

void CUITool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUITool, CDialogEx)
END_MESSAGE_MAP()


// CUITool 진단

#ifdef _DEBUG
void CUITool::AssertValid() const
{
	CDialogEx::AssertValid();
}

#ifndef _WIN32_WCE
void CUITool::Dump(CDumpContext& dc) const
{
	CDialogEx::Dump(dc);
}
#endif
#endif //_DEBUG


// CUITool 메시지 처리기


BOOL CUITool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


