#pragma once
#include "Camera.h"
class CCamera_Light :
    public CCamera
{
private:
	explicit CCamera_Light(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Light(const CCamera_Light& rhs);
	virtual ~CCamera_Light() = default;
public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	static CCamera_Light* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
protected:
	virtual void Free();
private:
	POINT			m_ptMouse;
};

