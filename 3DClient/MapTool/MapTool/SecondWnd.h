#pragma once



// CSecondWnd 폼 보기
class CUnitTool;
class CEffectTool;
class CUITool;
class CNaviTool;
class CCameraTool;

class CSecondWnd : public CFormView
{
	DECLARE_DYNCREATE(CSecondWnd)

protected:
	CSecondWnd();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSecondWnd();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECONDWND };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
public:
	CTabCtrl m_Tab;
public:
	CUnitTool*		m_pUnitTool = nullptr;
	CEffectTool*	m_pEffectTool = nullptr;
	CUITool*		m_pUITool = nullptr;
	CNaviTool*		m_pNaviTool	= nullptr;
	CCameraTool*	m_pCameraTool = nullptr;
};



