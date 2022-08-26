#pragma once
#include "GameObject.h"
class CShader;
class CRenderer;
class CTransform;
class CTexture;
class CParticleSystem :
    public CGameObject
{
protected:
    explicit				CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
    explicit				CParticleSystem(CParticleSystem& rhs);
    virtual					~CParticleSystem() = default;
public:
	virtual HRESULT			Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
public:
	virtual bool			Init();
	virtual void			Reset();
	virtual void			Reset_Particle(PSATTRIBUTE* pAttribute) PURE;
	virtual void			Add_Particle();
	virtual void			Pre_Render();
	virtual void			Post_Render();
	virtual void			Remove_Dead_Particle();
public:
	_bool					IsEmpty();
	_bool					IsDead();
	_float					GetRandom_Float(_float fLowBound, _float fHighBound);
	void					GetRandom_Vector(_vec3& vOut, _vec3& vMin, _vec3& vMax);
private:
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	virtual CGameObject*	Clone_GameObject(void* pArg = nullptr)PURE;
	virtual void			Free();

public:
	_vec3&					GetOriginPos(){return m_vOriginPos;}
protected:
	_vec3					m_vOriginPos;
	_float					m_fEmiRate = 0.f;
	_float					m_fSize = 0.f;
	_uint					m_maxParticle = 0;
	_uint					m_iParticleSize = 0;
	DWORD					m_iVBSize = 0;
	DWORD					m_iVBOffset = 0;
	DWORD					m_iVBBatchSize = 0;
	_float					m_fLifeTime = 0.f;
	_float					m_fTimeDelta = 0.f;
	_bool					m_isChangeColor = false;
	_int					m_iTextureIdx = 0;
protected:
	_vec4					m_vParticleColor = {};
protected:
	BoundingBox				m_tBoundingBox;
	PDIRECT3DVERTEXBUFFER9	m_vb;
protected:
	list<PSATTRIBUTE>		m_lstParticle;
protected:
	CShader*				m_pShaderCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
protected:
	CTransform*				m_pPlayerTransform = nullptr;

};


