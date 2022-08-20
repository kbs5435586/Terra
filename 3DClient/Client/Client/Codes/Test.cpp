#include "stdafx.h"
#include "..\Headers\Test.h"
#include "Management.h"
CTest::CTest(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTest::CTest(const CTest & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTest::Ready_Prototype()
{
	return S_OK;
}

HRESULT CTest::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->SetUp_Speed(5.0f, D3DXToRadian(90.0f));

	m_pTransformCom->Scaling(1.f, 1.f, 1.f);
	m_pTransformCom->Set_StateInfo(STATE_POSITION, &_vec3(0.0f, 0.f, 0.f));
	return S_OK;
}

_int CTest::Update_GameObject(const _float & fTimeDelta)
{
	const _vec3* pPosition = m_pTransformCom->Get_StateInfo(STATE_POSITION);

	return _int();
}

_int CTest::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

void CTest::Render_GameObject()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	if (FAILED(m_pTextureCom->SetUp_OnGraphicDev()))
		return;
	m_pBufferCom->Render_VIBuffer();
}

HRESULT CTest::Ready_Component()
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;
	m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;
	m_pTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_LOGO, L"Component_Texture_Test");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;
	m_pBufferCom = (CBuffer_RcTex*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;
	Safe_Release(pManagement);
	return S_OK;
}

CTest * CTest::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTest*	pInstance = new CTest(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
	{
		MessageBox(0, L"CBack_Logo Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTest::Clone_GameObject()
{
	CTest*	pInstance = new CTest(*this);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MessageBox(0, L"CBack_Logo Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTest::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	CGameObject::Free();
}
