
// MapToolView.cpp: CMapToolView 클래스의 구현
//

#include "pch.h"
#include "pch.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapTool.h"
#endif

#include "MapToolDoc.h"
#include "MapToolView.h"


#include "MainFrm.h"
#include "SecondWnd.h"
#include "NaviTool.h"

#include "Management.h"
#include "GameObject.h"
#include "Layer.h"
#include "Cell.h"

#include "UnitTool.h"
#include "CameraTool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CView)

BEGIN_MESSAGE_MAP(CMapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapToolView 그리기

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMapToolView 인쇄

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView 메시지 처리기


void CMapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CSecondWnd* pSecondWnd = dynamic_cast<CSecondWnd*>(pMainFrm->m_MainSplt.GetPane(0, 1));

	switch (eMode)
	{
	case TOOL_MODE::MODE_UNIT:
		Select_Mode_Drag_Unit(pMainFrm, pSecondWnd);
		break;
	case TOOL_MODE::MODE_EFFECT:
		break;
	case TOOL_MODE::MODE_NAVIMESH:
		Select_Mode_Drag_Navi(pMainFrm, pSecondWnd);
		break;
	}

	CView::OnMouseMove(nFlags, point);
}


void CMapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	RECT rcMainFrm = {};
	pMainFrm->GetWindowRect(&rcMainFrm);
	SetRect(&rcMainFrm, 0, 0,
		rcMainFrm.right - rcMainFrm.left, rcMainFrm.bottom - rcMainFrm.top);

	RECT rcView = {};
	GetClientRect(&rcView);
	int iRowFrm = rcMainFrm.right - rcView.right;
	int iColFrm = rcMainFrm.bottom - rcView.bottom;

	pMainFrm->SetWindowPos(nullptr, 0, 0, g_iBackCX + iRowFrm, g_iBackCY + iColFrm, SWP_NOZORDER);
}


void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CSecondWnd* pSecondWnd = dynamic_cast<CSecondWnd*>(pMainFrm->m_MainSplt.GetPane(0, 1));

	switch (eMode)
	{
	case TOOL_MODE::MODE_UNIT:
		Select_Mode_Unit(pMainFrm, pSecondWnd);
		break;
	case TOOL_MODE::MODE_EFFECT:
		break;
	case TOOL_MODE::MODE_UI:
		break;
	case TOOL_MODE::MODE_NAVIMESH:
		Select_Mode_Navi(pMainFrm, pSecondWnd);
		break;
	case TOOL_MODE::MODE_CAMERA:
		Select_Mode_Camera(pMainFrm, pSecondWnd);
		break;
	}
	CView::OnLButtonDown(nFlags, point);
}

