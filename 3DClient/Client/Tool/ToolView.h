
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

class CToolDoc;
class CMainFrame;
class CSecondWnd;
class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	void Mode_Unit(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd);
	void Mode_Effect(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd);
	void Mode_UI(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd);
	void Mode_NaviMesh(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd);
public:
	void Mode_Unit_Drag(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd);
	void Mode_Effect_Drag(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd);
	void Mode_UI_Drag(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd);
	void Mode_NaviMesh_Drag(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd);
public:
	_int			m_iNaviMeshCnt = 0;
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

