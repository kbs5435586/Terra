#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Management.h"
#include "Mini_Shraken.h"
#include "Target_Manager.h"
#include "Target.h"
#include "ParticleSystem.h"
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Ready_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Ready_GameObject(void* pArg)
{
	if (nullptr != pArg)
	{
		m_eCurScene = *(SCENEID*)pArg;
	}
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);
	m_pTransformCom->SetUp_Speed(50.f, D3DXToRadian(180.f));
	m_pTransformCom->Set_StateInfo(STATE_POSITION, &_vec3(30.f, 0.f, 30.f));
//	m_pTransformCom->Set_StateInfo(STATE_POSITION, &_vec3(90.f, 0.f, 90.f));
	m_eNavi = NAVI::NAVI_PLAYER;
	m_eCurState = CPlayer::STATE_PL_WAIT;
	m_IsOnce = false;
	m_isAttack = false;
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{	
	_vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);

	Reset_Combo(fTimeDelta);
	End_Loop(fTimeDelta);
	Change_State(m_eCurState);
	
	m_pMeshCom->Play_Animation(fTimeDelta * m_fMove);
	Input_Key(fTimeDelta);
	m_fAccTime += fTimeDelta;


	if (CManagement::GetInstance()->KeyDown(KEY_G))
		test_Input ^= true;

	return _int();
}

_int CPlayer::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	Obb_Collision(m_pTransformCom, 5.f);

	m_fBlurTime += fTimeDelta;
	//if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
	//	return -1;
	if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_SHADOW, this)))
		return -1;
	//if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_BLUR, this)))
	//	return -1;
	//if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_POSTEFFECT, this)))
	//	return -1;


	if (!test_Input)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_NONEALPHA, this)))
			return -1;
	}
	else
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(RENDER_POSTEFFECT, this)))
			return -1;
	}

	return _int();
}

void CPlayer::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	_ulong dwNumMeshContainer = m_pMeshCom->Get_NumMeshContainer();

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMeshContainer; i++)
	{
		m_pMeshCom->Update_Skinning(i);
		const D3DXMESHCONTAINER_DERIVED* pMeshContainer = m_pMeshCom->Get_MeshContainer(i);

		for (size_t j = 0; j < pMeshContainer->NumMaterials; ++j)
		{
			if (FAILED(SetUp_ConstantTable(pEffect, pMeshContainer, j)))
				return;

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);
		}

	}

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);


	//if (nullptr != m_pColliderCom[0])
	//	m_pColliderCom[0]->Render_Collider();
	if (nullptr != m_pColliderCom[1])
		m_pColliderCom[1]->Render_Collider();
}

void CPlayer::Render_GameObject_Shadow()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom_Shadow)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom_Shadow->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	_ulong dwNumMeshContainer = m_pMeshCom->Get_NumMeshContainer();

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMeshContainer; i++)
	{
		m_pMeshCom->Update_Skinning(i);

		const D3DXMESHCONTAINER_DERIVED* pMeshContainer = m_pMeshCom->Get_MeshContainer(i);

		for (size_t j = 0; j < pMeshContainer->NumMaterials; ++j)
		{
			if (FAILED(SetUp_ConstantTable_Shadow(pEffect, pMeshContainer, j)))
				return;

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);
		}

	}

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);

}

void CPlayer::Render_GameObject_Blur()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom_Shadow)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom_Blur->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	_ulong dwNumMeshContainer = m_pMeshCom->Get_NumMeshContainer();

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMeshContainer; i++)
	{	
		m_pMeshCom->Update_Skinning(i);

		const D3DXMESHCONTAINER_DERIVED* pMeshContainer = m_pMeshCom->Get_MeshContainer(i);

		for (size_t j = 0; j < pMeshContainer->NumMaterials; ++j)
		{
			if (FAILED(SetUp_ConstantTable_Blur(pEffect, pMeshContainer, j)))
				return;

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);

	
		}


	}

	if (m_fBlurTime >= 0.1f)
	{
		_matrix		matView;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
		m_matOldView = matView;
		m_matOldWorld = m_pTransformCom->Get_Mul_Parent_Matrix();
		m_fBlurTime = 0.f;
	}

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);
}


