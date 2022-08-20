#include "pch.h"
#include "Camera_Debug.h"
#include "Management.h"

CCamera_Debug::CCamera_Debug(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Debug::CCamera_Debug(const CCamera_Debug& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Debug::Ready_Prototype()
{
	if (FAILED(CCamera::Ready_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCamera_Debug::Ready_GameObject(void* pArg)
{
	if (FAILED(CCamera::Ready_GameObject(pArg)))
		return E_FAIL;


	m_eRotate = ROTATE_DIR::DIR_END;
	m_eRotateUD = ROTATE_DIRUD::DIR_END_;
	return NOERROR;
}


_int CCamera_Debug::Update_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pInput_Device)
		return -1;
	m_pTransform->SetUp_Speed(fCameraSpeed, D3DXToRadian(90.f));
	_long	MouseMove = 0;
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();


	if (pManagement->KeyDown(KEY_F1))
		IsMouse ^= true;
	if (IsMouse)
	{

		if (pManagement->KeyPressing(KEY_LEFT))
			m_pTransform->Go_Left(fTimeDelta);
		if (pManagement->KeyPressing(KEY_RIGHT))
			m_pTransform->Go_Right(fTimeDelta);
		if (pManagement->KeyPressing(KEY_UP))
			m_pTransform->Go_Straight(fTimeDelta);
		if (pManagement->KeyPressing(KEY_DOWN))
			m_pTransform->BackWard(fTimeDelta);


		D3DVIEWPORT9			ViewPort;
		m_pGraphic_Device->GetViewport(&ViewPort);
		m_ptMouse.x = ViewPort.Width >> 1;
		m_ptMouse.y = ViewPort.Height >> 1;

		ClientToScreen(g_hWnd, &m_ptMouse);

		SetCursorPos(m_ptMouse.x, m_ptMouse.y);
		if (!IsCameraMove)
		{
			if (MouseMove = m_pInput_Device->Get_DIMouseMove(DIM_X))
				m_pTransform->Rotation_Y(D3DXToRadian(MouseMove) * fTimeDelta);
			if (MouseMove = m_pInput_Device->Get_DIMouseMove(DIM_Y))
				m_pTransform->Rotation_Axis(D3DXToRadian(MouseMove) * fTimeDelta, m_pTransform->Get_StateInfo(STATE_RIGHT));
		}

	}


	if (IsCameraMove)
	{
		Select_RotateDir(fTimeDelta);
	}

	Safe_Release(pManagement);
	Invalidate_ViewProjMatrix();
	return _int();
}

_int CCamera_Debug::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CCamera_Debug::Render_GameObject()
{
}

void CCamera_Debug::Compute_DIR_LR(_vec3& vCross)
{
	CTransform* pTargetTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC,
		L"Layer_Arrow", L"Com_Transform", g_iCameraMoveIdx);

	_vec3 vThisLook = *m_pTransform->Get_StateInfo(STATE::STATE_LOOK);
	_vec3 vThisPos = *m_pTransform->Get_StateInfo(STATE::STATE_POSITION);
	_vec3 vTargetPos = *pTargetTransform->Get_StateInfo(STATE::STATE_POSITION);

	vTargetPos.y = 0.f;
	vThisPos.y = 0.f;
	_vec3	vP_M = vTargetPos - vThisPos;
	D3DXVec3Normalize(&vP_M, &vP_M);

	_vec3	vTempLook = vThisLook;
	vTempLook.y = 0.f;
	D3DXVec3Normalize(&vTempLook, &vTempLook);
	m_fDot_LR = D3DXVec3Dot(&vTempLook, &vP_M);

	D3DXVec3Cross(&vCross, &vTempLook, &vP_M);
}

void CCamera_Debug::Compute_DIR_UD(_vec3& vCross)
{
	CTransform* pTargetTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC,
		L"Layer_Arrow", L"Com_Transform", g_iCameraMoveIdx);

	_vec3 vThisLook = *m_pTransform->Get_StateInfo(STATE::STATE_LOOK);
	_vec3 vThisPos = *m_pTransform->Get_StateInfo(STATE::STATE_POSITION);
	_vec3 vTargetPos = *pTargetTransform->Get_StateInfo(STATE::STATE_POSITION);

	vTargetPos.x = 0.f;
	vThisPos.x = 0.f;
	_vec3	vP_M = vTargetPos - vThisPos;
	D3DXVec3Normalize(&vP_M, &vP_M);

	_vec3	vTempLook = vThisLook;
	vTempLook.x = 0.f;
	D3DXVec3Normalize(&vTempLook, &vTempLook);
	m_fDot_UD = D3DXVec3Dot(&vTempLook, &vP_M);

	D3DXVec3Cross(&vCross, &vTempLook, &vP_M);
}


void CCamera_Debug::Select_RotateDir(const _float& fTimeDelta)
{
	if (CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->size() <= g_iCameraMoveIdx)
	{
		g_iCameraMoveIdx = 0;
		IsCameraMove = false;
	}
	_vec3 vCross_LR = {};
	_vec3 vCross_UD = {};

	Compute_DIR_LR(vCross_LR);
	Compute_DIR_UD(vCross_UD);

	if (!m_IsRotateEnd_LR)
	{
		if (vCross_LR.y > 0.f)
		{
			m_eRotate = ROTATE_DIR::DIR_RIGHT;
		}
		else
		{
			m_eRotate = ROTATE_DIR::DIR_LEFT;
		}

		m_IsRotateEnd_LR = true;
	}
	if (!m_IsRotateEnd_UD)
	{
		if (vCross_UD.x >= 0.f)
		{
			m_eRotateUD = ROTATE_DIRUD::DIR_UP;
		}
		else
		{
			m_eRotateUD = ROTATE_DIRUD::DIR_DOWN;
		}
		m_IsRotateEnd_UD = true;
	}





	if (m_IsRotateFinish_LR == false && m_IsRotateFinish_UD == false)
	{
		if (m_fDot_LR <= 1.01f && m_fDot_LR >= 0.99f)
		{
			
			m_IsRotateFinish_LR = true;
		}
		else
		{
			if (m_eRotate == ROTATE_DIR::DIR_LEFT)
				m_pTransform->Rotation_Y(-fTimeDelta);
			else if (m_eRotate == ROTATE_DIR::DIR_RIGHT)
				m_pTransform->Rotation_Y(fTimeDelta);
		}
	}



	if (m_IsRotateFinish_LR == true && m_IsRotateFinish_UD == false)
	{
		if (m_fDot_UD <= 1.01f && m_fDot_UD >= 0.99f)
		{
			
			m_IsRotateFinish_UD = true;
		}
		else
		{
		
			if (m_eRotateUD == ROTATE_DIRUD::DIR_DOWN)
				m_pTransform->Rotation_Axis(-fTimeDelta, m_pTransform->Get_StateInfo(STATE_RIGHT));
			else if (m_eRotateUD == ROTATE_DIRUD::DIR_UP)
				m_pTransform->Rotation_Axis(fTimeDelta, m_pTransform->Get_StateInfo(STATE_RIGHT));
		}
	}

	if (m_IsRotateFinish_LR == true && m_IsRotateFinish_UD == true)
	{
		_float	fLen = 0.f;
		CTransform* pTargetTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC,
			L"Layer_Arrow", L"Com_Transform", g_iCameraMoveIdx);
		_vec3 vPos = *m_pTransform->Get_StateInfo(STATE::STATE_POSITION);
		_vec3 vTargetPos = *pTargetTransform->Get_StateInfo(STATE::STATE_POSITION);

		fLen = D3DXVec3Length(&(vPos - vTargetPos));
		if (fLen >= 3.f)
		{
			m_pTransform->Go_ToTarget(&vTargetPos, fTimeDelta * 1.f);
			//m_IsRotateFinish_LR = false;
			//m_IsRotateFinish_UD = false;
			//m_IsRotateEnd_LR = false;
			//m_IsRotateEnd_UD = false;
		}
		else
		{
			m_IsRotateFinish_LR = false;
			m_IsRotateFinish_UD = false;
			m_IsRotateEnd_LR = false;
			m_IsRotateEnd_UD = false;
			if (CManagement::GetInstance()->Get_ObjectList(SCENE_STATIC, L"Layer_Arrow")->size() > g_iCameraMoveIdx)
				++g_iCameraMoveIdx;
		}
	}
}

CCamera_Debug* CCamera_Debug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Debug* pInstance = new CCamera_Debug(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);
	return pInstance;
}


CGameObject* CCamera_Debug::Clone_GameObject(void* pArg)
{
	CCamera_Debug* pInstance = new CCamera_Debug(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

void CCamera_Debug::Free()
{
	CCamera::Free();
}
