#include "stdafx.h"
#include "..\Headers\Scene.h"
#include "GameObject_Mgr.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pGameObject_Mgr(CGameObject_Mgr::GetInstance())
{
	m_pGraphic_Device->AddRef();
	m_pGameObject_Mgr->AddRef();
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pGameObject_Mgr)
		return -1;

	return m_pGameObject_Mgr->Update_GameObject_Manager(fTimeDelta);
}

_int CScene::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pGameObject_Mgr)
		return -1;

	return m_pGameObject_Mgr->LastUpdate_GameObject_Manager(fTimeDelta);
}


void CScene::Render_Scene()
{
}

void CScene::Remove_End(_tchar* pFileName)
{
	_uint i = 0;

	while (pFileName[i])
	{
		++i;
	}

	pFileName[i - 1] = '\0';
}

void CScene::Free()
{
	Safe_Release(m_pGameObject_Mgr);
	Safe_Release(m_pGraphic_Device);
}