void CPlayer::Render_GameObject_PostEffect()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom_Shadow)
		return;

	LPD3DXEFFECT		pEffect = m_pShaderCom_PostEffect->GetEffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	_ulong dwNumMeshContainer = m_pMeshCom->Get_NumMeshContainer();

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	for (size_t i = 0; i < dwNumMeshContainer; i++)
	{
		m_pMeshCom->Update_Skinning(i);

		const D3DXMESHCONTAINER_DERIVED* pMeshContainer = m_pMeshCom->Get_MeshContainer(i);

		for (size_t j = 0; j < pMeshContainer->NumMaterials; ++j)
		{
			if (FAILED(SetUp_ConstantTable_PostEffect(pEffect, pMeshContainer, j)))
				return;

			pEffect->CommitChanges();

			m_pMeshCom->Render_Mesh(i, j);
		}

	}

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);


}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		MessageBox(0, L"CPlayer Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer::Clone_GameObject(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MessageBox(0, L"CPlayer Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pShaderCom_Shadow);
	Safe_Release(m_pShaderCom_Blur);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom_PostEffect);
	Safe_Release(m_pNaviCom);
	Safe_Release(m_pTextureCom_Hatch);
	Safe_Release(m_pColliderCom[0]);
	Safe_Release(m_pColliderCom[1]);
	CGameObject::Free();
}

HRESULT CPlayer::Ready_Component()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For.Com_Transform
	m_pTransformCom = (CTransform*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	//Component_Shader_Shadow
	m_pShaderCom_Shadow = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Shadow");
	if (FAILED(Add_Component(L"Com_Shader_Shadow", m_pShaderCom_Shadow)))
		return E_FAIL;

	m_pShaderCom_Blur = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Blur");
	if (FAILED(Add_Component(L"Com_Shader_Blur", m_pShaderCom_Blur)))
		return E_FAIL;	
	//m_pShaderCom_PostEffect
	m_pShaderCom_PostEffect = (CShader*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader_PostEffect", m_pShaderCom_PostEffect)))
		return E_FAIL;
	// For.Com_Mesh
	m_pMeshCom = (CDynamic_Mesh*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Mesh_Player");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pTextureCom_Hatch = (CTexture*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Texture_Hatch");
	if (FAILED(Add_Component(L"Com_Texture_Hatch", m_pTextureCom_Hatch)))
		return E_FAIL;




	_uint		iCurrentCellIdx = 0;
	m_pNaviCom = (CNavi*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Navi", &iCurrentCellIdx);
	if (FAILED(Add_Component(L"Com_Navi", m_pNaviCom)))
		return E_FAIL;

	_matrix			matLocalTransform;
	D3DXMatrixScaling(&matLocalTransform, 15.f, 30.f, 15.f);
	matLocalTransform._42 = 15.f;
	m_pColliderCom[0] = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_AABB, matLocalTransform, nullptr, m_pTransformCom->Get_Matrix_Pointer()));
	if (FAILED(Add_Component(L"Com_Collider_AABB", m_pColliderCom[0])))
		return E_FAIL;

	m_pColliderCom[1] = (CCollider*)pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box",
		&COLLIDER_INFO(BOX_TYPE_OBB, matLocalTransform, nullptr, m_pTransformCom->Get_Matrix_Pointer()));
	if (FAILED(Add_Component(L"Com_Collider_OBB", m_pColliderCom[1])))
		return E_FAIL;

	Safe_Release(pManagement);
	return NOERROR;
}


HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer,
	const _uint& iAttributeID)
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

	const SUBSETDESC* pSubSet = &pMeshContainer->pSubSetDesc[iAttributeID];
	if (nullptr == pSubSet)
		return E_FAIL;

	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pSubSet->Material.MatD3D.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pSubSet->Material.MatD3D.Ambient);
	pEffect->SetFloat("g_fPower", pSubSet->Material.MatD3D.Power);

	pEffect->SetTexture("g_DiffuseTexture", pSubSet->MeshTexture.pDiffuseTexture);

	return NOERROR;
}

HRESULT CPlayer::SetUp_ConstantTable_Blur(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer,
	const _uint& iAttributeID)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetMatrix("g_matOldWorld", &m_matOldWorld);
	pEffect->SetMatrix("g_matOldView", &m_matOldView);


	return NOERROR;
}


