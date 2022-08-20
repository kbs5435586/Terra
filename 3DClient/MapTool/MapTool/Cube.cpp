#include "pch.h"
#include "Cube.h"
#include "Management.h"

CCube::CCube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CCube::CCube(const CCube& rhs)
    : CGameObject(rhs)
{
}

HRESULT CCube::Ready_Prototype()
{
    return S_OK;
}

HRESULT CCube::Ready_GameObject(void* pArg)
{
    if (FAILED(Ready_Component(pArg)))
        return E_FAIL;

	_vec3 vPos = *(_vec3*)pArg;
	m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &vPos);
	//m_pTransformCom->Scaling(_vec3(10.f,10.f,10.f));

    return S_OK;
}

_int CCube::Update_GameObject(const _float& fTimeDelta)
{
    return _int();
}

_int CCube::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;

    return _int();
}

void CCube::Render_GameObject()
{
	if (m_pShaderCom == nullptr || m_pBufferCom == nullptr)
		return;
	LPD3DXEFFECT		pEffect = m_pShaderCom->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable (pEffect)))
		return;

	pEffect->AddRef();

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CCube::Ready_Component(void* pArg)
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

	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Wire");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	m_pPickingCom = (CPicking*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Picking");
	if (FAILED(Add_Component(L"Com_Picking", m_pPickingCom)))
		return E_FAIL;

	m_pBufferCom = (CBuffer_CubeWire*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeWire");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;


	Safe_Release(pManagement);
	return S_OK; 
}

HRESULT CCube::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetInt("g_iPick", m_isNaviPick);


	return S_OK;
}

CCube* CCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCube* pInstance = new CCube(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		return nullptr;
	return pInstance;
}

CGameObject* CCube::Clone_GameObject(void* pArg)
{
	CCube* pInstance = new CCube(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		return nullptr;
	return pInstance;
}

void CCube::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPickingCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
