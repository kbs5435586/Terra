#pragma once
#include "Component.h"
class CCell;
class CNavi :
	public CComponent
{
private:
	CNavi(LPDIRECT3DDEVICE9 pGraphic_Device);
	CNavi(const CNavi& rhs);
	virtual ~CNavi()=default;
public:
	HRESULT							Ready_Navigation(const _tchar* pFileName);
	HRESULT							Ready_Neighbor();
	HRESULT							Ready_Clone_Navigation(void* pArg);
	_bool							Move_OnNavigation(const _vec3* vPosition, const _vec3* vDirectionPerSec);
	_bool							Move_OnNavigation(const _vec3* vPosition, const _vec3* vDirectionPerSec, _float& fY);
	_bool							Move_OnNavigation(const _vec3* vPosition, const _vec3* vDirectionPerSec, _float& fY, NAVI eNavi, _vec3* vSliding = nullptr);
	void							Render_Navigation();
public:
	HRESULT							Delete_Cell();
public:
	HRESULT							Set_PositionVector(_vec3 vPos);
	HRESULT							Set_PositionVector(_uint iIdx, _vec3 vPos);
	HRESULT							Set_CellVector();
	HRESULT							Set_CellVector(NAVI navi);
	HRESULT							Set_CellVector(vector<_vec3> vTotalPos);
	HRESULT							Set_CellVector(_vec3 vTotalPos[]);
	HRESULT							Set_CellVector(map<_uint, _vec3> mapTemp);

	void							Clear_Position_Vector();
public:
	map<_uint, _vec3>				&GetTotalPos() { return m_mapTotlaPos; }
	vector<CCell*>					&GetCellVector() { return m_vecCell; }
	NAVI							&GetNavi() { return m_eNavi; }
private:	
	NAVI							m_eNavi = NAVI::NAVI_END;
private:
	vector<CCell*>					m_vecCell;
	typedef vector<CCell*>			VECCELL;
private:
	vector<_vec3>					m_vPos;
	map<_uint, _vec3>				m_mapTotlaPos;
private:
	_uint							m_iCurrentIdx = 0;
private:
	_uint							TotlaPointIdx = 0;
public:
	static CNavi*					Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFileName);
	virtual CComponent*				Clone_Component(void* pArg);
	virtual void					Free();

};

