#include "stdafx.h"
#include "Navi.h"
#include "Management.h"
#include "Cell.h"
#include "Line.h"

CNavi::CNavi(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CNavi::CNavi(const CNavi & rhs)
	: CComponent(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_mapTotlaPos(rhs.m_mapTotlaPos)
	, m_vPos(rhs.m_vPos)
	, m_eNavi(rhs.m_eNavi)
{
	for (auto& pCell : m_vecCell)
		pCell->AddRef();
}

HRESULT CNavi::Ready_Navigation(const _tchar* pFileName)
{
	HANDLE			hFile = 0;
	_ulong			dwByte = 0;

	hFile = CreateFile(pFileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	m_mapTotlaPos.clear();
	//m_vTotlaPos.shrink_to_fit();


	_uint		iLen = 0;
	NAVI		eLoadNavi = NAVI::NAVI_END;
	_uint		iCnt = 0;
	_vec3		vPoint[3] = {};
	while (TRUE)
	{

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &eLoadNavi, sizeof(NAVI), &dwByte, nullptr);
		m_eNavi = eLoadNavi;
		if (dwByte == 0)
		{
			break;
		}
		_uint m_iNaviMeshCnt = 0;

		for (_uint i = 0; i < iLen; ++i)
		{
			for (_uint j = 0; j < 3; ++j)
			{
				ReadFile(hFile, &vPoint[j], sizeof(_vec3), &dwByte, nullptr);
				_vec3 vPos = vPoint[j];

				if (m_iNaviMeshCnt < 3)
				{
					int iPointIdx = 0;
					bool result = false;
					if (m_mapTotlaPos.empty())
						result = true;
					for (auto& iter : m_mapTotlaPos)
					{
						_vec3 vTemp = {};
						_vec3	vIter_Pos = iter.second;
						vIter_Pos.y = 0.f;

						_vec3	vTemp_Pos = vPos;
						vTemp_Pos.y = 0.f;

						vTemp = vIter_Pos - vTemp_Pos;
						_float fLength = D3DXVec3Length(&vTemp);
						if (fLength < 3.5f)
						{
							vPos = iter.second;
							iPointIdx = iter.first;
							result = false;
							break;
						}
						else
						{
							result = true;
						}
					}

					if (result == false)
					{
						if (FAILED(Set_PositionVector(iPointIdx, vPos)))
							return E_FAIL;
						m_iNaviMeshCnt++;
					}
					else
					{
						if (FAILED(Set_PositionVector(vPos)))
							return E_FAIL;
						m_iNaviMeshCnt++;
					}


				}



				if (m_iNaviMeshCnt > 2)
				{
					if (Set_CellVector(eLoadNavi))
						return E_FAIL;


					Clear_Position_Vector();
					m_iNaviMeshCnt = 0;
				}
			}
		}




	}


	if (FAILED(Ready_Neighbor()))
		return E_FAIL;
	CloseHandle(hFile);


	return S_OK;
}

HRESULT CNavi::Set_PositionVector(_uint iIdx, _vec3 vPos)
{
	m_vPos.push_back(vPos);
	return S_OK;
}
HRESULT CNavi::Set_CellVector(NAVI navi)
{
	for (auto& iter : m_vecCell)
		iter->GetIsLast() = false;

	CCell* pCell = CCell::Create(m_pGraphic_Device, &m_vPos[0], &m_vPos[1], &m_vPos[2], m_vecCell.size());
	if (nullptr == pCell)
		return E_FAIL;
	m_eNavi = navi;
	pCell->GetNavi() = navi;
	pCell->Compute_Color();

	pCell->GetIsLast() = true;
	m_vecCell.push_back(pCell);
	return S_OK;
}


HRESULT CNavi::Ready_Neighbor()
{
 	_uint	iNumCells = m_vecCell.size();

	for (size_t i = 0; i < iNumCells; ++i)
	{
		for (size_t j = 0; j < iNumCells; j++)
		{
			if (i == j)
				continue;

			if (true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(POINT_A), m_vecCell[i]->Get_Point(POINT_B)))
				m_vecCell[i]->Set_Neighbor(NEIGHBOR_AB, m_vecCell[j]);

			if (true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(POINT_B), m_vecCell[i]->Get_Point(POINT_C)))
				m_vecCell[i]->Set_Neighbor(NEIGHBOR_BC, m_vecCell[j]);

			if (true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(POINT_C), m_vecCell[i]->Get_Point(POINT_A)))
				m_vecCell[i]->Set_Neighbor(NEIGHBOR_CA, m_vecCell[j]);
		}
	}

	return S_OK;
}

HRESULT CNavi::Ready_Clone_Navigation(void * pArg)
{
	if (nullptr == pArg)
		return S_OK;

	m_iCurrentIdx = *(_uint*)pArg;

	if (m_vecCell.size() <= m_iCurrentIdx)
		return E_FAIL;

	return S_OK;
}

_bool CNavi::Move_OnNavigation(const _vec3 * vPosition, const _vec3 * vDirectionPerSec)
{
	if (m_vecCell.size() <= m_iCurrentIdx)
		return false;

	LINE			eOutLine =LINE(-1);
	const CCell*	pNeighbor = nullptr;


	for (auto& iter : m_vecCell)
	{
		_bool IsIn = iter->is_InCell(*vPosition + *vDirectionPerSec, &eOutLine);
		if (IsIn)
			return true;
	}
	return false;
}

