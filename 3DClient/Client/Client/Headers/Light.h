#pragma once
#include "Base.h"
class CLight :
	public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight() = default;
public:
	 D3DLIGHT9* Get_LightInfo()  {return &m_LightInfo;}
public:
	HRESULT Ready_Light(const D3DLIGHT9& LightInfo);
	void Render_Light(LPD3DXEFFECT pEffect);
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	D3DLIGHT9			m_LightInfo;
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightInfo);
	virtual void Free();
};

