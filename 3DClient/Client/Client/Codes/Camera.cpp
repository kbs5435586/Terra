#include "stdafx.h"
#include "..\Headers\Camera.h"
#include "Management.h"
CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_pInput_Device(CInput::GetInstance())
{
	m_pInput_Device->AddRef();
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_CameraDesc(rhs.m_CameraDesc)
	, m_ProjDesc(rhs.m_ProjDesc)
	, m_matView(rhs.m_matView)
	, m_matProj(rhs.m_matProj)
	, m_pInput_Device(rhs.m_pInput_Device)
{
	m_pInput_Device->AddRef();
}

HRESULT CCamera::Ready_Prototype()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	ZeroMemory(&m_CameraDesc, sizeof(CAMERADESC));
	ZeroMemory(&m_ProjDesc, sizeof(PROJDESC));

	return NOERROR;
}

HRESULT CCamera::Ready_GameObject(void* pArg)
{
	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
		return E_FAIL;

	if (FAILED(Add_Component(L"Com_Transform", m_pTransform)))
		return E_FAIL;

	return NOERROR;
}

_int CCamera::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CCamera::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CCamera::Render_GameObject()
{
}

HRESULT CCamera::SetUp_CameraProjDesc(const CAMERADESC & CameraDesc, const PROJDESC & ProjDesc)
{
	m_CameraDesc = CameraDesc;
	m_ProjDesc = ProjDesc;

	SetUp_ViewProjMatrices();

	return NOERROR;
}

void CCamera::Invalidate_ViewProjMatrix()
{
	m_matView = m_pTransform->Get_Matrix_Inverse();

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

HRESULT CCamera::SetUp_CameraProjDesc(const CAMERADESC& CameraDesc, const PROJDESC& ProjDesc, _bool IsShadow)
{
	m_tShadowCameraDesc = CameraDesc;
	m_tShadowProjDesc = ProjDesc;
	SetUp_ViewProjMatrices(IsShadow);

	return S_OK;
}

HRESULT CCamera::SetUp_ViewProjMatrices(_bool IsShadow)
{
	_vec3		vLook;
	vLook = m_tShadowCameraDesc.vAt - m_tShadowCameraDesc.vEye;
	D3DXVec3Normalize(&vLook, &vLook);


	_vec3		vRight;
	D3DXVec3Cross(&vRight, &m_tShadowCameraDesc.vAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_vec3		vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransform->Set_StateInfo(STATE::STATE_RIGHT, &vRight);
	m_pTransform->Set_StateInfo(STATE::STATE_UP, &vUp);
	m_pTransform->Set_StateInfo(STATE::STATE_LOOK, &vLook);
	m_pTransform->Set_StateInfo(STATE::STATE_POSITION, (const _vec3*)&m_tShadowCameraDesc.vEye);

	CManagement::GetInstance()->GetShadowWorlsMatrix() = m_pTransform->Get_Matrix();
	Invalidate_ViewProjMatrix(IsShadow);
	return S_OK;
}

void CCamera::Invalidate_ViewProjMatrix(_bool IsShadow)
{
	//m_tShadowCameraDesc.vEye.x = -1000.f ;
	//m_tShadowCameraDesc.vAt.x = 1.f ;
	_vec3		vLook;
	vLook = m_tShadowCameraDesc.vAt - m_tShadowCameraDesc.vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3		vRight;
	D3DXVec3Cross(&vRight, &m_tShadowCameraDesc.vAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_vec3		vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransform->Set_StateInfo(STATE::STATE_RIGHT, &vRight);
	m_pTransform->Set_StateInfo(STATE::STATE_UP, &vUp);
	m_pTransform->Set_StateInfo(STATE::STATE_LOOK, &vLook);
	m_pTransform->Set_StateInfo(STATE::STATE_POSITION, (const _vec3*)&m_tShadowCameraDesc.vEye);

	CManagement::GetInstance()->GetShadowWorlsMatrix()= m_pTransform->Get_Matrix();

	_vec3 vPos = *dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform", 0))->
		Get_StateInfo(STATE::STATE_POSITION);
	
	vPos.x -= 100.f;
	vPos.y += 100.f;
	vPos.z -= 100.f;
	m_pTransform->Set_StateInfo(STATE::STATE_POSITION, &vPos);

	

	m_matShadowView = m_pTransform->Get_Matrix_Inverse();
	D3DXMatrixOrthoLH(&m_matShadowProj,(_float)120.f, (_float)120.f, 0.1f, 100000.f);
	//D3DXMatrixOrthoLH(&m_matShadowProj,(_float)70.f, (_float)70.f, 1.f, 10000.f);

	CManagement::GetInstance()->GetShadowViewMatrix()= m_matShadowView;
	CManagement::GetInstance()->GetShadowProjMatrix()= m_matShadowProj;
}

HRESULT CCamera::SetUp_ViewProjMatrices()
{
	_vec3		vLook;
	vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3		vRight;
	D3DXVec3Cross(&vRight, &m_CameraDesc.vAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_vec3		vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransform->Set_StateInfo(STATE_RIGHT, &vRight);
	m_pTransform->Set_StateInfo(STATE_UP, &vUp);
	m_pTransform->Set_StateInfo(STATE_LOOK, &vLook);
	m_pTransform->Set_StateInfo(STATE_POSITION, &m_CameraDesc.vEye);

	m_matProj._11 = (1.f / tan(m_ProjDesc.fFovY * 0.5f)) / m_ProjDesc.fAspect;
	m_matProj._22 = 1.f / tan(m_ProjDesc.fFovY * 0.5f);
	m_matProj._33 = m_ProjDesc.fFar / (m_ProjDesc.fFar - m_ProjDesc.fNear);
	m_matProj._43 = (m_ProjDesc.fFar * m_ProjDesc.fNear) / (m_ProjDesc.fFar - m_ProjDesc.fNear) * -1.f;
	m_matProj._34 = 1.f;
	m_matProj._44 = 0.0f;
	Invalidate_ViewProjMatrix();

	return NOERROR;
}

void CCamera::Free()
{
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTransform);
	CGameObject::Free();
}