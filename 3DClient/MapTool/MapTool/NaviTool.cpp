// NaviTool.cpp: 구현 파일
//

#include "pch.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "NaviTool.h"
#include "Management.h"
#include "GameObject.h"
#include "Cell.h"


// CNaviTool 대화 상자

IMPLEMENT_DYNAMIC(CNaviTool, CDialogEx)

CNaviTool::CNaviTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NaviTool, pParent)

	, m_strCompute_Y(_T(""))
{

}

CNaviTool::~CNaviTool()
{
}

void CNaviTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Radio_Creative[0]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio_Creative[1]);
	DDX_Control(pDX, IDC_RADIO14, m_Radio_Creative[2]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio_Attribute[0]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio_Attribute[1]);
	DDX_Control(pDX, IDC_RADIO6, m_Radio_Attribute[2]);
	DDX_Text(pDX, IDC_EDIT1, m_strCompute_Y);
}


BEGIN_MESSAGE_MAP(CNaviTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON8, &CNaviTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON9, &CNaviTool::OnBnClickedDelete)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON2, &CNaviTool::OnBnClickedApply)
END_MESSAGE_MAP()



void CNaviTool::OnBnClickedSave()
{
	UpdateData(TRUE);
	CFileDialog Dlg(FALSE, L"dat", L"제목없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szCurPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurPath);

	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		CString FilePath = Dlg.GetPathName();

		HANDLE hFile = CreateFile(FilePath.GetString(), GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		CManagement* pManagement = CManagement::GetInstance();
		if (nullptr == pManagement)
			return;
		pManagement->AddRef();
		CNavi*	pNavi = (CNavi*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Navi");
		_uint	iLen = pNavi->GetCellVector().size() ;

		NAVI	eSaveNavi = pNavi->GetNavi();
		WriteFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &eSaveNavi, sizeof(NAVI), &dwByte, nullptr);

		for (_uint i = 0; i < iLen; ++i)
		{
			for (_uint j = 0; j < 3; ++j)
			{
				_vec3 vPos = pNavi->GetCellVector()[i]->GetPoint()[j];
				WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
			}
		}



		Safe_Release(pManagement);
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CNaviTool::OnBnClickedLoad()
{
	UpdateData(TRUE);
	CFileDialog Dlg(TRUE, L"dat", L"제목없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szCurPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurPath);

	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;
	if (IDOK == Dlg.DoModal())
	{
		CString FilePath = Dlg.GetPathName();

		HANDLE hFile = CreateFile(FilePath.GetString(), GENERIC_READ, 0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;
		_vec3		vPoint[3] = {};
		DWORD		dwByte = 0;

		CManagement* pManagement = CManagement::GetInstance();
		if (nullptr == pManagement)
			return;
		pManagement->AddRef();
		CNavi* pNavi = (CNavi*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Navi");

		pNavi->GetTotalPos().clear();
	
		_uint	iLen = 0;
		NAVI	eLoadNavi = NAVI::NAVI_END;
		_uint	iCnt = 0;
		while (TRUE)
		{

			ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &eLoadNavi, sizeof(NAVI), &dwByte, nullptr);
			pNavi->GetNavi() = eLoadNavi;
			if (dwByte == 0)
			{
				break;
			}
			int m_iNaviMeshCnt = 0;

			for (int i = 0; i < iLen; ++i)
			{
				for (_uint j = 0; j < 3; ++j)
				{
					ReadFile(hFile, &vPoint[j], sizeof(_vec3), &dwByte, nullptr);			
					_vec3 vPos = vPoint[j];
					
					if (m_iNaviMeshCnt < 3)
					{
						//vPos.y = 5.f;

						int iPointIdx = 0;
						bool result = false;
						if (pNavi->GetTotalPos().empty())
							result = true;
						for (auto& iter : pNavi->GetTotalPos())
						{
							_vec3 vTemp = {};
							_vec3	vIter_Pos = iter.second;
							vIter_Pos.y = 0.f;

							_vec3	vTemp_Pos = vPos;
							vTemp_Pos.y = 0.f;

							vTemp = vIter_Pos - vTemp_Pos;
							_float fLength = D3DXVec3Length(&vTemp);
							if (fLength < 3.5f)
							{
								vPos = iter.second;
								iPointIdx = iter.first;
								result = false;
								break;
							}
							else
							{
								result = true;
							}
						}

						if (result == false)
						{

							if (FAILED(pNavi->Set_PositionVector(iPointIdx, vPos)))
								return;
							m_iNaviMeshCnt++;
						}
						else
						{
							if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Cube_Wire", SCENE_STATIC, L"Layer_CubeWire", (void*)&vPos)))
								return;
							if (FAILED(pNavi->Set_PositionVector(vPos)))
								return;
							m_iNaviMeshCnt++;
						}


					}



					if (m_iNaviMeshCnt > 2)
					{
						if (FAILED(pNavi->Set_CellVector(eLoadNavi)))
							return;
						pNavi->Ready_Neighbor();
						pNavi->Clear_Position_Vector();
						m_iNaviMeshCnt = 0;
					}
				}
			}



	
		}
		

		Safe_Release(pManagement);
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CNaviTool::OnBnClickedDelete()
{
	CNavi* pNavi = (CNavi*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Navi");
	if (nullptr == pNavi)
		return;

	pNavi->Delete_Cell();
}


void CNaviTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Radio_Creative[0].GetCheck())
	{
		g_eNaviMode = NAVI_MODE::NAVI_MODE_CREATE;
	}
	else if (m_Radio_Creative[1].GetCheck())
	{
		g_eNaviMode = NAVI_MODE::NAVI_MODE_MODIFY;
	}
	else if (m_Radio_Creative[2].GetCheck())
	{
		g_eNaviMode = NAVI_MODE::NAVI_MODE_DELETE;
	}


	if (m_Radio_Attribute[0].GetCheck())
	{
		eNavi = NAVI::NAVI_PLAYER;
	}
	else if (m_Radio_Attribute[1].GetCheck())
	{
		eNavi = NAVI::NAVI_MONSTER;
	}
	else if (m_Radio_Attribute[2].GetCheck())
	{
		eNavi = NAVI::NAVI_NPC;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CNaviTool::OnBnClickedApply()
{
	UpdateData(TRUE);

	CNavi* pNavi = (CNavi*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Navi");
	if (nullptr == pNavi)
		return;
	m_fCompute_Y = (_float)_ttof(m_strCompute_Y);

	CGameObject* pGameObject = nullptr;
	for (auto& iter : *CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_CubeWire"))
	{
		if (iter->GetIsNaviPick() == true)
		{

			pGameObject = iter;
		}
	}

	CTransform* pPickTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
	if (nullptr == pPickTransform)
		return;
	_vec3 vConfirmPos = {};
	vConfirmPos.x = pPickTransform->Get_StateInfo(STATE::STATE_POSITION)->x;
	vConfirmPos.z = pPickTransform->Get_StateInfo(STATE::STATE_POSITION)->z;
	vConfirmPos.y = m_fCompute_Y;

	pPickTransform->Set_StateInfo(STATE::STATE_POSITION, &vConfirmPos);

	_uint iIdx = pGameObject->m_iGetObjIdx;

	bool reulst = false;
	for (int i = 0; i < pNavi->m_vecCell.size(); ++i)
	{
		for (auto& iter : pNavi->m_vecCell[i]->m_vPoint)
		{
			_vec3 vTemp = {};
			_vec3	vIter_Pos = iter;
			vIter_Pos.y = 0.f;

			_vec3	vTemp_Pos = vConfirmPos;
			vTemp_Pos.y = 0.f;

			vTemp = vIter_Pos - vTemp_Pos;
			_float fLength = D3DXVec3Length(&vTemp);
			if (fLength < 2.f)
			{
				iter = vConfirmPos;
				reulst = true;
				pNavi->GetTotalPos()[iIdx] = vConfirmPos;
				CTransform* pTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
				pTransform->Set_StateInfo(STATE::STATE_POSITION, &vConfirmPos);
				//break;
			}
		}

	}
	if (reulst == false)
		return;

	pNavi->GetTotalPos()[iIdx] = vConfirmPos;
	CTransform* pTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
	pTransform->Set_StateInfo(STATE::STATE_POSITION, &vConfirmPos);

	UpdateData(FALSE);


}
