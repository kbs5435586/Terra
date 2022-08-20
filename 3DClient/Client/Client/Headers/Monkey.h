#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CDynamic_Mesh;
class CShader;
class CPicking;
class CCollider;

class CMonkey :
	public CGameObject
{
public:
	const _float ATTACK_RANGE = 9.f;
public:
	enum STATE_ {
		STATE_WAIT, STATE_WALK, STATE_RUN, STATE_SWING, STATE_ATT, STATE_DEATH, STATE_IDLE1, STATE_IDLE2, STATE_IDLE3, STATE_END
	};
private:
	CMonkey(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonkey(const CMonkey& rhs);
	virtual ~CMonkey() = default;
public:
	virtual HRESULT					Ready_Prototype();
	virtual HRESULT					Ready_GameObject(void* pArg);
	virtual _int					Update_GameObject(const _float& fTimeDelta);
	virtual _int					LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void					Render_GameObject();
	virtual void					Render_GameObject_Shadow();
public:
	static	CMonkey*				Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone_GameObject(void* pArg);
	virtual void					Free();
private:
	void							Change_State(const _float& fTimeDelta);
	void							End_Loop(const _float& fTimeDelta);
	void							Collision_Player();
private:
	void							Chase_Player(const _float& fTimeDelta);
	void							Attack1(const _float& fTimeDelta);
	void							MeaningLess_Move(const _float& fTimeDelta);
	void							IDLE1();
	void							IDLE2();
	void							IDLE3();
private:
	HRESULT							Ready_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
	HRESULT							SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
private:
	CTransform*						m_pTransformCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CDynamic_Mesh*					m_pMeshCom = nullptr;
	CDynamic_Mesh*					m_pMeshCom_Shadow = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CShader*						m_pShaderCom_Shadow = nullptr;
	CPicking*						m_pPickingCom = nullptr;
	CCollider*						m_pColliderCom = nullptr;
	CCollider*						m_pColliderCom_Attack = nullptr;
private:
	CTransform*						m_pPlayerTransform = nullptr;
private:
	STATE_							m_eCurState = STATE_END;
	STATE_							m_ePreState = STATE_END;
	SCENEID							m_eCurScene = SCENE_END;
private:
};

