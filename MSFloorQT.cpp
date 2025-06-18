#include "stdafx.h"
#include "MSFloorQT.h"
#include "GMLib\GMObject.h"
#include "GMLib\GM2DGeometry.h"
#include "GMLib\GM2DLineVector.h"
#include "MSCompoG.h"
#include "MSCompoGUtil.h"

double MSFloorQT::sm_BoundMargin = 1000.;
long MSFloorQT::m_MaxDepth = 5;
long MSFloorQT::m_MinSize = 20;

MSFloorQT::MSFloorQT(long MinSize)
{
  m_MinSize = MinSize;
  m_IsLeaf = 0;
  m_IsRoot = 0;
	m_MaxDepth = 5;
  Setup();
}

MSFloorQT::MSFloorQT(long MinSize, GM2DVector Origin, GM2DVector Corner)
{
  m_MinSize = MinSize;
  m_IsLeaf = 0;
	m_IsRoot = 0;
	m_MaxDepth = 5;
  Setup();
  m_Rect.m_Origin.SetXY(Origin.m_X-sm_BoundMargin, Origin.m_Y-sm_BoundMargin);
  m_Rect.m_Corner.SetXY(Corner.m_X+sm_BoundMargin, Corner.m_Y+sm_BoundMargin);
}

MSFloorQT::~MSFloorQT()
{
	for(long iChild=0; iChild<4; iChild++)
		if(m_pChildren[iChild] != NULL)
			delete m_pChildren[iChild];
}

void MSFloorQT::Setup()
{
  m_Rect.m_Origin.SetXY(0, 0);
  m_Rect.m_Corner.SetXY(1, 1);

  m_pChildren[0] = NULL;
  m_pChildren[1] = NULL;
  m_pChildren[2] = NULL;
  m_pChildren[3] = NULL;
}

void MSFloorQT::Build(double dTol)
{
	sm_BoundMargin = dTol;
  FindBoundsRect(dTol);
  BuildLeaves(0, dTol);
}

void MSFloorQT::SetBoundsRect(MSCompoG* pCompoG)
{
	if(pCompoG == NULL)
		return;
	
	long NumVec = pCompoG->ma_Vec.size();
	if(NumVec == 0)
		return;

	double dXMin = INT_MAX;
	double dYMin = INT_MAX;
	double dXMax = INT_MIN;
	double dYMax = INT_MIN;
	
	for(long iVec = 0 ; iVec < NumVec ; iVec++)
	{
		GM2DVector* pVec = pCompoG->ma_Vec[iVec];
		dXMin = min(dXMin, pVec->m_X);
		dYMin = min(dYMin, pVec->m_Y);
		dXMax = max(dXMax, pVec->m_X);
		dYMax = max(dYMax, pVec->m_Y);
	}
	
	m_Rect.m_Origin.m_X = min(m_Rect.m_Origin.m_X, dXMin);
	m_Rect.m_Origin.m_Y = min(m_Rect.m_Origin.m_Y, dYMin);
	m_Rect.m_Corner.m_X = max(m_Rect.m_Corner.m_X, dXMax);
	m_Rect.m_Corner.m_Y = max(m_Rect.m_Corner.m_Y, dYMax);
}

void MSFloorQT::FindBoundsRect(double dTol)
{
	long NumVerMem = ma_VerticalMem.GetSize();
	for(long iVerMem = 0 ; iVerMem < NumVerMem ; iVerMem++)
	{
		MSCompoG* pCompoG = ma_VerticalMem[iVerMem];
		SetBoundsRect(pCompoG);
	}

	long NumFloorMem = ma_FloorMem.GetSize();
	for(long iFloorMem = 0 ; iFloorMem < NumFloorMem ; iFloorMem++)
	{
		MSCompoG* pCompoG = ma_VerticalMem[iFloorMem];
		SetBoundsRect(pCompoG);
	}
}

