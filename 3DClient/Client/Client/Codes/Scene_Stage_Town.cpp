#include "stdafx.h"
#include "..\Headers\Scene_Stage_Town.h"
#include "Management.h"
#include "Camera_Debug.h"
#include "Camera_Light.h"
#include "Test.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Player.h"
#include "Beatle.h"
#include "Shiraken.h"
#include "Sword.h"
#include "Light_Mgr.h"
#include "Snow.h"
#include "Spark.h"
#include "Trail.h"
#include "FireSpark.h"
#include "Explosion.h"


CScene_Stage_Town::CScene_Stage_Town(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Stage_Town::Ready_Scene()
{
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;
	if (FAILED(Ready_Layer_BackGround(L"Layer_Terrain")))
		return E_FAIL;
	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;
	if (FAILED(Ready_Layer_Light_Camera(L"Layer_Light_Camera")))
		return E_FAIL;
	if (FAILED(Ready_Layer_SkyBox(L"Layer_SkyBox")))
		return E_FAIL;	
	if (FAILED(Ready_Layer_Shiraken(L"Layer_Shiraken")))
		return E_FAIL;	
	if (FAILED(Ready_Layer_Particle(L"Layer_Particle")))
		return E_FAIL;
	//if (FAILED(Ready_Layer_Test(L"Layer_Test")))
	//	return E_FAIL;


	if (FAILED(Ready_LightInfo()))
		return E_FAIL;
	if (FAILED(Load_Object(L"../../Data/Monster.dat")))
		return E_FAIL;
	
	//if (FAILED(Load_UI(L"../../Data/UI_Test.dat")))
	//	return E_FAIL;
	return S_OK;
}

_int CScene_Stage_Town::Update_Scene(const _float& fTimeDelta)
{
	CManagement::GetInstance()->Collision_Update(fTimeDelta);
	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Stage_Town::LastUpdate_Scene(const _float& fTimeDelta)
{
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Stage_Town::Render_Scene()
{
}

HRESULT CScene_Stage_Town::Ready_Prototype_GameObject()
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_SkyBox", CSkyBox::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Shiraken", CShiraken::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Sword", CSword::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Trail", CTrail::Create(m_pGraphic_Device))))
		return E_FAIL;

	BoundingBox boundingBox;
	boundingBox._min = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);
	boundingBox._max = D3DXVECTOR3(30.0f, 30.0f, 30.0f);
	//if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Particle_Snow",CSnow::Create(m_pGraphic_Device, &boundingBox, 30))))
	//	return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Particle_Spark", CSpark::Create(m_pGraphic_Device, 3))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Particle_FireSparck", CFireSpark::Create(m_pGraphic_Device, 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Particle_Explosion", CExplosion::Create(m_pGraphic_Device, 1))))
		return E_FAIL;

	/*if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Particle_Spark", SCENE_STATIC, L"Layer_Particle")))
		return E_FAIL;
	dynamic_cast<CParticleSystem*>(CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Particle"))->GetOriginPos() = _vec3(-99999.f, 99999999.f, 0.f);*/

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Prototype_Component()
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();








	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();


	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Terrain", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();
	CCamera_Debug* pCameraObject = nullptr;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Camera_Debug", SCENE_STATIC, pLayerTag, nullptr, (CGameObject**)&pCameraObject)))
		return E_FAIL;

	CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CAMERADESC));
	CameraDesc.vEye = _vec3(90.f, 10.f, 90.f);
	CameraDesc.vAt = _vec3(1.f, -1.f, 1.f);
	CameraDesc.vAxisY = _vec3(0.f, 1.f, 0.f);

	PROJDESC		ProjDesc;
	ZeroMemory(&ProjDesc, sizeof(PROJDESC));
	ProjDesc.fFovY = D3DXToRadian(60.0f);
	ProjDesc.fAspect = _float(g_iBackCX) / g_iBackCY;
	ProjDesc.fNear = 0.2f;
	ProjDesc.fFar = 500.0f;

	if (FAILED(pCameraObject->SetUp_CameraProjDesc(CameraDesc, ProjDesc)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Layer_Light_Camera(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();
	CCamera_Debug* pCameraObject = nullptr;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Camera_Light", SCENE_STATIC, pLayerTag, nullptr, (CGameObject**)&pCameraObject)))
		return E_FAIL;

	CAMERADESC		tCameraDesc;
	ZeroMemory(&tCameraDesc, sizeof(CAMERADESC));
	tCameraDesc.vEye = _vec3(-10000.f, 10000.f, -10000.f);
	tCameraDesc.vAt = _vec3(1.f, -1.f, 1.f);
	tCameraDesc.vAxisY = _vec3(0.f, 1.f, 0.f);

	PROJDESC		tProjDesc;
	ZeroMemory(&tProjDesc, sizeof(tProjDesc));
	tProjDesc.fFovY = D3DXToRadian(60.f);
	tProjDesc.fAspect = _float(g_iBackCX) / g_iBackCY;
	tProjDesc.fNear = 0.2f;
	tProjDesc.fFar = 500.0f;

	if (FAILED(pCameraObject->SetUp_CameraProjDesc(tCameraDesc, tProjDesc, true)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Layer_SkyBox(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_SkyBox", SCENE_STATIC, pLayerTag)))
		return E_FAIL;
	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Popori", SCENE_STATIC, pLayerTag)))
		return E_FAIL;
	dynamic_cast<CPlayer*>(pManagement->Get_GameObject(SCENE_STATIC, pLayerTag, 0))->GetCurScene() = SCENE_STATIC;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Layer_Shiraken(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Shiraken", SCENE_STATIC, pLayerTag)))
		return E_FAIL;
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Sword", SCENE_STATIC, pLayerTag)))
		return E_FAIL;	
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Trail", SCENE_STATIC, L"Layer_Trail")))
		return E_FAIL;
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Layer_Particle(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	//if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Particle_Spark", SCENE_STATIC, pLayerTag)))
	//	return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_Layer_Test(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Beatle", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Stage_Town::Load_Object(const _tchar* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

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

				lstrcpy(pName, szName);
				lstrcpy(pLayerTag, szLayerTag);
				lstrcpy(pComrTag, szComTag);

				CScene::Remove_End(pLayerTag);


				CString strDefaultPath = L"../../Resource/Mesh/StaticMesh/";
				CString strPath;
				CString strFileName;
				CString strToint;
				CString strSlash = L"/";
				strToint.Format(_T("%d"), iNum);

				strDefaultPath += (szMeshName + strSlash);
				strFileName = lstrcat(szMeshName, strToint);

				if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, pComrTag))
				{
					//GameObject_SKHouse
					if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, pComrTag,
						CStatic_Mesh::Create(m_pGraphic_Device,
							strDefaultPath + strFileName + strSlash, strFileName + L".X"))))
						return E_FAIL;

				}
				if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(pName, SCENEID::SCENE_STATIC, pLayerTag, pComrTag)))
					return E_FAIL;

				ReadFile(hFile, mat, sizeof(_matrix), &dwByte, nullptr);
				CTransform* pTransform = (CTransform*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, pLayerTag)->Get_ComponentPointer(L"Com_Transform");
				pTransform->Set_Matrix(mat);

				ReadFile(hFile, (void*)&fAdd_PosY, sizeof(_float), &dwByte, nullptr);
				pTransform->Set_Add_PosY(fAdd_PosY);

				m_vecComponentTag.push_back(pName);
				m_vecComponentTag.push_back(pLayerTag);
				m_vecComponentTag.push_back(pComrTag);
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



				lstrcpy(pName, szName);
				lstrcpy(pLayerTag, szLayerTag);
				lstrcpy(pComrTag, szComTag);
	

				CString strDefaultPath = L"../../Resource/Mesh/DynamicMesh/";
				CString strSlash = L"/";
				strDefaultPath += (szMeshName);

				_bool	isTemp = false;
				_tchar* temp = lstrcat(szMeshName, L".X");
				if (!CManagement::GetInstance()->IsComponent(SCENE_STATIC, pComrTag))
				{
					if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_STATIC, pComrTag,
						CDynamic_Mesh::Create(m_pGraphic_Device, strDefaultPath + strSlash, temp))))
						return E_FAIL;

					isTemp = true;
				}

				if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(szName, SCENEID::SCENE_STATIC, L"Layer_Monster", &isTemp)))
					return E_FAIL;
				ReadFile(hFile, mat, sizeof(_matrix), &dwByte, nullptr);
				CTransform* pTransform = (CTransform*)CManagement::GetInstance()->Get_BackObject(SCENEID::SCENE_STATIC, L"Layer_Monster")->Get_ComponentPointer(L"Com_Transform");
				pTransform->Set_Matrix(mat);

				ReadFile(hFile, (void*)&fAdd_PosY, sizeof(_float), &dwByte, nullptr);
				pTransform->Set_Add_PosY(fAdd_PosY);



				m_vecComponentTag.push_back(pName);
				m_vecComponentTag.push_back(pLayerTag);
				m_vecComponentTag.push_back(pComrTag);
				//m_vecComponentTag.push_back(pMeshTag);
			}


			
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CScene_Stage_Town::Load_NaviMesh(const _tchar* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	_vec3		vPoint = {};
	DWORD dwByte = 0;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	CNavi* pNavi = (CNavi*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Navi");
	if (nullptr == pNavi)
		return E_FAIL;

	pNavi->GetTotalPos().clear();
	//pNavi->GetTotalPos().shrink_to_fit();
	//while (TRUE)
	//{
	//	ReadFile(hFile, &vPoint, sizeof(_vec3), &dwByte, nullptr);
	//	if (dwByte == 0)
	//		break;
	//	pNavi->GetTotalPos().push_back(vPoint);
	//}

	pNavi->Set_CellVector(pNavi->GetTotalPos());
	Safe_Release(pManagement);
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CScene_Stage_Town::Load_UI(const _tchar* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	TCHAR	szGameObject_Tag[MAX_STR] = L"";
	TCHAR	szComponentTag[MAX_STR] = L"";
	DWORD dwByte = 0;

	_int iLength_Component_Tag = 0;
	_int iLength_GameObject_Tag = 0;

	_float fX = 0.f;
	_float fY = 0.f;
	_float fSizeX = 0.f;
	_float fSizeY = 0.f;


	while (TRUE)
	{
		//Component_Tag
		ReadFile(hFile, (void*)&iLength_Component_Tag, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szComponentTag, sizeof(TCHAR) * iLength_Component_Tag, &dwByte, nullptr);
		//GameObject_Tag
		ReadFile(hFile, (void*)&iLength_GameObject_Tag, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szGameObject_Tag, sizeof(TCHAR) * iLength_GameObject_Tag, &dwByte, nullptr);
		//UI_Position
		ReadFile(hFile, (void*)&fX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, (void*)&fY, sizeof(_float), &dwByte, nullptr);
		//UI_Size
		ReadFile(hFile, (void*)&fSizeX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, (void*)&fSizeY, sizeof(_float), &dwByte, nullptr);

		if (dwByte == 0)
			break;


		if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(szGameObject_Tag, SCENE_STATIC, L"Layer_UI")))
			return E_FAIL;

		CGameObject* pGameObject = CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_UI");
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->Get_FX() = fX;
		pGameObject->Get_FY() = fY;
		pGameObject->Get_SizeX() = fSizeX;
		pGameObject->Get_SizeY() = fSizeY;

	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CScene_Stage_Town::Ready_LightInfo()
{
	CLight_Mgr* pLight_Manager = CLight_Mgr::GetInstance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	pLight_Manager->AddRef();

	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);

	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(pLight_Manager->Add_LightInfo(m_pGraphic_Device, LightInfo)))
		return E_FAIL;


	Safe_Release(pLight_Manager);

	return S_OK;
}

CScene_Stage_Town* CScene_Stage_Town::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Stage_Town* pInstance = new CScene_Stage_Town(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);
	return pInstance;
}

void CScene_Stage_Town::Free()
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return;

	pManagement->AddRef();

	pManagement->Clear_Layers(SCENE_STAGE_FIELD);

	for (auto& iter : m_vecComponentTag)
		Safe_Delete_Array(iter);

	Safe_Release(pManagement);

	CScene::Free();
}
