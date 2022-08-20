#include "pch.h"
#include "Collider.h"
#include "Shader.h"
#include "Transform.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pShader(rhs.m_pShader)
	, m_isColl(rhs.m_isColl)
{
	m_pShader->AddRef();
	m_isClone = true;
	rhs.m_pCollider->CloneMeshFVF(rhs.m_pCollider->GetOptions(), rhs.m_pCollider->GetFVF(), rhs.m_pGraphic_Device, &m_pCollider);
}

HRESULT CCollider::Ready_Collider(COLLIDER_TYPE eType)
{
	HRESULT		hr = 0;

	switch (eType)
	{
	case COLLIDER_TYPE_BOX:
		hr = Ready_BoundingBox();
		break;
	case COLLIDER_TYPE_SPHERE:
		hr = Ready_BoundingSphere();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	m_pShader = CShader::Create(m_pGraphic_Device, L"../ShaderFiles/Shader_Collider.fx");
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::Ready_Clone_Collider(void * pArg)
{
	COLLIDER_INFO*			pColliderInfo = (COLLIDER_INFO*)pArg;

	m_ColliderInfo = *pColliderInfo;

	m_eBoxType = pColliderInfo->eBoxType;

	void*			pVertices = nullptr;

	_uint			iVertexSize = D3DXGetFVFVertexSize(m_pCollider->GetFVF());

	m_pCollider->LockVertexBuffer(0, &pVertices);

	_ulong			dwNumVertices = m_pCollider->GetNumVertices();

	for (size_t i = 0; i < dwNumVertices; ++i)
	{
		D3DXVec3TransformCoord((_vec3*)((_byte*)pVertices + (i * iVertexSize)), (_vec3*)((_byte*)pVertices + (i * iVertexSize)), &pColliderInfo->matLocalTransform);
		int a = 10;
	}

	// 로컬영역상의 민, 맥스포인트. 
	D3DXComputeBoundingBox((_vec3*)pVertices, dwNumVertices, iVertexSize, &m_vMin, &m_vMax);
	//0.1f, 0.1f, 0.1f
	//15.f, 1.f, 15.f
	m_pCollider->UnlockVertexBuffer();

	if (BOX_TYPE_OBB == m_eBoxType)
	{
		m_pOBB = new OBB;
		ZeroMemory(m_pOBB, sizeof(OBB));

		m_pOBB->vPoint[0] = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
		m_pOBB->vPoint[1] = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
		m_pOBB->vPoint[2] = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
		m_pOBB->vPoint[3] = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);

		m_pOBB->vPoint[4] = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
		m_pOBB->vPoint[5] = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
		m_pOBB->vPoint[6] = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
		m_pOBB->vPoint[7] = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);
	}

	return S_OK;
}

_bool CCollider::Collision_AABB(const CCollider * pTargetCollider)
{
	// 콜라이더들이 가지고 있는 로컬영역상의 min, max를 각 콜라이더의 월드스페이스변환을 위한 행렬. 
	_matrix		SourMatrix = Compute_WorldTransform();
	_matrix		DestMatrix = pTargetCollider->Compute_WorldTransform();

	_vec3		vSourMin, vSourMax;
	_vec3		vDestMin, vDestMax;

	D3DXVec3TransformCoord(&vSourMin, &m_vMin, &SourMatrix);
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, &SourMatrix);

	D3DXVec3TransformCoord(&vDestMin, &pTargetCollider->m_vMin, &DestMatrix);
	D3DXVec3TransformCoord(&vDestMax, &pTargetCollider->m_vMax, &DestMatrix);

	m_isColl = false;

	// x축선상에서 너비로 겹치지 않은 경우.
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return false;

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return false;

	m_isColl = true;

	return _bool(true);
}

