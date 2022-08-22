#include "stdafx.h"
#include "..\Headers\Camera_Debug.h"
#include "Management.h"



CCamera_Debug::CCamera_Debug(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Debug::CCamera_Debug(const CCamera_Debug & rhs)
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

	m_pTransform->SetUp_Speed(50.f, D3DXToRadian(360.f));
	m_pTransform->Set_StateInfo(STATE::STATE_POSITION, &_vec3(90.f,0.f,90.f));

	D3DVIEWPORT9			ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);
	m_ptMouse.x = ViewPort.Width >> 1;
	m_ptMouse.y = ViewPort.Height >> 1;

	ClientToScreen(g_hWnd, &m_ptMouse);

	return NOERROR;
}

_int CCamera_Debug::Update_GameObject(const _float & fTimeDelta)
{
	_long	MouseMove = 0;
	SetCursorPos(m_ptMouse.x, m_ptMouse.y);
	if (nullptr == m_pInput_Device)
		return -1;

	_vec3 vlook, vRight, vLookNormal, vUp, vPos, vSize;
	vUp = *m_pTransform->Get_StateInfo(STATE_UP);
	vRight = *m_pTransform->Get_StateInfo(STATE_RIGHT);
	vlook = *m_pTransform->Get_StateInfo(STATE_LOOK);
	D3DXVec3Normalize(&vLookNormal, &vlook);


	CTransform* pTransform = (CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC, L"Layer_Player", L"Com_Transform", 0);

	vPos = *pTransform->Get_StateInfo(STATE_POSITION) - *m_pTransform->Get_StateInfo(STATE_LOOK) * 20;

	m_pTransform->Set_StateInfo(STATE_POSITION,&vPos);


	//if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
	//	m_pTransform->Go_Right(fTimeDelta);
	//if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
	//	m_pTransform->Go_Left(fTimeDelta);
	//if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
	//	m_pTransform->Go_Straight(fTimeDelta);
	//if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
	//	m_pTransform->BackWard(fTimeDelta);
	if (MouseMove = m_pInput_Device->Get_DIMouseMove(DIM_X))
		m_pTransform->Rotation_Y(D3DXToRadian(MouseMove) * fTimeDelta);
	if (MouseMove = m_pInput_Device->Get_DIMouseMove(DIM_Y))
		m_pTransform->Rotation_Axis(D3DXToRadian(MouseMove) * fTimeDelta, m_pTransform->Get_StateInfo(STATE_RIGHT));

	Invalidate_ViewProjMatrix();
	return _int();
}

_int CCamera_Debug::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CCamera_Debug::Render_GameObject()
{
}

CCamera_Debug * CCamera_Debug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Debug*	pInstance = new CCamera_Debug(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);
	return pInstance;
}


CGameObject * CCamera_Debug::Clone_GameObject(void* pArg)
{
	CCamera_Debug*	pInstance = new CCamera_Debug(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

void CCamera_Debug::Free()
{
	CCamera::Free();
}