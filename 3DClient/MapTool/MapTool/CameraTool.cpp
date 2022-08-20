// CameraTool.cpp: 구현 파일
//

#include "pch.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "CameraTool.h"
#include "Management.h"
#include "Layer.h"
#include "GameObject.h"
#include "Device.h"
#include "Camera.h"
#include "Transform.h"

// CCameraTool 대화 상자

IMPLEMENT_DYNAMIC(CCameraTool, CDialogEx)

CCameraTool::CCameraTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CameraTool, pParent)
	, m_strRotationX(_T(""))
	, m_strRotationY(_T(""))
	, m_strRotationZ(_T(""))
	, m_strPositionX(_T(""))
	, m_strPositionY(_T(""))
	, m_strPositionZ(_T(""))
{

}

CCameraTool::~CCameraTool()
{
}

void CCameraTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Text(pDX, IDC_EDIT4, m_strRotationX);
	DDX_Text(pDX, IDC_EDIT5, m_strRotationY);
	DDX_Text(pDX, IDC_EDIT6, m_strRotationZ);
	DDX_Text(pDX, IDC_EDIT1, m_strPositionX);
	DDX_Text(pDX, IDC_EDIT2, m_strPositionY);
	DDX_Text(pDX, IDC_EDIT3, m_strPositionZ);
	DDX_Control(pDX, IDC_RADIO4, m_Radio_Rotation[0]);
	DDX_Control(pDX, IDC_RADIO8, m_Radio_Rotation[1]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio_Rotation[2]);
}