void MSFloorQT::BuildLeaves(long Zone, double dTol)
{
    ma_Route.Add(Zone);
    long CurDepth = ma_Route.GetSize();

	long NumVerMem = ma_VerticalMem.GetSize();
	long NumFloorMem = ma_FloorMem.GetSize();
	if(NumFloorMem + NumVerMem > m_MinSize && CurDepth < m_MaxDepth)
		CreateChildren();
	else
	{
		m_IsLeaf = 1;
		long nObj = ma_FloorMem.GetSize();
		for(long iObj=0; iObj<nObj; iObj++)
			ma_FloorMem[iObj]->AddFloorQT(this);

		nObj = ma_VerticalMem.GetSize();
		for(long iObj=0; iObj<nObj; iObj++)
			ma_VerticalMem[iObj]->AddFloorQT(this);
	}
}

void MSFloorQT::CreateChildren()
{

  //     A -- E -- B
  //     | 3  |  2 |
  //     F -- G -- H
  //     | 0  |  1 |
  //     D -- I -- C

    GM2DVector a(m_Rect.m_Origin.m_X+sm_BoundMargin, m_Rect.m_Corner.m_Y-sm_BoundMargin);
    GM2DVector b(m_Rect.m_Corner.m_X-sm_BoundMargin, m_Rect.m_Corner.m_Y-sm_BoundMargin);
    GM2DVector c(m_Rect.m_Corner.m_X-sm_BoundMargin, m_Rect.m_Origin.m_Y+sm_BoundMargin);
    GM2DVector d(m_Rect.m_Origin.m_X+sm_BoundMargin, m_Rect.m_Origin.m_Y+sm_BoundMargin);

	double abLen = a.Distance(b);
	double adLen = a.Distance(d);
	if(abLen < sm_BoundMargin * 2 || adLen < sm_BoundMargin * 2) 
	{
// 		CString strMsg = _T("더이상 쿼드 트리를 확장 할수 없습니다.");
// 		CString strGroup = _T("쿼드 트리");
// 		XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
// 		XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(0, MESSAGE_LEVEL_WARNING, strMsg);
// 		pMsgObject->SetGroupName(strGroup);
// 		pMessageContainer->AddErrorObject(pMsgObject);
		m_IsLeaf = 1;
		return;
	}

    GM2DVector g;
    m_Rect.GetCenter(g);

    GM2DVector e(g.m_X, a.m_Y);
    GM2DVector f(a.m_X, g.m_Y);
    GM2DVector h(b.m_X, g.m_Y);
    GM2DVector i(g.m_X, d.m_Y);

    m_pChildren[0] = new MSFloorQT(m_MinSize, d, g);
    m_pChildren[1] = new MSFloorQT(m_MinSize, i, h);
    m_pChildren[2] = new MSFloorQT(m_MinSize, g, b);
    m_pChildren[3] = new MSFloorQT(m_MinSize, f, e);

    m_IsLeaf = 0;

    for(long ChildNum=0; ChildNum<4; ChildNum++)
    {
        m_pChildren[ChildNum]->m_IsRoot = 0;

        MSFloorQT* ATree = m_pChildren[ChildNum];
        long NumDepth = ma_Route.GetSize();
        for(long Depth=0; Depth<NumDepth; Depth++)
            ATree->ma_Route.Add(ma_Route[Depth]);

        long nObj = ma_FloorMem.GetSize();
        for(long iObj=0; iObj<nObj; iObj++)
        {
            MSCompoG* pFloorMem = ma_FloorMem[iObj];
            if(pFloorMem->VisibleInRect(&ATree->m_Rect))
                pFloorMem->AddFloorQT(ATree);
        }

        nObj = ma_VerticalMem.GetSize();
        for(long iObj=0; iObj<nObj; iObj++)
        {
            MSCompoG* pCompoG = ma_VerticalMem[iObj];
			if(pCompoG->VisibleInRect(&ATree->m_Rect))
				pCompoG->AddFloorQT(ATree);
        }

        ATree->BuildLeaves(ChildNum, sm_BoundMargin);
    }
    if(m_IsRoot == 0)
    {
        ma_FloorMem.RemoveAll();
        ma_VerticalMem.RemoveAll();
    }

}

