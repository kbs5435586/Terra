// CUnitTool.cpp: 구현 파일
//

#include "pch.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "UnitTool.h"
#include "Management.h"
#include "GameObject.h"
#include "Layer.h"
#include "Static_Mesh.h"
#include "Dynamic_Mesh.h"
#include <string>


// CUnitTool 대화 상자

IMPLEMENT_DYNAMIC(CUnitTool, CDialogEx)

CUnitTool::CUnitTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UnitTool, pParent)
	, m_strPos_X(_T("0"))
	, m_strPos_Y(_T("0"))
	, m_strPos_Z(_T("0"))
	, m_strRot_X(_T("0"))
	, m_strRot_Y(_T("0"))
	, m_strRot_Z(_T("0"))
	, m_strScale_X(_T("0.1"))
	, m_strScale_Y(_T("0.1"))
	, m_strScale_Z(_T("0.1"))
	, m_strCameraSpeed(_T("20"))
	, m_strColliderSize(_T("10"))
{

}

CUnitTool::~CUnitTool()
{
	for (auto& iter : m_vecComponentTag)
		Safe_Delete(iter);
	for (auto& iter : m_vecGameObjectTag)
		Safe_Delete(iter);
	for (auto& iter : m_vecLayerTag)
		Safe_Delete(iter);


}

void CUnitTool::DoDataExchange(CDataExchange* pDX)	
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_Static);
	DDX_Control(pDX, IDC_LIST2, m_ListBox_Dynamic);
	DDX_Text(pDX, IDC_EDIT1, m_strPos_X);
	DDX_Text(pDX, IDC_EDIT2, m_strPos_Y);
	DDX_Text(pDX, IDC_EDIT3, m_strPos_Z);
	DDX_Text(pDX, IDC_EDIT4, m_strRot_X);
	DDX_Text(pDX, IDC_EDIT5, m_strRot_Y);
	DDX_Text(pDX, IDC_EDIT6, m_strRot_Z);
	DDX_Text(pDX, IDC_EDIT7, m_strScale_X);
	DDX_Text(pDX, IDC_EDIT8, m_strScale_Y);
	DDX_Text(pDX, IDC_EDIT9, m_strScale_Z);
	DDX_Text(pDX, IDC_EDIT10, m_strCameraSpeed);
	DDX_Control(pDX, IDC_RADIO1, m_Radio_Deffered[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio_Deffered[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio_Rotate[0]);
	DDX_Control(pDX, IDC_RADIO4, m_Radio_Rotate[1]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio_Rotate[2]);
	DDX_Control(pDX, IDC_RADIO6, m_Radio_Creative[0]);
	DDX_Control(pDX, IDC_RADIO7, m_Radio_Creative[1]);
	DDX_Control(pDX, IDC_LIST3, m_ListBox_PartList);
	DDX_Text(pDX, IDC_EDIT11, m_strColliderSize);
	DDX_Control(pDX, IDC_RADIO9, m_Radio_Drag[0]);
	DDX_Control(pDX, IDC_RADIO10, m_Radio_Drag[1]);
	DDX_Control(pDX, IDC_LIST4, m_ListBox_Input);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnBnClickedLoad)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelchange_Static_Mesh)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedCamera_Apply)
	ON_WM_ACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ACTIVATE()
	ON_LBN_SELCHANGE(IDC_LIST2, &CUnitTool::OnLbnSelchange_Dynamic_Mesh)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_BUTTON13, &CUnitTool::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON18, &CUnitTool::OnBnClickedPartList_Save)
	ON_BN_CLICKED(IDC_BUTTON19, &CUnitTool::OnBnClickedPartList_Load)
	ON_BN_CLICKED(IDC_BUTTON20, &CUnitTool::OnBnClickedPartApply)
	ON_LBN_SELCHANGE(IDC_LIST3, &CUnitTool::OnLbnSelchangePartList)
	ON_BN_CLICKED(IDC_BUTTON21, &CUnitTool::OnBnClickedPartList_Delete)
	ON_BN_CLICKED(IDC_BUTTON22, &CUnitTool::OnBnClickedInput)
	ON_LBN_SELCHANGE(IDC_LIST4, &CUnitTool::OnLbnSelchangeInput)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기


