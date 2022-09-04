#include "stdafx.h"
#include "..\Headers\Static_Mesh.h"


CStatic_Mesh::CStatic_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CStatic_Mesh::CStatic_Mesh(const CStatic_Mesh & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_dwNumMaterials(rhs.m_dwNumMaterials)
	, m_pSubSetDesc(rhs.m_pSubSetDesc)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
{
	for (size_t i = 0; i < m_dwNumMaterials; i++)
	{
		if (nullptr != m_pSubSetDesc[i].MeshTexture.pDiffuseTexture)
			m_pSubSetDesc[i].MeshTexture.pDiffuseTexture->AddRef();
		if (nullptr != m_pSubSetDesc[i].MeshTexture.pNormalTexture)
			m_pSubSetDesc[i].MeshTexture.pNormalTexture->AddRef();
		if (nullptr != m_pSubSetDesc[i].MeshTexture.pSpecularTexture)
			m_pSubSetDesc[i].MeshTexture.pSpecularTexture->AddRef();
	}

	m_pMesh->AddRef();
}

HRESULT CStatic_Mesh::Ready_Mesh_Static(const _tchar * pFilePath, const _tchar * pFileName, void* pArg)
{
	LPD3DXMESH		pMesh = nullptr;

	_tchar			szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, 0, m_pGraphic_Device, &m_pAdjacency, &m_pMaterials, nullptr, &m_dwNumMaterials, &pMesh)))
		return E_FAIL;

	D3DXMatrixRotationY(&m_matPivot, D3DXToRadian(270.f));
	m_pSubSetDesc = new SUBSETDESC[m_dwNumMaterials];
	ZeroMemory(m_pSubSetDesc, sizeof(SUBSETDESC) * m_dwNumMaterials);

	for (size_t i = 0; i < m_dwNumMaterials; i++)
	{
		D3DXMATERIAL* pMaterial = (D3DXMATERIAL*)m_pMaterials->GetBufferPointer() + i;

		m_pSubSetDesc[i].Material = *pMaterial;

	
		ZeroMemory(szFullPath, sizeof(_tchar) * MAX_PATH);

		_tchar			szFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, pMaterial->pTextureFilename, strlen(pMaterial->pTextureFilename), szFileName, MAX_PATH);


		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);

		_tchar		szTmp[MAX_PATH] = L"";
		lstrcpy(szTmp, szFullPath);


		if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szTmp, &m_pSubSetDesc[i].MeshTexture.pDiffuseTexture)))
		{

		}

		lstrcpy(szTmp, szFullPath);
		if (SUCCEEDED(Change_TextureFileName(szTmp, L"D", L"N")))
		{
			D3DXCreateTextureFromFile(m_pGraphic_Device, szTmp, &m_pSubSetDesc[i].MeshTexture.pNormalTexture);
		}

		lstrcpy(szTmp, szFullPath);
		if (SUCCEEDED(Change_TextureFileName(szTmp, L"D", L"S")))
		{
			D3DXCreateTextureFromFile(m_pGraphic_Device, szTmp, &m_pSubSetDesc[i].MeshTexture.pSpecularTexture);
		}


	}

	_ulong		dwFVF = pMesh->GetFVF();

	if (false == (dwFVF & D3DFVF_NORMAL))
	{
		if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphic_Device, &m_pMesh)))
			return E_FAIL;

		if (dwFVF == 0)
			_int i = 0;
		else if (FAILED(D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer())))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphic_Device, &m_pMesh)))
			return E_FAIL;
	}

	Safe_Release(pMesh);

	D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	_ushort	wOffset = 0;

	m_pMesh->GetDeclaration(Element);

	
	for (size_t i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (D3DDECLUSAGE_POSITION == Element[i].Usage)
		{
			wOffset = Element[i].Offset;
			break;
		}
	}



	void*			pVertices = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertices);

	if (FAILED(D3DXComputeBoundingBox(((_vec3*)((_byte*)pVertices) + wOffset), m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pMesh->GetFVF()), &m_vMin, &m_vMax)))
		return E_FAIL;

	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

void CStatic_Mesh::Render_Mesh(_uint iAttributeID)
{
	if (nullptr == m_pMesh)
		return;
	m_pMesh->DrawSubset(iAttributeID);
}

CStatic_Mesh * CStatic_Mesh::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName, void* pArg)
{
	CStatic_Mesh*	pInstance = new CStatic_Mesh(pGraphic_Device);

	if (FAILED(pInstance->Ready_Mesh_Static(pFilePath, pFileName, pArg)))
	{
		MessageBox(0, L"CMesh_Static Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CStatic_Mesh::Clone_Component(void* pArg)
{
	return new CStatic_Mesh(*this);
}

void CStatic_Mesh::Free()
{
	for (size_t i = 0; i < m_dwNumMaterials; i++)
	{
		Safe_Release(m_pSubSetDesc[i].MeshTexture.pDiffuseTexture);
		Safe_Release(m_pSubSetDesc[i].MeshTexture.pNormalTexture);
		Safe_Release(m_pSubSetDesc[i].MeshTexture.pSpecularTexture);
	}

	if (false == m_isClone)
		Safe_Delete_Array(m_pSubSetDesc);

	Safe_Release(m_pMesh);

	CComponent::Free();
}

HRESULT CStatic_Mesh::Change_TextureFileName(_tchar * pFullPath, const _tchar * pSour, const _tchar * pDest)
{
	size_t	iLength = lstrlen(pFullPath);

	for (size_t i = iLength; i >= 0; --i)
	{

		if (pFullPath[i] == *pSour)
		{
			pFullPath[i] = *pDest;
			break;
		}

		if (i == 0)
			return E_FAIL;
	}

	return S_OK;
}

_matrix CStatic_Mesh::Get_LocalTransform() const
{
	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMax.z - m_vMin.z);

	D3DXMatrixTranslation(&matTrans, (m_vMax.x + m_vMin.x) * 0.5f, (m_vMax.y + m_vMin.y) * 0.5f, (m_vMax.z + m_vMin.z) * 0.5f);

	return matScale * matTrans;
}
