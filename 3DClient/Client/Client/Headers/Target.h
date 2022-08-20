#pragma once
#include "Base.h"
class CTarget :
    public CBase
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;
public:
	HRESULT Ready_Target(const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT Release_OnGraphicDev(const _uint& iIndex);
	HRESULT Clear_Target();
public:
	HRESULT Ready_Debug_Buffer(const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);
	HRESULT Render_Debug_Buffer();
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9			m_pTarget_Texture = nullptr;
	LPDIRECT3DSURFACE9			m_pTarget_Surface = nullptr;
	LPDIRECT3DSURFACE9			m_pOld_Surface = nullptr;
private:
	D3DXCOLOR					m_ClearColor;
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
public:
	static CTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
protected:
	virtual void Free();
};

