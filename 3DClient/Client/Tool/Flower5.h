#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CStatic_Mesh;
class CShader;
class CPicking;
class COnHeadFont;
class CFlower5 :
	public CGameObject
{
private:
	CFlower5(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFlower5(const CFlower5& rhs);
	virtual ~CFlower5() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
public:
	static	CFlower5*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CStatic_Mesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CPicking*				m_pPickingCom = nullptr;
private:
	COnHeadFont*			m_pOnHeadFont = nullptr;
};

