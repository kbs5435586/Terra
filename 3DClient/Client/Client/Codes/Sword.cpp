#include "stdafx.h"
#include "..\Headers\Player.h"
#include "..\Headers\Sword.h"
#include "Management.h"

CSword::CSword(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSword::CSword(const CSword& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSword::Ready_Prototype()
{
	return S_OK;
}

HRESULT CSword::Ready_GameObject(void* pArg)
{
	if (nullptr == CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);
	m_pTransformCom->SetUp_Speed(30.f, D3DXToRadian(2000.f));

	m_pTransformCom->SetUp_RotationX(D3DXToRadian(90.f));


	D3DXMatrixIdentity(&m_matIden);

	m_pTransformCom->Set_Parent(&m_matParent);

	return S_OK;
}

_int CSword::Update_GameObject(const _float& fTimeDelta)
{
	m_pPlayerTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform");
	if (nullptr != m_pPlayerTransform)
	{
		m_vPos = *m_pPlayerTransform->Get_StateInfo(STATE::STATE_POSITION);
	}
	return _int();
}

_int CSword::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	m_pHandMatrix = ((CDynamic_Mesh*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC,
		L"Layer_Player", L"Com_Mesh"))->Find_Frame(m_strBack.c_str());
	m_matParent = *m_pHandMatrix * ((CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC,
		L"Layer_Player", L"Com_Transform"))->Get_Matrix();

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_SHADOW, this)))
		return -1;
	m_fBlurTime += fTimeDelta;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_BLUR, this)))
		return -1;
	return _int();
}

void CSword::Render_GameObject()
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

void CSword::Render_GameObject_Shadow()
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

void CSword::Render_GameObject_Blur()
{
	if (m_pShaderCom == nullptr || m_pMeshCom == nullptr)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom_Blur->GetEffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	_ulong dwNumMaterials = m_pMeshCom->Get_NumMaterials();
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMaterials; i++)
	{
		if (FAILED(SetUp_ConstantTable_Blur(pEffect, i)))
			return;

		pEffect->CommitChanges();

		m_pMeshCom->Render_Mesh(i);
	}



	if (m_fBlurTime >= 0.1f)
	{
		_matrix		matView;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
		m_matOldView = matView;
		m_matOldWorld = m_pTransformCom->Get_Mul_Parent_Matrix();
		m_fBlurTime = 0.f;
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSword* pInstance = new CSword(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		return nullptr;
	return pInstance;
}

CGameObject* CSword::Clone_GameObject(void* pArg)
{
	CSword* pInstance = new CSword(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		return nullptr;
	return pInstance;
}

void CSword::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pShaderCom_Shadow);
	Safe_Release(m_pShaderCom_Blur);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNormalTextureCom);
	CGameObject::Free();
}

HRESULT CSword::Ready_Component()
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
	m_pShaderCom_Blur = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Blur");
	if (FAILED(Add_Component(L"Com_Shader_Blur", m_pShaderCom_Blur)))
		return E_FAIL;

	m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_StaticMesh_Sword");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;


	m_pColliderCom = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, m_pMeshCom->Get_LocalTransform(), nullptr, m_pTransformCom->Get_Matrix_Pointer(), &m_matParent));
	if (FAILED(Add_Component(L"Com_Collider_OBB", m_pColliderCom)))
		return E_FAIL;


	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CSword::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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

	return S_OK;
}

HRESULT CSword::SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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

HRESULT CSword::SetUp_ConstantTable_Blur(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetMatrix("g_matOldWorld", &m_matOldWorld);
	pEffect->SetMatrix("g_matOldView", &m_matOldView);



	return S_OK;
}
