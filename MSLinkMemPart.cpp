#include "StdAfx.h"
#include "MSLinkMemPart.h"

#include "MSConnectivity.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GM2DArc.h"

#include "MSLinkMemberG.h"
#include "MSWallMemberG.h"
#include "MSConnLevelZone.h"
#include "MSFloor.h"

map<MSLinkMemPart*, bool> MSLinkMemPart::_deleted_map;
MSLinkMemPart::MSLinkMemPart(MSLinkMemberG* pLineMember)
{
	Init(true);
	if(pLineMember)
	{
		mp_SConn = NULL;
		mp_EConn = NULL;
		mp_LinkMem = pLineMember;
	}
}

MSLinkMemPart::~MSLinkMemPart(void)
{
	if(mp_RLine) delete mp_RLine;
	if(mp_LLine) delete mp_LLine;

//     if(mp_InitRLine) delete mp_InitRLine;
//     if(mp_InitLLine) delete mp_InitLLine;
// 
//     if(mp_LSExtLine) delete mp_LSExtLine;
//     if(mp_LEExtLine) delete mp_LEExtLine;
//     if(mp_RSExtLine) delete mp_RSExtLine;
//     if(mp_REExtLine) delete mp_REExtLine;

    if(mp_SEndLine) delete mp_SEndLine;
    if(mp_EEndLine) delete mp_EEndLine;
	if(mp_LPolyLine != NULL) delete mp_LPolyLine;
	if(mp_RPolyLine != NULL) delete mp_RPolyLine;
    if(mp_SPolyLine != NULL) delete mp_SPolyLine;
    if(mp_EPolyLine != NULL) delete mp_EPolyLine;
	if(mp_Boundary != NULL) delete mp_Boundary;
    if(mp_TrimmedBoundary!= NULL) delete mp_TrimmedBoundary;
    DeleteTrimLines();
}

void MSLinkMemPart::Serialize_AsID( CArchive& ar, MSLinkMemPart*& part, long& rIdx )
{
	if(ar.IsStoring())
	{
		long ID = 0;
		long idx = 0;
		if (part != NULL && part->mp_LinkMem != NULL)
		{
			ID = part->mp_LinkMem->m_ID;
// 			for (int i=0 ; i<part->mp_LinkMem->ma_MemPart.size() ; ++i)
// 			{
// 				if (part->mp_LinkMem->ma_MemPart[i] == part) { idx = i; break; }
// 			}
		}
		ar << ID << idx;
	}
	else
	{
		long ID = 0;
		long idx = 0;
		ar >> ID;
		ar >> idx;
		part = (MSLinkMemPart*)ID; // NEED TO CONVERT ID->POINTER
		rIdx = idx;
	}
}

void MSLinkMemPart::Serialize( CArchive& ar )
{
	MSObject::Serialize(ar);

	if(ar.IsStoring())
	{
		long ID = (mp_SConn == NULL ? 0 : mp_SConn->m_ID);
		ar << ID;
		ID = (mp_EConn == NULL ? 0 : mp_EConn->m_ID);
		ar << ID;

		GM2DCurve::SerializeWithNullFlag(ar, mp_CenLine);
		ar << m_CenLineSetCnt;
		ar << m_IsParallelCenLine;

		GM2DCurve::SerializeWithNullFlag(ar, mp_LLine);
		GM2DCurve::SerializeWithNullFlag(ar, mp_RLine);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_LPolyLine);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_RPolyLine);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_SPolyLine);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_EPolyLine);

		int count = ma_StartJointLevel.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i) ma_StartJointLevel[i]->Serialize(ar);

		count = ma_StartJointEndPoly.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i) GM2DPolyline::SerializeWithNullFlag(ar, ma_StartJointEndPoly[i]);

		count = ma_EndJointLevel.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i) ma_EndJointLevel[i]->Serialize(ar);

		count = ma_EndJointEndPoly.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i) GM2DPolyline::SerializeWithNullFlag(ar, ma_EndJointEndPoly[i]);

		GM2DLineVector::SerializeWithNullFlag(ar, mp_SEndLine);
		GM2DLineVector::SerializeWithNullFlag(ar, mp_EEndLine);

		count = ma_LSegments.size();
		ar << count;
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_LSegments[i]);

		count = ma_RSegments.size();
		ar << count;
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_RSegments[i]);

		count = ma_SSegments.size();
		ar << count;
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_SSegments[i]);

		count = ma_ESegments.size();
		ar << count;
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_ESegments[i]);

		count = ma_LTrimLine.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_LTrimLine[i]);

		count = ma_RTrimLine.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_RTrimLine[i]);

		ID = (mp_LinkMem == NULL ? 0 : mp_LinkMem->m_ID);
		ar << ID;
		long idx_dummy = 0;
		MSLinkMemPart::Serialize_AsID(ar, mp_FloorPartUp, idx_dummy);
		MSLinkMemPart::Serialize_AsID(ar, mp_WallPartBelow, idx_dummy);

		ID = (mp_LSlab == NULL ? 0 : mp_LSlab->m_ID);
		ar << ID;
		ID = (mp_RSlab == NULL ? 0 : mp_RSlab->m_ID);
		ar << ID;

		GM2DPolyline::SerializeWithNullFlag(ar, mp_Boundary);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_TrimmedBoundary);
	}
	else
	{
		long ID = 0;
		ar >> ID;
		mp_SConn = (MSConnectivity*)ID; // NEED TO CONVERT ID->POINTER
		ar >> ID;
		mp_EConn = (MSConnectivity*)ID; // NEED TO CONVERT ID->POINTER

		GM2DCurve::SerializeWithNullFlag(ar, mp_CenLine);
		ar >> m_CenLineSetCnt;
		ar >> m_IsParallelCenLine;

		if(mp_LPolyLine != NULL) { delete mp_LPolyLine; mp_LPolyLine = NULL; }
		if(mp_RPolyLine != NULL) { delete mp_RPolyLine; mp_RPolyLine = NULL; }
		if(mp_SPolyLine != NULL) { delete mp_SPolyLine; mp_SPolyLine = NULL; }
		if(mp_EPolyLine != NULL) { delete mp_EPolyLine; mp_EPolyLine = NULL; }
		GM2DCurve::SerializeWithNullFlag(ar, mp_LLine);
		GM2DCurve::SerializeWithNullFlag(ar, mp_RLine);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_LPolyLine);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_RPolyLine);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_SPolyLine);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_EPolyLine);

		int count = 0;
		ar >> count;
		ma_StartJointLevel.RemoveAll();
		for (int i=0 ; i<count ; ++i) ma_StartJointLevel.Add(new MSConnLevelZone(NULL, 0, 0));
		for (int i=0 ; i<count ; ++i) ma_StartJointLevel[i]->Serialize(ar);

		ar >> count;
		ma_StartJointEndPoly.RemoveAll();
		for (int i=0 ; i<count ; ++i) ma_StartJointEndPoly.Add(NULL);
		for (int i=0 ; i<count ; ++i) GM2DPolyline::SerializeWithNullFlag(ar, ma_StartJointEndPoly[i]);

		ar >> count;
		ma_EndJointLevel.RemoveAll();
		for (int i=0 ; i<count ; ++i) ma_EndJointLevel.Add(new MSConnLevelZone(NULL, 0, 0));
		for (int i=0 ; i<count ; ++i) ma_EndJointLevel[i]->Serialize(ar);

		ar >> count;
		ma_EndJointEndPoly.RemoveAll();
		for (int i=0 ; i<count ; ++i) ma_EndJointEndPoly.Add(NULL);
		for (int i=0 ; i<count ; ++i) GM2DPolyline::SerializeWithNullFlag(ar, ma_EndJointEndPoly[i]);

		GM2DLineVector::SerializeWithNullFlag(ar, mp_SEndLine);
		GM2DLineVector::SerializeWithNullFlag(ar, mp_EEndLine);

		ar >> count;
		ma_LSegments.clear();
		for (int i=0 ; i<count ; ++i) ma_LSegments.push_back(NULL);
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_LSegments[i]);

		ar >> count;
		ma_RSegments.clear();
		for (int i=0 ; i<count ; ++i) ma_RSegments.push_back(NULL);
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_RSegments[i]);

		ar >> count;
		ma_SSegments.clear();
		for (int i=0 ; i<count ; ++i) ma_SSegments.push_back(NULL);
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_SSegments[i]);

		ar >> count;
		ma_ESegments.clear();
		for (int i=0 ; i<count ; ++i) ma_ESegments.push_back(NULL);
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_ESegments[i]);

		ar >> count;
		ma_LTrimLine.RemoveAll();
		for (int i=0 ; i<count ; ++i) ma_LTrimLine.Add(NULL);
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_LTrimLine[i]);

		ar >> count;
		ma_RTrimLine.RemoveAll();
		for (int i=0 ; i<count ; ++i) ma_RTrimLine.Add(NULL);
		for (int i=0 ; i<count ; ++i) GM2DCurve::SerializeWithNullFlag(ar, ma_RTrimLine[i]);

		ar >> ID;
		mp_LinkMem = (MSLinkMemberG*)ID; // NEED TO CONVERT ID->POINTER

		MSLinkMemPart::Serialize_AsID(ar, mp_FloorPartUp, mp_FloorPartUp_Idx); // NEED TO CONVERT ID->POINTER
		MSLinkMemPart::Serialize_AsID(ar, mp_WallPartBelow, mp_WallPartBelow_Idx); // NEED TO CONVERT ID->POINTER
		ar >> ID;
		mp_LSlab = (MSSlabG*)ID; // NEED TO CONVERT ID->POINTER
		ar >> ID;
		mp_RSlab = (MSSlabG*)ID; // NEED TO CONVERT ID->POINTER

		if(mp_Boundary != NULL) { delete mp_Boundary; mp_Boundary = NULL; }
		if(mp_TrimmedBoundary != NULL) { delete mp_TrimmedBoundary; mp_TrimmedBoundary = NULL; }
		GM2DPolyline::SerializeWithNullFlag(ar, mp_Boundary);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_TrimmedBoundary);

		if (!MSConnectivity::JOINT_SERIALIZE())
		{
			mp_SConn = NULL;
			mp_EConn = NULL;

			mp_LinkMem = NULL;
			mp_FloorPartUp = NULL;
			mp_WallPartBelow = NULL;
			mp_LSlab = NULL;
			mp_RSlab = NULL;
		}


	}
	
}

