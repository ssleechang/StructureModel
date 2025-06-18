#include "StdAfx.h"
#include "MSSlabRebar.h"

#include "MSPolylineObject.h"

#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSMaterial.h"
#include "MSStoryMaterial.h"
#include "MSModelChecker.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(MSSlabRebar, MSObject, VERSIONABLE_SCHEMA | 1)

// long MSSlabRebar::ms_LastID = 0;
MSSlabRebar::MSSlabRebar(bool bReverse)
{
	m_ID = 0;
	m_pFloor = NULL;

	m_bReverse = bReverse;

	mp_PolylineInfo = new MSPolylineObject;

	m_isPolylineType = false;
	m_dLxDeg = 0.0;

	m_nMaterialID = 0;
	m_dOffset = 0;

	m_IsUsedStoryMaterial = true;
	m_IsStartHook = false;
	m_IsEndHook = false;

	mp_RebarInfo= NULL;

	m_dStartCover = 0;
	m_dEndCover = 0;
	m_nTopCoverType = 0;
	m_dTopCover = 0;
	m_nBotCoverType = 0;
	m_dBotCover = 0;
	m_eSlabRebarType = MSSlabRebar::Slab;

	IsClipBoardSerialize = false;
}

MSSlabRebar::~MSSlabRebar()
{
	if (mp_RebarInfo)	delete mp_RebarInfo;
	DeleteBentCutLines();
	ma_BentLocValue.clear();
	m_BentLine.ResetPolyline();
}

void MSSlabRebar::DeleteBentCutLines()
{
	int nBentCut = ma_BentCutLines.size();
	for (int iBentCut = 0; iBentCut < nBentCut; iBentCut++)
		delete ma_BentCutLines[iBentCut];
	ma_BentCutLines.clear();
}

bool MSSlabRebar::AddBentPoint(GM2DVector& BentPoint)
{
	double LocValue = m_RebarLine.OrthoBaseLocValue(BentPoint);
	return AddBentLocValue(LocValue);
}

bool MSSlabRebar::AddBentLocValue(double LocValue)
{
	if (LocValue<-DTOL_GM || LocValue>1.0 + DTOL_GM)
		return false;

	long nValue = ma_BentLocValue.size();
	for (long iValue = 0; iValue<nValue; iValue++)
	{
		if (LocValue<ma_BentLocValue[iValue])
		{
			ma_BentLocValue.insert(ma_BentLocValue.begin() + iValue, LocValue);
			ma_BentCutLines.insert(ma_BentCutLines.begin() + iValue, GetBentCutLine(LocValue));
			break;
		}
	}
	if (iValue == nValue)
	{
		ma_BentLocValue.push_back(LocValue);
		ma_BentCutLines.push_back(GetBentCutLine(LocValue));
	}

	return true;
}

bool MSSlabRebar::AddBentLocations(vector<double> Locs)
{
	ma_BentLocValue.clear();
	ma_BentLocValue = Locs;
	return true;
}

GM2DLineVector* MSSlabRebar::GetBentCutLine(double LocValue)
{
	GM2DVector dir1 = m_RebarLine.PointAtParam(LocValue);
	GM2DVector dir2 = m_RebarZone.OrthoBaseVector(dir1);
	GM2DVector direction = dir1 - dir2;

	GM2DVector temp0 = m_RebarZone.m_P0 + direction;
	GM2DVector temp1 = m_RebarZone.m_P1 + direction;

	return new GM2DLineVector(temp0, temp1);
}

void MSSlabRebar::FixBentLocByLastPntToCenPnt()
{
	double dHalfOffset = 250.; //
	for (long iLoc = 0; iLoc < ma_BentLocValue.size(); iLoc++)
		ma_BentLocValue[iLoc] = ma_BentLocValue[iLoc] - dHalfOffset;
}

void MSSlabRebar::CaluateRebarOrigin()
{
	MSSlabRebarInfo* pRebarInfo = GetRebarInfo();
	if (pRebarInfo->GetRebarType() != MSSlabRebarInfo::Bent)
		return;

	// 	GM2DPolyline* profile = GetProfile();
	// 	if(profile->Contains(&m_RebarOrigin) == 0)	// 변경된 폴리라인의 선상에 존재
	// 		return;

	GM2DLineVector Line(m_RebarOrigin - (m_Direction * 100000), m_RebarOrigin + (m_Direction * 100000));
	GM2DVector* pNewOrigin = GetFirstIntersectedPoint(Line, *GetProfile());
	// 기준점이 새 폴리라인에 포함된 경우
	//if(pNewOrigin != NULL)
	//{
	//	if(profile->Contains(pNewOrigin) > 0)	// 변경된 폴리라인의 선상에 존재
	//	{
	m_RebarOrigin.SetXY(pNewOrigin->m_X, pNewOrigin->m_Y);
	//	}
	//	else
	//	{
	//	}
	//}
	delete pNewOrigin;
}

bool MSSlabRebar::IsValidBentLine()
{
	MSSlabRebarInfo* pRebarInfo = GetRebarInfo();
	if (pRebarInfo->GetRebarType() != MSSlabRebarInfo::Bent)
		return true;

	long NumVec = m_BentLine.GetSize();
	if (NumVec == 0)
		return false;

	GM2DPolyline* pRebarPoly = GetProfile();

	long nRVal = pRebarPoly->Contains(&m_BentLine.FirstPnt());
	if (nRVal > 0)
		return false;

	nRVal = pRebarPoly->Contains(&m_BentLine.LastPnt());
	if (nRVal > 0)
		return false;

	return true;
}

