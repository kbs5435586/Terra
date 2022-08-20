#include "stdafx.h"
#include "..\Headers\Buffer_Trail.h"

CBuffer_Trail::CBuffer_Trail(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_Trail::CBuffer_Trail(const CBuffer_Trail& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CBuffer_Trail::Ready_VIBuffer(_uint iCnt, _uint iLerpCnt, _float fLifeTime)
{
	m_iNumVertices = iCnt;
	m_iLerpCnt = iLerpCnt;
	m_fLifeTime = fLifeTime;


	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;

	m_iNumPolygons = m_iNumVertices-2;
	m_iPolygonSize = sizeof(POLYGON16);
	m_eFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

void CBuffer_Trail::Add_NewTrail(const _vec3& matUpPosition, const _vec3& matDownPosition)
{
	TRAIL data(matUpPosition, matDownPosition);
	m_vecTrailData.push_back(data);
}

void CBuffer_Trail::Update_TrailBuffer(const _float& fTimeDelta, const _matrix& matWorld)
{
	auto iter = m_vecTrailData.begin();
	for (; iter != m_vecTrailData.end();)
	{
		iter->fTimeCount += fTimeDelta;
		if (iter->fTimeCount >= m_fLifeTime)
			iter = m_vecTrailData.erase(iter);
		else
			++iter;
	}

	if (m_vecTrailData.size() <= 1)
		return;

	VTXTEX* pVertex = nullptr;
	POLYGON16* pIndex = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertex), 0);
	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndex), 0);


	_uint iTrailSize = m_vecTrailData.size();

	_uint iIdx = 0;

	for (_uint i = 0; i < iTrailSize; ++i)
	{
		SplineTrailPosition(pVertex, i, iIdx, matWorld);
		if(m_iNumVertices <= iIdx)
			break;
	}

	m_fUVRate = 1.f / float(iIdx - 2);
	for (UINT i = 0; i < iIdx; i += 2)
	{
		pVertex[i].vTexUV = _vec2(0.f, 1.f - (m_fUVRate * i));
		pVertex[i + 1].vTexUV = _vec2(1.f, 1.f - (m_fUVRate * i));
	}

	m_iCurVtxCnt = iIdx;
	m_iCurTriCnt = m_iCurVtxCnt - 2;

	for (_uint i = 0; i < m_iCurTriCnt; i += 2)
	{
		pIndex[i]._0 = i;
		pIndex[i]._1 = i + 1;
		pIndex[i]._2 = i + 3;
		pIndex[i + 1]._0 = i;
		pIndex[i + 1]._1 = i + 3;
		pIndex[i + 1]._2 = i + 2;
	}
	m_pVB->Unlock();
	m_pIB->Unlock();
}



void CBuffer_Trail::SplineTrailPosition(VTXTEX* pVertexBuffer, const _uint& iDataIdx, _uint& iIdx, const _matrix& matWorld)
{
	D3DXMATRIX im;
	D3DXMatrixInverse(&im, 0, &matWorld);

	if (m_iNumVertices <= iIdx)
		return;

	_uint iCurIdx = iIdx;
	D3DXVec3TransformCoord(&pVertexBuffer[iIdx].vPosition, &m_vecTrailData[iDataIdx].vPosition[0], &im);
	++iIdx;


	if (m_iNumVertices <= iIdx)
		return;

	D3DXVec3TransformCoord(&pVertexBuffer[iIdx].vPosition, &m_vecTrailData[iDataIdx].vPosition[1], &im);
	++iIdx;

	if (m_iNumVertices <= iIdx)
		return;

	D3DXVECTOR3 vLerpPos[2];

	_uint	iSize = m_vecTrailData.size();

	for (_uint i = 1; i < m_iLerpCnt; ++i)
	{
		int iEditIndexV0 = (iDataIdx < 1 ? 0 : iDataIdx - 1);
		int iEditIndexV2 = (iDataIdx + 1 >= iSize ? iDataIdx : iDataIdx + 1);
		int iEditIndexV3 = (iDataIdx + 2 >= iSize ? iEditIndexV2 : iDataIdx + 2);

		D3DXVec3CatmullRom(&vLerpPos[0],
			&m_vecTrailData[iEditIndexV0].vPosition[0],
			&m_vecTrailData[iDataIdx].vPosition[0],
			&m_vecTrailData[iEditIndexV2].vPosition[0],
			&m_vecTrailData[iEditIndexV3].vPosition[0],
			i / float(m_iLerpCnt));

		D3DXVec3CatmullRom(&vLerpPos[1],
			&m_vecTrailData[iEditIndexV0].vPosition[1],
			&m_vecTrailData[iDataIdx].vPosition[1],
			&m_vecTrailData[iEditIndexV2].vPosition[1],
			&m_vecTrailData[iEditIndexV3].vPosition[1],
			i / float(m_iLerpCnt));

		D3DXVec3TransformCoord(&pVertexBuffer[iIdx].vPosition, &vLerpPos[0], &im);
		++iIdx;
		if (m_iNumVertices <= iIdx)
			return;
		D3DXVec3TransformCoord(&pVertexBuffer[iIdx].vPosition, &vLerpPos[1], &im);
		//vtx[index].color = D3DCOLOR_XRGB(255, 255, 0);
		++iIdx;
		if (m_iNumVertices <= iIdx)
			return;

	}
}
void CBuffer_Trail::Render_VIBuffer()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iCurVtxCnt, 0, m_iCurTriCnt);
}

CBuffer_Trail* CBuffer_Trail::Create(LPDIRECT3DDEVICE9 pGraphic_Device,
	_uint iCnt, _uint iLerpCnt, _float fLifeTime)
{
	CBuffer_Trail* pInstance = new CBuffer_Trail(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(iCnt, iLerpCnt, fLifeTime)))
	{
		MessageBox(0, L"CBuffer_Trail Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CBuffer_Trail::Clone_Component(void* pArg)
{
	return new CBuffer_Trail(*this);
}

void CBuffer_Trail::Free()
{
	CVIBuffer::Free();
}