void CUnitTool::OnBnClickedApply()
{
	UpdateData(TRUE);


	m_fPosX = (_float)_ttof(m_strPos_X);
	m_fPosY = (_float)_ttof(m_strPos_Y);
	m_fPosZ = (_float)_ttof(m_strPos_Z);
	m_fScaleX = (_float)_ttof(m_strScale_X);
	m_fScaleY = (_float)_ttof(m_strScale_Y);
	m_fScaleZ = (_float)_ttof(m_strScale_Z);
	m_fRotX = (_float)_ttof(m_strRot_X);
	m_fRotY = (_float)_ttof(m_strRot_Y);
	m_fRotZ = (_float)_ttof(m_strRot_Z);


	//if (m_Radio_Creative[1].GetCheck())
	//{
	//	return;
	//}

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();
	map<const _tchar*, CLayer*>	mapTemp = *pManagement->Get_MapLayer(SCENE_STATIC);

	for (auto& Layer : mapTemp)
	{
		auto& GameObject = Layer.second->GetGameObjectList()->begin();
		for (; GameObject != Layer.second->GetGameObjectList()->end(); ++GameObject)
		{
			if ((*GameObject)->GetIsPick())
			{
				CTransform* pTransform = (CTransform*)(*GameObject)->Get_ComponentPointer(L"Com_Transform");
				pTransform->SetUp_Speed(5.f, D3DXToRadian(90.f));
				pTransform->Set_StateInfo(STATE_POSITION, &_vec3(m_fPosX, m_fPosY, m_fPosZ));
				if (m_Radio_Rotate[0].GetCheck())
				{
					m_fRotX = (_float)_ttof(m_strRot_X);
					pTransform->SetUp_RotationX(D3DXToRadian(m_fRotX));
				}
				else if (m_Radio_Rotate[1].GetCheck())
				{
					m_fRotY = (_float)_ttof(m_strRot_Y);
					pTransform->SetUp_RotationY(D3DXToRadian(m_fRotY));
				}
				else if (m_Radio_Rotate[2].GetCheck())
				{
					m_fRotZ = (_float)_ttof(m_strRot_Z);
					pTransform->SetUp_RotationZ(D3DXToRadian(m_fRotZ));
				}

				pTransform->Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
				pTransform->Set_Add_PosY(m_fPosY);
			}
		}
	}
	Safe_Release(pManagement);
	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedSave()
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

		DWORD dwByte_Size = 0;
		DWORD dwByte = 0;

		CManagement* pManagement = CManagement::GetInstance();
		pManagement->AddRef();

		for (int i = 0; i < m_ListBox_Static.GetCount(); i++)
		{
			CString str, strLayer;
			strLayer = L"Layer_";

	

			m_ListBox_Static.GetText(i, str);
			strLayer += str;


			if (CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, strLayer) == nullptr)
				continue;

			vector<CGameObject*> ObjList = *CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, strLayer);
			auto iter = ObjList.begin();

			int size = ObjList.size();
			WriteFile(hFile, &size, sizeof(int), &dwByte_Size, nullptr);

			for (iter; iter != ObjList.end(); iter++)
			{
				WriteFile(hFile, &(*iter)->GetIsStatic(), sizeof(bool), &dwByte, nullptr);
				CComponent* pMesh = (*iter)->Get_ComponentPointer(L"Com_Mesh");

				CTransform* pTransform = nullptr;
				_matrix mat = {};
				_float  fAdd_PosY = 0.f;
				TCHAR* pNames = pMesh->Get_ObjectName();
				TCHAR* pLayerTag = (TCHAR*)(LPCTSTR)(strLayer);
				TCHAR* pComTag = (*iter)->Get_Component_Tag();

				TCHAR* pMeshName = pMesh->Get_Object_FileName();
				CString cstrTemp = pMeshName;
				string strTemp = CT2CA(cstrTemp);
				string strTempNum;
				strTempNum = strTemp.back();//tag

				strTemp.pop_back();
				cstrTemp = strTemp.c_str();

				int iLength = lstrlen(pMesh->Get_ObjectName()) + 1;
				int iLength_Layer = lstrlen(strLayer) + 1;
				int iLength_ComTag = lstrlen(pComTag) + 1;
				int iLength_MeshName = lstrlen(cstrTemp) + 1;

				int iNum = atoi(strTempNum.c_str());

				WriteFile(hFile, &iLength, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, pNames, sizeof(TCHAR) * iLength, &dwByte, nullptr);

				WriteFile(hFile, &iLength_Layer, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, pLayerTag, sizeof(TCHAR) * iLength_Layer, &dwByte, nullptr);

				WriteFile(hFile, &iLength_ComTag, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, pComTag, sizeof(TCHAR) * iLength_ComTag, &dwByte, nullptr);

				WriteFile(hFile, &iLength_MeshName, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, cstrTemp, sizeof(TCHAR) * iLength_MeshName, &dwByte, nullptr);
				WriteFile(hFile, &iNum, sizeof(int), &dwByte, nullptr);
	
				

				pTransform = (CTransform*)(*iter)->Get_ComponentPointer(L"Com_Transform");
				mat = pTransform->Get_Matrix();
				WriteFile(hFile, mat, sizeof(_matrix), &dwByte, nullptr);
				fAdd_PosY = pTransform->Get_Add_PosY();
				WriteFile(hFile, (void*)&fAdd_PosY, sizeof(_float), &dwByte, nullptr);


			}
		}

		for (int i = 0; i < m_ListBox_Dynamic.GetCount(); ++i)
		{
			CString str, strLayer;
			strLayer += L"Layer_";
			m_ListBox_Dynamic.GetText(i, str);
			strLayer += str;

			if (CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, strLayer) == nullptr)
				continue;

			vector<CGameObject*>		ObjList = *CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, strLayer);
			auto iter = ObjList.begin();
			int size = ObjList.size();
			WriteFile(hFile, &size, sizeof(int), &dwByte_Size, nullptr);

			for (; iter != ObjList.end(); ++iter)
			{
				WriteFile(hFile, &(*iter)->GetIsStatic(), sizeof(bool), &dwByte, nullptr);
				CComponent* pMesh = (*iter)->Get_ComponentPointer(L"Com_Mesh");

				CTransform* pTransform = nullptr;
				_matrix mat = {};
				_float  fAdd_PosY = 0.f;
				TCHAR* pNames = pMesh->Get_ObjectName();
				TCHAR* pLayerTag = (TCHAR*)(LPCTSTR)(strLayer);
				TCHAR* pComTag = (*iter)->Get_Component_Tag();
				TCHAR* pMeshName = pMesh->Get_Object_FileName();

				int iLength = lstrlen(pMesh->Get_ObjectName()) + 1;
				int iLength_Layer = lstrlen(strLayer) + 1;
				int iLength_ComTag = lstrlen(pComTag) + 1;
				int iLength_MeshName = lstrlen(pMeshName) + 1;

				WriteFile(hFile, &iLength, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, pNames, sizeof(TCHAR) * iLength, &dwByte, nullptr);

				WriteFile(hFile, &iLength_Layer, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, pLayerTag, sizeof(TCHAR) * iLength_Layer, &dwByte, nullptr);

				WriteFile(hFile, &iLength_ComTag, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, pComTag, sizeof(TCHAR)* iLength_ComTag, &dwByte, nullptr);

				WriteFile(hFile, &iLength_MeshName, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, pMeshName, sizeof(TCHAR)* iLength_MeshName, &dwByte, nullptr);

				pTransform = (CTransform*)(*iter)->Get_ComponentPointer(L"Com_Transform");
				mat = pTransform->Get_Matrix();
				WriteFile(hFile, mat, sizeof(_matrix), &dwByte, nullptr);
				fAdd_PosY = pTransform->Get_Add_PosY();
				WriteFile(hFile, (void*)&fAdd_PosY, sizeof(_float), &dwByte, nullptr);
			}

		}

		Safe_Release(pManagement);
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}