void CMapToolView::Select_Mode_Unit(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
	if (IsDrag == false)
		return;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Transform");
	if (nullptr == pTransform)
	{
		Safe_Release(pManagement);
		return;
	}
	CBuffer_Terrain* pBuffer = (CBuffer_Terrain*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Buffer");
	if (nullptr == pBuffer)
	{
		Safe_Release(pManagement);
		return;
	}
	CPicking* pPicking = (CPicking*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Picking");
	if (nullptr == pPicking)
	{
		Safe_Release(pManagement);
		return;
	}


	_vec3 vPos = {};

	// if Mouse Fix Center Display
	if (IsMouse)
	{
		// Terrain Buffer
		if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
		{
			if (pSecondWnd->m_pUnitTool->m_Radio_Creative[0].GetCheck())	// if Crreative Mode On
			{
				if (pSecondWnd->m_pUnitTool->m_eMeshType == MESH_TYPE::STATIC)
				{
					TEMP tTemp = TEMP(pSecondWnd->m_pUnitTool->m_pComponent_Tag, vPos);
					if (FAILED(pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObject_Tag,
						SCENE_STATIC, pSecondWnd->m_pUnitTool->m_pLayer_Tag, (void*)&tTemp)))
					{
						Safe_Release(pManagement);
						return;
					}
					_vec3 vSize = { pSecondWnd->m_pUnitTool->m_fScaleX, pSecondWnd->m_pUnitTool->m_fScaleY,  pSecondWnd->m_pUnitTool->m_fScaleZ };
					_vec3 vRotate = { pSecondWnd->m_pUnitTool->m_fRotX, pSecondWnd->m_pUnitTool->m_fRotY,  pSecondWnd->m_pUnitTool->m_fRotZ };
					CTransform* pTempTransform = (CTransform*)pManagement->Get_BackObject(SCENE_STATIC, pSecondWnd->m_pUnitTool->m_pLayer_Tag)->Get_ComponentPointer(L"Com_Transform");
					pTempTransform->Scaling(vSize);
					if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[0].GetCheck())
					{

						pSecondWnd->m_pUnitTool->m_fRotY = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotZ = 0.f;
						pTempTransform->SetUp_RotationX(D3DXToRadian(vRotate.x));
					}
					else 	if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[1].GetCheck())
					{
						pSecondWnd->m_pUnitTool->m_fRotX = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotZ = 0.f;
						pTempTransform->SetUp_RotationY(D3DXToRadian(vRotate.y));
					}
					else 	if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[2].GetCheck())
					{
						pSecondWnd->m_pUnitTool->m_fRotY = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotX = 0.f;
						pTempTransform->SetUp_RotationZ(D3DXToRadian(vRotate.z));
					}
				}
				else if (pSecondWnd->m_pUnitTool->m_eMeshType == MESH_TYPE::DYNAMIC)
				{
					TEMP tTemp = TEMP(pSecondWnd->m_pUnitTool->m_pComponent_Tag, vPos);
					if (FAILED(pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObject_Tag,
						SCENE_STATIC, pSecondWnd->m_pUnitTool->m_pLayer_Tag, (void*)&tTemp)))
					{
						Safe_Release(pManagement);
						return;
					}
					_vec3 vSize = { pSecondWnd->m_pUnitTool->m_fScaleX, pSecondWnd->m_pUnitTool->m_fScaleY,  pSecondWnd->m_pUnitTool->m_fScaleZ };
					_vec3 vRotate = { pSecondWnd->m_pUnitTool->m_fRotX, pSecondWnd->m_pUnitTool->m_fRotY,  pSecondWnd->m_pUnitTool->m_fRotZ };
					CTransform* pTempTransform = (CTransform*)pManagement->Get_BackObject(SCENE_STATIC, pSecondWnd->m_pUnitTool->m_pLayer_Tag)->Get_ComponentPointer(L"Com_Transform");
					pTempTransform->Scaling(vSize);
					if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[0].GetCheck())
					{

						pSecondWnd->m_pUnitTool->m_fRotY = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotZ = 0.f;
						pTempTransform->SetUp_RotationX(D3DXToRadian(vRotate.x));
					}
					else 	if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[1].GetCheck())
					{
						pSecondWnd->m_pUnitTool->m_fRotX = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotZ = 0.f;
						pTempTransform->SetUp_RotationY(D3DXToRadian(vRotate.y));
					}
					else 	if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[2].GetCheck())
					{
						pSecondWnd->m_pUnitTool->m_fRotY = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotX = 0.f;
						pTempTransform->SetUp_RotationZ(D3DXToRadian(vRotate.z));
					}
				}
				//pSecondWnd->m_pUnitTool->m_eMeshType = MESH_TYPE::NONE;

				map<const _tchar*, CLayer*>	mapTemp = *pManagement->Get_MapLayer(SCENE_STATIC);
				for (auto& iter_in : mapTemp)
				{
					auto& iter_Object = iter_in.second->GetGameObjectList()->begin();
					for (; iter_Object != iter_in.second->GetGameObjectList()->end(); ++iter_Object)
						(*iter_Object)->GetIsPick() = false;
				}

				CGameObject* pGameObject = CManagement::GetInstance()->Get_BackObject(SCENEID::SCENE_STATIC, pSecondWnd->m_pUnitTool->m_pLayer_Tag);
				pGameObject->GetIsPick() = true;

			}
			else if (pSecondWnd->m_pUnitTool->m_Radio_Creative[1].GetCheck())	// if Creative Mode Off
			{
				_float fLength = 0.f;
				// map을 순회해서 내부에 있는 모든 객체들의 IsPick 변수를 false로 만듬
				// 이 temp 변수를 사용해서 삭제를 유연하게 만들 예정
				map<const _tchar*, CLayer*>	mapLayer = *pManagement->Get_MapLayer(SCENE_STATIC);
				for (auto& layer : mapLayer)
				{
					auto& GameObject = layer.second->GetGameObjectList()->begin();
					for (; GameObject != layer.second->GetGameObjectList()->end(); ++GameObject)
					{
						CTransform* pTransform = (CTransform*)(*GameObject)->Get_ComponentPointer(L"Com_Transform");
						_vec3 vTempPos = *pTransform->Get_StateInfo(STATE_POSITION);

						_vec3 vTemp = vTempPos - vPos;
						fLength = D3DXVec3Length(&vTemp);

						if (fLength < 3.f)
						{
							//모든 객체들이 가진 IsPick변수의 값을 false로 바꿔준다.
							map<const _tchar*, CLayer*>	mapTemp = *pManagement->Get_MapLayer(SCENE_STATIC);
							for (auto& iter_in : mapTemp)
							{
								auto& iter_Object = iter_in.second->GetGameObjectList()->begin();
								for (; iter_Object != iter_in.second->GetGameObjectList()->end(); ++iter_Object)
									(*iter_Object)->GetIsPick() = false;
							}

							//해당 객체만 IsPick 변수를 true
							(*GameObject)->GetIsPick() = true;
							break;
						}
					}
				}
			}
		}
	}


	pManagement->Find_Clear_Layer(SCENE_STATIC, L"Layer_Drag");
	if (pSecondWnd->m_pUnitTool->m_eMeshType == MESH_TYPE::NONE)
	{
		//드래그 할 때 화면에 생성되는 객체들을 다 삭제 시킨다.
		pManagement->Find_Clear_Layer(SCENE_STATIC, L"Layer_Drag");
	}
	Safe_Release(pManagement);
}

