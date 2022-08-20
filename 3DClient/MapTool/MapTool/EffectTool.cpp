// EffectTool.cpp: 구현 파일
//

#include "pch.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "EffectTool.h"
#include "Management.h"
#include "GameObject.h"
#include "Layer.h"
#include "Component.h"

// CEffectTool 대화 상자

IMPLEMENT_DYNAMIC(CEffectTool, CDialogEx)

CEffectTool::CEffectTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EffectTool, pParent)
	, m_str_Size(_T("0.1"))
	, m_strMove_Amount(_T(""))
{
	
}

CEffectTool::~CEffectTool()
{
	for_each(m_ImgInfoLst.begin(), m_ImgInfoLst.end(), Safe_Delete<IMGPATHINFO*>);
	m_ImgInfoLst.clear();

}

void CEffectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_EffectMesh);
	DDX_Control(pDX, IDC_RADIO2, m_Radio_Rotation[0]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio_Rotation[1]);
	DDX_Text(pDX, IDC_EDIT4, m_str_Size);
	DDX_Control(pDX, IDC_RADIO11, m_RadioRotate[0]);
	DDX_Control(pDX, IDC_RADIO12, m_RadioRotate[1]);
	DDX_Control(pDX, IDC_RADIO13, m_RadioRotate[2]);
	DDX_Text(pDX, IDC_EDIT8, m_strMove_Amount);
}


BEGIN_MESSAGE_MAP(CEffectTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEffectTool::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON6, &CEffectTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CEffectTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON8, &CEffectTool::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON9, &CEffectTool::OnBnClickedRotate)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEffectTool::OnLbnSelchangeEffectBox)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON10, &CEffectTool::OnBnClickedDropFile_Save)
	ON_BN_CLICKED(IDC_BUTTON11, &CEffectTool::OnBnClickedDropFile_Load)
	ON_BN_CLICKED(IDC_BUTTON2, &CEffectTool::OnBnClickedUp)
	ON_BN_CLICKED(IDC_BUTTON3, &CEffectTool::OnBnClickedDown)
	ON_BN_CLICKED(IDC_BUTTON12, &CEffectTool::OnBnClickedRight)
	ON_BN_CLICKED(IDC_BUTTON4, &CEffectTool::OnBnClickedLeft)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON23, &CEffectTool::OnBnClickedForward)
	ON_BN_CLICKED(IDC_BUTTON24, &CEffectTool::OnBnClickedBackward)
	ON_BN_CLICKED(IDC_BUTTON25, &CEffectTool::OnBnClickedMove_Apply)
END_MESSAGE_MAP()


// CEffectTool 메시지 처리기


void CEffectTool::OnBnClickedApply()
{
	UpdateData(TRUE);
	m_fSize = (_float)_ttof(m_str_Size);
	auto iter = CManagement::GetInstance()->Get_MapLayer(SCENE_STATIC)->find(m_pLayer_Tag);
	if (iter == CManagement::GetInstance()->Get_MapLayer(SCENE_STATIC)->end())
	{

		if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(m_pGameObject_Tag,
			SCENEID::SCENE_STATIC, m_pLayer_Tag, (void*)&m_pComponent_Tag)))
			return;

		CTransform* pTempTransform = (CTransform*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC,
			m_pLayer_Tag)->Get_ComponentPointer(L"Com_Transform");
		pTempTransform->Scaling(m_fSize, m_fSize, m_fSize);
	}
	else
	{
		CManagement::GetInstance()->Find_Clear_Layer(SCENE_STATIC, m_pLayer_Tag);
		if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(m_pGameObject_Tag,
			SCENEID::SCENE_STATIC, m_pLayer_Tag, (void*)&m_pComponent_Tag)))
			return;
		CTransform* pTempTransform = (CTransform*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC,
			m_pLayer_Tag)->Get_ComponentPointer(L"Com_Transform");
		pTempTransform->Scaling(m_fSize, m_fSize, m_fSize);
	}

	fEffectMeshX = 0.f;
	fEffectMeshY = 0.f;
	fEffectMeshZ = 0.f;
	UpdateData(FALSE);
}


void CEffectTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEffectTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEffectTool::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEffectTool::OnBnClickedRotate()
{
	IsRotation_EffectMesh ^= true;

	if(m_RadioRotate[0].GetCheck() == true)
	{
		g_iEffectRotateNum = 0;
	}
	else if (m_RadioRotate[1].GetCheck() == true)
	{
		g_iEffectRotateNum = 1;
	}
	else if (m_RadioRotate[2].GetCheck() == true)
	{
		g_iEffectRotateNum = 2;
	}
}


void CEffectTool::OnLbnSelchangeEffectBox()
{
	_tchar* strName = L"";
	_uint   iSelect = m_ListBox_EffectMesh.GetCurSel();

	if (-1 == iSelect)
		return;


	m_pComponent_Tag = L"Component_Mesh_" + m_vecEffectMesh[iSelect];
	m_pLayer_Tag = L"Layer_Effect";
	m_pGameObject_Tag = L"GameObject_EffectMesh";


	auto& iter_find = m_setEffectMesh.find(m_vecEffectMesh[iSelect]);
	if (iter_find == m_setEffectMesh.end())
	{
		if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, m_pComponent_Tag))
		{
			if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, m_pComponent_Tag,
				CStatic_Mesh::Create(CManagement::GetInstance()->Get_Graphic_Device(),
					L"../Resource/Mesh/EffectMesh/", m_vecEffectMesh[iSelect] + L".X"))))
				return;
		}

		m_setEffectMesh.insert(m_vecEffectMesh[iSelect]);
	}

}

