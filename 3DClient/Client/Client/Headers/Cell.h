#pragma once
#include "Base.h"
class CLine;
class CCell :
	public CBase
{
private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCell() = default;
public:
	const _vec3*				Get_Point(POINT_ ePoint) const {return &m_vPoint[ePoint];}
	const CCell*				Get_Neighbor(NEIGHBOR eNeighbor) const {return m_pNeighbor[eNeighbor];}
	_uint						Get_CellIndex() const {return m_iIndex;}
public:
	_float						Compute_PositionY(const _vec3& vPosition);
public:
	void						Set_Neighbor(NEIGHBOR eNeighbor, CCell* pCell);
public:
	HRESULT						Ready_Cell(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, const _uint& iIndex);
	_bool						is_InCell(_vec3 vMovedPos, LINE* pOutLine);
	_bool						is_InCell(_vec3 vMovedPos, LINE* pOutLine, _float& fY);
	_bool						Compare_Point(const _vec3* pPointA, const _vec3* pPointB);
	void						Clear_Neighbor();
	void						Render_Cell();
	CLine* GetLine(LINE eLine)
	{
		if (m_pLine[(_uint)eLine] == nullptr)
			return nullptr;
		return m_pLine[(_uint)eLine];
	}
public:
	void						Compute_Color();
public:
	NAVI&						GetNavi() { return m_eNavi; }
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPD3DXLINE					m_pLineSDK = nullptr;
	_vec3						m_vPoint[POINT_END];
	_uint						m_iIndex = 0;
private:
	CCell*						m_pNeighbor[NEIGHBOR_END] = { nullptr };
	CLine*						m_pLine[LINE_END];
	NAVI						m_eNavi = NAVI::NAVI_END;
	D3DXCOLOR					m_Color = {};
private:
	_bool						m_IsLast = false;
public:
	_bool						&GetIsLast() { return m_IsLast; }

public:
	static CCell*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, const _uint& iIndex);
	virtual void				Free();
};

