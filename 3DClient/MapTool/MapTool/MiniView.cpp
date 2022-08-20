// MiniView.cpp: 구현 파일
//

#include "pch.h"
#include "MapTool.h"
#include "MiniView.h"
#include "Management.h"
#include "Layer.h"
#include "GameObject.h"
#include "MainFrm.h"
#include "MapToolView.h"
#include "Device.h"
// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

void CMiniView::DoDataExchange(CDataExchange* pDX)
{
	CView::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView 진단

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기


void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	g_MinihWnd = m_hWnd;

	RECT rcMainFrm = {};
	this->GetWindowRect(&rcMainFrm);
	SetRect(&rcMainFrm, 0, 0,
		rcMainFrm.right - rcMainFrm.left, rcMainFrm.bottom - rcMainFrm.top);

	RECT rcView = {};
	GetClientRect(&rcView);
	int iRowFrm = rcMainFrm.right - rcView.right;
	int iColFrm = rcMainFrm.bottom - rcView.bottom;

	this->SetWindowPos(nullptr, rcMainFrm.left, rcMainFrm.top+g_iBackCY, rcMainFrm.right, rcMainFrm.bottom + g_iBackCY, SWP_NOZORDER);
}


void CMiniView::OnDraw(CDC* /*pDC*/)
{


}