void MSSlabRebar::CalculateBentLine()
{
	MSSlabRebarInfo* pRebarInfo = GetRebarInfo();
	if (pRebarInfo->GetRebarType() != MSSlabRebarInfo::Bent)
		return;

	m_BentLine.ResetPolyline();
	GM2DVector normal = m_Normal.UnitVector();

	if (m_bReverse)
		normal.ReverseDirection();
	GM2DVector* pRebarOrigin = new GM2DVector(m_RebarOrigin);
	m_BentLine.Add(pRebarOrigin);
	m_BentLine.ma_Bulge.Add(0.0); ////

	double previous = 0.0;
	double offset = _BENT_OFFSET; //
	for (int i = 0; i<ma_BentLocValue.size(); i++)
	{
		double location = ma_BentLocValue[i];
		double length = location - previous - (offset / 2);
		m_BentLine.Add(new GM2DVector(*(m_BentLine.GetAt(m_BentLine.GetSize() - 1)) + m_Direction *length));
		m_BentLine.ma_Bulge.Add(0.0); ////
		previous = location + (offset / 2);

		normal.ReverseDirection();
		m_BentLine.Add(new GM2DVector(*(m_BentLine.GetAt(m_BentLine.GetSize() - 1)) + (m_Direction + normal) *offset));
		m_BentLine.ma_Bulge.Add(0.0); ////
	}

	// find end point
	GM2DVector* pCrossPt = GetFirstIntersectedPoint(*(m_BentLine.GetAt(m_BentLine.GetSize() - 1)), m_Direction, *GetProfile());
	if (pCrossPt != NULL)
		m_BentLine.Add(pCrossPt);

	// rebar line
	pCrossPt = GetFirstIntersectedPoint(m_RebarOrigin, m_Direction, *GetProfile());
	if (pCrossPt != NULL)
	{
		m_RebarLine.m_P0 = m_RebarOrigin;
		m_RebarLine.m_P1 = *pCrossPt;
		delete pCrossPt;
	}

	// info line
	GM2DVector* pCrossPt1 = NULL;
	GM2DVector* pCrossPt2 = NULL;
	GM2DVector midPt = m_RebarLine.PointAtParam(0.5);
	pCrossPt1 = GetFirstIntersectedPoint(midPt, m_Normal, *GetProfile());
	pCrossPt2 = GetFirstIntersectedPoint(midPt, m_Normal*(-1.0), *GetProfile());

	m_RebarZone.m_P0 = *pCrossPt1;
	m_RebarZone.m_P1 = *pCrossPt2;
	delete pCrossPt1;
	delete pCrossPt2;
}

GM2DVector* MSSlabRebar::GetFirstIntersectedPoint(GM2DVector Origin, GM2DVector Direction, GM2DPolyline Poly)
{
	// find end point
	GM2DLineVector Line(Origin, Origin + Direction * 100000);
	return GetFirstIntersectedPoint(Line, Poly);
}


GM2DVector* MSSlabRebar::GetFirstIntersectedPoint(GM2DLineVector Line, GM2DPolyline Poly)
{
	GM2DVector* Pt = NULL;
	CTypedPtrArray<CObArray, GM2DLineVector*> Segments;
	Line.GetSegments(Segments, &Poly);
	if (Segments.GetSize()>0)
		Pt = new GM2DVector(Segments[0]->m_P1);

	// delete
	for (int i = 0; i<Segments.GetSize(); i++)
		delete Segments[i];
	Segments.RemoveAll();

	return Pt;
}

bool MSSlabRebar::CheckRebarLine(bool bMakeErrorObject)
{
	bool HasError = false;
	MSSlabRebarInfo* pRebarInfo = GetRebarInfo();
	HasError = pRebarInfo->CheckReBarStr();

	if (bMakeErrorObject == true && HasError == true)
	{
		CString strGroup = _T("슬래브 보강근");
		CString strErrMsg;
		strErrMsg.Format(_T("%s : 슬래브 보강근(%d) 정보가 변경 되었습니다."), GetFloorNameForUser(), m_ID);
		MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 0, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_UNKNOWN, this);
	}

	return HasError;
}

double MSSlabRebar::GetLocValueByType(MSSlabRebarInfo::RebarType type)
{
	double LocValue = 0.5;

	if (type == MSSlabRebarInfo::TopX)				LocValue = 0.4;
	if (type == MSSlabRebarInfo::BottomX)		LocValue = 0.3;
	if (type == MSSlabRebarInfo::TopY)				LocValue = 0.6;
	if (type == MSSlabRebarInfo::BottomY)		LocValue = 0.7;
	if (type == MSSlabRebarInfo::Shear)				LocValue = 0.45;
	return LocValue;
}

bool MSSlabRebar::isXDirection()
{
	GM2DLineVector Line(m_RebarOrigin - (m_Direction * 100000), m_RebarOrigin + (m_Direction * 100000));

	double dDeg = Line.GetDegree();
	if (dDeg >= 0 && dDeg <= 45 || dDeg >= 135 && dDeg <= 225 || dDeg >= 315 && dDeg <= 360)
		return true;

	return false;
}