void CUnitTool::OnBnClickedLoad()
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

		_matrix	mat;
		_float  fAdd_PosY = 0.f;
		D3DXMatrixIdentity(&mat);
		TCHAR szName[MAX_STR] = L"";
		TCHAR szLayerTag[MAX_STR] = L"";
		TCHAR szComTag[MAX_STR] = L"";
		TCHAR szMeshName[MAX_STR] = L"";

		DWORD	dwByte = 0;
		DWORD	dwByte_Size = 0;
		int		iLength = 0;
		int		iLength_Layer = 0;
		int		iLength_Com = 0;
		int		iLength_MeshName = 0;
		int		iNum = 0;
		bool	isStatic = false;
		while (TRUE)
		{
			int iSize = 0;
			ReadFile(hFile, &iSize, sizeof(int), &dwByte_Size, nullptr);
			if (dwByte_Size == 0)
				break;

			for (int i = 0; i < iSize; i++)
			{
				ReadFile(hFile, &isStatic, sizeof(bool), &dwByte, nullptr);

				if (isStatic)
				{
					ReadFile(hFile, &iLength, sizeof(int), &dwByte, nullptr);
					ReadFile(hFile, szName, sizeof(TCHAR) * iLength, &dwByte, nullptr);

					ReadFile(hFile, &iLength_Layer, sizeof(int), &dwByte, nullptr);
					ReadFile(hFile, szLayerTag, sizeof(TCHAR) * iLength_Layer, &dwByte, nullptr);

					ReadFile(hFile, &iLength_Com, sizeof(int), &dwByte, nullptr);
					ReadFile(hFile, szComTag, sizeof(TCHAR) * iLength_Com, &dwByte, nullptr);

					ReadFile(hFile, &iLength_MeshName, sizeof(int), &dwByte, nullptr);
					ReadFile(hFile, szMeshName, sizeof(TCHAR) * iLength_MeshName, &dwByte, nullptr);
					ReadFile(hFile, &iNum, sizeof(int), &dwByte, nullptr);

					_tchar* pName = new _tchar[iLength + 1];
					ZeroMemory(pName, iLength + 1);

					_tchar* pLayerTag = new _tchar[iLength_Layer + 1];
					ZeroMemory(pLayerTag, iLength_Layer + 1);

					_tchar* pComrTag = new _tchar[iLength_Com + 1];
					ZeroMemory(pComrTag, iLength_Com + 1);

					//New
					_tchar* pMeshTag = new _tchar[iLength_MeshName + 1];
					ZeroMemory(pMeshTag, iLength_MeshName + 1);

					lstrcpy(pName, szName);
					lstrcpy(pLayerTag, szLayerTag);
					lstrcpy(pComrTag, szComTag);
					lstrcpy(pMeshTag, szMeshName);
				

					CString strDefaultPath = L"../Resource/Mesh/StaticMesh/";
					CString strPath;
					CString strFileName;
					CString strToint;
					CString strSlash = L"/";
					strToint.Format(_T("%d"), iNum);

					strDefaultPath += (pMeshTag + strSlash);
					strFileName = lstrcat(pMeshTag, strToint);

					if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, pComrTag))
					{
						if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, pComrTag,
							CStatic_Mesh::Create(CManagement::GetInstance()->Get_Graphic_Device(),
								strDefaultPath + strFileName + strSlash, strFileName + L".X"))))
							return;

					}
					if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_StaticMesh", SCENEID::SCENE_STATIC, pLayerTag, pComrTag)))
						return;

					ReadFile(hFile, mat, sizeof(_matrix), &dwByte, nullptr);
					CTransform* pTransform = (CTransform*)CManagement::GetInstance()->Get_BackObject(SCENEID::SCENE_STATIC, szLayerTag)->Get_ComponentPointer(L"Com_Transform");
					pTransform->Set_Matrix(mat);

					ReadFile(hFile, (void*)&fAdd_PosY, sizeof(_float), &dwByte, nullptr);
					pTransform->Set_Add_PosY(fAdd_PosY);
				}
				else
				{

					ReadFile(hFile, &iLength, sizeof(int), &dwByte, nullptr);
					ReadFile(hFile, szName, sizeof(TCHAR) * iLength, &dwByte, nullptr);

					ReadFile(hFile, &iLength_Layer, sizeof(int), &dwByte, nullptr);
					ReadFile(hFile, szLayerTag, sizeof(TCHAR) * iLength_Layer, &dwByte, nullptr);

					ReadFile(hFile, &iLength_Com, sizeof(int), &dwByte, nullptr);
					ReadFile(hFile, szComTag, sizeof(TCHAR) * iLength_Com, &dwByte, nullptr);

					ReadFile(hFile, &iLength_MeshName, sizeof(int), &dwByte, nullptr);
					ReadFile(hFile, szMeshName, sizeof(TCHAR) * iLength_MeshName, &dwByte, nullptr);

					_tchar* pName = new _tchar[iLength + 1];
					ZeroMemory(pName, iLength + 1);

					_tchar* pLayerTag = new _tchar[iLength_Layer + 1];
					ZeroMemory(pLayerTag, iLength_Layer + 1);

					_tchar* pComrTag = new _tchar[iLength_Com + 1];
					ZeroMemory(pComrTag, iLength_Com + 1);

					_tchar* pMeshTag = new _tchar[iLength_MeshName + 1];
					ZeroMemory(pMeshTag, iLength_MeshName + 1);

					lstrcpy(pName, szName);
					lstrcpy(pLayerTag, szLayerTag);
					lstrcpy(pComrTag, szComTag);
					lstrcpy(pMeshTag, szMeshName);

					CString strDefaultPath = L"../Resource/Mesh/DynamicMesh/";
					CString strSlash = L"/";
					strDefaultPath += (pMeshTag);


					_tchar* temp = lstrcat(pMeshTag, L".X");
					if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, pComrTag))
					{
						if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, pComrTag,
							CDynamic_Mesh::Create(CManagement::GetInstance()->Get_Graphic_Device(),
								strDefaultPath  + strSlash, temp))))
							return;

						//if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Mesh_Beatle",
						//	CDynamic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/DynamicMesh/Beatle/", L"Beatle.X"))))
						//	return E_FAIL; 

					}

					if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_DynamicMesh", SCENEID::SCENE_STATIC, pLayerTag, pComrTag)))
						return;

					//pTransform = (CTransform*)(*iter)->Get_ComponentPointer(L"Com_Transform");
					//mat = pTransform->Get_Matrix();
					//WriteFile(hFile, mat, sizeof(_matrix), &dwByte, nullptr);
					//fAdd_PosY = pTransform->Get_Add_PosY();
					//WriteFile(hFile, (void*)&fAdd_PosY, sizeof(_float), &dwByte, nullptr);

					ReadFile(hFile, mat, sizeof(_matrix), &dwByte, nullptr);
					CTransform* pTransform = (CTransform*)CManagement::GetInstance()->Get_BackObject(SCENEID::SCENE_STATIC, szLayerTag)->Get_ComponentPointer(L"Com_Transform");
					pTransform->Set_Matrix(mat);

					ReadFile(hFile, (void*)&fAdd_PosY, sizeof(_float), &dwByte, nullptr);
					pTransform->Set_Add_PosY(fAdd_PosY);

				}

			/*	ReadFile(hFile, &iLength, sizeof(int), &dwByte, nullptr);
				ReadFile(hFile, szName, sizeof(TCHAR) * iLength, &dwByte, nullptr);

				ReadFile(hFile, &iLength_Layer, sizeof(int), &dwByte, nullptr);
				ReadFile(hFile, szLayerTag, sizeof(TCHAR) * iLength_Layer, &dwByte, nullptr);

				ReadFile(hFile, &iLength_Com, sizeof(int), &dwByte, nullptr);
				ReadFile(hFile, szComTag, sizeof(TCHAR) * iLength_Com, &dwByte, nullptr);

				ReadFile(hFile, &iLength_MeshName, sizeof(int), &dwByte, nullptr);
				ReadFile(hFile, szMeshName, sizeof(TCHAR) * iLength_MeshName, &dwByte, nullptr);
				ReadFile(hFile, &iNum, sizeof(int), &dwByte, nullptr);


				_tchar* pName = new _tchar[iLength + 1];
				ZeroMemory(pName, iLength + 1);

				_tchar* pLayerTag = new _tchar[iLength_Layer + 1];
				ZeroMemory(pLayerTag, iLength_Layer + 1);

				_tchar* pComrTag = new _tchar[iLength_Com + 1];
				ZeroMemory(pComrTag, iLength_Com + 1);

				lstrcpy(pName, szName);
				lstrcpy(pLayerTag, szLayerTag);
				lstrcpy(pComrTag, szComTag);

				if (isStatic)
				{
					CString strDefaultPath= L"../Resource/Mesh/StaticMesh/";
					CString strPath;
					CString strFileName;
					CString strToint;
					CString strSlash = L"/";
					strToint.Format(_T("%d"), iNum);

					strDefaultPath += (szMeshName+ strSlash);
					strFileName = lstrcat(szMeshName, strToint);
			
					if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, pComrTag))
					{
						if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, pComrTag,
							CStatic_Mesh::Create(CManagement::GetInstance()->Get_Graphic_Device(),
								strDefaultPath + strFileName + strSlash,strFileName + L".X"))))
							return;

					}

					if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_StaticMesh", SCENEID::SCENE_STATIC, pLayerTag, pComrTag)))
						return;
				}
				else
				{
					CString strDefaultPath = L"../Resource/Mesh/DynamicMesh/";
					CString strPath;
					CString strFileName;
					CString strToint;
					CString strSlash = L"/";
					strToint.Format(_T("%d"), iNum);

					strDefaultPath += (szMeshName + strSlash);
					strFileName = lstrcat(szMeshName, strToint);

					if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, pComrTag))
					{
						if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, pComrTag,
							CStatic_Mesh::Create(CManagement::GetInstance()->Get_Graphic_Device(),
								strDefaultPath + strFileName + strSlash, strFileName + L".X"))))
							return;

					}

					if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_DynamicMesh", SCENEID::SCENE_STATIC, pLayerTag, pComrTag)))
						return;
				}


				ReadFile(hFile, mat, sizeof(_matrix), &dwByte, nullptr);
				CTransform* pTransform = (CTransform*)CManagement::GetInstance()->Get_BackObject(SCENEID::SCENE_STATIC, szLayerTag)->Get_ComponentPointer(L"Com_Transform");
				pTransform->Set_Matrix(mat);

				ReadFile(hFile, (void*)&fAdd_PosY, sizeof(_float), &dwByte, nullptr);
				pTransform->Set_Add_PosY(fAdd_PosY);*/
			}
		}

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


