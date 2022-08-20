#include "stdafx.h"
#include "Mantis.h"
#include "Management.h"

CMantis::CMantis(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CMantis::CMantis(const CMantis& rhs)
	: CGameObject(rhs)
{
	for (auto& iter : rhs.m_vecPartList)
	{
		m_vecPartList.push_back(iter);
	}
}

HRESULT CMantis::Ready_Prototype()
{
	HANDLE hFile = CreateFile(L"../../Data/PartList/Mantis_PartList.dat", GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	DWORD	dwByte = 0;
	while (TRUE)
	{
		int iSize = 0;
		ReadFile(hFile, &iSize, sizeof(int), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		_tchar pMeshName[MAX_STR] = L"";
		ReadFile(hFile, pMeshName, sizeof(TCHAR) * iSize, &dwByte, nullptr);

		_uint iInsertSize = 0;
		ReadFile(hFile, &iInsertSize, sizeof(int), &dwByte, nullptr);


		for (_uint i = 0; i < iInsertSize; ++i)
		{
			_uint		iSize = 0;
			_uint		iColliderSize = 0;
			char pMeshName[MAX_STR] = "";
			ReadFile(hFile, &iSize, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, pMeshName, sizeof(char) * iSize, &dwByte, nullptr);
			ReadFile(hFile, &iColliderSize, sizeof(int), &dwByte, nullptr);

			string strPartName = pMeshName;
			m_vecPartList.push_back({ strPartName, iColliderSize });
		}


	}
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CMantis::Ready_GameObject(void* pArg)
{

 	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet((_int)STATE_::STATE_IDLE);
	m_pMeshCom_Shadow->Set_AnimationSet((_int)STATE_::STATE_IDLE);

	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);
	m_pTransformCom->SetUp_Speed(5.f, D3DXToRadian(30.f));
	return S_OK;
}

_int CMantis::Update_GameObject(const _float& fTimeDelta)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;
	pManagement->AddRef();


	m_fTimeDelta = fTimeDelta;
	//CGameObject::Obb_Collision(m_pTransformCom, 1.f);



	m_pPlayerTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform");
	if (nullptr == m_pPlayerTransform)
		return _int();

	/*Change_State(fTimeDelta);
	Collision_Player();
	End_Loop(fTimeDelta);*/

	Safe_Release(pManagement);
	return _int();
}

_int CMantis::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_SHADOW, this)))
		return -1;
	return _int();
}


void CMantis::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return;
	LPD3DXEFFECT		pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();
	m_pMeshCom->Play_Animation(m_fTimeDelta * m_fMove);
	_ulong dwNumMeshContainer = m_pMeshCom->Get_NumMeshContainer();

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMeshContainer; i++)
	{
		m_pMeshCom->Update_Skinning(i);

		const D3DXMESHCONTAINER_DERIVED* pMeshContainer = m_pMeshCom->Get_MeshContainer(i);

		for (size_t j = 0; j < pMeshContainer->NumMaterials; ++j)
		{
			if (FAILED(SetUp_ConstantTable(pEffect, pMeshContainer, j)))
				return;

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);
		}

	}

	pEffect->EndPass();

	pEffect->End();


	Safe_Release(pEffect);

	if (nullptr != m_pColliderCom)
		m_pColliderCom->Render_Collider();
	if (nullptr != m_pColliderCom_Attack)
		m_pColliderCom_Attack->Render_Collider();

	for (auto& iter : m_vecCollider_Bone)
	{
		if (nullptr != iter)
			iter->Render_Collider();
	}
}

void CMantis::Render_GameObject_Shadow()
{
	if (nullptr == m_pMeshCom_Shadow ||
		nullptr == m_pShaderCom_Shadow)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom_Shadow->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();
	m_pMeshCom_Shadow->Play_Animation(m_fTimeDelta * m_fMove);
	_ulong dwNumMeshContainer = m_pMeshCom_Shadow->Get_NumMeshContainer();

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMeshContainer; i++)
	{
		m_pMeshCom_Shadow->Update_Skinning(i);

		const D3DXMESHCONTAINER_DERIVED* pMeshContainer = m_pMeshCom_Shadow->Get_MeshContainer(i);

		for (size_t j = 0; j < pMeshContainer->NumMaterials; ++j)
		{
			if (FAILED(SetUp_ConstantTable_Shadow(pEffect, pMeshContainer, j)))
				return;

			pEffect->CommitChanges();

			m_pMeshCom_Shadow->Render_Mesh(i, j);
		}

	}

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);
}

