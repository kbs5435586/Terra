#include "stdafx.h"
#include "Buffer_Terrain.h"
#include "Transform.h"

CBuffer_Terrain::CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_Terrain::CBuffer_Terrain(const CBuffer_Terrain& rhs)
	: CVIBuffer(rhs)
	, m_pPixel(rhs.m_pPixel)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_fInterval(rhs.m_fInterval)
	, m_vecVertex(rhs.m_vecVertex)
{
}

HRESULT CBuffer_Terrain::Ready_VIBuffer(const _uint& iNumVerticesX, const _uint& iNumVerticesZ, const _float& fInterval)
{
	vector<_vec3> vecV;

	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_fInterval = fInterval;

	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iStride = sizeof(VTXNORTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	m_iNumPolygons = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	m_iPolygonSize = sizeof(POLYGON32);
	m_eFormat = D3DFMT_INDEX32;


	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;


	VTXNORTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, 0.0f, i * m_fInterval);
			vecV.push_back(pVertices[iIndex].vPosition);
			pVertices[iIndex].vTexUV = _vec2(j / (iNumVerticesX - 1.f), i / (iNumVerticesZ - 1.f));
			pVertices[iIndex].vNormal = _vec3(0.f, 1.f, 0.f);
		}
	}


	POLYGON32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint			iPolygonIndex = 0;

	for (size_t i = 0; i < iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < iNumVerticesX - 1; j++)
		{
			size_t iIndex = i * iNumVerticesX + j;

			// 우.상
			pIndices[iPolygonIndex]._0 = iIndex + iNumVerticesX;
			pIndices[iPolygonIndex]._1 = iIndex + iNumVerticesX + 1;
			pIndices[iPolygonIndex]._2 = iIndex + 1;

			_vec3		vSour, vDest;
			_vec3		vNormal;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPosition - pVertices[pIndices[iPolygonIndex]._0].vPosition;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPosition - pVertices[pIndices[iPolygonIndex]._1].vPosition;

			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSour, &vDest));

			pVertices[pIndices[iPolygonIndex]._0].vNormal += vNormal;
			pVertices[pIndices[iPolygonIndex]._1].vNormal += vNormal;
			pVertices[pIndices[iPolygonIndex]._2].vNormal += vNormal;
			++iPolygonIndex;
			m_vecVertex.push_back(vecV[iIndex + iNumVerticesX]);
			m_vecVertex.push_back(vecV[iIndex + iNumVerticesX + 1]);
			m_vecVertex.push_back(vecV[iIndex + 1]);
			// 좌.하
			pIndices[iPolygonIndex]._0 = iIndex + iNumVerticesX;
			pIndices[iPolygonIndex]._1 = iIndex + 1;
			pIndices[iPolygonIndex]._2 = iIndex;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPosition - pVertices[pIndices[iPolygonIndex]._0].vPosition;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPosition - pVertices[pIndices[iPolygonIndex]._1].vPosition;

			m_vecVertex.push_back(vecV[iIndex + iNumVerticesX]);
			m_vecVertex.push_back(vecV[iIndex + 1]);
			m_vecVertex.push_back(vecV[iIndex]);

			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSour, &vDest));

			pVertices[pIndices[iPolygonIndex]._0].vNormal += vNormal;
			pVertices[pIndices[iPolygonIndex]._1].vNormal += vNormal;
			pVertices[pIndices[iPolygonIndex]._2].vNormal += vNormal;
			++iPolygonIndex;
		}
	}

	for (size_t i = 0; i < m_iNumVertices; ++i)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();


	return S_OK;
}
HRESULT CBuffer_Terrain::Ready_VIBuffer(const _tchar* pHeightMap, const _float& fInterval)
{
	vector<_vec3> vecV;

	_uint		iNumVerticesX, iNumVerticesZ;

	HANDLE		hFile = 0;
	_ulong		dwByte = 0;

	hFile = CreateFile(pHeightMap, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	// 파일정보
	ReadFile(hFile, &m_fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	// 이미지정보
	ReadFile(hFile, &m_ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	iNumVerticesX = m_ih.biWidth;
	iNumVerticesZ = m_ih.biHeight;

	m_pPixel = new _ulong[iNumVerticesX * iNumVerticesZ];

	// 픽셀정보
	ReadFile(hFile, m_pPixel, sizeof(_ulong) * (iNumVerticesX * iNumVerticesZ), &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_fInterval = fInterval;

	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iStride = sizeof(VTXNORTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPolygons = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	m_iPolygonSize = sizeof(POLYGON32);
	m_eFormat = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXNORTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, (m_pPixel[iIndex] & 0x000000ff) / 10.0f, i * m_fInterval);
			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _vec2(j / (iNumVerticesX - 1.f), i / (iNumVerticesZ - 1.f));
			vecV.push_back(pVertices[iIndex].vPosition);
		}
	}


	POLYGON32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint			iPolygonIndex = 0;


	for (size_t i = 0; i < iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < iNumVerticesX - 1; j++)
		{
			size_t iIndex = i * iNumVerticesX + j;

			// 우.상
			pIndices[iPolygonIndex]._0 = iIndex + iNumVerticesX;
			pIndices[iPolygonIndex]._1 = iIndex + iNumVerticesX + 1;
			pIndices[iPolygonIndex]._2 = iIndex + 1;

			_vec3		vSour, vDest;
			_vec3		vNormal;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPosition - pVertices[pIndices[iPolygonIndex]._0].vPosition;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPosition - pVertices[pIndices[iPolygonIndex]._1].vPosition;

			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSour, &vDest));

			m_vecVertex.push_back(vecV[iIndex + iNumVerticesX]);
			m_vecVertex.push_back(vecV[iIndex + iNumVerticesX + 1]);
			m_vecVertex.push_back(vecV[iIndex + 1]);

			pVertices[pIndices[iPolygonIndex]._0].vNormal += vNormal;
			pVertices[pIndices[iPolygonIndex]._1].vNormal += vNormal;
			pVertices[pIndices[iPolygonIndex]._2].vNormal += vNormal;
			++iPolygonIndex;

			// 좌.하
			pIndices[iPolygonIndex]._0 = iIndex + iNumVerticesX;
			pIndices[iPolygonIndex]._1 = iIndex + 1;
			pIndices[iPolygonIndex]._2 = iIndex;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPosition - pVertices[pIndices[iPolygonIndex]._0].vPosition;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPosition - pVertices[pIndices[iPolygonIndex]._1].vPosition;

			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSour, &vDest));

			m_vecVertex.push_back(vecV[iIndex + iNumVerticesX]);
			m_vecVertex.push_back(vecV[iIndex + 1]);
			m_vecVertex.push_back(vecV[iIndex]);

			pVertices[pIndices[iPolygonIndex]._0].vNormal += vNormal;
			pVertices[pIndices[iPolygonIndex]._1].vNormal += vNormal;
			pVertices[pIndices[iPolygonIndex]._2].vNormal += vNormal;
			++iPolygonIndex;
		}
	}

	for (size_t i = 0; i < m_iNumVertices; ++i)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVB->Unlock();

	m_pIB->Unlock();

	return NOERROR;
}
void CBuffer_Terrain::Render_VIBuffer()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPolygons);
}



