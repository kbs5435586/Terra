#include "stdafx.h"
#include "..\Headers\Target_Manager.h"
#include "Target.h"

_IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{

}

HRESULT CTarget_Manager::Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, 
	const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color)
{
	CTarget* pTarget = Find_Target(pTargetTag);

	if (nullptr != pTarget)
		return E_FAIL;

	pTarget = CTarget::Create(pGraphic_Device, iSizeX, iSizeY, Format, Color);
	if (nullptr == pTarget)
		return E_FAIL;

	m_mapTarget.insert(MAPTARGET::value_type(pTargetTag, pTarget));

	return NOERROR;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	CTarget* pTarget = Find_Target(pTargetTag);
	if (nullptr == pTarget)
		return E_FAIL;

	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CTarget*>	TargetList;
		TargetList.push_back(pTarget);

		m_mapMRT.insert(MAPMRT::value_type(pMRTTag, TargetList));
	}
	else
		pMRTList->push_back(pTarget);

	pTarget->AddRef();

	return NOERROR;
}

HRESULT CTarget_Manager::Begin_MRT(const _tchar* pMRTTag)
{
	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pTarget : *pMRTList)
	{
		pTarget->Clear_Target();
	}

	_uint		iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		pTarget->SetUp_OnGraphicDev(iIndex++);
	}

	return NOERROR;
}

HRESULT CTarget_Manager::End_MRT(const _tchar* pMRTTag)
{
	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	_uint		iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		pTarget->Release_OnGraphicDev(iIndex++);
	}


	return NOERROR;
}

HRESULT CTarget_Manager::SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName)
{
	CTarget* pTarget = Find_Target(pTargetTag);
	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->SetUp_OnShader(pEffect, pConstantName);
}

HRESULT CTarget_Manager::Ready_Debug_Buffer(const _tchar* pTargetTag, const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY)
{
	CTarget* pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->Ready_Debug_Buffer(fStartX, fStartY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug_Buffer(const _tchar* pMRTTag)
{
	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pTarget : *pMRTList)
		pTarget->Render_Debug_Buffer();

	return NOERROR;
}


CTarget* CTarget_Manager::Find_Target(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_mapTarget.begin(), m_mapTarget.end(), CFinder_Tag(pTargetTag));

	if (iter == m_mapTarget.end())
		return nullptr;

	return iter->second;
}

list<CTarget*>* CTarget_Manager::Find_MRT(const _tchar* pMRTTag)
{
	auto	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CFinder_Tag(pMRTTag));

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_mapMRT)
	{
		for (auto& pTarget : Pair.second)
		{
			Safe_Release(pTarget);
		}
		Pair.second.clear();
	}
	m_mapMRT.clear();

	for (auto& Pair : m_mapTarget)
		Safe_Release(Pair.second);

	m_mapTarget.clear();
}
