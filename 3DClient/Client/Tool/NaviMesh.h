#pragma once


// CNaviMesh ��ȭ �����Դϴ�.

class CNaviMesh : public CDialogEx
{
	DECLARE_DYNAMIC(CNaviMesh)

public:
	CNaviMesh(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNaviMesh();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIMESH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
};
