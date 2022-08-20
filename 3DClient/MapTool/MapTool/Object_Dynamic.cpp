#include "pch.h"
#include "Object_Dynamic.h"
#include "Management.h"

CObject_Dynamic::CObject_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CObject_Dynamic::CObject_Dynamic(const CObject_Dynamic& rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_Dynamic::Ready_Prototype()
{
	return S_OK;
}

HRESULT CObject_Dynamic::Ready_GameObject(void* pArg)
{
	TEMP tTemp = {};
	if (nullptr != pArg)
	{
		tTemp = *(TEMP*)pArg;
		if (FAILED(Ready_Component(tTemp.lstrComponentTag)))
			return E_FAIL;
		m_pTransformCom->Set_StateInfo(STATE_POSITION, &tTemp.vPos);

		lstrcpy(m_pComponent_Tag, tTemp.lstrComponentTag);
	}
	m_pTransformCom->SetUp_Speed(5.0f, D3DXToRadian(90.0f));
	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);
	m_IsStatic = false;
	m_pMeshCom->Set_AnimationSet(0);
	return S_OK;
}

_int CObject_Dynamic::Update_GameObject(const _float& fTimeDelta)
{
	m_pMeshCom->Play_Animation(fTimeDelta);
	m_pTransformCom->Set_PositionY(0.f + m_pTransformCom->Get_Add_PosY());
	return _int();
}

_int CObject_Dynamic::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;
	return _int();
}

void CObject_Dynamic::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

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
}

HRESULT CObject_Dynamic::Ready_Component(const _tchar* pComponentTag)
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

	m_pPickingCom = (CPicking*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Picking");
	if (FAILED(Add_Component(L"Com_Picking", m_pPickingCom)))
		return E_FAIL;

	m_pMeshCom = (CDynamic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, pComponentTag);
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;
	

	m_pBoneMatrix = m_pMeshCom->Find_Frame("");
	if (m_pBoneMatrix)
	{
		_matrix			matLocalTransform;
		D3DXMatrixScaling(&matLocalTransform, 1000.f, 1000.f, 1000.f);
		m_pColliderCom = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
			&COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, m_pBoneMatrix, m_pTransformCom->Get_Matrix_Pointer()));
		if (FAILED(Add_Component(L"Com_Collider_Bone", m_pColliderCom)))
			return E_FAIL;
	}

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CObject_Dynamic::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetInt("g_iPick", m_IsPick);
	

	const SUBSETDESC* pSubSet = &pMeshContainer->pSubSetDesc[iAttributeID];
	if (nullptr == pSubSet)
		return E_FAIL;
	pEffect->SetTexture("g_DiffuseTexture", pSubSet->MeshTexture.pDiffuseTexture);



	return S_OK;
}

CObject_Dynamic* CObject_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CObject_Dynamic* pInstance = new CObject_Dynamic(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CObject_Dynamic::Clone_GameObject(void* pArg)
{
	CObject_Dynamic* pInstance = new CObject_Dynamic(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CObject_Dynamic::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPickingCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pColliderCom);
	CGameObject::Free();
}
