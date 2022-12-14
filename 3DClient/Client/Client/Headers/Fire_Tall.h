#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CShader;
class CTexture;
class CStatic_Mesh;
class CPlayer;
class CColiider;



class CFire_Tall :
    public CGameObject
{
private:
	CFire_Tall(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFire_Tall(const CFire_Tall& rhs);
	virtual ~CFire_Tall() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
	virtual void			Render_GameObject_PostEffect();
public:
	static	CFire_Tall*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
	HRESULT					SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CShader*				m_pShaderCom_PostEffect = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureCom_Fire[3]={nullptr, };
	CStatic_Mesh*			m_pMeshCom = nullptr;
private:
	CPlayer*				m_pPlayer = nullptr;
	TEXINFO					m_tTexInfo = {};
	DISTORTION				m_tDistortion = {};
private:
	_float					m_fLifeTime = 0.f;
	_uint					m_iFirBallIdx = 0;
	_uint					m_iRandidx = 0;
	CTransform*				m_pParentTransform = nullptr;


	
};

