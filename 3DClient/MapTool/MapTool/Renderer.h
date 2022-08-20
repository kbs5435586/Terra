#pragma once
#include "Component.h"

class CTarget_Manager;
class CGameObject;
class CShader;
class CRenderer :
	public CComponent
{
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer()=default;

public:
	HRESULT							Ready_Renderer();
	HRESULT							Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject);
	HRESULT							Render_RenderGroup();
private:
	void							Render_Priority();
	void							Render_NoneAlpha();
	void							Render_Alpha();
	void							Render_UI();
private:
	void							Render_Effect();
private:
	void							Render_Deferred();
	void							Render_LightAcc();
	void							Render_Blend();
private:
	void							Render_Effect_Defered();
public:
	static CRenderer*				Create(LPDIRECT3DDEVICE9 pGraphic_Device); 
	virtual CComponent*				Clone_Component(void* pArg);
	virtual void					Free();
private:
	vector<CGameObject*>			m_RenderList[RENDER_END];
	typedef vector<CGameObject*>	OBJECTLIST;
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
private:
	CTarget_Manager*				m_pTarget_Manager = nullptr;
	CShader*						m_pShaderCom_LightAcc = nullptr;
	CShader*						m_pShaderCom_Blend = nullptr;
};

