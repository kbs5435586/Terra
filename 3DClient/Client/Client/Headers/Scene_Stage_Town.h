#pragma once
#include "Scene.h"
class CScene_Stage_Town :
	public CScene
{
private:
	explicit CScene_Stage_Town(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Stage_Town() = default;
public:
	virtual HRESULT				Ready_Scene() override;
	virtual _int				Update_Scene(const _float& fTimeDelta) override;
	virtual _int				LastUpdate_Scene(const _float& fTimeDelta) override;
	virtual void				Render_Scene() override;
private:
	HRESULT						Ready_Prototype_GameObject();
	HRESULT						Ready_Prototype_Component();
private:
	HRESULT						Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT						Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT						Ready_Layer_Light_Camera(const _tchar* pLayerTag);
	HRESULT						Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT						Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT						Ready_Layer_Shiraken(const _tchar* pLayerTag);
	HRESULT						Ready_Layer_Particle(const _tchar* pLayerTag);
	HRESULT						Ready_Layer_Test(const _tchar* pLayerTag);
private:
	HRESULT						Load_Object(const _tchar* pFilePath);
	HRESULT						Load_NaviMesh(const _tchar* pFilePath);
	HRESULT						Load_UI(const _tchar* pFilePath);
private:
	HRESULT						Ready_LightInfo();
private:
	vector< _tchar*>			m_vecComponentTag;
public:
	static CScene_Stage_Town*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void				Free();
};


