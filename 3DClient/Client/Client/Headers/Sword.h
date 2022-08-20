#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CStatic_Mesh;
class CShader;
class CNavi;
class CCollider;
class CTexture;
class CSword :
	public CGameObject
{
private:
	const _float	RANGE = 30.f;
private:
	CSword(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSword(const CSword& rhs);
	virtual ~CSword() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
	virtual void			Render_GameObject_Shadow();
	virtual void			Render_GameObject_Blur();
public:
	static	CSword*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
	HRESULT					SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
	HRESULT					SetUp_ConstantTable_Blur(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CStatic_Mesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CShader*				m_pShaderCom_Shadow = nullptr;
	CShader*				m_pShaderCom_Blur = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CTexture*				m_pNormalTextureCom = nullptr;
private:
	CTransform*				m_pPlayerTransform = nullptr;
private:
	_matrix					m_matParent;
	_matrix*				m_pHandMatrix;
	_matrix					m_matIden;
private:
	string					m_strBack = "Skirt_B_03";
	string					m_strRHand = "L_Sword";
	string					m_strCurState;
private:
	_vec3					m_vSize = {};
	_vec3					m_vPos = {};
private:
	_bool					m_isFirst = false;
private:
	SHIRAKEN_TYPE			m_eShirakenType = SHIRAKEN_TYPEEND;


};