void MSSlabRebar::CalulateRebarLineRebarZone()
{
	MSSlabRebarInfo* pRebarInfo = GetRebarInfo();

	if (pRebarInfo->GetRebarType() == MSSlabRebarInfo::Bent)	return;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Re-Make RebarZone, RebarLine, InfoLine
	GM2DPolyline* profile = GetProfile();
	bool IsRectangle = profile->IsRectangle();
	if (IsRectangle)	m_isPolylineType = false;
	else
		m_isPolylineType = true;

	GM2DVector cenVec = profile->MassCenter();
	GM2DVector* pCrossPt1 = GetFirstIntersectedPoint(cenVec, m_Direction*(-1.0), *GetProfile());
	GM2DVector* pCrossPt2 = GetFirstIntersectedPoint(cenVec, m_Direction, *GetProfile());

	m_RebarLine.m_P0 = *pCrossPt1;
	m_RebarLine.m_P1 = *pCrossPt2;
	delete pCrossPt1;	delete pCrossPt2;

	GM2DVector midPt = m_RebarLine.PointAtParam(0.5);
	pCrossPt1 = GetFirstIntersectedPoint(midPt, m_Normal, *GetProfile());
	pCrossPt2 = GetFirstIntersectedPoint(midPt, m_Normal*(-1.0), *GetProfile());

	m_RebarZone.m_P0 = *pCrossPt1;
	m_RebarZone.m_P1 = *pCrossPt2;
	delete pCrossPt1;
	delete pCrossPt2;

	double degree = m_RebarLine.GetDegree();
	return;

	if (!m_isPolylineType && profile->ma_Point.GetSize() >= 4)
	{
		double LocValue = GetLocValueByType(pRebarInfo->GetRebarType());

		GM2DLineVector yline(GetMidPoint(profile->ma_Point[0], profile->ma_Point[1]), GetMidPoint(profile->ma_Point[2], profile->ma_Point[3]));
		GM2DLineVector xline(GetMidPoint(profile->ma_Point[0], profile->ma_Point[3]), GetMidPoint(profile->ma_Point[1], profile->ma_Point[2]));
		if (pRebarInfo->GetRebarType() == MSSlabRebarInfo::TopX || pRebarInfo->GetRebarType() == MSSlabRebarInfo::BottomX)
		{
			MakeRebarLine(GM2DLineVector(*profile->ma_Point[0], *profile->ma_Point[3]), GM2DLineVector(*profile->ma_Point[1], *profile->ma_Point[2]), LocValue);
			m_RebarZone = yline;
		}
		else
		{
			MakeRebarLine(GM2DLineVector(*profile->ma_Point[0], *profile->ma_Point[1]), GM2DLineVector(*profile->ma_Point[3], *profile->ma_Point[2]), LocValue);
			m_RebarZone = xline;
		}
	}
	else
	{
		GM2DVector* pEndVec = GetFirstIntersectedPoint(m_RebarOrigin, m_Direction, *GetProfile());
		if (pEndVec != NULL)
		{
			m_RebarLine.m_P0 = m_RebarOrigin;
			m_RebarLine.m_P1 = *pEndVec;
			delete pEndVec;

			GM2DVector midPt = m_RebarLine.PointAtParam(0.5);
			GM2DVector* pCrossPt1 = GetFirstIntersectedPoint(midPt, m_Normal, *GetProfile());
			GM2DVector* pCrossPt2 = GetFirstIntersectedPoint(midPt, m_Normal*(-1.0), *GetProfile());

			m_RebarZone.m_P0 = *pCrossPt1;
			m_RebarZone.m_P1 = *pCrossPt2;
			delete pCrossPt1;
			delete pCrossPt2;

		}
	}
}

GM2DVector MSSlabRebar::GetMidPoint(GM2DVector* p1, GM2DVector* p2)
{
	double x = (p1->m_X + p2->m_X) * 0.5;
	double y = (p1->m_Y + p2->m_Y) * 0.5;
	GM2DVector ret(x, y);
	return ret;
}

void MSSlabRebar::MakeRebarLine(GM2DLineVector& line1, GM2DLineVector& line2, double LocValue)
{
	GM2DLineVector LineVec1(line1.GetP0(), line1.GetP1());
	GM2DVector LocVec1 = LineVec1.PointAtParam(LocValue);

	GM2DLineVector LineVec2(line2.GetP0(), line2.GetP1());
	GM2DVector LocVec2 = LineVec2.PointAtParam(LocValue);

	m_RebarLine.m_P0 = LocVec1;
	m_RebarLine.m_P1 = LocVec2;
}

vector<double> MSSlabRebar::GetBentDistancesFromStart()
{
	vector<double> distances;
	long nLoc = ma_BentLocValue.size();
	for (long iLoc = 0; iLoc<nLoc; iLoc++)
		distances.push_back(m_RebarLine.Length()*ma_BentLocValue[iLoc]);
	return distances;
}

vector<GM2DVector> MSSlabRebar::GetBentPoints()
{
	vector<GM2DVector> bentPoints;
	GM2DLineVector offsetRebarLine = m_RebarLine.NormalTranslate(-_BENT_OFFSET / 2);
	double length = offsetRebarLine.Length();
	long nLoc = ma_BentLocValue.size();
	for (long iLoc = 0; iLoc<nLoc; iLoc++)
	{
		double bentLength = ma_BentLocValue[iLoc];
		GM2DVector locVec = offsetRebarLine.PointVector(bentLength / length);
		bentPoints.push_back(locVec);
	}
	return bentPoints;
}

