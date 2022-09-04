#include "stdafx.h"
#include "Range_Floor.h"
#include "Fire_Tall.h"
#include "Management.h"
#include "Player.h"
#include "Target_Manager.h"
#include "Target.h"
CRange_Floor::CRange_Floor(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CRange_Floor::CRange_Floor(const CRange_Floor& rhs)
    : CGameObject(rhs)
{
}

HRESULT CRange_Floor::Ready_Prototype()
{
    return S_OK;
}

HRESULT CRange_Floor::Ready_GameObject(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pParentTransform = (CTransform*)(pArg);
    m_pTransformCom->SetUp_Speed(100.f, D3DXToRadian(1360.f));
    m_pTransformCom->Set_Matrix(m_pParentTransform->Get_Matrix());
    m_pTransformCom->Scaling(2.f,2.f,2.f);
    
    _vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE_POSITION);
    vPos.y += 1.f;

    m_pTransformCom->Set_StateInfo(STATE_POSITION, &vPos);
    return S_OK;
}

_int CRange_Floor::Update_GameObject(const _float& fTimeDelta)
{
    if (m_fAccTime >= m_fMaxGageTime)
    {
        m_isFire = true;
        m_IsRender = false;
    }

    m_pPlayer = (CPlayer*)CManagement::GetInstance()->Get_BackObject(SCENE_STATIC, L"Layer_Player");
   
    m_pTransformCom->Set_Matrix(m_pParentTransform->Get_Matrix());
    m_pTransformCom->Scaling(2.f, 2.f, 2.f);
    _vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE_POSITION);
    vPos.y += 1.f;

    m_pTransformCom->Set_StateInfo(STATE_POSITION, &vPos);
    {
        /*if (m_fBazierCnt <= 1.f)
        {
            if (!m_IsBazier)
            {
                if (m_iFirBallIdx == 0)
                {
                    m_vStartPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION);
                    _vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_LOOK);
                    vLook *= -1.f;
                    m_vEndPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION) + (vLook * 150);
                    m_vMidPoint = (m_vStartPoint + m_vEndPoint) / 2;
                    m_vMidPoint.y += 10.f;
                    m_IsBazier = true;
                }
                else if (m_iFirBallIdx == 1)
                {
                    m_vStartPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION);
                    _vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_LOOK);
                    _vec3 vRight = *m_pTransformCom->Get_StateInfo(STATE_RIGHT);

                    m_vEndPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION) + (vLook * 150);
                    m_vMidPoint = (m_vStartPoint + m_vEndPoint) / 2 + (vRight * 50);;
                    m_vMidPoint.y += 10.f;
                    m_IsBazier = true;
                }
                else
                {
                    m_vStartPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION);
                    _vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_LOOK);
                    _vec3 vRight = *m_pTransformCom->Get_StateInfo(STATE_RIGHT);

                    m_vEndPoint = *m_pTransformCom->Get_StateInfo(STATE_POSITION) + (vLook * 150);
                    m_vMidPoint = (m_vStartPoint + m_vEndPoint) / 2 + (-vRight * 50);;
                    m_vMidPoint.y += 10.f;
                    m_IsBazier = true;
                }


            }
            Hit_Object(m_pTransformCom, m_fBazierCnt, m_vStartPoint, m_vEndPoint, m_vMidPoint, fTimeDelta, m_vSize);

        }
        if (m_fBazierCnt >= 5.f)
        {
            m_fBazierCnt = 0.f;
            m_IsBazier = false;
        }*/
    }


    return NO_EVENT;
}

_int CRange_Floor::LastUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsRender)
    {
        if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this)))
            return -1;
    }



    return _int();
}

void CRange_Floor::Render_GameObject()
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


CRange_Floor* CRange_Floor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CRange_Floor* pInstance = new CRange_Floor(pGraphic_Device);
    if (FAILED(pInstance->Ready_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CRange_Floor::Clone_GameObject(void* pArg)
{
    CRange_Floor* pInstance = new CRange_Floor(*this);
    if (FAILED(pInstance->Ready_GameObject(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CRange_Floor::Free()
{
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pMeshCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);


    CGameObject::Free();
}

HRESULT CRange_Floor::Ready_Component()
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

    m_pTextureCom = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Test");
    if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
        return E_FAIL;

    m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Range");
    if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
        return E_FAIL;

    m_pMeshCom = (CStatic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_EffectMesh_RangeFloor");
    if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
        return E_FAIL;



    Safe_Release(pManagement);
    return S_OK;
}

HRESULT CRange_Floor::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint& iAttributeID)
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

    pEffect->SetFloat("g_fAccTime", m_fAccTime);
    pEffect->SetFloat("g_fMaxTime", m_fMaxGageTime);


    m_pTextureCom->SetUp_OnShader(pEffect,"g_DiffuseTexture", 0);
    return S_OK;
}

