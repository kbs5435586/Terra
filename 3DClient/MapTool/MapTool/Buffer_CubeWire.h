#pragma once
#include "VIBuffer.h"
class CBuffer_CubeWire :
	public CVIBuffer
{
private:
	CBuffer_CubeWire(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBuffer_CubeWire(const CBuffer_CubeWire& rhs);
	virtual ~CBuffer_CubeWire() = default;
public:
	HRESULT Ready_VIBuffer();
	void Render_VIBuffer();
public:
	virtual _bool Picking_ToBuffer(_vec3* pOut, const CTransform* pTransformCom, const CPicking* pPickingCom);
public:
	static CBuffer_CubeWire* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component(void* pArg);
protected:
	virtual void Free();
};