BOOL CUnitTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ifstream readFile_Static("../Resource/MeshList/StaticMeshList.txt");
	if (readFile_Static.is_open())
	{
		while (!readFile_Static.eof())
		{
			string str;
			CString strTemp;
			getline(readFile_Static, str);
			strTemp = str.c_str();
			m_vecStaticMesh.push_back(strTemp);
			m_ListBox_Static.AddString(strTemp);
		}
		readFile_Static.close();
	}
	ifstream readFile_Dynamic("../Resource/MeshList/DynamicMeshList.txt");
	if (readFile_Dynamic.is_open())
	{
		while (!readFile_Dynamic.eof())
		{
			string str;
			CString strTemp;
			getline(readFile_Dynamic, str);
			strTemp = str.c_str();
			m_vecDynamicMesh.push_back(strTemp);
			m_ListBox_Dynamic.AddString(strTemp);
		}
		readFile_Dynamic.close();
	}

	m_Radio_Deffered[0].SetCheck(true);
	m_Radio_Deffered[1].SetCheck(false);
	return TRUE;
}

void CUnitTool::SetUp_PartList(string pTag)
{
	auto iter_find = m_mapPart.find(pTag);

	if (iter_find == m_mapPart.end())
		return;


	m_ListBox_PartList.ResetContent();

	for (auto& iter : iter_find->second)
	{		
		CString strTemp;
		strTemp = (LPSTR)iter;
		m_ListBox_PartList.AddString(strTemp);
	}
}