GM2DLineVector MSSlabRebar::GetRebarCenterLine()
{
	MSSlabRebarInfo* pRebarInfo = GetRebarInfo();
	if (pRebarInfo->GetRebarType() != MSSlabRebarInfo::Bent)
		return m_RebarLine;
	else
	{
		GM2DLineVector cenline = m_RebarLine.NormalTranslate(-_BENT_OFFSET / 2);
		return cenline;
	}
}

GM2DPolyline* MSSlabRebar::GetProfile()
{
	return mp_PolylineInfo->GetProfile();
}

void MSSlabRebar::Dump(EFS::EFS_Buffer& ar)
{
	// mylee - 일단 Undo/Redo 에서 꼭 필요한 정보만 Dump/Recovery
	MSElement::Dump(ar);
	mp_PolylineInfo->Dump(ar);

	GetRebarInfo()->Dump(ar);

	int nBentLoc = ma_BentLocValue.size();
	ar << nBentLoc;
	for (int iBentLoc = 0; iBentLoc < nBentLoc; iBentLoc++)
	{
		ar << ma_BentLocValue[iBentLoc];
	}

	GM2DLineVector* pLineVec = &m_RebarLine;
	GM2DLineVector::Dump(ar, pLineVec);
	pLineVec = &m_RebarZone;
	GM2DLineVector::Dump(ar, pLineVec);

	ar << m_dLxDeg;
	m_BentLine.Dump(ar);
	m_Direction.Dump(ar);
	m_Normal.Dump(ar);
	m_RebarOrigin.Dump(ar);

	ar << m_nMaterialID;
	ar << m_dOffset;
	ar << m_IsUsedStoryMaterial;
	ar << m_IsStartHook;
	ar << m_IsEndHook;

	ar << m_dStartCover;
	ar << m_dEndCover;
	ar << m_nTopCoverType;
	ar << m_dTopCover;
	ar << m_nBotCoverType;
	ar << m_dBotCover;

	int slabRebarType = (int)m_eSlabRebarType;
	ar << slabRebarType;
}

void MSSlabRebar::Recovery(EFS::EFS_Buffer& ar)
{
	// mylee - 일단 Undo/Redo 에서 꼭 필요한 정보만 Dump/Recovery
	MSElement::Recovery(ar);
	mp_PolylineInfo->Recovery(ar);

	GetRebarInfo()->Recovery(ar);
	int nBentLoc = 0;
	double Loc = 0;
	ma_BentLocValue.clear();
	ar >> nBentLoc;
	for (int iBentLoc = 0; iBentLoc < nBentLoc; iBentLoc++)
	{
		ar >> Loc;
		ma_BentLocValue.push_back(Loc);
	}

	GM2DLineVector* pLineVec = &m_RebarLine;
	GM2DLineVector::Recovery(ar, pLineVec);
	pLineVec = &m_RebarZone;
	GM2DLineVector::Recovery(ar, pLineVec);

	ar >> m_dLxDeg;
	m_BentLine.Recovery(ar);
	m_Direction.Recovery(ar);
	m_Normal.Recovery(ar);
	m_RebarOrigin.Recovery(ar);

	ar >> m_nMaterialID;
	ar >> m_dOffset;
	ar >> m_IsUsedStoryMaterial;
	ar >> m_IsStartHook;
	ar >> m_IsEndHook;

	ar >> m_dStartCover;
	ar >> m_dEndCover;
	ar >> m_nTopCoverType;
	ar >> m_dTopCover;
	ar >> m_nBotCoverType;
	ar >> m_dBotCover;
	int slabRebarType = 0;
	ar >> slabRebarType;
	m_eSlabRebarType = (eSlabRebarType)slabRebarType;
}