void CMapToolView::Select_Mode_Navi(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Transform");
	if (nullptr == pTransform)
		return;
	CBuffer_Terrain* pBuffer = (CBuffer_Terrain*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Buffer");
	if (nullptr == pBuffer)
		return;
	CPicking* pPicking = (CPicking*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Picking");
	if (nullptr == pPicking)
		return;

	CNavi* pNavi = (CNavi*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Navi");
	if (nullptr == pNavi)
		return;

	_vec3 vPos = {};
	_bool IsPick = false;


	if (g_eNaviMode == NAVI_MODE::NAVI_MODE_CREATE)//create
	{
		if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
		{
			if (m_iNaviMeshCnt < 3)
			{
				//vPos.y = 5.f;

				int iPointIdx = 0;
				bool result = false;
				if (pNavi->GetTotalPos().empty())
					result = true;
				for (auto& iter : pNavi->GetTotalPos())
				{
					_vec3 vTemp = {};
					_vec3	vIter_Pos = iter.second;
					vIter_Pos.y = 0.f;

					_vec3	vTemp_Pos = vPos;
					vTemp_Pos.y = 0.f;

					vTemp = vIter_Pos - vTemp_Pos;
					_float fLength = D3DXVec3Length(&vTemp);
					if (fLength < 3.5f)
					{
						vPos = iter.second;
						iPointIdx = iter.first;
						result = false;
						break;
					}
					else
					{
						result = true;
					}
				}

				if (result == false)
				{

					if (FAILED(pNavi->Set_PositionVector(iPointIdx, vPos)))
						return;
					m_iNaviMeshCnt++;
				}
				else
				{
					if (FAILED(CManagement::GetInstance()->Add_GameObjectToLayer(L"GameObject_Cube_Wire", SCENE_STATIC, L"Layer_CubeWire", (void*)&vPos)))
						return;
					if (FAILED(pNavi->Set_PositionVector(vPos)))
						return;
					m_iNaviMeshCnt++;
				}


			}



			if (m_iNaviMeshCnt > 2)
			{
				if (FAILED(pNavi->Set_CellVector(eNavi)))
					return;
				pNavi->Ready_Neighbor();
				pNavi->Clear_Position_Vector();
				m_iNaviMeshCnt = 0;
			}

		}
		//if (IsMouse)
		//{
		//	IsPick = pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking);
		//	vPos.y = 5.f;
		//	if (m_iNaviMeshCnt < 3)
		//	{
		//		if (IsPick)
		//		{
		//			for (auto& iter : pNavi->GetTotalPos())
		//			{
		//				_vec3 vTemp = {};
		//				_vec3	vIter_Pos = iter;
		//				vIter_Pos.y = 0.f;

		//				_vec3	vTemp_Pos = vPos;
		//				vTemp_Pos.y = 0.f;

		//				vTemp = vIter_Pos - vTemp_Pos;
		//				_float fLength = D3DXVec3Length(&vTemp);
		//				if (fLength < 5.f)
		//				{
		//					vPos = iter;
		//				}

		//			}

		//			if (FAILED(pNavi->Set_PositionVector(vPos)))
		//				return;
		//			m_iNaviMeshCnt++;
		//		}
		//	}
		//	if (m_iNaviMeshCnt > 2)
		//	{
		//		if (FAILED(pNavi->Set_CellVector(eNavi)))
		//			return;
		//		pNavi->Ready_Neighbor();
		//		pNavi->Clear_Position_Vector();
		//		m_iNaviMeshCnt = 0;
		//	}
		//}
		//else
		//{
		//	IsPick = pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking);
		//}
	}
	else if (g_eNaviMode == NAVI_MODE::NAVI_MODE_MODIFY)
	{
		//	for (auto& iter : *CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_CubeWire"))
		//		iter->GetIsNaviPick() = false;


		//	_uint iIdx = 0;
		//	bool reulst_f = false;
		//	if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
		//	{
		//		for (auto& iter : pNavi->GetTotalPos())
		//		{
		//			_vec3 vTemp = {};
		//			_vec3	vIter_Pos = iter.second;
		//			vIter_Pos.y = 0.f;

		//			_vec3	vTemp_Pos = vPos;
		//			vTemp_Pos.y = 0.f;

		//			vTemp = vIter_Pos - vTemp_Pos;
		//			_float fLength = D3DXVec3Length(&vTemp);
		//			if (fLength < 2.f)
		//			{
		//				vPos = iter.second;
		//				iIdx = iter.first;
		//				reulst_f = true;
		//			}

		//		}
		//		if (reulst_f == false)
		//			return;

		//		_uint iCellIdx = iIdx / 3;
		//		list<CGameObject*> temp = *CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_CubeWire");
		//		///////////////////////////////////////

		//			
		//		CGameObject* pGameObject = CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_CubeWire", iIdx);
		//		pGameObject->GetIsNaviPick() = true;

		//		bool reulst = false;
		//		for (int i = 0; i < pNavi->m_vecCell.size(); ++i)
		//		{
		//			for (auto& iter : pNavi->m_vecCell[i]->m_vPoint)
		//			{
		//				_vec3 vTemp = {};
		//				_vec3	vIter_Pos = iter;
		//				vIter_Pos.y = 0.f;

		//				_vec3	vTemp_Pos = vPos;
		//				vTemp_Pos.y = 0.f;

		//				vTemp = vIter_Pos - vTemp_Pos;
		//				_float fLength = D3DXVec3Length(&vTemp);
		//				if (fLength < 2.f)
		//				{
		//					iter = _vec3(0.f, 0.f, 0.f);
		//					reulst = true;
		//					pNavi->GetTotalPos()[iIdx] = _vec3(0.f, 0.f, 0.f);
		//					CTransform* pTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
		//					pTransform->Set_StateInfo(STATE::STATE_POSITION, &_vec3(0.f, 0.f, 0.f));
		//					//break;
		//				}
		//			}
		//			
		//		}
		//		if (reulst == false)
		//			return;
		//		

		//		//_vec3 vPoint[3] = {};
		//		//for (int i = 0; i < 3; ++i)
		//		//{
		//		//	vPoint[i] = pNavi->m_vecCell[iCellIdx]->m_vPoint[i];
		//		//}
		//		//Safe_Release(pNavi->m_vecCell[iCellIdx]);
	 //			//pNavi->m_vecCell.erase(pNavi->m_vecCell.begin() + iCellIdx);

		//		//pNavi->Set_CellVector(vPoint);
		//		pNavi->GetTotalPos()[iIdx]= _vec3(0.f, 0.f, 0.f);
		//		CTransform* pTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
		//		pTransform->Set_StateInfo(STATE::STATE_POSITION,&_vec3(0.f, 0.f, 0.f));
		//		int i = 0;
		//	}



	}
	else if (g_eNaviMode == NAVI_MODE::NAVI_MODE_DELETE)
	{

	}




	Safe_Release(pManagement);
}

void CMapToolView::Select_Mode_Camera(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Transform");
	if (nullptr == pTransform)
		return;
	CBuffer_Terrain* pBuffer = (CBuffer_Terrain*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Buffer");
	if (nullptr == pBuffer)
		return;
	CPicking* pPicking = (CPicking*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Picking");
	if (nullptr == pPicking)
		return;
	_bool IsPick = false;
	_vec3 vPos = {};
	if (IsMouse)
	{
		IsPick = pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking);

		if (IsPick)
		{
			if (FAILED(pManagement->Add_GameObjectToLayer(L"GameObject_Arrow",
				SCENE_STATIC, L"Layer_Arrow", (void*)&vPos)))
			{
				Safe_Release(pManagement);
				return;
			}
			CTransform* pTempTransform = (CTransform*)pManagement->Get_BackObject(SCENE_STATIC,
				L"Layer_Arrow")->Get_ComponentPointer(L"Com_Transform");
			_vec3 vTemp = *pTempTransform->Get_StateInfo(STATE::STATE_POSITION);
			CString strResult;
			CString strX;
			CString strY;
			CString strZ;
			strX.Format(_T("%f"), vTemp.x);
			strY.Format(_T("%f"), vTemp.y);
			strZ.Format(_T("%f"), vTemp.z);

			strResult += strX + L", " + strY + L", " + strZ;
			pSecondWnd->m_pCameraTool->m_ListBox.AddString(strResult);
			CManagement::GetInstance()->GetCameraMatrix().push_back(pTempTransform->Get_Matrix());
		}

	}

	Safe_Release(pManagement);
}

void CMapToolView::Select_Mode_Drag_Unit(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
	if (IsDrag == false)
		return;

	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Transform");
	if (nullptr == pTransform)
	{
		Safe_Release(pManagement);
		return;
	}
	CBuffer_Terrain* pBuffer = (CBuffer_Terrain*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Buffer");
	if (nullptr == pBuffer)
	{
		Safe_Release(pManagement);
		return;
	}
	CPicking* pPicking = (CPicking*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Picking");
	if (nullptr == pPicking)
	{
		Safe_Release(pManagement);
		return;
	}

	_vec3 vPos = {};
	_vec3 vSize = { pSecondWnd->m_pUnitTool->m_fScaleX, pSecondWnd->m_pUnitTool->m_fScaleY,  pSecondWnd->m_pUnitTool->m_fScaleZ };
	_vec3 vRotate = { pSecondWnd->m_pUnitTool->m_fRotX, pSecondWnd->m_pUnitTool->m_fRotY,  pSecondWnd->m_pUnitTool->m_fRotZ };

	if (pSecondWnd->m_pUnitTool->m_Radio_Creative[0].GetCheck())
	{
		if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
		{
			if (pManagement->Get_ObjectList(SCENE_STATIC, L"Layer_Drag") == nullptr)
			{
				if (pSecondWnd->m_pUnitTool->m_eMeshType == MESH_TYPE::STATIC)
				{
					TEMP tTemp = TEMP(pSecondWnd->m_pUnitTool->m_pComponent_Tag, vPos);
					if (FAILED(pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObject_Tag,
						SCENE_STATIC, L"Layer_Drag", (void*)&tTemp)))
					{
						Safe_Release(pManagement);
						return;
					}
				}
				else if (pSecondWnd->m_pUnitTool->m_eMeshType == MESH_TYPE::DYNAMIC)
				{
					TEMP tTemp = TEMP(pSecondWnd->m_pUnitTool->m_pComponent_Tag, vPos);
					if (FAILED(pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObject_Tag,
						SCENE_STATIC, L"Layer_Drag", (void*)&tTemp)))
					{
						Safe_Release(pManagement);
						return;
					}
				}
			}
			else
			{
				if (pManagement->Get_ObjectList(SCENE_STATIC, L"Layer_Drag")->size() == 0)
				{
					TEMP tTemp = TEMP(pSecondWnd->m_pUnitTool->m_pComponent_Tag, vPos);
					if (pSecondWnd->m_pUnitTool->m_eMeshType == MESH_TYPE::STATIC)
					{
						pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObject_Tag, SCENE_STATIC, L"Layer_Drag", (void*)&tTemp);
						CTransform* pTransformDrag = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Drag", L"Com_Transform");
						pTransformDrag->Set_StateInfo(STATE_POSITION, &vPos);
					}
					else if (pSecondWnd->m_pUnitTool->m_eMeshType == MESH_TYPE::DYNAMIC)
					{
						pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObject_Tag, SCENE_STATIC, L"Layer_Drag", (void*)&tTemp);
						CTransform* pTransformDrag = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Drag", L"Com_Transform");
						pTransformDrag->Set_StateInfo(STATE_POSITION, &vPos);
					}
				}
				else
				{
					CTransform* pTransformDrag = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Drag", L"Com_Transform");
					pTransformDrag->Scaling(vSize);
					if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[0].GetCheck())
					{

						pSecondWnd->m_pUnitTool->m_fRotY = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotZ = 0.f;
						pTransformDrag->SetUp_RotationX(D3DXToRadian(vRotate.x));
					}
					else 	if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[1].GetCheck())
					{
						pSecondWnd->m_pUnitTool->m_fRotX = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotZ = 0.f;
						pTransformDrag->SetUp_RotationY(D3DXToRadian(vRotate.y));
					}
					else 	if (pSecondWnd->m_pUnitTool->m_Radio_Rotate[2].GetCheck())
					{
						pSecondWnd->m_pUnitTool->m_fRotY = 0.f;
						pSecondWnd->m_pUnitTool->m_fRotX = 0.f;
						pTransformDrag->SetUp_RotationZ(D3DXToRadian(vRotate.z));
					}
					pTransformDrag->Set_StateInfo(STATE_POSITION, &vPos);
				}
			}
		}

	}
	else if (pSecondWnd->m_pUnitTool->m_Radio_Creative[1].GetCheck())
	{
		pManagement->Find_Clear_Layer(SCENE_STATIC, L"Layer_Drag");
	}

	Safe_Release(pManagement);
}

