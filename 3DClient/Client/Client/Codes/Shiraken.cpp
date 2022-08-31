#include "stdafx.h"
#include "..\Headers\Player.h"
#include "..\Headers\Shiraken.h"
#include "Management.h"

CShiraken::CShiraken(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CShiraken::CShiraken(const CShiraken& rhs)
	: CGameObject(rhs)
{
}

HRESULT CShiraken::Ready_Prototype()
{
	return S_OK;
}

HRESULT CShiraken::Ready_GameObject(void* pArg)
{
	if (nullptr == CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);
	m_pTransformCom->Scaling(50.f, 50.f, 0.5f);
	m_pTransformCom->SetUp_Speed(50.f, D3DXToRadian(4000.f));

	m_pTransformCom->SetUp_RotationX(D3DXToRadian(90.f));


	D3DXMatrixIdentity(&m_matIden);
	D3DXMatrixIdentity(&m_matParent);

	m_pTransformCom->Set_Parent(&m_matParent);

	return S_OK;
}

_int CShiraken::Update_GameObject(const _float& fTimeDelta)
{
	m_pPlayerTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC,L"Layer_Player", L"Com_Transform");
	if (nullptr != m_pPlayerTransform)
	{
		m_vPos = *m_pPlayerTransform->Get_StateInfo(STATE::STATE_POSITION);
	}
	
	return _int();
}

_int CShiraken::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	
	if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))->GetIsInputWeapon())
	{
		m_pHandMatrix = ((CDynamic_Mesh*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC,
			L"Layer_Player", L"Com_Mesh"))->Find_Frame(m_strBack.c_str());
	}
	else
	{
		m_pHandMatrix = ((CDynamic_Mesh*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, 
			L"Layer_Player", L"Com_Mesh"))->Find_Frame(m_strRHand.c_str());
	}
	
	if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))->GetIsThrow())
	{
		m_fThrowTime += fTimeDelta;
		if (!m_isFirst)
		{
			m_pTransformCom->SetUp_RotationX(D3DXToRadian(90.f));
			m_isFirst = true;
			m_vPos.y += 3.f;
			m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &m_vPos);
		}
		
		m_pTransformCom->Scaling(0.05f, 0.05f, 0.05f);
		//m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);
		//*m_pHandMatrix = m_matIden;
		m_pTransformCom->Set_Parent(nullptr);
		m_pColliderCom_OBB->GetColliderInfo().pParentMatrix = nullptr;		
		m_pTransformCom->Rotation_Y(fTimeDelta);

		_float fLen = 0.f;
		_vec3 vThisPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
		_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(STATE::STATE_POSITION);
		vPlayerPos.y += 3.f;
		_vec3	vLen = vPlayerPos - vThisPos;
		fLen = D3DXVec3Length(&vLen);

		if (!m_isDest)
		{
			if (!m_isReturn)
			{
				m_fThrowRange = dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))->GetThrowPower();
				_vec3 vLook = *m_pPlayerTransform->Get_StateInfo(STATE::STATE_LOOK);
				D3DXVec3Normalize(&vLook, &vLook);
				m_vTargetPos = vLook * m_fThrowRange + m_vPos;
				m_vTargetPos.y += 1.5f;
				m_isReturn = true;
			}

			if (fLen >= m_fThrowRange)
			{
				m_isDest = true;
				m_fThrowTime = 0.f;
			}
			else if (m_fThrowTime > 2.f)
			{
				m_isDest = true;
				m_fThrowTime = 0.f;
			}
			m_pTransformCom->Go_ToTarget(&m_vTargetPos,fTimeDelta);
		}
		else
		{
			m_pTransformCom->Go_ToTarget(&vPlayerPos, fTimeDelta);

			if (fLen <= 1.f)
			{
				dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))->GetThrowPower() = 0.f;
				dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))->GetIsThrow() = false;
				m_isDest = false;
				m_isReturn = false;
			}

			
		}

		D3DXMatrixIdentity(&m_matTemp);
	}
	else
	{
		m_isFirst = false;
		m_pTransformCom->SetUp_RotationX(D3DXToRadian(0.f));
		_vec3 vPos = {0.f,0.f,0.f };
		 // = { m_pHandMatrix->_41, m_pHandMatrix->_42, m_pHandMatrix->_43 };
		m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &vPos);
		m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);
		m_pTransformCom->Set_Parent(&m_matParent);
		m_pColliderCom_OBB->GetColliderInfo().pParentMatrix = &m_matParent;
		m_matTemp = m_matParent;
	}

	m_matTrail = m_pTransformCom->Get_Matrix() * m_matTemp;
	m_matParent = *m_pHandMatrix * ((CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform"))->Get_Matrix();


	m_fBlurTime += fTimeDelta;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_SHADOW, this)))
		return -1;
	//if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_BLUR, this)))
	//	return -1;
	//if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_EFFECT, this)))
	//	return -1;
	return _int();
}

