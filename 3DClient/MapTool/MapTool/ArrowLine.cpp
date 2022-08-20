#include "pch.h"
#include "ArrowLine.h"

CArrowLine::CArrowLine(LPDIRECT3DDEVICE9 pGraphic_Device)
{
}

HRESULT CArrowLine::Ready_Line(const _vec3& vStart, const _vec3& vEnd)
{

    return S_OK;
}

CArrowLine* CArrowLine::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3& vStart, const _vec3& vEnd)
{
    CArrowLine* pInstance = new CArrowLine(pGraphic_Device);
    if (FAILED(pInstance->Ready_Line(vStart, vEnd)))
    {
        Safe_Release(pInstance); 
    }
    return pInstance; 
}

void CArrowLine::Free()
{
}
