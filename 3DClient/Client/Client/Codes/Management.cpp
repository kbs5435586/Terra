#include "stdafx.h"
#include "..\Headers\Management.h"
#include "Scene.h"
#include "System.h"
#include "Timer_Mgr.h"
#include "Frame_Mgr.h"
#include "Device.h"
#include "GameObject_Mgr.h"
#include "Component_Mgr.h"
#include "Light_Mgr.h"
#include "input.h"
#include "Target_Manager.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Player.h"

_IMPLEMENT_SINGLETON(CManagement)
CManagement::CManagement()
	: m_pComponent_Mgr(CComponent_Mgr::GetInstance())
	, m_pGameObject_Mgr(CGameObject_Mgr::GetInstance())
{
	m_pGameObject_Mgr->AddRef();
	m_pComponent_Mgr->AddRef();
}

HRESULT CManagement::Ready_Management(const _uint& iNumScene)
{
	if (nullptr == m_pGameObject_Mgr ||
		nullptr == m_pComponent_Mgr)
		return E_FAIL;

	if (FAILED(m_pGameObject_Mgr->Reserve_Object_Manager(iNumScene)))
		return E_FAIL;

	if (FAILED(m_pComponent_Mgr->Reserve_Component_Manager(iNumScene)))
		return E_FAIL;


	return S_OK;
}

HRESULT CManagement::Add_Prototype_GameObject(const _tchar* pGameObjectTag, CGameObject* pGameObject)
{
	if (nullptr == m_pGameObject_Mgr)
		return E_FAIL;

	return m_pGameObject_Mgr->Add_Prototype_GameObject(pGameObjectTag, pGameObject);
}

HRESULT CManagement::Add_GameObjectToLayer(const _tchar* pProtoTag, const _uint& iSceneID, const _tchar* pLayerTag, void* pArg, CGameObject** ppCloneObject)
{
	if (nullptr == m_pGameObject_Mgr)
		return E_FAIL;

	return m_pGameObject_Mgr->Add_GameObjectToLayer(pProtoTag, iSceneID, pLayerTag, pArg, ppCloneObject);
}

HRESULT CManagement::Add_Prototype_Component(const _uint& iSceneID, const _tchar* pComponentTag, CComponent* pComponent)
{
	if (nullptr == m_pComponent_Mgr)
		return E_FAIL;

	return m_pComponent_Mgr->Add_Prototype_Component(iSceneID, pComponentTag, pComponent);
}

CComponent* CManagement::Clone_Component(const _uint& iSceneID, const _tchar* pComponentTag, void* pArg)
{
	if (nullptr == m_pComponent_Mgr)
		return nullptr;

	return m_pComponent_Mgr->Clone_Component(iSceneID, pComponentTag, pArg);
}

HRESULT CManagement::SetUp_ScenePointer(CScene* pNewScenePointer)
{
	if (nullptr == pNewScenePointer)
		return E_FAIL;

	if (0 != Safe_Release(m_pScene))
		return E_FAIL;

	m_pScene = pNewScenePointer;

	m_pScene->AddRef();

	return NOERROR;

}

_int CManagement::Update_Management(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	_int	iProcessCodes = 0;

	iProcessCodes = m_pScene->Update_Scene(fTimeDelta);
	if (iProcessCodes & 0x80000000)
		return iProcessCodes;

	iProcessCodes = m_pScene->LastUpdate_Scene(fTimeDelta);
	if (iProcessCodes & 0x80000000)
		return iProcessCodes;

	return _int(0);
}