void MSLinkMemPart::RecoverPointerAfterSerialize(MSFloor* floor)
{
	long ID = 0;
	ID = (long)mp_SConn;
	mp_SConn = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);
	ID = (long)mp_EConn;
	mp_EConn = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);

	for (int i=0 ; i<ma_StartJointLevel.GetSize() ; ++i)
		ma_StartJointLevel[i]->RecoverPointerAfterSerialize(floor);
	for (int i=0 ; i<ma_EndJointLevel.GetSize() ; ++i)
		ma_EndJointLevel[i]->RecoverPointerAfterSerialize(floor);

	ID = (long)mp_LinkMem;
	mp_LinkMem = (MSLinkMemberG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);

	int idx = 0;
	ID = (long)mp_FloorPartUp; idx = mp_FloorPartUp_Idx;
	mp_FloorPartUp = NULL;
	if (floor->mm_CompoG.count(ID) > 0)
	{
		MSCompoG* compoG = (floor->mm_CompoG[ID]);
		MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(compoG);
// 		if (link != NULL && idx < link->ma_MemPart.size())
// 			mp_FloorPartUp = link->ma_MemPart[idx];
	}

	ID = (long)mp_WallPartBelow; idx = mp_WallPartBelow_Idx;
	mp_WallPartBelow = NULL;
	if (floor->mm_CompoG.count(ID) > 0)
	{
		MSCompoG* compoG = (floor->mm_CompoG[ID]);
		MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(compoG);
// 		if (link != NULL && idx < link->ma_MemPart.size())
// 			mp_WallPartBelow = link->ma_MemPart[idx];
	}

	ID = (long)mp_LSlab;
	mp_LSlab = (MSSlabG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);
	ID = (long)mp_RSlab;
	mp_RSlab = (MSSlabG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);

}

MSObject* MSLinkMemPart::Copy( bool bNewID /*= true*/ )
{
	MSLinkMemPart* pNewObj = new MSLinkMemPart();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSLinkMemPart::CopyFromMe( MSLinkMemPart* pSource, bool bNewID/*=true*/)
{
	if(pSource==NULL)
		return;

	MSObject::CopyFromMe(pSource, bNewID);
	mp_LinkMem = pSource->mp_LinkMem;
	mp_SConn = pSource->GetSConn();
	mp_EConn = pSource->GetEConn();

	mp_LLine = pSource->mp_LLine->Copy();
	mp_RLine = pSource->mp_RLine->Copy();
	mp_CenLine = pSource->mp_CenLine->Copy();

    mp_FloorPartUp = pSource->mp_FloorPartUp;
    mp_WallPartBelow = pSource->mp_WallPartBelow;

	long NumSeg = pSource->ma_LSegments.size();
	for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
	{
		GM2DCurve* pCurve = pSource->ma_LSegments[iSeg];
		ma_LSegments.push_back(pCurve->Copy());
	}

	NumSeg = pSource->ma_RSegments.size();
	for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
	{
		GM2DCurve* pCurve = pSource->ma_RSegments[iSeg];
		ma_RSegments.push_back(pCurve->Copy());
	}
	

	/*
	ma_StartJointLevel.Copy(pSource->ma_StartJointLevel);
	ma_EndJointLevel.Copy(pSource->ma_EndJointLevel);

	ma_StartJointEndPoly.Copy(pSource->ma_StartJointEndPoly);
	ma_EndJointEndPoly.Copy(pSource->ma_EndJointEndPoly);
	*/
}

void MSLinkMemPart::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSObject::Init(bInConstructor);

	mp_SConn = NULL;
	mp_EConn = NULL;

	mp_LinkMem = NULL;

	mp_FloorPartUp = NULL;
	mp_WallPartBelow = NULL;
	mp_FloorPartUp_Idx = mp_WallPartBelow_Idx = 0;

	mp_LSlab = NULL;
	mp_RSlab = NULL;

	mp_LLine = NULL;
	mp_RLine = NULL;
	mp_CenLine = NULL;
	m_CenLineSetCnt = 0;
	m_IsParallelCenLine = false;

	mp_LPolyLine = new GM2DPolyline(TRUE);
	mp_RPolyLine = new GM2DPolyline(TRUE);
    mp_SPolyLine = new GM2DPolyline(TRUE);
    mp_EPolyLine = new GM2DPolyline(TRUE);

	mp_Boundary = new GM2DPolyline(TRUE);
    mp_TrimmedBoundary = new GM2DPolyline(TRUE);
//     mp_InitLLine = NULL;
//     mp_InitRLine = NULL;
// 
//     mp_LSExtLine = NULL;
//     mp_LEExtLine = NULL;
//     mp_RSExtLine = NULL;
//     mp_REExtLine = NULL;

    mp_SEndLine = NULL;
    mp_EEndLine = NULL;
	m_bDeleted = false;
	
}

double MSLinkMemPart::GetDegree(long StartOrEnd)
{
    double MyAngle;
    if(mp_LinkMem->mp_Curve==NULL || mp_LinkMem->GetType()  == MSElement::msLineLevelDifG)
    {
        GM2DLineVector MemLVec = mp_LinkMem->GetCenLVec();
        MyAngle = MemLVec.GetDegree();
        if(StartOrEnd == 1)
            MyAngle = MyAngle>180. ? MyAngle-180. : MyAngle+180.;
    }
    else
    {
        MSConnectivity* pJoint = StartOrEnd==0 ? mp_SConn : mp_EConn;

        GM2DArc* pArc = (GM2DArc*)mp_LLine;
        MyAngle = StartOrEnd==0 ? pArc->m_Theta1*Rad2Deg + 90. : pArc->m_Theta2*Rad2Deg - 90.;
    }

	if(MyAngle >= 360.) MyAngle -= 360.;
	if(MyAngle < 0. ) MyAngle += 360.;

	return MyAngle;
}

/*
void MSLinkMemPart::SetSlabLines()
{
	if(mp_LLine)	delete mp_LLine;
	if(mp_RLine)	delete mp_RLine;
	mp_LLine = NULL;
	mp_RLine = NULL;

	GM2DLineVector PartLVec(mp_SConn->m_dXc, mp_SConn->m_dYc, mp_EConn->m_dXc, mp_EConn->m_dYc);
	if(!mp_LinkMem && !mp_WallMemBelow)
	{
		mp_LLine = new GM2DLineVector(PartLVec);
		mp_RLine = new GM2DLineVector(PartLVec);
		return;
	}

	if(mp_LinkMem)
	{
		double LFarDist = -1.E10, RFarDist = 1.E10;
		GM2DVector DiagVec;

		if(mp_LinkMem->mp_Curve == NULL)
		{
			mp_LinkMem->SetLines();
			FindFarLine(PartLVec, (GM2DLineVector*)mp_LinkMem->mp_LLine, LFarDist, RFarDist);
			FindFarLine(PartLVec, (GM2DLineVector*)mp_LinkMem->mp_RLine, LFarDist, RFarDist);
		}
		else if(mp_LinkMem->mp_Curve->m_ObjType == GT_ARC)
		{
			GM2DArc* pArc = (GM2DArc*)mp_LinkMem->mp_Curve;

		}
		else if(mp_WallMemBelow && mp_WallMemBelow->mp_Curve==NULL)
		{
			mp_WallMemBelow->SetLines();
			FindFarLine(PartLVec, (GM2DLineVector*)mp_WallMemBelow->mp_LLine, LFarDist, RFarDist);
			FindFarLine(PartLVec, (GM2DLineVector*)mp_WallMemBelow->mp_RLine, LFarDist, RFarDist);
		}
	}
	else if(mp_WallMemBelow)
	{
		double LFarDist = -1.E10, RFarDist = 1.E10;
		GM2DVector DiagVec;
		if(mp_WallMemBelow->mp_Curve == NULL)
		{
			mp_WallMemBelow->SetLines();
			FindFarLine(PartLVec, (GM2DLineVector*)mp_WallMemBelow->mp_LLine, LFarDist, RFarDist);
			FindFarLine(PartLVec, (GM2DLineVector*)mp_WallMemBelow->mp_RLine, LFarDist, RFarDist);
		}
		else if(mp_WallMemBelow->mp_Curve->m_ObjType == GT_ARC)
		{
			GM2DArc* pArc = (GM2DArc*)mp_WallMemBelow->mp_Curve;

		}
		else if(mp_WallMemBelow->mp_Curve==NULL)
		{
			mp_WallMemBelow->SetLines();
			FindFarLine(PartLVec, (GM2DLineVector*)mp_WallMemBelow->mp_LLine, LFarDist, RFarDist);
			FindFarLine(PartLVec, (GM2DLineVector*)mp_WallMemBelow->mp_RLine, LFarDist, RFarDist);
		}
	}
}
*/

void MSLinkMemPart::SetSlabLines()
{
    if(mp_LLine)
        delete mp_LLine;
    mp_LLine = new GM2DLineVector(*mp_SConn, *mp_EConn);
    if(mp_RLine)
        delete mp_RLine;
    mp_RLine = new GM2DLineVector(*mp_SConn, *mp_EConn);
}

void MSLinkMemPart::FindFarLine(GM2DLineVector &PartLVec, GM2DLineVector* CurLVec, double &LFarDist, double &RFarDist )
{
	double Dist = PartLVec.Distance(CurLVec->m_P0, true);
	if(Dist > LFarDist)
	{
		if(mp_LLine)	delete mp_LLine;
		LFarDist = Dist;
		mp_LLine = new GM2DLineVector(*CurLVec);
	}
	if(Dist < RFarDist)
	{
		if(mp_RLine)	delete mp_RLine;
		RFarDist = Dist;
		mp_RLine = new GM2DLineVector(*CurLVec);
	}
}