void CCollider::Collision_AABB(const CCollider * pTargetCollider, CTransform* pSourTransform, CTransform* pDestTransform)
{
	// 콜라이더들이 가지고 있는 로컬영역상의 min, max를 각 콜라이더의 월드스페이스변환을 위한 행렬. 
	_matrix		SourMatrix = Compute_WorldTransform();
	_matrix		DestMatrix = pTargetCollider->Compute_WorldTransform();

	_vec3		vSourMin, vSourMax;
	_vec3		vDestMin, vDestMax;


	_vec3 vSourPos = *pSourTransform->Get_StateInfo(STATE_POSITION);
	_vec3 vDestPos = *pDestTransform->Get_StateInfo(STATE_POSITION);

	D3DXVec3TransformCoord(&vSourMin, &m_vMin, &SourMatrix);
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, &SourMatrix);

	D3DXVec3TransformCoord(&vDestMin, &pTargetCollider->m_vMin, &DestMatrix);
	D3DXVec3TransformCoord(&vDestMax, &pTargetCollider->m_vMax, &DestMatrix);

	m_isColl = false;

	if (max(vSourMin.x, vDestMin.x) < min(vSourMax.x, vDestMax.x)
		&& max(vSourMin.z, vDestMin.z) < min(vSourMax.z, vDestMax.z))
	{
		m_isColl = true;
		_float	fMoveX = (min(vSourMax.x, vDestMax.x) - max(vSourMin.x, vDestMin.x));
		_float	fMoveZ = (min(vSourMax.z, vDestMax.z) - max(vSourMin.z, vDestMin.z));

		if (abs(fMoveX) < abs(fMoveZ))
		{
			if (vSourPos.x < vDestPos.x)
			{
				_vec3	vTemp = { pDestTransform->Get_Matrix()._41 + fMoveX,
								  pDestTransform->Get_Matrix()._42,
								  pDestTransform->Get_Matrix()._43 };
				pDestTransform->Set_StateInfo(STATE_POSITION, &vTemp);
			}
			else
			{
				_vec3	vTemp = { pDestTransform->Get_Matrix()._41 - fMoveX,
								  pDestTransform->Get_Matrix()._42,
								  pDestTransform->Get_Matrix()._43 };
				pDestTransform->Set_StateInfo(STATE_POSITION, &vTemp);
			}
			return;
		}
		else
		{
			if (vSourPos.z < vDestPos.z)
			{
				_vec3	vTemp = { pDestTransform->Get_Matrix()._41,
								  pDestTransform->Get_Matrix()._42,
								  pDestTransform->Get_Matrix()._43 + fMoveZ };
				pDestTransform->Set_StateInfo(STATE_POSITION, &vTemp);
			}
			else
			{
				_vec3	vTemp = { pDestTransform->Get_Matrix()._41 ,
								  pDestTransform->Get_Matrix()._42,
								  pDestTransform->Get_Matrix()._43 - fMoveZ };
				pDestTransform->Set_StateInfo(STATE_POSITION, &vTemp);
			}
			return;
		}

	}


	return;
}