void CUnitTool::OnLbnSelchange_Static_Mesh()
{
	_tchar* strName = L"";
	_uint	iSelect = m_ListBox_Static.GetCurSel();

	if (-1 == iSelect)
		return;


	CString Component_Tag = L"Component_Mesh_" + m_vecStaticMesh[iSelect];
	CString Layer_Tag = L"Layer_" + m_vecStaticMesh[iSelect];

	_int iLen_Component = Component_Tag.GetLength();
	TCHAR* pComponentTag = new TCHAR[iLen_Component + 1];
	lstrcpy(pComponentTag, Component_Tag);

	_int	iLen_Layer = Layer_Tag.GetLength();
	m_pLayer_Tag = new TCHAR[iLen_Layer + 1];
	lstrcpy(m_pLayer_Tag, Layer_Tag);



	m_vecComponentTag.push_back(pComponentTag);
	m_vecLayerTag.push_back(m_pLayer_Tag);

	m_pComponent_Tag = pComponentTag;
	m_pGameObject_Tag = L"GameObject_StaticMesh";

	string strtemp;
	strtemp = CT2CA(m_vecStaticMesh[iSelect]);
	string strSave = strtemp;
	CString strMeshName;
	strtemp.pop_back();
	strMeshName = strtemp.c_str();
	strtemp += "/";
	strtemp += strSave + "/";;

	auto& iter_find = m_setStaticMesh.find(m_vecStaticMesh[iSelect]);
	if (iter_find == m_setStaticMesh.end())
	{
		CString cstrTemp;
		cstrTemp = strtemp.c_str();
		if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, pComponentTag))
		{
			if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, pComponentTag,
				CStatic_Mesh::Create(CManagement::GetInstance()->Get_Graphic_Device(),
					L"../Resource/Mesh/StaticMesh/" + cstrTemp, m_vecStaticMesh[iSelect] + L".X"))))
				return;
		}

		m_setStaticMesh.insert(m_vecStaticMesh[iSelect]);
	}
	m_eMeshType = STATIC;
}


