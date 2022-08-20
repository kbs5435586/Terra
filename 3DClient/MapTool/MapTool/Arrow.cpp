#include "pch.h"
#include "Arrow.h"
#include "Management.h"


CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CArrow::CArrow(const CArrow& rhs)
	: CGameObject(rhs)
{
}

HRESULT CArrow::Ready_Prototype()
{
	return S_OK;
}

HRESULT CArrow::Ready_GameObject(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	_vec3 vPos = *(_vec3*)pArg;
	vPos.y += 1.f;
	m_pTransformCom->SetUp_Speed(5.0f, D3DXToRadian(90.0f));
	m_pTransformCom->Set_StateInfo(STATE_POSITION, &vPos);

	return NOERROR;
}

_int CArrow::Update_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	return _int();
}

_int CArrow::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;
	return _int();
}

void CArrow::Render_GameObject()
{
	if (nullptr == m_pBufferCom)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ContantTable(pEffect)))
		return;


	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CArrow* CArrow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArrow* pInstance = new CArrow(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		return nullptr;
	return pInstance;
}

CGameObject* CArrow::Clone_GameObject(void* pArg)
{
	CArrow* pInstance = new CArrow(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		return nullptr;
	return pInstance;
}

void CArrow::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	CGameObject::Free();
}

HRESULT CArrow::Ready_Component()
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
	if (nullptr == m_pRendererCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Arrow");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pBufferCom = (CBuffer_RcTex*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_Texture");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Default");
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))													
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CArrow::SetUp_ContantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetInt("g_iPick", m_IsPick);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", 0);
	return S_OK;
}

