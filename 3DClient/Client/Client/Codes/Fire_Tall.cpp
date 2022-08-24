#include "stdafx.h"
#include "Fire_Tall.h"
#include "Management.h"
#include "Player.h"

CFire_Tall::CFire_Tall(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CFire_Tall::CFire_Tall(const CFire_Tall& rhs)
    : CGameObject(rhs)
{
}

HRESULT CFire_Tall::Ready_Prototype()
{
    return S_OK;
}

HRESULT CFire_Tall::Ready_GameObject(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;


    _matrix matTemp = *(_matrix*)pArg;
   
    m_pTransformCom->Scaling(2.f, 2.f, 2.f);
    _matrix matWorld = m_pTransformCom->Get_Matrix();

    matTemp._42 += 2.f;
    matWorld *= matTemp;
    m_pTransformCom->Set_Matrix(matWorld);
    m_pTransformCom->SetUp_Speed(100.f, D3DXToRadian(360.f));

    {
        m_tTexInfo.vScrollSpeed = _vec3(1.3f, 2.1f, 2.3f);
        m_tTexInfo.vScale = _vec3(1.f, 2.f, 3.f);


        m_tDistortion.fDistortion1 = _vec4(0.1f, 0.2f,0.f,0.f);
        m_tDistortion.fDistortion2 = _vec4(0.1f, 0.3f,0.f,0.f);
        m_tDistortion.fDistortion3 = _vec4(0.1f, 0.1f,0.f,0.f);
        m_tDistortion.fDistortionScale = 0.8f;
        m_tDistortion.fDistortionBias = 0.5f;
    }

    return S_OK;
}

_int CFire_Tall::Update_GameObject(const _float& fTimeDelta)
{
    m_pPlayer = (CPlayer*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Player");
    m_fLifeTime += fTimeDelta;

    m_pTransformCom->Go_Straight(fTimeDelta);
    //m_pTransformCom->Rotation_Z(fTimeDelta);

    if (m_fLifeTime >= 1.5f)
         return DEAD_OBJ;

    return NO_EVENT;
}

_int CFire_Tall::LastUpdate_GameObject(const _float& fTimeDelta)
{
    if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this)))
        return -1;

    m_tTexInfo.fFrameTime += fTimeDelta;

    if (m_tTexInfo.fFrameTime > 10.0f)
    {
        m_tTexInfo.fFrameTime = 0.0f;
    }

    return _int();
}

void CFire_Tall::Render_GameObject()
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

    if (nullptr != m_pColliderCom)
        m_pColliderCom->Render_Collider();

    Safe_Release(pEffect);
}

void CFire_Tall::Render_GameObject_PostEffect()
{

}

CFire_Tall* CFire_Tall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFire_Tall* pInstance = new CFire_Tall(pGraphic_Device);
    if (FAILED(pInstance->Ready_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CFire_Tall::Clone_GameObject(void* pArg)
{
    CFire_Tall* pInstance = new CFire_Tall(*this);
    if (FAILED(pInstance->Ready_GameObject(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CFire_Tall::Free()
{
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pMeshCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);
    for (int i = 0; i < 3; ++i)
    {
        Safe_Release(m_pTextureCom_Fire[i]);     
    }

    CGameObject::Free();
}

HRESULT CFire_Tall::Ready_Component()
{
    CManagement* pManagement = CManagement::GetInstance();
    if (nullptr == pManagement)
        return E_FAIL;
    pManagement->AddRef();

    m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
    if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
        return E_FAIL;

    m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
    if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
        return E_FAIL;

    m_pTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Trail");
    if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
        return E_FAIL;

    m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_FireTall");
    if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
        return E_FAIL;

    m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_EffectMesh_FireTall");
    if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
        return E_FAIL;

    m_pTextureCom_Fire[0] = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_FireAlpha");
    if (FAILED(Add_Component(L"Com_Texture_0", m_pTextureCom_Fire[0])))
        return E_FAIL;

    m_pTextureCom_Fire[1] = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Firefire");
    if (FAILED(Add_Component(L"Com_Texture_1", m_pTextureCom_Fire[1])))
        return E_FAIL;

    m_pTextureCom_Fire[2] = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Firenoise");
    if (FAILED(Add_Component(L"Com_Texture_2", m_pTextureCom_Fire[2])))
        return E_FAIL;

    _matrix			matLocalTransform;
    D3DXMatrixScaling(&matLocalTransform, 15.f, 10.f, 15.f);
    matLocalTransform._42 = 4.f;
    m_pColliderCom = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
        &COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, nullptr, m_pTransformCom->Get_Matrix_Pointer()));
    if (FAILED(Add_Component(L"Com_Collider_OBB", m_pColliderCom)))
        return E_FAIL;

    Safe_Release(pManagement);
    return S_OK;
}

HRESULT CFire_Tall::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
{
    m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

    _matrix		matView, matProj;

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

    pEffect->SetMatrix("g_matView", &matView);
    pEffect->SetMatrix("g_matProj", &matProj);

    D3DLIGHT9* pLightInfo = CLight_Mgr::GetInstance()->Get_LightInfo();
    if (nullptr == pLightInfo)
        return E_FAIL;

    const SUBSETDESC* pSubSet = m_pMeshCom->Get_SubSetDesc(iAttributeID);
    if (nullptr == pSubSet)
        return E_FAIL;

    pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pSubSet->Material.MatD3D.Specular);
    pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pSubSet->Material.MatD3D.Ambient);
    pEffect->SetFloat("g_fPower", pSubSet->Material.MatD3D.Power);

    m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", 6);
    m_pTextureCom_Fire[0]->SetUp_OnShader(pEffect, "g_texture1");
    m_pTextureCom_Fire[1]->SetUp_OnShader(pEffect, "g_texture2");
    m_pTextureCom_Fire[2]->SetUp_OnShader(pEffect, "g_texture3");

    {
        pEffect->SetFloat("g_fFrameTime", m_tTexInfo.fFrameTime);
        pEffect->SetVector("g_vScrollSpeed", &_vec4(m_tTexInfo.vScrollSpeed.x, m_tTexInfo.vScrollSpeed.y, m_tTexInfo.vScrollSpeed.z, 1.f));
        pEffect->SetVector("g_vScale", &_vec4(m_tTexInfo.vScale.x, m_tTexInfo.vScale.y, m_tTexInfo.vScale.z, 1.f));

        pEffect->SetVector("g_fDistortion1", &m_tDistortion.fDistortion1);
        pEffect->SetVector("g_fDistortion2", &m_tDistortion.fDistortion2);
        pEffect->SetVector("g_fDistortion3", &m_tDistortion.fDistortion3);

        pEffect->SetFloat("g_fDistortionScale", m_tDistortion.fDistortionScale);
        pEffect->SetFloat("g_fDistortionBias", m_tDistortion.fDistortionBias);
    }

    return S_OK;
}
