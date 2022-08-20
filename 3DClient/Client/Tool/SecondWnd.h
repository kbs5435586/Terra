#pragma once
#include "afxcmn.h"



// CSecondWnd �� ���Դϴ�.
class CUnitTool;
class CEffectTool;
class CNaviMesh;
class CUI;
class CAnimationTool;
class CTerrainTool;
class CSecondWnd : public CFormView
{
	DECLARE_DYNCREATE(CSecondWnd)

protected:
	CSecondWnd();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CSecondWnd();
public:
	CUnitTool* m_pUnitTool;
	CEffectTool* m_pEffectTool;
	CUI*		m_pUI;
	CNaviMesh*	m_pNaviMesh;
	CAnimationTool*	m_pAnimationTool;
	CTerrainTool*	m_pTerrainTool;
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_Tab;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


