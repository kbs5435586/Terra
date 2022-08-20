#pragma once
#include "afxdialogex.h"


// CEffectTool 대화 상자

class CEffectTool : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectTool)

public:
	CEffectTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEffectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EffectTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox				m_ListBox_EffectMesh;
	CButton					m_Radio_Rotation[2];
	CButton					m_RadioRotate[3];
	CString					m_str_Size;
public:
	vector<CString>			m_vecEffectMesh;
	set<CString>			m_setEffectMesh;
	list<IMGPATHINFO*>		m_ImgInfoLst;
public:
	CString					m_pComponent_Tag = L"";
	CString					m_pLayer_Tag = L"";
	CString					m_pGameObject_Tag = L"";
public:
	_float					m_fSize = 0.f;
	_float					m_fMove_Amount = 0.f;
public:
	afx_msg void			OnBnClickedApply();
	afx_msg void			OnBnClickedSave();
	afx_msg void			OnBnClickedLoad();
	afx_msg void			OnBnClickedStop();
	afx_msg void			OnBnClickedRotate();
	afx_msg void			OnLbnSelchangeEffectBox();
	afx_msg void			OnDropFiles(HDROP hDropInfo);
	afx_msg void			OnBnClickedDropFile_Save();
	afx_msg void			OnBnClickedDropFile_Load();
	afx_msg void			OnBnClickedUp();
	afx_msg void			OnBnClickedDown();
	afx_msg void			OnBnClickedRight();
	afx_msg void			OnBnClickedLeft();
public:
	virtual BOOL			OnInitDialog();
public:
	void					SetHorizontalScroll();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedForward();
	afx_msg void OnBnClickedBackward();

	CString m_strMove_Amount;
	afx_msg void OnBnClickedMove_Apply();
};
