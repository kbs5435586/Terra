#include "stdafx.h"
#include "AnimationCtrl.h"

CAnimationCtrl::CAnimationCtrl()
{
}

HRESULT CAnimationCtrl::Ready_Animation(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	m_pAniCtrl = pAniCtrl;
	m_pAniCtrl->AddRef();

	return NOERROR;
}

HRESULT CAnimationCtrl::Clone_Animation(const CAnimationCtrl & rhs)
{
	if (FAILED(rhs.m_pAniCtrl->CloneAnimationController(
		rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),
		rhs.m_pAniCtrl->GetMaxNumTracks(),
		rhs.m_pAniCtrl->GetMaxNumEvents(),
		&m_pAniCtrl)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CAnimationCtrl::Set_AnimationSet(const _uint & iAnimationID)
{
	if (m_iOldAnimationIdx == iAnimationID)
		return NOERROR;

	LPD3DXANIMATIONSET		pAniSet = nullptr;

	m_pAniCtrl->GetAnimationSet(iAnimationID, &pAniSet);

	if (nullptr == pAniSet)
		return E_FAIL;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;

	if (FAILED(m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAniSet)))
		return E_FAIL;

	Safe_Release(pAniSet);

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeAcc + 0.25);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_TimeAcc, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_TimeAcc, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_TimeAcc, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_TimeAcc, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	m_pAniCtrl->ResetTime();
	m_TimeAcc = 0.0;

	m_iCurrentTrack = m_iNewTrack;
	m_iOldAnimationIdx = iAnimationID;

	return NOERROR;
}

void CAnimationCtrl::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, nullptr);
	m_TimeAcc += fTimeDelta;
}
_uint CAnimationCtrl::Get_TrackIndex()
{
	_double AllTime = Get_AllTime();
	_double CurrTime = Get_CurrTime();

	_uint Index = (_uint)((CurrTime / AllTime) * 100);

	return Index;
}
_double CAnimationCtrl::Get_CurrTime()
{
	D3DXTRACK_DESC			TrackDesc;

	LPD3DXANIMATIONSET		pAniSet = nullptr;
	m_pAniCtrl->GetAnimationSet(m_iOldAnimationIdx, &pAniSet);
	double animationTime = pAniSet->GetPeriod();


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	double CureentPostion = TrackDesc.Position;
	int Double = 1;
	if (animationTime < TrackDesc.Position)
	{
		Double = (int)((TrackDesc.Position / animationTime) * 100) / 100;

		CureentPostion = TrackDesc.Position - (animationTime * Double);
	}


	return CureentPostion;
}
_double CAnimationCtrl::Get_AllTime()
{
	LPD3DXANIMATIONSET		pAniSet = nullptr;
	m_pAniCtrl->GetAnimationSet(m_iOldAnimationIdx, &pAniSet);
	double animationTime = pAniSet->GetPeriod();


	return animationTime;
}
_bool CAnimationCtrl::Get_EndLoop()
{
	_bool isEnd = false;
	D3DXTRACK_DESC			TrackDesc;

	LPD3DXANIMATIONSET		pAniSet = nullptr;
	m_pAniCtrl->GetAnimationSet(m_iOldAnimationIdx, &pAniSet);
	double animationTime = pAniSet->GetPeriod();


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	if (animationTime - .1f <= TrackDesc.Position)
		isEnd = true;
	return _bool(isEnd);
}
_bool CAnimationCtrl::Is_EndAnim()
{
	_bool isEnd = false;
	D3DXTRACK_DESC			TrackDesc;

	LPD3DXANIMATIONSET		pAniSet = nullptr;
	m_pAniCtrl->GetAnimationSet(m_iOldAnimationIdx, &pAniSet);
	double animationTime = pAniSet->GetPeriod();


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	if (animationTime - .1f <= TrackDesc.Position)
		isEnd = true;
	return _bool(isEnd);
}
CAnimationCtrl * CAnimationCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAnimationCtrl*	pInstance = new CAnimationCtrl();

	if (FAILED(pInstance->Ready_Animation(pAniCtrl)))
	{
		MessageBox(0, L"pAniCtrl Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;

}

CAnimationCtrl * CAnimationCtrl::Clone()
{
	CAnimationCtrl*	pInstance = new CAnimationCtrl();

	if (FAILED(pInstance->Clone_Animation(*this)))
	{
		MessageBox(0, L"pAniCtrl Cloned Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;

}

void CAnimationCtrl::Free()
{
	Safe_Release(m_pAniCtrl);
}