BEGIN_MESSAGE_MAP(CCameraTool, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CCameraTool::OnLbnSelchangeListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CCameraTool::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON7, &CCameraTool::OnBnClickedApply)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON14, &CCameraTool::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_BUTTON15, &CCameraTool::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON6, &CCameraTool::OnBnClickeedStop)
	ON_BN_CLICKED(IDC_BUTTON16, &CCameraTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON17, &CCameraTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CCameraTool 메시지 처리기


void CCameraTool::OnLbnSelchangeListBox()
{
	_uint	iSelect = m_ListBox.GetCurSel();
	if (-1 == iSelect)
		return;
	_matrix matTemp = CManagement::GetInstance()->GetCameraMatrix()[iSelect];

	for (auto& pGameObject : *CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow"))
	{
		pGameObject->GetIsPick() = false;
	}

	CGameObject* pGameObject = CManagement::GetInstance()->Get_GameObject(SCENE_STATIC, L"Layer_Arrow", iSelect);
	pGameObject->GetIsPick() = true;

}


void CCameraTool::OnBnClickedPlay()
{
	IsCameraMove = true;
}


void CCameraTool::OnBnClickedApply()
{
	UpdateData(TRUE);
	m_fRotationX = (_float)_ttof(m_strRotationX);
	m_fRotationY = (_float)_ttof(m_strRotationY);
	m_fRotationZ = (_float)_ttof(m_strRotationZ);

	m_fPositionX = (_float)_ttof(m_strPositionX);
	m_fPositionY = (_float)_ttof(m_strPositionY);
	m_fPositionZ = (_float)_ttof(m_strPositionZ);

	auto iter = CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->begin();

	for (; iter != CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->end(); ++iter)
	{
		if ((*iter)->GetIsPick())
		{
			CTransform* pTransform = (CTransform*)(*iter)->Get_ComponentPointer(L"Com_Transform");
			pTransform->SetUp_Speed(5.f, D3DXToRadian(90.f));
			pTransform->Set_StateInfo(STATE_POSITION, &_vec3(m_fPositionX, m_fPositionY, m_fPositionZ));
			if (m_Radio_Rotation[0].GetCheck())
			{
				m_fRotationX = (_float)_ttof(m_strRotationX);
				pTransform->SetUp_RotationX(D3DXToRadian(m_fRotationX));
			}
			else if (m_Radio_Rotation[1].GetCheck())
			{
				m_fRotationY = (_float)_ttof(m_strRotationY);
				pTransform->SetUp_RotationY(D3DXToRadian(m_fRotationY));
			}
			else if (m_Radio_Rotation[2].GetCheck())
			{
				m_fRotationZ = (_float)_ttof(m_strRotationZ);
				pTransform->SetUp_RotationZ(D3DXToRadian(m_fRotationZ));
			}
		}
	}

	UpdateData(FALSE);
}


void CCameraTool::OnMouseMove(UINT nFlags, CPoint point)
{
	m_strRotationX.Format(_T("%f"), m_fRotationX);
	m_strRotationY.Format(_T("%f"), m_fRotationY);
	m_strRotationZ.Format(_T("%f"), m_fRotationZ);
	if (m_Radio_Rotation[0].GetCheck())
	{
		m_fRotationY = 0.f;
		m_fRotationZ = 0.f;
		m_strRotationY.Format(_T("%f"), m_fRotationY);
		m_strRotationZ.Format(_T("%f"), m_fRotationZ);
	}
	else if (m_Radio_Rotation[1].GetCheck())
	{
		m_fRotationX = 0.f;
		m_fRotationZ = 0.f;
		m_strRotationX.Format(_T("%f"), m_fRotationX);
		m_strRotationZ.Format(_T("%f"), m_fRotationZ);
	}
	else if (m_Radio_Rotation[2].GetCheck())
	{
		m_fRotationX = 0.f;
		m_fRotationY = 0.f;
		m_strRotationY.Format(_T("%f"), m_fRotationX);
		m_strRotationZ.Format(_T("%f"), m_fRotationY);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CCameraTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	return TRUE;

}


void CCameraTool::OnBnClickedUpdate()
{
	UpdateData(TRUE);
	auto iter = CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->begin();

	for (; iter != CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->end(); ++iter)
	{
		if ((*iter)->GetIsPick())
		{
			CTransform* pTransform = (CTransform*)(*iter)->Get_ComponentPointer(L"Com_Transform");
			_vec3 vPos = _vec3
			(
				pTransform->Get_StateInfo(STATE_POSITION)->x,
				pTransform->Get_StateInfo(STATE_POSITION)->y,
				pTransform->Get_StateInfo(STATE_POSITION)->z
			);
			_vec3 vSize = pTransform->Get_Scale();
			_vec3 vRotate = pTransform->Get_Rotate();

			m_strPositionX.Format(_T("%f"), vPos.x);
			m_strPositionY.Format(_T("%f"), vPos.y);
			m_strPositionZ.Format(_T("%f"), vPos.z);

			m_strRotationX.Format(_T("%f"), m_fRotationX);
			m_strRotationY.Format(_T("%f"), m_fRotationY);
			m_strRotationZ.Format(_T("%f"), m_fRotationZ);
		}
	}
	UpdateData(FALSE);
}


void CCameraTool::OnBnClickedDelete()
{
	_uint	iSelect = m_ListBox.GetCurSel();
	if (-1 == iSelect)
		return;

	m_ListBox.DeleteString(iSelect);

	CManagement::GetInstance()->GetCameraMatrix().erase(CManagement::GetInstance()->GetCameraMatrix().begin() + iSelect);

	auto iter = CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->begin();

	for (; iter != CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->end();)
	{
		if ((*iter)->GetIsPick())
		{
			Safe_Release(*iter);
			iter = CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->erase(iter);
		}
		else
			++iter;
	}


}


void CCameraTool::OnBnClickeedStop()
{
	IsCameraMove = false;
}


void CCameraTool::OnBnClickedSave()
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
		_uint iSize = pManagement->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->size();
		WriteFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iSize; ++i)
		{
			CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Arrow", L"Com_Transform", i);
			if (nullptr == pTransform)
				continue;

			_vec3 vPos = *pTransform->Get_StateInfo(STATE::STATE_POSITION);
			WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		}
		

		Safe_Release(pManagement);
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CCameraTool::OnBnClickedLoad()
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
		_vec3		vPoint = {};
		DWORD		dwByte = 0;
		_uint		iSize = 0;

		CManagement* pManagement = CManagement::GetInstance();
		if (nullptr == pManagement)
			return;
		pManagement->AddRef();
		pManagement->Find_Clear_Layer(SCENE_STATIC, L"Layer_Arrow");
		
		while (TRUE)
		{
			ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);
			if (dwByte == 0)
				break;

			for (_uint i = 0; i < iSize; ++i)
			{
				ReadFile(hFile, &vPoint, sizeof(_vec3),&dwByte, nullptr);
				if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Arrow", 
					SCENEID::SCENE_STATIC, L"Layer_Arrow", (void*)&vPoint)))
					return;

				CTransform* pTempTransform = (CTransform*)pManagement->Get_BackObject(SCENE_STATIC,
					L"Layer_Arrow")->Get_ComponentPointer(L"Com_Transform");

				CString strResult;
				CString strX;
				CString strY;
				CString strZ;
				strX.Format(_T("%f"), vPoint.x);
				strY.Format(_T("%f"), vPoint.y);
				strZ.Format(_T("%f"), vPoint.z);

				strResult += strX + L", " + strY + L", " + strZ;
				m_ListBox.AddString(strResult);
				CManagement::GetInstance()->GetCameraMatrix().push_back(pTempTransform->Get_Matrix());
			}


		}

		Safe_Release(pManagement);
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}