GM2DLineVector MSLinkMemPart::GetEndLine( long StartOrEnd, BOOL bShort /*= TRUE */ )
{
	GM2DLineVector EndLine;
	GM2DVector LVec, RVec;
	double dLoc = 0;

	if(StartOrEnd==0)
	{
		if(mp_LinkMem->GetType() == MSElement::msLineLevelDifG)
		{
			EndLine.m_P0 = mp_LLine->GetP0();
			EndLine.m_P1 = mp_RLine->GetP0();
		}
		else
		{
			dLoc = mp_LLine->OrthoBaseLocValue(mp_RLine->GetP0());
			if(bShort)
			{
				if(dLoc < 0) // RLine 이 길다.
				{
					EndLine.m_P0 = mp_LLine->GetP0();
					EndLine.m_P1 = mp_RLine->OrthoBaseVector(mp_LLine->GetP0());
				}
				else // LLine 이 길다.
				{
					EndLine.m_P0 = mp_LLine->OrthoBaseVector(mp_RLine->GetP0());
					EndLine.m_P1 = mp_RLine->GetP0();
				}
			}
			else
			{
				if(dLoc < 0) // RLine 이 길다.
				{
					EndLine.m_P0 = mp_LLine->OrthoBaseVector(mp_RLine->GetP0());
					EndLine.m_P1 = mp_RLine->GetP0();				
				}
				else // LLine 이 길다.
				{
					EndLine.m_P0 = mp_LLine->GetP0();
					EndLine.m_P1 = mp_RLine->OrthoBaseVector(mp_LLine->GetP0());			
				}			
			}
		}
	}
	else
	{
		if(mp_LinkMem->GetType() == MSElement::msLineLevelDifG)
		{
			EndLine.m_P0 = mp_LLine->GetP1();
			EndLine.m_P1 = mp_RLine->GetP1();
		}
		else
		{
			dLoc = mp_LLine->OrthoBaseLocValue(mp_RLine->GetP1());
			if(bShort)
			{
				if(dLoc > 1 ) // RLine 이 길다.
				{
					EndLine.m_P0 = mp_LLine->GetP1();
					EndLine.m_P1 = mp_RLine->OrthoBaseVector(mp_LLine->GetP1());
				}
				else // LLine이 길다.
				{
					EndLine.m_P0 = mp_LLine->OrthoBaseVector(mp_RLine->GetP1());
					EndLine.m_P1 = mp_RLine->GetP1();
				}
			}
			else
			{
				if(dLoc > 1 ) // RLine 이 길다.
				{
					EndLine.m_P0 = mp_LLine->OrthoBaseVector(mp_RLine->GetP1());
					EndLine.m_P1 = mp_RLine->GetP1();
				}
				else // LLine이 길다.
				{
					EndLine.m_P0 = mp_LLine->GetP1();
					EndLine.m_P1 = mp_RLine->OrthoBaseVector(mp_LLine->GetP1());
				}
			}
		}
	}

	/*
	if(StartOrEnd==0)
	{
		//dLoc = mp_LLine->OrthoBaseLocValue(mp_LLine->GetP0());==0
		dLoc = mp_LLine->OrthoBaseLocValue(mp_RLine->GetP0());
		if(dLoc<0 && bShort)
		{
			EndLine.m_P0 = mp_LLine->GetP0();
			EndLine.m_P1 = mp_RLine->OrthoBaseVector(mp_LLine->GetP0());
		}
		else
		{
			EndLine.m_P0 = mp_LLine->OrthoBaseVector(mp_RLine->GetP0());
			EndLine.m_P1 = mp_RLine->GetP0();
		}
	}
	else
	{
		//LLoc = CenLine.OrthoBaseLocValue(mp_LLine->GetP1());
		dLoc = mp_LLine->OrthoBaseLocValue(mp_RLine->GetP1());
		if(dLoc>1 && bShort)
		{
			EndLine.m_P0 = mp_LLine->GetP1();
			EndLine.m_P1 = mp_RLine->OrthoBaseVector(mp_LLine->GetP1());
		}
		else
		{
			EndLine.m_P0 = mp_LLine->OrthoBaseVector(mp_RLine->GetP1());
			EndLine.m_P1 = mp_RLine->GetP1();
		}
	}
	*/
	return EndLine;
}

long MSLinkMemPart::AddEndJointLine2( MSConnLevelZone* pZone ,GM2DPolyline& Section, long nMemPartInJoint, long NumMemberG)
{
	MSConnectivity* pJoint = pZone->mp_Conn;
	
	MSConnectivity* pSJoint = GetSConn();
	MSConnectivity* pEJoint = GetEConn();

	GM2DCurve* pCenLine;
	GM2DLineVector MyCenLVec;
	if(mp_LinkMem->mp_Curve==NULL)
	{
		MyCenLVec = mp_LinkMem->GetCenLVec();
		pCenLine = &MyCenLVec;
	}
	else
		pCenLine = mp_LinkMem->mp_Curve;

	BOOL bHasDownLinkJoint = FALSE;
	if(pZone->mp_Conn->ma_DownLinkJoint.size() > 0)
		bHasDownLinkJoint = TRUE;

// 	GM2DVector LSVec = mp_LPolyLine->GetStartDrawVec();
// 	GM2DVector LEVec = mp_LPolyLine->GetEndDrawVec();
// 	GM2DVector RSVec = mp_RPolyLine->GetStartDrawVec();
// 	GM2DVector REVec = mp_RPolyLine->GetEndDrawVec();

	GM2DVector LSVec = mp_LLine->m_P0;
	GM2DVector LEVec = mp_LLine->m_P1;
	GM2DVector RSVec = mp_RLine->m_P0;
	GM2DVector REVec = mp_RLine->m_P1;

	double LLoc,RLoc;
	long numVec = Section.GetSize();
	if(pJoint->IsSameOrUpDown(GetSConn()))//R~L
	{
		LLoc = pCenLine->OrthoBaseLocValue(LSVec);
		RLoc = pCenLine->OrthoBaseLocValue(RSVec);
		if(LLoc>RLoc)
		{
			if(numVec == 0)
			{
				Section.AddCopy(RSVec);
				Section.ma_Bulge.Add(0);
			}
			else if(!Section.ma_Point[numVec-1]->SameVector(RSVec))
			{
				Section.AddCopy(RSVec);
				Section.ma_Bulge.Add(0);
			}
			//if(nMemPartInJoint==2 && !bHasDownLinkJoint) //Wall과 Beam 조인트 구할때 필요 없음 by kkw
			{
				/*if(mp_LinkMem->isArc())	// 아크부재일 경우는 LLine과  RLine을 바꿔서 계산한다.(Arc가 뒤집어지는걸 방지..)
				{
					Section.AddCopy(mp_LLine->OrthoBaseVector(RSVec));
					Section.ma_Bulge.Add(0);
				}
				else*/
				if(nMemPartInJoint == 2 || (nMemPartInJoint != NumMemberG*2) )
				{
					Section.AddCopy(mp_RLine->OrthoBaseVector(LSVec));
					Section.ma_Bulge.Add(0);
				}
			}
			Section.AddCopy(LSVec);
			Section.ma_Bulge.Add(0);
		}
		else
		{
			if(numVec == 0)
			{
				Section.AddCopy(RSVec);
								Section.ma_Bulge.Add(0);
			}
			else if(!Section.ma_Point[numVec-1]->SameVector(RSVec))
			{
				Section.AddCopy(RSVec);
				Section.ma_Bulge.Add(0);
			}
			//if(nMemPartInJoint==2 && !bHasDownLinkJoint) //Wall과 Beam 조인트 구할때 필요 없음 by kkw
			{
				/*if(mp_LinkMem->isArc()) // 아크부재일 경우는 LLine과  RLine을 바꿔서 계산한다.(Arc가 뒤집어지는걸 방지..)
				{
					Section.AddCopy(mp_RLine->OrthoBaseVector(LSVec));
					Section.ma_Bulge.Add(0);
				}
				else*/
				if(nMemPartInJoint == 2 || (nMemPartInJoint != NumMemberG*2) )
				{
					Section.AddCopy(mp_LLine->OrthoBaseVector(RSVec));
					Section.ma_Bulge.Add(0);
				}
			}
			Section.AddCopy(LSVec);
			Section.ma_Bulge.Add(0);
		}
	}
	else//L~R
	{
		LLoc = pCenLine->OrthoBaseLocValue(LEVec);
		RLoc = pCenLine->OrthoBaseLocValue(REVec);
		if(LLoc<RLoc)
		{
			if(numVec == 0)
			{
				Section.AddCopy(LEVec);
				//Section.ma_Bulge.Add(mp_LLine->GetBulge());
				Section.ma_Bulge.Add(0);
			}
			else if(!Section.ma_Point[numVec-1]->SameVector(LEVec))
			{
				Section.AddCopy(LEVec);
				//Section.ma_Bulge.Add(mp_LLine->GetBulge());
				Section.ma_Bulge.Add(0);
			}
			//if(nMemPartInJoint==2 && !bHasDownLinkJoint) //Wall과 Beam 조인트 구할때 필요 없음 by kkw
			{
				/*if(mp_LinkMem->isArc())	// 아크부재일 경우는 LLine과  RLine을 바꿔서 계산한다.(Arc가 뒤집어지는걸 방지..)
				{
					Section.AddCopy(mp_LLine->OrthoBaseVector(REVec));
					//Section.ma_Bulge.Add(mp_RLine->GetBulge());
					Section.ma_Bulge.Add(0);
				}
				else*/
				if(nMemPartInJoint == 2 || (nMemPartInJoint != NumMemberG*2) )
				{
					Section.AddCopy(mp_RLine->OrthoBaseVector(LEVec));
					//Section.ma_Bulge.Add(mp_RLine->GetBulge());
					Section.ma_Bulge.Add(0);
				}
				
			}
			Section.AddCopy(REVec);
			//Section.ma_Bulge.Add(mp_RLine->GetBulge());
			Section.ma_Bulge.Add(0);

		}
		else
		{
			if(numVec == 0)
			{
				Section.AddCopy(LEVec);
				//Section.ma_Bulge.Add(mp_LLine->GetBulge());
				Section.ma_Bulge.Add(0);
			}
			else if(!Section.ma_Point[numVec-1]->SameVector(LEVec))
			{
				Section.AddCopy(LEVec);
				//Section.ma_Bulge.Add(mp_LLine->GetBulge());
				Section.ma_Bulge.Add(0);
			}
			//if(nMemPartInJoint==2 && !bHasDownLinkJoint) //Wall과 Beam 조인트 구할때 필요 없음 by kkw
			{
				/*
				if(mp_LinkMem->isArc()) // 아크부재일 경우는 LLine과  RLine을 바꿔서 계산한다.(Arc가 뒤집어지는걸 방지..)
				{
					Section.AddCopy(mp_RLine->OrthoBaseVector(LEVec));
					//Section.ma_Bulge.Add(mp_LLine->GetBulge());
					Section.ma_Bulge.Add(0);
				}
				else*/
				if(nMemPartInJoint == 2 || (nMemPartInJoint != NumMemberG*2) )
				{
					Section.AddCopy(mp_LLine->OrthoBaseVector(REVec));
					//Section.ma_Bulge.Add(mp_LLine->GetBulge());
					Section.ma_Bulge.Add(0);
				}
			}
			Section.AddCopy(REVec);
			//Section.ma_Bulge.Add(mp_RLine->GetBulge());
			Section.ma_Bulge.Add(0);
		}
	}
	return Section.GetSize();
}

