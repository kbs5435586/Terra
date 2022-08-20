#include "stdafx.h"
#include "Line.h"


CLine::CLine(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CLine::Ready_Line(const _vec3 & vStart, const _vec3 & vEnd)
{
	m_vStart = vStart;
	m_vEnd = vEnd;

	//_vec3	vDir = vEnd - vStart;
	_vec3	vDir = vStart - vEnd;

	m_vNormal = _vec3(vDir.z * -1.f, 0.f, vDir.x);

	D3DXVec3Normalize(&m_vNormal, &m_vNormal);

	return NOERROR;
}

_bool CLine::is_Right(const _vec3 & vPosition)
{
	_vec3		vTargetPos = vPosition;
	vTargetPos.y = 0.f;

	_vec3		vSourDir = vTargetPos - m_vStart;
	D3DXVec3Normalize(&vSourDir, &vSourDir);

	if (0 > D3DXVec3Dot(&vSourDir, &m_vNormal))
		return true;
	if (0 > D3DXVec3Dot(&m_vNormal, &vSourDir))
		return true;

	return _bool(false);;
}

_bool CLine::is_Right(const _vec3& vPosition, _float& fY)
{
	_vec3		vTargetPos = vPosition;
	vTargetPos.y = 0.f;

	_vec3		vSourDir = vTargetPos - m_vStart;
	D3DXVec3Normalize(&vSourDir, &vSourDir);
	

	if (0 > D3DXVec3Dot(&vSourDir, &m_vNormal))
	{
		fY = Compute_PositionY(m_vStart, m_vEnd, vPosition);
		return _bool(true);
	}
	else
	{
		fY = Compute_PositionY(m_vEnd, m_vStart, vPosition);
		return _bool(false);
	}
	return false;
}

_float CLine::Compute_PositionY(const _vec3& fStart, const _vec3& fEnd, const _vec3& fPosition)
{
	_float	fTotalLen = D3DXVec3Length(&(fStart - fEnd));
	_float	fRatioLen = D3DXVec3Length(&(fStart - fPosition));

	_float	fRatio = fRatioLen / fTotalLen;
	_float	fResult = fStart.y * (1 - fRatio) + fEnd.y * fRatio;

	return fResult;
}


CLine * CLine::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3 & vStart, const _vec3 & vEnd)
{
	CLine*	pInstance = new CLine(pGraphic_Device);

	if (FAILED(pInstance->Ready_Line(vStart, vEnd)))
	{
		MessageBox(0, L"CLine Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLine::Free()
{
	Safe_Release(m_pGraphic_Device);
	
}
