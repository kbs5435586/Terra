#include "pch.h"
#include "Camera.h"
#include "Management.h"
#include "Device.h"

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
	, m_EffectCameraDesc(rhs.m_EffectCameraDesc)
	, m_matView(rhs.m_matView)
	, m_matProj(rhs.m_matProj)
	, m_matEffectView(rhs.m_matEffectView)
	, m_pInput_Device(rhs.m_pInput_Device)
{
	m_pInput_Device->AddRef();
}

HRESULT CCamera::Ready_Prototype()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matEffectView);

	ZeroMemory(&m_CameraDesc, sizeof(CAMERADESC));
	ZeroMemory(&m_ProjDesc, sizeof(PROJDESC));
	ZeroMemory(&m_EffectCameraDesc, sizeof(CAMERADESC));

	return NOERROR;
}

HRESULT CCamera::Ready_GameObject(void* pArg)
{
	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
		return E_FAIL;
	m_pEffectCameraTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pEffectCameraTransform)
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

HRESULT CCamera::SetUp_CameraProjDesc(const CAMERADESC & CameraDesc, 
	const PROJDESC & ProjDesc, const CAMERADESC& EffectCameraDesc)
{
	m_CameraDesc = CameraDesc;
	m_ProjDesc = ProjDesc;
	m_EffectCameraDesc = EffectCameraDesc;

	SetUp_ViewProjMatrices();

	return NOERROR;
}

void CCamera::Invalidate_ViewProjMatrix()
{
	m_matView = m_pTransform->Get_Matrix_Inverse();
	m_matEffectView = m_pEffectCameraTransform->Get_Matrix_Inverse();


	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_matProj);
	//m_pGraphic_Device->SetTransform(D3DTS_TEXTURE0, &m_matEffectView);
	CDevice::GetInstance()->m_matTempView = m_matEffectView;
}

HRESULT CCamera::SetUp_ViewProjMatrices()
{
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
	}

	{
		_vec3		vLook;
		vLook = m_EffectCameraDesc.vAt - m_EffectCameraDesc.vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3		vRight;
		D3DXVec3Cross(&vRight, &m_EffectCameraDesc.vAxisY, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);

		_vec3		vUp;
		D3DXVec3Cross(&vUp, &vLook, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		m_pEffectCameraTransform->Set_StateInfo(STATE_RIGHT, &vRight);
		m_pEffectCameraTransform->Set_StateInfo(STATE_UP, &vUp);
		m_pEffectCameraTransform->Set_StateInfo(STATE_LOOK, &vLook);
		m_pEffectCameraTransform->Set_StateInfo(STATE_POSITION, &m_EffectCameraDesc.vEye);
	}


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
	Safe_Release(m_pEffectCameraTransform);
	CGameObject::Free();
}