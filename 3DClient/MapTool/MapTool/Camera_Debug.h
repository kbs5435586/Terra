#pragma once
#include "Camera.h"
class CCamera_Debug :
	public CCamera
{
private:
	enum ROTATE_DIR {DIR_LEFT, DIR_RIGHT, DIR_END };
	enum ROTATE_DIRUD {DIR_UP, DIR_DOWN, DIR_END_ };
private:
	explicit CCamera_Debug(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Debug(const CCamera_Debug& rhs);
	virtual ~CCamera_Debug() = default;
public:
	virtual HRESULT				Ready_Prototype();
	virtual HRESULT				Ready_GameObject(void* pArg);
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject();
private:
	void						Compute_DIR_LR(_vec3& vCross);
	void						Compute_DIR_UD(_vec3& vCross);
	void						Select_RotateDir(const _float& fTimeDelta);
public:
	static						CCamera_Debug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone_GameObject(void* pArg);
protected:
	virtual void				Free();
private:
	POINT						m_ptMouse;
	ROTATE_DIR					m_eRotate;
	ROTATE_DIRUD				m_eRotateUD;
private:
	_bool						m_IsRotateEnd_LR = false;
	_bool						m_IsRotateEnd_UD = false;
	_bool						m_IsRotateFinish_LR = false;
	_bool						m_IsRotateFinish_UD = false;
	_float						m_fDot_LR = 0.f;
	_float						m_fDot_UD = 0.f;
};

