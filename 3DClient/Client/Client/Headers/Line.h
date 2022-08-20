#pragma once
#include "Base.h"
class CLine :
	public CBase
{
private:
	explicit CLine(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLine() = default;
public:
	HRESULT					Ready_Line(const _vec3& vStart, const _vec3& vEnd);
public:
	_bool					is_Right(const _vec3& vPosition);
	_bool					is_Right(const _vec3& vPosition, _float& fY);
	_vec3					 Get_Normal() { return m_vNormal; }
private:
	_float					Compute_PositionY(const _vec3& fStart, const _vec3& fEnd, const _vec3& fPosition);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
private:
	_vec3					m_vStart;
	_vec3					m_vEnd;
	_vec3					m_vNormal;
private:
	_float					m_fY = 0.f;
public:
	static CLine*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3& vStart, const _vec3& vEnd);
	virtual void			Free();

};

