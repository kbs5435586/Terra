#include "stdafx.h"
#include "..\Headers\Camera_Light.h"


CCamera_Light::CCamera_Light(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Light::CCamera_Light(const CCamera_Light& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Light::Ready_Prototype()
{
	if (FAILED(CCamera::Ready_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCamera_Light::Ready_GameObject(void* pArg)
{
	if (FAILED(CCamera::Ready_GameObject(pArg)))
		return E_FAIL;

	m_pTransform->SetUp_Speed(50.f, D3DXToRadian(360.f));

	D3DVIEWPORT9			ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);
	m_ptMouse.x = ViewPort.Width >> 1;
	m_ptMouse.y = ViewPort.Height >> 1;

	ClientToScreen(g_hWnd, &m_ptMouse);

	return NOERROR;
}

_int CCamera_Light::Update_GameObject(const _float& fTimeDelta)
{
	_long	MouseMove = 0;


	_matrix		matWorld = m_pTransform->Get_Matrix();
	Invalidate_ViewProjMatrix(true);

	return _int();
}

_int CCamera_Light::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CCamera_Light::Render_GameObject()
{
}

CCamera_Light* CCamera_Light::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Light* pInstance = new CCamera_Light(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);
	return pInstance;
}


CGameObject* CCamera_Light::Clone_GameObject(void* pArg)
{
	CCamera_Light* pInstance = new CCamera_Light(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

void CCamera_Light::Free()
{
	CCamera::Free();
}