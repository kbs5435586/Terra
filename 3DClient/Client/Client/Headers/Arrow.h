#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CStatic_Mesh;
class CShader;
class CTexture;
class CNavi;

class  CArrow :
	public CGameObject
{
private:
	 CArrow(LPDIRECT3DDEVICE9 pGraphic_Device);
	 CArrow(const  CArrow& rhs);
	virtual ~ CArrow() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
public:
	static	 CArrow*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
public:
	_bool&					GetIsFire(){return m_isFire;}
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CStatic_Mesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CNavi*					m_pNaviCom = nullptr;
private:
	_bool					m_isFire = false;

};

