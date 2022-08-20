#pragma once
#include "afxdialogex.h"


// CNaviTool 대화 상자

class CNaviTool : public CDialogEx
{
	DECLARE_DYNAMIC(CNaviTool)

public:
	CNaviTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CNaviTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NaviTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void	OnBnClickedSave();
	afx_msg void	OnBnClickedLoad();
	afx_msg void	OnBnClickedDelete();
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg			void OnBnClickedApply();
public:
	CButton			m_Radio_Creative[3];
	CButton			m_Radio_Attribute[3];
	CButton			m_Check_OnOff;
	CString			m_strCompute_Y;
public:
	_float			m_fCompute_Y = 0.f;
};
