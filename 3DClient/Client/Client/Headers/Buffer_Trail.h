#pragma once
#include "VIBuffer.h"

class CBuffer_Trail :
	public CVIBuffer
{
private:
	explicit				CBuffer_Trail(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit				CBuffer_Trail(const CBuffer_Trail& rhs);
	virtual					~CBuffer_Trail() = default;
public:
	HRESULT					Ready_VIBuffer(_uint iCnt, _uint iLerpCnt, _float fLifeTime);
	void					Add_NewTrail(const _vec3& matUpPosition, const _vec3& matDownPosition);
	void					Update_TrailBuffer(const _float& fTimeDelta, const _matrix& matWorld);
	void					Render_VIBuffer();
private:
	void					SplineTrailPosition(VTXTEX* pVertexBuffer, const _uint& iDataIdx, _uint& iIdx, const _matrix& matWorld);
public:
	static CBuffer_Trail*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iCnt, _uint iLerpCnt, _float fLifeTime);
	virtual CComponent*		Clone_Component(void* pArg);
	virtual void			Free();
public:
	void					Trail_AllDetelte();
private:
	vector<TRAIL>			m_vecTrailData = {};
private:
	_float					m_fLifeTime = 0.5f;
	_float					m_fUVRate = 0.f;

private:
	_uint					m_iLerpCnt = 20;
	_uint					m_iCurVtxCnt = 0;
	_uint					m_iCurTriCnt = 0;

};
