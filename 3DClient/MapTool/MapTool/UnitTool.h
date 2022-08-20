#pragma once
#include "afxdialogex.h"


// CUnitTool 대화 상자

class CUnitTool : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UnitTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox								m_ListBox_Static;
	CListBox								m_ListBox_Dynamic;
public:
	_float									m_fPosX = 0.f;
	_float									m_fPosY = 0.f;
	_float									m_fPosZ = 0.f;
	_float									m_fRotX = 0.f;
	_float									m_fRotY = 0.f;
	_float									m_fRotZ = 0.f;
	_float									m_fScaleX = 0.f;
	_float									m_fScaleY = 0.f;
	_float									m_fScaleZ = 0.f;
	_float									m_fColliderSize = 10.f;
public:
	_uint									m_iPartListIdx = 0;
public:
	_bool									m_IsStatic = false;
	_bool									m_IsDynamic = false;
public:
	MESH_TYPE								m_eMeshType = MESH_TYPE::NONE;
public:
	vector<CString>							m_vecStaticMesh;
	set<CString>							m_setStaticMesh;
	vector<CString>							m_vecDynamicMesh;
	set<CString>							m_setDynamicMesh;
public:
	map<string, vector<const char*>>		m_mapPart;
	vector < pair< string,int> >			m_vecInsert;
	vector<const char*>						m_vecPartName;
	vector<string>							m_vecMeshName;
public:
	vector<_tchar*>							m_vecComponentTag;
	vector<_tchar*>							m_vecLayerTag;
	vector<_tchar*>							m_vecGameObjectTag;
public:
	afx_msg void							OnBnClickedApply();
	afx_msg void							OnBnClickedSave();
	afx_msg void							OnBnClickedLoad();
	afx_msg void							OnLbnSelchange_Static_Mesh();
	afx_msg void							OnLbnSelchange_Dynamic_Mesh();
	afx_msg void							OnBnClickedCamera_Apply();
	afx_msg void							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void							OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void							OnBnClickedUpdate();
	afx_msg void							OnBnClickedDelete();
	afx_msg void							OnBnClickedPartList_Save();
	afx_msg void							OnBnClickedPartList_Load();

	afx_msg void							OnBnClickedPartApply();
	virtual BOOL							OnInitDialog();
public:
	void									SetUp_PartList(string pTag);
public:
	CString									m_strPos_X;
	CString									m_strPos_Y;
	CString									m_strPos_Z;
	CString									m_strRot_X;
	CString									m_strRot_Y;
	CString									m_strRot_Z;
	CString									m_strScale_X;
	CString									m_strScale_Y;
	CString									m_strScale_Z;
	CString									m_strCameraSpeed;
	CButton									m_Radio_Deffered[2];
	CButton									m_Radio_Rotate[3];
	CButton									m_Radio_Creative[2];
	CButton									m_Radio_Drag[2];
	CListBox								m_ListBox_PartList;
	CString									m_strColliderSize;

public:
	CString									m_pComponent_Tag=L"";
	TCHAR*									m_pLayer_Tag=nullptr;
	CString									m_pGameObject_Tag = L"";
	CString									m_cstrMeshName;


	afx_msg void OnLbnSelchangePartList();
	afx_msg void OnBnClickedPartList_Delete();

	afx_msg void OnBnClickedInput();
	CListBox m_ListBox_Input;
	afx_msg void OnLbnSelchangeInput();
};
