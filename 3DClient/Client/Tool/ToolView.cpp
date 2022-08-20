
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"

#include "MainFrm.h"
#include "Management.h"
#include "Terrain.h"
#include "SecondWnd.h"
#include "UnitTool.h"

#include "GameObject.h"
#include "Layer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ��������

// CToolView


IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG



void CToolView::OnInitialUpdate()
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


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CSecondWnd* pSecondWnd = dynamic_cast<CSecondWnd*>(pMainFrm->m_MainSplt.GetPane(0, 1));

	switch (eMode)
	{
	case MODE_UNIT:
		Mode_Unit_Drag(pMainFrm, pSecondWnd);
		break;
	case MODE_EFFECT:
		Mode_Effect_Drag(pMainFrm, pSecondWnd);
		break;
	case MODE_UI:
		Mode_UI_Drag(pMainFrm, pSecondWnd);
		break;
	case MODE_NAVIMESH:
		Mode_NaviMesh_Drag(pMainFrm, pSecondWnd);
		break;
	}
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CSecondWnd* pSecondWnd = dynamic_cast<CSecondWnd*>(pMainFrm->m_MainSplt.GetPane(0, 1));

	switch (eMode)
	{
	case MODE_UNIT:
		Mode_Unit(pMainFrm, pSecondWnd);
		break;
	case MODE_EFFECT:
		Mode_Effect(pMainFrm, pSecondWnd);
		break;
	case MODE_UI:
		Mode_UI(pMainFrm, pSecondWnd);
		break;
	case MODE_NAVIMESH:
		Mode_NaviMesh(pMainFrm, pSecondWnd);
		break;
	}
}

void CToolView::Mode_Unit(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Transform");
	if (nullptr == pTransform)
		return;
	CBuffer_Terrain* pBuffer = (CBuffer_Terrain*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Buffer");
	if (nullptr == pBuffer)
		return;
	CPicking*	pPicking = (CPicking*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Picking");
	if (nullptr == pPicking)
		return;

	_vec3 vPos;
	_bool IsPick = false;

	if (IsMouseCheck)
	{
		if (IsEditMode) // ũ������Ʈ ����� ���
		{
			if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
			{
				m_IsCheckListBox = false;
				if (pSecondWnd->m_pUnitTool->m_IsStatic)
				{
					if (FAILED(pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObjectTag,
						SCENE_LOGO, pSecondWnd->m_pUnitTool->m_pLayerTag, (void*)&vPos)))
						return;
				}
				else
				{
					if (FAILED(pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pDynamicObjectTag,
						SCENE_LOGO, pSecondWnd->m_pUnitTool->m_pDynamicLayerTag, (void*)&vPos)))
						return;
				}
		
			}
		}
		else // Picking ����� ���
		{			
			if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
			{
				_float fLength = 0.f;
				// map�� ��ȸ�ؼ� ���ο� �ִ� ��� ��ü���� IsPick ������ false�� ����
				// �� temp ������ ����ؼ� ������ �����ϰ� ���� ����
				map<const _tchar*, CLayer*>	mapTemp = *pManagement->Get_MapLayer(SCENE_LOGO);
				for (auto& iter : mapTemp)
				{
					auto& iter1 = iter.second->GetGameObjectList()->begin();
					for (; iter1 != iter.second->GetGameObjectList()->end(); ++iter1)
					{
						CTransform* pTransform = (CTransform*)(*iter1)->Get_ComponentPointer(L"Com_Transform");
						_vec3 vTempPos = *pTransform->Get_StateInfo(STATE_POSITION);

						_vec3 vTemp = vTempPos - vPos;
						fLength = D3DXVec3Length(&vTemp);

						if (fLength < 3.f)
						{
							//��� ��ü���� ���� IsPick������ ���� false�� �ٲ��ش�.
							map<const _tchar*, CLayer*>	mapTemp1 = *pManagement->Get_MapLayer(SCENE_LOGO);
							for (auto& iter_in : mapTemp1)
							{
								auto& iter_Object = iter_in.second->GetGameObjectList()->begin();
								for (; iter_Object != iter_in.second->GetGameObjectList()->end(); ++iter_Object)
									(*iter_Object)->GetIsPick() = false;
							}

							//�ش� ��ü�� IsPick ������ true
							(*iter1)->GetIsPick() = true;
							break;
						}
					}
				}				
			}
		}
	}
	else
		IsPick = pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking);

	if (!m_IsCheckListBox)
	{
		//�巡�� �� �� ȭ�鿡 �����Ǵ� ��ü���� �� ���� ��Ų��.
		pManagement->Find_Clear_Layer(SCENE_LOGO, L"Layer_Drag");
	}
	Safe_Release(pManagement);
}