long MSLinkMemPart::AddEndJointLine( MSConnLevelZone* pZone ,GM2DPolyline& Section, long nMemPartInJoint)
{
	MSConnectivity* pJoint = pZone->mp_Conn;

	GM2DCurve* pCenLine;
	GM2DLineVector MyCenLVec;
	if(mp_LinkMem->mp_Curve==NULL)
	{
		MyCenLVec = mp_LinkMem->GetCenLVec();
		pCenLine = &MyCenLVec;
	}
	else
		pCenLine = mp_LinkMem->mp_Curve;

	BOOL bHasDownLinkJoint = FALSE;
	if(pZone->mp_Conn->ma_DownLinkJoint.size() > 0 )
		bHasDownLinkJoint = TRUE;

	double LLoc,RLoc;
	long numVec = Section.GetSize();
	if(pJoint->IsSameOrUpDown(GetSConn()))//R~L
	{
		LLoc = pCenLine->OrthoBaseLocValue(mp_LLine->GetP0());
		RLoc = pCenLine->OrthoBaseLocValue(mp_RLine->GetP0());
		if(LLoc>RLoc)
		{
			if(numVec == 0)
			{
				Section.AddCopy(mp_RLine->GetP0());
				Section.ma_Bulge.Add(0);
			}
			else if(!Section.ma_Point[numVec-1]->SameVector(mp_RLine->GetP0()))
			{
				Section.AddCopy(mp_RLine->GetP0());
				Section.ma_Bulge.Add(0);
			}
			//if(nMemPartInJoint==2 && !bHasDownLinkJoint) //Wall과 Beam 조인트 구할때 필요 없음 by kkw
			{
				/*if(mp_LinkMem->isArc())	// 아크부재일 경우는 LLine과  RLine을 바꿔서 계산한다.(Arc가 뒤집어지는걸 방지..)
				{
					Section.AddCopy(mp_LLine->OrthoBaseVector(mp_RLine->GetP0()));
					Section.ma_Bulge.Add(0);
				}
				else*/
				//if(nMemPartInJoint == 2)
				{
					Section.AddCopy(mp_RLine->OrthoBaseVector(mp_LLine->GetP0()));
					Section.ma_Bulge.Add(0);
				}
			}
			Section.AddCopy(mp_LLine->GetP0());
			Section.ma_Bulge.Add(0);
		}
		else
		{
			if(numVec == 0)
			{
				Section.AddCopy(mp_RLine->GetP0());
								Section.ma_Bulge.Add(0);
			}
			else if(!Section.ma_Point[numVec-1]->SameVector(mp_RLine->GetP0()))
			{
				Section.AddCopy(mp_RLine->GetP0());
				Section.ma_Bulge.Add(0);
			}
			//if(nMemPartInJoint==2 && !bHasDownLinkJoint) //Wall과 Beam 조인트 구할때 필요 없음 by kkw
			{
				/*if(mp_LinkMem->isArc()) // 아크부재일 경우는 LLine과  RLine을 바꿔서 계산한다.(Arc가 뒤집어지는걸 방지..)
				{
					Section.AddCopy(mp_RLine->OrthoBaseVector(mp_LLine->GetP0()));
					Section.ma_Bulge.Add(0);
				}
				else*/
				//if(nMemPartInJoint == 2)
				{
					Section.AddCopy(mp_LLine->OrthoBaseVector(mp_RLine->GetP0()));
					Section.ma_Bulge.Add(0);
				}
			}
			Section.AddCopy(mp_LLine->GetP0());
			Section.ma_Bulge.Add(0);
		}
	}
	else//L~R
	{
		LLoc = pCenLine->OrthoBaseLocValue(mp_LLine->GetP1());
		RLoc = pCenLine->OrthoBaseLocValue(mp_RLine->GetP1());
		if(LLoc<RLoc)
		{
			if(numVec == 0)
			{
				Section.AddCopy(mp_LLine->GetP1());
				//Section.ma_Bulge.Add(mp_LLine->GetBulge());
				Section.ma_Bulge.Add(0);
			}
			else if(!Section.ma_Point[numVec-1]->SameVector(mp_LLine->GetP1()))
			{
				Section.AddCopy(mp_LLine->GetP1());
				//Section.ma_Bulge.Add(mp_LLine->GetBulge());
				Section.ma_Bulge.Add(0);
			}
			//if(nMemPartInJoint==2 && !bHasDownLinkJoint) //Wall과 Beam 조인트 구할때 필요 없음 by kkw
			{
				/*if(mp_LinkMem->isArc())	// 아크부재일 경우는 LLine과  RLine을 바꿔서 계산한다.(Arc가 뒤집어지는걸 방지..)
				{
					Section.AddCopy(mp_LLine->OrthoBaseVector(mp_RLine->GetP1()));
					//Section.ma_Bulge.Add(mp_RLine->GetBulge());
					Section.ma_Bulge.Add(0);
				}
				else*/
				//if(nMemPartInJoint == 2)
				{
					Section.AddCopy(mp_RLine->OrthoBaseVector(mp_LLine->GetP1()));
					//Section.ma_Bulge.Add(mp_RLine->GetBulge());
					Section.ma_Bulge.Add(0);
				}
				
			}
			Section.AddCopy(mp_RLine->GetP1());
			//Section.ma_Bulge.Add(mp_RLine->GetBulge());
			Section.ma_Bulge.Add(0);

		}
		else
		{
			if(numVec == 0)
			{
				Section.AddCopy(mp_LLine->GetP1());
				//Section.ma_Bulge.Add(mp_LLine->GetBulge());
				Section.ma_Bulge.Add(0);
			}
			else if(!Section.ma_Point[numVec-1]->SameVector(mp_LLine->GetP1()))
			{
				Section.AddCopy(mp_LLine->GetP1());
				//Section.ma_Bulge.Add(mp_LLine->GetBulge());
				Section.ma_Bulge.Add(0);
			}
			//if(nMemPartInJoint==2 && !bHasDownLinkJoint) //Wall과 Beam 조인트 구할때 필요 없음 by kkw
			{
				/*
				if(mp_LinkMem->isArc()) // 아크부재일 경우는 LLine과  RLine을 바꿔서 계산한다.(Arc가 뒤집어지는걸 방지..)
				{
					Section.AddCopy(mp_RLine->OrthoBaseVector(mp_LLine->GetP1()));
					//Section.ma_Bulge.Add(mp_LLine->GetBulge());
					Section.ma_Bulge.Add(0);
				}
				else*/
				//if(nMemPartInJoint == 2)
				{
					Section.AddCopy(mp_LLine->OrthoBaseVector(mp_RLine->GetP1()));
					//Section.ma_Bulge.Add(mp_LLine->GetBulge());
					Section.ma_Bulge.Add(0);
				}
			}
			Section.AddCopy(mp_RLine->GetP1());
			//Section.ma_Bulge.Add(mp_RLine->GetBulge());
			Section.ma_Bulge.Add(0);
		}
	}
	return Section.GetSize();
}

void MSLinkMemPart::SetLinesOfAMemPart(bool SConn, bool EConn)
{
    GM2DCurve* pOldLLine = NULL;
    GM2DCurve* pOldRLine = NULL;
	GM2DCurve* pOldCenLine = NULL;

    MSLinkMemberG* pLinkMem = mp_LinkMem;
    if(mp_LLine)
    {
        pOldLLine = mp_LLine->Copy();
        delete mp_LLine;
    }
    if(mp_RLine)
    {
        pOldRLine = mp_RLine->Copy();
        delete mp_RLine;
    }
	if(mp_CenLine)
	{
		pOldCenLine = mp_CenLine->Copy();
		delete mp_CenLine;
	}

    if( ( pLinkMem->GetType() == MSElement::msLineLevelDifG && !pLinkMem->isArc())
		|| pLinkMem->mp_Curve==NULL)
    {
        GM2DLineVector* LLine = (GM2DLineVector*)pLinkMem->mp_LLine;
        GM2DLineVector* RLine = (GM2DLineVector*)pLinkMem->mp_RLine;
        mp_LLine = new GM2DLineVector(*LLine);
        mp_RLine = new GM2DLineVector(*RLine);
		mp_CenLine = new GM2DLineVector(*pLinkMem->GetSVec(), *pLinkMem->GetEVec());
    }
    else
    {
		mp_LLine = pLinkMem->mp_LLine->Copy();
		mp_RLine = pLinkMem->mp_RLine->Copy();
		mp_CenLine = pLinkMem->mp_Curve->Copy();
    }
//     if(mp_SConn && pLinkMem->ma_Joint[0] != mp_SConn)
//     {
//         GM2DVector midVec = *mp_SConn; 
//         mp_LLine->SetP0(mp_LLine->OrthoBaseVector(midVec));
//         mp_RLine->SetP0(mp_RLine->OrthoBaseVector(midVec));
// 		mp_CenLine->SetP0(mp_CenLine->OrthoBaseVector(midVec));
// 
//     }
//     if(mp_EConn && pLinkMem->ma_Joint[1] != mp_EConn)
//     {
//         GM2DVector midVec = *mp_EConn; 
//         mp_LLine->SetP1(mp_LLine->OrthoBaseVector(midVec));
//         mp_RLine->SetP1(mp_RLine->OrthoBaseVector(midVec));
// 		mp_CenLine->SetP1(mp_CenLine->OrthoBaseVector(midVec));
//     }
    if(pOldLLine)
    {
        if(!EConn)
        {
            mp_LLine->SetP1(pOldLLine->m_P1);
            mp_RLine->SetP1(pOldRLine->m_P1);
			mp_CenLine->SetP1(pOldCenLine->m_P1);
        }
        if(!SConn)
        {
            mp_LLine->SetP0(pOldLLine->m_P0);
            mp_RLine->SetP0(pOldRLine->m_P0);
			mp_CenLine->SetP0(pOldCenLine->m_P0);
        }
        delete pOldLLine;
        delete pOldRLine;
		delete pOldCenLine;
    }

    // for L-Segments
    vector<GM2DCurve*> tmpSegments;
    long NumSeg = ma_LSegments.size();
    for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
    {
        GM2DCurve* pCurve = ma_LSegments[iSeg];
        double dSLoc = mp_LLine->OrthoBaseLocValue(pCurve->m_P0);
        double dELoc = mp_LLine->OrthoBaseLocValue(pCurve->m_P1);

        if(dELoc < -DTOL_GM)
        {
            delete pCurve;
            continue;
        }
        if(dSLoc > 1+DTOL_GM)
        {
            delete pCurve;
            continue;
        }

        if(dSLoc < -DTOL_GM && SConn)
            pCurve->SetP0(mp_LLine->m_P0);

        if(dELoc > 1+DTOL_GM && EConn)
            pCurve->SetP1(mp_LLine->m_P1);

        tmpSegments.push_back(pCurve);
    }
    ma_LSegments = tmpSegments;

    // for R-Segments
    tmpSegments.clear();
    NumSeg = ma_RSegments.size();
    for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
    {
        GM2DCurve* pCurve = ma_RSegments[iSeg];
        double dSLoc = mp_RLine->OrthoBaseLocValue(pCurve->m_P0);
        double dELoc = mp_RLine->OrthoBaseLocValue(pCurve->m_P1);

        if(dELoc < -DTOL_GM)
        {
            delete pCurve;
            continue;
        }
        if(dSLoc > 1+DTOL_GM)
        {
            delete pCurve;
            continue;
        }

        if(dSLoc < 0 && SConn)
            pCurve->SetP0(mp_RLine->m_P0);

        if(dELoc > 1 && EConn)
            pCurve->SetP1(mp_RLine->m_P1);

        tmpSegments.push_back(pCurve);
    }
    ma_RSegments = tmpSegments;

}

