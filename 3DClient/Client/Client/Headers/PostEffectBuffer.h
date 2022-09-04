#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CShader;
class CBuffer_RcTex;

class CPostEffectBuffer :
    public CGameObject
{
private:
	CPostEffectBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPostEffectBuffer(const  CPostEffectBuffer& rhs);
	virtual ~CPostEffectBuffer() = default;
public:
	virtual HRESULT					Ready_Prototype();
	virtual HRESULT					Ready_GameObject(void* pArg);
	virtual _int					Update_GameObject(const _float& fTimeDelta);
	virtual _int					LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void					Render_GameObject();
	virtual void					Render_GameObject_PostEffect();
public:
	static	CPostEffectBuffer*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone_GameObject(void* pArg);
	virtual void					Free();
private:
	HRESULT							Ready_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT							SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect);
private:
	CTransform*						m_pTransformCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CBuffer_RcTex*					m_pBufferCom = nullptr;
};