CMantis* CMantis::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMantis* pInstance = new CMantis(pGraphic_Device);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		MessageBox(0, L"CMantis Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMantis::Clone_GameObject(void* pArg)
{
	CMantis* pInstance = new CMantis(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MessageBox(0, L"CMantis Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMantis::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pShaderCom_Shadow);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pMeshCom_Shadow);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPickingCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pColliderCom_Attack);
	for (auto& iter : m_vecCollider_Bone)
	{
		Safe_Release(iter);
	}

	CGameObject::Free();
}

void CMantis::Change_State(const _float& fTimeDelta)
{
	{
		_vec3 vTemp = *m_pPlayerTransform->Get_StateInfo(STATE_POSITION) - *m_pTransformCom->Get_StateInfo(STATE_POSITION);
		_float fLength = D3DXVec3Length(&vTemp);;
		if (fLength <= ATTACK_RANGE)
		{
			m_iAttMotionIdx = rand() % 2;
			if (m_iAttMotionIdx)
			{
				if(m_pMeshCom->Get_EndLoop())
					Attack1(fTimeDelta);
			}
			else
			{
				if (m_pMeshCom->Get_EndLoop())
					Attack3(fTimeDelta);
			}
		}
		else if (fLength <= 15.f)
		{
			Chase_Player(fTimeDelta);
		}
		else
		{
			Fly(fTimeDelta);
			MeaningLess_Move(fTimeDelta);
		}

	}

	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE_::STATE_IDLE:
			m_pMeshCom->Set_AnimationSet((_int)STATE_::STATE_IDLE);
			m_pMeshCom_Shadow->Set_AnimationSet((_int)STATE_::STATE_IDLE);
			break;
		case STATE_::STATE_WALK:
			m_pMeshCom->Set_AnimationSet((_int)STATE_::STATE_WALK);
			m_pMeshCom_Shadow->Set_AnimationSet((_int)STATE_::STATE_WALK);
			break;
		case STATE_::STATE_ATT1:
			m_pMeshCom->Set_AnimationSet((_int)STATE_::STATE_ATT1);
			m_pMeshCom_Shadow->Set_AnimationSet((_int)STATE_::STATE_ATT1);
			break;
		case STATE_::STATE_ATT2:
			m_pMeshCom->Set_AnimationSet((_int)STATE_::STATE_ATT2);
			m_pMeshCom_Shadow->Set_AnimationSet((_int)STATE_::STATE_ATT2);
			break;
		case STATE_::STATE_ATT3:
			m_pMeshCom->Set_AnimationSet((_int)STATE_::STATE_ATT3);
			m_pMeshCom_Shadow->Set_AnimationSet((_int)STATE_::STATE_ATT3);
			break;

		}
		if (m_eCurState == m_pMeshCom->Get_CurrentState())
			m_ePreState = m_eCurState;
	}
}

void CMantis::End_Loop(const _float& fTimeDelta)
{
	if (m_pMeshCom->Get_EndLoop() && m_IsOnce)
	{
		m_fMove = 0.f;

	}
	else
	{
		m_fMove = 1.f;
	}
}

void CMantis::Collision_Player()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();
	if (m_pColliderCom->Collision_OBB((CCollider*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Collider_OBB")))
	{
		pManagement->Get_GameObject(SCENE_STATIC, L"Layer_Player", 0)->GetOBBCollision() = true;
	}
	Safe_Release(pManagement);
}

void CMantis::Chase_Player(const _float& fTimeDelta)
{
	_vec3	vLook = *m_pTransformCom->Get_StateInfo(STATE::STATE_LOOK);

	_vec3	vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
	_vec3	vTargetPos = *m_pPlayerTransform->Get_StateInfo(STATE_POSITION);

	vTargetPos.y = 0.f;
	vPos.y = 0.f;

	_vec3	vP_M = vTargetPos - vPos;
	D3DXVec3Normalize(&vP_M, &vP_M);

	_vec3	vTempLook = vLook;
	vTempLook.y = 0.f;
	D3DXVec3Normalize(&vTempLook, &vTempLook);
	m_fRotateDot = D3DXVec3Dot(&vTempLook, &vP_M);

	D3DXVec3Cross(&m_vRotateCross, &vTempLook, &vP_M);

	if (!m_isRotateEnd)
	{
		if (m_vRotateCross.y >= 0.f)
		{
			m_eRotate = ROTATE_DIR::DIR_RIGHT;
		}
		else
		{
			m_eRotate = ROTATE_DIR::DIR_LEFT;
		}
		m_isRotateEnd = true;
	}




	if (!m_isRotateFinish)
	{
		if (m_fRotateDot <= 1.01f && m_fRotateDot >= 0.99f)
		{
			m_isRotateFinish = true;
		}
		else
		{
			if (m_eRotate == DIR_LEFT)
				m_pTransformCom->Rotation_Y(-fTimeDelta);
			else if (m_eRotate == DIR_RIGHT)
				m_pTransformCom->Rotation_Y(fTimeDelta);
		}

	}
	else
	{
		if (m_fRotateDot >= 1.01f || m_fRotateDot <= 0.99f)
		{
			m_isRotateFinish = false;
			m_isRotateEnd = false;
			return;
		}
		_float	fLen = 0.f;
		_vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);

		fLen = D3DXVec3Length(&(vPos - vTargetPos));
		if (fLen >= ATTACK_RANGE)
		{
			m_pTransformCom->Go_ToTarget(&vTargetPos, fTimeDelta);

		}
		else
		{
			m_isRotateEnd = false;
			m_isRotateFinish = false;
			m_isDest = false;
		}
	}
	m_IsOnce = false;
	m_eCurState = STATE_::STATE_WALK;
}

void CMantis::IDLE()
{
	m_IsOnce = false;
	m_eCurState = STATE_::STATE_IDLE;
}

void CMantis::Attack1(const _float& fTimeDelta)
{

	m_isRotateEnd = false;
	m_IsOnce = false;
	m_eCurState = STATE_::STATE_ATT1;
}

void CMantis::Attack3(const _float& fTimeDelta)
{
	m_isRotateEnd = false;
	m_IsOnce = false;
	m_eCurState = STATE_::STATE_ATT3;
}

void CMantis::MeaningLess_Move(const _float& fTimeDelta)
{
	if (m_iFlyCnt < CNT_FLY)
	{
		if (nullptr == m_pTransformCom)
			return;
		if (!m_isDest)
		{
			_int	iMoveX = rand() % 50 + 1;
			_int	iMoveZ = rand() % 50 + 1;

			m_vDest = { (_float)iMoveX, 0.f, (_float)iMoveZ };
			m_isDest = true;
		}

		_vec3	vLook = *m_pTransformCom->Get_StateInfo(STATE::STATE_LOOK);
		_vec3	vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
		_vec3	vTargetPos = m_vDest;

		vTargetPos.y = 0.f;
		vPos.y = 0.f;

		_vec3	vP_M = vTargetPos - vPos;
		D3DXVec3Normalize(&vP_M, &vP_M);

		_vec3	vTempLook = vLook;
		vTempLook.y = 0.f;
		D3DXVec3Normalize(&vTempLook, &vTempLook);
		m_fRotateDot = D3DXVec3Dot(&vTempLook, &vP_M);

		D3DXVec3Cross(&m_vRotateCross, &vTempLook, &vP_M);

		if (!m_isRotateEnd)
		{
			if (m_vRotateCross.y > 0.f)
			{
				m_eRotate = ROTATE_DIR::DIR_RIGHT;
			}
			else
			{
				m_eRotate = ROTATE_DIR::DIR_LEFT;
			}
			m_isRotateEnd = true;
		}

		if (m_fRotateDot <= 1.01f && m_fRotateDot >= 0.99f)
		{
			m_isRotateFinish = true;
		}
		else
		{
			if (m_eRotate == ROTATE_DIR::DIR_LEFT)
				m_pTransformCom->Rotation_Y(-fTimeDelta);
			else if (m_eRotate == ROTATE_DIR::DIR_RIGHT)
				m_pTransformCom->Rotation_Y(fTimeDelta);
		}

		if (m_isRotateFinish)
		{
			_float	fLen = 0.f;
			_vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
			_vec3 vTargetPos = m_vDest;

			fLen = D3DXVec3Length(&(vPos - vTargetPos));
			if (fLen >= 3.f)
			{
				m_pTransformCom->Go_ToTarget(&vTargetPos, fTimeDelta);

			}
			else
			{
				m_isRotateEnd = false;
				m_isRotateFinish = false;
				m_isDest = false;
			}
		}
		m_IsOnce = false;
		m_eCurState = STATE_::STATE_WALK;
	}
}

void CMantis::Fly(const _float& fTimeDelta)
{
}

HRESULT CMantis::Ready_Component()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For.Com_Transform
	m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	m_pShaderCom_Shadow = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Shadow");
	if (FAILED(Add_Component(L"Com_Shader_Shadow", m_pShaderCom_Shadow)))
		return E_FAIL;

	// For.Com_Mesh
	m_pMeshCom = (CDynamic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Mesh_Mantis");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pMeshCom_Shadow = (CDynamic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Mesh_Mantis");
	if (FAILED(Add_Component(L"Com_Mesh_Shadow", m_pMeshCom_Shadow)))
		return E_FAIL;

	m_pPickingCom = (CPicking*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Picking");
	if (FAILED(Add_Component(L"Com_Picking", m_pPickingCom)))
		return E_FAIL;

	_matrix			matLocalTransform;
	D3DXMatrixScaling(&matLocalTransform, 80.f, 80.f, 80.f);
	matLocalTransform._42 = 40.f;
	m_pColliderCom = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, nullptr, m_pTransformCom->Get_Matrix_Pointer()));
	if (FAILED(Add_Component(L"Com_Collider_OBB", m_pColliderCom)))
		return E_FAIL;
	
	D3DXMatrixScaling(&matLocalTransform, 90.f, 80.f, 90.f);
	matLocalTransform._43 += matLocalTransform._43 + 20.f;
	matLocalTransform._42 = 40.f;
	m_pColliderCom_Attack = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, nullptr, m_pTransformCom->Get_Matrix_Pointer()));
	if (FAILED(Add_Component(L"Com_Collider_Attack", m_pColliderCom_Attack)))
		return E_FAIL;

	for (_uint i = 0; i < m_vecPartList.size(); ++i)
	{
		const _matrix* pBoneMatrix = m_pMeshCom->Find_Frame(m_vecPartList[i].first.c_str());
		if (nullptr != pBoneMatrix)
		{
			_matrix			matLocalTransform;
			D3DXMatrixScaling(&matLocalTransform, m_vecPartList[i].second,
				m_vecPartList[i].second, m_vecPartList[i].second);

			CCollider* pCollider = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
				&COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, pBoneMatrix, m_pTransformCom->Get_Matrix_Pointer()));
			TCHAR szRet[128] = {};
			_stprintf_s(szRet, _countof(szRet), _T("%d"), i);
			_tchar szComponentName[MAX_STR] = L"Com_Collider_Bone";
			lstrcat(szComponentName, szRet);
			_uint iComponentLen = lstrlen(szComponentName);

			_tchar* pComponentTag = new _tchar[iComponentLen + 1];
			lstrcpy(pComponentTag, szComponentName);

			if (FAILED(Add_Component(pComponentTag, pCollider)))
				return E_FAIL;
			m_vecPartName.push_back(pComponentTag);
			m_vecBoneMatrix.push_back(pBoneMatrix);
			m_vecCollider_Bone.push_back(pCollider);
		}

	}



	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CMantis::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	const SUBSETDESC* pSubSet = &pMeshContainer->pSubSetDesc[iAttributeID];
	if (nullptr == pSubSet)
		return E_FAIL;

	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pSubSet->Material.MatD3D.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pSubSet->Material.MatD3D.Ambient);
	pEffect->SetFloat("g_fPower", pSubSet->Material.MatD3D.Power);

	pEffect->SetTexture("g_DiffuseTexture", pSubSet->MeshTexture.pDiffuseTexture);

	return S_OK;
}

HRESULT CMantis::SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;
	_matrix		matLightView, matLightProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	matLightView = CManagement::GetInstance()->GetShadowViewMatrix();
	matLightProj = CManagement::GetInstance()->GetShadowProjMatrix();

	pEffect->SetMatrix("g_matShadowView", &matLightView);
	pEffect->SetMatrix("g_matShadowProj", &matLightProj);


	return NOERROR;
}
