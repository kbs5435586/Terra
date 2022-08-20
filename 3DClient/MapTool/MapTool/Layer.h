#pragma once
#include "Base.h"
#include "GameObject.h"
class CLayer :
	public CBase
{
public:
	CLayer();
	virtual ~CLayer()=default;
public:
	HRESULT Add_Object(CGameObject* pGameObject);
	HRESULT Ready_Layer();
	_int Update_Object(const _float& fTimeDelta);
	_int LastUpdate_Object(const _float& fTimeDelta);
public:
	CComponent*			Get_ComponentPointer(const _tchar* pComponentTag, const _uint& iIdx);
public:
	vector<CGameObject*>* GetGameObjectList();
	CGameObject*		Get_GameObject(const _uint& iIndex);
	CGameObject*		Get_BackObject();
public:
	static CLayer*	Create();
protected:
	virtual void Free();
private:
	vector<CGameObject*>			m_ObjectList;

};