void CShiraken::Render_GameObject()
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

	if (nullptr != m_pColliderCom_OBB)
		m_pColliderCom_OBB->Render_Collider();
}

void CShiraken::Render_GameObject_Shadow()
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

void CShiraken::Render_GameObject_Blur()
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



	if (m_fBlurTime >= 0.01f)
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

void CShiraken::Render_GameObject_Effect()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom_Shadow)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom_Effect->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	_ulong dwNumMaterials = m_pMeshCom->Get_NumMaterials();
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMaterials; i++)
	{
		if (FAILED(SetUp_ConstantTable_Effect(pEffect, i)))
			return;

		pEffect->CommitChanges();

		m_pMeshCom->Render_Mesh(i);
	}


	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);
}

CShiraken* CShiraken::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShiraken* pInstance = new CShiraken(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		return nullptr;
	return pInstance;
}

CGameObject* CShiraken::Clone_GameObject(void* pArg)
{
	CShiraken* pInstance = new CShiraken(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		return nullptr;
	return pInstance;
}

void CShiraken::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pShaderCom_Shadow);
	Safe_Release(m_pShaderCom_Blur);
	Safe_Release(m_pColliderCom_OBB);
	Safe_Release(m_pTextureCom_Hatch);
	Safe_Release(m_pShaderCom_Effect);
	CGameObject::Free();
}

HRESULT CShiraken::Ready_Component()
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

	m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_StaticMesh_Shiraken1");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pShaderCom_Effect = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader_Effect", m_pShaderCom_Effect)))
		return E_FAIL;

	m_pTextureCom_Hatch = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Hatch");
	if (FAILED(Add_Component(L"Com_Texture_Hatch", m_pTextureCom_Hatch)))
		return E_FAIL;



	m_pColliderCom_OBB = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, m_pMeshCom->Get_LocalTransform(), nullptr, m_pTransformCom->Get_Matrix_Pointer(), &m_matParent));
	if (FAILED(Add_Component(L"Com_Collider_OBB", m_pColliderCom_OBB)))
		return E_FAIL;


	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CShiraken::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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

HRESULT CShiraken::SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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

HRESULT CShiraken::SetUp_ConstantTable_Blur(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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

HRESULT CShiraken::SetUp_ConstantTable_Effect(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;
	_matrix		matLightView, matLightProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pTextureCom_Hatch->SetUp_OnShader(pEffect, "g_DiffuseTexture_0", 0);
	m_pTextureCom_Hatch->SetUp_OnShader(pEffect, "g_DiffuseTexture_1", 1);
	m_pTextureCom_Hatch->SetUp_OnShader(pEffect, "g_DiffuseTexture_2", 2);
	m_pTextureCom_Hatch->SetUp_OnShader(pEffect, "g_DiffuseTexture_3", 3);
	m_pTextureCom_Hatch->SetUp_OnShader(pEffect, "g_DiffuseTexture_4", 4);
	m_pTextureCom_Hatch->SetUp_OnShader(pEffect, "g_DiffuseTexture_5", 5);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	matLightView = CManagement::GetInstance()->GetShadowViewMatrix();
	matLightProj = CManagement::GetInstance()->GetShadowProjMatrix();


	const D3DLIGHT9* pLightInfo = CLight_Mgr::GetInstance()->Get_LightInfo();
	if (nullptr == pLightInfo)
		return E_FAIL;
	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));

	return NOERROR;
}
