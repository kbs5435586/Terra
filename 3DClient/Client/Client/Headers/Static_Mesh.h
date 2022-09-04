#pragma once
#include "Component.h"
class CStatic_Mesh :
	public CComponent
{
private:
	CStatic_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStatic_Mesh(const CStatic_Mesh& rhs);
	virtual ~CStatic_Mesh() = default;
public:
	HRESULT						Ready_Mesh_Static(const _tchar* pFilePath, const _tchar* pFileName, void* pArg);
	void						Render_Mesh(_uint iAttributeID);
public:
	const SUBSETDESC*			Get_SubSetDesc(const _uint& iAttributeID) const {	return &m_pSubSetDesc[iAttributeID];	}
	const _ulong				Get_NumMaterials() const {	return m_dwNumMaterials;	}
public:
	static CStatic_Mesh*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName, void* pArg = nullptr);
	virtual CComponent*			Clone_Component(void* pArg);
	virtual void				Free();
public:
	_vec3						GetMin() { return m_vMin; }
	_vec3						GetMax() { return m_vMax; }
private:
	HRESULT						Change_TextureFileName(_tchar* pFullPath, const _tchar* pSour, const _tchar* pDest);
	void						Remove_End(_tchar* pFileName);
public:
	_matrix						Get_LocalTransform() const;
private:
	LPD3DXMESH					m_pMesh = nullptr;
	LPD3DXBUFFER				m_pAdjacency = nullptr;
	LPD3DXBUFFER				m_pMaterials = nullptr;
	_ulong						m_dwNumMaterials = 0;
	SUBSETDESC*					m_pSubSetDesc = nullptr;
private:
	_vec3						m_vMin, m_vMax;
	_matrix						m_matPivot;

};