void CEffectTool::SetHorizontalScroll()
{
	CString strName = L"";
	CSize   size;

	int iCX = 0;

	CDC* pDC = m_ListBox_EffectMesh.GetDC();
	for (int i = 0; i < m_ListBox_EffectMesh.GetCount(); ++i)
	{
		m_ListBox_EffectMesh.GetText(i, strName);
		size = pDC->GetTextExtent(strName);

		if (size.cx > iCX)
			iCX = size.cx;
	}

	m_ListBox_EffectMesh.ReleaseDC(pDC);

	if (iCX > m_ListBox_EffectMesh.GetHorizontalExtent())
		m_ListBox_EffectMesh.SetHorizontalExtent(iCX);
}



BOOL CEffectTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_RadioRotate[1].SetCheck(true);
	return TRUE; 
}


void CEffectTool::OnDropFiles(HDROP hDropInfo)
{
	int iCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFullPath[MAX_STR] = L"";
	wstring strFullPath = L"";
	wstring strX = L".X";
	for (int i = 0; i < iCount; ++i)
	{
		CFileFind find;
		TCHAR szFileName[MAX_STR] = L"";

		DragQueryFile(hDropInfo, i, szFullPath, MAX_STR);
		strFullPath = szFullPath;
		IMGPATHINFO* pInfo = new IMGPATHINFO;

		wstring wstrFilePath = strFullPath;
		wstring temp = PathFindExtension(wstrFilePath.c_str());
		if (strX != temp)
			continue;

		temp = PathFindFileName(wstrFilePath.c_str());
		lstrcpy(szFileName, temp.c_str());
		PathRemoveExtension(szFileName);
		temp = szFileName;
		pInfo->wstrObjKey= temp;
		m_ImgInfoLst.push_back(pInfo);
	}

	wstring wstrCombined = L"";
	TCHAR szCount[MAX_STR] = L"";

	for (auto& pImgPathInfo : m_ImgInfoLst)
	{
		wstrCombined = pImgPathInfo->wstrObjKey;

		m_ListBox_EffectMesh.AddString(wstrCombined.c_str());
		m_vecEffectMesh.push_back(wstrCombined.c_str());
	}

	SetHorizontalScroll();


	CDialogEx::OnDropFiles(hDropInfo);
}


void CEffectTool::OnBnClickedDropFile_Save()
{
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"txt", L"제목없음.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Txt Files(*.txt)|*.txt||", this);

	TCHAR szCurPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurPath);


	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		// C++ 파일 입출력
		wofstream fout;
		fout.open(Dlg.GetPathName().GetString());

		if (fout.fail())
			return;

		wstring wstrCombined = L"";
		TCHAR szCount[MAX_STR] = L"";

		for (auto& pImgPathInfo : m_ImgInfoLst)
		{

			wstrCombined = pImgPathInfo->wstrObjKey;

			fout << wstrCombined << endl;
		}

		fout.close();
	}

	UpdateData(FALSE);

}


void CEffectTool::OnBnClickedDropFile_Load()
{
	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"txt", L"제목없음.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Txt Files(*.txt)|*.txt||", this);

	TCHAR szCurPath[MAX_STR] = L"";


	GetCurrentDirectory(MAX_STR, szCurPath);


	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data");


	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		// C++ 파일 입출력
		ifstream in;
		in.open(Dlg.GetPathName().GetString());
		if (in.fail())
			return;

		m_ListBox_EffectMesh.ResetContent();

		for_each(m_ImgInfoLst.begin(), m_ImgInfoLst.end(), Safe_Delete<IMGPATHINFO*>);
		m_ImgInfoLst.clear();

		wstring wstrCombined = L"";
		TCHAR szBuf[MAX_STR] = L"";

		IMGPATHINFO* pImgPathInfo = nullptr;

		while (!in.eof())
		{
			string temp;
			wstring strObjectKey;
			pImgPathInfo = new IMGPATHINFO;

			getline(in, temp);
			strObjectKey.assign(temp.begin(), temp.end());
			pImgPathInfo->wstrObjKey = strObjectKey;

			m_ListBox_EffectMesh.AddString(pImgPathInfo->wstrObjKey.c_str());
			m_vecEffectMesh.push_back(pImgPathInfo->wstrObjKey.c_str());
			m_ImgInfoLst.push_back(pImgPathInfo);
		}

		in.close();
	}

	SetHorizontalScroll();

	UpdateData(FALSE);
}

void CEffectTool::OnBnClickedUp()
{
	fEffectMeshY += m_fMove_Amount;
}
void CEffectTool::OnBnClickedDown()
{
	fEffectMeshY -= m_fMove_Amount;
}
void CEffectTool::OnBnClickedRight()
{
	fEffectMeshX += m_fMove_Amount;;
}
void CEffectTool::OnBnClickedLeft()
{
	fEffectMeshX -= m_fMove_Amount;;
}
void CEffectTool::OnBnClickedForward()
{
	fEffectMeshZ += m_fMove_Amount;
}
void CEffectTool::OnBnClickedBackward()
{
	fEffectMeshZ -= m_fMove_Amount;
}

void CEffectTool::OnMouseMove(UINT nFlags, CPoint point)
{
	m_str_Size;

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CEffectTool::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}





void CEffectTool::OnBnClickedMove_Apply()
{
	UpdateData(TRUE);
	m_fMove_Amount = (_float)_ttof(m_strMove_Amount);;
	UpdateData(FALSE);
}
