#pragma once
#include "afxwin.h"


// CAnimationTool ��ȭ �����Դϴ�.

class CAnimationTool : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationTool)

public:
	CAnimationTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimationTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strObjectName;
	CListBox m_ListBox_Picked;

	CListBox m_ListBox_New_Animation_Name;
	int m_iInput_Key;
public:
	afx_msg void OnLbnSelchange_ListBox_Picked();
	afx_msg void OnLbnSelchangeNew_Animation_Name();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedAdd_Key();
	afx_msg void OnBnClickedAdd_Value();
public:
	map<_uint, vector<_int>>	m_mapNewAnimation;
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
};
