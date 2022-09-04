#pragma once
#include "Base.h"
class CComponent;
class CTransform;
class CCollider;
class CGameObject :
	public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	enum ROTATE_DIR { DIR_LEFT, DIR_RIGHT, DIR_END };
public:
	virtual HRESULT							Ready_GameObject(void* pArg);
	virtual _int							Update_GameObject(const _float& fTimeDelta);
	virtual _int							LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void							Render_GameObject();
	virtual void							Render_GameObject_Shadow();
	virtual void							Render_GameObject_Blur();
	virtual void							Render_GameObject_PostEffect();
public:
	void									Create_Particle();
	void									Create_Particle(const _float& fTimeDelta, const _float& fperiod=0.05f);
protected:
	void									Obb_Collision(CTransform* pTransform, const _float& fAddY = 0.f);
	void									Hit_Object(CTransform* pTransform, _float& fCnt, _vec3 vStart, _vec3 vEnd, _vec3 vMid);
	void									Hit_Object(CTransform* pTransform, _float& fCnt, _vec3 vStart, _vec3 vEnd, _vec3 vMid, const float& fTimeDelta, const _vec3& vSizs);
	void									Hit_Arrow(CTransform* pTransform, _float& fCnt, _vec3 vStart, _vec3 vEnd, _vec3 vMid, const float& fTimeDelta, const _vec3& vSizs);
public:
	LPDIRECT3DDEVICE9						Get_Device() { return m_pGraphic_Device; }
public:
	CComponent*								Get_ComponentPointer(const _tchar* pComponentTag);
protected:
	LPDIRECT3DDEVICE9						m_pGraphic_Device = nullptr;
protected:
	map<const _tchar*, CComponent*>			m_mapComponent;
protected:
	HRESULT									Add_Component(const _tchar* pComponentTag, CComponent* pComponent);
	CComponent*								Find_Component(const _tchar* pComponentTag);
public:
	virtual CGameObject*					Clone_GameObject(void* pArg) = 0;
	const _tchar*							m_pCurrent_TextureTag = L"";
public:
	const _tchar*							Get_Current_TextureTag() { return m_pCurrent_TextureTag; }
protected:		//UI
	_float									m_fX, m_fY, m_fSizeX, m_fSizeY;
protected:
	_vec3									m_vecPickingPos = {};
	_bool									m_IsPick = false;
	_uint									m_iAnimationNum = 0;
	_uint									m_iNewAnimationNum = 0;
protected:
	_bool									m_IsUI = false;
	_float									m_fMove = 1.f;
protected:
	_bool									m_IsLoadUI_Texture = false;
protected:
	_tchar*									m_pGameObject_Tag = L"";
	_tchar*									m_pComponent_Tag = L"";
protected:
	_float									m_fTimeDelta = 0.f;
	_double									m_fAnimTime = 0.f;
	_float									m_fAccTime = 0.f;
protected:
	UI										m_eUI = UI::UI_END;
protected://Animation
	_bool									m_IsOnce = false;
	_bool									m_isIdle = false;
	_float									m_fOnIdleTime = 0.f;
protected://UnitInfo
	UNIT_INFO								m_tInfo;
protected://OBB Collision
	_bool									m_IsOBB_Collision = false;
	_bool									m_IsHit = false;
	_float									m_fBazierCnt = 0.f;
	_bool									m_IsBazier = false;
	_vec3									m_vStartPoint = {};
	_vec3									m_vEndPoint = {};
	_vec3									m_vMidPoint = {};
	_float									m_fCollisionAccTime = 0.f;
	_vec3									m_vCollisionPos = {};
	_bool									m_IsPlayer_Particle = false;
protected://Rotate
	ROTATE_DIR								m_eRotate;
	_bool									m_isRotateEnd = false;
	_bool									m_isRotateFinish = false;
	_bool									m_isDest = false;
	_float									m_fRotateDot = 0.f;
	_vec3									m_vRotateCross = {};
	_vec3									m_vDest = {};
protected://PartList
	vector<pair<string,int>>				m_vecPartList;
	vector<_tchar*>							m_vecPartName;
	vector<CCollider*>						m_vecCollider_Bone;
protected://Blur
	_matrix									m_matOldWorld;
	_matrix									m_matOldView;
	_float									m_fBlurTime = 0.f;
protected:
	_matrix									m_matParent = {};;
	_matrix									m_matTrail;
	_matrix									m_matTrail_Out;
	_float									m_fFrame = 0.f;
protected:
	_vec3									m_vSize;
protected:
	NAVI									m_eNavi = NAVI::NAVI_END;
public:
	vector<CCollider*>&						GetColliderBone(){return m_vecCollider_Bone;}
public:
	_bool&									GetIsPick() { return m_IsPick; }
	_bool&									GetIsUI() { return m_IsUI; }
	_bool&									GetLoadUI_Texture() { return m_IsLoadUI_Texture; }
	_float&									Get_FX() { return m_fX; }
	_float&									Get_FY() { return m_fY; }
	_float&									Get_SizeX() { return m_fSizeX; }
	_float&									Get_SizeY() { return m_fSizeY; }
	_tchar*									Get_GameObject_Tag() { return m_pGameObject_Tag; }
	_tchar*&								Get_Component_Tag() { return m_pComponent_Tag; }
	UI&										GetEnumUI() { return m_eUI; }
	UNIT_INFO&								GetUnit_Info() { return m_tInfo; }
	_bool&									GetOBBCollision() { return m_IsOBB_Collision; }
	_bool&									GetIsHit(){return m_IsHit;}
	_matrix									GetParentMat() { return m_matParent; }
	_matrix									GetTrailMat() { return m_matTrail; }
	_float&									GetCollisionAccTime() {return m_fCollisionAccTime;	}
	_vec3&									GetCollisionPos(){return m_vCollisionPos;}
	_bool&									GetIsPlayerCollision(){return m_IsPlayer_Particle;}
	_float&									GetAccTime() {return m_fAccTime;}
public:
	_float									GetRandom_Float(_float fLowBound, _float fHighBound);
	void									GetRandom_Vector(_vec3& vOut, _vec3& vMin, _vec3& vMax);
protected:
	virtual void							Free();
};

