#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CShader;
class CTexture;
class CStatic_Mesh;
class CPlayer;
class CColiider;


struct TEXINFO
{
	_float	fFrameTime;
	_vec3	vScrollSpeed;
	_vec3	vScale;
};

struct DISTORTION
{
	_vec4		fDistortion1;
	_vec4		fDistortion2;
	_vec4		fDistortion3;
	_float		fDistortionScale;
	_float		fDistortionBias;
};

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
public:
	static	CFire_Tall*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureCom_Fire[3]={nullptr, };
	CStatic_Mesh*			m_pMeshCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
private:
	CPlayer*				m_pPlayer = nullptr;
	TEXINFO					m_tTexInfo = {};
	DISTORTION				m_tDistortion = {};
private:
	_float					m_fLifeTime = 0.f;
};

