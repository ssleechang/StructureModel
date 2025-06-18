#include "stdafx.h"
#include "GMLib/MSVersionInfo.h"
#include "MSFWSurfaceFoldingG.h"
#include "MSFWSurfaceFoldingM.h"

IMPLEMENT_SERIAL(MSFWSurfaceFoldingG, MSVerticalLineMemberG, VERSIONABLE_SCHEMA | 1)
MSFWSurfaceFoldingG::MSFWSurfaceFoldingG()
{
	Init(true);
	m_FirstCompoGID = 0;
	m_SecondCompoGID = 0;
	m_BasePnt;
}

MSFWSurfaceFoldingG::~MSFWSurfaceFoldingG()
{

}

void MSFWSurfaceFoldingG::Serialize(CArchive &ar)
{
	MSVerticalLineMemberG::Serialize(ar);
	bool bEachCompoM = false;
	if (ar.IsStoring())
	{
		if (mp_CompoM)
		{
			if (mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if (bEachCompoM == true)
			{
				Serialize_EachCompoM(ar);
			}

			ar << m_FirstCompoGID;
			ar << m_SecondCompoGID;
			m_BasePnt.Serialize(ar);
		}
	}
	else
	{
		ar >> bEachCompoM;
		if (bEachCompoM == true)
		{
			Serialize_EachCompoM(ar);
		}

		ar >> m_FirstCompoGID;
		ar >> m_SecondCompoGID;
		m_BasePnt.Serialize(ar);

		mp_Boundary->MakeCCW();
		GetTrimmedBoundary()->MakeCCW();
	}
}

MSObject* MSFWSurfaceFoldingG::Copy(bool bNewID /*= true*/)
{
	MSFWSurfaceFoldingG* pNewObj;

	pNewObj = new MSFWSurfaceFoldingG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSFWSurfaceFoldingG::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	if (!pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msFWSurfaceFoldingG) { ASSERT(0); return; }

	MSFWSurfaceFoldingG* pSource = (MSFWSurfaceFoldingG*)pSource_in;
	MSVerticalLineMemberG::CopyFromMe(pSource_in, bNewID);

	m_FirstCompoGID = pSource->m_FirstCompoGID;
	m_SecondCompoGID = pSource->m_SecondCompoGID;
	m_BasePnt = pSource->m_BasePnt;
}

void MSFWSurfaceFoldingG::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSFWSurfaceFoldingM* pSurfaceFoldingM = dynamic_cast<MSFWSurfaceFoldingM*>(mp_CompoM);
	if (pSurfaceFoldingM == nullptr)
		return;

	MSCompoG::MirrorByAxis(pAxis);
	GM2DPolyline* pProfile = pSurfaceFoldingM->GetProfile();
	GM2DLineVector AxisLine = pAxis;
	AxisLine.Translate(-AxisLine.m_P0);
	pProfile->MirrorByAxis(AxisLine);
}

bool MSFWSurfaceFoldingG::MakeBoundary()
{
	mp_Boundary->ResetPolyline();
	MSFWSurfaceFoldingM* pSurfaceFoldingM = dynamic_cast<MSFWSurfaceFoldingM*>(mp_CompoM);
	if (pSurfaceFoldingM == nullptr)
		return false;
	
	mp_Boundary->CopyFrom(pSurfaceFoldingM->GetProfile());
	GM2DVector* pSVec = GetSVec();
	mp_Boundary->Translate(*pSVec);
	mp_Boundary->RotateZ(Deg2Rad*m_dAxisRotation, pSVec);

	return true;
}

eSurfaceFoldingMType MSFWSurfaceFoldingG::GetSurfaceFoldingMType()
{
	MSFWSurfaceFoldingM* pSurfaceFoldingM = dynamic_cast<MSFWSurfaceFoldingM*>(mp_CompoM);
	if (pSurfaceFoldingM == nullptr)
		return eSurfaceFoldingMType::PolyLine;

	return pSurfaceFoldingM->GetSurfaceFoldingMType();
}

void MSFWSurfaceFoldingG::SetSurfaceFoldingMType(eSurfaceFoldingMType type)
{
	MSFWSurfaceFoldingM* pSurfaceFoldingM = dynamic_cast<MSFWSurfaceFoldingM*>(mp_CompoM);
	if (pSurfaceFoldingM == nullptr)
		return;
	
	pSurfaceFoldingM->SetSurfaceFoldingMType(type);
}