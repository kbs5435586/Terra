#include "stdafx.h"
#include "BoomWave.h"
#include "Management.h"
#include "Player.h"
#include "Target_Manager.h"
#include "Target.h"
CBoomWave::CBoomWave(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CBoomWave::CBoomWave(const CBoomWave& rhs)
    : CGameObject(rhs)
{
}

HRESULT CBoomWave::Ready_Prototype()
{
    return S_OK;
}

HRESULT CBoomWave::Ready_GameObject(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    FIRE_BALL tFireBall = *(FIRE_BALL*)pArg;
    _matrix matTemp = tFireBall.matWorld;
    m_iFirBallIdx = tFireBall.iIdx;


   // m_pTransformCom->Scaling(2.f, 2.f, 2.f);
    _matrix matWorld = m_pTransformCom->Get_Matrix();


    /* matTemp._42 += 2.f;
     matWorld *= matTemp;
     m_pTransformCom->Set_Matrix(matWorld);*/
    _vec3 vPos = *((CTransform*)CManagement::GetInstance()->Get_ComponentPointer(SCENE_STATIC,
        L"Layer_Player", L"Com_Transform"))->Get_StateInfo(STATE::STATE_POSITION);
    m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &vPos);
    m_pTransformCom->SetUp_Speed(100.f, D3DXToRadian(1360.f));


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

_int CBoomWave::Update_GameObject(const _float& fTimeDelta)
{
    m_pPlayer = (CPlayer*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Player");
    m_fLifeTime += fTimeDelta;




    if (m_fLifeTime >= 1.4f)
        return DEAD_OBJ;



    {
        if (m_fBazierCnt <= 1.f)
        {
            if (!m_IsBazier)
            {
                if (m_iFirBallIdx == 0)
                {
                    m_vStartPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION);
                    _vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_LOOK);
                    //vLook *= -1.f;
                    m_vEndPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION) + (vLook * 50);
                    m_vMidPoint = (m_vStartPoint + m_vEndPoint) / 2;
                    m_vMidPoint.y += 20.f;
                    m_IsBazier = true;
                }
                else if (m_iFirBallIdx == 1)
                {
                    m_vStartPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION);
                    _vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_LOOK);
                    _vec3 vRight = *m_pTransformCom->Get_StateInfo(STATE_RIGHT);

                    m_vEndPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION) + (vLook * 50);
                    m_vMidPoint = (m_vStartPoint + m_vEndPoint) / 2 + (vRight * 50);;
                    m_vMidPoint.y += 20.f;
                    m_IsBazier = true;
                }
                else
                {
                    m_vStartPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION);
                    _vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_LOOK);
                    _vec3 vRight = *m_pTransformCom->Get_StateInfo(STATE_RIGHT);

                    m_vEndPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION) + (vLook * 50);
                    m_vMidPoint = (m_vStartPoint + m_vEndPoint) / 2 + (-vRight * 50);;
                    m_vMidPoint.y += 20.f;
                    m_IsBazier = true;
                }


            }
            Hit_Object(m_pTransformCom, m_fBazierCnt, m_vStartPoint, m_vEndPoint, m_vMidPoint, fTimeDelta * 0.8f, m_vSize);

        }
        if (m_fBazierCnt >= 5.f)
        {
            m_fBazierCnt = 0.f;
            m_IsBazier = false;
        }
    }

    return NO_EVENT;
}

_int CBoomWave::LastUpdate_GameObject(const _float& fTimeDelta)
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

void CBoomWave::Render_GameObject()
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

void CBoomWave::Render_GameObject_PostEffect()
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

CBoomWave* CBoomWave::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBoomWave* pInstance = new CBoomWave(pGraphic_Device);
    if (FAILED(pInstance->Ready_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CBoomWave::Clone_GameObject(void* pArg)
{
    CBoomWave* pInstance = new CBoomWave(*this);
    if (FAILED(pInstance->Ready_GameObject(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CBoomWave::Free()
{
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pMeshCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom_PostEffect);

    for (int i = 0; i < 3; ++i)
    {
        Safe_Release(m_pTextureCom_Fire[i]);
    }

    CGameObject::Free();
}

HRESULT CBoomWave::Ready_Component()
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
    m_pShaderCom_PostEffect = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Effect");
    if (FAILED(Add_Component(L"Com_Shader_PostEffect", m_pShaderCom_PostEffect)))
        return E_FAIL;


    Safe_Release(pManagement);
    return S_OK;
}

HRESULT CBoomWave::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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

HRESULT CBoomWave::SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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
