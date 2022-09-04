#include "stdafx.h"
#include "Arrow.h"
#include "Management.h"

 CArrow:: CArrow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

 CArrow:: CArrow(const  CArrow& rhs)
	: CGameObject(rhs)
{
}

HRESULT  CArrow::Ready_Prototype()
{
	return S_OK;
}

HRESULT  CArrow::Ready_GameObject(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;



	m_pTransformCom->SetUp_RotationY(D3DXToRadian(-90.f));
	m_pTransformCom->Rotation_Axis(D3DXToRadian(30.f), &_vec3(1.f, 0.f, 0.f));

	_matrix pTemp = *(_matrix*)pArg;
	_matrix matWorld = m_pTransformCom->Get_Matrix();
	pTemp._42 += 5.f;
	matWorld *= pTemp;
	m_pTransformCom->Set_Matrix(matWorld);

	m_pTransformCom->SetUp_Speed(100.f, D3DXToRadian(180.f));
	m_pTransformCom->Scaling(0.03f, 0.03f, 0.03f);

	m_vSize = m_pTransformCom->Get_Scale();
	m_eNavi = NAVI::NAVI_PLAYER;


	//if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_PostEffectBuffer", SCENE_STATIC, 
	//	L"Layer_PostEffect_Buffer", m_pTransformCom->Get_Matrix())))
	//	return E_FAIL;
	if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Effect_Fire_Tall", SCENE_STATIC,
		L"Layer_Fire_Tail", m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

_int  CArrow::Update_GameObject(const _float& fTimeDelta)
{
	
	_vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_RIGHT);
	_float fTemp = 0.f;
	_vec3 vSlide = {};

	if (m_pNaviCom->Move_OnNavigation(this->m_pTransformCom->Get_StateInfo(STATE_POSITION),
		&(vLook * 50.f * fTimeDelta), fTemp, m_eNavi, &vSlide))
	{
		_vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
		
		if (fTemp >= vPos.y)
		{
			m_fAccTime += fTimeDelta;
		}
		else
		{
			if (m_isFire)
			{
				m_pTransformCom->Go_Right(fTimeDelta);
			}
		}

		
	}

	if (m_fAccTime >= 5.f)
		return DEAD_OBJ;

	return _int();
}

_int  CArrow::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

void  CArrow::Render_GameObject()
{
	if (m_pShaderCom == nullptr || m_pMeshCom == nullptr)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->GetEffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	_ulong dwNumMaterials = m_pMeshCom->Get_NumMaterials();
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMaterials; i++)
	{
		if (FAILED(SetUp_ConstantTable(pEffect, i)))
			return;

		pEffect->CommitChanges();

		m_pMeshCom->Render_Mesh(i);
	}
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

 CArrow*  CArrow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	 CArrow* pInstance = new  CArrow(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		return nullptr;
	return pInstance;
}

CGameObject*  CArrow::Clone_GameObject(void* pArg)
{
	 CArrow* pInstance = new  CArrow(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		return nullptr;
	return pInstance;
}

void  CArrow::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pNaviCom);
	CGameObject::Free();
}

HRESULT  CArrow::Ready_Component()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	pManagement->AddRef();

	m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_EffectMesh_Arrow");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Arrow");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	_uint		iCurrentCellIdx = 0;
	m_pNaviCom = (CNavi*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Navi", &iCurrentCellIdx);
	if (FAILED(Add_Component(L"Com_Navi", m_pNaviCom)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT  CArrow::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	const SUBSETDESC* pSubSet = m_pMeshCom->Get_SubSetDesc(iAttributeID);
	if (nullptr == pSubSet)
		return E_FAIL;

	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture");

	pEffect->SetFloat("g_fAccTime", m_fAccTime);
	return S_OK;
}
