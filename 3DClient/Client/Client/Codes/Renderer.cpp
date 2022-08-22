#include "stdafx.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Shader.h"
#include "Light_Mgr.h"
#include "Management.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
{
	m_pTarget_Manager->AddRef();
}

HRESULT CRenderer::Ready_Renderer()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3DVIEWPORT9			ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	// For.Target_Diffuse
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Diffuse", ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Diffuse", 0.0f, 0.f, 200.f, 200.f)))
		return E_FAIL;

	// For.Target_Normal
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Normal", 0.0f, 200.f, 200.f, 200.f)))
		return E_FAIL;

	// For.Target_Depth
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Depth", 0.0f, 400.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Position", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Position", 0.0f, 600.f, 200.f, 200.f)))
		return E_FAIL;


	// For.Target_Shade
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Shade", 200.0f, 0.f, 200.f, 200.f)))
		return E_FAIL;
	// For.Target_Specular
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Specular", 200.0f, 200.f, 200.f, 200.f)))
		return E_FAIL;

	// For.Target_Shadow
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Shadow", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Shadow", 400.f, 0.f, 200.f, 200.f)))
		return E_FAIL;

	// For.Target_Blur
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Blur", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Blur", 400.f, 200.f, 200.f, 200.f)))
		return E_FAIL;
	// For.Target_Effect_Hatch
	if (FAILED(m_pTarget_Manager->Add_Target(m_pGraphic_Device, L"Target_Effect_Trail", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Effect_Trail", 600.f, 0.f, 200.f, 200.f)))
		return E_FAIL;



	// For.MRT_Deferred	
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;	
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Position")))
		return E_FAIL;

	// For.MRT_LightAcc
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Shadow", L"Target_Shadow")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Blur", L"Target_Blur")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Effect", L"Target_Effect_Trail")))
		return E_FAIL;


	// For.Shader_LightAcc
	m_pShaderCom_LightAcc = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_LightAcc.fx");
	if (nullptr == m_pShaderCom_LightAcc)
		return E_FAIL;
	// For.Shader_Blend
	m_pShaderCom_Blend = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Blend.fx");
	if (nullptr == m_pShaderCom_Blend)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXVIEWPORT) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXVIEWPORT* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(0.f - 0.5f, 0.0f - 0.5f, 0.0f, 1.f);
	pVertices[0].vTexUV = _vec2(0.0f, 0.f);

	pVertices[1].vPosition = _vec4(ViewPort.Width - 0.5f, 0.0f - 0.5f, 0.0f, 1.f);
	pVertices[1].vTexUV = _vec2(1.0f, 0.f);

	pVertices[2].vPosition = _vec4(ViewPort.Width - 0.5f, ViewPort.Height - 0.5f, 0.0f, 1.f);
	pVertices[2].vTexUV = _vec2(1.0f, 1.f);

	pVertices[3].vPosition = _vec4(0.0f - 0.5f, ViewPort.Height - 0.5f, 0.0f, 1.f);
	pVertices[3].vTexUV = _vec2(0.0f, 1.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(sizeof(POLYGON16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;
	POLYGON16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (RENDER_END <= eGroup)
		return E_FAIL;

	if (nullptr == pGameObject)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	pGameObject->AddRef();
	return S_OK;
}

HRESULT CRenderer::Render_RenderGroup()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	Render_Priority();
	Render_Effect();

	Render_Deferred();
	Render_Shadow();
	Render_Blur();
	Render_LightAcc();
	Render_Blend();
	

	Render_Alpha();
	Render_UI();

	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Deferred");
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_LightAcc");
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Shadow");
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Blur");
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Effect");
	m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Post_Effect");

	return S_OK;
}

void CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_PRIORITY].clear();
}

void CRenderer::Render_NoneAlpha()
{
	for (auto& pGameObject : m_RenderList[RENDER_NONEALPHA])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_NONEALPHA].clear();
}

void CRenderer::Render_Alpha()
{
	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_ALPHA].clear();
}

void CRenderer::Render_UI()
{
	sort(m_RenderList[RENDER_UI].begin(), m_RenderList[RENDER_UI].end(),
		[](CGameObject* pGameObject1, CGameObject* pGameObject2) {return pGameObject1->GetEnumUI() < pGameObject2->GetEnumUI(); }
	);

	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_UI].clear();
}

void CRenderer::Render_Deferred()
{
	m_pTarget_Manager->AddRef();
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Deferred")))
		return;

	Render_NoneAlpha();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Deferred")))
		return;
	Safe_Release(m_pTarget_Manager);
}

void CRenderer::Render_LightAcc()
{
	if (nullptr == m_pTarget_Manager ||
		nullptr == m_pShaderCom_LightAcc)
		return;
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc")))
		return;
	LPD3DXEFFECT pEffect = m_pShaderCom_LightAcc->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Normal", "g_NormalTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Position", "g_PositionTex")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Shadow", "g_ShadowTex")))
		return;


	pEffect->Begin(nullptr, 0);
	CLight_Mgr::GetInstance()->Render_Light(pEffect);
	pEffect->End();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_LightAcc")))
		return;

	Safe_Release(pEffect);
}

void CRenderer::Render_Blend()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pShaderCom_Blend ||
		nullptr == m_pTarget_Manager)
		return;



	LPD3DXEFFECT pEffect = m_pShaderCom_Blend->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Diffuse", "g_DiffuseTexture")))
		return;	
	//if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Effect_Hatch", "g_DiffuseTexture")))
	//	return;

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Shade", "g_ShadeTexture")))
		return;

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Specular", "g_SpecularTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Shadow", "g_ShadowTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Blur", "g_BlurTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Effect_Trail", "g_EffectTexture")))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	pEffect->SetMatrix("g_matShadowView", &CManagement::GetInstance()->GetShadowViewMatrix());
	pEffect->SetMatrix("g_matShadowProj", &CManagement::GetInstance()->GetShadowProjMatrix());

	// 임의의 버퍼를 렌더링한다. == 백버퍼에 그린다.
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();





	Safe_Release(pEffect);
}

void CRenderer::Render_Shadow()
{
	m_pTarget_Manager->AddRef();
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Shadow")))
		return;

	for (auto& pGameObject : m_RenderList[RENDER_SHADOW])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject_Shadow();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_SHADOW].clear();
	

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Shadow")))
		return;
	Safe_Release(m_pTarget_Manager);
}

void CRenderer::Render_Blur()
{
	m_pTarget_Manager->AddRef();
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Blur")))
		return;

	for (auto& pGameObject : m_RenderList[RENDER_BLUR])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject_Blur();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_BLUR].clear();


	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Blur")))
		return;
	Safe_Release(m_pTarget_Manager);
}

void CRenderer::Render_Effect()
{

	m_pTarget_Manager->AddRef();
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Effect")))
		return;

	for (auto& pGameObject : m_RenderList[RENDER_EFFECT])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject_Effect();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_EFFECT].clear();


	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Effect")))
		return;
	Safe_Release(m_pTarget_Manager);
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*	pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		MessageBox(0, L"CRenderer Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRenderer::Clone_Component(void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);


	Safe_Release(m_pShaderCom_Blend);
	Safe_Release(m_pShaderCom_LightAcc);
	//Safe_Release(m_pShaderCom_Shadow);
	Safe_Release(m_pTarget_Manager);
	CComponent::Free();
}
