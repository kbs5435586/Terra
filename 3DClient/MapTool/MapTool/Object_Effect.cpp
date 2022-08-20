#include "pch.h"
#include "Object_Effect.h"
#include "Management.h"
#include "Device.h"

CObject_Effect::CObject_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CObject_Effect::CObject_Effect(const CObject_Effect& rhs)
    : CGameObject(rhs)
{

}

HRESULT CObject_Effect::Ready_Prototype()
{
    return S_OK;
}

HRESULT CObject_Effect::Ready_GameObject(void* pArg)
{
	if (nullptr != pArg)
	{
		CString temp = *(CString*)pArg;
		if (FAILED(Ready_Component(temp)))
			return E_FAIL;
	}

	m_pTransformCom->Set_StateInfo(STATE_POSITION, &_vec3(0.f, 0.f,0.f));
	m_pTransformCom->SetUp_Speed(5.0f, D3DXToRadian(90.0f));
	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);
	m_IsStatic = true;
	return S_OK;
}

_int CObject_Effect::Update_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	if (IsRotation_EffectMesh)
	{
		if (m_iPreRotNum != g_iEffectRotateNum)
		{
			m_pTransformCom->SetUp_RotationX(D3DXToRadian(0.f));
			m_pTransformCom->SetUp_RotationY(D3DXToRadian(0.f));
			m_pTransformCom->SetUp_RotationZ(D3DXToRadian(0.f));
		
			m_iPreRotNum = g_iEffectRotateNum;
		}


		if (g_iEffectRotateNum == 0)
		{
			m_pTransformCom->Rotation_X(fTimeDelta);
		}
		else if (g_iEffectRotateNum == 1)
		{
			m_pTransformCom->Rotation_Y(fTimeDelta);
		}
		else if (g_iEffectRotateNum == 2)
		{
			m_pTransformCom->Rotation_Z(fTimeDelta);
		}
	
	}
	m_pTransformCom->Set_StateInfo(STATE_POSITION, &_vec3(fEffectMeshX, fEffectMeshY, fEffectMeshZ));
    return _int();
}

_int CObject_Effect::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_tTexInfo.fFrameTime += fTimeDelta;

	if (m_tTexInfo.fFrameTime > 100.f)
	{
		m_tTexInfo.fFrameTime = 0.0f;
	}
	m_tTexInfo.vScrollSpeed = _vec3(1.3f, 2.1f, 2.3f);
	m_tTexInfo.vScale = _vec3(1.f, 2.f, 3.f);

	m_tDistortion.fDistortion1 = _vec2(0.1f, 0.2f);
	m_tDistortion.fDistortion2 = _vec2(0.1f, 0.3f);
	m_tDistortion.fDistortion3 = _vec2(0.1f, 0.1f);
	m_tDistortion.fDistortionScale = 0.8f;
	m_tDistortion.fDistortionBias = 0.5f;

	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_EFFECT, this)))
		return -1;
	return _int();
}

void CObject_Effect::Render_GameObject()
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

HRESULT CObject_Effect::Ready_Component(const _tchar* pComponentTag)
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

	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	m_pPickingCom = (CPicking*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Picking");
	if (FAILED(Add_Component(L"Com_Picking", m_pPickingCom)))
		return E_FAIL;

	m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, pComponentTag);
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pTextureCom_Fire = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Fire");
	if (FAILED(Add_Component(L"Com_Texture_Fire", m_pTextureCom_Fire)))
		return E_FAIL;
	
	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CObject_Effect::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	//m_pGraphic_Device->GetTransform(D3DTS_TEXTURE0, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	matView = CDevice::GetInstance()->m_matTempView;

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetInt("g_iPick", m_IsPick);

	pEffect->SetFloatArray("fDistortion1", m_tDistortion.fDistortion1, 2);
	pEffect->SetFloatArray("fDistortion2", m_tDistortion.fDistortion2, 2);
	pEffect->SetFloatArray("fDistortion3", m_tDistortion.fDistortion3, 2);
	pEffect->SetFloat("fDistortionScale", m_tDistortion.fDistortionScale);
	pEffect->SetFloat("fDistortionBias", m_tDistortion.fDistortionBias);
	
	pEffect->SetFloat("fFrameTime", m_tTexInfo.fFrameTime);
	pEffect->SetFloatArray("vScrollSpeed", m_tTexInfo.vScrollSpeed,3);
	pEffect->SetFloatArray("vScale", m_tTexInfo.vScale,3);





	D3DLIGHT9* pLightInfo = CLight_Mgr::GetInstance()->Get_LightInfo();
	if (nullptr == pLightInfo)
		return E_FAIL;

	const SUBSETDESC* pSubSet = m_pMeshCom->Get_SubSetDesc(iAttributeID);
	if (nullptr == pSubSet)
		return E_FAIL;

	pEffect->SetTexture("g_DiffuseTexture", pSubSet->MeshTexture.pDiffuseTexture);

	m_pTextureCom_Fire->SetUp_OnShader(pEffect, "g_FireAlpha", 0);
	m_pTextureCom_Fire->SetUp_OnShader(pEffect, "g_Fire",1);
	m_pTextureCom_Fire->SetUp_OnShader(pEffect, "g_FireNoise", 1);




	return S_OK;
}

CObject_Effect* CObject_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CObject_Effect* pInstance = new CObject_Effect(pGraphic_Device);
	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);
	return pInstance;
}

CGameObject* CObject_Effect::Clone_GameObject(void* pArg)
{
	CObject_Effect* pInstance = new CObject_Effect(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

void CObject_Effect::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPickingCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTextureCom_Fire);
	CGameObject::Free();
}
