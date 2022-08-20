#pragma once
#include "GameObject.h"

class CPlayer;
class CTransform;
class CBuffer_Trail;
class CRenderer;
class CShader;
class CTexture;
class CTrail :
    public CGameObject
{
private:
	CTrail(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrail(const CTrail& rhs);
	virtual ~CTrail() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float & fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float & fTimeDelta);
	virtual void			Render_GameObject_Effect();
public:
	static	CTrail*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable_Effect(LPD3DXEFFECT pEffect);
private:
	void					Create_Trail(const _float& fTimeDelta);
private:
	CTransform*				m_pTransformCom = nullptr;
	CBuffer_Trail*			m_pBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
private:
	CPlayer*				m_pPlayer = nullptr;
private:
	_float					m_fDuration = 0.03f;
	_float					m_fCreateTime = 0.f;
	_float					m_fAccTime = 0.f;


};

