#include "pch.h"
#include "Picking.h"


CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CPicking::CPicking(const CPicking & rhs)
	: CComponent(rhs)
	, m_hWnd(rhs.m_hWnd)
{

}

HRESULT CPicking::Ready_Picking(HWND hWnd)
{
	m_hWnd = hWnd;
	return S_OK;
}

void CPicking::Update_Ray()
{
	D3DVIEWPORT9		ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	POINT		ptMouse;
	GetCursorPos(&ptMouse);

	// ViewPort�������� ���콺 ��ǥ
	ScreenToClient(m_hWnd, &ptMouse);

	// ���������̽� �������� ���콺 ��ǥ�� ������.
	_vec4	vMousePos;
	vMousePos.x = (_float)ptMouse.x / (ViewPort.Width >> 1) - 1.f;
	vMousePos.y = (_float)ptMouse.y / ((ViewPort.Height >> 1) * -1.f) + 1.f;
	vMousePos.z = 0.0f;
	vMousePos.w = 1.f;

	// �佺���̽� �������� ���콺 ��ǥ.
	_matrix	matProj;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec4Transform(&vMousePos, &vMousePos, &matProj);


	// ���콺 ������ ������.
	_vec3		vCameraPos(0.f, 0.f, 0.f);

	// ���콺 ����.
	_vec3		vMouseRay = _vec3(vMousePos.x, vMousePos.y, vMousePos.z) - vCameraPos;

	// ���彺���̽� �������� ���̿� ������.
	_matrix	matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&m_vRayPivot, &vCameraPos, &matView);
	D3DXVec3TransformNormal(&m_vRay, &vMouseRay, &matView);
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd)
{
	CPicking*	pInstance = new CPicking(pGraphic_Device);

	if (FAILED(pInstance->Ready_Picking(hWnd)))
	{
		MessageBox(0, L"CPicking Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CPicking::Clone_Component(void* pArg)
{
	return new CPicking(*this);
}

void CPicking::Free()
{
	CComponent::Free();
}