void CToolView::Mode_Effect(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
}

void CToolView::Mode_UI(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();


	Safe_Release(pManagement);
}

void CToolView::Mode_NaviMesh(CMainFrame* pMainFrm, CSecondWnd* pSecondWnd)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Transform");
	if (nullptr == pTransform)
		return;
	CBuffer_Terrain* pBuffer = (CBuffer_Terrain*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Buffer");
	if (nullptr == pBuffer)
		return;
	CPicking*	pPicking = (CPicking*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Picking");
	if (nullptr == pPicking)
		return;

	CNavi*	pNavi = (CNavi*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Navi");
	if (nullptr == pNavi)
		return;

	_vec3 vPos;
	_bool IsPick = false;


	if (IsMouseCheck)
	{
		IsPick = pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking);

		if (m_iNaviMeshCnt < 3)
		{
			if (IsPick)
			{
				vPos.y = 2.f;
				for (auto& iter : pNavi->GetTotalPos())
				{
					_vec3 vTemp = {};
					vTemp = iter - vPos;
					_float fLength=D3DXVec3Length(&vTemp);
					if (fLength < 2.f)
						vPos = iter;
				}

				if (FAILED(pNavi->Set_PositionVector(vPos)))
					return;
				m_iNaviMeshCnt++;
			}
		}
		if (m_iNaviMeshCnt > 2)
		{
			if (FAILED(pNavi->Set_CellVector()))
				return;
			pNavi->Ready_Neighbor();
			pNavi->Clear_Position_Vector();
			m_iNaviMeshCnt = 0;
		}
	}
	else
	{
		IsPick = pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking);
	}


	Safe_Release(pManagement);
}

void CToolView::Mode_Unit_Drag(CMainFrame * pMainFrm, CSecondWnd * pSecondWnd)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;
	pManagement->AddRef();

	CTransform* pTransform = (CTransform*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Transform");
	if (nullptr == pTransform)
		return;
	CBuffer_Terrain* pBuffer = (CBuffer_Terrain*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Buffer");
	if (nullptr == pBuffer)
		return;
	CPicking*	pPicking = (CPicking*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Terrain", L"Com_Picking");
	if (nullptr == pPicking)
		return;
	
	_vec3 vPos = {};

	if (pBuffer->Picking_ToBuffer(&vPos, pTransform, pPicking))
	{
		if (m_IsCheckListBox)
		{
			if (pManagement->Get_ObjectList(SCENE_LOGO, L"Layer_Drag") == nullptr)
			{
				//m_IsCheckListBox = false;
				if (pSecondWnd->m_pUnitTool->m_IsStatic)
				{
					if (FAILED(pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObjectTag,
						SCENE_LOGO, L"Layer_Drag", (void*)&vPos)))
						return;
				}
				else
				{
					if (FAILED(pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pDynamicObjectTag,
						SCENE_LOGO, L"Layer_Drag", (void*)&vPos)))
						return;
				}

			}
			else
			{
				if (pManagement->Get_ObjectList(SCENE_LOGO, L"Layer_Drag")->size() == 0)
				{
					if (pSecondWnd->m_pUnitTool->m_IsStatic)
					{
						pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pGameObjectTag, SCENE_LOGO, L"Layer_Drag", (void*)&vPos);
						CTransform* pTransformDrag = (CTransform*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Drag", L"Com_Transform");
						pTransformDrag->Set_StateInfo(STATE_POSITION, &vPos);
					}
					else
					{
						pManagement->Add_GameObjectToLayer(pSecondWnd->m_pUnitTool->m_pDynamicObjectTag, SCENE_LOGO, L"Layer_Drag", (void*)&vPos);
						CTransform* pTransformDrag = (CTransform*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Drag", L"Com_Transform");
						pTransformDrag->Set_StateInfo(STATE_POSITION, &vPos);
					}
				
				}
				else
				{
					CTransform* pTransformDrag = (CTransform*)pManagement->Get_ComponentPointer(SCENE_LOGO, L"Layer_Drag", L"Com_Transform");
					pTransformDrag->Set_StateInfo(STATE_POSITION, &vPos);
				}
			}
		}
	}
	
	

	Safe_Release(pManagement);
}

void CToolView::Mode_Effect_Drag(CMainFrame * pMainFrm, CSecondWnd * pSecondWnd)
{
}

void CToolView::Mode_UI_Drag(CMainFrame * pMainFrm, CSecondWnd * pSecondWnd)
{
}

void CToolView::Mode_NaviMesh_Drag(CMainFrame * pMainFrm, CSecondWnd * pSecondWnd)
{
}
