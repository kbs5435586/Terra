#include "stdafx.h"
#include "PostEffectBuffer.h"
#include "Management.h"
#include "Target_Manager.h"
#include "Target.h"

CPostEffectBuffer::CPostEffectBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPostEffectBuffer::CPostEffectBuffer(const  CPostEffectBuffer& rhs)
	: CGameObject(rhs)
{
}

HRESULT  CPostEffectBuffer::Ready_Prototype()
{
	return S_OK;
}

HRESULT  CPostEffectBuffer::Ready_GameObject(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;

	_matrix matWorld = *(_matrix*)pArg;
	m_pTransformCom->Set_Matrix(matWorld);
	m_pTransformCom->Scaling(1.f, 1.f, 1.f);
	m_pTransformCom->SetUp_RotationY(D3DXToRadian(180.f));
	m_pTransformCom->Rotation_Axis(D3DXToRadian(30.f), &_vec3(1.f, 0.f, 0.f));


	return S_OK;
}

_int  CPostEffectBuffer::Update_GameObject(const _float& fTimeDelta)
{


	return _int();
}

_int  CPostEffectBuffer::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_POSTEFFECT , this)))
		return -1;

	return _int();
}

void CPostEffectBuffer::Render_GameObject()
{
	if (m_pShaderCom == nullptr || m_pBufferCom == nullptr)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->GetEffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);


	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);
}

void CPostEffectBuffer::Render_GameObject_PostEffect()
{
	if (m_pShaderCom == nullptr || m_pBufferCom == nullptr)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->GetEffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(SetUp_ConstantTable_PostEffect(pEffect)))
		return;
	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);
}

CPostEffectBuffer* CPostEffectBuffer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPostEffectBuffer* pInstance = new  CPostEffectBuffer(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		return nullptr;
	return pInstance;
}

CGameObject* CPostEffectBuffer::Clone_GameObject(void* pArg)
{
	CPostEffectBuffer* pInstance = new  CPostEffectBuffer(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		return nullptr;
	return pInstance;
}

void  CPostEffectBuffer::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}

HRESULT  CPostEffectBuffer::Ready_Component()
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

	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	m_pBufferCom = (CBuffer_RcTex*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTex");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;


	Safe_Release(pManagement);
	return S_OK;
}

HRESULT  CPostEffectBuffer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	return S_OK;
}

HRESULT CPostEffectBuffer::SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;
	_matrix		matLightView, matLightProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetTexture("g_DiffuseTexture", m_pRendererCom->Get_TargetManager()->GetTarget(L"Target_PostEffect")->Get_Texture());
	return NOERROR;
}

