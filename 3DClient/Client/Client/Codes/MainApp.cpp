#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "Input.h"
#include "Management.h"
#include "Renderer.h"
#include "Scene_Logo.h"
#include "Camera_Debug.h"
#include "Camera_Light.h"

CMainApp::CMainApp()
	: m_pManagement(CManagement::GetInstance())
{
	m_pManagement->AddRef();
}

HRESULT CMainApp::Ready_MainApp()
{

	if (FAILED(Ready_Default_Setting(WINMODE_WIN, g_iBackCX, g_iBackCY)))
		return E_FAIL;
	if (FAILED(Ready_Render_State()))
		return E_FAIL;
	if (FAILED(Ready_Sampler_State()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;
	if (FAILED(Ready_Start_Scene(SCENE_LOGO)))
		return E_FAIL;

	srand(unsigned(time(NULL)));
	return S_OK;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	if (nullptr == m_pManagement)
		return -1;

	m_fTimeDelta = fTimeDelta;
	m_fTimeAcc += fTimeDelta;

	CInput::GetInstance()->SetUp_InputState();
	m_pManagement->Collision_Update(fTimeDelta);
	m_pManagement->Key_Update();

	return m_pManagement->Update_Management(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	if (nullptr == m_pGraphic_Device)
		return;
	if (nullptr == m_pManagement)
		return;

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphic_Device->BeginScene();
	if (nullptr != m_pRenderer)
		m_pRenderer->Render_RenderGroup();

	IDirect3DSurface9* offscreenSurface = 0;
	m_pGraphic_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &offscreenSurface);
	//D3DXSaveSurfaceToFile(L"test.bmp", D3DXIFF_BMP, offscreenSurface, 0, 0);


	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);

	++m_dwRenderCnt;
	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS:%d", m_dwRenderCnt);
		m_dwRenderCnt = 0;
		m_fTimeAcc = 0.f;
	}
	SetWindowText(g_hWnd, m_szFPS);
}

HRESULT CMainApp::Ready_Default_Setting(WINMODE eMode, const _uint & iSizeX, const _uint & iSizeY)
{
	if (FAILED(CDevice::GetInstance()->Ready_Graphic_Device(eMode, g_hWnd, iSizeX, iSizeY, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(CInput::GetInstance()->Ready_Input_Device(g_hInst, g_hWnd)))
		return E_FAIL;
	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{

	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_pRenderer->AddRef();
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Buffer_RcTex", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	return NOERROR;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	if (FAILED(m_pManagement->Add_Prototype_GameObject(L"GameObject_Camera_Debug", CCamera_Debug::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Prototype_GameObject(L"GameObject_Camera_Light", CCamera_Light::Create(m_pGraphic_Device))))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainApp::Ready_Start_Scene(SCENEID eSceneID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene*			pScene = nullptr;

	switch (eSceneID)
	{
	case SCENE_LOGO:
		pScene = CScene_Logo::Create(m_pGraphic_Device);
		break;
	case SCENE_STAGE_TOWN:
		break;
	case SCENE_STAGE_FIELD:
		break;
	case SCENE_STAGE_BOSS:
		break;
	}

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_ScenePointer(pScene)))
		return E_FAIL;

	Safe_Release(pScene);

	return S_OK;
}

HRESULT CMainApp::Ready_Render_State()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	return S_OK;
}

HRESULT CMainApp::Ready_Sampler_State()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MessageBox(0, L"CMainApp Created Failed", L"System Error", MB_OK);

		if (nullptr != pInstance)
		{
			delete pInstance;
			pInstance = nullptr;
		}
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pManagement);
	Safe_Release(m_pGraphic_Device);


	CManagement::Release_Engine();
}
