#include "pch.h"
#include "Object_Static.h"
#include "Management.h"

CObject_Static::CObject_Static(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CObject_Static::CObject_Static(const CObject_Static& rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_Static::Ready_Prototype()
{
	return S_OK;
}

HRESULT CObject_Static::Ready_GameObject(void* pArg)
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
	m_IsStatic = true;
	return S_OK;
}

_int CObject_Static::Update_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	m_pTransformCom->Set_PositionY(0.f + m_pTransformCom->Get_Add_PosY());
	return _int();
}

_int CObject_Static::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	_matrix			matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	//m_pTransformCom->Set_StateInfo(STATE_POSITION, (_vec3*)&matView.m[3][0]);

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;
	return _int();
}

void CObject_Static::Render_GameObject()
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
}

HRESULT CObject_Static::Ready_Component(const _tchar* pComponentTag)
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

	m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, pComponentTag);
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CObject_Static::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetInt("g_iPick", m_IsPick);

	D3DLIGHT9* pLightInfo = CLight_Mgr::GetInstance()->Get_LightInfo();
	if (nullptr == pLightInfo)
		return E_FAIL;

	const SUBSETDESC* pSubSet = m_pMeshCom->Get_SubSetDesc(iAttributeID);
	if (nullptr == pSubSet)
		return E_FAIL;

	pEffect->SetTexture("g_DiffuseTexture", pSubSet->MeshTexture.pDiffuseTexture);

	return S_OK;
}


CObject_Static* CObject_Static::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CObject_Static* pInstance = new CObject_Static(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);
	return pInstance;
}

CGameObject* CObject_Static::Clone_GameObject(void* pArg)
{
	CObject_Static* pInstance = new CObject_Static(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CObject_Static::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPickingCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	CGameObject::Free();
}
