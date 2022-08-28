#include "stdafx.h"
#include "WaveRing.h"
#include "Management.h"
#include "Player.h"
#include "Target_Manager.h"
#include "Target.h"


CWaveRing::CWaveRing(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CWaveRing::CWaveRing(const CWaveRing& rhs)
    : CGameObject(rhs)
{
}

HRESULT CWaveRing::Ready_Prototype()
{
    return S_OK;
}

HRESULT CWaveRing::Ready_GameObject(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    //FIRE_BALL tFireBall = *(FIRE_BALL*)pArg;
    //_matrix matTemp = tFireBall.matWorld;
    //m_iFirBallIdx = tFireBall.iIdx;


   // m_pTransformCom->Scaling(2.f, 2.f, 2.f);
    _matrix matWorld = m_pTransformCom->Get_Matrix();


    m_pTransformCom->SetUp_Speed(100.f, D3DXToRadian(60.f));
    _vec3 vPos = *(_vec3*)(pArg);
    m_pTransformCom->Set_StateInfo(STATE_POSITION, &vPos);

    m_vSize = m_pTransformCom->Get_Scale();

    m_pTransformCom->SetUp_RotationY(D3DXToRadian(90.f));


    {
        m_tTexInfo.vScrollSpeed = _vec3(1.3f, 2.1f, 2.3f);
        m_tTexInfo.vScale = _vec3(1.f, 2.f, 3.f);


        m_tDistortion.fDistortion1 = _vec4(0.1f, 0.2f, 0.f, 0.f);
        m_tDistortion.fDistortion2 = _vec4(0.1f, 0.3f, 0.f, 0.f);
        m_tDistortion.fDistortion3 = _vec4(0.1f, 0.1f, 0.f, 0.f);
        m_tDistortion.fDistortionScale = 2.f;
        m_tDistortion.fDistortionBias = 0.5f;
    }
    m_iRandidx = rand() % 2;
    return S_OK;
}

_int CWaveRing::Update_GameObject(const _float& fTimeDelta)
{
    m_pPlayer = (CPlayer*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Player");
    m_fAccTime += fTimeDelta*0.5f;
    m_fLifeTime += fTimeDelta;
    //m_pTransformCom->Rotation_Y(fTimeDelta);

    m_pTransformCom->Scaling(m_fAccTime, m_fAccTime, m_fAccTime);


    if (m_fLifeTime >= 1.5f)
        return DEAD_OBJ;


    return NO_EVENT;
}

_int CWaveRing::LastUpdate_GameObject(const _float& fTimeDelta)
{
    //if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this)))
    //    return -1;
    if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_POSTEFFECT, this)))
        return -1;

    m_tTexInfo.fFrameTime += fTimeDelta;

    if (m_tTexInfo.fFrameTime > 10.0f)
    {
        m_tTexInfo.fFrameTime = 0.0f;
    }

    return _int();
}

void CWaveRing::Render_GameObject()
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

void CWaveRing::Render_GameObject_PostEffect()
{
    if (m_pShaderCom_PostEffect == nullptr || m_pMeshCom == nullptr)
        return;

    LPD3DXEFFECT	pEffect = m_pShaderCom_PostEffect->GetEffectHandle();
    if (pEffect == nullptr)
        return;

    pEffect->AddRef();

    _ulong dwNumMaterials = m_pMeshCom->Get_NumMaterials();
    pEffect->Begin(nullptr, 0);
    pEffect->BeginPass(0);

    for (size_t i = 0; i < dwNumMaterials; i++)
    {
        if (FAILED(SetUp_ConstantTable_PostEffect(pEffect, i)))
            return;

        pEffect->CommitChanges();

        m_pMeshCom->Render_Mesh(i);
    }
    pEffect->EndPass();
    pEffect->End();



    Safe_Release(pEffect);
}

CWaveRing* CWaveRing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWaveRing* pInstance = new CWaveRing(pGraphic_Device);
    if (FAILED(pInstance->Ready_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CWaveRing::Clone_GameObject(void* pArg)
{
    CWaveRing* pInstance = new CWaveRing(*this);
    if (FAILED(pInstance->Ready_GameObject(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CWaveRing::Free()
{
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pMeshCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom_PostEffect);


    CGameObject::Free();
}

HRESULT CWaveRing::Ready_Component()
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

    m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_EffectMesh_WaveRing");
    if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
        return E_FAIL;

    m_pShaderCom_PostEffect = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Effect");
    if (FAILED(Add_Component(L"Com_Shader_PostEffect", m_pShaderCom_PostEffect)))
        return E_FAIL;


    Safe_Release(pManagement);
    return S_OK;
}

HRESULT CWaveRing::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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

HRESULT CWaveRing::SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
{
    m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

    _matrix		matView, matProj;
    _matrix		matLightView, matLightProj;

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

    pEffect->SetMatrix("g_matView", &matView);
    pEffect->SetMatrix("g_matProj", &matProj);

    matLightView = CManagement::GetInstance()->GetShadowViewMatrix();
    matLightProj = CManagement::GetInstance()->GetShadowProjMatrix();

    const SUBSETDESC* pSubSet = m_pMeshCom->Get_SubSetDesc(iAttributeID);
    if (nullptr == pSubSet)
        return E_FAIL;

    pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pSubSet->Material.MatD3D.Specular);
    pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pSubSet->Material.MatD3D.Ambient);
    pEffect->SetFloat("g_fPower", pSubSet->Material.MatD3D.Power);


    pEffect->SetTexture("g_DiffuseTexture", m_pRendererCom->Get_TargetManager()->GetTarget(L"Target_PostEffect")->Get_Texture());
    pEffect->SetFloat("g_fAccTime", m_fAccTime);
    return NOERROR;
}
