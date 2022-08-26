#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CDynamic_Mesh;
class CShader;
class CNavi;
class CCollider;
class CTexture;

class CPlayer :
	public CGameObject
{
public:
	enum STATE_ {STATE_JUMP_ENDLAND, STATE_JUMP_END, STATE_JUMPLOOP, STATE_JUMP_START,
				STATE_PL_WAIT, STATE_PL_RUN, STATE_OUT_WEAPON, STATE_IN_WEAPON, STATE_PL_COM7, STATE_PL_COM6R,
				STATE_PL_COM6, STATE_PL_COM5R, STATE_PL_COM5, STATE_PL_COM4R, STATE_PL_COM4, STATE_PL_COM3R,
				STATE_PL_COM3,STATE_PL_COM2R, STATE_PL_COM2, STATE_PL_COM1R, STATE_PL_COM1, STATE_BACKJUMP, STATE_BACKJUMP_END,
				STATE_ASS_MOVE_LOOP, STATE_ASS_MOVE_END, STATE_ASS_MOVE_STOP, STATE_DASH_START, STATE_DASH_END,
				STATE_ROT_SIRAKEN_START, STATE_ROT_SIRAKEN_LOOP, STATE_ROT_SIRAKEN_END, STATE_FIRE_START, STATE_FIRE_READY, STATE_FIRE_SHOT,
				STATE_FIRE_END, STATE_TWIN_START, STATE_TWIN_SHOT, STATE_TWIN_R, STATE_CALL_ELEC, STATE_CALL_ELEC_LOOP,
				STATE_ELEC_END, STATE_B_REACT_START, STATE_B_REACT_LOOP, STATE_B_REACT_END, STATE_END};
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject();
	virtual void			Render_GameObject_Shadow();
	virtual void			Render_GameObject_Blur();
	virtual void			Render_GameObject_PostEffect();
public:
	static	CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone_GameObject(void* pArg);
	virtual void			Free();
private:
	void					Change_State(STATE_ eID);
	void					Input_Key(const _float& fTimeDelta);
	void					End_Loop(const _float& fTimeDelta);
private:
	void					Input_Weapon();
	void					Output_Weapon();
	void					Attack_Shiraken();
	void					Attack_Fire();
	void					Jump();
	void					BackJump();
	void					AssMove();
	void					Dash();
	void					Fire();
private:
	void					Reset_Combo(const _float& fTimeDelta);
public:
	SCENEID&				GetCurScene() { return m_eCurScene; }
private:
	HRESULT					Ready_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
	HRESULT					SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
	HRESULT					SetUp_ConstantTable_Blur(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
	HRESULT					SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID);
private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CDynamic_Mesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CShader*				m_pShaderCom_Shadow = nullptr;
	CShader*				m_pShaderCom_Blur = nullptr;
	CShader*				m_pShaderCom_PostEffect = nullptr;
	CNavi*					m_pNaviCom = nullptr;
	CTexture*				m_pTextureCom_Hatch = nullptr;
	CCollider*				m_pColliderCom[2] = {nullptr, nullptr};
private:
	STATE_					m_eCurState = STATE_END;
	STATE_					m_ePreState = STATE_END;
	SCENEID					m_eCurScene = SCENE_END;
	NAVI					m_eNavi = NAVI::NAVI_END;
private:
	_float					m_fComboMaxTime = 1.;
	_float					m_fComboTime = 0.f;
private:
	_bool					m_isInputWeapon = false;
	_int					m_iCurComboCnt = -1;
	_int					m_iPreComboCnt = -1;
	_bool					m_isCombo = false;
private:
	_bool					m_isAttack = false;
private:
	_bool					m_isThrow = false;
	_bool					m_isMiniThrow = false;
private:
	_bool					m_isInput = true;
	_float					m_fInputTime = 0.f;
private://	ThrowPower
	const	float			m_fMaxThrowPower = 50.f;
	float					m_fThrowPower = 50.f;
public:
	_bool&					GetIsInputWeapon(){return m_isInputWeapon;}
	_bool&					GetIsThrow(){return m_isThrow;}
	_bool&					GetIsMiniThrow(){return m_isMiniThrow;}
	_bool					GetIsAttack() { return m_isAttack; }
	_float&					GetThrowPower(){return m_fThrowPower;}
	


private:
	bool		test_Input = false;
};

