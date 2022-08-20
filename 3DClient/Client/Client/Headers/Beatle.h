#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CDynamic_Mesh;
class CShader;
class CPicking;
class CCollider;

#define CNT_FLY 1
class CBeatle :
	public CGameObject
{
public:
	const _float ATTACK_RANGE = 15.f;
public:
	enum STATE_ {STATE_IDLE, STATE_WALK, STATE_ATT, STATE_FLINCH, STATE_DEATH, 
		STATE_DEATH_WAIT, STATE_FLY_START, STATE_FLYING, STATE_FLY_END, STATE_END};
	enum ROTATE_DIR {DIR_LEFT, DIR_RIGHT, DIR_END};
private:
	CBeatle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBeatle(const CBeatle& rhs);
	virtual ~CBeatle() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
	virtual void			Render_GameObject_Shadow();
private:
	void					Change_State(const _float& fTimeDelta);
	void					End_Loop(const _float& fTimeDelta);
	void					Collision_Player();
public:
	static	CBeatle*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
	HRESULT					SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
private:
	void					Chase_Player(const _float& fTimeDelta, _float fLenght);
	void					IDLE();
	void					Attack(const _float& fTimeDelta);
	void					MeaningLess_Move(const _float& fTimeDelta);
	void					Fly(const _float& fTimeDelta);

private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CDynamic_Mesh*			m_pMeshCom = nullptr;
	CDynamic_Mesh*			m_pMeshCom_Shadow = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CShader*				m_pShaderCom_Shadow = nullptr;
	CPicking*				m_pPickingCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CCollider*				m_pColliderCom_Attack = nullptr;
private:
	vector<const _matrix*>	m_vecBoneMatrix;
private:
	CTransform*				m_pPlayerTransform = nullptr;
private:
	STATE_					m_eCurState = STATE_END;
	STATE_					m_ePreState = STATE_END;
	SCENEID					m_eCurScene = SCENE_END;
private:
	ROTATE_DIR				m_eRotate = DIR_END;
private:
	_bool					m_IsDest = false;
	_vec3					m_vDest = {};
	_int					m_iDestLength = 0;
private:
	_bool					m_IsRotateEnd = false;
	_float					m_fDot = 0.f;
	_int					m_iFlyCnt = 0;
private:
	_bool					m_IsChangeAnimation = false;

};

