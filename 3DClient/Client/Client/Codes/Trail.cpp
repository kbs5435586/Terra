#include "stdafx.h"
#include "Trail.h"
#include "Management.h"
#include "Shiraken.h"
#include "Player.h"
#include "Target_Manager.h"

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
	m_fFrame += 60.f * fTimeDelta;
	m_fAccTime += fTimeDelta;
	if (60.f <= m_fFrame)
		m_fFrame = 0.f;

	m_pPlayer = (CPlayer*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Player");


	Create_Trail(fTimeDelta);


	return _int();
}

_int CTrail::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_EFFECT, this)))
		return -1;
	return _int();
}

void CTrail::Render_GameObject_Effect()
{
	if (nullptr == m_pBufferCom)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable_Effect(pEffect)))
		return;


	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	//if (m_pPlayer->GetIsAttack())
	{
		m_pBufferCom->Render_VIBuffer();
	}


	

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
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
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);


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
	m_pBufferCom = (CBuffer_Trail*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_Trail");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;
	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Trail");
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CTrail::SetUp_ConstantTable_Effect(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	//pEffect->SetFloat("g_fAccTime", m_fAccTime);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", 0);
	if (FAILED(m_pRendererCom->Get_TargetManager()->SetUp_OnShader(pEffect, L"Target_Diffuse", "g_PostEffect")))
		return E_FAIL;

	return S_OK;
}

void CTrail::Create_Trail(const _float& fTimeDelta)
{
	CGameObject* pGameObject = CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_Shiraken")->front();
	_vec3 vMax = dynamic_cast<CCollider*>(pGameObject->Get_ComponentPointer(L"Com_Collider_OBB"))->GetMax();
	_vec3 vMin = dynamic_cast<CCollider*>(pGameObject->Get_ComponentPointer(L"Com_Collider_OBB"))->GetMin();

	vMax *= 0.25f;
	vMin *= 0.25f;

	m_matTrail = pGameObject->GetTrailMat();
	m_pTransformCom->Set_Matrix(m_matTrail);

	//_vec3 vUp = *dynamic_cast<CTransform*>(pGameObject->Get_ComponentPointer(L"Com_Transform"))->Get_StateInfo(STATE::STATE_UP);
	//
	//_vec3 vMax = *dynamic_cast<CTransform*>(pGameObject->Get_ComponentPointer(L"Com_Transform"))->Get_StateInfo(STATE::STATE_POSITION) +vUp*2 ;
	//_vec3 vMin=  *dynamic_cast<CTransform*>(pGameObject->Get_ComponentPointer(L"Com_Transform"))->Get_StateInfo(STATE::STATE_POSITION) +vUp *-2;

	D3DXVec3TransformCoord(&vMax, &vMax, &m_matTrail);
	D3DXVec3TransformCoord(&vMin, &vMin, &m_matTrail);

	m_fCreateTime += fTimeDelta;

	if (m_fDuration <= m_fCreateTime)
	{
		dynamic_cast<CBuffer_Trail*>(m_pBufferCom)->Add_NewTrail(vMax, vMin);
		m_fCreateTime = 0.f;
	}



	dynamic_cast<CBuffer_Trail*>(m_pBufferCom)->Update_TrailBuffer(fTimeDelta, m_matTrail);
}
