#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"
#include "Management.h"
#include "Back_Logo.h"
#include "Camera_Debug.h"
#include "Camera_Light.h"
#include "Scene_Stage_Town.h"
//Static_Mesh
#include "Cliff.h"
#include "Fence.h"
#include "Flower.h"
#include "Grass.h"
#include "Hill.h"
#include "House.h"
#include "Rock.h"
#include "Tree.h"
#include "Wall.h"
#include "SkHouse.h"
#include "Mini_Shraken.h"

//Dynamic_Mesh
#include "Player.h"
#include "Bee.h"
#include "Beatle.h"
#include "Mantis.h"
#include "Monkey.h"
#include "NPC.h"
#include "Tower.h"
#include "Vergos.h"

//UI
#include "HP.h"
#include "MP.h"
#include "Back_Hp.h"
#include "Back_Mp.h"
//Font
#include "DamageFont.h"

// Effect
#include "Fire_Tall.h"
#include "Meteor_Stone.h"
#include "BoomWave.h"
#include "Range_Floor.h"
#include "WaveRing.h"
#include "Arrow.h"
#include "PostEffectBuffer.h"

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Logo::Ready_Scene()
{
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;
	return S_OK;
}

_int CScene_Logo::Update_Scene(const _float& fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Logo::LastUpdate_Scene(const _float& fTimeDelta)
{
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CManagement* pManagement = CManagement::GetInstance();
		if (nullptr == pManagement)
			return -1;
		pManagement->AddRef();

		CScene_Stage_Town* pNewScene = CScene_Stage_Town::Create(m_pGraphic_Device);
		if (nullptr == pNewScene)
			return -1;

		if (FAILED(pManagement->SetUp_ScenePointer(pNewScene)))
			return -1;

		Safe_Release(pNewScene);
		Safe_Release(pManagement);

		return 0;
	}
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Logo::Render_Scene()
{

}

HRESULT CScene_Logo::Ready_Component_Funtion(CManagement* pManagement)
{
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Picking",
		CPicking::Create(m_pGraphic_Device, g_hWnd))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Navi",
		CNavi::Create(m_pGraphic_Device, L"../../Data/test_new_navi_1.dat"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Collider_Box",
		CCollider::Create(m_pGraphic_Device, COLLIDER_TYPE_BOX))))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Logo::Ready_Component_Shader(CManagement* pManagement)
{
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Default",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Default.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Terrain",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Mesh",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Shadow",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Shadow.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Sky",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Sky.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Particle",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Particle.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Blur",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Blur.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Trail",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Trail.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Effect",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Effect.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_FireTall",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_FireTall.fx"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Range",
		CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Range.fx"))))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Logo::Ready_Component_Mesh(CManagement* pManagement)
{
	{
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Mesh_Player",
			CDynamic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/DynamicMesh/PoPori/", L"PoPori.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Shiraken1",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Shiraken/Shiraken1/", L"Shiraken1.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Shiraken2",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Shiraken/Shiraken2/", L"Shiraken2.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Shiraken3",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Shiraken/Shiraken3/", L"Shiraken3.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Shiraken4",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Shiraken/Shiraken4/", L"Shiraken4.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Sword",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Sword/", L"Sword.X"))))
			return E_FAIL;
	}


	////Cliff
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Cliff0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Cliff/Cliff0/", L"Cliff0.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Cliff1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Cliff/Cliff1/", L"Cliff1.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Cliff2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Cliff/Cliff2/", L"Cliff2.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Cliff3",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Cliff/Cliff3/", L"Cliff3.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Cliff4",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Cliff/Cliff4/", L"Cliff4.X"))))
	//		return E_FAIL;
	//}
	////Fence
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Fence0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Fence/Fence0/", L"Fence0.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Fence1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Fence/Fence1/", L"Fence1.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Fence2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Fence/Fence2/", L"Fence2.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Fence3",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Fence/Fence3/", L"Fence3.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Fence4",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Fence/Fence4/", L"Fence4.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Fence5",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Fence/Fence5/", L"Fence5.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Fence6",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Fence/Fence6/", L"Fence6.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Fence7",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Fence/Fence7/", L"Fence7.X"))))
	//		return E_FAIL;
	//}
	////Flower
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower0/", L"Flower0.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower1/", L"Flower1.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower2/", L"Flower2.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower3",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower3/", L"Flower3.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower4",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower4/", L"Flower4.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower5",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower5/", L"Flower5.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower6",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower6/", L"Flower6.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower7",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower7/", L"Flower7.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower8",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower8/", L"Flower8.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower9",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower9/", L"Flower9.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Flower10",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Flower/Flower10/", L"Flower10.X"))))
	//		return E_FAIL;
	//}
	////Grass
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass0/", L"Grass0.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass1/", L"Grass1.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass2/", L"Grass2.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass3",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass3/", L"Grass3.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass4",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass4/", L"Grass4.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass5",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass5/", L"Grass5.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass6",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass6/", L"Grass6.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass7",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass7/", L"Grass7.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass8",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass8/", L"Grass8.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass9",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass9/", L"Grass9.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass10",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass10/", L"Grass10.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass11",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass11/", L"Grass11.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass12",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass12/", L"Grass12.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass13",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass13/", L"Grass13.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass14",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass14/", L"Grass14.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass15",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass15/", L"Grass15.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass16",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass16/", L"Grass16.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass17",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass17/", L"Grass17.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass18",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass18/", L"Grass18.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Grass19",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Grass/Grass19/", L"Grass19.X"))))
	//		return E_FAIL;
	//}
	////Hill
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Hill0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Hill/Hill0/", L"Hill0.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Hill1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Hill/Hill1/", L"Hill1.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Hill2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Hill/Hill2/", L"Hill2.X"))))
	//		return E_FAIL;
	//}
	////House
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_House0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/House/House0/", L"House0.X"))))
	//		return E_FAIL;
	//}
	////Rock
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Rock0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Rock/Rock0/", L"Rock0.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Rock1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Rock/Rock1/", L"Rock1.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Rock2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Rock/Rock2/", L"Rock2.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Rock3",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Rock/Rock3/", L"Rock3.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Rock4",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Rock/Rock4/", L"Rock4.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Rock5",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Rock/Rock5/", L"Rock5.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Rock6",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Rock/Rock6/", L"Rock6.X"))))
	//		return E_FAIL;
	//}
	////Tree
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree0/", L"Tree0.X", (void*)TRUE))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree1/", L"Tree1.X", (void*)TRUE))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree2/", L"Tree2.X", (void*)TRUE))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree3",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree3/", L"Tree3.X", (void*)TRUE))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree4",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree4/", L"Tree4.X", (void*)TRUE))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree5",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree5/", L"Tree5.X", (void*)TRUE))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree6",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree6/", L"Tree6.X", (void*)TRUE))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree7",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree7/", L"Tree7.X", (void*)TRUE))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Tree8",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Tree/Tree8/", L"Tree8.X", (void*)TRUE))))
	//		return E_FAIL;
	//}
	////Wall
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall0/", L"Wall0.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall1/", L"Wall1.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall2/", L"Wall2.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall3",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall3/", L"Wall3.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall4",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall4/", L"Wall4.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall5",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall5/", L"Wall5.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall6",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall6/", L"Wall6.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall7",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall7/", L"Wall7.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall8",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall8/", L"Wall8.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall9",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall9/", L"Wall9.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_Wall10",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Wall/Wall10/", L"Wall10.X"))))
	//		return E_FAIL;
	//}
	////SkHouse
	//{
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_SkHouse0",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/SkHouse/SkHouse0/", L"SkHouse0.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_SkHouse1",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/SkHouse/SkHouse1/", L"SkHouse1.X"))))
	//		return E_FAIL;
	//	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_StaticMesh_SkHouse2",
	//		CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/SkHouse/SkHouse2/", L"SkHouse2.X"))))
	//		return E_FAIL;
	//}
	// 
	//UI


	{
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_EffectMesh_FireTall",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Effect/EffectMesh/", L"Mesh_FireTail.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_EffectMesh_Meteor",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Effect/EffectMesh/", L"Meteor_Stone.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_EffectMesh_BoomWave",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Effect/EffectMesh/", L"BoomWave1.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_EffectMesh_RangeFloor",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Effect/EffectMesh/", L"45big.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_EffectMesh_WaveRing",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Effect/EffectMesh/", L"Plat_WaveRing.X"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_EffectMesh_Arrow",
			CStatic_Mesh::Create(m_pGraphic_Device, L"../../Resource/Mesh/StaticMesh/Arrow/", L"Arrow.X"))))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CScene_Logo::Ready_Component_Buffer(CManagement* pManagement)
{
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Buffer_CubeTex",
		CBuffer_CubeTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Buffer_RectTex",
		CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Buffer_Terrain_TF",
		CBuffer_Terrain::Create(m_pGraphic_Device, (_uint)300.f, (_uint)400.f, 1.f)/**/)))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Buffer_Trail",
		CBuffer_Trail::Create(m_pGraphic_Device, 10000, 2, 0.01f))))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Logo::Ready_Component_Texture(CManagement* pManagement)
{
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Terrain",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../Bin/Texture/Terrain/Grass_%d.tga", 2))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_SkyBox",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_CUBE, L"../Bin/Texture/SkyBox/burger%d.dds", 4))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_LOGO, L"Component_Texture_Logo",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../Bin/Texture/Back_Logo/Back_Logo.png"))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Number",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/UI/Number/%d.png", 9))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Red_Trail",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Effect/Trail/RedTrail%d.png", 1))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Particle_Flame",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Effect/Texture/Flame_Particle/%d.png", 61))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Hatch",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../Bin/Texture/Hatch/Hatch%d.dds", 6))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Mask",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/Mask/MMask_0%d.tga", 9))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Trail",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/Effect/TRAIL_0%d.tga", 8))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_HP",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/UI/Bar/Hp/Hp%d.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_HP_Back",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/UI/Bar/Hp_Back/Hp_Back%d.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_MP",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/UI/Bar/Mp/Mp%d.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_MP_Back",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/UI/Bar/Mp_Back/Mp_Back%d.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Monster_HP",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/UI/Bar/Monster_Hp/Monster_Hp%d.tga", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Monster_HP_Back",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/UI/Bar/Monster_Hp_Back/Monster_Hp_Back%d.tga", 1))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_FireAlpha",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Effect/Fire/Fire_alpha%d.dds", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Firefire",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Effect/Fire/Fire_fire%d.dds", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Firenoise",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Effect/Fire/Fire_noise%d.dds", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Hit",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Effect/Texture/Hit/%d.png", 16))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_FireSpark",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Effect/Texture/Hit/%d.png", 16))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Explosion",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Effect/Texture/Explosion/Explosion%d.png", 90))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Test",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Texture/Test/Test%d.png", 1))))
		return E_FAIL;
 	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Texture_Arrow",
		CTexture::Create(m_pGraphic_Device, TEXTURE_TYPE_GENERAL, L"../../Resource/Mesh/StaticMesh/Arrow/UD_Standard_Units%d.png", 1))))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Logo::Ready_Prototype_GameObject()
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();

	// For.GameObject_Back_Logo
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Back_Logo", CBack_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;
	//Static_Mesh
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Cliff", CCliff::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Fence", CFence::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Flower", CFlower::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Grass", CGrass::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_House", CHill::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Rock", CRock::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Tree", CTree::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Wall", CWall::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_SkHouse", CSkHouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Popori", CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Beatle", CBeatle::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Bee", CBee::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Mantis", CMantis::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Monkey", CMonkey::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_NPC", CNPC::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Tower", CTower::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Vergos", CVergos::Create(m_pGraphic_Device))))
		return E_FAIL;


	//UI
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_UI_HP", CHP::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_UI_MP", CMP::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_UI_HP_Back", CBack_Hp::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_UI_MP_Back", CBack_Mp::Create(m_pGraphic_Device))))
		return E_FAIL;

	// Font
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Font_Number", CDamageFont::Create(m_pGraphic_Device))))
		return E_FAIL;

	// Effect
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Effect_Fire_Tall", CFire_Tall::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Effect_Meteor_Stone", CMeteor_Stone::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Effect_BoomWave", CBoomWave::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Effect_RangeFloor", CRange_Floor::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Effect_WaveRing", CWaveRing::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Effect_Arrow", CArrow::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_PostEffectBuffer", CPostEffectBuffer::Create(m_pGraphic_Device))))
		return E_FAIL;
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Logo::Ready_Prototype_Component()
{

	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();

	if (FAILED(Ready_Component_Funtion(pManagement)))
		return E_FAIL;
	if (FAILED(Ready_Component_Shader(pManagement)))
		return E_FAIL;
	if (FAILED(Ready_Component_Mesh(pManagement)))
		return E_FAIL;
	if (FAILED(Ready_Component_Buffer(pManagement)))
		return E_FAIL;
	if (FAILED(Ready_Component_Texture(pManagement)))
		return E_FAIL;
	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Back_Logo", SCENE_LOGO, pLayerTag)))
		return E_FAIL;
	Safe_Release(pManagement);

	return S_OK;
}



