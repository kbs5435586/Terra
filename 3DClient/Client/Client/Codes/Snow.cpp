#include "stdafx.h"
#include "..\Headers\Snow.h"
#include "Management.h"

CSnow::CSnow(LPDIRECT3DDEVICE9 pGraphic_Device, BoundingBox* pBoundingBox, _uint iNumParticle)
	: CParticleSystem(pGraphic_Device)
{
	m_tBoundingBox = *pBoundingBox;
	m_fSize = 80.f;
	m_iVBSize = 2048;
	m_iVBOffset = 0;
	m_iVBBatchSize = 512;
	m_iNumParticle = iNumParticle;
	
}

CSnow::CSnow(CSnow& rhs)
	: CParticleSystem(rhs)
	, m_iNumParticle(rhs.m_iNumParticle)
{
	m_iParticleSize = 2.f;
	m_vParticleColor = { 1.f,1.f,1.f,1.f };
}

HRESULT CSnow::Ready_Prototype()
{
	return S_OK;
}

HRESULT CSnow::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &_vec3(0, 0, 0));
	//m_pTransformCom->Scaling(8.f, 8.f, 8.f);
	return S_OK;
}

_int CSnow::Update_GameObject(const _float& fTimeDelta)
{
	auto iter = m_lstParticle.begin();
	for (; iter != m_lstParticle.end(); ++iter)
	{
		iter->vPosition += iter->vVelocity * fTimeDelta * 3.f + iter->vAcceleration * fTimeDelta;
		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->isAlive = false;
	}

	Remove_Dead_Particle();

	Active();
	return _int();
}

_int CSnow::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;

	m_pPlayerTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform");
	if (nullptr != m_pPlayerTransform)
	{
		_vec3 vPos = *m_pPlayerTransform->Get_StateInfo(STATE::STATE_POSITION);
		m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &vPos);
	}

	return _int();
}

void CSnow::Render_GameObject()
{
	//m_pTransformCom->SetUp_OnGraphicDev();
	CParticleSystem::Render_GameObject();
}

void CSnow::preRender()
{
	CParticleSystem::Pre_Render();
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CSnow::postRender()
{
	CParticleSystem::Post_Render();
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CSnow::Reset_Particle(PSATTRIBUTE* pAttribute)
{
	static int i = 0;
	pAttribute->isAlive = true;


	pAttribute->vPosition = _vec3(0.f, 20.f, 0.f);


	pAttribute->vVelocity.x = GetRandom_Float(-5.f, 5.0f);
	pAttribute->vVelocity.y = GetRandom_Float(-10.f, -10.f);
	pAttribute->vVelocity.z = GetRandom_Float(-5.f, 5.f);

	pAttribute->vAcceleration.x = GetRandom_Float(-5.f, 5.0f);
	pAttribute->vAcceleration.y = 0.f;
	pAttribute->vAcceleration.z = GetRandom_Float(-5.f, 5.0f);

	pAttribute->vColor = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	pAttribute->fAge = 0.0f;
	pAttribute->fLifeTime = 1.0f;
}

void CSnow::Active()
{
	for (_uint i = 0; i < m_iNumParticle; i++)
		Add_Particle();
}

HRESULT CSnow::Ready_Component(void* pArg)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;
	m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;	
	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Particle");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pManagement);
	return NOERROR;
}

CSnow* CSnow::Create(PDIRECT3DDEVICE9 pGrapic_Device, BoundingBox* pBoundingBox, _uint iNumParticle)
{
	CSnow* pInstance = new CSnow(pGrapic_Device, pBoundingBox, iNumParticle);
	pInstance->Init();
	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX("SnowCreateFailed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSnow::Clone_GameObject(void* pArg)
{
	CSnow* pInstance = new CSnow(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("SnowCreateFailed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSnow::Free()
{
	//Safe_Release(m_pRendererCom);
	//Safe_Release(m_pTransformCom);
	//Safe_Release(m_pShaderCom);
	CParticleSystem::Free();
}
