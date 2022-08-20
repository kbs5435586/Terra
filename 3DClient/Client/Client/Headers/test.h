#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CTexture;
class CBuffer_RcTex;
class CTest :
	public CGameObject
{

private:
	explicit CTest(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTest(const CTest& rhs);
	virtual ~CTest() = default;
public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CBuffer_RcTex*		m_pBufferCom = nullptr;
private:
	HRESULT Ready_Component();
public:
	static CTest* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg) override;
protected:
	virtual void Free();
};

