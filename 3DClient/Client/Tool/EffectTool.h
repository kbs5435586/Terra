#pragma once
#include "afxwin.h"


// CEffectTool ��ȭ �����Դϴ�.

class CEffectTool : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectTool)

public:
	CEffectTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEffectTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeEffectTexture();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedCreate();
	virtual BOOL OnInitDialog();
public:
	CListBox m_ListBox_Effect_Texture;
	CString m_strMoveX;
	CString m_strMoveY;
	CString m_strMoveZ;
	CString m_strRotateX;
	CString m_strRotateY;
	CString m_strRotateZ;
public:
	map<const _tchar*, _int>		m_mapEffectMesh;

};