void CUnitTool::OnBnClickedCamera_Apply()
{
	UpdateData(TRUE);
	fCameraSpeed = (_float)_ttof(m_strCameraSpeed);
	UpdateData(FALSE);
}


void CUnitTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Radio_Drag[0].GetCheck())
		IsDrag = true;
	else if (m_Radio_Drag[1].GetCheck())
		IsDrag = false;

	if (m_Radio_Deffered[0].GetCheck())
		IsDeffered = true;
	else
		IsDeffered = false;

	m_strRot_X.Format(_T("%f"), m_fRotX);
	m_strRot_Y.Format(_T("%f"), m_fRotY);
	m_strRot_Z.Format(_T("%f"), m_fRotZ);
	if (m_Radio_Rotate[0].GetCheck())
	{
		m_fRotY = 0.f;
		m_fRotZ = 0.f;
		m_strRot_Y.Format(_T("%f"), m_fRotY);
		m_strRot_Z.Format(_T("%f"), m_fRotZ);
	}
	else if (m_Radio_Rotate[1].GetCheck())
	{
		m_fRotX = 0.f;
		m_fRotZ = 0.f;
		m_strRot_X.Format(_T("%f"), m_fRotX);
		m_strRot_Z.Format(_T("%f"), m_fRotZ);
	}
	else if (m_Radio_Rotate[2].GetCheck())
	{
		m_fRotX = 0.f;
		m_fRotY = 0.f;
		m_strRot_Y.Format(_T("%f"), m_fRotY);
		m_strRot_Z.Format(_T("%f"), m_fRotZ);
	}





	CDialogEx::OnMouseMove(nFlags, point);
}


void CUnitTool::OnLButtonDown(UINT nFlags, CPoint point)
{

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CUnitTool::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	int i = 0;
}


void CUnitTool::OnLbnSelchange_Dynamic_Mesh()
{
	_tchar* strName = L"";
	_uint	iSelect = m_ListBox_Dynamic.GetCurSel();

	if (-1 == iSelect)
		return;
	m_ListBox_Input.ResetContent();
	m_vecInsert.clear();
	m_vecInsert.shrink_to_fit();

	m_ListBox_Dynamic.GetText(iSelect, m_cstrMeshName);
	string str = string(CT2CA(m_cstrMeshName));

	CString Component_Tag = L"Component_Mesh_" + m_vecDynamicMesh[iSelect];
	CString Layer_Tag = L"Layer_" + m_vecDynamicMesh[iSelect];

	_int iLen_Component = Component_Tag.GetLength();
	TCHAR* pComponentTag = new TCHAR[iLen_Component + 1];
	lstrcpy(pComponentTag, Component_Tag);

	_int	iLen_Layer = Layer_Tag.GetLength();
	m_pLayer_Tag = new TCHAR[iLen_Layer + 1];
	lstrcpy(m_pLayer_Tag, Layer_Tag);



	m_vecComponentTag.push_back(pComponentTag);
	m_vecLayerTag.push_back(m_pLayer_Tag);

	m_pComponent_Tag = pComponentTag;
	m_pGameObject_Tag = L"GameObject_DynamicMesh";

	string strtemp;
	strtemp = CT2CA(m_vecDynamicMesh[iSelect]);
	string strSave = strtemp;
	CString strMeshName;

	strMeshName = strtemp.c_str();
	strtemp += "/";
	auto& iter_find = m_setDynamicMesh.find(m_vecDynamicMesh[iSelect]);
	if (iter_find == m_setDynamicMesh.end())
	{
		CString cstrTemp;
		cstrTemp = strtemp.c_str();
		if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, pComponentTag))
		{
			if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, pComponentTag,
				CDynamic_Mesh::Create(CManagement::GetInstance()->Get_Graphic_Device(),
					L"../Resource/Mesh/DynamicMesh/" + cstrTemp, m_vecDynamicMesh[iSelect] + L".X"))))
				return;

			if (m_vecPartName.size())
			{
				m_mapPart.insert({ str, m_vecPartName });
				m_vecPartName.clear();
				m_vecMeshName.push_back(str);
			}
		}

		m_setDynamicMesh.insert(m_vecDynamicMesh[iSelect]);
	}
	m_eMeshType = DYNAMIC;


	_tchar* pTag = (TCHAR*)(LPCTSTR)m_cstrMeshName;
	SetUp_PartList(str);
}