long MSFloorQT::GetRoute()
{
	long Result=0;
	long NumNode=ma_Route.GetSize();
	for(long Depth=0; Depth<NumNode; Depth++)
		Result = Result*10 + ma_Route[Depth] + 1;
	return Result;
}

void MSFloorQT::AddFloorMem(MSCompoG* pLinkMem)
{
	long NumMem = ma_FloorMem.GetSize();
	for(long iMem = 0 ; iMem < NumMem ; iMem++)
	{
		MSCompoG* pCurCompoG = ma_FloorMem[iMem];
		if(pCurCompoG == pLinkMem)
			return;
	}
	ma_FloorMem.Add(pLinkMem);
}

void MSFloorQT::AddVerticalMem(MSCompoG* pCompoG)
{
	long NumMem = ma_VerticalMem.GetSize();
	for(long iMem = 0 ; iMem < NumMem ; iMem++)
	{
		MSCompoG* pCurCompoG = ma_VerticalMem[iMem];
		if(pCurCompoG == pCompoG)
			return;
	}
	ma_VerticalMem.Add(pCompoG);
}

void MSFloorQT::AddToLeaves( MSCompoG* pNewCompoG,bool  bCreateChildrenWhenNeeded, bool bResetFirst )
{
	//MSElement::Type_MSCompoG eType = pNewCompoG->GetType();
	//if (eType == MSElement::msSlabG || eType == MSElement::msOpeningG)
	//{
		pNewCompoG->CalcBoundRectBeforeAddQuadTree(); // mylee 0324
	//}

	FindCrossQTs(pNewCompoG, bResetFirst);

	long nCrossQT = pNewCompoG->ma_CrossQT.GetSize();
	for(long iCrossQT=0; iCrossQT<nCrossQT; iCrossQT++)
	{
		long NumVerMem = pNewCompoG->ma_CrossQT[iCrossQT]->ma_VerticalMem.GetSize();
		long NumHorMem = pNewCompoG->ma_CrossQT[iCrossQT]->ma_FloorMem.GetSize();
 		if(NumHorMem + NumVerMem > m_MinSize && ma_Route.GetSize()<m_MaxDepth && bCreateChildrenWhenNeeded)
 			pNewCompoG->ma_CrossQT[iCrossQT]->CreateChildren();
	}
}
//
//void MSFloorQT::FindCrossQTs( GM2DLineVector& aLVec, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst )
//{
//	if(bResetFirst)
//		CrossQTArr.RemoveAll();
//	if(m_IsLeaf)
//	{
//		if(!bResetFirst)
//		{
//			long nQT = CrossQTArr.GetSize();
//			for(long iQT=0; iQT<nQT; iQT++)
//				if(CrossQTArr[iQT]==this)
//					return;
//		}
//		if(aLVec.VisibleInRect(m_Rect))
//			CrossQTArr.Add(this);
//		else
//			return;
//	}
//
//	for(long ChildNum=0; ChildNum<4; ChildNum++)
//	{
//		if(m_pChildren[ChildNum] == NULL) 
//			continue;
//		m_pChildren[ChildNum]->FindCrossQTs(aLVec, CrossQTArr, false);
//	}
//}



void MSFloorQT::FindCrossQTs(GM2DLineVector& aLVec, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst)
{
	if (bResetFirst)
		CrossQTArr.RemoveAll();

	//VisibleInRect검사를 먼저해서 걸리지 않는 경우 밑으로 내려가지 않도록 한다.
	if (!aLVec.VisibleInRect(m_Rect))
		return;

	if (m_IsLeaf)
	{
		if (!bResetFirst)
		{
			long nQT = CrossQTArr.GetSize();
			for (long iQT = 0; iQT < nQT; iQT++)
				if (CrossQTArr[iQT] == this)
					return;
		}

		CrossQTArr.Add(this);

	}

	for (long ChildNum = 0; ChildNum < 4; ChildNum++)
	{
		if (m_pChildren[ChildNum] == NULL)
			continue;
		m_pChildren[ChildNum]->FindCrossQTs(aLVec, CrossQTArr, false);
	}
}