void MSLinkMemPart::SetLRPolyLine(MSConnectivity* pWhichEndConn)
{
	mp_LPolyLine->MergeSameDrawLine();
	mp_RPolyLine->MergeSameDrawLine();

	long NumLBulge = mp_LPolyLine->ma_Bulge.GetSize();
	long NumRBulge = mp_RPolyLine->ma_Bulge.GetSize();
	long NumLVec = mp_LPolyLine->ma_Point.GetSize();
	long NumRVec = mp_RPolyLine->ma_Point.GetSize();
	long NumLLine = mp_LPolyLine->GetNumLines();
	long NumRLine = mp_RPolyLine->GetNumLines();

/*
	mp_LPolyLine->ResetPolyline();
	mp_LPolyLine->AddSegment(mp_LPolyLine, mp_LLine, true);
	mp_RPolyLine->ResetPolyline();
	mp_RPolyLine->AddSegment(mp_RPolyLine, mp_RLine, true);

	return;
*/

	if(NumLVec == 0)
	{
		mp_LPolyLine->ResetPolyline();
		mp_LPolyLine->AddSegment(mp_LPolyLine, mp_LLine);
	}
	else
	{
		
		if(pWhichEndConn == mp_EConn)
		{
			long FindIdx = -1;
			for(long iLine = NumLLine - 1 ; iLine >= 0 ; iLine--)
			{
				GM2DCurve* pCurve = mp_LPolyLine->GetCurve(iLine);
				double dLoc = pCurve->OrthoBaseLocValue(mp_LLine->m_P1);
				if(iLine == NumLLine -1)
				{
					if(dLoc > 0)
					{
						pCurve->SetP1(mp_LLine->m_P1);
						mp_LPolyLine->ma_Point[iLine + 1]->SetXY(pCurve->m_P1.m_X, pCurve->m_P1.m_Y);
						mp_LPolyLine->ma_Bulge[iLine] = pCurve->GetBulge();
						mp_LPolyLine->SetVisibility(iLine, true);
						FindIdx = iLine;
						delete pCurve;
						break;
					}
				}
				else
				{
					if(dLoc > 0 && dLoc <= 1)
					{
						pCurve->SetP1(mp_LLine->m_P1);
						mp_LPolyLine->ma_Point[iLine + 1]->SetXY(pCurve->m_P1.m_X, pCurve->m_P1.m_Y);
						mp_LPolyLine->ma_Bulge[iLine] = pCurve->GetBulge();
						mp_LPolyLine->SetVisibility(iLine, true);
						FindIdx = iLine;
						delete pCurve;
						break;
					}
				}
				delete pCurve;
			}

			if(FindIdx != -1 && NumLLine > 1)
			{
				for(long iLine = NumLLine - 1 ; iLine > FindIdx ; iLine--)
				{
					delete mp_LPolyLine->ma_Point[iLine + 1];
					mp_LPolyLine->ma_Point.RemoveAt(iLine + 1);
					mp_LPolyLine->RemoveVisibilityAt(iLine);
					mp_LPolyLine->ma_Bulge.RemoveAt(iLine);
				}
			}

			/*
			GM2DCurve* pCurve = mp_LPolyLine->GetCurve(NumLLine -1);
			pCurve->SetP1(mp_LLine->m_P1);
			mp_LPolyLine->ma_Point[NumLVec -1]->SetXY(pCurve->m_P1.m_X, pCurve->m_P1.m_Y);
			mp_LPolyLine->ma_Bulge[NumLBulge - 1] = pCurve->GetBulge();
			//mp_LPolyLine->SetVisibility(NumLLine -1, true);
			delete pCurve;
			*/
		}
		else if(pWhichEndConn == mp_SConn)
		{
			long FindIdx = -1;
			for(long iLine = 0 ; iLine < NumLLine ; iLine++)
			{
				GM2DCurve* pCurve = mp_LPolyLine->GetCurve(iLine);
				double dLoc = pCurve->OrthoBaseLocValue(mp_LLine->m_P0);
				if(iLine == 0)
				{
					if(dLoc < 1)
					{
						pCurve->SetP0(mp_LLine->m_P0);
						mp_LPolyLine->ma_Point[iLine]->SetXY(pCurve->m_P0.m_X, pCurve->m_P0.m_Y);
						mp_LPolyLine->ma_Bulge[iLine] = pCurve->GetBulge();
						mp_LPolyLine->SetVisibility(iLine, true);
						FindIdx = iLine;
						delete pCurve;
						break;
					}
				}
				else
				{
					if(dLoc >= 0 && dLoc < 1)
					{
						pCurve->SetP0(mp_LLine->m_P0);
						mp_LPolyLine->ma_Point[iLine]->SetXY(pCurve->m_P0.m_X, pCurve->m_P0.m_Y);
						mp_LPolyLine->ma_Bulge[iLine] = pCurve->GetBulge();
						mp_LPolyLine->SetVisibility(iLine, true);
						FindIdx = iLine;
						delete pCurve;
						break;
					}
				}
				delete pCurve;
			}

			if(FindIdx != -1 && NumLLine > 1)
			{
				for(long iLine = FindIdx -1 ; iLine >= 0 ; iLine--)
				{
					delete mp_LPolyLine->ma_Point[iLine];
					mp_LPolyLine->ma_Point.RemoveAt(iLine);
					mp_LPolyLine->RemoveVisibilityAt(iLine);
					mp_LPolyLine->ma_Bulge.RemoveAt(iLine);
				}
			}

			/*
			GM2DCurve* pCurve = mp_LPolyLine->GetCurve(0);
			pCurve->SetP0(mp_LLine->m_P0);
			mp_LPolyLine->ma_Point[0]->SetXY(pCurve->m_P0.m_X, pCurve->m_P0.m_Y);
			mp_LPolyLine->ma_Bulge[0] = pCurve->GetBulge();
			//mp_LPolyLine->SetVisibility(0, true);
			delete pCurve;
			*/
		}
		else
			ASSERT(0);
	}
	if(NumRVec == 0)
	{
		mp_RPolyLine->ResetPolyline();
		mp_RPolyLine->AddSegment(mp_RPolyLine, mp_RLine);
	}
	else
	{
		
		if(pWhichEndConn == mp_EConn)
		{
			long FindIdx = -1;
			for(long iLine = NumRLine - 1 ; iLine >= 0 ; iLine--)
			{
				GM2DCurve* pCurve = mp_RPolyLine->GetCurve(iLine);
				double dLoc = pCurve->OrthoBaseLocValue(mp_RLine->m_P1);
				if(iLine == NumRLine -1)
				{
					if(dLoc > 0)
					{
						pCurve->SetP1(mp_RLine->m_P1);
						mp_RPolyLine->ma_Point[iLine + 1]->SetXY(pCurve->m_P1.m_X, pCurve->m_P1.m_Y);
						mp_RPolyLine->ma_Bulge[iLine] = pCurve->GetBulge();
						mp_RPolyLine->SetVisibility(iLine, true);
						FindIdx = iLine;
						delete pCurve;
						break;
					}
				}
				else
				{
					if(dLoc > 0 && dLoc <= 1)
					{
						pCurve->SetP1(mp_RLine->m_P1);
						mp_RPolyLine->ma_Point[iLine + 1]->SetXY(pCurve->m_P1.m_X, pCurve->m_P1.m_Y);
						mp_RPolyLine->ma_Bulge[iLine] = pCurve->GetBulge();
						mp_RPolyLine->SetVisibility(iLine, true);
						FindIdx = iLine;
						delete pCurve;
						break;
					}
				}
				delete pCurve;
			}

			if(FindIdx != -1 && NumRLine > 1)
			{
				for(long iLine = NumRLine - 1 ; iLine > FindIdx ; iLine--)
				{
					delete mp_RPolyLine->ma_Point[iLine + 1];
					mp_RPolyLine->ma_Point.RemoveAt(iLine + 1);
					mp_RPolyLine->RemoveVisibilityAt(iLine);
					mp_RPolyLine->ma_Bulge.RemoveAt(iLine);
				}
			}
			/*GM2DCurve* pCurve = mp_RPolyLine->GetCurve(NumRLine -1);
			pCurve->SetP1(mp_RLine->m_P1);
			mp_RPolyLine->ma_Point[NumRVec -1]->SetXY(pCurve->m_P1.m_X, pCurve->m_P1.m_Y);
			mp_RPolyLine->ma_Bulge[NumRBulge - 1] = pCurve->GetBulge();
			//mp_RPolyLine->SetVisibility(NumRLine -1, true);
			delete pCurve;*/
		}
		else if(pWhichEndConn == mp_SConn)
		{
			long FindIdx = -1;
			for(long iLine = 0 ; iLine < NumRLine ; iLine++)
			{
				GM2DCurve* pCurve = mp_RPolyLine->GetCurve(iLine);
				double dLoc = pCurve->OrthoBaseLocValue(mp_RLine->m_P0);
				if(iLine == 0)
				{
					if(dLoc < 1)
					{
						pCurve->SetP0(mp_RLine->m_P0);
						mp_RPolyLine->ma_Point[iLine]->SetXY(pCurve->m_P0.m_X, pCurve->m_P0.m_Y);
						mp_RPolyLine->ma_Bulge[iLine] = pCurve->GetBulge();
						mp_RPolyLine->SetVisibility(iLine, true);
						FindIdx = iLine;
						delete pCurve;
						break;
					}
				}
				else
				{
					if(dLoc >= 0 && dLoc < 1)
					{
						pCurve->SetP0(mp_RLine->m_P0);
						mp_RPolyLine->ma_Point[iLine]->SetXY(pCurve->m_P0.m_X, pCurve->m_P0.m_Y);
						mp_RPolyLine->ma_Bulge[iLine] = pCurve->GetBulge();
						mp_RPolyLine->SetVisibility(iLine, true);
						FindIdx = iLine;
						delete pCurve;
						break;
					}
				}
				delete pCurve;
			}

			if(FindIdx != -1 && NumRLine > 1)
			{
				for(long iLine = FindIdx -1 ; iLine >= 0 ; iLine--)
				{
					delete mp_RPolyLine->ma_Point[iLine];
					mp_RPolyLine->ma_Point.RemoveAt(iLine);
					mp_RPolyLine->RemoveVisibilityAt(iLine);
					mp_RPolyLine->ma_Bulge.RemoveAt(iLine);
				}
			}
			/*GM2DCurve* pCurve = mp_RPolyLine->GetCurve(0);
			pCurve->SetP0(mp_RLine->m_P0);
			mp_RPolyLine->ma_Point[0]->SetXY(pCurve->m_P0.m_X, pCurve->m_P0.m_Y);
			mp_RPolyLine->ma_Bulge[0] = pCurve->GetBulge();
			//mp_RPolyLine->SetVisibility(0, true);
			delete pCurve;*/
		}
		else
			ASSERT(0);
	}
}

