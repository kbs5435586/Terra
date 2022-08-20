// PathFind.cpp: 구현 파일
//

#include "pch.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "PathFind.h"


// CPathFind 대화 상자

IMPLEMENT_DYNAMIC(CPathFind, CFormView)

CPathFind::CPathFind(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PathFind)
{

}

CPathFind::~CPathFind()
{
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialogEx)
END_MESSAGE_MAP()


// CPathFind 메시지 처리기



BOOL CPathFind::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
