#pragma once
#include "Component.h"
class CShader;
class CTransform;
class CCollider :
	public CComponent
{
private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
public:
	const OBB* Get_OBBDescPointer() const {return m_pOBB;}
public:
	HRESULT						Ready_Collider(COLLIDER_TYPE eType);
	HRESULT						Ready_Clone_Collider(void* pArg);
	_bool						Collision_AABB(const CCollider* pTargetCollider);
	void						Collision_AABB(const CCollider* pTargetCollider,CTransform* pSourTransform, CTransform* pDestTransform);
	_bool						Collision_OBB(const CCollider* pTargetCollider);
	_bool						Collision_OBB(const CCollider* pTargetCollider, _vec3& vOut);
	void						Render_Collider();
public:
	HRESULT						Update_Collider_Info(_matrix	matInfo);
private:
	LPD3DXMESH					m_pCollider;
	LPD3DXBUFFER				m_pAdjacency;
	CShader*					m_pShader = nullptr;
	_bool						m_isColl = false;
	BOX_TYPE					m_eBoxType = BOX_TYPE(0);
	_vec3						m_vMin, m_vMax;
	COLLIDER_INFO				m_ColliderInfo;	
	OBB*						m_pOBB = nullptr;
public:
	_vec3						GetMin() { return m_vMin; }
	_vec3						GetMax() { return m_vMax; }
public:
	COLLIDER_INFO&				GetColliderInfo(){return m_ColliderInfo;}
private:
	HRESULT						Ready_BoundingBox();
	HRESULT						Ready_BoundingSphere();
	_matrix						Remove_Rotation(_matrix matTransform) const;
	_matrix						Compute_WorldTransform() const;
	void						Compute_AlignAxis(OBB* pOBB);
	void						Compute_ProjAxis(OBB* pOBB);
public:
	static CCollider*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER_TYPE eType);
	virtual CComponent*			Clone_Component(void* pArg = nullptr);
	virtual void				Free();
};

