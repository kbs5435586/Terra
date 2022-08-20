#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "Management.h"
#include "MainFrm.h"
#include "SecondWnd.h"
#include "UnitTool.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)

{
	lstrcpy(m_pComponent_Tag, rhs.m_pComponent_Tag);
	m_pGraphic_Device->AddRef();
}


HRESULT CGameObject::Ready_GameObject(void* pArg)
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CGameObject::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CGameObject::Render_GameObject()
{
}

void CGameObject::Render_GameObject_Mini()
{
}

CComponent * CGameObject::Get_ComponentPointer(const _tchar * pComponentTag)
{
	CComponent*	pComponent = Find_Component(pComponentTag);
	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

HRESULT CGameObject::Add_Component(const _tchar * pComponentTag, CComponent * pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	m_mapComponent.insert({ pComponentTag, pComponent });

	pComponent->AddRef();

	return S_OK;
}


HRESULT CGameObject::Delete_Component(const _tchar * pComponentTag, CComponent * pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	if (nullptr != Find_Component(pComponentTag))
		m_mapComponent.erase(pComponentTag);
	Safe_Release(pComponent);
	return S_OK;
}


CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CFinder_Tag(pComponentTag));

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Delete_Component(const _tchar* pComponentTag)
{
	auto iter = m_mapComponent.begin();
	for (; iter != m_mapComponent.end();)
	{
		if (!lstrcmp(iter->first, pComponentTag))
		{
			//Safe_Release((iter));
			iter = m_mapComponent.erase(iter);
		}
		else
			++iter;
	}
	
}

void CGameObject::Allocation_Component(CTransform* pTransform)
{ 
	_float fColliderSize = 0.f;
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CSecondWnd* pSecondWnd = dynamic_cast<CSecondWnd*>(pMainFrm->m_MainSplt.GetPane(0, 1));

	fColliderSize = pSecondWnd->m_pUnitTool->m_fColliderSize;

	_matrix			matLocalTransform;
	D3DXMatrixScaling(&matLocalTransform, fColliderSize, fColliderSize, fColliderSize);
	m_pColliderCom = (CCollider*)CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, m_pBoneMatrix, pTransform->Get_Matrix_Pointer()));
	if (FAILED(Add_Component(L"Com_Collider_Bone", m_pColliderCom)))
		return;
}


void CGameObject::Free()
{
	for (auto& Pair : m_mapComponent)
		Safe_Release(Pair.second);
	m_mapComponent.clear();

	Safe_Release(m_pGraphic_Device);
}
