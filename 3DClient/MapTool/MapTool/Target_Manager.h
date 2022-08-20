#pragma once
#include "Base.h"
class CTarget;
class CTarget_Manager :
    public CBase
{
	_DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;
public:
	HRESULT Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pTargetTag, const _uint & iSizeX, const _uint & iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
	HRESULT Add_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag);
	HRESULT Begin_MRT(const _tchar * pMRTTag);
	HRESULT End_MRT(const _tchar * pMRTTag);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar * pTargetTag, const char* pConstantName);
public:
	HRESULT Ready_Debug_Buffer(const _tchar * pTargetTag, const _float & fStartX, const _float & fStartY, const _float & fSizeX, const _float & fSizeY);
	HRESULT Render_Debug_Buffer(const _tchar * pMRTTag);
private:
	map<const _tchar*, CTarget*>				m_mapTarget;
	typedef map<const _tchar*, CTarget*>		MAPTARGET;
private:
	map<const _tchar*, list<CTarget*>>			m_mapMRT;
	typedef map<const _tchar*, list<CTarget*>>	MAPMRT;
private:
	CTarget* Find_Target(const _tchar * pTargetTag);
	list<CTarget*>* Find_MRT(const _tchar * pMRTTag);
protected:
	virtual void Free();
};