void MSSlabRebar::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	GM2DPolyline Temp(TRUE);

	bool isBasic, isTopbar, isBotbar;
	MSStirrupBar tempSBars;
	int slabRebarType = 0;
	if (ar.IsStoring())
	{
		// New Data 2014.07.11
		m_RebarZone.Serialize(ar);
		m_RebarLine.Serialize(ar);

		int nLoc = ma_BentLocValue.size();
		ar << nLoc;
		for (int iLoc = 0; iLoc < nLoc; iLoc++)
		{
			ar << ma_BentLocValue[iLoc];
		}

		int nBentCutline = ma_BentCutLines.size();
		ar << nBentCutline;
		for (int iLine = 0; iLine < nBentCutline; iLine++)
			ma_BentCutLines[iLine]->Serialize(ar);
		ar << m_bReverse;

		ar << m_isPolylineType;
		Temp.Serialize(ar);
		ar << m_dLxDeg;

		mp_PolylineInfo->Serialize(ar);
		m_RebarOrigin.Serialize(ar);

		ar << m_nMaterialID;
		ar << m_dOffset;
		ar << m_IsUsedStoryMaterial;
		ar << m_IsStartHook;
		ar << m_IsEndHook;

		GetRebarInfo()->Serialize(ar);

		ar << m_dStartCover;
		ar << m_dEndCover;
		ar << m_nTopCoverType;
		ar << m_dTopCover;
		ar << m_nBotCoverType;
		ar << m_dBotCover;

		slabRebarType = (int)m_eSlabRebarType;
		ar << slabRebarType;
	}
	else
	{
		if (MSVersionInfo::GetCurrentVersion() < 20140721) // mylee - for old version
		{
			ar >> isBasic;
			ar >> isTopbar;
			ar >> isBotbar;

			int NumBentLoc = 0;
			ar >> NumBentLoc;
			for (int iBentLoc = 0; iBentLoc < NumBentLoc; iBentLoc++)
			{
				double dBentLoc = 0;
				ar >> dBentLoc;
				ma_BentLocValue.push_back(dBentLoc);
			}
			tempSBars.Serialize(ar);
		}

		m_RebarZone.Serialize(ar);
		m_RebarLine.Serialize(ar);
		SetZoneProfile();

		if (MSVersionInfo::GetCurrentVersion() >= 20140711)
		{
			int nLoc = 0;
			ar >> nLoc;
			for (int iLoc = 0; iLoc < nLoc; iLoc++)
			{
				double dLoc = 0.0;
				ar >> dLoc;
				ma_BentLocValue.push_back(dLoc);
			}

			if (MSVersionInfo::GetCurrentVersion() < 20160621)
			{
				int nInfo = 0;
				ar >> nInfo;
				for (int iInfo = 0; iInfo < nInfo; iInfo++)
				{
					GM2DLineVector InfoLine;
					InfoLine.Serialize(ar);
				}

				int nRebar = 0;
				ar >> nRebar;
				for (int iRebar = 0; iRebar < nRebar; iRebar++)
				{
					GetRebarInfo()->Serialize(ar);
				}
			}

			int nBentCutline = 0;
			ar >> nBentCutline;
			for (int iLine = 0; iLine < nBentCutline; iLine++)
			{
				GM2DLineVector* pLineVec = new GM2DLineVector();
				pLineVec->Serialize(ar);
				ma_BentCutLines.push_back(pLineVec);
			}
			ar >> m_bReverse;

		}

		if (MSVersionInfo::GetCurrentVersion() >= 20140929)
		{
			ar >> m_isPolylineType;
			Temp.Serialize(ar);
			ar >> m_dLxDeg;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20141203)
		{
			mp_PolylineInfo->GetProfile()->ResetPolyline();
			mp_PolylineInfo->Serialize(ar);
			if (MSVersionInfo::GetCurrentVersion() < 20141208)
				CorrectedProfile();
			m_RebarOrigin.Serialize(ar);
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150327)
			ar >> m_nMaterialID;
		if (MSVersionInfo::GetCurrentVersion() >= 20150328)
			ar >> m_dOffset;

		// default : m_IsUsedStoryMaterial는 true 이전 데이터는 false로 설정
		// MSCompoM::Serialize 참고
		if (MSVersionInfo::GetCurrentVersion() >= 20150720)
			ar >> m_IsUsedStoryMaterial;
		else
			m_IsUsedStoryMaterial = false;

		SetRebarDirection(m_dLxDeg);
		if (MSVersionInfo::GetCurrentVersion() < 20151026)
			FixBentLocByLastPntToCenPnt();

		if (MSVersionInfo::GetCurrentVersion() >= 20151204)
		{
			ar >> m_IsStartHook;
			ar >> m_IsEndHook;
		}
		//CheckRebarLine(true);
		if (MSVersionInfo::GetCurrentVersion() >= 20160621)
		{
			GetRebarInfo()->Serialize(ar);
			CalulateRebarLineRebarZone();
		}

		if (MSVersionInfo::GetCurrentVersion() < 20160621)
		{
			if (!GetPolylineType() && (GetRebarInfo()->GetRebarType() == MSSlabRebarInfo::TopY || GetRebarInfo()->GetRebarType() == MSSlabRebarInfo::BottomY))
			{
				m_dLxDeg += 90;
				SetRebarDirection(m_dLxDeg);
				CalulateRebarLineRebarZone();
			}
		}

		if (m_RebarLine.Length() < DTOL1)
			CalulateRebarLineRebarZone();
		CalculateBentLine();

		if (MSVersionInfo::GetCurrentVersion() >= 20170406)
		{
			ar >> m_dStartCover;
			ar >> m_dEndCover;
			ar >> m_nTopCoverType;
			ar >> m_dTopCover;
			ar >> m_nBotCoverType;
			ar >> m_dBotCover;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170823)
		{
			ar >> slabRebarType;
			m_eSlabRebarType = (eSlabRebarType)slabRebarType;
		}

		if (IsClipBoardSerialize == false)
		{
			MSMaterial* pMat = GetFloor()->GetBuilding()->GetMaterial(m_nMaterialID);
			if (pMat == nullptr)	SetMaterialID(GetFloor()->GetBuilding()->GetDefMaterial()->m_ID);
		}
	}
}

bool MSSlabRebar::IsValidProfile()
{
	if (mp_PolylineInfo == NULL)
		return false;
	GM2DPolyline* pProfile = GetProfile();
	if (pProfile == NULL)
		return false;

	if (pProfile->ma_Point.GetSize() <= 2)
		return false;

 	if (!pProfile->CheckPolygon(false))
 		return false;
	return true;
}

