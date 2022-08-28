#pragma once
#include "ParticleSystem.h"
class CTransform;
class CRenderer;
class CShader;
class CTexture;
class CFireSpark :
    public CParticleSystem
{
private:
	CFireSpark(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumParticle);
	CFireSpark(CFireSpark& rhs);
	~CFireSpark() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
	virtual void			preRender();
	virtual void			postRender();
	virtual void			Reset_Particle(PSATTRIBUTE* pAttribute) override;
	void					Active();
private:
	HRESULT					Ready_Component(void* pArg = nullptr);
private:
	_uint					m_iNumParticle = 0;
public:
	static CFireSpark*		Create(PDIRECT3DDEVICE9 pGrapic_Device, _uint iNumParticle);
	virtual CGameObject*	Clone_GameObject(void* pArg) override;
	virtual void			Free();
private:
	CTransform* m_pParentTransform = nullptr;


};