void CMapToolView::Select_Mode_Drag_Navi(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	if (g_eNaviMode != NAVI_MODE::NAVI_MODE_MODIFY)
		return;

	CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Transform");
	if (nullptr == pTransform)
		return;
	CBuffer_Terrain* pBuffer = (CBuffer_Terrain*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Buffer");
	if (nullptr == pBuffer)
		return;
	CPicking* pPicking = (CPicking*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Picking");
	if (nullptr == pPicking)
		return;

	CNavi* pNavi = (CNavi*)pManagement->Get_ComponentPointer(SCENE_STATIC, L"Layer_Terrain", L"Com_Navi");
	if (nullptr == pNavi)
		return;

	_vec3 vPos = {};
	_bool IsPick = false;

	if (pManagement->KeyUp(KEY_LBUTTON))
	{

		for (auto& iter : *CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_CubeWire"))
			iter->GetIsNaviPick() = false;

	}
	else if (pManagement->KeyPressing(KEY_LBUTTON))
	{
		CGameObject* pGameObject = nullptr;
		for (auto& iter : *CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_CubeWire"))
			if (iter->GetIsNaviPick() == true)
				pGameObject = iter;
		if (nullptr == pGameObject)
			return;

		_uint iIdx = 0;
		bool reulst_f = false;
		if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
		{
		
			CTransform* pPickTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
			if (nullptr == pPickTransform)
				return;
			_vec3 vConfirmPos = {};
			vConfirmPos.x = vPos.x;
			vConfirmPos.z = vPos.z;
			vConfirmPos.y= pPickTransform->Get_StateInfo(STATE::STATE_POSITION)->y;


			pPickTransform->Set_StateInfo(STATE::STATE_POSITION, &vConfirmPos);
			_uint iIdx = pGameObject->m_iGetObjIdx;

			bool reulst = false;
			for (int i = 0; i < pNavi->m_vecCell.size(); ++i)
			{
				for (auto& iter : pNavi->m_vecCell[i]->m_vPoint)
				{
					_vec3 vTemp = {};
					_vec3	vIter_Pos = iter;
					vIter_Pos.y = 0.f;

					_vec3	vTemp_Pos = vConfirmPos;
					vTemp_Pos.y = 0.f;

					vTemp = vIter_Pos - vTemp_Pos;
					_float fLength = D3DXVec3Length(&vTemp);
					if (fLength < 2.f)
					{
						iter = vConfirmPos;
						reulst = true;
						pNavi->GetTotalPos()[iIdx] = vConfirmPos;
						CTransform* pTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
						pTransform->Set_StateInfo(STATE::STATE_POSITION, &vConfirmPos);
						//break;
					}
				}

			}
			if (reulst == false)
				return;

			pNavi->GetTotalPos()[iIdx] = vConfirmPos;
			CTransform* pTransform = (CTransform*)pGameObject->Get_ComponentPointer(L"Com_Transform");
			pTransform->Set_StateInfo(STATE::STATE_POSITION, &vConfirmPos);

		}


	}



	if (pManagement->KeyDown(KEY_RBUTTON))
	{

		for (auto& iter : *CManagement::GetInstance()->Get_ObjectList(SCENEID::SCENE_STATIC, L"Layer_CubeWire"))
			iter->GetIsNaviPick() = false;


		_uint iIdx = 0;
		bool reulst_f = false;
		if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
		{
			for (auto& iter : pNavi->GetTotalPos())
			{
				_vec3 vTemp = {};
				_vec3	vIter_Pos = iter.second;
				vIter_Pos.y = 0.f;

				_vec3	vTemp_Pos = vPos;
				vTemp_Pos.y = 0.f;

				vTemp = vIter_Pos - vTemp_Pos;
				_float fLength = D3DXVec3Length(&vTemp);
				if (fLength < 2.f)
				{
					vPos = iter.second;
					iIdx = iter.first;
					reulst_f = true;
				}

			}
			if (reulst_f == false)
				return;

			CGameObject* pGameObject = CManagement::GetInstance()->Get_GameObject(SCENEID::SCENE_STATIC, L"Layer_CubeWire", iIdx);
			pGameObject->GetIsNaviPick() = true;



		}


	}

	Safe_Release(pManagement);
}