CBuffer_Terrain* CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint& iNumVerticesX, const _uint& iNumVerticesZ, const _float& fInterval)
{
	CBuffer_Terrain* pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(iNumVerticesX, iNumVerticesZ, fInterval)))
	{
		MessageBox(0, L"CBuffer_Terrain Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBuffer_Terrain* CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _float& fInterval)
{
	CBuffer_Terrain* pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(pFilePath, fInterval)))
	{
		MessageBox(0, L"CBuffer_Terrain Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CBuffer_Terrain::Clone_Component(void* pArg)
{
	return new CBuffer_Terrain(*this);
}

void CBuffer_Terrain::Free()
{
	if (false == m_isClone)
		Safe_Delete_Array(m_pPixel);
	CVIBuffer::Free();
}

_bool CBuffer_Terrain::Picking_ToBuffer(_vec3* pOut, const CTransform* pTransformCOm, const CPicking* pPickingCom)
{
	_vec3	vRayPivot = pPickingCom->Get_MouseRayPivot();
	_vec3	vRay = pPickingCom->Get_MouseRay();

	_matrix		matWorldInv = pTransformCOm->Get_Matrix_Inverse();


	D3DXVec3TransformCoord(&vRayPivot, &vRayPivot, &matWorldInv);
	D3DXVec3TransformNormal(&vRay, &vRay, &matWorldInv);

	_float fU, fV, fDist;
	_bool	isPick = false;


	if (D3DXIntersectTri(&m_pPosition[1], &m_pPosition[0], &m_pPosition[2], &vRayPivot, &vRay, &fU, &fV, &fDist))
		isPick = true;

	if (D3DXIntersectTri(&m_pPosition[3], &m_pPosition[2], &m_pPosition[0], &vRayPivot, &vRay, &fU, &fV, &fDist))
		isPick = true;

	if (pOut != nullptr)
	{
		*pOut = vRayPivot + vRay * fDist;
	}
	return isPick;
}


_float CBuffer_Terrain::Compute_HeightOnTerrain(CTransform* pTransform)
{
	_vec3* pPosition = pTransform->Get_StateInfo(STATE::STATE_POSITION);
	_ulong	dwIndex = _ulong(pPosition->z / m_fInterval) * m_iNumVerticesX + _ulong(pPosition->x / m_fInterval);

	_float	fRatioX = (pPosition->x - m_vecVertex[dwIndex + m_iNumVerticesX].x) / m_fInterval;
	_float	fRatioZ = (m_vecVertex[dwIndex + m_iNumVerticesX].z - pPosition->z) / m_fInterval;

	D3DXPLANE			Plane;

	if (fRatioX > fRatioZ)
		D3DXPlaneFromPoints(&Plane, &m_vecVertex[dwIndex + m_iNumVerticesX], &m_vecVertex[dwIndex + m_iNumVerticesX + 1], &m_vecVertex[dwIndex + 1]);
	else
		D3DXPlaneFromPoints(&Plane, &m_vecVertex[dwIndex + m_iNumVerticesX], &m_vecVertex[dwIndex + 1], &m_vecVertex[dwIndex]);


	return ((Plane.a * pPosition->x * -1.f) + (Plane.c * pPosition->z * -1.f) + Plane.d * -1.f) / Plane.b;

	return _float();
}
