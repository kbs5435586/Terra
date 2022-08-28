#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CShader;
class CTexture;
class CStatic_Mesh;
class CPlayer;
class CColiider;

class CMeteor_Stone :
    public CGameObject
{
private:
	CMeteor_Stone(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMeteor_Stone(const CMeteor_Stone& rhs);
	virtual ~CMeteor_Stone() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
	virtual void			Render_GameObject_PostEffect();
public:
	static	CMeteor_Stone* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
	HRESULT					SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect, const _uint& iAttributeID);
	CTransform* m_pTransformCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CShader* m_pShaderCom_PostEffect = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTexture* m_pTextureCom_Fire[3] = { nullptr, };
	CStatic_Mesh* m_pMeshCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
private:
	CPlayer* m_pPlayer = nullptr;
	TEXINFO					m_tTexInfo = {};
	DISTORTION				m_tDistortion = {};
private:
	_float					m_fLifeTime = 0.f;
	_uint					m_iFirBallIdx = 0;
	CTransform*				m_pParentTransform = nullptr;

};

