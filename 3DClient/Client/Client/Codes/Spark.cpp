#include "stdafx.h"
#include "..\Headers\Spark.h"
#include "Management.h"


CSpark::CSpark(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumParticle)
	: CParticleSystem(pGraphic_Device)
{
	m_fSize = 80.f;
	m_iVBSize = 2048;
	m_iVBOffset = 0;
	m_iVBBatchSize = 512;
	m_iNumParticle = iNumParticle;

}

CSpark::CSpark(CSpark& rhs)
	: CParticleSystem(rhs)
	, m_iNumParticle(rhs.m_iNumParticle)
{
	//m_iParticleSize = 7.f;
	m_iParticleSize = 5.f;
	m_vParticleColor = { 1.f,1.f,0.f,1.f };
	m_isChangeColor = true;
	m_vOriginPos = { 0.f,0.f,0.f };
}

HRESULT CSpark::Ready_Prototype()
{
	return S_OK;
}

HRESULT CSpark::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &_vec3(0, 0, 0));
	m_pTransformCom->SetUp_Speed(50.f, D3DXToRadian(5.f));

	return S_OK;
}

_int CSpark::Update_GameObject(const _float& fTimeDelta)
{

	
	m_fLifeTime += fTimeDelta;
	m_fTimeDelta += fTimeDelta;
	auto iter = m_lstParticle.begin();
	for (; iter != m_lstParticle.end(); ++iter)
	{
		iter->vPosition += iter->vVelocity * fTimeDelta  + iter->vAcceleration * fTimeDelta;

		iter->fAge += fTimeDelta;
		if (iter->fAge > iter->fLifeTime)
			iter->isAlive = false;
	}

	Remove_Dead_Particle();

	Active();

	if (m_fLifeTime >= 1.f)
	{
		return -1;
	}

	return _int();
}

_int CSpark::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this)))
		return -1;



	return _int();
}

void CSpark::Render_GameObject()
{
	CParticleSystem::Render_GameObject();
}

void CSpark::preRender()
{
	CParticleSystem::Pre_Render();
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CSpark::postRender()
{
	CParticleSystem::Post_Render();
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CSpark::Reset_Particle(PSATTRIBUTE* pAttribute)
{
	static int i = 0;
	pAttribute->isAlive = true;

	CGameObject* pGameObject = CManagement::GetInstance()->Get_BackObject(SCENEID::SCENE_STATIC, L"Layer_Player");
	if (nullptr == pGameObject)
		return;

	CTransform* pTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
	if (nullptr == pTransform)
		return;

	_matrix matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	
	_vec3 vRight, vUp, vLook, vPostion;
	vRight = _vec3(matView._11, matView._12, matView._13);
	vUp = _vec3(matView._21, matView._22, matView._23);
	vLook = _vec3(matView._31, matView._32, matView._33);
	vPostion = _vec3(matView._41, matView._42, matView._43);


	vLook = *pTransform->Get_StateInfo(STATE::STATE_LOOK);
	vPostion = *pTransform->Get_StateInfo(STATE::STATE_POSITION);



	pAttribute->vPosition = m_vOriginPos - vLook;
	pAttribute->vVelocity = vLook *7 + vRight * GetRandom_Float(-3.f, 3.f) + vUp * GetRandom_Float(-2.f, 6.f);

	pAttribute->vAcceleration.x = 0.f;
	pAttribute->vAcceleration.y = -0.02f;
	pAttribute->vAcceleration.z = 0.f;
	pAttribute->vAcceleration = -vLook;

	pAttribute->fAge = 0.0f;
	pAttribute->fLifeTime = GetRandom_Float(0.1f, 1.f);

}

void CSpark::Active()
{
	for (_uint i = 0; i < m_iNumParticle; i++)
		Add_Particle();
}

HRESULT CSpark::Ready_Component(void* pArg)
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
	return S_OK;
}

CSpark* CSpark::Create(PDIRECT3DDEVICE9 pGrapic_Device, _uint iNumParticle)
{
	CSpark* pInstance = new CSpark(pGrapic_Device, iNumParticle);
	pInstance->Init();
	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX("SnowCreateFailed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSpark::Clone_GameObject(void* pArg)
{
	CSpark* pInstance = new CSpark(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("SnowCreateFailed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpark::Free()
{
	CParticleSystem::Free();
}