void CManagement::Render_Management()
{
	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

_bool	CManagement::IsComponent(const _uint& iSceneID, const _tchar* pComponentTag)
{
	return m_pComponent_Mgr->IsComponent(iSceneID, pComponentTag);
}

HRESULT CManagement::Clear_Layers(const _uint& iSceneID)
{
	if (nullptr == m_pGameObject_Mgr)
		return E_FAIL;

	return m_pGameObject_Mgr->Clear_Layers(iSceneID);
}
HRESULT CManagement::Find_Clear_Layer(const _uint& iSceneID, const _tchar* pLayerTag)
{
	if (nullptr == m_pGameObject_Mgr)
		return E_FAIL;
	return m_pGameObject_Mgr->Find_Clear_Layer(iSceneID, pLayerTag);
}

CGameObject* CManagement::Get_GameObject(const _uint& iSceneID, const _tchar* pLayerTag, const _uint& iIndex)
{
	if (nullptr == m_pGameObject_Mgr)
		return nullptr;
	return m_pGameObject_Mgr->Get_GameObject(iSceneID, pLayerTag, iIndex);
}

list<CGameObject*>* CManagement::Get_ObjectList(const _uint& iSceneID, const _tchar* pLayerTag)
{
	if (nullptr == m_pGameObject_Mgr)
	{
#if _DEBUG
		_MSG_BOX("Can't Find ObjectList");
#endif 
		return nullptr;
	}

	return m_pGameObject_Mgr->Get_ObjectList(iSceneID, pLayerTag);
}

CGameObject* CManagement::Get_BackObject(const _uint& iSceneID, const _tchar* pLayerTag)
{
	return CGameObject_Mgr::GetInstance()->Get_BackObject(iSceneID, pLayerTag);
}

map<const _tchar*, CLayer*>* CManagement::Get_MapLayer(const _uint& iSceneID)
{
	if (m_pGameObject_Mgr != nullptr)
		return m_pGameObject_Mgr->Get_MapLayer(iSceneID);

	return &map<const _tchar*, CLayer*>();
}

void CManagement::Key_Update()
{
	m_dwKey = 0;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_dwKey |= KEY_SHIFT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;

	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_RIGHT;


	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		m_dwKey |= KEY_NUM1;
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		m_dwKey |= KEY_NUM2;
	if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
		m_dwKey |= KEY_NUM3;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_AUP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_ADOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_ALEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_ARIGHT;

	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_RIGHT;

}

bool CManagement::KeyUp(DWORD dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}
	else if (m_dwKeyPressed & dwKey)
	{
		m_dwKeyPressed ^= dwKey;
		return true;
	}

	return false;
}

