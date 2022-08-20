#pragma once
#include "Component.h"
class CHierarchyLoader;
class CAnimationCtrl;
class CDynamic_Mesh :
	public CComponent
{
private:
	CDynamic_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDynamic_Mesh(const CDynamic_Mesh& rhs);
	virtual ~CDynamic_Mesh()=default;
public:
	_uint										Get_NumMeshContainer() const {return m_vecMeshContainer.size();}
	const D3DXMESHCONTAINER_DERIVED*			Get_MeshContainer(const _uint& iIndex) const {return m_vecMeshContainer[iIndex];}
	_matrix*									Find_Frame(const char* pFrameName) const 
	{
		return &((D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName))->CombinedTransformationMatrix;
	}
public:
	HRESULT										Ready_Mesh_Dynamic(const _tchar* pFilePath, const _tchar* pFileName);
	void										Update_Skinning(const _uint& iMeshContainerIdx);
	void										Update_Skinning(const _uint& iMeshContainerIdx, vector<_matrix> pOut);
	void										Update_Skinning_AnimBlur(const _uint& iMeshContainerIdx, _matrix* pIn);
	void										Render_Mesh(const _uint& iMeshContainerIdx, const _uint& iAttributeID);
	HRESULT										Set_AnimationSet(const _uint& iAnimationID); 
	void										Play_Animation(const _float& fTimeDelta); 
	_bool										Get_EndLoop();
	_uint										Get_TrackIndex();
	_double										Get_CurrTime();
	_double										Get_AllTime();
public:
	_uint										Get_CurrentState();
private:
	CHierarchyLoader*							m_pLoader = nullptr;
	D3DXFRAME*									m_pRootFrame = nullptr;
	_matrix										m_matPivot;
	CAnimationCtrl*								m_pAnimationCtrl = nullptr;
private:
	_uint										m_iAnimIdx = 0;
private:
	vector<D3DXMESHCONTAINER_DERIVED*>			m_vecMeshContainer;
	typedef vector<D3DXMESHCONTAINER_DERIVED*>	VECMESHCONTAINER;
public:
	void										Update_CombinedTransformationMatrices(D3DXFRAME_DERIVED*	pFrame, const _matrix* pParentMatrix);
	HRESULT										SetUp_CombinedMatrixPointer(D3DXFRAME_DERIVED*	pFrame);

public:
	static CDynamic_Mesh*						Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*							Clone_Component(void* pArg);
	virtual void								Free();
};

