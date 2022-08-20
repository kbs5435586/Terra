#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CTexture;
class CDynamic_Mesh;
class CShader;
class CPicking;
class CCollider;
class CObject_Dynamic :
    public CGameObject
{
private:
	CObject_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	CObject_Dynamic(const CObject_Dynamic& rhs);
	virtual ~CObject_Dynamic() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float & fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float & fTimeDelta);
	virtual void			Render_GameObject();
private:
	HRESULT					Ready_Component(const _tchar * pComponentTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
public:
	static					CObject_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg) override;
	virtual void			Free();
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CDynamic_Mesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CPicking*				m_pPickingCom = nullptr;
	
};