HRESULT CPlayer::SetUp_ConstantTable_PostEffect(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID)
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

	const SUBSETDESC* pSubSet = &pMeshContainer->pSubSetDesc[iAttributeID];
	if (nullptr == pSubSet)
		return E_FAIL;

	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pSubSet->Material.MatD3D.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pSubSet->Material.MatD3D.Ambient);
	pEffect->SetFloat("g_fPower", pSubSet->Material.MatD3D.Power);

	
	pEffect->SetTexture("g_DiffuseTexture", m_pRendererCom->Get_TargetManager()->GetTarget(L"Target_PostEffect")->Get_Texture());
	pEffect->SetFloat("g_fAccTime", m_fAccTime);
	return NOERROR;
}

HRESULT CPlayer::SetUp_ConstantTable_Shadow(LPD3DXEFFECT pEffect, const D3DXMESHCONTAINER_DERIVED* pMeshContainer, const _uint& iAttributeID)
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

	pEffect->SetMatrix("g_matShadowView", &matLightView);
	pEffect->SetMatrix("g_matShadowProj", &matLightProj);


	return NOERROR;
}

void CPlayer::Change_State(STATE_ eID)
{
	if (m_ePreState != eID)
	{
		m_pMeshCom->Set_AnimationSet(eID);
		/*	switch (eID)
		{
		case CPlayer::STATE_PL_WAIT:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_WAIT);
			break;
		case CPlayer::STATE_JUMP_START:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_JUMP_START);
			break;
		case CPlayer::STATE_JUMP_END:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_JUMP_END);
			break;
		case CPlayer::STATE_JUMPLOOP:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_JUMPLOOP);
			break;
		case CPlayer::STATE_DASH_START:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_DASH_START);
			break;
		case CPlayer::STATE_DASH_END:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_DASH_END);
			break;
		case CPlayer::STATE_ASS_MOVE_LOOP:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_ASS_MOVE_LOOP);
			break;
		case CPlayer::STATE_ASS_MOVE_END:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_ASS_MOVE_END);
			break;
		case CPlayer::STATE_ASS_MOVE_STOP:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_ASS_MOVE_STOP);
			break;
		case CPlayer::STATE_PL_RUN:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_RUN);
			break;
		case CPlayer::STATE_ROT_SIRAKEN_START:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_ROT_SIRAKEN_START);
			break;
		case CPlayer::STATE_ROT_SIRAKEN_LOOP:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_ROT_SIRAKEN_LOOP);
			break;
		case CPlayer::STATE_ROT_SIRAKEN_END:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_ROT_SIRAKEN_END);
			break;
		case CPlayer::STATE_IN_WEAPON:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_IN_WEAPON);
			break;
		case CPlayer::STATE_OUT_WEAPON:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_OUT_WEAPON);
			break;
		case CPlayer::STATE_PL_COM1:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM1);
			break;
		case CPlayer::STATE_PL_COM1R:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM1R);
			break;

		case CPlayer::STATE_PL_COM2:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM2);
			break;
		case CPlayer::STATE_PL_COM2R:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM2R);
			break;

		case CPlayer::STATE_PL_COM3:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM3);
			break;
		case CPlayer::STATE_PL_COM3R:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM3R);
			break;

		case CPlayer::STATE_PL_COM4:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM4);
			break;
		case CPlayer::STATE_PL_COM5:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM5);
			break;
		case CPlayer::STATE_PL_COM6:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM6);
			break;
		case CPlayer::STATE_PL_COM7:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM7);
			break;

		case CPlayer::STATE_PL_COM4R:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM4R);
			break;
		case CPlayer::STATE_PL_COM5R:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM5R);
			break;
		case CPlayer::STATE_PL_COM6R:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_PL_COM6R);
			break;

		case CPlayer::STATE_FIRE_START:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_FIRE_START);
			break;
		case CPlayer::STATE_FIRE_READY:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_FIRE_READY);
			break;
		case CPlayer::STATE_FIRE_SHOT:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_FIRE_SHOT);
			break;
		case CPlayer::STATE_FIRE_END:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_FIRE_END);
			break;
		case CPlayer::STATE_ELEC_END:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_ELEC_END);
			break;
		case CPlayer::STATE_JUMP_ENDLAND:
			m_pMeshCom->Set_AnimationSet((_int)CPlayer::STATE_JUMP_ENDLAND);
			break;
		}*/
		if (m_eCurState == eID)
			m_ePreState = eID;

		m_fOnIdleTime = 0.f;
	}
}