void MSFloorQT::FindCrossQTs( GM2DRectangle* pRect, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst )
{
	if(bResetFirst)
		CrossQTArr.RemoveAll();
	if(m_IsLeaf)
	{
		if(!bResetFirst)
		{
			long nQT = CrossQTArr.GetSize();
			for(long iQT=0; iQT<nQT; iQT++)
				if(CrossQTArr[iQT]==this)
					return;
		}
		if(pRect->VisibleInRect(&m_Rect))
			CrossQTArr.Add(this);
		else
			return;
	}

	for(long ChildNum=0; ChildNum<4; ChildNum++)
	{
		if(m_pChildren[ChildNum] == NULL) 
			continue;
		m_pChildren[ChildNum]->FindCrossQTs(pRect, CrossQTArr, false);
	}
}

void MSFloorQT::FindCrossQTs( GM2DCurve* pCurve, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst )
{
    if(bResetFirst)
        CrossQTArr.RemoveAll();
    if(m_IsLeaf)
    {
        if(!bResetFirst)
        {
            long nQT = CrossQTArr.GetSize();
            for(long iQT=0; iQT<nQT; iQT++)
                if(CrossQTArr[iQT]==this)
                    return;
        }
        if(pCurve->VisibleInRect(m_Rect))
            CrossQTArr.Add(this);
        else
			return;
    }

    for(long ChildNum=0; ChildNum<4; ChildNum++)
    {
		if(m_pChildren[ChildNum] == NULL) 
			continue;
		m_pChildren[ChildNum]->FindCrossQTs(pCurve, CrossQTArr, false);
	}
}

void MSFloorQT::FindCrossQTs( MSElement* pElement, bool bResetFirst )
{
	if(bResetFirst)
		pElement->ma_CrossQT.RemoveAll();
	if(m_IsLeaf)
	{
		if(!bResetFirst)
		{
			long nQT = pElement->ma_CrossQT.GetSize();
			for(long iQT=0; iQT<nQT; iQT++)
				if(pElement->ma_CrossQT[iQT]==this)
					return;
		}
		if(pElement->VisibleInRect(&m_Rect))
			pElement->AddFloorQT(this);
		else
			return;
	}

	for(long ChildNum=0; ChildNum<4; ChildNum++)
	{
		if(m_pChildren[ChildNum] == NULL) 
			continue;
		m_pChildren[ChildNum]->FindCrossQTs(pElement, false);
	}
}

void MSFloorQT::FindCrossQTs(MSElement* pElement, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst)
{
	if (pElement == NULL)
		return;
	if (bResetFirst)
		CrossQTArr.RemoveAll();
	if (m_IsLeaf)
	{
		if (!bResetFirst)
		{
			long nQT = CrossQTArr.GetSize();
			for (long iQT = 0; iQT < nQT; iQT++)
				if (CrossQTArr[iQT] == this)
					return;
		}
		if (pElement->VisibleInRect(&m_Rect))
			CrossQTArr.Add(this);
		else
			return;
	}

	for (long ChildNum = 0; ChildNum < 4; ChildNum++)
	{
		if (m_pChildren[ChildNum] == NULL)
			continue;
		m_pChildren[ChildNum]->FindCrossQTs(pElement, CrossQTArr, false);
	}
}

