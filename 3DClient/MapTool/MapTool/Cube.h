#pragma once
#include "GameObject.h"
class CBuffer_CubeWire;
class CTransform;
class CRenderer;
class CShader;
class CPicking;

class CCube :
    public CGameObject
{
private:
	CCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCube(const CCube& rhs);
	virtual ~CCube() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float & fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float & fTimeDelta);
	virtual void			Render_GameObject();
private:
	HRESULT					Ready_Component(void* pArg);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static					CCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg) override;
	virtual void			Free();
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CPicking*				m_pPickingCom = nullptr;
	CBuffer_CubeWire*		m_pBufferCom = nullptr;

};