void MSLinkMemPart::SetEdgeTrim( long StartOrEnd, char LineSide, GM2DVector& LineStartVec )
{
//     if(StartOrEnd==0)
//     {
//         if(LineSide=='L')
//         {
//             if(mp_LSExtLine)
//             {
//                 double ExtLineLoc = mp_LLine->Length();
//             }
//                 delete mp_LSExtLine;
//             mp_LSExtLine = mp_LLine->Copy();
//             mp_LSExtLine->SetP1(LineStartVec);
//         }
//         else
//         {
//             if(mp_RSExtLine)
//                 delete mp_RSExtLine;
//             mp_RSExtLine = mp_RLine->Copy();
//             mp_RSExtLine->SetP1(LineStartVec);
//         }
//         if(mp_SEndLine)
//             delete mp_SEndLine;
//         mp_SEndLine = new GM2DLineVector;
//         *mp_SEndLine = GetEndLine(0, FALSE);
//     }
//     else
//     {
//         if(LineSide=='L')
//         {
//             if(mp_LEExtLine)
//                 delete mp_LEExtLine;
//             mp_LEExtLine = mp_LLine->Copy();
//             mp_LEExtLine->SetP0(LineStartVec);
//         }
//         else
//         {
//             if(mp_REExtLine)
//                 delete mp_REExtLine;
//             mp_REExtLine = mp_RLine->Copy();
//             mp_REExtLine->SetP0(LineStartVec);
//         }
//         if(mp_EEndLine)
//             delete mp_EEndLine;
//         mp_EEndLine = new GM2DLineVector;
//         *mp_EEndLine = GetEndLine(1, FALSE);
// 
//     }
}

/*
bool MSLinkMemPart::SetLineXY( long StartOrEnd, char LineSide, GM2DVector& NewPoint, GM2DCurve* pExtLine, bool bOnlyShorten )
{
    double OldLength, NewLength;
    if(StartOrEnd==0)
    {
        if(LineSide=='L')
        {
//             if(bOnlyShorten==false)
            if(mp_LSExtLine==NULL && bOnlyShorten==false)
            {
                mp_LLine->SetP0(NewPoint);
                return true;
            }
            OldLength = mp_LLine->Length();
            NewLength = mp_LLine->m_P1.Distance(NewPoint);
            if(NewLength<OldLength)
            {
                if(mp_LSExtLine)
                    delete mp_LSExtLine;
                mp_LSExtLine =pExtLine;
                mp_LLine->SetP0(NewPoint);
                return true;
            }
        }
        else
        {
            if(mp_RSExtLine==NULL && bOnlyShorten==false)
            {
                mp_RLine->SetP0(NewPoint);
                return true;
            }
            OldLength = mp_RLine->Length();
            NewLength = mp_RLine->m_P1.Distance(NewPoint);
            if(NewLength<OldLength)
            {
                if(mp_RSExtLine)
                    delete mp_RSExtLine;
                mp_RSExtLine =pExtLine;
                mp_RLine->SetP0(NewPoint);
                return true;
            }
        }
    }
    else
    {
        if(LineSide=='L')
        {
            if(mp_LEExtLine==NULL && bOnlyShorten==false)
            {
                mp_LLine->SetP1(NewPoint);
                return true;
            }
            OldLength = mp_LLine->Length();
            NewLength = mp_LLine->m_P0.Distance(NewPoint);
            if(NewLength<OldLength)
            {
                if(mp_LEExtLine)
                    delete mp_LEExtLine;
                mp_LEExtLine =pExtLine;
                mp_LLine->SetP1(NewPoint);
                return true;
            }
        }
        else
        {
            if(mp_REExtLine==NULL && bOnlyShorten==false)
            {
                mp_RLine->SetP1(NewPoint);
                return true;
            }
            OldLength = mp_RLine->Length();
            NewLength = mp_RLine->m_P0.Distance(NewPoint);
            if(NewLength<OldLength)
            {
                if(mp_REExtLine)
                    delete mp_REExtLine;
                mp_REExtLine =pExtLine;
                mp_RLine->SetP1(NewPoint);
                return true;
            }
        }
    }
    return false;
}
*/
bool MSLinkMemPart::PreCheckSetLine( long StartOrEnd, char LineSide, GM2DVector& NewPoint, bool bOnlyShorten)
{
	double dWidth = mp_LinkMem->GetWidth();
	double dMaxExtendLen = dWidth*2;
	double dLineHitDistLoc = 0;
	double dDist = 0;

	if(StartOrEnd==0)
	{
		if(LineSide=='L')
		{
			double NewPointLoc = mp_LLine->OrthoBaseLocValue(NewPoint);
			if(NewPointLoc < 0)
				dDist = mp_LLine->m_P0.Distance(NewPoint);
			else if(NewPointLoc > 1)
				dDist = mp_LLine->m_P1.Distance(NewPoint);

			if(dDist > dMaxExtendLen)
				return false;
		}
		else
		{
			double NewPointLoc = mp_RLine->OrthoBaseLocValue(NewPoint);
			if(NewPointLoc < 0)
				dDist = mp_RLine->m_P0.Distance(NewPoint);
			else if(NewPointLoc > 1)
				dDist = mp_RLine->m_P1.Distance(NewPoint);

			if(dDist > dMaxExtendLen)
				return false;
		}
	}
	else
	{
		if(LineSide=='L')
		{
			double NewPointLoc = mp_LLine->OrthoBaseLocValue(NewPoint);
			if(NewPointLoc < 0)
				dDist = mp_LLine->m_P0.Distance(NewPoint);
			else if(NewPointLoc > 1)
				dDist = mp_LLine->m_P1.Distance(NewPoint);

			if(dDist > dMaxExtendLen)
				return false;

		
		}
		else
		{
			double NewPointLoc = mp_RLine->OrthoBaseLocValue(NewPoint);
			if(NewPointLoc < 0)
				dDist = mp_RLine->m_P0.Distance(NewPoint);
			else if(NewPointLoc > 1)
				dDist = mp_RLine->m_P1.Distance(NewPoint);

			if(dDist > dMaxExtendLen)
				return false;
		}
	}
	return true;
}

long MSLinkMemPart::SetLineXY( long StartOrEnd, char LineSide, GM2DVector& NewPoint, bool bOnlyShorten )
{
	MSElement::Type_MSCompoG eMemType = mp_LinkMem->GetType();
	
	double dWidth = mp_LinkMem->GetWidth();
	double dMaxExtendLen = dWidth*2;
	double dLineHitDistLoc = 0;
	double dDist = 0;

    if(StartOrEnd==0)
    {
        if(LineSide=='L')
        {
            double NewPointLoc = mp_LLine->OrthoBaseLocValue(NewPoint);
			dLineHitDistLoc = MSFloor::ms_LineHitDistTol / mp_LLine->Length();
            if(bOnlyShorten==false || eMemType == MSElement::msLineLevelDifG)
            {
                mp_LLine->SetP0(NewPoint);
                if(NewPointLoc >= 1. + dLineHitDistLoc)
                    return -1;
                return 1;
            }
            if(NewPointLoc > 0. - dLineHitDistLoc && NewPointLoc < 1. + dLineHitDistLoc)
            {
                mp_LLine->SetP0(NewPoint);
                if(NewPointLoc >= 1.)
                    return -1;
                return 1;
            }
        }
        else
        {
            double NewPointLoc = mp_RLine->OrthoBaseLocValue(NewPoint);
			dLineHitDistLoc = MSFloor::ms_LineHitDistTol / mp_RLine->Length();
            if(bOnlyShorten==false || eMemType == MSElement::msLineLevelDifG)
            {
                mp_RLine->SetP0(NewPoint);
                if(NewPointLoc >= 1. + dLineHitDistLoc)
                    return -1;
                return 1;
            }
            if(NewPointLoc > 0. - dLineHitDistLoc && NewPointLoc < 1. + dLineHitDistLoc)
            {
                mp_RLine->SetP0(NewPoint);
                if(NewPointLoc >= 1.)
                    return -1;
                return 1;
            }
        }
    }
    else
    {
        if(LineSide=='L')
        {
            double NewPointLoc = mp_LLine->OrthoBaseLocValue(NewPoint);
			dLineHitDistLoc = MSFloor::ms_LineHitDistTol / mp_LLine->Length();
            if(bOnlyShorten==false || eMemType == MSElement::msLineLevelDifG)
            {
                mp_LLine->SetP1(NewPoint);
                if(NewPointLoc <= 0. - dLineHitDistLoc)
                    return -1;
                return 1;
            }
            if(NewPointLoc > 0. - dLineHitDistLoc && NewPointLoc < 1. + dLineHitDistLoc)
            {
                mp_LLine->SetP1(NewPoint);
                if(NewPointLoc <= 0.)
                    return -1;
                return 1;
            }
        }
        else
        {
            double NewPointLoc = mp_RLine->OrthoBaseLocValue(NewPoint);
			dLineHitDistLoc = MSFloor::ms_LineHitDistTol / mp_RLine->Length();
            if(bOnlyShorten==false || eMemType == MSElement::msLineLevelDifG)
            {
                mp_RLine->SetP1(NewPoint);
                if(NewPointLoc <= 0. - dLineHitDistLoc)
                    return -1;
                return 1;
            }
            if(NewPointLoc > 0. - dLineHitDistLoc&& NewPointLoc < 1. + dLineHitDistLoc)
            {
                mp_RLine->SetP1(NewPoint);
                if(NewPointLoc <= 0.)
                    return -1;
                return 1;
            }
        }
    }
    return 0;
}

