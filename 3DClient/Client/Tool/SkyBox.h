#pragma once
#include "GameObject.h"
class CTransform;
class CRenderer;
class CTexture;
class CBuffer_CubeTex;
class CShader;

class CSkyBox :
	public CGameObject
{
private:
	CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox()=default;
public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	HRESULT Ready_Component(); // �� ��ü�ȿ��� ����� ������Ʈ���� ������ü�κ��� ����(����) �ؼ� �´�.
	HRESULT SetUp_ContantTable(LPD3DXEFFECT pEffect);
public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg) override;
	virtual void Free();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CBuffer_CubeTex*	m_pBufferCom = nullptr;
	CShader*			m_pShaderCom = nullptr;


};

