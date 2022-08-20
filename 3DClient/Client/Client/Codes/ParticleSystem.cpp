#include "stdafx.h"
#include "..\Headers\ParticleSystem.h"
#include "Management.h"

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_vb = 0;
}

CParticleSystem::CParticleSystem(CParticleSystem& rhs)
	: CGameObject(rhs)
	, m_vb(rhs.m_vb)
	, m_tBoundingBox(rhs.m_tBoundingBox)
	, m_fSize(rhs.m_fSize)
	, m_iVBSize(rhs.m_iVBSize)
	, m_iVBOffset(rhs.m_iVBOffset)
	, m_iVBBatchSize(rhs.m_iVBBatchSize)
{
	m_vb->AddRef();
}

HRESULT CParticleSystem::Ready_GameObject()
{
	return S_OK;
}
_int CParticleSystem::Update_GameObject(const _float& fTimeDelta)
{

	return _int();
}

_int CParticleSystem::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;

	return _int();
}

void CParticleSystem::Render_GameObject()
{
	if (m_lstParticle.empty())
		return;


	//Pre_Render();


	D3DXCOLOR temp;
	D3DXCOLOR red = D3DXCOLOR(1.f,0.f,0.f, 1.f);
	D3DXCOLOR yellow = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	D3DXColorLerp(&temp, &red, &yellow, m_fTimeDelta/ 1.f);



	m_vParticleColor.x = temp.r;
	m_vParticleColor.y = temp.g;
	m_vParticleColor.z = temp.b;
	m_vParticleColor.w = temp.a;

	if (FAILED(SetUp_ConstantTable(m_pShaderCom->GetEffectHandle())))
		return ;


	

	m_pShaderCom->GetEffectHandle()->Begin(nullptr, 0);
	m_pShaderCom->GetEffectHandle()->BeginPass(0);


	m_pGraphic_Device->SetFVF(D3DFVF_XYZ| D3DFVF_DIFFUSE);
	
	m_pGraphic_Device->SetStreamSource(0, m_vb, 0, sizeof(PSYSTEM));

	if (m_iVBOffset >= m_iVBSize)
		m_iVBOffset = 0;

	PSYSTEM* pPS = 0;

	m_vb->Lock(
		m_iVBOffset * sizeof(PSYSTEM),
		m_iVBBatchSize * sizeof(PSYSTEM),
		(void**)&pPS,
		m_iVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

	DWORD iNumParticleBatch = 0;

	auto iter = m_lstParticle.begin();

	for (; iter != m_lstParticle.end(); ++iter)
	{
		if (iter->isAlive)
		{
			pPS->vPos = iter->vPosition;

			_vec2	vUV;
			vUV.x = pPS->vPos.x ;
			vUV.y = pPS->vPos.y ;
			pPS->vColor = iter->vColor;
			++pPS;

			++iNumParticleBatch;

			if (iNumParticleBatch == m_iVBBatchSize)
			{
				m_vb->Unlock();
				m_pGraphic_Device->DrawPrimitive(
					D3DPT_POINTLIST,
					m_iVBOffset,
					m_iVBBatchSize);


				m_iVBOffset += m_iVBBatchSize;
				if (m_iVBOffset >= m_iVBSize)
					m_iVBOffset = 0;

				m_vb->Lock(
					m_iVBOffset * sizeof(PSYSTEM),
					m_iVBBatchSize * sizeof(PSYSTEM),
					(void**)&pPS,
					m_iVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

				iNumParticleBatch = 0; // reset for new batch
			}
		}
	}

	m_vb->Unlock();

	if (iNumParticleBatch)
	{
		m_pGraphic_Device->DrawPrimitive(
			D3DPT_POINTLIST,
			m_iVBOffset,
			iNumParticleBatch);
	}
	m_iVBOffset += m_iVBBatchSize;



	//Post_Render();


	m_pShaderCom->GetEffectHandle()->EndPass();
	m_pShaderCom->GetEffectHandle()->End();
		
}

bool CParticleSystem::Init()
{
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(
		m_iVBSize * sizeof(PSYSTEM),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ | D3DFVF_DIFFUSE,D3DPOOL_DEFAULT,
		&m_vb, 0)))
	{
		return false;
	}

	return true;
}

void CParticleSystem::Reset()
{
	auto iter = m_lstParticle.begin();
	for (; iter != m_lstParticle.end(); ++iter)
		Reset_Particle(&(*iter));
}

void CParticleSystem::Add_Particle()
{
	PSATTRIBUTE tAttribute;
	Reset_Particle(&tAttribute);
	m_lstParticle.push_back(tAttribute);
}

void CParticleSystem::Pre_Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);//텍스처를 포인트 스프라이트에 텍스처 매핑을 할 것이다.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);//카메라와 가까우면 크게 멀먼 작게.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));//모든 크기 0.8
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// control the size of the particle relative to distance
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// use alpha from texture
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
}

void CParticleSystem::Post_Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);


}

void CParticleSystem::Remove_Dead_Particle()
{
	auto iter = m_lstParticle.begin();

	while (iter != m_lstParticle.end())
	{
		if (!iter->isAlive)
		{
			iter = m_lstParticle.erase(iter);
		}
		else ++iter;
	}
}

_bool CParticleSystem::IsEmpty()
{
	return m_lstParticle.empty();
}

_bool CParticleSystem::IsDead()
{
	auto iter = m_lstParticle.begin();
	for (; iter != m_lstParticle.end(); ++iter)
	{
		if (iter->isAlive)
			return false;
	}
	return true;
}

_float CParticleSystem::GetRandom_Float(_float fLowBound, _float fHighBound)
{
	if (fLowBound >= fHighBound)
		return fLowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (fHighBound - fLowBound)) + fLowBound;
}

void CParticleSystem::GetRandom_Vector(_vec3& vOut, _vec3& vMin, _vec3& vMax)
{
	vOut.x =GetRandom_Float(vMin.x, vMax.x);
	vOut.y =GetRandom_Float(vMin.y, vMax.y);
	vOut.z =GetRandom_Float(vMin.z, vMax.z);
}

HRESULT CParticleSystem::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetInt("g_iParticleSize", m_iParticleSize);

	pEffect->SetVector("g_vParticleColor", &m_vParticleColor);

	

	return S_OK;
}



void CParticleSystem::Free()
{
	Safe_Release(m_vb);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	CGameObject::Free();
}