long MSLinkMemPart::SetLineXY2( long StartOrEnd, char LineSide, GM2DVector& NewPoint, bool bOnlyShorten, bool bOnlyCross )
{
	double dLineHitDistLoc = 0;
	if(StartOrEnd==0)
	{
		if(LineSide=='L')
		{
			double NewPointLoc = mp_LLine->OrthoBaseLocValue(NewPoint);
			dLineHitDistLoc = MSFloor::ms_LineHitDistTol / mp_LLine->Length();
			if(bOnlyCross)
			{
				mp_LLine->SetP0(NewPoint);
				return 1;
			}
			if(bOnlyShorten==true)
			{
				if(NewPointLoc >= 0 + dLineHitDistLoc )
					return -1;
				mp_LLine->SetP0(NewPoint);
				return 1;
			}
			if(NewPointLoc > 0. - dLineHitDistLoc && NewPointLoc < 1. + dLineHitDistLoc)
			{
				mp_LLine->SetP0(NewPoint);
				return 1;
			}
		}
		else
		{
			double NewPointLoc = mp_RLine->OrthoBaseLocValue(NewPoint);
			dLineHitDistLoc = MSFloor::ms_LineHitDistTol / mp_RLine->Length();
			if(bOnlyCross)
			{
				mp_RLine->SetP0(NewPoint);
				return 1;
			}
			if(bOnlyShorten==true)
			{
				if(NewPointLoc >= 0 + dLineHitDistLoc )
					return -1;
				mp_RLine->SetP0(NewPoint);
				return 1;
			}
			if(NewPointLoc > 0. - dLineHitDistLoc && NewPointLoc < 1. + dLineHitDistLoc)
			{
				mp_RLine->SetP0(NewPoint);
				return 1;
			}
		}
	}
	else
	{
		if(LineSide=='L')
		{
			double NewPointLoc = mp_LLine->OrthoBaseLocValue(NewPoint);
			dLineHitDistLoc = MSFloor::ms_LineHitDistTol / mp_LLine->Length();
			if(bOnlyCross)
			{
				mp_LLine->SetP1(NewPoint);
				return 1;
			}
			if(bOnlyShorten==true)
			{
				if(NewPointLoc <= 1 - dLineHitDistLoc )
					return -1;
				mp_LLine->SetP1(NewPoint);
				return 1;
			}
			if(NewPointLoc > 0. - dLineHitDistLoc && NewPointLoc < 1. + dLineHitDistLoc)
			{
				mp_LLine->SetP1(NewPoint);
				return 1;
			}
		}
		else
		{
			double NewPointLoc = mp_RLine->OrthoBaseLocValue(NewPoint);
			dLineHitDistLoc = MSFloor::ms_LineHitDistTol / mp_RLine->Length();
			if(bOnlyCross)
			{
				mp_RLine->SetP1(NewPoint);
				return 1;
			}
			if(bOnlyShorten==true)
			{
				if(NewPointLoc <= 1 - dLineHitDistLoc )
					return -1;
				mp_RLine->SetP1(NewPoint);
				return 1;
			}
			if(NewPointLoc > 0. - dLineHitDistLoc&& NewPointLoc < 1. + dLineHitDistLoc)
			{
				mp_RLine->SetP1(NewPoint);
				return 1;
			}
		}
	}
	return 0;
}

void MSLinkMemPart::DeleteTrimLines()
{
    long nLine=ma_LTrimLine.GetSize();
    for(long iLine=0; iLine<nLine; iLine++)
        delete ma_LTrimLine[iLine];
    nLine=ma_RTrimLine.GetSize();
    for(long iLine=0; iLine<nLine; iLine++)
        delete ma_RTrimLine[iLine];
    ma_LTrimLine.RemoveAll();
    ma_RTrimLine.RemoveAll();
}

void MSLinkMemPart::AddEndJointPoly( MSConnLevelZone* MyZone, long StartOrEnd, GM2DPolyline* pPoly )
{
    if(StartOrEnd==0)
    {
        long nLevelZone = ma_StartJointLevel.GetSize();
        for(long iZone=0; iZone<nLevelZone; iZone++)
        {
			MSConnLevelZone* pZone = ma_StartJointLevel[iZone];
			if(pZone == MyZone)
			{
				GM2DPolyline* pCurPoly = ma_StartJointEndPoly[iZone];
				if(pCurPoly != NULL)
					delete pCurPoly;
				ma_StartJointEndPoly[iZone] = GM2DPolyline::CopyOrNull(pPoly);
				break;
			}
            else if(MyZone->m_BotLevel < pZone->m_BotLevel)
            {
				ma_StartJointLevel.InsertAt(iZone, MyZone->CopyOnlyTopBot());
                ma_StartJointEndPoly.InsertAt(iZone, GM2DPolyline::CopyOrNull(pPoly));
                break;
            }
        }
        if(iZone==nLevelZone)
        {
            ma_StartJointLevel.Add(MyZone->CopyOnlyTopBot());
            ma_StartJointEndPoly.Add(GM2DPolyline::CopyOrNull(pPoly));
        }
    }
    else
    {
        long nLevelZone = ma_EndJointLevel.GetSize();
        for(long iZone=0; iZone<nLevelZone; iZone++)
        {
			MSConnLevelZone* pZone = ma_EndJointLevel[iZone];
			if(pZone == MyZone)
			{
				GM2DPolyline* pCurPoly = ma_EndJointEndPoly[iZone];
				if(pCurPoly != NULL)
					delete pCurPoly;
				ma_EndJointEndPoly[iZone] = GM2DPolyline::CopyOrNull(pPoly);
				break;
			}
			else if(MyZone->m_BotLevel < ma_EndJointLevel[iZone]->m_BotLevel)
            {
                ma_EndJointLevel.InsertAt(iZone, MyZone->CopyOnlyTopBot());
                ma_EndJointEndPoly.InsertAt(iZone, GM2DPolyline::CopyOrNull(pPoly));
                break;
            }
        }
        if(iZone==nLevelZone)
        {
            ma_EndJointLevel.Add(MyZone->CopyOnlyTopBot());
            ma_EndJointEndPoly.Add(GM2DPolyline::CopyOrNull(pPoly));
        }
    }
}

void MSLinkMemPart::DeleteEndJointPoly(MSConnLevelZone* DelZone, long StartOrEnd)
{
	long nFindIdx = -1;
	if(StartOrEnd==0)
	{
		long NumLevelZone = ma_StartJointLevel.GetSize();
		for(long iLevel=0; iLevel<NumLevelZone; iLevel++)
		{
			if(ma_StartJointLevel[iLevel] == DelZone)
			{
				nFindIdx = iLevel;
				break;
			}
		}

		if(nFindIdx > -1)
		{
 			if(ma_StartJointEndPoly[nFindIdx] != NULL)
 				delete ma_StartJointEndPoly[nFindIdx];
			ma_StartJointLevel.RemoveAt(nFindIdx);
			ma_StartJointEndPoly.RemoveAt(nFindIdx);
		}
	}
	else
	{
		long NumLevelZone = ma_EndJointLevel.GetSize();
		for(long iLevel=0; iLevel<NumLevelZone; iLevel++)
		{
			if(ma_EndJointLevel[iLevel] == DelZone)
			{
				nFindIdx = iLevel;
				break;
			}
		}

		if(nFindIdx > -1)
		{
 			if(ma_EndJointEndPoly[nFindIdx] != NULL)
 				delete ma_EndJointEndPoly[nFindIdx];
			ma_EndJointLevel.RemoveAt(nFindIdx);
			ma_EndJointEndPoly.RemoveAt(nFindIdx);
		}
	}
}

void MSLinkMemPart::DeleteEndJointPolyAll(long StartOrEnd)
{
    if(StartOrEnd==0)
    {
        long nLevel = ma_StartJointLevel.GetSize();
        for(long iLevel=0; iLevel<nLevel; iLevel++)
        {
//            delete ma_StartJointLevel[iPoint];	//MSConnLevelZone 클래스에서 삭제함
             if(ma_StartJointEndPoly[iLevel] != NULL)
 				delete ma_StartJointEndPoly[iLevel];
        }
        ma_StartJointLevel.RemoveAll();
        ma_StartJointEndPoly.RemoveAll();
    }
    else
    {
        long nLevel = ma_EndJointLevel.GetSize();
        for(long iLevel=0; iLevel<nLevel; iLevel++)
        {
//            delete ma_EndJointLevel[iPoint];
 			if(ma_EndJointEndPoly[iLevel] != NULL)
 				delete ma_EndJointEndPoly[iLevel];
        }
        ma_EndJointLevel.RemoveAll();
        ma_EndJointEndPoly.RemoveAll();
    }
}

bool MSLinkMemPart::IsWallAndGirderWith( MSLinkMemPart* pUrPart )
{
    if(this==NULL || pUrPart==NULL)
        return false;

    if( (mp_FloorPartUp && mp_FloorPartUp==pUrPart) || (mp_WallPartBelow && mp_WallPartBelow==pUrPart) )
        return true;
    return false;
}

GM2DPolyline* MSLinkMemPart::GetMemberBoundary()
{
	if(mp_LinkMem == NULL)
		return NULL;

	GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
	GM2DCurve* pLLine = mp_LLine;
	GM2DCurve* pRLine = mp_RLine;
	
	if(pLLine == NULL || pRLine == NULL) return NULL;

	if(mp_LinkMem->mp_Curve==NULL)
	{
		pPoly->AddCopy(pRLine->m_P0);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(pRLine->m_P1);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(pLLine->m_P1);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(pLLine->m_P0);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(pRLine->m_P0);
	}
	else
	{
		pPoly->AddCopy(pRLine->m_P0);
		pPoly->ma_Bulge.Add(pRLine->GetBulge());
		pPoly->AddCopy(pRLine->m_P1);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(pLLine->m_P1);
		pPoly->ma_Bulge.Add(pLLine->GetBulge() * -1);
		pPoly->AddCopy(pLLine->m_P0);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(pRLine->m_P0);
	}
	return pPoly;
}

