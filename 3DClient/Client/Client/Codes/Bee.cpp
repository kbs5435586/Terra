#include "stdafx.h"
#include "Bee.h"
#include "Management.h"

CBee::CBee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBee::CBee(const CBee & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBee::Ready_Prototype()
{
	return S_OK;
}

HRESULT CBee::Ready_GameObject(void * pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		_vec3 vPos = *(_vec3*)pArg;
		m_pTransformCom->Set_StateInfo(STATE_POSITION, &vPos);
	}

	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);
	m_pTransformCom->SetUp_Speed(5.f, D3DXToRadian(90.0f));

	return S_OK;
}

_int CBee::Update_GameObject(const _float & fTimeDelta)
{
	m_pMeshCom->Play_Animation(fTimeDelta*m_fMove);

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	pManagement->AddRef();
	m_fTimeDelta = fTimeDelta;
	m_pPlayerTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform");
	if (nullptr == m_pPlayerTransform)
		return _int();

	//Change_State(fTimeDelta);
	//Collision_Player();
	//End_Loop(fTimeDelta);

	Safe_Release(pManagement);
	return _int();
}

_int CBee::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_SHADOW, this)))
		return -1;

	return _int();
}

void CBee::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom->GetEffectHandle();
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

	//if (nullptr != m_pColliderCom)
	//	m_pColliderCom->Render_Collider();
	//if (nullptr != m_pColliderCom_Attack)
	//	m_pColliderCom_Attack->Render_Collider();

	Safe_Release(pEffect);
}

void CBee::Render_GameObject_Shadow()
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

void CBee::Change_State(const _float & fTimeDelta)
{
	{
		_vec3 vTemp = *m_pPlayerTransform->Get_StateInfo(STATE_POSITION) - *m_pTransformCom->Get_StateInfo(STATE_POSITION);
		_float fLength = D3DXVec3Length(&vTemp);
		if (fLength <= ATTACK_RANGE)
		{
			Attack(fTimeDelta);
		}
		else if (fLength <= 15.f)
		{
			Chase_Player(fTimeDelta, fLength);
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
		case CBee::STATE_IDLE:
			m_pMeshCom->Set_AnimationSet((_int)CBee::STATE_IDLE);
			m_pMeshCom_Shadow->Set_AnimationSet((_int)CBee::STATE_IDLE);
			break;
		case CBee::STATE_WALK:
			m_pMeshCom->Set_AnimationSet((_int)CBee::STATE_WALK);
			m_pMeshCom_Shadow->Set_AnimationSet((_int)CBee::STATE_WALK);
			break;
		case CBee::STATE_ATT:
			m_pMeshCom->Set_AnimationSet((_int)CBee::STATE_ATT);
			m_pMeshCom_Shadow->Set_AnimationSet((_int)CBee::STATE_ATT);
			break;

		}
		if (m_eCurState == m_pMeshCom->Get_CurrentState())
			m_ePreState = m_eCurState;
	}
}

void CBee::End_Loop(const _float & fTimeDelta)
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

void CBee::Collision_Player()
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

CBee * CBee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBee*	pInstance = new CBee(pGraphic_Device);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		MessageBox(0, L"CBee Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBee::Clone_GameObject(void * pArg)
{
	CBee*	pInstance = new CBee(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MessageBox(0, L"CBee Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBee::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPickingCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom_Shadow);
	Safe_Release(m_pMeshCom_Shadow);
	Safe_Release(m_pColliderCom_Attack);
	CGameObject::Free();
}

void CBee::Chase_Player(const _float & fTimeDelta, _float fLenght)
{
	_vec3	vLook = *m_pTransformCom->Get_StateInfo(STATE::STATE_RIGHT);
	vLook *= -1.f;
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

void CBee::IDLE()
{
	m_IsOnce = false;
	m_eCurState = STATE_IDLE;
}

void CBee::Attack(const _float & fTimeDelta)
{
	m_isRotateEnd = false;
	m_IsOnce = false;
	m_eCurState = STATE_::STATE_ATT;
}

void CBee::MeaningLess_Move(const _float & fTimeDelta)
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

		_vec3	vLook = *m_pTransformCom->Get_StateInfo(STATE::STATE_RIGHT);
		vLook *= -1.f;
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

void CBee::Fly(const _float & fTimeDelta)
{
}


HRESULT CBee::Ready_Component()
{
	CManagement*		pManagement = CManagement::GetInstance();
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
	m_pMeshCom = (CDynamic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Mesh_Bee");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;
	m_pMeshCom_Shadow = (CDynamic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Mesh_Bee");
	if (FAILED(Add_Component(L"Com_Mesh_Shadow", m_pMeshCom_Shadow)))
		return E_FAIL;
	m_pPickingCom = (CPicking*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Picking");
	if (FAILED(Add_Component(L"Com_Picking", m_pPickingCom)))
		return E_FAIL;

	_matrix			matLocalTransform;
	D3DXMatrixScaling(&matLocalTransform, 12.f, 8.f, 12.f);
	matLocalTransform._42 = 35.f;
	m_pColliderCom = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, nullptr, m_pTransformCom->Get_Matrix_Pointer()));
	if (FAILED(Add_Component(L"Com_Collider_OBB", m_pColliderCom)))
		return E_FAIL;


	D3DXMatrixScaling(&matLocalTransform, 14.f, 8.f, 14.f);
	matLocalTransform._43 += matLocalTransform._43 - 4.f;
	matLocalTransform._42 = 35.f;
	m_pColliderCom_Attack = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, nullptr, m_pTransformCom->Get_Matrix_Pointer()));
	if (FAILED(Add_Component(L"Com_Collider_Attack", m_pColliderCom_Attack)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CBee::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED * pMeshContainer, const _uint & iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DLIGHT9*	pLightInfo = CLight_Mgr::GetInstance()->Get_LightInfo();
	if (nullptr == pLightInfo)
		return E_FAIL;

	D3DLIGHT9 pLightInfoTemp = *pLightInfo;

	if (m_IsPick)
	{
		pLightInfoTemp.Diffuse.a;
		pLightInfoTemp.Diffuse.r;
		pLightInfoTemp.Diffuse.g = 0.f;
		pLightInfoTemp.Diffuse.b = 0.f;
	}

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfoTemp.Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfoTemp.Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfoTemp.Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfoTemp.Ambient);


	const SUBSETDESC* pSubSet = &pMeshContainer->pSubSetDesc[iAttributeID];
	if (nullptr == pSubSet)
		return E_FAIL;

	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pSubSet->Material.MatD3D.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pSubSet->Material.MatD3D.Ambient);
	pEffect->SetFloat("g_fPower", pSubSet->Material.MatD3D.Power);

	pEffect->SetTexture("g_DiffuseTexture", pSubSet->MeshTexture.pDiffuseTexture);

	return S_OK;
}

HRESULT CBee::SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID)
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