void MSFloorQT::FindCrossQTs( GM2DVector* pVec, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst )
{
	if(pVec == NULL)
		return;
	if(bResetFirst)
		CrossQTArr.RemoveAll();
	if(m_IsLeaf)
	{
		if(!bResetFirst)
		{
			long nQT = CrossQTArr.GetSize();
			for(long iQT=0; iQT<nQT; iQT++)
				if(CrossQTArr[iQT]==this)
					return;
		}
		if(pVec->VisibleInRect(m_Rect))
			CrossQTArr.Add(this);
		else
			return;
	}

	for(long ChildNum=0; ChildNum<4; ChildNum++)
	{
		if(m_pChildren[ChildNum] == NULL) 
			continue;
		m_pChildren[ChildNum]->FindCrossQTs(pVec, CrossQTArr, false);
	}
}

void MSFloorQT::FindCrossQTs(GMObject* pGeometry, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bFindOnlyOnLine, bool bResetFirst)
{
	GM2DVector* pVec = dynamic_cast<GM2DVector*>(pGeometry);
	GM2DCurve* pCurve = dynamic_cast<GM2DCurve*>(pGeometry);
	GM2DPolyline* pPoly = dynamic_cast<GM2DPolyline*>(pGeometry);
	GM2DRectangle* pRect = dynamic_cast<GM2DRectangle*>(pGeometry);

	if (pVec != NULL)
		FindCrossQTs(pVec, CrossQTArr, false);
	else if (pCurve != NULL)
		FindCrossQTs(pCurve, CrossQTArr, false);
	else if(pRect != NULL)
		FindCrossQTs(pRect, CrossQTArr, false);
	else if (pPoly != NULL)
	{
		if (bFindOnlyOnLine == true)
		{
			long NumCurve = pPoly->GetNumLines();
			for (long iCurve = 0; iCurve < NumCurve; iCurve++)
			{
				GM2DCurve* pCurve = pPoly->GetCurve(iCurve);
				FindCrossQTs(pCurve, CrossQTArr, false);
				delete pCurve;
			}
		}
		else
		{
			if (pPoly->ma_Point.GetSize() > 0)
			{
				GM2DRectangle& Rect = pPoly->GetRectangle();
				
				FindCrossQTs(&Rect, CrossQTArr, false);
			}
		}
	}
}

void MSFloorQT::Remove( MSCompoG* pCompoG )
{
    long nCompoG;
    if(pCompoG->IsVerticalMember())
    {
        nCompoG = ma_VerticalMem.GetSize();

        for(long iCompoG=0; iCompoG<nCompoG; iCompoG++)
        {
			MSCompoG* pVertG = ma_VerticalMem[iCompoG];
            if(pCompoG==ma_VerticalMem[iCompoG])
            {
                ma_VerticalMem.RemoveAt(iCompoG);
                break;
            }
        }
    }
    else
    {
        nCompoG = ma_FloorMem.GetSize();
        for(long iCompoG=0; iCompoG<nCompoG; iCompoG++)
        {
            if(pCompoG==ma_FloorMem[iCompoG])
            {
                ma_FloorMem.RemoveAt(iCompoG);
                break;
            }
        }
    }

    if(m_IsRoot)
    {
        long nCrossLeaf = pCompoG->ma_CrossQT.GetSize();
        for(long iCrossLeaf=0; iCrossLeaf<nCrossLeaf; iCrossLeaf++)
        {
            MSFloorQT* pLeaf = pCompoG->ma_CrossQT[iCrossLeaf];
            pLeaf->Remove(pCompoG);
        }
    }
	nCompoG = ma_VerticalMem.GetSize();
}

vector<MSCompoG*> MSFloorQT::FindCompoG(GMObject* pGeometry, FindType_MSElement memberType, bool bFindOnlyOnLine, bool bCheckOverlap, double dTol, bool bIncludeOnLine)
{
	CTypedPtrArray<CObArray, MSFloorQT*> CrossQTArr;
	FindCrossQTs(pGeometry, CrossQTArr, bFindOnlyOnLine, false);
	
	return MSCompoGUtil::FindCompoGInCrossQTArr(pGeometry, CrossQTArr, memberType, bFindOnlyOnLine, bCheckOverlap, dTol, bIncludeOnLine);
}