void MSSlabRebar::CorrectedProfile()
{
	if (mp_PolylineInfo == NULL)
		return;
	GM2DPolyline* pProfile = GetProfile();
	if (pProfile == NULL)
		return;

	// 1. 중복된 Point를 삭제하고 해당 인덱스를 NULL로 설정
	int NumPoint = pProfile->ma_Point.GetSize();
	for (int iPoint = 0; iPoint < NumPoint; iPoint++)
	{
		GM2DVector* pIVec = pProfile->ma_Point[iPoint];
		for (int jPoint = iPoint + 1; jPoint < NumPoint; jPoint++)
		{
			GM2DVector* pJVec = pProfile->ma_Point[jPoint];
			if (pJVec == NULL)	continue;
			if (pIVec->SameVector4DTOL(*pJVec, 1.0))
			{
				delete pJVec;
				pProfile->ma_Point[jPoint] = NULL;
			}
		}
	}

	// 2. NULL로 설정된 Point Remove
	for (iPoint = NumPoint - 1; iPoint >= 0; iPoint--)
	{
		GM2DVector* pVec = pProfile->ma_Point[iPoint];
		if (pVec == NULL)
			pProfile->ma_Point.RemoveAt(iPoint);
	}

	NumPoint = pProfile->ma_Point.GetSize();
	if (NumPoint == 0)
		return;

	// 3. 마지막 Point에 첫번째 Point를 추가
	GM2DVector* pFirstVec = pProfile->ma_Point[0];
	GM2DVector* pLastVec = new GM2DVector(*pFirstVec);
	pProfile->ma_Point.Add(pLastVec);

	// 4. 정리된 Points수에 맞게 Bulge설정(Arc형상은 고려하지 않는다.)
	NumPoint = pProfile->ma_Point.GetSize();
	pProfile->ma_Bulge.RemoveAll();
	for (int iPoint = 0; iPoint < NumPoint - 1; iPoint++)
		pProfile->ma_Bulge.Add(0.0);
}

bool MSSlabRebar::CheckOverlap(MSSlabRebar* pUrSlabRebar)
{
	if (this == pUrSlabRebar)
		return false;

	if (fabs(m_dOffset - pUrSlabRebar->GetOffset()) > DTOL1)	return false;
	if (!GetRebarInfo()->IsSameBar(pUrSlabRebar->GetRebarInfo()))	return false;

	if (m_RebarLine.IsParallel4DTol(pUrSlabRebar->m_RebarLine) == 0)
		return false;
	
	GM2DPolyline* pProfile = GetProfile();
	if (pProfile == nullptr)
		return false;

	if (pProfile->SamePolyLine(pUrSlabRebar->GetProfile()) == false)
		return false;

	return true;
}

bool MSSlabRebar::SetZoneProfile(GM2DPolyline* pZoneProfile/*=NULL*/)
{
	if (pZoneProfile)
	{
		m_RebarZone.m_P0.SetXY(0., 0.);
		m_RebarZone.m_P1.SetXY(0., 0.);
		SetProfile(pZoneProfile);
		delete pZoneProfile;
		return true;
	}
	else
	{
		// commented this code because of TopY, BottomY - 20150122 mylee
		/*
		if(m_RebarZone.Length() < DTOL_GM || m_RebarLine.Length() < DTOL_GM)
		return false;

		GM2DVector CrossVec;
		double MyLoc, UrLoc;
		long Result = m_RebarLine.IntersectEndWithLine(m_RebarZone, CrossVec, &MyLoc, &UrLoc);
		if(Result != 1)
		return false;

		GM2DPolyline* pProfile = mp_PolylineInfo->mp_Profile;

		pProfile->ResetPolyline();

		GM2DLineVector RebarLVec(m_RebarLine);
		RebarLVec.Translate(m_RebarZone.m_P0 - CrossVec);
		pProfile->AddCopy(RebarLVec.m_P0);
		pProfile->ma_Bulge.Add(0.);
		pProfile->AddCopy(RebarLVec.m_P1);
		pProfile->ma_Bulge.Add(0.);
		RebarLVec.Translate(m_RebarZone.m_P1 - m_RebarZone.m_P0);
		pProfile->AddCopy(RebarLVec.m_P1);
		pProfile->ma_Bulge.Add(0.);
		pProfile->AddCopy(RebarLVec.m_P0);
		pProfile->ma_Bulge.Add(0.);
		pProfile->AddCopy(*(pProfile->ma_Point[0]));
		*/
	}
	return true;
}

void MSSlabRebar::SetBoxProfile(GM2DLineVector vertical, GM2DLineVector horizon)
{
	GM2DPolyline* pProfile = GetProfile();
	pProfile->ResetPolyline();

	GM2DVector direction = vertical.DirVector().UnitVector();
	direction *= -1.0;
	double distance1 = horizon.Distance(vertical.m_P0, true);
	double distance2 = horizon.Distance(vertical.m_P1, true);
	pProfile->Add(new GM2DVector(horizon.m_P0 + direction*distance1));
	pProfile->Add(new GM2DVector(horizon.m_P0 + direction*distance2));
	pProfile->Add(new GM2DVector(horizon.m_P1 + direction*distance2));
	pProfile->Add(new GM2DVector(horizon.m_P1 + direction*distance1));
	pProfile->MakeClosedPolyline(DTOL1);
}

