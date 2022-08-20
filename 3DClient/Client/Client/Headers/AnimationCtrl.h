#pragma once

#include "Base.h"

class CAnimationCtrl final : public CBase
{
private:
	explicit CAnimationCtrl();
	virtual ~CAnimationCtrl() = default;
public:
	HRESULT Ready_Animation(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	HRESULT Clone_Animation(const CAnimationCtrl& rhs);
	HRESULT Set_AnimationSet(const _uint& iAnimationID); // Ư�� �ִϸ��̼� ������ �غ��Ų��.
	void	Play_Animation(const _float& fTimeDelta); // �ִϸ��̼��� �����Ų��.
public:
	_uint			Get_TrackIndex();
	_double			Get_CurrTime();
	_double			Get_AllTime();
	_bool			Get_EndLoop();
	_bool			Is_EndAnim();
public:
	_uint	Get_CurrentState() { return m_iOldAnimationIdx; }
private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl = nullptr;
	_uint							m_iCurrentTrack = 0;
	_uint							m_iNewTrack = 1;
	_double							m_TimeAcc = 0.0;
	_uint							m_iOldAnimationIdx = 0;

public:
	static CAnimationCtrl* Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	CAnimationCtrl* Clone();
	virtual void Free();
};
