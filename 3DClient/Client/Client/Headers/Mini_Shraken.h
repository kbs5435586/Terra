#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CStatic_Mesh;
class CShader;
class CNavi;
class CCollider;
class CTexture;

class CMini_Shraken :
    public CGameObject
{
private:
	const _float	RANGE = 30.f;
private:
	CMini_Shraken(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMini_Shraken(const CMini_Shraken& rhs);
	virtual ~CMini_Shraken() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float & fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float & fTimeDelta);
	virtual void			Render_GameObject();
	virtual void			Render_GameObject_Shadow();
public:
	static	CMini_Shraken*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint & iAttributeID);
	HRESULT					SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const _uint & iAttributeID);
public:
	_vec3&					GetFirstPos(){return m_vFirstPos;}
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CStatic_Mesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CShader*				m_pShaderCom_Shadow = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CTexture*				m_pNormalTextureCom = nullptr;
private:
	CTransform*				m_pPlayerTransform = nullptr;
private:
	_matrix					m_matParent;
	_matrix*				m_pHandMatrix;
	_matrix					m_matIden;
private:
	string					m_strBack = "Skirt_B_00";
	string					m_strRHand = "R_Sword";
	string					m_strCurState;
private:
	_vec3					m_vSize = {};
	_vec3					m_vTargetPos = {};
	_vec3					m_vPos = {};
	_vec3					m_vFirstPos = {};
private:
	_bool					m_isFirst = false;
private:
	SHIRAKEN_TYPE			m_eShirakenType = SHIRAKEN_TYPEEND;

};

