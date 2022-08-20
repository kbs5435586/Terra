#include "stdafx.h"
#include "..\Headers\Player.h"
#include "..\Headers\Mini_Shraken.h"
#include "Management.h"

CMini_Shraken::CMini_Shraken(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CMini_Shraken::CMini_Shraken(const CMini_Shraken& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMini_Shraken::Ready_Prototype()
{
	return S_OK;
}

HRESULT CMini_Shraken::Ready_GameObject(void* pArg)
{
	if (nullptr == pArg)
	{
		//return E_FAIL;
	}
	m_vTargetPos = *(_vec3*)pArg;
	m_vFirstPos = m_vTargetPos;
	m_eShirakenType = SHIRAKEN_TYPE::SHIRAKEN_TYPE1;
	if (nullptr == CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))
		return E_FAIL;


	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
	m_pTransformCom->SetUp_Speed(10.f, D3DXToRadian(2000.f));


	m_pPlayerTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform");
	m_vPos = *m_pPlayerTransform->Get_StateInfo(STATE::STATE_POSITION);
	m_vPos.y += 5.f;
	m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &m_vPos);


	return S_OK;
}

_int CMini_Shraken::Update_GameObject(const _float& fTimeDelta)
{
	m_pPlayerTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform");
	m_pTransformCom->Rotation_Y(fTimeDelta);

	m_pTransformCom->Go_ToTarget(&m_vTargetPos, fTimeDelta);
	return _int();
}

_int CMini_Shraken::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	/*if (!m_isFirst)
	{
		m_pTransformCom->SetUp_RotationX(D3DXToRadian(90.f));
		m_isFirst = true;
		m_vPos.y += 1.5f;
		m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &m_vPos);
	}
	_float fLen = 0.f;
	_vec3 vThisPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(STATE::STATE_POSITION);
	vPlayerPos.y += 1.5f;
	_vec3	vLen = vPlayerPos - vThisPos;
	fLen = D3DXVec3Length(&vLen);
	if (!m_isDest)
	{
		if (fLen >= RANGE)
			m_isDest = true;

		_vec3 vLook = *m_pPlayerTransform->Get_StateInfo(STATE::STATE_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);
		_vec3 vTargetPos = vLook * RANGE + m_vPos;
		vTargetPos.y += 1.5f;
		m_pTransformCom->Go_ToTarget(&vTargetPos, fTimeDelta);
	}*/


	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_SHADOW, this)))
		return -1;

	return _int();
}

void CMini_Shraken::Render_GameObject()
{
	if (m_pShaderCom == nullptr || m_pMeshCom == nullptr)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->GetEffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	_ulong dwNumMaterials = m_pMeshCom->Get_NumMaterials();
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMaterials; i++)
	{
		if (FAILED(SetUp_ConstantTable(pEffect, i)))
			return;

		pEffect->CommitChanges();

		m_pMeshCom->Render_Mesh(i);
	}
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	if (nullptr != m_pColliderCom)
		m_pColliderCom->Render_Collider();
}

void CMini_Shraken::Render_GameObject_Shadow()
{
	if (m_pShaderCom == nullptr || m_pMeshCom == nullptr)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom_Shadow->GetEffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	_ulong dwNumMaterials = m_pMeshCom->Get_NumMaterials();
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMaterials; i++)
	{
		if (FAILED(SetUp_ConstantTable(pEffect, i)))
			return;

		pEffect->CommitChanges();

		m_pMeshCom->Render_Mesh(i);
	}
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CMini_Shraken* CMini_Shraken::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMini_Shraken* pInstance = new CMini_Shraken(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		return nullptr;
	return pInstance;
}

CGameObject* CMini_Shraken::Clone_GameObject(void* pArg)
{
	CMini_Shraken* pInstance = new CMini_Shraken(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		return nullptr;
	return pInstance;
}

void CMini_Shraken::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pShaderCom_Shadow);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNormalTextureCom);
	CGameObject::Free();
}

HRESULT CMini_Shraken::Ready_Component()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	pManagement->AddRef();

	m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	m_pShaderCom_Shadow = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Shadow");
	if (FAILED(Add_Component(L"Com_Shader_Shadow", m_pShaderCom_Shadow)))
		return E_FAIL;

	m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_StaticMesh_Shiraken1");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	if (m_eShirakenType == SHIRAKEN_TYPE::SHIRAKEN_TYPE1)
	{
		m_pNormalTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Shiraken1_NormalTex");
		if (FAILED(Add_Component(L"Com_Texture", m_pNormalTextureCom)))
			return E_FAIL;
	}
	else if (m_eShirakenType == SHIRAKEN_TYPE::SHIRAKEN_TYPE2)
	{
		m_pNormalTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Shiraken2_NormalTex");
		if (FAILED(Add_Component(L"Com_Texture", m_pNormalTextureCom)))
			return E_FAIL;
	}
	else if (m_eShirakenType == SHIRAKEN_TYPE::SHIRAKEN_TYPE3)
	{
		m_pNormalTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Shiraken3_NormalTex");
		if (FAILED(Add_Component(L"Com_Texture", m_pNormalTextureCom)))
			return E_FAIL;
	}
	else if (m_eShirakenType == SHIRAKEN_TYPE::SHIRAKEN_TYPE4)
	{
		m_pNormalTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Shiraken4_NormalTex");
		if (FAILED(Add_Component(L"Com_Texture", m_pNormalTextureCom)))
			return E_FAIL;
	}

	m_pTransformCom->SetUp_RotationX(D3DXToRadian(90.f));
	m_pColliderCom = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, m_pMeshCom->Get_LocalTransform(), nullptr, m_pTransformCom->Get_Matrix_Pointer()));
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;


	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CMini_Shraken::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const SUBSETDESC* pSubSet = m_pMeshCom->Get_SubSetDesc(iAttributeID);
	if (nullptr == pSubSet)
		return E_FAIL;

	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pSubSet->Material.MatD3D.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pSubSet->Material.MatD3D.Ambient);
	pEffect->SetFloat("g_fPower", pSubSet->Material.MatD3D.Power);

	pEffect->SetTexture("g_DiffuseTexture", pSubSet->MeshTexture.pDiffuseTexture);
	m_pNormalTextureCom->SetUp_OnShader(pEffect, "g_NormalTexture", 0);

	return S_OK;
}

HRESULT CMini_Shraken::SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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


	return S_OK;
}