void CPlayer::Input_Key(const _float& fTimeDelta)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;

	pManagement->AddRef();

	if (pManagement->KeyDown(KEY_LBUTTON))
	{
		m_IsOnce = true;
		m_isCombo = true;
		m_isAttack = true; 
		m_iCurComboCnt++;
		if (m_iPreComboCnt != m_iCurComboCnt)
		{
			m_fComboTime = 0.f;
			m_isMiniThrow = false;
			m_isThrow = false;
		}

		//Create_Particle();

	}
	else if (m_isCombo ||(m_eCurState <= STATE_PL_COM1 &&
						  m_eCurState >= STATE_PL_COM7 &&
						  m_pMeshCom->Get_EndLoop()))
	{
		m_eCurState = (STATE_)(STATE_PL_COM1 + (-2 * m_iCurComboCnt));
		m_IsOnce = true;
		m_isAttack = true;

		//if (m_eCurState == STATE_PL_COM7)
		{
			Create_Particle(fTimeDelta, 1.f);
		}

	}
	



	if (pManagement->KeyDown(KEY_RBUTTON))
	{
	
		Attack_Shiraken();
		m_isAttack = true;
	}
	if (pManagement->KeyDown(KEY_E))
	{
		m_isInputWeapon ? Output_Weapon() : Input_Weapon();
		m_isAttack = false;
	}
	if (pManagement->KeyUp(KEY_Q))
	{
		m_isAttack = true;
		if (m_isInputWeapon == false)
		{
			m_eCurState = STATE_CALL_ELEC;
			m_IsOnce = true;
		}
		Create_Particle(fTimeDelta, 1.f);
	}
	else if (pManagement->KeyPressing(KEY_Q))
	{
		m_fThrowPower += fTimeDelta * 10.f;
		if (m_fMaxThrowPower <= m_fThrowPower)
			m_fThrowPower = m_fMaxThrowPower;
		m_isAttack = false;
	
	}
	if (pManagement->KeyDown(KEY_SPACE))
	{
		Jump();
		m_isAttack = false;
	}


	if (pManagement->KeyUp(KEY_UP))
	{
		m_isAttack = false;
		m_IsOnce = false;
		m_eCurState = STATE_PL_WAIT;
	}
	else if (pManagement->KeyPressing(KEY_UP))
	{
		m_IsOnce = false;
		m_eCurState = STATE_PL_RUN;

		_vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_LOOK);
		_float fTemp = 0.f;
		_vec3 vSlide = {};
		if (m_pNaviCom->Move_OnNavigation(this->m_pTransformCom->Get_StateInfo(STATE_POSITION), 
			&(vLook * 50.f * fTimeDelta), fTemp, m_eNavi, &vSlide))
		{
			if (fTemp <= 2.f)
			{
				int i = 0; 
			}
			_vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
			vPos.y = fTemp;
		
			m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &vPos);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}
		else
		{
			//D3DXVec3Normalize(&vSlide, &vSlide);
			//vSlide.y = fTemp;
			m_pTransformCom->Go_There(-vSlide);
		}

		//_vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
		//vPos.y = fTemp;
		//
		//m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &vPos);
		//m_pTransformCom->Go_Straight(fTimeDelta);

		m_isAttack = false;
	}
	if (pManagement->KeyUp(KEY_DOWN))
	{
		m_isAttack = false;
		m_IsOnce = false;
		m_eCurState = STATE_PL_WAIT;
	}
	else if (pManagement->KeyPressing(KEY_DOWN))
	{
		m_IsOnce = false;
		m_eCurState = STATE_PL_RUN;

		_vec3 vLook = *m_pTransformCom->Get_StateInfo(STATE_LOOK);
		_float fTemp = 0.f;
		_vec3 vSlide = {};
		if (m_pNaviCom->Move_OnNavigation(this->m_pTransformCom->Get_StateInfo(STATE_POSITION),
			&(vLook * 50.f * fTimeDelta), fTemp, m_eNavi, &vSlide))
		{
			_vec3 vPos = *m_pTransformCom->Get_StateInfo(STATE::STATE_POSITION);
			vPos.y = fTemp;

			m_pTransformCom->Set_StateInfo(STATE::STATE_POSITION, &vPos);
			m_pTransformCom->BackWard(fTimeDelta);
		}
		else
		{
			//D3DXVec3Normalize(&vSlide, &vSlide);
			//vSlide.y = fTemp;
			m_pTransformCom->Go_There(-vSlide);
		}
		m_isAttack = false;
	}

	if (pManagement->KeyPressing(KEY_LEFT))
	{
		m_pTransformCom->Rotation_Y(-fTimeDelta);
		m_isAttack = false;
	}
	else if (pManagement->KeyPressing(KEY_RIGHT))
	{
		m_pTransformCom->Rotation_Y(fTimeDelta);
	}


	if (pManagement->KeyDown(KEY_F) /*&& m_isInputWeapon*/)
	{	
		Fire();
	}






	Safe_Release(pManagement);
}

