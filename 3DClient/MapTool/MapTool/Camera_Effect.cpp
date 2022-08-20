#include "pch.h"
#include "Camera_Effect.h"

CCamera_Effect::CCamera_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCamera(pGraphic_Device)
{
}

CCamera_Effect::CCamera_Effect(const CCamera_Effect& rhs)
    : CCamera(rhs)
{
}

HRESULT CCamera_Effect::Ready_Prototype()
{
    if (FAILED(CCamera::Ready_Prototype()))
        return E_FAIL;

    return NOERROR;
}

HRESULT CCamera_Effect::Ready_GameObject(void* pArg)
{
    if (FAILED(CCamera::Ready_GameObject(pArg)))
        return E_FAIL;




    return NOERROR;
}

_int CCamera_Effect::Update_GameObject(const _float& fTimeDelta)
{
    Invalidate_ViewProjMatrix();
    return _int();
}

_int CCamera_Effect::LastUpdate_GameObject(const _float& fTimeDelta)
{
    return _int();
}

void CCamera_Effect::Render_GameObject()
{
}

CCamera_Effect* CCamera_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCamera_Effect* pInstance = new CCamera_Effect(pGraphic_Device);
    if (FAILED(pInstance->Ready_Prototype()))
        Safe_Release(pInstance);
    return pInstance;
}

CGameObject* CCamera_Effect::Clone_GameObject(void* pArg)
{
    CCamera_Effect* pInstance = new CCamera_Effect(*this);

    if (FAILED(pInstance->Ready_GameObject(pArg)))
        Safe_Release(pInstance);
    return pInstance;
}

void CCamera_Effect::Free()
{
    CCamera::Free();
}