void CUnitTool::OnBnClickedUpdate()
{
	UpdateData(TRUE);
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	map<const _tchar*, CLayer*>	mapTemp = *pManagement->Get_MapLayer(SCENEID::SCENE_STATIC);

	for (auto& iter : mapTemp)
	{
		auto& iter1 = iter.second->GetGameObjectList()->begin();
		for (; iter1 != iter.second->GetGameObjectList()->end(); ++iter1)
		{
			if ((*iter1)->GetIsPick())
			{
				CTransform* pTransform = (CTransform*)(*iter1)->Get_ComponentPointer(L"Com_Transform");
				_vec3 vPos = _vec3
				(
					pTransform->Get_StateInfo(STATE_POSITION)->x,
					pTransform->Get_StateInfo(STATE_POSITION)->y,
					pTransform->Get_StateInfo(STATE_POSITION)->z
				);
				_vec3 vSize = pTransform->Get_Scale();
				_vec3 vRotate = pTransform->Get_Rotate();

				m_strPos_X.Format(_T("%f"), vPos.x);
				m_strPos_Y.Format(_T("%f"), vPos.y);
				m_strPos_Z.Format(_T("%f"), vPos.z);

				m_strScale_X.Format(_T("%f"), vSize.x);
				m_strScale_Y.Format(_T("%f"), vSize.y);
				m_strScale_Z.Format(_T("%f"), vSize.z);

				m_strRot_X.Format(_T("%f"), m_fRotX);
				m_strRot_Y.Format(_T("%f"), m_fRotY);
				m_strRot_Z.Format(_T("%f"), m_fRotZ);
			}
		}
	}
	Safe_Release(pManagement);
	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedDelete()
{
	map<const _tchar*, CLayer*>	mapTemp = *CManagement::GetInstance()->Get_MapLayer(SCENE_STATIC);
	for (auto& iter : mapTemp)
	{
		auto& iter1 = iter.second->GetGameObjectList()->begin();
		for (; iter1 != iter.second->GetGameObjectList()->end();)
		{
			if ((*iter1)->GetIsPick())
			{
				Safe_Release(*iter1);
				iter1 = iter.second->GetGameObjectList()->erase(iter1);
			}
			else
				iter1++;
		}
	}
}


void CUnitTool::OnBnClickedPartList_Save()
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


		_uint iDynamicMesh_ListIdx = m_ListBox_Dynamic.GetCurSel();
		if (-1 == iDynamicMesh_ListIdx)
		{
			return;
		}
		CString cstrSelectDynamicList;;
		m_ListBox_Dynamic.GetText(iDynamicMesh_ListIdx, cstrSelectDynamicList);
		_uint iSelectDynamicListLen = cstrSelectDynamicList.GetLength();
		TCHAR* pSelectDyanamicMesh = new TCHAR[iSelectDynamicListLen+1];

		lstrcpy(pSelectDyanamicMesh, cstrSelectDynamicList);

		WriteFile(hFile, &iSelectDynamicListLen, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, pSelectDyanamicMesh, sizeof(TCHAR) * iSelectDynamicListLen,& dwByte,  nullptr);
		int iInsertSize = m_vecInsert.size();
		WriteFile(hFile, &iInsertSize, sizeof(_uint), &dwByte, nullptr);

		for (auto& iter : m_vecInsert)
		{
			_uint iSize = iter.first.length()+1;
			_uint iColliderSize = iter.second;
			char szTemp[MAX_STR] = "";
			strcpy(szTemp, iter.first.c_str());

			WriteFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, iter.first.c_str(), sizeof(char) * iSize, &dwByte, nullptr);
			WriteFile(hFile, &iColliderSize, sizeof(_uint), &dwByte, nullptr);
		}
		

		
		Safe_Delete_Array(pSelectDyanamicMesh);
		CloseHandle(hFile);
	}



}