void CPlayer::End_Loop(const _float& fTimeDelta)
{
	if (m_pMeshCom->Get_CurrentState() == CPlayer::STATE_OUT_WEAPON)
	{
		m_fOnIdleTime += fTimeDelta;
		m_fAnimTime = m_pMeshCom->Get_AllTime();
		if (m_fAnimTime - m_fOnIdleTime < 0.8f)
		{
			m_eCurState = CPlayer::STATE_PL_WAIT;
			m_IsOnce = false;
			m_fOnIdleTime = 0.f;
			m_isInputWeapon = false;
			return;
		}

	}
	if (m_pMeshCom->Get_CurrentState() == CPlayer::STATE_IN_WEAPON)
	{
		m_fOnIdleTime += fTimeDelta;
		m_fAnimTime = m_pMeshCom->Get_AllTime();

		if (m_fAnimTime - m_fOnIdleTime < 0.25f)
		{
			m_eCurState = CPlayer::STATE_PL_WAIT;
			m_IsOnce = false;
			m_fOnIdleTime = 0.f;
			m_isInputWeapon = true;
			return;
		}

	}
	if (m_pMeshCom->Get_CurrentState() == CPlayer::STATE_ROT_SIRAKEN_LOOP)
	{
		m_fOnIdleTime += fTimeDelta;
		m_fAnimTime = m_pMeshCom->Get_AllTime();

		if (m_fAnimTime - m_fOnIdleTime < 0.25f)
		{
			m_eCurState = CPlayer::STATE_PL_WAIT;
			m_IsOnce = false;
			m_fOnIdleTime = 0.f;
			return;
		}
		Create_Particle(fTimeDelta, 1.f);
	}
	if (m_pMeshCom->Get_CurrentState() == CPlayer::STATE_FIRE_END)
	{
		m_fOnIdleTime += fTimeDelta;
		m_fAnimTime = m_pMeshCom->Get_AllTime();

		if (m_fAnimTime - m_fOnIdleTime < 0.65f)
		{
			m_eCurState = CPlayer::STATE_PL_WAIT;
			m_IsOnce = false;
			m_fOnIdleTime = 0.f;
			return;
		}
	}
	if (m_pMeshCom->Get_CurrentState() == CPlayer::STATE_ELEC_END)
	{
		m_fOnIdleTime += fTimeDelta;
		m_fAnimTime = m_pMeshCom->Get_AllTime();

		if (m_fAnimTime - m_fOnIdleTime < 0.65f)
		{
			m_eCurState = CPlayer::STATE_PL_WAIT;
			m_IsOnce = false;
			m_fOnIdleTime = 0.f;
			return;
		}
	}
	if (m_pMeshCom->Get_CurrentState() == CPlayer::STATE_PL_COM7)
	{
		m_fOnIdleTime += fTimeDelta;
		m_fInputTime += fTimeDelta;
		m_fAnimTime = m_pMeshCom->Get_AllTime();

		if (m_fAnimTime - m_fOnIdleTime < 0.9f)
		{
			if (m_isInputWeapon == false)
			{
				m_isThrow = true;
				m_fThrowPower = 30.f;
				m_fOnIdleTime = 0.f;
				m_isMiniThrow = true;
			}
		}

	}
	if (m_pMeshCom->Get_CurrentState() == CPlayer::STATE_FIRE_START)
	{
		m_fOnIdleTime += fTimeDelta;
		m_fAnimTime = m_pMeshCom->Get_AllTime();
	
		if (m_fAnimTime - m_fOnIdleTime <  0.25f)
		{
			m_eCurState = CPlayer::STATE_FIRE_READY;
		}
	}

	if (m_IsOnce && m_pMeshCom->Get_EndLoop())
	{
		//Jump
		{
			if (m_eCurState == CPlayer::STATE_JUMP_END)
			{
				m_IsOnce = false;
				m_eCurState = CPlayer::STATE_PL_WAIT;
			}
			else if (m_eCurState == CPlayer::STATE_JUMP_START)
			{
				m_eCurState = CPlayer::STATE_JUMP_END;
			}
		}
		//Dash
		{
			if (m_eCurState == CPlayer::STATE_DASH_END)
			{
				m_IsOnce = false;
				m_eCurState = CPlayer::STATE_PL_RUN;
			}
			else if (m_eCurState == CPlayer::STATE_DASH_START)
				m_eCurState = CPlayer::STATE_DASH_END;

		}
		//Attack_Shiraken
		{
			if (m_eCurState == CPlayer::STATE_ROT_SIRAKEN_START)
			{
				m_eCurState = CPlayer::STATE_ROT_SIRAKEN_LOOP;
			}
		}

		//Output Weapon
		{

			if (m_pMeshCom->Get_CurrentState() == CPlayer::STATE_OUT_WEAPON)
			{
				m_fAnimTime = m_pMeshCom->Get_AllTime();

				m_eCurState = CPlayer::STATE_PL_WAIT;
				m_IsOnce = false;
			}
		}
		//Input Weapon
		{
			if (m_eCurState == CPlayer::STATE_IN_WEAPON)
			{
				m_IsOnce = false;
				m_eCurState = CPlayer::STATE_PL_WAIT;
				return;
			}
		}

		//Fire
		{
			if (m_eCurState == CPlayer::STATE_FIRE_SHOT)
			{
				m_eCurState = CPlayer::STATE_FIRE_END;
			}
			else if (m_eCurState == CPlayer::STATE_FIRE_READY)
			{
				m_eCurState = CPlayer::STATE_FIRE_SHOT;
				for (int i = 0; i < 3; ++i)
				{
					_matrix matTemp = m_pTransformCom->Get_Matrix();
					FIRE_BALL tFireBall(matTemp, i);
					if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Effect_Fire_Tall",
						SCENE_STATIC, L"Layer_Fire_Tall", &tFireBall)))
						return;

				}

			}
		}

		//Elec
		{

			if (m_eCurState == CPlayer::STATE_CALL_ELEC_LOOP)
			{

				m_eCurState = CPlayer::STATE_ELEC_END;
			}
			else if (m_eCurState == CPlayer::STATE_CALL_ELEC)
			{
				m_isThrow ^= true;
				m_eCurState = CPlayer::STATE_CALL_ELEC_LOOP;
			}
		}

		//Combo7
		{
			if (m_eCurState == CPlayer::STATE_PL_COM7)
			{
				m_isMiniThrow = false;

			}
		}
		m_fMove = 0.f;

	}
	else
	{
		m_fMove = 1.f;
	}



}