bool CManagement::KeyDown(DWORD dwKey)
{
	if (!(m_dwKeyDown & dwKey) && (m_dwKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CManagement::KeyPressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}

bool CManagement::KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if (KeyDown(dwSecondKey) && (m_dwKey & dwFirstKey))
		return true;

	return false;
}
void CManagement::Collision_Update(const _float& fTimeDelta)
{
   //	Collision_AABB(fTimeDelta);
	Collision_OBB(fTimeDelta);
	//Collision_Part(fTimeDelta);
	//Collision_RANGE(fTimeDelta);
}
void CManagement::Collision_AABB(const _float& fTimeDelta)
{
}
void CManagement::Collision_OBB(const _float& fTimeDelta)
{
	if (!CManagement::GetInstance()->Get_ObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Player"))
		return;
	if (!CManagement::GetInstance()->Get_ObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Monster"))
		return;
	for (auto& iter0 : *CManagement::GetInstance()->Get_ObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Player"))
	{
		for (auto& iter1 : *CManagement::GetInstance()->Get_ObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Monster"))
		{

			_vec3 vIter0Pos = *dynamic_cast<CTransform*>(iter0->Get_ComponentPointer(L"Com_Transform"))->Get_StateInfo(STATE::STATE_POSITION);
			_vec3 vIter1Pos = *dynamic_cast<CTransform*>(iter1->Get_ComponentPointer(L"Com_Transform"))->Get_StateInfo(STATE::STATE_POSITION);

			_vec3 vLenTemp = vIter0Pos - vIter1Pos;
			_float fLen = D3DXVec3Length(&vLenTemp);
			if (fLen >= 30.f)
				continue;

			if (dynamic_cast<CCollider*>(iter1->Get_ComponentPointer(L"Com_Collider_OBB"))
				->Collision_OBB(dynamic_cast<CCollider*>(iter0->Get_ComponentPointer(L"Com_Collider_OBB"))))
			{
				_uint i = 0;
				if (iter1->GetIsHit())	//Monster Attack ¸¸
				{
					//	(iter0)->GetIsBack() = true;
					//	iter0->GetAttackedObject_Matrix() = dynamic_cast<CTransform*>(iter1->Get_ComponentPointer(L"Com_Transform"))->Get_Matrix();
					//	iter0->GetIsParticle() = true;
					//	iter0->GetInfo().fHP -= iter1->GetInfo().fAtt;
					(iter0)->GetOBBCollision() = true;
					//iter1->GetIsHit() = false;
				}


			}
		}
	}
}
void CManagement::Collision_RANGE(const _float& fTimeDelta)
{
}
void CManagement::Collision_Part(const _float& fTimeDelta)
{
	if (!CManagement::GetInstance()->Get_ObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Shiraken"))
		return;
	if (!CManagement::GetInstance()->Get_ObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Monster"))
		return;
	for (auto& pShiraken : *CManagement::GetInstance()->Get_ObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Shiraken"))
	{
		for (auto& pMonster : *CManagement::GetInstance()->Get_ObjectList((_uint)SCENEID::SCENE_STATIC, L"Layer_Monster"))
		{
			pMonster->GetCollisionAccTime() += fTimeDelta;
			_vec3 vIter0Pos = *dynamic_cast<CTransform*>(pShiraken->Get_ComponentPointer(L"Com_Transform"))->Get_StateInfo(STATE::STATE_POSITION);
			_vec3 vIter1Pos = *dynamic_cast<CTransform*>(pMonster->Get_ComponentPointer(L"Com_Transform"))->Get_StateInfo(STATE::STATE_POSITION);

			_vec3 vLenTemp = vIter0Pos - vIter1Pos;
			_float fLen = D3DXVec3Length(&vLenTemp);

			_vec3 vParticlePos = {};
			if (dynamic_cast<CCollider*>(pMonster->Get_ComponentPointer(L"Com_Collider_OBB"))
				->Collision_OBB(dynamic_cast<CCollider*>(pShiraken->Get_ComponentPointer(L"Com_Collider_OBB")), vParticlePos))
			{
				if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_Player", 0))->GetIsParticle())
				{
					if (pMonster->GetCollisionAccTime() >= 1.f)
					{
						if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Particle_Spark", SCENE_STATIC, L"Layer_Particle")))
							return;
						pMonster->GetCollisionAccTime() = 0.f;
					}

					dynamic_cast<CParticleSystem*>(CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Particle"))->GetOriginPos() = vParticlePos;
				}


				
				(pMonster)->GetOBBCollision() = true;
				pMonster->GetIsHit() = false;
			}



		}
	}

}
CComponent* CManagement::Get_ComponentPointer(const _uint& iSceneID, const _tchar* pLayerTag, const _tchar* pComponentTag, const _uint& iIndex)
{
	if (nullptr == m_pGameObject_Mgr)
		return nullptr;

	return m_pGameObject_Mgr->Get_ComponentPointer(iSceneID, pLayerTag, pComponentTag, iIndex);
}

void CManagement::Release_Engine()
{
	_ulong			dwRefCnt = 0;


	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
		_MSG_BOX("CManagement Release Failed");
	if (dwRefCnt = CLight_Mgr::GetInstance()->DestroyInstance())
		_MSG_BOX("CLight_Mgr Release Failed");
	if (dwRefCnt = CGameObject_Mgr::GetInstance()->DestroyInstance())
		_MSG_BOX("CObject_Manager Release Failed");
	if (dwRefCnt = CComponent_Mgr::GetInstance()->DestroyInstance())
		_MSG_BOX("CComponent_Manager Release Failed");
	if (dwRefCnt = CSystem::GetInstance()->DestroyInstance())
		_MSG_BOX("CSystem Release Failed");
	if (dwRefCnt = CTimer_Mgr::GetInstance()->DestroyInstance())
		_MSG_BOX("CTimer_Manager Release Failed");
	if (dwRefCnt = CFrame_Mgr::GetInstance()->DestroyInstance())
		_MSG_BOX("CTimer_Manager Release Failed");
	if (dwRefCnt = CInput::GetInstance()->DestroyInstance())
		_MSG_BOX("CInput Release Failed");
	if (dwRefCnt = CTarget_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX("CTarget Manager Failed");
	if (dwRefCnt = CDevice::GetInstance()->DestroyInstance())
		_MSG_BOX("CGraphic_Device Release Failed");

	int i = 0;
}

void CManagement::Free()
{
	Safe_Release(m_pComponent_Mgr);
	Safe_Release(m_pGameObject_Mgr);
	Safe_Release(m_pScene);
}