void CUnitTool::OnBnClickedPartList_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CUnitTool::OnBnClickedPartApply()
{
	UpdateData(TRUE);
	m_fColliderSize = (_float)_ttof(m_strColliderSize);
	UpdateData(FALSE);
}


void CUnitTool::OnLbnSelchangePartList()
{
	_uint	iSelect = m_ListBox_PartList.GetCurSel();
	if (-1 == iSelect)
		return;
	m_iPartListIdx = iSelect;

	CString cstrTemp;
	m_ListBox_PartList.GetText(m_iPartListIdx, cstrTemp);

	map<const _tchar*, CLayer*>	mapLayer= *CManagement::GetInstance()->Get_MapLayer(SCENE_STATIC);
	for (auto& Layer : mapLayer)
	{
		auto& iter_Object = Layer.second->GetGameObjectList()->begin();
		for (; iter_Object != Layer.second->GetGameObjectList()->end(); ++iter_Object)
		{
			if ((*iter_Object)->GetIsPick())
			{
				CComponent* pMesh = (*iter_Object)->Get_ComponentPointer(L"Com_Mesh");
				CTransform* pTransform = (CTransform*)(*iter_Object)->Get_ComponentPointer(L"Com_Transform");
				TCHAR* pMeshName = pMesh->Get_Object_FileName();

				wstring wstrTemp(&pMeshName[0]);
				string strTemp(wstrTemp.begin(), wstrTemp.end());

				string str = string(CT2CA(m_cstrMeshName));

				if (str == strTemp)
				{
					char* st = new char[cstrTemp.GetLength()+1];
					strcpy(st, CT2A(cstrTemp));

					(*iter_Object)->GetBoneMatrix() =  dynamic_cast<CDynamic_Mesh*>(pMesh)->Find_Frame(st);
					Safe_Delete_Array(st);

					(*iter_Object)->Delete_Component(L"Com_Collider_Bone");
					(*iter_Object)->Allocation_Component(pTransform);

				}
			}
		}
	}
}


void CUnitTool::OnBnClickedPartList_Delete()
{
	_uint iDynamicMesh_ListIdx = m_ListBox_Dynamic.GetCurSel();
	if (-1 == iDynamicMesh_ListIdx)
		return;

	
	
	CString cstrSelectDynamicList;;
	m_ListBox_Dynamic.GetText(iDynamicMesh_ListIdx, cstrSelectDynamicList);

	

	_uint iSelectDynamicListLen = cstrSelectDynamicList.GetLength();
	_tchar* pSelectDyanamicMesh = new _tchar[iSelectDynamicListLen + 1];

	lstrcpy(pSelectDyanamicMesh, cstrSelectDynamicList);

	char* szDyanmicMeshName = new char[iSelectDynamicListLen + 1];
	WideCharToMultiByte(CP_ACP, 0, pSelectDyanamicMesh, iSelectDynamicListLen + 1,
		szDyanmicMeshName, iSelectDynamicListLen + 1, NULL, NULL);

	if (-1 == m_iPartListIdx)
		return;
	CString cstrPartList;
	m_ListBox_PartList.GetText(m_iPartListIdx, cstrPartList);


	_uint iSelectPartListLen = cstrPartList.GetLength();
	_tchar* pSelectPartList = new _tchar[iSelectPartListLen + 1];

	lstrcpy(pSelectPartList, cstrPartList);

	char* szPartList = new char[iSelectPartListLen + 1];
	WideCharToMultiByte(CP_ACP, 0, pSelectPartList, iSelectPartListLen + 1,
		szPartList, iSelectPartListLen + 1, NULL, NULL);


	auto iter_find = m_mapPart.begin();
	for (; iter_find != m_mapPart.end(); ++iter_find)
	{
		if (!strcmp(iter_find->first.c_str(), szDyanmicMeshName))
		{
			auto iter = iter_find->second.begin();
			for (; iter != iter_find->second.end();)
			{
				if (!strcmp(szPartList, *iter))
				{
					iter = iter_find->second.erase(iter);
				}
				else ++iter;
			}
		}
	}

	Safe_Delete_Array(szDyanmicMeshName);
	Safe_Delete_Array(pSelectDyanamicMesh);

	Safe_Delete_Array(szPartList);
	Safe_Delete_Array(pSelectPartList);
	m_ListBox_PartList.DeleteString(m_iPartListIdx);

}


void CUnitTool::OnBnClickedInput()
{
	if (-1 == m_iPartListIdx)
		return;
	CString cstrPartList;
	m_ListBox_PartList.GetText(m_iPartListIdx, cstrPartList);

	m_ListBox_Input.AddString(cstrPartList);
	
	string temp = string(CT2CA(cstrPartList));
	m_vecInsert.push_back({ temp , (_uint)m_fColliderSize });


}


void CUnitTool::OnLbnSelchangeInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