_bool CCollider::Collision_OBB(const CCollider * pTargetCollider)
{
	const OBB*	pTargetOBB = pTargetCollider->Get_OBBDescPointer();
	if (nullptr == pTargetOBB)
		return false;


	m_isColl = false;

	OBB			tOBB[2];
	ZeroMemory(tOBB, sizeof(OBB) * 2);

	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&tOBB[0].vPoint[i], &m_pOBB->vPoint[i], &Compute_WorldTransform());
		D3DXVec3TransformCoord(&tOBB[1].vPoint[i], &pTargetOBB->vPoint[i], &pTargetCollider->Compute_WorldTransform());
	}

	tOBB[0].vCenter = (tOBB[0].vPoint[0] + tOBB[0].vPoint[6]) * 0.5f;
	tOBB[1].vCenter = (tOBB[1].vPoint[0] + tOBB[1].vPoint[6]) * 0.5f;

	for (size_t i = 0; i < 2; ++i)
	{
		Compute_AlignAxis(&tOBB[i]);
		Compute_ProjAxis(&tOBB[i]);
	}

	_vec3	vAlignAxis[9];

	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			_uint		iIndex = i * 3 + j;
			D3DXVec3Cross(&vAlignAxis[iIndex], &tOBB[0].vAlignAxis[i], &tOBB[1].vAlignAxis[j]);
		}
	}

	// 충돌비교!
	_float		fDistance[3];

	// 중점과 중점을 잇는 벡터의 길이.

	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			_vec3		vCenterDir = tOBB[1].vCenter - tOBB[0].vCenter;
			fDistance[0] = fabs(D3DXVec3Dot(&vCenterDir, &tOBB[i].vAlignAxis[j]));


			//투영
			fDistance[1] =
				fabs(D3DXVec3Dot(&tOBB[0].vProjAxis[0], &tOBB[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&tOBB[0].vProjAxis[1], &tOBB[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&tOBB[0].vProjAxis[2], &tOBB[i].vAlignAxis[j]));



			fDistance[2] =
				fabs(D3DXVec3Dot(&tOBB[1].vProjAxis[0], &tOBB[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&tOBB[1].vProjAxis[1], &tOBB[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&tOBB[1].vProjAxis[2], &tOBB[i].vAlignAxis[j]));

			if (fDistance[1] + fDistance[2] < fDistance[0])
				return false;

		}
	}

	for (size_t i = 0; i < 9; ++i)
	{
		_vec3		vCenterDir = tOBB[1].vCenter - tOBB[0].vCenter;
		fDistance[0] = fabs(D3DXVec3Dot(&vCenterDir, &vAlignAxis[i]));

		fDistance[1] = fabs(D3DXVec3Dot(&tOBB[0].vProjAxis[0], &vAlignAxis[i])) +
			fabs(D3DXVec3Dot(&tOBB[0].vProjAxis[1], &vAlignAxis[i])) +
			fabs(D3DXVec3Dot(&tOBB[0].vProjAxis[2], &vAlignAxis[i]));

		fDistance[2] = fabs(D3DXVec3Dot(&tOBB[1].vProjAxis[0], &vAlignAxis[i])) +
			fabs(D3DXVec3Dot(&tOBB[1].vProjAxis[1], &vAlignAxis[i])) +
			fabs(D3DXVec3Dot(&tOBB[1].vProjAxis[2], &vAlignAxis[i]));

		if (fDistance[1] + fDistance[2] < fDistance[0])
			return false;
	}


	return _bool(m_isColl = true);
}

void CCollider::Render_Collider()
{
	if (nullptr == m_pShader)
		return;

	LPD3DXEFFECT pEffect = m_pShader->GetEffectHandle();

	if (nullptr == pEffect)
		return;

	_matrix		matTransform;

	if (nullptr != m_ColliderInfo.pBoneMatrix)
		matTransform = *m_ColliderInfo.pBoneMatrix * *m_ColliderInfo.pWorldMatrix;
	else
		matTransform = *m_ColliderInfo.pWorldMatrix;


	matTransform = m_eBoxType == BOX_TYPE_AABB ? Remove_Rotation(matTransform) : matTransform;

	pEffect->SetMatrix("g_matWorld", &matTransform);

	_matrix		matView, matProj;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	_vec4		vColor;

	vColor = m_isColl == true ? _vec4(1.f, 0.f, 0.f, 1.f) : _vec4(0.f, 1.f, 0.f, 1.f);

	pEffect->SetVector("g_vColor", &vColor);


	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pCollider->DrawSubset(0);

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CCollider::Update_Collider_Info(_matrix matInfo)
{
	m_ColliderInfo.pWorldMatrix = &matInfo;
	return S_OK;
}

HRESULT CCollider::Ready_BoundingBox()
{
	if (FAILED(D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pCollider, &m_pAdjacency)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCollider::Ready_BoundingSphere()
{
	if (FAILED(D3DXCreateSphere(m_pGraphic_Device, 0.5f, 10, 10, &m_pCollider, &m_pAdjacency)))
		return E_FAIL;

	return NOERROR;
}

_matrix CCollider::Remove_Rotation(_matrix matTransform) const
{
	_vec3			vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= D3DXVec3Length((_vec3*)&matTransform.m[0][0]);
	vUp *= D3DXVec3Length((_vec3*)&matTransform.m[1][0]);
	vLook *= D3DXVec3Length((_vec3*)&matTransform.m[2][0]);

	memcpy(&matTransform.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&matTransform.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&matTransform.m[2][0], &vLook, sizeof(_vec3));

	return _matrix(matTransform);
}

_matrix CCollider::Compute_WorldTransform() const
{
	_matrix		matTransform;

	if (nullptr == m_ColliderInfo.pBoneMatrix)
		matTransform = *m_ColliderInfo.pWorldMatrix;
	else
	{

		matTransform = *m_ColliderInfo.pBoneMatrix * *m_ColliderInfo.pWorldMatrix;
	}

	if (BOX_TYPE_AABB == m_ColliderInfo.eBoxType)
		matTransform = Remove_Rotation(matTransform);


	return _matrix(matTransform);
}

void CCollider::Compute_AlignAxis(OBB * pOBB)
{
	pOBB->vAlignAxis[0] = pOBB->vPoint[2] - pOBB->vPoint[3];
	pOBB->vAlignAxis[1] = pOBB->vPoint[0] - pOBB->vPoint[3];
	pOBB->vAlignAxis[2] = pOBB->vPoint[7] - pOBB->vPoint[3];

	for (size_t i = 0; i < 3; ++i)
	{
		D3DXVec3Normalize(&pOBB->vAlignAxis[i], &pOBB->vAlignAxis[i]);

	}
}

void CCollider::Compute_ProjAxis(OBB * pOBB)
{
	pOBB->vProjAxis[0] = (pOBB->vPoint[5] + pOBB->vPoint[2]) * 0.5f - pOBB->vCenter;
	pOBB->vProjAxis[1] = (pOBB->vPoint[5] + pOBB->vPoint[0]) * 0.5f - pOBB->vCenter;
	pOBB->vProjAxis[2] = (pOBB->vPoint[5] + pOBB->vPoint[7]) * 0.5f - pOBB->vCenter;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER_TYPE eType)
{
	CCollider* pInstance = new CCollider(pGraphic_Device);
	if (FAILED(pInstance->Ready_Collider(eType)))
		return nullptr;
	return pInstance;
}

CComponent * CCollider::Clone_Component(void * pArg)
{
	CCollider*	pCollider = new CCollider(*this);

	if (FAILED(pCollider->Ready_Clone_Collider(pArg)))
	{
		_MSG_BOX("CCollider Cloned Failed");
		Safe_Release(pCollider);
	}
	return pCollider;
}

void CCollider::Free()
{
	if (m_isClone)
		Safe_Delete(m_pOBB);
	Safe_Release(m_pShader);
	Safe_Release(m_pCollider);
	CComponent::Free();
}