MSObject* MSSlabRebar::Copy(bool bNewID /*= true*/)
{
	MSSlabRebar* pNewObj;

	pNewObj = new MSSlabRebar();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSSlabRebar::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	if (!pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msSlabRebar) { ASSERT(0); return; }
	MSElement::CopyFromMe(pSource_in, bNewID);

	MSSlabRebar* pSource = (MSSlabRebar*)pSource_in;

	m_pFloor = pSource->m_pFloor;

	m_RebarLine = pSource->m_RebarLine;
	m_RebarZone = pSource->m_RebarZone;
	m_bReverse = pSource->m_bReverse;

	long nBentLoc = pSource->ma_BentLocValue.size();
	for (long iBentLoc = 0; iBentLoc<nBentLoc; iBentLoc++)
		ma_BentLocValue.push_back(pSource->ma_BentLocValue[iBentLoc]);

	if (mp_PolylineInfo)
		delete mp_PolylineInfo;
	if (pSource->mp_PolylineInfo)
		mp_PolylineInfo = (MSPolylineObject*)pSource->mp_PolylineInfo->Copy();
	else
		mp_PolylineInfo = NULL;

	DeleteBentCutLines();
	for (int iBentCut = 0; iBentCut < pSource->ma_BentCutLines.size(); iBentCut++)
	{
		GM2DLineVector* pUr_BentCut = pSource->ma_BentCutLines[iBentCut];
		GM2DLineVector* pMy_BentCut = (GM2DLineVector*)pUr_BentCut->Copy();
		ma_BentCutLines.push_back(pMy_BentCut);
	}

	GetRebarInfo()->CopyFromMe(pSource->GetRebarInfo());

	m_isPolylineType = pSource->m_isPolylineType;
	//m_rebarPolyline.CopyFrom(&(pSource->m_rebarPolyline));
	//m_dLxDeg = pSource->m_dLxDeg;

	//new
	m_RebarOrigin.CopyFrom(&(pSource->m_RebarOrigin));
	SetRebarDirection(pSource->m_dLxDeg);
	m_RebarLine.CopyFrom(&pSource->m_RebarLine);
	CalculateBentLine();

	m_nMaterialID = pSource->m_nMaterialID;
	m_dOffset = pSource->m_dOffset;
	m_IsUsedStoryMaterial = pSource->GetIsUsedStoryMaterial();

	m_IsStartHook = pSource->m_IsStartHook;
	m_IsEndHook = pSource->m_IsEndHook;

	m_dStartCover = pSource->m_dStartCover;
	m_dEndCover = pSource->m_dEndCover;
	m_nTopCoverType = pSource->m_nTopCoverType;
	m_dTopCover = pSource->m_dTopCover;
	m_nBotCoverType = pSource->m_nBotCoverType;
	m_dBotCover = pSource->m_dBotCover;
	m_eSlabRebarType = pSource->GetSlabRebarType();
}

void MSSlabRebar::Init(bool bInConstructor/*=false*/)
{
	if (!bInConstructor)
		MSElement::Init(bInConstructor);

	m_RebarLine.m_P0.SetXY(0., 0.);
	m_RebarLine.m_P1.SetXY(0., 0.);
	m_RebarZone.m_P0.SetXY(0., 0.);
	m_RebarZone.m_P1.SetXY(0., 0.);
	ma_BentLocValue.clear();
}

void MSSlabRebar::Translate(GM2DVector &pDelta)
{
	MSElement::Translate(pDelta);
	m_RebarLine.Translate(pDelta);
	m_RebarZone.Translate(pDelta);
	m_RebarOrigin.Translate(pDelta);

	int nBentCut = ma_BentCutLines.size();
	for (int iBentCut = 0; iBentCut < nBentCut; iBentCut++)
		ma_BentCutLines[iBentCut]->Translate(pDelta);
}

void MSSlabRebar::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSElement::MirrorByAxis(pAxis);
	m_RebarLine.MirrorByAxis(pAxis);
	m_RebarZone.MirrorByAxis(pAxis);
	m_RebarOrigin.MirrorByAxis(pAxis);

	double finalDegree = 2 * pAxis.GetDegree() - m_dLxDeg;
	while (finalDegree < 0)
		finalDegree += 360;
	while (finalDegree > 360)
		finalDegree -= 360;

	SetRebarDirection(finalDegree);

	int nBentCut = ma_BentCutLines.size();
	for (int iBentCut = 0; iBentCut < nBentCut; iBentCut++)
		ma_BentCutLines[iBentCut]->MirrorByAxis(pAxis);
}

void MSSlabRebar::RotateZ(double theta, GM2DVector *pCenter)
{
	MSElement::RotateZ(theta, pCenter);
	m_RebarLine.RotateZ(theta, pCenter);
	m_RebarZone.RotateZ(theta, pCenter);
	m_RebarOrigin.RotateZ(theta, pCenter);

	SetRebarDirection(m_dLxDeg + theta*Rad2Deg);
	int nBentCut = ma_BentCutLines.size();
	for (int iBentCut = 0; iBentCut < nBentCut; iBentCut++)
		ma_BentCutLines[iBentCut]->RotateZ(theta, pCenter);
}

void MSSlabRebar::SetRebarOrigin(GM2DVector vec)
{
	m_RebarOrigin = vec;
}

void MSSlabRebar::SetRebarDirection(double degree)
{
	m_dLxDeg = degree;
	m_Direction = GM2DVector(GM2DVector(0, 0), 1.0, Deg2Rad*degree);
	m_Normal = GM2DVector(GM2DVector(0, 0), 1.0, Deg2Rad*(90 + degree));
}

void MSSlabRebar::RotateZbyChangeDirection(double degree)
{
	GM2DPolyline* pProfile = GetProfile();
	if (pProfile == NULL)	return;

	GM2DVector CenVec = pProfile->MassCenter();

	double oldDegree = m_dLxDeg;
	SetRebarDirection(degree);

	double rotate = (degree - oldDegree)*Deg2Rad;
	RotateZ(rotate, &CenVec);
}

