#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CBuffer_CubeTex;
class CTexture;
class CShader;
class CSkyBox :
	public CGameObject
{
private:
	CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox()=default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
public:
	static	CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();

private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ContantTable(LPD3DXEFFECT pEffect);
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CBuffer_CubeTex*		m_pBufferCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
};