GM2DLineVector MSLinkMemPart::GetMemberEndLine( long StartOrEnd)
{
	GM2DLineVector EndLine;
	GM2DVector LVec, RVec;
	double dLoc = 0;

	if(mp_LinkMem == NULL)
		return EndLine;

	GM2DCurve* pLLine = mp_LinkMem->mp_LLine;
	GM2DCurve* pRLine = mp_LinkMem->mp_RLine;

	if(StartOrEnd==0)
	{
		EndLine.m_P0 = pLLine->GetP0();
		EndLine.m_P1 = pRLine->GetP0();
	}
	else
	{

		EndLine.m_P0 = pLLine->GetP1();
		EndLine.m_P1 = pRLine->GetP1();
	}
	return EndLine;
}

GM2DPolyline* MSLinkMemPart::GetBoundary(BOOL bShort /*= TRUE*/)
{
	GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
	GM2DLineVector SLine, ELine;
	SLine = GetEndLine(0 , bShort);
	ELine = GetEndLine(1 , bShort);
	
	//P0 = LLine,  P1 = RLine
	pPoly->AddCopy(SLine.m_P1);
	pPoly->ma_Bulge.Add(mp_RLine->GetBulge());
	pPoly->AddCopy(ELine.m_P1);
	pPoly->ma_Bulge.Add(0.0);
	pPoly->AddCopy(ELine.m_P0);
	pPoly->ma_Bulge.Add(mp_LLine->GetBulge() * -1);
	pPoly->AddCopy(SLine.m_P0);
	pPoly->ma_Bulge.Add(0.0);
	pPoly->AddCopy(SLine.m_P1);

	pPoly->MakeCCW();

	return pPoly;
}

GM2DPolyline* MSLinkMemPart::GetBoundary2(GM2DCurve* pBaseCurve)
{
	mp_Boundary->ResetPolyline();
	if(pBaseCurve == NULL)
	{
		mp_Boundary->AddCopy(mp_RLine->m_P0);
		mp_Boundary->ma_Bulge.Add(mp_RLine->GetBulge());
		mp_Boundary->AddCopy(mp_RLine->m_P1);
		mp_Boundary->ma_Bulge.Add(0.0);
		mp_Boundary->AddCopy(mp_LLine->m_P1);
		mp_Boundary->ma_Bulge.Add(mp_LLine->GetBulge() * -1);
		mp_Boundary->AddCopy(mp_LLine->m_P0);
		mp_Boundary->ma_Bulge.Add(0.0);
		mp_Boundary->AddCopy(mp_RLine->m_P0);
	}
	else
	{
		GM2DCurve* pLLine = mp_LLine->Copy();
		GM2DCurve* pRLine = mp_RLine->Copy();

		pLLine->SetP0(mp_LLine->OrthoBaseVector(pBaseCurve->m_P0));
		pLLine->SetP1(mp_LLine->OrthoBaseVector(pBaseCurve->m_P1));
		pRLine->SetP0(mp_RLine->OrthoBaseVector(pBaseCurve->m_P0));
		pRLine->SetP1(mp_RLine->OrthoBaseVector(pBaseCurve->m_P1));

		mp_Boundary->AddCopy(pRLine->m_P0);
		mp_Boundary->ma_Bulge.Add(pRLine->GetBulge());
		mp_Boundary->AddCopy(pRLine->m_P1);
		mp_Boundary->ma_Bulge.Add(0.0);
		mp_Boundary->AddCopy(pLLine->m_P1);
		mp_Boundary->ma_Bulge.Add(pLLine->GetBulge() * -1);
		mp_Boundary->AddCopy(pLLine->m_P0);
		mp_Boundary->ma_Bulge.Add(0.0);
		mp_Boundary->AddCopy(pRLine->m_P0);

		delete pLLine;
		delete pRLine;
	}

	mp_Boundary->MakeCCW();
	return mp_Boundary;
}

GM2DPolyline* MSLinkMemPart::GetTrimmedBoundary()
{
    return mp_TrimmedBoundary;
}

void MSLinkMemPart::Delete()
{
	//if (m_bDeleted) ASSERT(0);
	m_bDeleted = true;
	_deleted_map[this] = true;
}

void MSLinkMemPart::ClearAllDeletedMemParts()
{
	map<MSLinkMemPart*, bool>::iterator it;
	for(it = _deleted_map.begin(); it != _deleted_map.end(); it++)
		delete it->first;
	_deleted_map.clear();
}

bool MSLinkMemPart::isBuriedLinkMember()
{
	if(m_bDeleted == true)
		return true;

	if(mp_LinkMem == NULL)
		return true;
	return mp_LinkMem->m_isBuriedLinkMember;
}

// void MSLinkMemPart::MakeTrimmedPolyline()
// {
//     // reset boundary
//     mp_TrimmedBoundary->ResetPolyline();
// 
//     // for initial untrimmed boundary
//     if(mp_LPolyLine->GetSize()==0)
//     {
//         GetBoundary2()->CopyTo(mp_TrimmedBoundary);
//         return;
//     }
// 
//     double bulge = 0.0;
//     if(mp_LinkMem->isArc())
//     {
//         bulge = mp_LinkMem->mp_LLine->GetBulge();
//     }
// 
//     MSLinkMemPart* pPart;
//     long NumPart = mp_LinkMem->ma_MemPart.size();
//     for(long iPart = 0; iPart<NumPart; iPart++)
//     {
//         pPart = mp_LinkMem->ma_MemPart[iPart];
//         long NumSeg = pPart->ma_LSegments.size();
//         for (long jSeg = 0; jSeg<NumSeg; jSeg++)
//         {
//             GM2DCurve* pSeg = (pPart->ma_LSegments)[jSeg];
//             mp_TrimmedBoundary->AddCopy(pSeg->m_P0);
//             mp_TrimmedBoundary->ma_Bulge.Add(bulge);
//             mp_TrimmedBoundary->AddCopy(pSeg->m_P1);
//             mp_TrimmedBoundary->ma_Bulge.Add(bulge);
//         }
//     }
//     int count = mp_TrimmedBoundary->ma_Bulge.GetSize();
//     mp_TrimmedBoundary->ma_Bulge[count-1] = 0.0;
//     for(long iPart = 0; iPart<NumPart; iPart++)
//     {
//         pPart = mp_LinkMem->ma_MemPart[NumPart-iPart-1];
//         long NumSeg = pPart->ma_RSegments.size();
//         for (long jSeg = 0; jSeg<NumSeg; jSeg++)
//         {
//             GM2DCurve* pSeg = (pPart->ma_RSegments)[jSeg];
//             mp_TrimmedBoundary->AddCopy(pSeg->m_P1);
//             mp_TrimmedBoundary->ma_Bulge.Add(-bulge);
//             mp_TrimmedBoundary->AddCopy(pSeg->m_P0);
//             mp_TrimmedBoundary->ma_Bulge.Add(-bulge);
//         }
//     }
//     count = mp_TrimmedBoundary->ma_Bulge.GetSize();
//     mp_TrimmedBoundary->ma_Bulge[count-1] = 0.0;
// 
//     mp_TrimmedBoundary->DeleteInLinePoint();
//     mp_TrimmedBoundary->MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5
//     mp_TrimmedBoundary->MakeCCW();
// }


// ---- for trim solution 3 ----

void MSLinkMemPart::SetSegments()
{
    // check to initialize trimmed polyline by part default boundary
    
    // for LSegments
    if(ma_LSegments.size() == 0)
        ma_LSegments.push_back(mp_LLine->Copy());
    
    // for RSegments
    if(ma_RSegments.size() == 0)
        ma_RSegments.push_back(mp_RLine->Copy());
    
    // for SSegments
    if(ma_SSegments.size() == 0 && IsStart() && mp_SConn->HasOnlyOneMemPart())
        ma_SSegments.push_back(new GM2DLineVector(mp_LLine->m_P0, mp_RLine->m_P0));

    // for ESegments
    if(ma_ESegments.size() == 0 && IsEnd() && mp_EConn->HasOnlyOneMemPart())
        ma_ESegments.push_back(new GM2DLineVector(mp_LLine->m_P1, mp_RLine->m_P1));
}

bool MSLinkMemPart::IsStart()
{
	return false;
    //return (mp_SConn);
}

bool MSLinkMemPart::IsEnd()
{
    return false;
	//return (mp_EConn);
}

GM2DCurve* MSLinkMemPart::GetShortLRLine(long StartOrEnd)
{
	double dLLoc = 0;
	double dRLoc = 0;
	if(StartOrEnd == 0)
	{
		dLLoc = mp_CenLine->OrthoBaseLocValue(mp_LLine->m_P0);
		dRLoc = mp_CenLine->OrthoBaseLocValue(mp_RLine->m_P0);
		if(dLLoc > dRLoc)
			return mp_LLine;
		else
			return mp_RLine;

	}
	else
	{
		dLLoc = mp_CenLine->OrthoBaseLocValue(mp_LLine->m_P1);
		dRLoc = mp_CenLine->OrthoBaseLocValue(mp_RLine->m_P1);
		if(dLLoc < dRLoc)
			return mp_LLine;
		else
			return mp_RLine;
	}
}

GM2DCurve* MSLinkMemPart::GetLongLRLine(long StartOrEnd)
{
	double dLLoc = 0;
	double dRLoc = 0;
	if(StartOrEnd == 0)
	{
		dLLoc = mp_CenLine->OrthoBaseLocValue(mp_LLine->m_P0);
		dRLoc = mp_CenLine->OrthoBaseLocValue(mp_RLine->m_P0);
		if(dLLoc < dRLoc)
			return mp_LLine;
		else
			return mp_RLine;

	}
	else
	{
		dLLoc = mp_CenLine->OrthoBaseLocValue(mp_LLine->m_P1);
		dRLoc = mp_CenLine->OrthoBaseLocValue(mp_RLine->m_P1);
		if(dLLoc > dRLoc)
			return mp_LLine;
		else
			return mp_RLine;
	}
}