HRESULT CScene_Logo::Load_Terrain_Buffer_Component(const _tchar* pFilePath)
{

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	_float		fSizeX = 0.f;
	_float		fSizeZ = 0.f;
	_float		fInterval = 0.f;
	_int		iLength = 0;
	TCHAR szName[MAX_STR] = L"";

	while (1)
	{
		ReadFile(hFile, &iLength, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szName, sizeof(TCHAR) * iLength, &dwByte, nullptr);
		ReadFile(hFile, &fSizeX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &fSizeZ, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &fInterval, sizeof(_float), &dwByte, nullptr);

		if (0 == dwByte)
			break;


		_int	iLength_ = lstrlen(szName);
		_tchar* pComponent_Tag = new _tchar[iLength_ + 1];
		if (nullptr == pComponent_Tag)
			return E_FAIL;
		lstrcpy(pComponent_Tag, szName);


		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, pComponent_Tag, CBuffer_Terrain::Create(m_pGraphic_Device, (_uint)fSizeX, (_uint)fSizeZ, fInterval)/**/)))
		{
			CloseHandle(hFile);
			return E_FAIL;
		}
	}
	Safe_Release(pManagement);
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CScene_Logo::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	pManagement->AddRef();

	// For.Camera_Debug
	CCamera_Debug* pCameraObject = nullptr;

	if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Camera_Debug", SCENE_LOGO, pLayerTag, (CGameObject**)&pCameraObject)))
		return E_FAIL;

	CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CAMERADESC));
	CameraDesc.vEye = _vec3(0.f, 20.f, -10.f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _vec3(0.f, 1.f, 0.f);

	PROJDESC		ProjDesc;
	ZeroMemory(&ProjDesc, sizeof(PROJDESC));
	ProjDesc.fFovY = D3DXToRadian(60.0f);
	ProjDesc.fAspect = _float(g_iBackCX) / g_iBackCY;
	ProjDesc.fNear = 0.2f;
	ProjDesc.fFar = 500.0f;

	if (FAILED(pCameraObject->SetUp_CameraProjDesc(CameraDesc, ProjDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return NOERROR;
}

CScene_Logo* CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Logo* pInstance = new CScene_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(0, L"CScene_Logo Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Logo::Free()
{
	CManagement* pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return;

	pManagement->AddRef();

	pManagement->Clear_Layers(SCENE_LOGO);

	Safe_Release(pManagement);

	CScene::Free();
}
