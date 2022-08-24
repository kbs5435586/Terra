#pragma once
#include "GameObject.h"

class CPlayer;
class CBuffer_Trail;
class CTransform;
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
	virtual void			Render_GameObject();
public:
	static	CTrail*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	void					Create_Trail(const _float& fTimeDelta);
private:
	CBuffer_Trail*			m_pBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureCom_Mask = nullptr;
private:
	CPlayer*				m_pPlayer = nullptr;
private:
	_float					m_fDuration = 0.1f;
	_float					m_fCreateTime = 0.f;
private:
	_uint					m_iTextureIdx = 0;


};

