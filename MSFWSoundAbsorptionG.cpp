#include "stdafx.h"
#include "MSFWSoundAbsorptionG.h"
#include "MSFWSoundAbsorptionM.h"
#include "GMLib/GM2DBound.h"

IMPLEMENT_SERIAL(MSFWSoundAbsorptionG, MSCompoG, VERSIONABLE_SCHEMA | 1)
MSFWSoundAbsorptionG::MSFWSoundAbsorptionG(void)
{
	m_IsTopLocation = false;
}

MSFWSoundAbsorptionG::~MSFWSoundAbsorptionG(void)
{
}

void MSFWSoundAbsorptionG::Dump(EFS::EFS_Buffer& buffer)
{
	MSCompoG::Dump(buffer);
	buffer << m_IsTopLocation;
}

void MSFWSoundAbsorptionG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCompoG::Recovery(buffer);
	buffer >> m_IsTopLocation;
}

void MSFWSoundAbsorptionG::Serialize(CArchive &ar)
{
	MSCompoG::Serialize(ar);
	bool bEachCompoM = false;

	if (ar.IsStoring())
	{
		if (mp_CompoM)
		{
			if (mp_CompoM->m_ID == 0)
				bEachCompoM = true;
			ar << bEachCompoM;
			if (bEachCompoM == true)
				Serialize_EachCompoM(ar);
		}

		ar << m_IsTopLocation;
	}
	else
	{
		ar >> bEachCompoM;
		if (bEachCompoM)
			Serialize_EachCompoM(ar);

		ar >> m_IsTopLocation;
	}
}

MSObject* MSFWSoundAbsorptionG::Copy(bool bNewID)
{
	MSFWSoundAbsorptionG* pCopy = new MSFWSoundAbsorptionG();
	pCopy->CopyFromMe(this, bNewID);
	return pCopy;
}

void MSFWSoundAbsorptionG::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSCompoG::CopyFromMe(pSource_in, bNewID);

	MSFWSoundAbsorptionG* pSource = (MSFWSoundAbsorptionG*)pSource_in;
	mp_Owner = pSource->mp_Owner;
	if (pSource->mp_Source)
		mp_Source = pSource->mp_Source;

	m_IsTopLocation = pSource->GetIsTopLocation();
}

bool MSFWSoundAbsorptionG::MakeBoundary()
{
	return true;
}

void MSFWSoundAbsorptionG::CalcBoundRectBeforeAddQuadTree()
{
	GM2DBound MyBound;
	GetProfile()->Get2DBound(MyBound);
	GM2DVector* pOrigin = MyBound.GetBottomLeft();
	GM2DVector* pCorner = MyBound.GetTopRight();

	if (pOrigin == NULL || pCorner == NULL) return;
	m_RectForQuadTree.m_Origin.m_X = pOrigin->m_X; m_RectForQuadTree.m_Origin.m_Y = pOrigin->m_Y;
	m_RectForQuadTree.m_Corner.m_X = pCorner->m_X; m_RectForQuadTree.m_Corner.m_Y = pCorner->m_Y;

	if (mp_PolylineInfo == NULL)
		return;
}


bool MSFWSoundAbsorptionG::VisibleInRect(GM2DRectangle* pRect)
{
	if (GetProfile() == NULL)
		return false;

	if (m_RectForQuadTree.Right() < pRect->Left()) return false;
	else if (m_RectForQuadTree.Left() > pRect->Right()) return false;
	if (m_RectForQuadTree.Top() < pRect->Bottom()) return false;
	else if (m_RectForQuadTree.Bottom() > pRect->Top()) return false;

	if (pRect->Left() <= m_RectForQuadTree.Left() && m_RectForQuadTree.Right() <= pRect->Right())
	{
		if (pRect->Bottom() <= m_RectForQuadTree.Bottom() && m_RectForQuadTree.Top() <= pRect->Top())
			return true;
	}

	GM2DPolyline UrPoly(TRUE);
	pRect->GetBoundPolyline(UrPoly);

	if (GetProfile()->OverlapEx(UrPoly))
		return true;

	return false;
}

void MSFWSoundAbsorptionG::Translate(GM2DVector &pDelta)
{
	MSCompoG::Translate(pDelta);
	GetProfile()->Translate(pDelta);
}

void MSFWSoundAbsorptionG::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSCompoG::MirrorByAxis(pAxis);
	GetProfile()->MirrorByAxis(pAxis);
}

void MSFWSoundAbsorptionG::RotateZ(double theta, GM2DVector *pCenter)
{
	MSCompoG::RotateZ(theta, pCenter);
	GetProfile()->RotateZ(theta, pCenter);
}

bool MSFWSoundAbsorptionG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	MSCompoG* pMainCompoG = GetMainCompoG();
	
	double mainTopLevel = 0;
	double mainBottomLevel = 0;
	if(pMainCompoG != nullptr)
		pMainCompoG->GetZLevels(mainTopLevel, mainBottomLevel);
	else
	{
		double dLevel = GetNextSlabLevel();
		mainTopLevel = dLevel;
		mainBottomLevel = dLevel;
	}

	if (m_IsTopLocation == true)
	{
		TopLevel = mainTopLevel + GetThick();
		BottomLevel = mainTopLevel;
	}
	else
	{
		TopLevel = mainBottomLevel;
		BottomLevel = mainBottomLevel - GetThick();
	}
	return true;
}