_bool CNavi::Move_OnNavigation(const _vec3* vPosition, const _vec3* vDirectionPerSec, _float& fY)
{
	if (m_vecCell.size() <= m_iCurrentIdx)
		return false;

	

	LINE			eOutLine = LINE(-1);
	const CCell*	pNeighbor = nullptr;
	_float			fTempY = 0.f;

	_bool	isIn = m_vecCell[m_iCurrentIdx]->is_InCell(*vPosition + *vDirectionPerSec, &eOutLine, fTempY);
	fY = fTempY;
	if (false == isIn)
	{
		// 이웃이 있으면
		if (pNeighbor = m_vecCell[m_iCurrentIdx]->Get_Neighbor(NEIGHBOR(eOutLine)))
		{
			m_iCurrentIdx = pNeighbor->Get_CellIndex();
			
			return true;
		}
		// 이웃이 없으면
		else
		{
			return false;
		}
	}
	else
		return true;

	return false;
}

_bool CNavi::Move_OnNavigation(const _vec3* vPosition, const _vec3* vDirectionPerSec, _float& fY, NAVI eNavi, _vec3* vSliding)
{
	if (m_vecCell.size() <= m_iCurrentIdx)
		return false;
	if (eNavi != m_eNavi)
		return false;
	LINE			eOutLine = LINE(-1);
	const CCell* pNeighbor = nullptr;
	_float			fTempY = 0.f;
	CLine* pLine = nullptr;
	_bool	isIn = m_vecCell[m_iCurrentIdx]->is_InCell(*vPosition + *vDirectionPerSec, &eOutLine, fTempY);
	fY = fTempY;
	if (false == isIn)
	{
		if (pNeighbor = m_vecCell[m_iCurrentIdx]->Get_Neighbor(NEIGHBOR(eOutLine)))
		{
			m_iCurrentIdx = pNeighbor->Get_CellIndex();

			return true;
		}
		else
		{
			pLine = m_vecCell[m_iCurrentIdx]->GetLine(eOutLine);


			_vec3 vNormal = pLine->Get_Normal();
			_vec3 DirectionPerec = *vDirectionPerSec;

			_float fDot = D3DXVec3Dot(&DirectionPerec, &vNormal);
			*vSliding = DirectionPerec - (fDot * vNormal);


			return false;
		}
	}
	else
		return true;

	return false;
}

void CNavi::Render_Navigation()
{
	for (auto& pCell : m_vecCell)
		pCell->Render_Cell();
}

HRESULT CNavi::Delete_Cell()
{
	//auto& iter = m_vecCell.begin();
	//for (; iter != m_vecCell.end();)
	//{
	//	if (iter[0]->GetIsLast() == true)
	//	{
	//		iter = m_vecCell.erase(iter);
	//	}
	//	else
	//		iter++;
	//}

	//if(m_vecCell.size()>=1)
	//	m_vecCell.back()->GetIsLast() = true;


	//for (_int i = 0; i < 3; ++i)
	//	m_vTotlaPos.pop_back();

	return S_OK;
}

HRESULT CNavi::Set_PositionVector(_vec3 vPos)
{
	m_vPos.push_back(vPos);
	m_mapTotlaPos.insert({ TotlaPointIdx , vPos });
	TotlaPointIdx++;
	return S_OK;
}

HRESULT CNavi::Set_CellVector(_vec3 vTotalPos[])
{
	return S_OK;
}

HRESULT CNavi::Set_CellVector(map<_uint, _vec3> matTemp)
{
	return S_OK;

}

HRESULT CNavi::Set_CellVector()
{
	for (auto& iter : m_vecCell)
		iter->GetIsLast() = false;

	CCell*		pCell = CCell::Create(m_pGraphic_Device, &m_vPos[0], &m_vPos[1], &m_vPos[2], m_vecCell.size());
	if (nullptr == pCell)
		return E_FAIL;
	
	pCell->GetIsLast() = true;
	m_vecCell.push_back(pCell);
	return S_OK;
}

HRESULT CNavi::Set_CellVector(vector<_vec3> vTotalPos)
{
	for (auto& iter : m_vecCell)
		iter->GetIsLast() = false;

	for (_uint i = 0; i < vTotalPos.size();)
	{
		CCell*		pCell = CCell::Create(m_pGraphic_Device, &vTotalPos[i], &vTotalPos[i+1], &vTotalPos[i+2], m_vecCell.size());
		if (nullptr == pCell)
			return E_FAIL;
		pCell->GetNavi() = m_eNavi;
		pCell->Compute_Color();

		pCell->GetIsLast() = true;
		m_vecCell.push_back(pCell);
		i += 3;
	}
	return S_OK;
}

void CNavi::Clear_Position_Vector()
{
	m_vPos.clear();
	m_vPos.shrink_to_fit();
}


CNavi * CNavi::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFileName)
{
	CNavi* pInstance = new CNavi(pGraphic_Device);
	if (FAILED(pInstance->Ready_Navigation(pFileName)))
		return nullptr;
	return pInstance;
}

CComponent * CNavi::Clone_Component(void* pArg)
{
	CNavi*	pInstance = new CNavi(*this);

	if (FAILED(pInstance->Ready_Clone_Navigation(pArg)))
	{
		MessageBox(0, L"CNavigation Cloned Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavi::Free()
{
	if (false == m_isClone)
	{
		for (auto& pCell : m_vecCell)
			pCell->Clear_Neighbor();
	}
	for (auto& pCell : m_vecCell)
		Safe_Release(pCell);

	m_vecCell.clear();
	CComponent::Free();
}
