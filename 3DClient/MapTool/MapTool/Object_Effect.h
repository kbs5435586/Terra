#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CTexture;
class CStatic_Mesh;
class CShader;
class CPicking;
class CObject_Effect :
    public CGameObject
{
private:
	CObject_Effect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CObject_Effect(const CObject_Effect& rhs);
	virtual ~CObject_Effect() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float & fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float & fTimeDelta);
	virtual void			Render_GameObject();
private:
	HRESULT					Ready_Component(const _tchar * pComponentTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint & iAttributeID);
public:
	static					CObject_Effect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg) override;
	virtual void			Free();
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureCom_Fire = nullptr;
	CStatic_Mesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CPicking*				m_pPickingCom = nullptr;
private:
	DISTORTION				m_tDistortion;
	TEXINFO					m_tTexInfo;
private:
	_uint					m_iPreRotNum = 99;
};

