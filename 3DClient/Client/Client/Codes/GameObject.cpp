#include "stdafx.h"
#include "..\Headers\GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Management.h"
#include "ParticleSystem.h"
#include "Target_Manager.h"
#include "Target.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}


HRESULT CGameObject::Ready_GameObject(void* pArg)
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

_int CGameObject::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return _int();
}


void CGameObject::Render_GameObject()
{
}

void CGameObject::Render_GameObject_Shadow()
{

}

void CGameObject::Render_GameObject_Blur()
{
}


void CGameObject::Render_GameObject_PostEffect()
{
}

void CGameObject::Create_Particle()
{
	if (m_IsPlayer_Particle)
	{
		if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Particle_Spark", SCENE_STATIC, L"Layer_Particle")))
			return;
		dynamic_cast<CParticleSystem*>(CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Particle"))->GetOriginPos() = m_vCollisionPos;
		m_IsPlayer_Particle = false;
	}
}

void CGameObject::Create_Particle(const _float& fTimeDelta, const _float& fperiod)
{
	m_fAccTime += fTimeDelta;

	if (m_fAccTime >= fperiod)
	{
		if (m_IsPlayer_Particle)
		{
			if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Particle_Spark", SCENE_STATIC, L"Layer_Particle")))
				return;
			dynamic_cast<CParticleSystem*>(CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Particle"))->GetOriginPos() = m_vCollisionPos;
			m_IsPlayer_Particle = false;
		}
		m_fAccTime = 0.f;
	}

}

void CGameObject::Obb_Collision(CTransform* pTransform, const _float& fAddY)
{
	if (m_IsOBB_Collision && m_fBazierCnt <= 1.f)
	{
		if (!m_IsBazier)
		{
			m_vStartPoint = *pTransform->Get_StateInfo(STATE_POSITION);
			_vec3 vLook = *pTransform->Get_StateInfo(STATE_LOOK);
			vLook *= -1.f;
			m_vEndPoint = *pTransform->Get_StateInfo(STATE_POSITION) + (vLook * 50);
			m_vMidPoint = (m_vStartPoint + m_vEndPoint) / 2;
			m_vMidPoint.y += fAddY;
			m_IsBazier = true;
		}
		Hit_Object(pTransform, m_fBazierCnt, m_vStartPoint, m_vEndPoint, m_vMidPoint);

	}
	if (m_fBazierCnt >= 1.f)
	{
		m_fBazierCnt = 0.f;
		m_IsOBB_Collision = false;
		m_IsBazier = false;
	}
}

void CGameObject::Hit_Object(CTransform* pTransform, _float& fCnt, _vec3 vStart, _vec3 vEnd, _vec3 vMid)
{
	_float fX = (pow((1.f - fCnt), 2) * vStart.x) + (2 * fCnt * (1.f - fCnt) * vMid.x) + (pow(fCnt, 2) * vEnd.x);
	_float fY = (pow((1.f - fCnt), 2) * vStart.y) + (2 * fCnt * (1.f - fCnt) * vMid.y) + (pow(fCnt, 2) * vEnd.y);
	_float fZ = (pow((1.f - fCnt), 2) * vStart.z) + (2 * fCnt * (1.f - fCnt) * vMid.z) + (pow(fCnt, 2) * vEnd.z);

	pTransform->Set_StateInfo(STATE_POSITION, &_vec3(fX, fY, fZ));
	fCnt += 0.01f;
}

void CGameObject::Hit_Object(CTransform* pTransform, _float& fCnt, _vec3 vStart, _vec3 vEnd, _vec3 vMid, const float& fTimeDelta, const _vec3& vSize)
{
	_float fX = (pow((1.f - fCnt), 2) * vStart.x) + (2 * fCnt * (1.f - fCnt) * vMid.x) + (pow(fCnt, 2) * vEnd.x);
	_float fY = (pow((1.f - fCnt), 2) * vStart.y) + (2 * fCnt * (1.f - fCnt) * vMid.y) + (pow(fCnt, 2) * vEnd.y);
	_float fZ = (pow((1.f - fCnt), 2) * vStart.z) + (2 * fCnt * (1.f - fCnt) * vMid.z) + (pow(fCnt, 2) * vEnd.z);

	pTransform->Set_StateInfo(STATE_POSITION, &_vec3(fX, fY, fZ));


	_vec3 vNewLook = vEnd - *pTransform->Get_StateInfo(STATE::STATE_POSITION);
	D3DXVec3Normalize(&vNewLook, &vNewLook);

	_vec3 vAxisY = _vec3(0.f, 1.f, 0.f);
	_vec3 vNewRight;
	D3DXVec3Cross(&vNewRight, &vAxisY, &vNewLook);
	D3DXVec3Normalize(&vNewRight, &vNewRight);

	_vec3 vNewUp;;

	D3DXVec3Cross(&vNewUp, &vNewLook, &vNewRight);
	D3DXVec3Normalize(&vNewUp, &vNewUp);

	pTransform->Set_StateInfo(STATE::STATE_RIGHT, &vNewRight);
	pTransform->Set_StateInfo(STATE::STATE_UP, &vNewUp);
	pTransform->Set_StateInfo(STATE::STATE_LOOK, &vNewLook);
	pTransform->Scaling(vSize);
	fCnt += fTimeDelta;
}

void CGameObject::Hit_Arrow(CTransform* pTransform, _float& fCnt, _vec3 vStart, _vec3 vEnd, _vec3 vMid, const float& fTimeDelta, const _vec3& vSizs)
{
	_float fX = (pow((1.f - fCnt), 2) * vStart.x) + (2 * fCnt * (1.f - fCnt) * vMid.x) + (pow(fCnt, 2) * vEnd.x);
	_float fY = (pow((1.f - fCnt), 2) * vStart.y) + (2 * fCnt * (1.f - fCnt) * vMid.y) + (pow(fCnt, 2) * vEnd.y);
	_float fZ = (pow((1.f - fCnt), 2) * vStart.z) + (2 * fCnt * (1.f - fCnt) * vMid.z) + (pow(fCnt, 2) * vEnd.z);


	pTransform->Set_StateInfo(STATE_POSITION, &_vec3(fX, fY, fZ));


	_vec3 vNewLook = vEnd - *pTransform->Get_StateInfo(STATE::STATE_POSITION);
	D3DXVec3Normalize(&vNewLook, &vNewLook);

	_vec3 vAxisY = _vec3(0.f, 1.f, 0.f);
	_vec3 vNewRight;
	D3DXVec3Cross(&vNewRight, &vAxisY, &vNewLook);
	D3DXVec3Normalize(&vNewRight, &vNewRight);

	_vec3 vNewUp;;

	D3DXVec3Cross(&vNewUp, &vNewLook, &vNewRight);
	D3DXVec3Normalize(&vNewUp, &vNewUp);

	pTransform->Set_StateInfo(STATE::STATE_RIGHT, &vNewRight);
	pTransform->Set_StateInfo(STATE::STATE_UP, &vNewUp);
	pTransform->Set_StateInfo(STATE::STATE_LOOK, &vNewLook);
	pTransform->Scaling(vSizs);
	//pTransform->SetUp_RotationY(D3DXToRadian(-90.f));
	fCnt += fTimeDelta;
}

CComponent* CGameObject::Get_ComponentPointer(const _tchar* pComponentTag)
{
	CComponent* pComponent = Find_Component(pComponentTag);
	if (nullptr == pComponent)
		return nullptr;


	return pComponent;
}

HRESULT CGameObject::Add_Component(const _tchar* pComponentTag, CComponent* pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	m_mapComponent.insert({ pComponentTag, pComponent });

	pComponent->AddRef();

	return S_OK;
}

CComponent* CGameObject::Find_Component(const _tchar* pComponentTag)
{
	auto iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CFinder_Tag(pComponentTag));

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

_float CGameObject::GetRandom_Float(_float fLowBound, _float fHighBound)
{
	if (fLowBound >= fHighBound)
		return fLowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (fHighBound - fLowBound)) + fLowBound;
}

void CGameObject::GetRandom_Vector(_vec3& vOut, _vec3& vMin, _vec3& vMax)
{
	vOut.x = GetRandom_Float(vMin.x, vMax.x);
	//vOut.y = GetRandom_Float(vMin.y, vMax.y);
	vOut.z = GetRandom_Float(vMin.z, vMax.z);
}

void CGameObject::Free()
{
	for (auto& Pair : m_mapComponent)
		Safe_Release(Pair.second);
	m_mapComponent.clear();

	for (auto& iter : m_vecPartName)
		Safe_Delete_Array(iter);

	Safe_Release(m_pGraphic_Device);
}
