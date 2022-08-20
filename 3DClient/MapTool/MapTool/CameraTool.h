#pragma once
#include "afxdialogex.h"


// CCameraTool 대화 상자

class CCameraTool : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraTool)

public:
	CCameraTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraTool();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CameraTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox		m_ListBox;
	CButton			m_Radio_Rotation[3];
public:
	CString			m_strRotationX;
	CString			m_strRotationY;
	CString			m_strRotationZ;
	CString			m_strPositionX;
	CString			m_strPositionY;
	CString			m_strPositionZ;
public:
	_float			m_fRotationX = 0.f;
	_float			m_fRotationY = 0.f;
	_float			m_fRotationZ = 0.f;
	_float			m_fPositionX = 0.f;
	_float			m_fPositionY = 0.f;
	_float			m_fPositionZ = 0.f;
public:
	afx_msg void	OnLbnSelchangeListBox();
	afx_msg void	OnBnClickedPlay();
	afx_msg void	OnBnClickedApply();
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
public:
	virtual BOOL	OnInitDialog();

	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickeedStop();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
};
