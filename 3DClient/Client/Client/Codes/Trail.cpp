#include "stdafx.h"
#include "Trail.h"
#include "Management.h"
#include "Shiraken.h"
#include "Player.h"
#include "Target_Manager.h"
#include "Target.h"
CTrail::CTrail(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTrail::CTrail(const CTrail& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTrail::Ready_Prototype()
{
	return S_OK;
}

HRESULT CTrail::Ready_GameObject(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &_vec3(0.f, 0.f, 10.f));
	return S_OK;
}

_int CTrail::Update_GameObject(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	if (0.5f <= m_fAccTime)
	{
		m_fAccTime = 0.f;
	}
	m_pPlayer = (CPlayer*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Player");




	return _int();
}

_int CTrail::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this)))
		return -1;
	Create_Trail(fTimeDelta);


	//if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_POSTEFFECT, this)))
	//	return -1;
	//Create_Trail_OutLine(fTimeDelta);

	return _int();
}

void CTrail::Render_GameObject()
{
	if (nullptr == m_pBufferCom)
		return;

	if (m_pPlayer->GetIsAttack())
	{
		LPD3DXEFFECT		pEffect = m_pShaderCom->Get_EffectHandle();
		if (nullptr == pEffect)
			return;

		pEffect->AddRef();

		if (FAILED(SetUp_ConstantTable(pEffect)))
			return;


		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);


		m_pBufferCom->Render_VIBuffer();




		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);
	}


}

void CTrail::Render_GameObject_PostEffect()
{
	if (m_pShaderCom_PostEffect == nullptr || m_pBufferCom_Out == nullptr)
		return;

	if (m_pPlayer->GetIsAttack())
	{
		LPD3DXEFFECT		pEffect = m_pShaderCom_PostEffect->Get_EffectHandle();
		if (nullptr == pEffect)
			return;

		pEffect->AddRef();

		if (FAILED(SetUp_ConstantTable_PostEffect(pEffect)))
			return;


		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);


		m_pBufferCom_Out->Render_VIBuffer();




		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);
	};
}

CTrail* CTrail::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrail* pInstance = new CTrail(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTrail::Clone_GameObject(void* pArg)
{
	CTrail* pInstance = new CTrail(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTrail::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pBufferCom_Out);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pShaderCom_PostEffect);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_Mask);


	CGameObject::Free();
}

HRESULT CTrail::Ready_Component()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Red_Trail");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pTextureCom_Mask = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Trail");
	if (FAILED(Add_Component(L"Com_Texture_Mask", m_pTextureCom_Mask)))
		return E_FAIL;

	m_pBufferCom_Out = (CBuffer_Trail*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_Trail");
	if (FAILED(Add_Component(L"Com_Buffer_Out", m_pBufferCom_Out)))
		return E_FAIL;

	m_pBufferCom = (CBuffer_Trail*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_Trail");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Trail");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	m_pShaderCom_PostEffect = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader_PostEffect", m_pShaderCom_PostEffect)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CTrail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_fAccTime", m_fAccTime);

	m_pTextureCom_Mask->SetUp_OnShader(pEffect, "g_DiffuseTexture", 5);
	//m_pTextureCom_Mask->SetUp_OnShader(pEffect, "g_DiffuseTexture",4);


	return S_OK;
}

HRESULT CTrail::SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect)
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


	pEffect->SetTexture("g_DiffuseTexture", m_pRendererCom->Get_TargetManager()->GetTarget(L"Target_PostEffect")->Get_Texture());
	pEffect->SetFloat("g_fAccTime", m_fAccTime);
	return NOERROR;
}

void CTrail::Create_Trail(const _float& fTimeDelta)
{
	CGameObject* pGameObject = CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_Shiraken")->front();

	_vec3 vMax = dynamic_cast<CCollider*>(pGameObject->Get_ComponentPointer(L"Com_Collider_OBB"))->GetMax();
	_vec3 vMin = dynamic_cast<CCollider*>(pGameObject->Get_ComponentPointer(L"Com_Collider_OBB"))->GetMin();




	m_matTrail = pGameObject->GetTrailMat();
	
	m_pTransformCom->Set_Matrix(m_matTrail);

	D3DXVec3TransformCoord(&vMax, &vMax, &m_matTrail);
	D3DXVec3TransformCoord(&vMin, &vMin, &m_matTrail);

	

	dynamic_cast<CBuffer_Trail*>(m_pBufferCom)->Add_NewTrail(vMax, vMin);
	dynamic_cast<CBuffer_Trail*>(m_pBufferCom)->Update_TrailBuffer(fTimeDelta, m_matTrail);
}

void CTrail::Create_Trail_OutLine(const _float& fTimeDelta)
{
	CGameObject* pGameObject = CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_Shiraken")->front();


	_vec3 vMax = dynamic_cast<CCollider*>(pGameObject->Get_ComponentPointer(L"Com_Collider_OBB"))->GetPoint(0);
	_vec3 vMin = dynamic_cast<CCollider*>(pGameObject->Get_ComponentPointer(L"Com_Collider_OBB"))->GetPoint(8);
	
	m_matTrail_Out = pGameObject->GetTrailMat();
	m_pTransformCom->Set_Matrix(m_matTrail_Out);


	D3DXVec3TransformCoord(&vMax, &vMax, &m_matTrail_Out);
	D3DXVec3TransformCoord(&vMin, &vMin, &m_matTrail_Out);

	if (m_pPlayer->GetIsAttack())
	{
 		m_fCreateTime += fTimeDelta;

		if (m_fDuration <= m_fCreateTime)
		{
			dynamic_cast<CBuffer_Trail*>(m_pBufferCom_Out)->Add_NewTrail(vMax, vMin);
			m_fCreateTime = 0.f;
		}

	}
	else
	{
		dynamic_cast<CBuffer_Trail*>(m_pBufferCom_Out)->Trail_AllDetelte();
	}


	dynamic_cast<CBuffer_Trail*>(m_pBufferCom_Out)->Update_TrailBuffer(fTimeDelta, m_matTrail_Out);
}