void CPlayer::Input_Weapon()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_IN_WEAPON;
}

void CPlayer::Output_Weapon()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_OUT_WEAPON;
}

void CPlayer::Attack_Shiraken()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_ROT_SIRAKEN_START;
}

void CPlayer::Attack_Fire()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_FIRE_START;
}

void CPlayer::Jump()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_JUMP_START;
}

void CPlayer::BackJump()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_BACKJUMP;
}

void CPlayer::AssMove()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_ASS_MOVE_LOOP;
}

void CPlayer::Dash()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_DASH_START;
}

void CPlayer::Fire()
{
	m_IsOnce = true;
	m_eCurState = CPlayer::STATE_FIRE_START;
}

void CPlayer::Reset_Combo(const _float& fTimeDelta)
{
	if (m_isCombo)
	{
		m_fComboTime += fTimeDelta;
	}
	if (m_fComboTime > m_fComboMaxTime)
	{
		m_isCombo = false;
		m_fComboTime = 0.f;
		m_iCurComboCnt = -1;
		m_iPreComboCnt = -1;
		m_IsOnce = false;
		m_eCurState = CPlayer::STATE_PL_WAIT;
	}

	if (m_iCurComboCnt >= 7)
	{
		m_iCurComboCnt = -1;
		m_iPreComboCnt = -1;
	}
}

