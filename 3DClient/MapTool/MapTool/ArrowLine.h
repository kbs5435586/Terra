#pragma once
#include "Base.h"
class CArrowLine :
    public CBase
{
private:
	explicit CArrowLine(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CArrowLine() = default;
public:
	HRESULT					Ready_Line(const _vec3 & vStart, const _vec3 & vEnd);
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
private:
	_vec3					m_vStart;
	_vec3					m_vEnd;
public:
	static CArrowLine*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3 & vStart, const _vec3 & vEnd);
	virtual void			Free();
};