void MSSlabRebar::CalcAutoDegree()
{
	GM2DPolyline* pProfile = GetProfile();
	if (pProfile == NULL)	return;

	double MinY = 0;
	double Degree = 0;
	int NumLine = pProfile->GetNumLines();
	for (int iLine = 0; iLine < NumLine; iLine++)
	{
		GM2DLineVector LineVec = pProfile->GetLine(iLine);
		double CurDegree = LineVec.GetDegree();
		if (CurDegree > 90 && CurDegree <= 270)
			LineVec.ReverseDirection();

		double CurY = LineVec.m_P0.m_Y + LineVec.m_P1.m_Y;
		if (iLine == 0)
		{
			MinY = CurY;
			Degree = LineVec.GetDegree();
		}
		else
		{
			if (MinY > CurY)
			{
				MinY = CurY;
				Degree = LineVec.GetDegree();
			}
		}
	}
	if (fabs(Degree - 360) < DTOL1)
		Degree = 0;
	SetRebarDirection(Degree);
}

void MSSlabRebar::SetOffset(double offset)
{
	m_dOffset = offset;
}

double MSSlabRebar::GetOffset()
{
	return m_dOffset;
}

MSMaterial* MSSlabRebar::GetMaterial()
{
	MSBaseBuilding* pBldg = m_pFloor->GetBuilding();
	return pBldg->GetMaterial(GetMaterialID());
}
bool MSSlabRebar::isPlainConcrete()
{
	MSMaterial* pMat = GetMaterial();
	MSReinforceBarSetting* pReinforceBarSetting = pMat->GetReinforceBarSetting();
	if(pReinforceBarSetting)
	{
		if(pReinforceBarSetting->mm_IndexToReinforceBar.size() == 0)
			return true;
	}
	return false;
}

long MSSlabRebar::GetMaterialID()
{
	if (m_IsUsedStoryMaterial)
	{
		if (m_pFloor == NULL)
		{
			ASSERT(0);
			return m_nMaterialID;
		}
		else
		{
			MSBaseBuilding* pBldg = m_pFloor->GetBuilding();
			MSStoryMaterial* pStoryMaterial = pBldg->GetStoryMaterialByFloorNum(m_pFloor->GetFloorNum());
			if (pStoryMaterial == NULL)
			{
				ASSERT(0);
				return m_nMaterialID;
			}

			MSMaterial* pMaterial = pStoryMaterial->GetMaterial(MSMaterial::msConcrete, false);
			if (pMaterial)	return pMaterial->m_ID;
		}
	}
	return m_nMaterialID;
}

shared_ptr<GM2DCurve> MSSlabRebar::GetStartLine()
{
	GM2DPolyline* pPoly = GetProfile();
	vector<shared_ptr<GM2DCurve>> CurveArr = pPoly->GetIntersectCurves(m_RebarLine.m_P0);
	if (CurveArr.size() == 0)
		return nullptr;

	return CurveArr[0];
}

shared_ptr<GM2DCurve> MSSlabRebar::GetEndLine()
{
	GM2DPolyline* pPoly = GetProfile();
	vector<shared_ptr<GM2DCurve>> CurveArr = pPoly->GetIntersectCurves(m_RebarLine.m_P1);
	if (CurveArr.size() == 0)
		return nullptr;

	return CurveArr[0];
}

MSSlabRebarInfo* MSSlabRebar::GetRebarInfo()
{
	if(mp_RebarInfo == NULL)
		mp_RebarInfo = new MSSlabRebarInfo();
	return mp_RebarInfo;
}

void MSSlabRebar::SetRebarInfo(MSSlabRebarInfo* pRebarInfo)
{
	if (mp_RebarInfo != NULL)
		delete mp_RebarInfo;

	mp_RebarInfo = pRebarInfo;
}

void MSSlabRebar::SetRebarInfo(MSSlabRebarInfo::RebarType type, CString rebarText)
{
	MSSlabRebarInfo* pRebarInfo = GetRebarInfo();
	pRebarInfo->SetRebarType(type);
	pRebarInfo->SetRebarText(rebarText);
}

bool MSSlabRebar::CheckBentLocation()
{
	if (mp_RebarInfo == nullptr)	return false;
	if (mp_RebarInfo->GetRebarType() == MSSlabRebarInfo::Bent)
	{
		double offset = _BENT_OFFSET; //
		GM2DPolyline offsetProfile;
		GetProfile()->OffSet_Acis(offset, false, offsetProfile);

		GM2DVector StartVec = m_RebarOrigin;

		GM2DVector DirectionVec = m_Direction;

		bool IsError = false;
		for (int i = 0; i < ma_BentLocValue.size(); i++)
		{
			double location = ma_BentLocValue[i];
			GM2DVector* bentLoc = new GM2DVector(StartVec + DirectionVec *location);
			if (offsetProfile.Contains(bentLoc) > 0)
				return false;
		}
	}
	return true;
}

void MSSlabRebar::FixProfile()
{
	GM2DPolyline* pProFile = GetProfile();
	if (pProFile == nullptr) return;
	pProFile->DeleteInLinePoint(DTOL_MODELING);
	pProFile->MakeClosedPolyline(DTOL_MODELING);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////