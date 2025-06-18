#include "StdAfx.h"
#include <math.h>

#include "MSGenStairG.h"
#include "MSContainer.h"
#include "MSRebarBuilding.h"
#include "MSFaceMemberM.h"
#include "MSFloor.h"
#include "MSStairM.h"
#include "MSConcrete.h"
#include "MSSlabG.h"
#include "MSCompoGUtil.h"
#include "GMLib/MSVersionInfo.h"
#include "GMLib/GM2DBound.h"
#include "RADFCore_CLI/AppGlobalOptionWrapper.h"

//#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSStairPart, MSPartCompoG, VERSIONABLE_SCHEMA | 1)
MSStairPart::MSStairPart()
{
	m_pPolyline = new GM2DPolyline();
	m_Thick = 200;
    m_nBottomLevel = 0;
    m_nTopLevel = 0;
    m_nStepsNum = 1;
	mp_OwnerCompoG = NULL;
	m_nSequence = 0;
	mp_Curve = nullptr;
	m_dWidth = 0;
	m_dStepHeight = 200;
	m_dStepWidth = 200;
	//m_IsEndRiser = false;
	m_StairStepShapeType = StairStepShapeType::BottomSlabLevelNStepHeight;
	m_IsCalculateBottomStep = false;
	m_nUnitPrecision = 3;
}

MSStairPart::~MSStairPart(void)
{
	delete m_pPolyline;
	if (mp_Curve != nullptr)
		delete mp_Curve;
}

MSFloor*MSStairPart::GetFloor()
{
	if (mp_OwnerCompoG != nullptr)
		return mp_OwnerCompoG->GetFloor();
	return nullptr;
}

void MSStairPart::Serialize(CArchive &ar)
{
	MSPartCompoG::Serialize(ar);
	bool bEachCompoM = false;
	if(ar.IsStoring())
	{
		ar << m_Thick;
        ar << m_nBottomLevel;
        ar << m_nTopLevel;
        ar << m_nStepsNum;
		m_pPolyline->Serialize(ar);

		if(mp_CompoM->m_ID == 0)
			bEachCompoM = true;

		ar << bEachCompoM;
		if(bEachCompoM)
			Serialize_EachCompoM(ar);

		ar << m_nSequence;
		GM2DCurve::SerializeWithNullFlag(ar, mp_Curve);
		ar << m_dWidth;
		ar << m_dStepHeight;
		
		ar << m_dStepWidth;
		//ar << m_IsEndRiser;
		ar << (int)m_StairStepShapeType;
		ar << m_IsCalculateBottomStep;
		ar << m_nUnitPrecision;
	}
	else
	{
		ar >> m_Thick;
        ar >> m_nBottomLevel;
        ar >> m_nTopLevel;
        ar >> m_nStepsNum;
		m_pPolyline->Serialize(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20140228)
		{
			ar >> bEachCompoM;
			if(bEachCompoM)
				Serialize_EachCompoM(ar);
		}
		
		if (MSVersionInfo::GetCurrentVersion() >= 20170414)
		{
			ar >> m_nSequence;
			GM2DCurve::SerializeWithNullFlag(ar, mp_Curve);
			ar >> m_dWidth;
			ar >> m_dStepHeight;
		}

		if (IsMVAddToBHQ() && MSVersionInfo::GetCurrentVersion() >= 20190308)
		{
			ar >> m_dStepWidth;
			//ar >> m_IsEndRiser;
			if (MSVersionInfo::GetCurrentVersion() < 20200409)
			{
				bool isEndRiser = false;
				ar >> isEndRiser;
				m_StairStepShapeType = StairStepShapeType::BottomSlabLevelNStepHeight;
			}
			else
			{
				int nStepType = 0;
				ar >> nStepType;
				m_StairStepShapeType = (StairStepShapeType)nStepType;
			}
			ar >> m_IsCalculateBottomStep;
			ar >> m_nUnitPrecision;
		}

		
	}
}

void MSStairPart::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
		long MType = mp_CompoM->GetType();
		ar << MType;
		long MaterialID = 0;
		MSMaterial* pMaterial = mp_CompoM->GetMaterial();
		if (pMaterial) MaterialID = pMaterial->m_ID;
		ar << MaterialID;
		ar << mp_CompoM->m_Name;
	}
	else
	{
		MSBaseBuilding* pBldg = GetBuilding();
		long MType = 0;
		ar >> MType;

		MSStairM* pStairM = (MSStairM*)pBldg->CreateNewCompoM(MType);
		pStairM->m_nType = (MSCompoM::Type_MSCompoM)MType;

		long nMaterialID = 0;
		ar >> nMaterialID;
		pStairM->SetMaterial(pBldg->GetMaterial(nMaterialID));
		ar >> pStairM->m_Name;
		pStairM->mp_Owner = pBldg;
		mp_CompoM = pStairM;
	}
}

void MSStairPart::Dump(EFS::EFS_Buffer& buffer)
{
	MSPartCompoG::Dump(buffer);

	buffer << m_Thick;
	buffer << m_nBottomLevel;
	buffer << m_nTopLevel;
	buffer << m_nStepsNum;
	m_pPolyline->Dump(buffer);
	buffer << m_nSequence;
	GM2DCurve::Dump(buffer, mp_Curve);
	buffer << m_dWidth;
	buffer << m_dStepHeight;

	buffer << m_dStepWidth;
	//buffer << m_IsEndRiser;
	buffer << m_StairStepShapeType;
	buffer << m_IsCalculateBottomStep;
	buffer << m_nUnitPrecision;
}

void MSStairPart::Recovery(EFS::EFS_Buffer& buffer)
{
	MSPartCompoG::Recovery(buffer);

	buffer >> m_Thick;
	buffer >> m_nBottomLevel;
	buffer >> m_nTopLevel;
	buffer >> m_nStepsNum;
	m_pPolyline->Recovery(buffer);
	buffer >> m_nSequence;
	GM2DCurve::Recovery(buffer, mp_Curve);
	buffer >> m_dWidth;
	buffer >> m_dStepHeight;

	buffer >> m_dStepWidth;
	//buffer >> m_IsEndRiser;
	buffer >> m_StairStepShapeType;
	buffer >> m_IsCalculateBottomStep;
	buffer >> m_nUnitPrecision;
}

MSObject* MSStairPart::Copy(bool bNewID)
{
	MSStairPart* pNewObj = new MSStairPart();
	pNewObj->CopyFromMe(this);
	return pNewObj;
}

void MSStairPart::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	MSPartCompoG::CopyFromMe(pSource_in, bNewID);

	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() == msNULL) { ASSERT(0); return; }
	MSStairPart* pSourcePart = (MSStairPart*)pSource_in;

	m_pPolyline->CopyFrom(pSourcePart->m_pPolyline);
	m_Thick = pSourcePart->m_Thick;
    m_nBottomLevel = pSourcePart->m_nBottomLevel;
    m_nTopLevel = pSourcePart->m_nTopLevel;
	m_nStepsNum = pSourcePart->m_nStepsNum;
	m_nSequence = pSourcePart->m_nSequence;

	if (mp_Curve != nullptr)
		delete mp_Curve;
	mp_Curve = nullptr;
	if (pSourcePart->mp_Curve != nullptr)
		mp_Curve = pSourcePart->mp_Curve->Copy();
	m_dWidth = pSourcePart->m_dWidth;
	m_dStepHeight = pSourcePart->m_dStepHeight;

	m_dStepWidth = pSourcePart->m_dStepWidth;
	//m_IsEndRiser = pSourcePart->m_IsEndRiser;
	m_StairStepShapeType = pSourcePart->m_StairStepShapeType;
	m_IsCalculateBottomStep = pSourcePart->m_IsCalculateBottomStep;
	m_nUnitPrecision = pSourcePart->m_nUnitPrecision;
}

bool MSStairPart::MakeBoundary()
{
	mp_Boundary->CopyFrom(m_pPolyline);
	mp_Boundary->MakeCCW();
	return true;
}

bool MSStairPart::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	double dFloorLevel = GetSlabLevel();
	double dBotOffset = 0;
//	MSGenStairG* pStairG = (MSGenStairG*)GetOwnerCompoG();
// 	if (pStairG != NULL)
// 		dBotOffset = pStairG->m_dBotOffset;
	
	if (m_nStepsNum == 1)
	{
		double dUsedLevel = m_nBottomLevel;
		if (dUsedLevel == 0)
			dUsedLevel = m_nTopLevel;
		TopLevel = dFloorLevel + dUsedLevel + dBotOffset;
		BottomLevel = dFloorLevel + dUsedLevel + dBotOffset - m_Thick;
	}
	else
	{
		TopLevel = dFloorLevel + max(m_nTopLevel, m_nBottomLevel) + dBotOffset;
		BottomLevel = dFloorLevel + min(m_nTopLevel, m_nBottomLevel) + dBotOffset - m_Thick;
	}

	return true;
}

// return value : m_SOffset 기준에서의 높이
double MSStairPart::GetTopSurfaceZ(double x, double y)
{
	if (m_pPolyline->GetNumLines() != 4) return 0;

	if (m_nTopLevel == m_nBottomLevel) return m_nBottomLevel;
	double dOneStepZ = m_dStepHeight;
	if(m_nStepsNum > 0)
		dOneStepZ = (m_nTopLevel - m_nBottomLevel) / m_nStepsNum;

	GM2DCurve* bot_edge = m_pPolyline->GetCurve(1);
	GM2DCurve* top_edge = m_pPolyline->GetCurve(3);

	double ratio = bot_edge->OrthoBaseLocValue(GM2DVector(x, y));
	double leng = bot_edge->Length();

	for (int i = 0; i < 3; ++i)
	{
		GM2DLineVector line(bot_edge->PointAtParam(ratio), top_edge->PointAtParam(1 - ratio));
		double dist = line.Distance(GM2DVector(x, y), true);
		ratio = ratio - dist / leng;
	}

	delete top_edge;
	delete bot_edge;

	double dEndLevel = m_nTopLevel - dOneStepZ;

	return ratio * (dEndLevel - m_nBottomLevel);
}

void MSStairPart::Translate( GM2DVector &pDelta )
{
	MSPartCompoG::Translate(pDelta);

	if(m_pPolyline != nullptr)
		m_pPolyline->Translate(pDelta);
	if (mp_Curve != nullptr)
		mp_Curve->Translate(pDelta);
}

void MSStairPart::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSPartCompoG::MirrorByAxis(pAxis);

	if(m_pPolyline != nullptr)
		m_pPolyline->MirrorByAxis(pAxis);
	if (mp_Curve != nullptr)
		mp_Curve->MirrorByAxis(pAxis);
}

void MSStairPart::RotateZ( double theta, GM2DVector *pCenter )
{
	MSPartCompoG::RotateZ(theta, pCenter);
	if(m_pPolyline != nullptr)
		m_pPolyline->RotateZ(theta, pCenter);
	if (mp_Curve != nullptr)
		mp_Curve->RotateZ(theta, pCenter);
}

CString MSStairPart::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋
	GM2DPolyline *pPoly = m_pPolyline;
	GM2DVector CenVec = pPoly->MassCenter();

	CString strCompoMKey;
	if (mp_CompoM == NULL || mp_CompoM->m_ID == 0)
		strCompoMKey.Format(_T("%d"), 0);
	else
		strCompoMKey.Format(_T("%d"), &mp_CompoM->m_ID);
	CString strGeo;
	strGeo.Format(_T("%d_%d_%d_%d_%d"), pPoly->GetNumLines(), (int)pPoly->GetLength(), (int)pPoly->Area(), (int)CenVec.m_X, (int)CenVec.m_Y);
	CString strOffSet;
	strOffSet.Format(_T("%d_%d_%d_%d_%d"), (int)IsSlopeMember(), (int)m_nBottomLevel, (int)m_nTopLevel, m_nStepsNum, m_Thick);

	CString strKey;
	strKey.Format(_T("%d_%d_%s_%s_%s"), GetFloorNum(), (int)GetType(), strCompoMKey, strGeo, strOffSet);
	if (bUseID)
	{
		CString strID;
		strID.Format(_T("%d_"), m_ID);
		strKey = strID + strKey;
	}

	return strKey;
}

int MSStairPart::GetSequence()
{
	return m_nSequence;
}

void MSStairPart::SetSequence(int nSeq)
{
	m_nSequence = nSeq;
}

GM2DCurve* MSStairPart::GetPathCurve()
{
	if (mp_Curve == nullptr)
		CalcPathCurve();
	return mp_Curve;
}

void MSStairPart::SetPathCurve(GM2DCurve* PathCurve)
{
	if (PathCurve == nullptr)
		return;

	if (mp_Curve != nullptr)
		delete mp_Curve;
	mp_Curve = PathCurve->Copy();
}

double MSStairPart::GetDepth()
{
	return m_Thick;
}

double MSStairPart::GetThick()
{
	return m_Thick;
}

double MSStairPart::GetWidth()
{
	return m_dWidth;
}

void MSStairPart::SetWidth(double dWidth)
{
	m_dWidth = dWidth;
}

bool MSStairPart::IsLanding()
{
	MSCompoM* pCompoM = GetCompoM();
	if (pCompoM->GetType() == MSCompoM::msStairLandingM)
		return true;

	return false;
}

long MSStairPart::GetStepsNum()
{
	if (m_nStepsNum == 0)
	{
		int NumStep = (m_nTopLevel - m_nBottomLevel) / GetOneStepHeight();
		return NumStep;
	}
	return m_nStepsNum;
}

void MSStairPart::SetStepsNum(long nStepsNum)
{
	// 계단의 최소 곗수 3개
	if (nStepsNum < 2)
		nStepsNum = 3;

	m_nStepsNum = nStepsNum;
	CalcOneStep();
	return;

// 	double dHeight = (m_LevelE - m_LevelS);
// 	double dStepHeight = dHeight / (double)nStepsNum;
// 	dStepHeight = (int)(dStepHeight / 10) * 10; // 버림
// 
// 	double dLastStepHeight = fabs(dHeight - ((nStepsNum)* dStepHeight));
// 	
// 	if (dLastStepHeight + dStepHeight > dStepHeight * 1.4) // 마지막 계단의 높이가 dStepHeight 보다 1.4 높은 경우 10단위에서 올림 처리
// 	{
// 		dStepHeight = dHeight / (double)nStepsNum;
// 		dStepHeight = (int)(ceil(dStepHeight / 10)) * 10; // 올림 처리
// 		dLastStepHeight = fabs(dHeight - ((nStepsNum)* dStepHeight));
// 	}
// 	m_dStepHeight = dStepHeight;
// 	m_nStepsNum = nStepsNum;
}

long MSStairPart::GetBottomLevel()
{
	return m_nBottomLevel;
}

void MSStairPart::SetBottomLevel(long nBottomLevel)
{
	m_nBottomLevel = nBottomLevel;
	CalcOneStep();
}

long MSStairPart::GetTopLevel()
{
	return m_nTopLevel;
}

void MSStairPart::SetTopLevel(long nTopLevel)
{
	m_nTopLevel = nTopLevel;
	CalcOneStep();
}

bool MSStairPart::GetIsEndRiser()
{
	if (m_StairStepShapeType == StairStepShapeType::TopSlabLevel)
		return true;

	return false;
}

bool MSStairPart::GetIsCalculateBottomStep()
{
	return m_IsCalculateBottomStep;
}

void MSStairPart::SetIsCalculateBottomStep(bool value)
{
	m_IsCalculateBottomStep = value;
}

StairStepShapeType MSStairPart::GetStairPartStepType()
{
	return m_StairStepShapeType;
}

void MSStairPart::SetStairPartStepType(StairStepShapeType stepType)
{
	m_StairStepShapeType = stepType;
}

int MSStairPart::GetUnitPrecision()
{
	return m_nUnitPrecision;
}

void MSStairPart::SetUnitPrecision(int value)
{
	m_nUnitPrecision = value;
	CalcOneStep();
}

void MSStairPart::CalcOneStep()
{
	CalcOneStepHeight();
	CalcOneStepWidth();
}

void MSStairPart::CalcOneStepHeight()
{
	// BH-Q 코드
	//m_dStepHeight = (m_nTopLevel - m_nBottomLevel) / (double)m_nStepsNum;

	if (IsLanding() == true)
	{
		m_dStepHeight = m_Thick;
		return;
	}

	MSStairPart* pNextPart = GetNextStairPart();
	double dNextPartThick = 0;
	if (pNextPart != nullptr)
		dNextPartThick = pNextPart->GetThick();
	else // 계단 상부에 연결된 슬래브를 찾아본다.
	{
		MSFloor* pCurFloor = GetFloor();

		vector<MSCompoG*> slabArr = this->FindConnectMember(FindType_MSElement::msSlabG, 0);
		for (int i = 0; i < slabArr.size(); i++)
		{
			dNextPartThick = slabArr[i]->GetThick();
			break;
		}
	}

	double A = 0;
	double Ly = m_nTopLevel - m_nBottomLevel;
	if (m_StairStepShapeType ==  StairStepShapeType::BottomSlabLevel)
		A = (Ly - dNextPartThick) / m_nStepsNum;
	else
		A = Ly / m_nStepsNum;

	double stepUnit = GetStepUnit();

	double B = ceil(A * stepUnit) / stepUnit;

	m_dStepHeight = B;
}

void MSStairPart::CalcOneStepWidth()
{
	if (IsLanding() == true)
	{
		m_dStepWidth = 0;
		return;
	}

	if (mp_Curve == nullptr)
		return;
	
	double Lx = mp_Curve->Length();
	
	int stepsNum = m_nStepsNum;
	if (m_StairStepShapeType == StairStepShapeType::BottomSlabLevelNStepHeight)
		stepsNum--;
	
	double A = Lx / stepsNum;

	double stepUnit = GetStepUnit();

	double B = ceil(A * stepUnit) / stepUnit;
	//double B = (A / nDivNum) * nDivNum;

	m_dStepWidth = B;
}

double MSStairPart::GetOneStepHeight()
{
	return m_dStepHeight;
}

double MSStairPart::GetOneStepWidth()
{
	return m_dStepWidth;
}

MSStairPart* MSStairPart::GetNextStairPart()
{
	MSGenStairG* pStairG = dynamic_cast<MSGenStairG*>(mp_OwnerCompoG);
	if (pStairG == nullptr)
		return nullptr;
	
	return pStairG->GetStairPartbySequence(m_nSequence + 1);
}

MSStairPart* MSStairPart::GetPreStairPart()
{
	MSGenStairG* pStairG = dynamic_cast<MSGenStairG*>(mp_OwnerCompoG);
	if (pStairG == nullptr)
		return nullptr;

	return pStairG->GetStairPartbySequence(m_nSequence - 1);
}


double MSStairPart::GetCalcStepHeight()
{
	MSStairPart* pNextPart = GetNextStairPart();
	double dNextPartThick = 0;
	if (pNextPart != nullptr)
		dNextPartThick = pNextPart->GetThick();
	else // 계단 상부에 연결된 슬래브를 찾아본다.
	{
		MSFloor* pCurFloor = GetFloor();

		vector<MSCompoG*> slabArr = this->FindConnectMember(FindType_MSElement::msSlabG, 0);
		for (int i = 0; i < slabArr.size(); i++)
		{
			dNextPartThick = slabArr[i]->GetThick();
			break;
		}
	}

	double A = 0;
	double Ly = m_nTopLevel - m_nBottomLevel;
	if (m_StairStepShapeType == StairStepShapeType::BottomSlabLevel)
		A = (Ly - dNextPartThick) / m_nStepsNum;
	else
		A = Ly / m_nStepsNum;
	
	double stepUnit = GetStepUnit();

	double B = ceil(A * stepUnit) / stepUnit;
	double C = B * m_nStepsNum;
	double D = 0;
	if (m_StairStepShapeType == StairStepShapeType::BottomSlabLevel)
		D = C - (Ly - dNextPartThick);
	else
		D = C - Ly;
	double dCalcStepHeight = B - D;

	return dCalcStepHeight;
}

double MSStairPart::GetStepUnit()
{
	//int nDivNum = 1;
	// 	if (m_nUnitPrecision > 1)
	// 		nDivNum = 100;
	// 	else if (m_nUnitPrecision > 0)
	// 		nDivNum = 10;
		// 자리수올림에서  소수점자리 올림으로 변경됨

	double dUnit = 100;
	if (m_nUnitPrecision == 0)
		dUnit = 1;
	else if (m_nUnitPrecision == 1)
		dUnit = 10;
	else
		dUnit = 100;

	return  dUnit;
}

double MSStairPart::GetCalcStepWidth()
{
	if (mp_Curve == nullptr)
		return m_dStepWidth;

	double Lx = mp_Curve->Length();

	int stepsNum = m_nStepsNum;
	if (m_StairStepShapeType == StairStepShapeType::BottomSlabLevelNStepHeight)
		stepsNum--;

	double A = Lx / stepsNum;

	double stepUnit = GetStepUnit();

	double B = ceil(A * stepUnit) / stepUnit;
	double C = B * stepsNum;
	double D = C - Lx;
	double dCalcStepWidth = B - D;
	
	return dCalcStepWidth;
}

void MSStairPart::SetOneStepHeight(double dStepHeight)
{
	if (dStepHeight > 0)
	{
		m_dStepHeight = (int)(dStepHeight / 10) * 10;

		double dHeight = (m_nTopLevel - m_nBottomLevel);
		int NumStep = (m_nTopLevel - m_nBottomLevel) / m_dStepHeight;

		if (NumStep < 3)
			NumStep = 3;

		double dLastStepHeight = fabs(dHeight - ((NumStep) * m_dStepHeight));

		if(dLastStepHeight + dStepHeight > dStepHeight * 1.4)// 마지막 계단의 높이가 dStepHeight 보다 1.4 높은 경우 계단의 갯수를 1개 늘린다.
			m_nStepsNum = NumStep + 1;
		else
			m_nStepsNum = NumStep;
	}
}

void MSStairPart::CalcPathCurve()
{
	MSGenStairG* pStair = (MSGenStairG*)mp_OwnerCompoG;
	if (pStair->isComponentStair() == true)
	{
		if (pStair->GetStairType() == MSGenStairG::StairType_Line)
			CalcPathCurve_LineStair();
		else
			CalcPathCurve_RectStair();
	}
	else
	{
		CalcPathCurve_UserStair();
	}
	CalcOneStep();
}

void MSStairPart::CalcPathCurve_RectStair()
{
	if (mp_OwnerCompoG == nullptr)
		return;
	
	if (mp_Curve != nullptr)
		delete mp_Curve;
	mp_Curve = nullptr;

	MSGenStairG* pStair = (MSGenStairG*)mp_OwnerCompoG;
	bool bLanding = IsLanding();
	double dStairBaseLineLen = 0;
	if (bLanding == true)
		dStairBaseLineLen = (double)INTMAX_MAX;

	GM2DLineVector StairBaseLine;
	vector<GM2DLineVector> LineVecArr;
	int NumVec = pStair->ma_Vec.size();
	for (int i = 0; i < NumVec; i++)
	{
		GM2DLineVector TheLine;
		TheLine.m_P0 = *pStair->ma_Vec[i];
		TheLine.m_P1 = *pStair->ma_Vec[(i + 1) % NumVec];

		double dCurLen = TheLine.Length();

		if (bLanding)
		{
			if (dStairBaseLineLen > dCurLen) // 짧은선 찾기
			{
				dStairBaseLineLen = dCurLen;
				StairBaseLine = TheLine;
			}
		}
		else
		{
			if (dStairBaseLineLen < dCurLen) //  긴선 찾기
			{
				dStairBaseLineLen = dCurLen;
				StairBaseLine = TheLine;
			}
		}
	}

	GM2DPolyline StairPoly;
	pStair->GetBoundary(StairPoly);
	double dMinLen = (double)INTMAX_MAX;
	GM2DCurve* pShortCurve = nullptr;

	long nSeg = m_pPolyline->ma_Point.GetCount() - 1;
	for (long iSeg = 0; iSeg < nSeg; iSeg++)
	{
		GM2DCurve* pCurve = m_pPolyline->GetCurve(iSeg);
		double dCurLen = pCurve->Length();

		if (pCurve->IsParallel4DTol(&StairBaseLine) == 0) // Not Parallel
		{
			if(bLanding == true)
			{
				if (StairPoly.Contains(&pCurve->PointAtParam(0.5)) == 0)
				{
					if (dCurLen < dMinLen)
					{
						dMinLen = dCurLen;
						pShortCurve = pCurve->Copy();
					}
				}
			}
			else
			{
				if (dCurLen < dMinLen)
				{
					dMinLen = dCurLen;
					pShortCurve = pCurve->Copy();
				}
			}
		}
		delete pCurve;
	}

	double dDeg = StairBaseLine.GetDegree();
	if (pShortCurve != nullptr)
	{
		GM2DVector CenVec = pShortCurve->PointAtParam(0.5);
		GM2DLineVector PathLine = m_pPolyline->GetLineVectorOnXY(dDeg, CenVec);

		delete pShortCurve;
		mp_Curve = PathLine.Copy();
	}
	else
		CalcPathCurve_UserStair();
}

void MSStairPart::CalcPathCurve_LineStair()
{
	if (mp_Curve != nullptr)
		m_pPolyline->GetCurveInPolygon(mp_Curve);
}

void MSStairPart::CalcPathCurve_UserStair()
{
	// 폴리라인중 가장 긴선의 각도 + 폴리라인의 중점으로 생성 PathCurve 생성
	if (mp_Curve != nullptr)
		delete mp_Curve;
	mp_Curve = nullptr;

	GM2DPolyline* pPoly = m_pPolyline;
	GM2DVector rib_pos;
	double deg = 0;
	//MSSlabG::CalcInitialLxDegFromProfile(*pPoly, deg, rib_pos);

	double dMaxLen = 0;
	GM2DCurve* pLongCurve = nullptr;
	long nSeg = m_pPolyline->ma_Point.GetCount() - 1;
	for (long iSeg = 0; iSeg < nSeg; iSeg++)
	{
		GM2DCurve* pCurve = m_pPolyline->GetCurve(iSeg);
		double dCurLen = pCurve->Length();
		if (dCurLen > dMaxLen)
		{
			dMaxLen = dCurLen;
			pLongCurve = pCurve;
		}
		else
			delete pCurve;
	}

	if (pLongCurve != nullptr)
		deg = pLongCurve->GetDegree();
	delete pLongCurve;

	GM2DVector CenVec = pPoly->MassCenter();
	GM2DLineVector PathLine = pPoly->GetLineVectorOnXY(deg, CenVec);

	mp_Curve = PathLine.Copy();
}

GM2DPolyline* MSStairPart::GetExtendLineProfile(eExtendType eType, double dExtendLen)
{
	GM2DPolyline* pPoly = new GM2DPolyline();
	if (mp_Curve == nullptr || m_dWidth <= 0)
	{
		pPoly->CopyFrom(m_pPolyline);
		return pPoly;
	}

	GM2DPolyline Boundary;

	GM2DCurve* pTmpCenCurve = mp_Curve->Copy();

	if (eType == ExtendType_Start || eType == ExtendType_Both)
		pTmpCenCurve->ExtendLineVector(TRUE, dExtendLen, FALSE);
	if(eType == ExtendType_End || eType == ExtendType_Both)
		pTmpCenCurve->ExtendLineVector(FALSE, dExtendLen, TRUE);

	GM2DCurve* pLCurve = pTmpCenCurve;
	GM2DCurve* pRCurve = pTmpCenCurve->Copy();
	
	pLCurve->NormalTranslateMe(m_dWidth / 2);
	pRCurve->NormalTranslateMe(-m_dWidth / 2);
	
	pPoly->AddCopy(pRCurve->m_P0);
	pPoly->ma_Bulge.Add(pRCurve->GetBulge());
	pPoly->AddCopy(pRCurve->m_P1);
	pPoly->ma_Bulge.Add(0);
	pPoly->AddCopy(pLCurve->m_P1);
	pPoly->ma_Bulge.Add(pLCurve->GetBulge() * -1);
	pPoly->AddCopy(pLCurve->m_P0);
	pPoly->ma_Bulge.Add(0);
	pPoly->AddCopy(pRCurve->m_P0);

	delete pLCurve;
	delete pRCurve;

	return pPoly;
}

bool MSStairPart::IsConnect(MSStairPart* pUrStairPart)
{
	MSFloor* pCurFloor = GetFloor();
	MSFloor* pUrFloor = pUrStairPart->GetFloor();

	if (pCurFloor != nullptr && pUrFloor != nullptr)
	{
		if (m_nBottomLevel + pCurFloor->GetSlabLevel() == pUrStairPart->GetBottomLevel() + pUrStairPart->GetSlabLevel() ||
			m_nBottomLevel + pCurFloor->GetSlabLevel() == pUrStairPart->GetTopLevel() + pUrStairPart->GetSlabLevel() ||
			m_nTopLevel + pCurFloor->GetSlabLevel() == pUrStairPart->GetBottomLevel() + pUrStairPart->GetSlabLevel() ||
			m_nTopLevel + pCurFloor->GetSlabLevel() == pUrStairPart->GetTopLevel() + pUrStairPart->GetSlabLevel())
		{
			if (m_pPolyline->OverlapEx(*pUrStairPart->m_pPolyline))
				return true;
		}
	}
	else
	{
		if (m_nBottomLevel == pUrStairPart->GetBottomLevel() ||
			m_nBottomLevel == pUrStairPart->GetTopLevel() ||
			m_nTopLevel == pUrStairPart->GetBottomLevel() ||
			m_nTopLevel == pUrStairPart->GetTopLevel())
		{
			if (m_pPolyline->OverlapEx(*pUrStairPart->m_pPolyline))
				return true;
		}
	}
	
	return false;
}


IMPLEMENT_SERIAL(MSGenStairG, MSPartCompoG, VERSIONABLE_SCHEMA | 1)

double MSStairPart::GetSlopeAngle()
{
	if (IsLanding())
		return 0;
	//step
	GM2DCurve* pCurve0 = m_pPolyline->GetCurve(0);
	GM2DCurve* pCurve2 = m_pPolyline->GetCurve(2);
	double xyLen = pCurve0->PointAtParam(0.5).Distance(pCurve2->PointAtParam(0.5));
	double angle = atan((m_nTopLevel - m_nBottomLevel) / xyLen);
	
	delete pCurve0;
	delete pCurve2;
	return angle;
}

double MSStairPart::GetStairPartDegree()
{
	double degree = 0;
	if (m_pPolyline->IsRectangle() == true)
	{
		if (mp_Curve != nullptr)
			return mp_Curve->GetDegree();

		GM2DCurve* pFirstCurve = m_pPolyline->GetCurve(0);
		GM2DCurve* pSecondCurve = m_pPolyline->GetCurve(1);
		if (pFirstCurve->Length() > pSecondCurve->Length())
			degree = pSecondCurve->GetDegree();
		else
			degree = pFirstCurve->GetDegree();
		
		delete pFirstCurve;
		delete pSecondCurve;
	}
	else
	{
		vector<GM2DCurve*> MySegments;
		m_pPolyline->GetSegments(MySegments);

		GM2DCurve* pMinCurve = nullptr;
		double minLen = MAXINT;
		for (int idx = 0; idx < MySegments.size(); idx++)
		{
			double curLen = MySegments[idx]->Length();
			if (curLen < minLen)
			{
				minLen = curLen;
				pMinCurve = MySegments[idx];
			}
		}

		degree = pMinCurve->GetDegree();
		GMObject::DeleteGMObject(MySegments);
	}
	return degree;
}

MSGenStairG::MSGenStairG()
{
	Init(true);
}

MSGenStairG::~MSGenStairG(void)
{
	m_InnerPoly.DeleteVecArr(); // remove later
	DeleteStairArr();
	DeleteLandingArr();
}

vector<MSStairPart*> MSGenStairG::GetStairPart()
{
	vector<MSStairPart*> StairPart;

	for (long i = 0; i < ma_Step.GetSize(); i++)
		StairPart.push_back(ma_Step[i]);

	for (long i = 0; i < ma_Landing.GetSize(); i++)
		StairPart.push_back(ma_Landing[i]);

	if (GetStairType() == MSGenStairG::StairType_General || GetStairType() == MSGenStairG::StairType_Double)
	{
		sort(StairPart.begin(), StairPart.end(),
			[](MSStairPart* a, MSStairPart* b)->bool { return a->GetSequence() < b->GetSequence(); });
	}

	return StairPart;
}

MSStairPart* MSGenStairG::GetFirstStep()
{
	return GetEndStairPart(ma_Step, true);
}

MSStairPart* MSGenStairG::GetLastStep()
{
	return GetEndStairPart(ma_Step, false);
}

MSStairPart* MSGenStairG::GetFirstLanding()
{
	return GetEndStairPart(ma_Landing, true);
}

MSStairPart* MSGenStairG::GetLastLanding()
{
	return GetEndStairPart(ma_Landing, false);
}

void MSGenStairG::DeleteStairArr()
{
	long StairCount = ma_Step.GetSize();
	for( long iStair = 0; iStair < StairCount; iStair++)
		delete ma_Step[iStair];
	ma_Step.RemoveAll();
}

void MSGenStairG::DeleteLandingArr()
{
	long LandingCount = ma_Landing.GetSize();
	for( long iLanding = 0; iLanding < LandingCount; iLanding++)
		delete ma_Landing[iLanding];
	ma_Landing.RemoveAll();
}

void MSGenStairG::AddStep(MSStairPart* steps)
{
	steps->mp_OwnerCompoG = this;
    ma_Step.Add(steps);
}

void MSGenStairG::AddLanding(MSStairPart* landing)
{
	landing->mp_OwnerCompoG = this;
    ma_Landing.Add(landing);
}

void MSGenStairG::Serialize(CArchive &ar)
{
	MSPartCompoG::Serialize(ar);
	int StepsCount = 0;
	int LandingCount = 0;
	MSBaseBuilding* pBldg = GetBuilding();
	int DefaultMaterialID = 0;
	if(pBldg->GetDefMaterial(MSMaterial::msConcrete) != nullptr)
		DefaultMaterialID = pBldg->GetDefMaterial(MSMaterial::msConcrete)->m_ID;
	int MaterialID = DefaultMaterialID;
	bool bEachCompoM = false;
	if (ar.IsStoring())
	{
		CheckPartProfile();

		ar << m_Name;
		StepsCount = ma_Step.GetSize();
		ar << StepsCount;
		for (int iStair = 0; iStair < StepsCount; iStair++)
		{
			MSStairPart* pStep = ma_Step[iStair];
			pStep->Serialize(ar);
		}

		LandingCount = ma_Landing.GetSize();
		ar << LandingCount;
		for (int iLanding = 0; iLanding < LandingCount; iLanding++)
		{
			MSStairPart* pLanding = ma_Landing[iLanding];
			pLanding->Serialize(ar);
		}
		//		if(mp_CompoM->m_ID == 0)
		bEachCompoM = true;

		ar << bEachCompoM;
		if (bEachCompoM)
			Serialize_EachCompoM(ar);

		ar << m_isComponentStair;
		ar << m_dBotOffset;

		ar << m_dTopOffset; // TopZOffset
		
		int nStairType = m_eStairType;
		ar << nStairType;
		
		int NumLandingHeight = ma_LandingHeight.size();
		ar << NumLandingHeight;
		for (int i = 0; i < NumLandingHeight; i++)
			ar << ma_LandingHeight[i];

		ar << m_bRemoveStartLanding;
		ar << m_bRemoveEndLanding;

		ar << m_dSetpHeight;
		ar << m_nStepNum;
	}
	else
	{
		ar >> m_Name;
		ar >> StepsCount;
		for (int iStair = 0; iStair < StepsCount; iStair++)
		{
			MSStairPart* pStep = new MSStairPart();
			pStep->SetOwner(mp_Owner);
			pStep->SetOwnerCompoG(this);
			pStep->Serialize(ar);
			AddStep(pStep);
		}

		ar >> LandingCount;
		for (int iLanding = 0; iLanding < LandingCount; iLanding++)
		{
			MSStairPart* pLanding = new MSStairPart();
			pLanding->SetOwner(mp_Owner);
			pLanding->SetOwnerCompoG(this);
			pLanding->Serialize(ar);
			AddLanding(pLanding);
		}

		ar >> bEachCompoM;
		if (bEachCompoM)
			Serialize_EachCompoM(ar);

		if (mp_CompoM == nullptr)
		{
			mp_CompoM = pBldg->CreateNewCompoM(MSCompoM::msStairStepM);
			mp_CompoM->m_nType = MSCompoM::msStairStepM;
			mp_CompoM->SetMaterial(pBldg->GetMaterial(DefaultMaterialID));
			mp_CompoM->mp_Owner = pBldg;
			mp_CompoM->m_Name = _T("NONAME");
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20140520)
		{
			ar >> m_isComponentStair;
		}

		long NumVec = ma_Vec.size();
		if (NumVec == 0)
			ReSetVec();

		if (MSVersionInfo::GetCurrentVersion() >= 20150629)
		{
			ar >> m_dBotOffset;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170414)
		{
			ar >> m_dTopOffset; // TopZOffset

			int nStairType = 0;
			ar >> nStairType;
			m_eStairType = (StairType)nStairType;
			int NumLandingHeight = 0;
			ar >> NumLandingHeight;

			double dHeight = 0;
			for (int i = 0; i < NumLandingHeight; i++)
			{
				ar >> dHeight;
				ma_LandingHeight.push_back(dHeight);
			}
			ar >> m_bRemoveStartLanding;
			ar >> m_bRemoveEndLanding;
		}
		else
		{
			m_eStairType = StairType_User;
			UpdateOldStairInfo();
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170424)
		{
			ar >> m_dSetpHeight;
			ar >> m_nStepNum;
		}

		if (MSVersionInfo::GetCurrentVersion() < 20170424)
		{
			if (ma_Step.GetSize() > 0)
				m_nStepNum = ma_Step[0]->GetStepsNum();
			for (int i = 0 ; i < ma_Step.GetSize() ; i++)
				ma_Step[i]->SetStepsNum(ma_Step[i]->GetStepsNum());
			SetPartSequenceNum();
		}

		if (MSVersionInfo::GetCurrentVersion() < 20190308)
		{
			CalculatePartHeight();
		}
	}
}

void MSGenStairG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	MSBaseBuilding* pBldg = GetBuilding();
	int DefaultMaterialID = pBldg->GetDefMaterial(MSMaterial::msConcrete)->m_ID;
	int MaterialID = DefaultMaterialID;

	if (ar.IsStoring())
	{
		long MType = mp_CompoM->GetType();
		ar << MType;
		if (mp_CompoM->GetMaterial() != nullptr)
			MaterialID = mp_CompoM->GetMaterial()->m_ID;
		ar << MaterialID;
		ar << mp_CompoM->m_Name;
	}
	else
	{
		long MType = 0;
		ar >> MType;

		MSStairM* pStairM = (MSStairM*)pBldg->CreateNewCompoM(MType);
		pStairM->m_nType = (MSCompoM::Type_MSCompoM)MType;

		ar >> MaterialID;
		pStairM->SetMaterial(pBldg->GetMaterial(MaterialID));
		if (pStairM->GetMaterial() == nullptr)
			pStairM->SetMaterial(pBldg->GetMaterial(DefaultMaterialID));
		ar >> pStairM->m_Name;
		pStairM->mp_Owner = pBldg;
		mp_CompoM = pStairM;
	}
}

void MSGenStairG::Dump(EFS::EFS_Buffer& buffer)
{
	MSPartCompoG::Dump(buffer);

	buffer << m_Name;
	buffer << m_isComponentStair;
	int NumStep = ma_Step.GetSize();
	buffer << NumStep;
	for( int iStair = 0; iStair < NumStep; iStair++)
	{
		MSStairPart* pStep = ma_Step[iStair];
		pStep->Dump(buffer);
	}		

	int NumLanding = ma_Landing.GetSize();
	buffer << NumLanding;
	for( int iLanding = 0; iLanding < NumLanding; iLanding++)
	{
		MSStairPart* pLanding = ma_Landing[iLanding];
		pLanding->Dump(buffer);
	}
	buffer << m_dBotOffset;

	if (mp_CompoM->m_ID != 0)
	{ // 이 경우 알려줄것 !!! - Gracias
		ASSERT(0);
	}

	buffer << m_dTopOffset;
	buffer << m_eStairType;
	int NumLandingHeight = ma_LandingHeight.size();
	buffer << NumLandingHeight;
	
	for(int i = 0 ; i < NumLandingHeight ; i++)
		buffer << ma_LandingHeight[i];

	buffer << m_bRemoveStartLanding;
	buffer << m_bRemoveEndLanding;

	buffer << m_dSetpHeight;
	buffer << m_nStepNum;
}

void MSGenStairG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSPartCompoG::Recovery(buffer);
	
	// Stair, Landing의 CompoM이 EachCompoM인 경우 Delete시키면 Undo/Redo시에 죽는다.
	// Dump 할때 CompoM 메모리 주소를 Dump하고 있음
	for (long iStair = 0; iStair < ma_Step.GetSize(); iStair++)
		ma_Step[iStair]->mp_CompoM = NULL;
	for (long iLanding = 0; iLanding < ma_Landing.GetSize(); iLanding++)
		ma_Landing[iLanding]->mp_CompoM = NULL;
	DeleteStairArr();
	DeleteLandingArr();

	buffer >> m_Name;
	buffer >> m_isComponentStair;
	int NumStep = 0;

	buffer >> NumStep;
	for( int iStair = 0; iStair < NumStep; iStair++)
	{
		MSStairPart* pStep = new MSStairPart();
		pStep->mp_Owner = mp_Owner;
		pStep->Recovery(buffer);
		AddStep(pStep);
	}		

	int NumLanding = 0;
	buffer >> NumLanding;
	for( int iLanding = 0; iLanding < NumLanding; iLanding++)
	{
		MSStairPart* pLanding = new MSStairPart();
		pLanding->mp_Owner = mp_Owner;
		pLanding->Recovery(buffer);
		AddLanding(pLanding);
	}
	buffer >> m_dBotOffset;

	if (mp_CompoM->m_ID != 0)
	{ // 이 경우 알려줄것 !!! - Gracias
		ASSERT(0);
	}

	buffer >> m_dTopOffset;
	buffer >> m_eStairType;
	int NumLandingHeight = 0;
	buffer >> NumLandingHeight;
	ma_LandingHeight.clear();
	for (int i = 0; i < NumLandingHeight; i++)
	{
		double dHeight = 0;
		buffer >> dHeight;
		ma_LandingHeight.push_back(dHeight);
	}

	buffer >> m_bRemoveStartLanding;
	buffer >> m_bRemoveEndLanding;

	buffer >> m_dSetpHeight;
	buffer >> m_nStepNum;
}

MSObject* MSGenStairG::Copy( bool bNewID /*= true*/ )
{
	MSGenStairG* pNewObj;

	pNewObj = new MSGenStairG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSGenStairG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msStairG) { ASSERT(0); return; }
	MSPartCompoG::CopyFromMe(pSource_in, bNewID);
	
	MSGenStairG* pSource = (MSGenStairG*)pSource_in;

	DeleteStairArr();
	DeleteLandingArr();

	mp_Boundary->CopyFrom(pSource->mp_Boundary);

	MSGenStairG* pStairSource = (MSGenStairG*)pSource;
	m_isComponentStair  = pStairSource->m_isComponentStair;	//계단타입 분류를 위해 추가함(HNJung) 
    m_Name = pStairSource->m_Name;

	int StepsCount = pStairSource->ma_Step.GetSize();
	for(int iStair = 0; iStair < StepsCount; iStair++)
	{
		MSStairPart* pPart = new MSStairPart();
		pPart->CopyFromMe(pStairSource->ma_Step[iStair]);
		AddStep(pPart);
	}

	int LandingCount = pStairSource->ma_Landing.GetSize();
	for(int iLanding = 0; iLanding < LandingCount; iLanding++)
	{
		MSStairPart* pPart = new MSStairPart();
		pPart->CopyFromMe(pStairSource->ma_Landing[iLanding]);
		AddLanding(pPart);
	}
	m_dBotOffset = pStairSource->m_dBotOffset;

	m_dTopOffset = pStairSource->m_dTopOffset;
	m_eStairType = pStairSource->m_eStairType;
	ma_LandingHeight = pStairSource->ma_LandingHeight;

	m_bRemoveStartLanding = pStairSource->m_bRemoveStartLanding;
	m_bRemoveEndLanding = pStairSource->m_bRemoveEndLanding;

	m_dSetpHeight = pStairSource->m_dSetpHeight;
	m_nStepNum = pStairSource->m_nStepNum;
}

void MSGenStairG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSPartCompoG::Init(bInConstructor);

	m_nStairFootNum = 0;
	m_nSupportType = 0;
	m_dBotOffset = 0;
	//m_dTopOffset = 0;
	m_pFirstWall = NULL;
	m_pLastWall = NULL;
	m_isComponentStair = TRUE;//m_b2Layer = TRUE;

	m_pLastWall = NULL;
	m_pFirstWall = NULL;
	m_pStartWall = NULL;
	m_pEndWall = NULL;

	m_dTopOffset = 0;
	m_eStairType = StairType_General;
	m_bRemoveStartLanding = false;
	m_bRemoveEndLanding = true;

	m_dSetpHeight = 200;
	m_nStepNum = 0;
}

void MSGenStairG::Translate( GM2DVector &pDelta )
{
	MSPartCompoG::Translate(pDelta);

	long nStepCount = ma_Step.GetSize();
	for(long iStep = 0; iStep < nStepCount; iStep++)
		ma_Step[iStep]->Translate(pDelta);

	long nLandingCount = ma_Landing.GetSize();
	for(long iLanding = 0; iLanding < nLandingCount; iLanding++)
		ma_Landing[iLanding]->Translate(pDelta);
}

void MSGenStairG::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSPartCompoG::MirrorByAxis(pAxis);

	long nStepCount = ma_Step.GetSize();
	for(long iStep = 0; iStep < nStepCount; iStep++)
		ma_Step[iStep]->MirrorByAxis(pAxis);

	long nLandingCount = ma_Landing.GetSize();
	for(long iLanding = 0; iLanding < nLandingCount; iLanding++)
		ma_Landing[iLanding]->MirrorByAxis(pAxis);
}

void MSGenStairG::RotateZ( double theta, GM2DVector *pCenter )
{
	MSPartCompoG::RotateZ(theta, pCenter);
	long nStepCount = ma_Step.GetSize();
	for(long iStep = 0; iStep < nStepCount; iStep++)
		ma_Step[iStep]->RotateZ(theta, pCenter);

	long nLandingCount = ma_Landing.GetSize();
	for(long iLanding = 0; iLanding < nLandingCount; iLanding++)
		ma_Landing[iLanding]->RotateZ(theta, pCenter);
}

//
//bool MSGenStairG::CalcVolume()
//{
//	if(m_dBoundaryArea == 0)//side area
//	{
//		MakeBoundary();
//		CalcBoundaryArea();	
//	}
//	try
//	{
//		if(this->ma_Joint.size() != 4)
//			AfxThrowUserException();
//
//		MSConnectivity* pJ1 = this->ma_Joint[0];
//		MSConnectivity* pJ2 = this->ma_Joint[1];
//		MSConnectivity* pJ3 = this->ma_Joint[2];
//		MSConnectivity* pJ4 = this->ma_Joint[3];
//
//		GMVector p1(pJ1->m_dXc,pJ1->m_dYc,pJ1->m_dZc), p2(pJ2->m_dXc,pJ2->m_dYc,pJ2->m_dZc);
//		GMVector p3(pJ3->m_dXc,pJ3->m_dYc,pJ3->m_dZc), p4(pJ4->m_dXc,pJ4->m_dYc,pJ4->m_dZc);
//		
//		double dWidth = p1.Distance(p2);
//		double dDepth = p2.Distance(p3);
//		
//		m_dVolume = m_dBoundaryArea*dWidth;// side area * dWidth/2.0 *2
//			#ifdef AFX_TARG_ENU
//	m_sVolumeCalcNote.Format(L"%.2f(Volume) = %.2f(Side Area) * %.2f(Stair Width)",m_dVolume/1E9, m_dBoundaryArea*2/1E6, dWidth/2.0/1E3);
//#else
//	m_sVolumeCalcNote.Format(L"%.2f(부피) = %.2f(계단 옆면 면적) * %.2f(계단 폭)",m_dVolume/1E9, m_dBoundaryArea*2/1E6, dWidth/2.0/1E3);
//#endif
//			}
//	
//	catch (...)
//	{
//		CLogger::strLog.Format("Error Failed in calculating the volume of StairG %d \n",this->m_ID);
//		CLogger::GetInstance()->WriteLog((LPCTSTR)CLogger::strLog);
//		return false;
//	}
//
//	
//
//	return true;
//}
//
//bool MSGenStairG::CalcFormWorkArea()
//{
//	if(m_dBoundaryArea == 0)
//	{
//		MakeBoundary();
//		CalcBoundaryArea();	
//	}
//
//	try
//	{
//		if(this->ma_Joint.size() != 4)
//			AfxThrowUserException();
//
//		MSConnectivity* pJ1 = this->ma_Joint[0];
//		MSConnectivity* pJ2 = this->ma_Joint[1];
//		MSConnectivity* pJ3 = this->ma_Joint[2];
//		MSConnectivity* pJ4 = this->ma_Joint[3];
//
//		GMVector p1(pJ1->m_dXc,pJ1->m_dYc,pJ1->m_dZc), p2(pJ2->m_dXc,pJ2->m_dYc,pJ2->m_dZc);
//		GMVector p3(pJ3->m_dXc,pJ3->m_dYc,pJ3->m_dZc), p4(pJ4->m_dXc,pJ4->m_dYc,pJ4->m_dZc);
//
//		double dWidth = p1.Distance(p2);//계단실 전체폭
//		double dDepth = p2.Distance(p3);//계단실 길이
//		double dHeight = ((MSFloor*)GetOwner())->m_dHeight;
//
//		MSFaceMemberM* stair_m = dynamic_cast<MSFaceMemberM*>(this->GetCompoM());
//		double dLLandLength = stair_m->GetTopLandLength();
//		double dULandLength = stair_m->GetBotLandLength();
//		double dStairProjLength = dDepth- dLLandLength - dULandLength;
//
//
//		double dLandingSideArea = 0; // Landing부의 옆면
//		if(stair_m != NULL)
//		{	
//			dLandingSideArea = stair_m->GetLandingThick()*(dLLandLength + dULandLength);
//		}
//
//
//		m_dFormWorkArea+= m_dBoundaryArea*2 - dLandingSideArea;// side area *2 (lower 1 upper 1)//한면씩 벽에 붙게 되므로 위 아래 한번만 계산, Landing부는 양옆모두 붙으므로 제거해야함
//		m_dFormWorkArea += dHeight* dWidth /2.0;// Step 앞면의 면적
//		double dStairBottomFace = sqrt(dStairProjLength*dStairProjLength + (dHeight/2)*(dHeight/2))*2*dWidth/2.0;//계단부 밑면
//		m_dFormWorkArea += dStairBottomFace;
//		double dLandingFace = dWidth*dLLandLength + dWidth*dULandLength;// 랜딩부 밑면
//		m_dFormWorkArea += dLandingFace;
//					#ifdef AFX_TARG_ENU
//	m_sFormWorkCalcNote.Format(L"%.2f(Formwork) = %.2f(Side Area)+%.2f(Floor level)x%.2f(Stair Width) + %.2f(Stair Bot. Area) + %.2f(Landing Bot. Area)",
//												m_dFormWorkArea/1E6, (m_dBoundaryArea*2 - dLandingSideArea)/1E6, dHeight/1E3, dWidth/2.0/1E3, dStairBottomFace/1E6, dLandingFace/1E6);
//
//#else
//	m_sFormWorkCalcNote.Format(L"%.2f(거푸집) = %.2f(계단 옆면 면적)+%.2f(층높이)x%.2f(계단폭) + %.2f(계단부 밑면넓이) + %.2f(랜딩부 밑면)",
//												m_dFormWorkArea/1E6, (m_dBoundaryArea*2 - dLandingSideArea)/1E6, dHeight/1E3, dWidth/2.0/1E3, dStairBottomFace/1E6, dLandingFace/1E6);
//
//#endif
//		
//	}
//
//	catch (...)
//	{
//		CLogger::strLog.Format("Error Failed in calculating the formwork area of StairG %d \n",this->m_ID);
//		CLogger::GetInstance()->WriteLog((LPCTSTR)CLogger::strLog);
//		return false;
//	}
//
//
//	return true;
//}
//
////by LSS  이 경우는 옆면 폴리곤 형상이다.
//bool MSGenStairG::MakeBoundary()
//{
//	if(ma_BoundaryPoints.size()>0)
//		return true;
//
//	try
//	{
//		if(ma_BoundaryPoints.size()>0) return true;
//
//		if(this->ma_Joint.size() != 4)
//			AfxThrowUserException();
//
//		if(this->GetCompoM() == NULL)
//			AfxThrowUserException();
//
//		MSFaceMemberM* stair_m = dynamic_cast<MSFaceMemberM*>(this->GetCompoM());
//
//		if(GetOwner() == NULL)
//			AfxThrowUserException();
//
//		double dHeight = ((MSFloor*)GetOwner())->m_dHeight;
//
//		if(dHeight<=1e-5)
//		{
//			CLogger::strLog.Format("Error StairG %d has invalid height value\n",this->m_ID);
//			CLogger::GetInstance()->WriteLog((LPCTSTR)CLogger::strLog);
//
//			AfxThrowUserException();
//		}
//
//		double dLLandLength = stair_m->GetTopLandLength();
//		double dULandLength = stair_m->GetBotLandLength();
//		int nStairFootNum = this->m_nStairFootNum;
//		double dUThick =stair_m->GetLandingThick();
//		double dLThick =dUThick;
//
//		MSConnectivity* pJ1 = this->ma_Joint[0];
//		MSConnectivity* pJ2 = this->ma_Joint[1];
//		MSConnectivity* pJ3 = this->ma_Joint[2];
//		MSConnectivity* pJ4 = this->ma_Joint[3];
//
//		GMVector v12(pJ2->m_dXc - pJ1->m_dXc, pJ2->m_dYc - pJ1->m_dYc, pJ2->m_dZc - pJ1->m_dZc);
//		GMVector v13(pJ3->m_dXc - pJ1->m_dXc, pJ3->m_dYc - pJ1->m_dYc, pJ3->m_dZc - pJ1->m_dZc);
//		GMVector vector_product = v12 & v13;
//
//		if(vector_product.m_Z < 0)
//		{
//			pJ1 = this->ma_Joint[1];
//			pJ2 = this->ma_Joint[0];
//			pJ3 = this->ma_Joint[3];
//			pJ4 = this->ma_Joint[2];
//		}
//
//
//		GMVector p1(pJ1->m_dXc,pJ1->m_dYc,pJ1->m_dZc), p2(pJ2->m_dXc,pJ2->m_dYc,pJ2->m_dZc);
//		GMVector p3(pJ3->m_dXc,pJ3->m_dYc,pJ3->m_dZc), p4(pJ4->m_dXc,pJ4->m_dYc,pJ4->m_dZc);
//		GMVector pFrontCenter( (pJ1->m_dXc + pJ2->m_dXc)/2.0, (pJ1->m_dYc + pJ2->m_dYc)/2.0, (pJ1->m_dYc + pJ2->m_dYc)/2.0) ;
//		GMVector pEndStart(p4.m_X, p4.m_Y, p4.m_Z + dHeight/2.0);
//		GMVector pEndCenter( (pJ3->m_dXc + pJ4->m_dXc)/2.0, (pJ3->m_dYc + pJ4->m_dYc)/2.0, (pJ3->m_dYc + pJ4->m_dYc)/2.0) ;
//		pEndCenter.m_Z += dHeight/2.0;
//
//
//		double dWidth = p1.Distance(p2);
//		double dDepth = p2.Distance(p3);
//		double dStairProjLength = dDepth- dLLandLength - dULandLength;
//
//		if(dStairProjLength<=0)
//		{
//			CLogger::strLog.Format("Error StairG %d has invalid shape, too long landing length\n",this->m_ID);
//			CLogger::GetInstance()->WriteLog((LPCTSTR)CLogger::strLog);
//
//			AfxThrowUserException();
//		}
//
//		if(nStairFootNum<=1)
//		{
//			CLogger::strLog.Format("Error StairG %d has invalid FootStep Number\n",this->m_ID);
//			CLogger::GetInstance()->WriteLog((LPCTSTR)CLogger::strLog);
//
//			nStairFootNum = (int)(dStairProjLength/230);
//		}
//
//		ma_BoundaryPoints.push_back(GM2DVector(0,0));
//		ma_BoundaryPoints.push_back(GM2DVector(dLLandLength,0));
//		ma_BoundaryPoints.push_back(GM2DVector(dLLandLength+ dStairProjLength,dHeight/2.0));
//		ma_BoundaryPoints.push_back(GM2DVector(dDepth,dHeight/2.0));
//		ma_BoundaryPoints.push_back(GM2DVector(dDepth,dHeight/2.0 + dUThick));
//
//		//pntArray.Append(ON_3dPoint(dLLandLength+ dStairProjLength,dHeight/2.0 + dUThick,0));
//		for(int i=0; i<nStairFootNum; i++)
//		{
//			ma_BoundaryPoints.push_back(GM2DVector(dLLandLength+ dStairProjLength - dStairProjLength/nStairFootNum*(i+1), 
//				dUThick+ dHeight/2.0 - dHeight/2.0/nStairFootNum*i));
//			ma_BoundaryPoints.push_back(GM2DVector(dLLandLength+ dStairProjLength - dStairProjLength/nStairFootNum*(i+1), 
//				dUThick+ dHeight/2.0 - dHeight/2.0/nStairFootNum*(i+1)));
//		}
//		//pntArray.Append(ON_3dPoint(dLLandLength,dLThick,0));
//
//		ma_BoundaryPoints.push_back(GM2DVector(0,dLThick));
//		
//
//	}
//	
//	catch (...)
//	{	
//		ma_BoundaryPoints.clear();
//
//		CLogger::strLog.Format("Error in making stair side boundary shape from CompoG %d !\r\n",this->m_ID);
//		CLogger::GetInstance()->WriteLog((LPCTSTR)CLogger::strLog);
//
//		return false;
//	}
//	
//
//	return true;
//}
//
////by LSS formwork not used in stair
////
//bool MSGenStairG::CalcBoundaryLengthForFormwork()
//{
//
//	return true;
//}

void MSGenStairG::SetInnerPGonCCW()
{
	GM2DVector pJ1 = *this->ma_Vec[0];
	GM2DVector pJ2 = *this->ma_Vec[1];
	GM2DVector pJ3 = *this->ma_Vec[2];
	GM2DVector pJ4 = *this->ma_Vec[3];

	GM2DLineVector FistLine;
	GM2DLineVector LastLine;
	GM2DLineVector StartLine;
	GM2DLineVector EndLine;

	//주변 벽을 찾는다.
	FistLine.m_P0 = pJ2;
	FistLine.m_P1 = pJ3;
	m_pFirstWall = FindWallMemberG(FistLine);
	
	LastLine.m_P0 = pJ4;
	LastLine.m_P1 = pJ1;
	m_pLastWall = FindWallMemberG(LastLine);

	StartLine.m_P0 = pJ1;
	StartLine.m_P1 = pJ2;
	m_pStartWall = FindWallMemberG(StartLine);
	
	EndLine.m_P0 = pJ3;
	EndLine.m_P1 = pJ4;
	m_pEndWall = FindWallMemberG(EndLine);
	
	GMPolygon StairPoly(true);
	GM2DVector crossVec;
	double dMyLoc, dUrLoc;
	LastLine.Intersect(StartLine,crossVec,&dMyLoc,&dUrLoc);
	StairPoly.AddCopy(crossVec);
	FistLine.Intersect(StartLine,crossVec,&dMyLoc,&dUrLoc);
	StairPoly.AddCopy(crossVec);
	FistLine.Intersect(EndLine,crossVec,&dMyLoc,&dUrLoc);
	StairPoly.AddCopy(crossVec);
	LastLine.Intersect(EndLine,crossVec,&dMyLoc,&dUrLoc);
	StairPoly.AddCopy(crossVec);
	
	m_InnerPoly.DeleteVecArr();
	if(StairPoly.CheckPolygon()==-1)
	{
		m_InnerPoly.Add(new GMVector(pJ1.m_X,pJ1.m_Y));
		m_InnerPoly.Add(new GMVector(pJ2.m_X,pJ2.m_Y));
		m_InnerPoly.Add(new GMVector(pJ3.m_X,pJ3.m_Y));
		m_InnerPoly.Add(new GMVector(pJ4.m_X,pJ4.m_Y));
		if(m_InnerPoly.CheckPolygon()==-1)
			ASSERT(0);
	}
	else
		m_InnerPoly = StairPoly;
}
//두점에 연결되는 벽중 두꺼운 벽을 찾아 리턴-by hana
MSWallMemberG* MSGenStairG::FindWallMemberG(GM2DLineVector InnerLine)
{
	MSWallMemberG* pSptWall = NULL;
	MSFloor* pFloor = GetFloor();
	if(pFloor == NULL)
		return pSptWall;
	
	double dMaxWidth = 0;
	vector<MSCompoG*> WallGArr = MSCompoGUtil::FindCompoG(pFloor, &InnerLine, FindType_MSElement::msWallG);
	for(long i = 0 ; i < WallGArr.size() ; i++)
	{
		MSWallMemberG* pWall = (MSWallMemberG*)WallGArr[i];
		double dWidth = pWall->GetWidth();
		if(dWidth > dMaxWidth)
		{
			dMaxWidth = dWidth;
			pSptWall = pWall;
		}
	}
 
	return pSptWall;
}

double MSGenStairG::GetHeight()
{
	long NumLand = ma_Landing.GetSize();
	long NumStep = ma_Step.GetSize();
	double dHeight = 0.;

	for(long iLand = 0 ; iLand < NumLand ; iLand++)
	{
		MSStairPart* pLand = ma_Landing[iLand];
		if(pLand == NULL) continue;
		dHeight = max(dHeight, pLand->GetBottomLevel());
		dHeight = max(dHeight, pLand->GetTopLevel());
	}

	for(long iStep = 0 ; iStep < NumStep ; iStep++)
	{
		MSStairPart* pStep = ma_Step[iStep];
		if(pStep == NULL) continue;
		dHeight = max(dHeight, pStep->GetBottomLevel());
		dHeight = max(dHeight, pStep->GetTopLevel());
	}

	return dHeight;
}

double MSGenStairG::GetOffsetLowZ()
{
	//if(mp_SFloor)
	//	mp_SFloor->
	//long numJoint = ma_Joint.size();
	//for(long iJ = 0;iJ<numJoint;iJ++)
	//{
	//	MSConnectivity* pJoint = ma_Joint[iJ];
	//	if(pJoint == NULL)	continue;
	//	return pJoint->m_dZc + m_dBotOffset;
	//}
	return m_dBotOffset;
}

long MSGenStairG::GetStairLandingJointCnt()
{
	//보완 해야함 중복 갯수 체크 안함
	long  nJointCnt = 0;
	//long NumStep = ma_Step.GetSize();
	//for(long iStep = 0 ; iStep < NumStep ; iStep++)
	//	nJointCnt += ma_Step[iStep]->m_pPolyline->ma_Point.GetCount() -1;

	long NumLanding = ma_Landing.GetSize();
	for(long iLanding = 0 ; iLanding < NumLanding ; iLanding++)
		nJointCnt += ma_Landing[iLanding]->m_pPolyline->ma_Point.GetCount() -1;

	return nJointCnt;
}

GM2DVector* MSGenStairG::GetVec(long Index)
{
	return NULL;
}

bool MSGenStairG::IsUsedMID(long MID)
{
	long NumLanding = ma_Landing.GetSize();
	for(long iLanding = 0; iLanding < NumLanding; iLanding++)
	{
		MSStairPart* pLanding = ma_Landing[iLanding];
		if( pLanding->mp_CompoM && pLanding->mp_CompoM->m_ID == MID)
			return true;
	}

	long NumStep = ma_Step.GetSize();
	for(long iStep = 0; iStep < NumStep; iStep++)
	{
		MSStairPart* pStep = ma_Step[iStep];
		if( pStep->mp_CompoM && pStep->mp_CompoM->m_ID == MID)
			return true;
	}
	return false;
}

bool MSGenStairG::MakeBoundary()
{
	long NumVec = ma_Vec.size();
	// 자유 형상 계단인경우 ma_Vec 값이 없는 상태임
	if (NumVec == 0)
	{
		double dStairMinX = 0, dStairMinY = 0;
		double dStairMaxX = 0, dStairMaxY = 0;
		
		bool isFirst = true;
		vector<MSStairPart*> StairParts = GetStairPart();

		for (int i = 0; i < StairParts.size(); i++)
		{
			MSStairPart* pPart = StairParts[i];
			double dMinX, dMinY, dMaxX, dMaxY;
			pPart->m_pPolyline->GetXYMinMax(dMinX, dMinY, dMaxX, dMaxY);
			if (isFirst)
			{
				dStairMinX = dMinX;
				dStairMinY = dMinY;
				dStairMaxX = dMaxX;
				dStairMaxY = dMaxY;
				isFirst = false;
			}
			else
			{
				dStairMinX = min(dStairMinX,dMinX);
				dStairMinY = min(dStairMinY,dMinY);
				dStairMaxX = max(dStairMaxX,dMaxX);
				dStairMaxY = max(dStairMaxY,dMaxY);
			}
		}

		ma_Vec.push_back(new GM2DVector(dStairMinX, dStairMinY));
		ma_Vec.push_back(new GM2DVector(dStairMaxX, dStairMinY));
		ma_Vec.push_back(new GM2DVector(dStairMaxX, dStairMaxY));
		ma_Vec.push_back(new GM2DVector(dStairMinX, dStairMaxY));
		NumVec = ma_Vec.size();
	}


	if(NumVec<3) // by ckkim : ma_Vec가 세팅되지 않네요. 검토 요망 150102
		return false; 
	GM2DPolyline* pBoundary = mp_Boundary;
	pBoundary->ResetPolyline();
	for(long iVec = 0 ; iVec < NumVec ; iVec++)
	{
		pBoundary->AddCopy(*ma_Vec[iVec]);
		pBoundary->ma_Bulge.Add(0);
	}
	pBoundary->AddCopy(*ma_Vec[0]);
	pBoundary->MakeCCW();
	return true;
}

void MSGenStairG::ReSetVec()
{
	GM2DBound MyBound;
	long NumLanding = ma_Landing.GetSize();
	for(long iLand = 0 ; iLand < NumLanding ; iLand++)
	{
		MSStairPart* pLand =  ma_Landing[iLand];
		if(pLand !=NULL && pLand->m_pPolyline != NULL)
			MyBound.Add2DPolyline(pLand->m_pPolyline);
	}

	long NumStep = ma_Step.GetSize();
	for(long iStep = 0 ; iStep < NumStep ; iStep++)
	{
		MSStairPart* pStep = ma_Step[iStep];
		if(pStep != NULL && pStep->m_pPolyline != NULL)
			MyBound.Add2DPolyline(pStep->m_pPolyline);
	}

	long NumVec = MyBound.ma_Point.GetSize();
	for(long iVec = 0 ; iVec < NumVec ; iVec++)
	{
		GM2DVector* pVec = (GM2DVector*)MyBound.ma_Point[iVec]->Copy();
		ma_Vec.push_back(pVec);
	}

}

bool MSGenStairG::isComponentStair()
{
	return (bool)m_isComponentStair;
}

void MSGenStairG::CalcBoundRectBeforeAddQuadTree()
{
	GM2DBound MyBound;
	GetProfile()->Get2DBound(MyBound);
	GM2DVector* pOrigin = MyBound.GetBottomLeft();
	GM2DVector* pCorner = MyBound.GetTopRight();

	if (pOrigin == NULL || pCorner == NULL) return;
	m_RectForQuadTree.m_Origin.m_X = pOrigin->m_X; m_RectForQuadTree.m_Origin.m_Y = pOrigin->m_Y;
	m_RectForQuadTree.m_Corner.m_X = pCorner->m_X; m_RectForQuadTree.m_Corner.m_Y = pCorner->m_Y;
}

bool MSGenStairG::VisibleInRect(GM2DRectangle* pRect)
{
	if(GetProfile() == NULL)
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
	
	if(GetProfile()->OverlapEx(UrPoly))	return true;

	return false;
}

bool MSGenStairG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal /*= true */, bool bCheckLevelZone )
{
	/*
	double dTopOffSetZ = 0, dBotOffSetZ = 0;
	//dTopOffSetZ = m_dTopOffset;
	//dBotOffSetZ = m_dBotOffset;
	double dFloorLevel = 0;
	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		dFloorLevel = GetSlabLevel() + ((MSFloor*)mp_Owner)->GetSlabHeight();
	}
	else
		dFloorLevel = GetSlabLevel();
	TopLevel = dFloorLevel + dBotOffSetZ;
	BottomLevel = dFloorLevel + dBotOffSetZ - GetDepth();
	*/

	GetZLevelsByPart(TopLevel, BottomLevel);





	// GetZLevels 체크시 가장 아래에 있는 파트의 두께를 다시 더해준다. 현재 계단 랜딩 부분은 오버랩 되는 구조임
	MSStairPart* pLowerPart = GetLowerPart();
	if(pLowerPart != NULL)
		BottomLevel += pLowerPart->m_Thick;

	return true;
}

bool MSGenStairG::GetZLevelsByPart(double& TopLevel, double& BottomLevel)
{
	double dTopLevel = 0;
	double dBotLevel = 0;

	double dMaxTopLevel = 0;
	double dMinBotLevel = 0;

	CTypedPtrArray<CObArray, MSStairPart*> PartArr;
	for (long i = 0; i < ma_Landing.GetSize(); i++)
		PartArr.Add(ma_Landing[i]);
	for (long i = 0; i < ma_Step.GetSize(); i++)
		PartArr.Add(ma_Step[i]);

	if (PartArr.GetSize() == 0)
		return false;

	for (long i = 0; i < PartArr.GetSize(); i++)
	{
		MSStairPart* pPart = PartArr[i];
		if (i == 0)
			pPart->GetZLevels(dMaxTopLevel, dMinBotLevel);
		else
		{
			pPart->GetZLevels(dTopLevel, dBotLevel);
			dMaxTopLevel = max(dMaxTopLevel, dTopLevel);
			dMinBotLevel = min(dMinBotLevel, dBotLevel);
		}
	}

	TopLevel = dMaxTopLevel;
	BottomLevel = dMinBotLevel;

	return true;
}

double MSGenStairG::GetDepth()
{
	double dTopLevel = 0;
	double dBotLevel = 0;

	double dMaxTopLevel = 0;
	double dMinBotLevel = 0;


	CTypedPtrArray<CObArray, MSStairPart*> PartArr;
	for(long i = 0 ; i < ma_Landing.GetSize() ; i++)
		PartArr.Add(ma_Landing[i]);
	for(long i = 0 ; i < ma_Step.GetSize() ; i++)
		PartArr.Add(ma_Step[i]);


	for(long i = 0 ; i < PartArr.GetSize() ; i++)
	{
		MSStairPart* pPart = PartArr[i];
		if(i == 0)
			pPart->GetZLevels(dMaxTopLevel, dMinBotLevel);
		else
		{
			pPart->GetZLevels(dTopLevel, dBotLevel);
			dMaxTopLevel = max(dMaxTopLevel, dTopLevel);
			dMinBotLevel = min(dMinBotLevel, dBotLevel);
		}
	}
	
	double dDepth = fabs(dMaxTopLevel - dMinBotLevel);

	return dDepth;

}

double MSGenStairG::GetDepthWithOutLowPart()
{
	double dHeight = 0;
	dHeight = GetDepth();
	MSStairPart* pLowerPart = GetLowerPart();
	if (pLowerPart != NULL)
		dHeight -= pLowerPart->m_Thick;
	return dHeight;
}

GM2DPolyline* MSGenStairG::GetProfile()
{
	if (mp_Boundary->GetNumLines() < 1)
		MakeBoundary();
	return mp_Boundary;
}

MSStairPart* MSGenStairG::GetLowerPart()
{
	CTypedPtrArray<CObArray, MSStairPart*> PartArr;
	for (long i = 0; i < ma_Landing.GetSize(); i++)
		PartArr.Add(ma_Landing[i]);
	for (long i = 0; i < ma_Step.GetSize(); i++)
		PartArr.Add(ma_Step[i]);

	return GetEndStairPart(PartArr, true);
}

MSStairPart* MSGenStairG::GetUpperPart()
{
	CTypedPtrArray<CObArray, MSStairPart*> PartArr;
	for (long i = 0; i < ma_Landing.GetSize(); i++)
		PartArr.Add(ma_Landing[i]);
	for (long i = 0; i < ma_Step.GetSize(); i++)
		PartArr.Add(ma_Step[i]);

	return GetEndStairPart(PartArr, false);
}

MSStairPart* MSGenStairG::GetEndStairPart(CTypedPtrArray<CObArray, MSStairPart*>& PartArr, bool IsLowerPart)
{
	MSStairPart* pRValPart = NULL;

	for (long i = 0; i < PartArr.GetSize(); i++)
	{
		MSStairPart* pPart = PartArr[i];

		if (pRValPart == NULL)
		{
			pRValPart = pPart;
			continue;
		}
		
		if (IsLowerPart)
		{
			if (min(pRValPart->GetBottomLevel(), pRValPart->GetTopLevel()) > min(pPart->GetBottomLevel(), pPart->GetTopLevel()))
				pRValPart = pPart;
		}
		else
		{
			if (max(pRValPart->GetBottomLevel(), pRValPart->GetTopLevel()) < max(pPart->GetBottomLevel(), pPart->GetTopLevel()))
				pRValPart = pPart;
		}
	}

	return pRValPart;
}


MSMaterial* MSGenStairG::GetMaterial()
{
	MSMaterial* ret = GetCompoM()->GetMaterial();

	CTypedPtrArray<CObArray, MSStairPart*> aParts;
	for (int i = 0; i < ma_Landing.GetSize(); ++i)
		aParts.Add(ma_Landing[i]);
	for (int i = 0; i < ma_Step.GetSize(); ++i)
		aParts.Add(ma_Step[i]);

	double fck_min = 0;
	for (int i = 0; i < aParts.GetSize(); ++i)
	{
		MSMaterial* curMaterial = aParts[i]->GetMaterial();
		if (curMaterial->GetType() == MSMaterial::msConcrete)
		{
			double fck = ((MSConcrete*)curMaterial)->GetFck();
			if (i==0 || fck < fck_min)
			{
				fck_min = fck;
				ret = curMaterial;
			}
		}
	}

	return ret;
}

MSStairPart* MSGenStairG::FindStairPartByPoly(GM2DPolyline* pPoly)
{
	if (pPoly == NULL)
		return NULL;

	for (long iPart = 0; iPart < ma_Landing.GetSize(); iPart++)
	{
		MSStairPart* pPart = ma_Landing[iPart];
		if (pPart->m_pPolyline->SamePolyLine(pPoly))
			return pPart;
	}

	for (long iPart = 0; iPart < ma_Step.GetSize(); iPart++)
	{
		MSStairPart* pPart = ma_Step[iPart];
		if (pPart->m_pPolyline->SamePolyLine(pPoly))
			return pPart;
		GM2DVector center = pPoly->MassCenter();
		if(pPart->m_pPolyline->Contains(&center) == -1)
			return pPart;
	}
	return NULL;
}

void MSGenStairG::MakeCCW()
{
	MSCompoG::MakeCCW();

	for (int i = 0; i < ma_Step.GetSize(); i++)
		ma_Step[i]->MakeCCW();
	for (int j = 0; i < ma_Landing.GetSize(); i++)
		ma_Landing[i]->MakeCCW();
}

void MSGenStairG::SetOwner(MSContainer* val)
{
	MSCompoG::SetOwner(val);
	for (int i = 0; i < ma_Step.GetSize(); i++)
		ma_Step[i]->SetOwner(val);
	for (int j = 0; i < ma_Landing.GetSize(); i++)
		ma_Landing[i]->SetOwner(val);
}

void MSGenStairG::CheckPartProfile()
{
	for (int i = ma_Step.GetSize() - 1; i >= 0; i--)
	{
		MSStairPart* pStep = ma_Step[i];
		if(pStep == NULL) 
			continue;
		if (pStep->m_pPolyline->CheckPolygon(false) == FALSE)
		{
			delete pStep;
			ma_Step.RemoveAt(i);
		}
	}
	for (int i = ma_Landing.GetSize() - 1; i >= 0; i--)
	{
		MSStairPart* pLanding = ma_Landing[i];
		if (pLanding == NULL)
			continue;
		if (pLanding->m_pPolyline->CheckPolygon(false) == FALSE)
		{
			delete pLanding;
			ma_Landing.RemoveAt(i);
		}
	}
}


bool MSGenStairG::CalculatePartHeight_General()
{
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return false;

	int NumLanding = ma_Landing.GetSize();
	double dStartLevel = m_dBotOffset;
	double dEndLevel = pCurFloor->GetSlabHeight() + m_dTopOffset;

	// Cli에서 받는 계단의 갯수 2 , 랜딩의 갯수 1~3
	
	if (ma_Step.GetSize() != 2 || ma_LandingHeight.size() != 1)
		return false;

	if (m_bRemoveStartLanding == false)
	{
		ma_Landing[0]->SetBottomLevel(dStartLevel);
		ma_Landing[0]->SetTopLevel(dStartLevel);
		ma_Landing[1]->SetBottomLevel(ma_LandingHeight[0]);
		ma_Landing[1]->SetTopLevel(ma_LandingHeight[0]);
	}
	else
	{
		ma_Landing[0]->SetBottomLevel(ma_LandingHeight[0]);
		ma_Landing[0]->SetTopLevel(ma_LandingHeight[0]);
	}

	if (m_bRemoveEndLanding == false)
	{
		ma_Landing[NumLanding - 1]->SetBottomLevel(dEndLevel);
		ma_Landing[NumLanding - 1]->SetTopLevel(dEndLevel);
	}

	ma_Step[0]->SetBottomLevel(dStartLevel);
	ma_Step[0]->SetTopLevel(ma_LandingHeight[0]);

	ma_Step[1]->SetBottomLevel(ma_LandingHeight[0]);
	ma_Step[1]->SetTopLevel(dEndLevel);
	

	if (m_nStepNum == 0)
		SetStepHeight(m_dSetpHeight);
	else
		SetStepNum(m_nStepNum);
	SetPartSequenceNum();
	return true;
}

bool MSGenStairG::CalculatePartHeight_Double()
{
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return false;

	int NumLanding = ma_Landing.GetSize();
	double dStartLevel = m_dBotOffset;
	double dEndLevel = pCurFloor->GetSlabHeight() + m_dTopOffset;

	// Cli에서 받는 계단의 갯수 4 , 랜딩의 갯수 3~5
	if (ma_Step.GetSize() != 4 && ma_LandingHeight.size() != 3)
		return false;

	int nIdx = 0;
	if (m_bRemoveStartLanding == false)
	{
		ma_Landing[0]->SetBottomLevel(dStartLevel);
		ma_Landing[0]->SetTopLevel(dStartLevel);
		nIdx++;
	}
	for (int i = 0; i < ma_LandingHeight.size(); i++)
	{
		ma_Landing[nIdx + i]->SetBottomLevel(ma_LandingHeight[i]);
		ma_Landing[nIdx + i]->SetTopLevel(ma_LandingHeight[i]);
	}

	if (m_bRemoveEndLanding == false)
	{
		ma_Landing[NumLanding - 1]->SetBottomLevel(dEndLevel);
		ma_Landing[NumLanding - 1]->SetTopLevel(dEndLevel);
	}

	if (ma_Step.GetSize() == 2)
	{
		AddStep((MSStairPart*)ma_Step[0]->Copy());
		AddStep((MSStairPart*)ma_Step[1]->Copy());
	}

	ma_Step[0]->SetBottomLevel(dStartLevel);
	ma_Step[0]->SetTopLevel(ma_LandingHeight[0]);

	ma_Step[1]->SetBottomLevel(ma_LandingHeight[0]);
	ma_Step[1]->SetTopLevel(ma_LandingHeight[1]);

	ma_Step[2]->SetBottomLevel(ma_LandingHeight[1]);
	ma_Step[2]->SetTopLevel(ma_LandingHeight[2]);

	ma_Step[3]->SetBottomLevel(ma_LandingHeight[2]);
	ma_Step[3]->SetTopLevel(dEndLevel);

	if (m_nStepNum == 0)
		SetStepHeight(m_dSetpHeight);
	else
		SetStepNum(m_nStepNum);

	SetPartSequenceNum();
	return true;
}

bool MSGenStairG::CalculatePartHeight_Line()
{
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return false;

	int NumLanding = ma_Landing.GetSize();
	double dStartLevel = m_dBotOffset;
	double dEndLevel = pCurFloor->GetSlabHeight() + m_dTopOffset;

	vector<MSStairPart*> PartArr;

	int NumStep = ma_Step.GetSize();
	double dLandingHeight = 0;
	if (NumStep > 0)
		dLandingHeight = pCurFloor->GetSlabHeight() / NumStep;

	if (NumLanding > 0)
	{
		int nPreLandingSeq = ma_Landing[0]->GetSequence();
		int nLandingSeq = nPreLandingSeq == 0 ? 0 : 1;
		for (int i = 0; i < NumLanding; i++)
		{
			if (nPreLandingSeq != ma_Landing[i]->GetSequence())
			{
				nLandingSeq++;
				nPreLandingSeq = ma_Landing[i]->GetSequence();
			}
			if (nLandingSeq == 0)
			{
				ma_Landing[i]->SetBottomLevel(dStartLevel);
				ma_Landing[i]->SetTopLevel(dStartLevel);
			}
			else
			{
				ma_Landing[i]->SetBottomLevel(dLandingHeight * nLandingSeq);
				ma_Landing[i]->SetTopLevel(dLandingHeight * nLandingSeq);
			}

			PartArr.push_back(ma_Landing[i]);
		}
	}

	for (int i = 0; i < NumStep; i++)
	{
		if (i == 0)
			ma_Step[i]->SetBottomLevel(dStartLevel);
		else
			ma_Step[i]->SetBottomLevel(dLandingHeight * i);
		if (i == NumStep - 1)
			ma_Step[i]->SetTopLevel(dEndLevel);
		else
			ma_Step[i]->SetTopLevel(dLandingHeight * (i + 1));

		ma_Step[i]->SetStepsNum(ma_Step[i]->GetStepsNum());
		PartArr.push_back(ma_Step[i]);
	}

	sort(PartArr.begin(), PartArr.end(),
		[](MSStairPart* a, MSStairPart* b)->bool {
		if (a->GetSequence() == b->GetSequence())
			return false;
		else
			return a->GetSequence() < b->GetSequence(); 
	}
	);

	int NumPart = PartArr.size();
	if (NumPart > 0 && PartArr[NumPart - 1]->IsLanding() == true)
	{
		PartArr[NumPart - 1]->SetBottomLevel(dEndLevel);
		PartArr[NumPart - 1]->SetTopLevel(dEndLevel);
	}

	vector<GM2DPolyline*> ExtendProfileArr;
	GM2DPolyline* pPoly = nullptr;
	for (int i = 0; i < PartArr.size(); i++)
	{
		if (i == 0)
			pPoly = PartArr[i]->GetExtendLineProfile(MSStairPart::ExtendType_End, PartArr[i]->GetWidth() * 2);
		else if (i == PartArr.size() - 1)
			pPoly = PartArr[i]->GetExtendLineProfile(MSStairPart::ExtendType_Start, PartArr[i]->GetWidth() * 2);
		else
			pPoly = PartArr[i]->GetExtendLineProfile(MSStairPart::ExtendType_Both, PartArr[i]->GetWidth() * 2);
		ExtendProfileArr.push_back(pPoly);
		PartArr[i]->CalcPathCurve();
	}

	vector<GM2DPolyline*> JoinedPoly;
	vector<GM2DPolyline*> JoinedPoly2;
	for (int i = 0; i < PartArr.size(); i++)
	{
		bool IsPreLanding = false;
		bool IsCurLanding = PartArr[i]->IsLanding();
		bool IsNextLanding = false;
		if (IsCurLanding == false)
			continue;

		GM2DPolyline* pCurExtendPoly = nullptr, *pPreExtendPoly = nullptr, *pNextExtendPoly = nullptr;
		GM2DPolyline* pCurOrgPoly = nullptr, *pPreOrgPoly = nullptr, *pNextOrgPoly = nullptr;
		GM2DPolyline* pPreIntersectPoly = nullptr, *pNextIntersectPoly = nullptr;
		pCurExtendPoly = ExtendProfileArr[i];
		pCurOrgPoly = PartArr[i]->m_pPolyline;

		GM2DVector OrgCenVec = pCurOrgPoly->MassCenter();
		bool IsChangExtendPoly = false;

		GM2DCurve* pCurPathCurve = PartArr[i]->GetPathCurve();
		CString strTrimFailMsg = _T("계단, 스텝 트림에 실패 했습니다.");
		
		if (i != 0)
		{
			IsPreLanding = PartArr[i - 1]->IsLanding();
			GM2DCurve* pPrePathCurve = PartArr[i - 1]->GetPathCurve();
			if (pCurPathCurve->IsParallel4DTol(pPrePathCurve) == 0)
			{
				pPreExtendPoly = ExtendProfileArr[i - 1];
				pPreOrgPoly = PartArr[i - 1]->m_pPolyline;
			}
		}

		if (i != PartArr.size() - 1)
		{
			IsNextLanding = PartArr[i + 1]->IsLanding();
			GM2DCurve* pNextPathCurve = PartArr[i + 1]->GetPathCurve();
			if (pCurPathCurve->IsParallel4DTol(pNextPathCurve) == 0)
			{
				pNextExtendPoly = ExtendProfileArr[i + 1];
				pNextOrgPoly = PartArr[i + 1]->m_pPolyline;
			}
		}

		if (pPreExtendPoly != nullptr && IsPreLanding == false)
		{
			pCurExtendPoly->BoolOperation('I', pPreExtendPoly, JoinedPoly, DTOL_MODELING);
			if (JoinedPoly.size() > 0)
			{
				JoinedPoly[0]->CheckAndFixFoldedEdges(DTOL_MODELING);
				if (JoinedPoly[0]->CheckPolygon() == TRUE)
				{
					pPreIntersectPoly = new GM2DPolyline(*JoinedPoly[0]);

					pCurExtendPoly->BoolOperation('A', JoinedPoly[0], JoinedPoly2, DTOL_MODELING);
					if (JoinedPoly2.size() > 0)
					{
						for (int iJoined = 0; iJoined < JoinedPoly2.size(); iJoined++)
						{
							if (JoinedPoly2[iJoined]->Contains(&OrgCenVec) == -1)
							{
								pCurExtendPoly->CopyFrom(JoinedPoly2[iJoined]);
								IsChangExtendPoly = true;
								break;
							}
						}
					}
					
				}
				else
					StatusReporterWrapper::OutputMessage(strTrimFailMsg);
			}

			if (pPreIntersectPoly != nullptr) // Aline으로 인해 이빠지는 폴리라인이 발생 
			{
				pPreExtendPoly->BoolOperation('A', pPreIntersectPoly, JoinedPoly, DTOL_MODELING);
				
				GM2DPolyline* pStepExtendedPoly = nullptr;
				GM2DVector PreOrgCenVec = pPreOrgPoly->MassCenter();
				for (int iJoined = 0; iJoined < JoinedPoly.size(); iJoined++)
				{
					if (JoinedPoly[iJoined]->Contains(&PreOrgCenVec) == -1)
					{
						JoinedPoly[iJoined]->BoolOperation('A', pPreOrgPoly, JoinedPoly2, DTOL_MODELING);
						for (int iPoly = 0; iPoly < JoinedPoly2.size(); iPoly++)
						{
							if (JoinedPoly2[iPoly]->OverlapEx(*pPreIntersectPoly) == true)
							{
								pStepExtendedPoly = JoinedPoly2[iPoly];
								break;
							}
						}
					}
				}
				if (pStepExtendedPoly != nullptr)
				{
					pStepExtendedPoly->BoolOperation('U', pPreIntersectPoly, JoinedPoly, DTOL_MODELING);
					if (JoinedPoly.size() == 1)
						pPreIntersectPoly->CopyFrom(JoinedPoly[0]);
				}

			}
		}

		if (pNextExtendPoly != nullptr)
		{
			pCurExtendPoly->BoolOperation('I', pNextExtendPoly, JoinedPoly, DTOL_MODELING);
			if (JoinedPoly.size() > 0)
			{
				JoinedPoly[0]->CheckAndFixFoldedEdges(DTOL_MODELING);
				if (JoinedPoly[0]->CheckPolygon() == TRUE)
				{
					pNextIntersectPoly = new GM2DPolyline(*JoinedPoly[0]);

					pCurExtendPoly->BoolOperation('A', JoinedPoly[0], JoinedPoly2, DTOL_MODELING);
					if (JoinedPoly2.size() > 0)
					{
						for (int iJoined = 0; iJoined < JoinedPoly2.size(); iJoined++)
						{
							if (JoinedPoly2[iJoined]->Contains(&OrgCenVec) == -1)
							{
								pCurExtendPoly->CopyFrom(JoinedPoly2[iJoined]);
								IsChangExtendPoly = true;
								break;
							}
						}
					}
				}
				else
					StatusReporterWrapper::OutputMessage(strTrimFailMsg);
			}

			if (IsNextLanding == false && pNextIntersectPoly) // Aline으로 인해 이빠지는 폴리라인이 발생 
			{
				pNextExtendPoly->BoolOperation('A', pNextIntersectPoly, JoinedPoly, DTOL_MODELING);
				
				GM2DPolyline* pStepExtendedPoly = nullptr;
				GM2DVector NextOrgCenVec = pNextOrgPoly->MassCenter();
				for (int iJoined = 0; iJoined < JoinedPoly.size(); iJoined++)
				{
					if (JoinedPoly[iJoined]->Contains(&NextOrgCenVec) == -1)
					{
						JoinedPoly[iJoined]->BoolOperation('A', pNextOrgPoly, JoinedPoly2, DTOL_MODELING);
						for (int iPoly = 0; iPoly < JoinedPoly2.size(); iPoly++)
						{
							if (JoinedPoly2[iPoly]->OverlapEx(*pNextIntersectPoly) == true)
							{
								pStepExtendedPoly = JoinedPoly2[iPoly];
								break;
							}
						}
					}
				}

				if (pStepExtendedPoly != nullptr)
				{
					pStepExtendedPoly->BoolOperation('U', pNextIntersectPoly, JoinedPoly, DTOL_MODELING);
					if (JoinedPoly.size() == 1)
						pNextIntersectPoly->CopyFrom(JoinedPoly[0]);
				}
			}
		}

		if(IsChangExtendPoly == false)
			pCurExtendPoly->CopyFrom(pCurOrgPoly);

		if (pPreExtendPoly != nullptr && IsPreLanding == true)
		{
			pCurExtendPoly->BoolOperation('A', pPreExtendPoly, JoinedPoly, DTOL_MODELING);
			if (JoinedPoly.size() > 0)
			{
				JoinedPoly[0]->CheckAndFixFoldedEdges(DTOL_MODELING);
				if (JoinedPoly[0]->CheckPolygon() == TRUE)
					pCurExtendPoly->CopyFrom(JoinedPoly[0]);
				else
					StatusReporterWrapper::OutputMessage(strTrimFailMsg);
			}
		}

		if (pPreIntersectPoly != nullptr)
		{
			pCurExtendPoly->BoolOperation('U', pPreIntersectPoly, JoinedPoly2, DTOL_MODELING);
			if (JoinedPoly2.size() > 0)
			{
				JoinedPoly2[0]->CheckAndFixFoldedEdges(DTOL_MODELING);
				JoinedPoly2[0]->BoolOperation('A', pPreOrgPoly, JoinedPoly, DTOL_MODELING);
				if (JoinedPoly.size() > 0)
				{
					JoinedPoly[0]->CheckAndFixFoldedEdges(DTOL_MODELING);
					if (JoinedPoly[0]->CheckPolygon() == TRUE)
						pCurExtendPoly->CopyFrom(JoinedPoly[0]);
					else
						StatusReporterWrapper::OutputMessage(strTrimFailMsg);
				}
			}
		}

		if (pNextIntersectPoly != nullptr)
		{
			pCurExtendPoly->BoolOperation('U', pNextIntersectPoly, JoinedPoly2, DTOL_MODELING);
			if (JoinedPoly2.size() > 0)
			{
				JoinedPoly2[0]->CheckAndFixFoldedEdges(DTOL_MODELING);
				if (IsNextLanding == false)
				{
					JoinedPoly2[0]->BoolOperation('A', pNextOrgPoly, JoinedPoly, DTOL_MODELING);
					if (JoinedPoly.size() > 0)
					{
						JoinedPoly[0]->CheckAndFixFoldedEdges(DTOL_MODELING);
						if (JoinedPoly[0]->CheckPolygon() == TRUE)
							pCurExtendPoly->CopyFrom(JoinedPoly[0]);
						else
							StatusReporterWrapper::OutputMessage(strTrimFailMsg);
					}
				}
				else
				{
					if (JoinedPoly2[0]->CheckPolygon() == TRUE)
						pCurExtendPoly->CopyFrom(JoinedPoly2[0]);
					else
						StatusReporterWrapper::OutputMessage(strTrimFailMsg);
				}
			}
		}
		pCurExtendPoly->DeleteInLinePoint();

		if (pPreIntersectPoly != nullptr)
			delete pPreIntersectPoly;
		if (pNextIntersectPoly != nullptr)
			delete pNextIntersectPoly;
		pPreIntersectPoly = pNextIntersectPoly = nullptr;
		
		GMObject::DeleteGMObject(JoinedPoly);
		GMObject::DeleteGMObject(JoinedPoly2);
	}

	for (int i = 0; i < PartArr.size(); i++)
	{
		MSStairPart* pPart = PartArr[i];
		if (pPart->IsLanding() == false)
			continue;
		PartArr[i]->m_pPolyline->CopyFrom(ExtendProfileArr[i]);
	}

	for (int i = 0; i < ExtendProfileArr.size(); i++)
		delete ExtendProfileArr[i];

	ExtendProfileArr.clear();

	if (m_nStepNum == 0)
		SetStepHeight(m_dSetpHeight);
	else
		SetStepNum(m_nStepNum);

	return true;
}

bool MSGenStairG::CalculatePartHeight_User()
{
	// 현재 필요 없는 로직임
	// 자유형 계단에서는 TopOffset, BotOffSet 설정이 반영되지 않음
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return false;
	// TopLanding, TopStep를 찾아서 TopOffSet 값을 셋팅
	// BotLanding, BotStep를 찾아서 BotOffSet 값을 셋팅
	// TopLanding의 Level이 TopSetp의 시작 래벨과 같은경우 처리 하지 않음
	// BotLanding의 Level이 BotSetp의 끝 래벨과 같은경우 처리 하지 않음

	int NumLanding = ma_Landing.GetSize();
	double dStartLevel = m_dBotOffset;
	double dEndLevel = pCurFloor->GetSlabHeight() + m_dTopOffset;
	
	MSStairPart* pBotStep = nullptr;
	MSStairPart* pTopStep = nullptr;
	for (int i = 0; i < ma_Step.GetSize(); i++)
	{
		MSStairPart* pCurPart = ma_Step[i];
		if (pBotStep == nullptr || pBotStep->GetBottomLevel() > pCurPart->GetBottomLevel())
			pBotStep = pCurPart;

		if (pTopStep == nullptr || pTopStep->GetTopLevel() < pCurPart->GetTopLevel())
			pTopStep = pCurPart;
	}

	MSStairPart* pBotLanding = nullptr;
	MSStairPart* pTopLanding = nullptr;
	for (int i = 0; i < ma_Landing.GetSize(); i++)
	{
		MSStairPart* pCurPart = ma_Landing[i];
		if (pBotLanding == nullptr || pBotLanding->GetBottomLevel() > pCurPart->GetBottomLevel())
			pBotLanding = pCurPart;
		if (pTopLanding == nullptr || pTopLanding->GetTopLevel() < pCurPart->GetTopLevel())
			pTopLanding = pCurPart;
	}

	if ((pBotStep != nullptr && pBotLanding != nullptr) &&
		(pBotStep->GetTopLevel() <= pBotLanding->GetBottomLevel()))
			pBotLanding = nullptr;

	if ((pTopStep != nullptr && pTopLanding != nullptr) &&
		(pTopStep->GetBottomLevel() >= pTopLanding->GetBottomLevel()))
			pTopLanding = nullptr;

	for (int i = 0; i < ma_Landing.GetSize(); i++)
	{
		MSStairPart* pCurPart = ma_Landing[i];
		if (pBotLanding != nullptr)
		{
			if (pCurPart->GetBottomLevel() == pBotLanding->GetBottomLevel())
			{
				pCurPart->SetBottomLevel(dStartLevel);
				pCurPart->SetTopLevel(dStartLevel);
			}
		}

		if (pBotLanding == pTopLanding)
			continue;

		if (pTopLanding != nullptr)
		{
			if (pCurPart->GetBottomLevel() == pTopLanding->GetBottomLevel())
			{
				pCurPart->SetBottomLevel(dEndLevel);
				pCurPart->SetTopLevel(dEndLevel);
			}
		}
	}

	for (int i = 0; i < ma_Step.GetSize(); i++)
	{
		MSStairPart* pCurPart = ma_Step[i];
		if (pBotStep != nullptr)
		{
			if (pCurPart->GetBottomLevel() == min(pBotStep->GetBottomLevel(), pBotStep->GetTopLevel()))
				pCurPart->SetBottomLevel(dStartLevel);
			else if (pCurPart->GetTopLevel() == min(pBotStep->GetBottomLevel(), pBotStep->GetTopLevel()))
				pCurPart->SetTopLevel(dStartLevel);
		}
		
		if (pTopStep != nullptr)
		{
			if (pCurPart->GetBottomLevel() == max(pTopStep->GetBottomLevel(), pTopStep->GetTopLevel()))
				pCurPart->SetBottomLevel(dEndLevel);
			else if (pCurPart->GetTopLevel() == max(pTopStep->GetBottomLevel(), pTopStep->GetTopLevel()))
				pCurPart->SetTopLevel(dEndLevel);
		}
	}

	return true;
}

void MSGenStairG::SetStepsInfo()
{
// 	if (m_nStepNum == 0)
// 		SetStepHeight(m_dSetpHeight);
// 	else
// 		SetStepNum(m_nStepNum);
	
	for (int i = 0; i < ma_Step.GetSize(); i++)
		ma_Step[i]->CalcOneStep();
}

bool MSGenStairG::CalculatePartHeight()
{
	if (m_eStairType == StairType_General)
		return CalculatePartHeight_General();
	else if (m_eStairType == StairType_Double)
		return CalculatePartHeight_Double();
	else if (m_eStairType == StairType_Line)
		return CalculatePartHeight_Line();
	else if (m_eStairType == StairType_User)
		return true;

	return false;
}

bool MSGenStairG::CalculatePartHeight_Formwork()
{
	if (m_eStairType == StairType_General)
		return CalculatePartHeight_General_Formwork();
	else if (m_eStairType == StairType_Double)
		return CalculatePartHeight_Double_Formwork();
	else if (m_eStairType == StairType_Line)
		return CalculatePartHeight_Line_Formwork();
	else if (m_eStairType == StairType_User)
		return true;

	return false;
}

bool MSGenStairG::GetRemoveStartLanding()
{
	return m_bRemoveStartLanding;
}

void MSGenStairG::SetRemoveStartLanding(bool value)
{
	m_bRemoveStartLanding = value;
}

bool MSGenStairG::GetRemoveEndLanding()
{
	return m_bRemoveEndLanding;
}

void MSGenStairG::SetRemoveEndLanding(bool value)
{
	m_bRemoveEndLanding = value;
}

double MSGenStairG::GetBotOffset()
{
	return m_dBotOffset;
}

void MSGenStairG::SetBotOffset(double value)
{
	m_dBotOffset = value;
	CalculatePartHeight();
}

double MSGenStairG::GetTopOffset()
{
	return m_dTopOffset;
}

void MSGenStairG::SetTopOffset(double value)
{
	m_dTopOffset = value;
	CalculatePartHeight();
}

vector<double> MSGenStairG::GetLandHeightList()
{
	return ma_LandingHeight;
}

void MSGenStairG::SetLandHeightList(vector<double> value)
{
	ma_LandingHeight = value;
	CalculatePartHeight();
}

vector<GM2DPolyline*> MSGenStairG::GetPathPolys()
{
	vector<GM2DPolyline*> RVal;

	vector<MSStairPart*> PartArr;

	for (int i = 0; i < ma_Landing.GetSize(); i++)
		PartArr.push_back(ma_Landing[i]);

	for (int i = 0; i < ma_Step.GetSize(); i++)
		PartArr.push_back(ma_Step[i]);
		
	sort(PartArr.begin(), PartArr.end(),
		[](MSStairPart* a, MSStairPart* b)->bool 
		{
		if (a->GetSequence() == b->GetSequence())
		{
			if (a->GetBottomLevel() == b->GetBottomLevel())
				return false;
			else
				return a->GetBottomLevel() < b->GetBottomLevel();
		}
			else
				return a->GetSequence() < b->GetSequence();
		}
	);

	// 시작, 끝이 랜딩인 경우 PathLine 구하지 않는다.
	
	if (PartArr.size() > 0 && PartArr[PartArr.size() - 1]->IsLanding())
		PartArr.erase(PartArr.begin() + (PartArr.size() - 1));
	if(PartArr.size() > 0 && PartArr[0]->IsLanding())
		PartArr.erase(PartArr.begin());

	GM2DPolyline* pCurPoly = nullptr;

	vector<GM2DVector> CurCrossVecArr;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResultArr;

	if (PartArr.size() == 1 && PartArr[0]->IsLanding() == false)
	{
		GM2DCurve* pCurPath = PartArr[0]->GetPathCurve()->Copy();
		if (pCurPoly == nullptr)
		{
			pCurPoly = new GM2DPolyline();
			RVal.push_back(pCurPoly);
		}
		pCurPoly->AddCopy(pCurPath->m_P0);
		pCurPoly->ma_Bulge.Add(pCurPath->GetBulge());
		pCurPoly->AddCopy(pCurPath->m_P1);
		delete pCurPath;
	}
	else if(PartArr.size() > 1)
	{
		for (int i = 0; i < PartArr.size() - 1; i++)
		{
			GM2DCurve* pCurPath = PartArr[i]->GetPathCurve()->Copy();
			GM2DCurve* pNextPath = PartArr[i + 1]->GetPathCurve()->Copy();

			long nRval = pCurPath->IntersectEnd2(pNextPath, CurCrossVecArr, MyLocArr, UrLocArr, ResultArr, true);
			if (fabs(nRval % 10) == 1)
			{
				if (MyLocArr[0] < 0.5)
				{
					pCurPath->ReverseDirection();
					PartArr[i]->GetPathCurve()->ReverseDirection();
				}

				pCurPath->m_P1 = CurCrossVecArr[0];

				if (UrLocArr[0] > 0.5)
				{
					pNextPath->ReverseDirection();
					PartArr[i + 1]->GetPathCurve()->ReverseDirection();
				}
				pNextPath->m_P0 = CurCrossVecArr[0];
			}

			if (pCurPoly == nullptr)
			{
				pCurPoly = new GM2DPolyline();
				RVal.push_back(pCurPoly);
			}

			int NumPnt = pCurPoly->ma_Point.GetSize();

			bool IsConn = PartArr[i]->IsConnect(PartArr[i + 1]);
			if (IsConn)
			{
				long nRval = pCurPath->IntersectEnd2(pNextPath, CurCrossVecArr, MyLocArr, UrLocArr, ResultArr, true);
				if (fabs(nRval % 10) == 1)
				{
					if (NumPnt == 0)
					{
						pCurPoly->AddCopy(pCurPath->m_P0);
						pCurPoly->ma_Bulge.Add(pCurPath->GetBulge());
						pCurPoly->AddCopy(pCurPath->m_P1);
					}
					else
					{
						pCurPoly->ma_Bulge.Add(pCurPath->GetBulge());
						pCurPoly->AddCopy(pCurPath->m_P1);
					}

					if (i == PartArr.size() - 2)
					{
						pCurPoly->ma_Bulge.Add(pNextPath->GetBulge());
						pCurPoly->AddCopy(pNextPath->m_P1);
					}
				}
				else if (fabs(nRval % 10) == 2)
				{
					// 추후에, 아크인경우
				}
			}
			else
			{
				long nRval = pCurPath->IntersectEnd2(pNextPath, CurCrossVecArr, MyLocArr, UrLocArr, ResultArr, true);
				if (fabs(nRval % 10) == 1)
				{
					if (NumPnt == 0)
					{
						pCurPoly->AddCopy(pCurPath->m_P0);
						pCurPoly->ma_Bulge.Add(pCurPath->GetBulge());
						pCurPoly->AddCopy(pCurPath->m_P1);
					}
					else
					{
						pCurPoly->ma_Bulge.Add(pCurPath->GetBulge());
						pCurPoly->AddCopy(pCurPath->m_P1);
					}

					pCurPoly = nullptr;

					if (i == PartArr.size() - 2)
					{
						pCurPoly = new GM2DPolyline();
						RVal.push_back(pCurPoly);

						pCurPoly->AddCopy(pNextPath->m_P0);
						pCurPoly->ma_Bulge.Add(pNextPath->GetBulge());
						pCurPoly->AddCopy(pNextPath->m_P1);
					}
				}
				else if (fabs(nRval % 10) == 2)
				{
					// 추후에, 아크인경우
				}
			}

			delete pCurPath;
			delete pNextPath;
		}
	}

	return RVal;
}

void MSGenStairG::CalculatePartPathCurve()
{
	if (isComponentStair() == false)
		return;
	if (m_eStairType == StairType_Line)
		return;

	for (int i = 0 ;  i < ma_Step.GetSize(); i++)
		ma_Step[i]->CalcPathCurve();
	
	for (int i = 0; i < ma_Landing.GetSize(); i++)
		ma_Landing[i]->CalcPathCurve();
}

int MSGenStairG::GetEndComponentType()
{
	if (m_eStairType == StairType_General || m_eStairType == StairType_Double)
		return m_bRemoveEndLanding;
	
	MSStairPart* pPart = GetUpperPart();
	if (pPart->IsLanding())
		return 0;

	return 1;
}

int MSGenStairG::GetStartComponentType()
{
	if (m_eStairType == StairType_General || m_eStairType == StairType_Double)
		return m_bRemoveStartLanding;

	MSStairPart* pPart = GetLowerPart();
	if (pPart->IsLanding())
		return 0;

	return 1;
}

double MSGenStairG::GetStepThick()
{
	if (ma_Step.GetSize() > 0)
		return ma_Step[0]->m_Thick;

	return 0;
}

void MSGenStairG::SetStepThick(double nThick)
{
	for (int i = 0; i < ma_Step.GetSize(); i++)
		ma_Step[i]->m_Thick = nThick;
}

int MSGenStairG::GetStepNum()
{
    if (m_nStepNum == 0 && ma_Step.GetSize() > 0)
		return ma_Step[0]->GetStepsNum();
	return m_nStepNum;
}

void MSGenStairG::SetStepNum(int nStepNum)
{
	if (nStepNum <= 0)
		return;

	if (m_eStairType != StairType_User)
	{
		for (int i = 0; i < ma_Step.GetSize(); i++)
			ma_Step[i]->SetStepsNum(nStepNum);
	}

	m_nStepNum = nStepNum;
	m_dSetpHeight = 0;
}

double MSGenStairG::GetStepHeight()
{
	return m_dSetpHeight;
}

void MSGenStairG::SetStepHeight(double dStepHeight)
{
	if (dStepHeight <= 0)
		return;

	if (m_eStairType != StairType_User)
	{
		for (int i = 0; i < ma_Step.GetSize(); i++)
			ma_Step[i]->SetOneStepHeight(dStepHeight);
	}

	m_dSetpHeight = dStepHeight;
	m_nStepNum = 0;
}

double MSGenStairG::GetLandingThick()
{
	if (ma_Landing.GetSize() > 0)
		return ma_Landing[0]->m_Thick;

	return 0;
}

void MSGenStairG::SetLandingThick(double nThick)
{
	for (int i = 0; i < ma_Landing.GetSize(); i++)
		ma_Landing[i]->m_Thick = nThick;
}

void MSGenStairG::CalculateRectStairInVecs()
{
	if (ma_Landing.GetSize() == 2 && ma_Step.GetSize() == 2)
	{
		GM2DCurve* pBaseCurve = nullptr;
		GM2DCurve* pBaseCurve2 = nullptr;

		for (int i = 0; i < ma_Landing[0]->m_pPolyline->GetNumLines() - 1; i++)
		{
			GM2DCurve* pICurve = ma_Landing[0]->m_pPolyline->GetCurve(i);
			for (int j = 0; j < ma_Landing[1]->m_pPolyline->GetNumLines() - 1; j++)
			{
				GM2DCurve* pJCurve = ma_Landing[1]->m_pPolyline->GetCurve(j);
				int nRval = pICurve->IsParallel4DTol(pJCurve);
				if (nRval == 0)
				{
					delete pJCurve;
					continue;
				}
				else if (nRval == -1)
					pJCurve->ReverseDirection();

				double dSLoc = pICurve->OrthoBaseLocValue(pJCurve->m_P0);
				double dELoc = pICurve->OrthoBaseLocValue(pJCurve->m_P1);

				if ((dSLoc > -DTOL_GM && dSLoc < DTOL_GM) && (dELoc > 1 - DTOL_GM && dELoc < 1 + DTOL_GM))
				{
					pBaseCurve = pICurve;
					pBaseCurve2 = pJCurve;
					break;
				}

				delete pJCurve;
			}
			if (pBaseCurve != nullptr && pBaseCurve2 != nullptr)
				break;

			delete pICurve;
		}

		if (pBaseCurve != nullptr && pBaseCurve2 != nullptr)
		{
			if (pBaseCurve->IsParallel4DTol(pBaseCurve2) == -1)
				pBaseCurve2->ReverseDirection();

			DeleteVectorArr();
			ma_Vec.push_back(new GM2DVector(pBaseCurve->m_P0.m_X, pBaseCurve->m_P0.m_Y));
			ma_Vec.push_back(new GM2DVector(pBaseCurve2->m_P0.m_X, pBaseCurve2->m_P0.m_Y));
			ma_Vec.push_back(new GM2DVector(pBaseCurve2->m_P1.m_X, pBaseCurve2->m_P1.m_Y));
			ma_Vec.push_back(new GM2DVector(pBaseCurve->m_P1.m_X, pBaseCurve->m_P1.m_Y));
		}

		delete pBaseCurve;
		delete pBaseCurve2;
	}
}

void MSGenStairG::UpdateOldStairInfo()
{
	CalculateRectStairInVecs();

	if (m_dBotOffset != 0)
	{
		for (int iLandind = 0; iLandind < ma_Landing.GetSize(); iLandind++)
		{
			ma_Landing[iLandind]->SetBottomLevel(ma_Landing[iLandind]->GetBottomLevel() + m_dBotOffset);
			ma_Landing[iLandind]->SetTopLevel(ma_Landing[iLandind]->GetTopLevel() + m_dBotOffset);
		}

		for (int iStep = 0; iStep < ma_Step.GetSize(); iStep++)
		{
			ma_Step[iStep]->SetBottomLevel(ma_Step[iStep]->GetBottomLevel() + m_dBotOffset);
			ma_Step[iStep]->SetTopLevel(ma_Step[iStep]->GetTopLevel() + m_dBotOffset);
		}
	}

	CalculateOffSetByUserStair();
}

void MSGenStairG::SetStairType(StairType eType)
{
	if (eType == StairType_User)
	{
		m_isComponentStair = FALSE;
		CalculateOffSetByUserStair();
	}
	else
		m_isComponentStair = TRUE;

	m_eStairType = eType; 
}

MSGenStairG::StairType MSGenStairG::GetStairType()
{ 
	return m_eStairType;
};

void MSGenStairG::CalculateOffSetByUserStair()
{
	if (m_eStairType == StairType_User)
	{
		MSFloor* pFloor = GetFloor();
		if (pFloor->m_bIsSerializing == false)
		{
			m_dTopOffset = GetUpperPart()->GetTopLevel() - pFloor->GetSlabHeight();
			m_dBotOffset = GetLowerPart()->GetBottomLevel();
		}
	}
}

void MSGenStairG::SetPartSequenceNum()
{
	if (m_eStairType == StairType_General || m_eStairType == StairType_Double)
	{
		vector<MSStairPart*> PartArr;
		for (int i = 0; i < ma_Landing.GetSize(); i++)
			PartArr.push_back(ma_Landing[i]);

		for (int i = 0; i < ma_Step.GetSize(); i++)
			PartArr.push_back(ma_Step[i]);
		
		sort(PartArr.begin(), PartArr.end(),
			[](MSStairPart* a, MSStairPart* b)->bool 
		{
			if (a->GetBottomLevel() == b->GetBottomLevel())
			{
				if (a->IsLanding() == b->IsLanding())
					return false;
				else
					return a->IsLanding();
			}
			else
				return a->GetBottomLevel() < b->GetBottomLevel();
		}
		);

		int nSeq = 0;
		for (int i = 0; i < PartArr.size(); i++)
			PartArr[i]->SetSequence(nSeq++);
	}
}
MSStairPart* MSGenStairG::GetUpFloorStairLowLanding()
{
	// 계단의 마지막이 스텝인 경우에만 처리함
	// 계단의 마지막이 랜딩인 경우 0을 리턴

	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return nullptr;
	MSFloor* pUpperFloor = pCurFloor->GetUpperFloor();
	if (pUpperFloor == nullptr)
		return nullptr;

	GM2DPolyline* pMyPoly = GetTrimmedBoundary();

	vector<MSCompoG*> UpStairArr = FindConnectMember(FindType_MSElement::msStairG, FIND_UPFLOOR);
	MSStairPart* pCurUpPart = GetUpperPart();

	if (pCurUpPart->IsLanding() == true) // 랜딩인 경우 0 , 스텝인 경우 윗층 계단의 랜딩의 두께를 넘겨준다.
		return nullptr;

	MSStairPart* pFindUpStairLowPart = nullptr;
	for (int i = 0; i < UpStairArr.size(); i++)
	{
		MSGenStairG* pUpStair = (MSGenStairG*)UpStairArr[i];
		MSStairPart* pLowPart = pUpStair->GetLowerPart();

		if (pLowPart->IsLanding() == false)
			continue;

		if (pCurUpPart->IsConnect(pLowPart) == true)
		{
			pFindUpStairLowPart = pLowPart;
			break;
		}
	}
	return pFindUpStairLowPart;
}
MSStairPart* MSGenStairG::GetUpFloorStairLowStep()
{
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return nullptr;
	MSFloor* pUpperFloor = pCurFloor->GetUpperFloor();
	if (pUpperFloor == nullptr)
		return nullptr;

	GM2DPolyline* pMyPoly = GetTrimmedBoundary();

	vector<MSCompoG*> UpStairArr = FindConnectMember(FindType_MSElement::msStairG, FIND_UPFLOOR);
	MSStairPart* pCurUpPart = GetUpperPart();

	if (pCurUpPart->IsLanding() == false) 
		return nullptr;

	MSStairPart* pFindUpStairLowPart = nullptr;
	for (int i = 0; i < UpStairArr.size(); i++)
	{
		MSGenStairG* pUpStair = (MSGenStairG*)UpStairArr[i];
		MSStairPart* pLowPart = pUpStair->GetLowerPart();

		if (pLowPart->IsLanding() == true)
			continue;

		if (pCurUpPart->IsConnect(pLowPart) == true)
		{
			pFindUpStairLowPart = pLowPart;
			break;
		}
	}
	return pFindUpStairLowPart;
}
MSStairPart* MSGenStairG::GetLowFloorStairUpLanding()
{
	// 계단의 시작이 스텝인 경우에만 처리함
	// 계단의 시작이 랜딩인 경우 0을 리턴

	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return nullptr;
	MSFloor* pUpperFloor = pCurFloor->GetLowerFloor();
	if (pUpperFloor == nullptr)
		return nullptr;

	GM2DPolyline* pMyPoly = GetTrimmedBoundary();

	vector<MSCompoG*> LowStairArr = FindConnectMember(FindType_MSElement::msStairG, FIND_DNFLOOR);
	MSStairPart* pCurUpPart = GetLowerPart();

	if (pCurUpPart->IsLanding() == true) // 랜딩인 경우 0 , 스텝인 경우 윗층 계단의 랜딩의 두께를 넘겨준다.
		return nullptr;

	MSStairPart* pFindLowStairUpPart = nullptr;
	for (int i = 0; i < LowStairArr.size(); i++)
	{
		MSGenStairG* pLowStair = (MSGenStairG*)LowStairArr[i];
		MSStairPart* pUpPart = pLowStair->GetUpperPart();

		if (pUpPart->IsLanding() == false)
			continue;

		if (pCurUpPart->IsConnect(pUpPart) == true)
		{
			pFindLowStairUpPart = pUpPart;
			break;
		}
	}
	return pFindLowStairUpPart;
}
MSStairPart* MSGenStairG::GetLowFloorStairUpStep()
{
	// 계단의 시작이 스텝인 경우에만 처리함
	// 계단의 시작이 랜딩인 경우 0을 리턴

	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return nullptr;
	MSFloor* pUpperFloor = pCurFloor->GetLowerFloor();
	if (pUpperFloor == nullptr)
		return nullptr;

	GM2DPolyline* pMyPoly = GetTrimmedBoundary();

	vector<MSCompoG*> LowStairArr = FindConnectMember(FindType_MSElement::msStairG, FIND_DNFLOOR);
	MSStairPart* pCurUpPart = GetLowerPart();

	if (pCurUpPart->IsLanding() == false) 
		return nullptr;

	MSStairPart* pFindLowStairUpPart = nullptr;
	for (int i = 0; i < LowStairArr.size(); i++)
	{
		MSGenStairG* pLowStair = (MSGenStairG*)LowStairArr[i];
		MSStairPart* pUpPart = pLowStair->GetUpperPart();

		if (pUpPart->IsLanding() == true)
			continue;

		if (pCurUpPart->IsConnect(pUpPart) == true)
		{
			pFindLowStairUpPart = pUpPart;
			break;
		}
	}
	return pFindLowStairUpPart;
}
double MSGenStairG::GetUpFloorStairLowLandingThick()
{
	// 계단의 마지막이 스텝인 경우에만 처리함
	// 계단의 마지막이 랜딩인 경우 0을 리턴

	double dLandingThich = 0;
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return dLandingThich;
	MSFloor* pUpperFloor = pCurFloor->GetUpperFloor();
	if (pUpperFloor == nullptr)
		return dLandingThich;

	GM2DPolyline* pMyPoly = GetTrimmedBoundary();

	vector<MSCompoG*> UpStairArr = FindConnectMember(FindType_MSElement::msStairG, FIND_UPFLOOR);
	MSStairPart* pCurUpPart = GetUpperPart();
	
	if (pCurUpPart->IsLanding() == true) // 랜딩인 경우 0 , 스텝인 경우 윗층 계단의 랜딩의 두께를 넘겨준다.
		return 0;

	MSStairPart* pFindUpStairLowPart = nullptr;
	for (int i = 0; i < UpStairArr.size(); i++)
	{
		MSGenStairG* pUpStair = (MSGenStairG*)UpStairArr[i];
		MSStairPart* pLowPart = pUpStair->GetLowerPart();

		if(pLowPart->IsLanding() == false)
			continue;

		if (pCurUpPart->IsConnect(pLowPart) == true)
		{
			pFindUpStairLowPart = pLowPart;
			break;
		}
	}

	if (pFindUpStairLowPart != nullptr)
		return pFindUpStairLowPart->GetThick();

	if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Formwork)
	{
		MSCompoG* upSlab = GetTopConnectSlab();
		if (upSlab != nullptr)
			return upSlab->GetThick();
	}
	else
	{
		vector<MSCompoG*> UpSlabArr = MSCompoGUtil::FindCompoGInCrossQTArr(pCurUpPart->m_pPolyline, ma_CrossQT, FindType_MSElement::msSlabG, false, true, DTOL_GM, true);
		for (int i = 0; i < UpSlabArr.size(); i++)
		{
			CheckModelOpt CheckFlag;
			if (IsLevelConnectedOverLap(UpSlabArr[i], CheckFlag, true) == false)
				continue;
			return UpSlabArr[i]->GetThick();
		}
	}

	return 0;
}

bool MSGenStairG::IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance)
{
	MSGenStairG* urStair = dynamic_cast<MSGenStairG*>(urCompoG);
	if (urStair == nullptr) return false;

	if (!GetProfile()->SamePolyLine(urStair->GetProfile())) return false;

	if (ma_Step.GetCount() != urStair->ma_Step.GetCount()) return false;
	if (ma_Landing.GetCount() != urStair->ma_Landing.GetCount()) return false;
	
	for (int iStairPart = 0; iStairPart < ma_Step.GetCount(); iStairPart++)
	{
		if (!(ma_Step[iStairPart]->m_pPolyline)->SamePolyLine(urStair->ma_Step[iStairPart]->m_pPolyline))
			return false;
	}

	for (int iStairPart = 0; iStairPart < ma_Landing.GetCount(); iStairPart++)
	{
		if (!(ma_Landing[iStairPart]->m_pPolyline)->SamePolyLine(urStair->ma_Landing[iStairPart]->m_pPolyline))
			return false;
	}
	return  true;
}
void MSGenStairG::FixCompoGInfoAfterSerialize()
{
	//[BH-3730] GM2DPolyline::CheckAndFixFoldedEdges BugFix, 계단만 자동 업데이트로 처리
	if (MSVersionInfo::GetCurrentVersion() < 20180515)
		FixProfile();

	CalculatePartPathCurve();
}

double MSGenStairG::GetFirstPathDegree()
{
	double degree = 0.0;
	vector<GM2DPolyline*> PolyArr = GetPathPolys();
	if (PolyArr.size() == 0)	return degree;
	GM2DPolyline* firstPolyline = PolyArr[0];
	if (firstPolyline->ma_Point.GetSize() >= 2)
	{
		degree = firstPolyline->ma_Point[0]->GetDegree(*firstPolyline->ma_Point[1]);
	}
	for (int i = 0; i < PolyArr.size(); i++)
		delete PolyArr[i];

	return degree;
}
CString MSGenStairG::GetClassificationName() 
{
	return GetStepClassificationName();
}
CString MSGenStairG::GetStepClassificationName()
{
	if(ma_Step.GetSize() == 0)
		return _T("");

	if(ma_Step[0]->mp_CompoM == nullptr)
		return _T("");

	return ma_Step[0]->mp_CompoM->GetClassificationName();
}
CString MSGenStairG::GetLandingClassificationName()
{
	if(ma_Landing.GetSize() == 0)
		return _T("");

	if(ma_Landing[0]->mp_CompoM == nullptr)
		return _T("");

	return ma_Landing[0]->mp_CompoM->GetClassificationName();
}

void MSGenStairG::FixProfile()
{
	GM2DPolyline* pProfile = GetProfile();
	pProfile->DeleteDuplicatedPoints(DTOL_MODELING);
	pProfile->CheckAndFixFoldedEdges(DTOL_MODELING);

	for (int i = 0; i < ma_Landing.GetCount(); i++)
	{
		ma_Landing[i]->m_pPolyline->DeleteDuplicatedPoints(DTOL_MODELING);
		ma_Landing[i]->m_pPolyline->CheckAndFixFoldedEdges(DTOL_MODELING);
	}
	for (int i = 0; i < ma_Step.GetCount(); i++)
	{
		ma_Step[i]->m_pPolyline->DeleteDuplicatedPoints(DTOL_MODELING);
		ma_Step[i]->m_pPolyline->CheckAndFixFoldedEdges(DTOL_MODELING);
	}
}

double MSGenStairG::GetDownFloorStairUpLandingThick()
{
	// 계단의 처음이 스텝인 경우에만 처리함
	// 계단의 처음이 랜딩인 경우 0을 리턴

	double dLandingThich = 0;
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return dLandingThich;
	MSFloor* pLowerFloor = pCurFloor->GetLowerFloor();
	if (pLowerFloor == nullptr)
		return dLandingThich;

	GM2DPolyline* pMyPoly = GetTrimmedBoundary();

	vector<MSCompoG*> DownStairArr = FindConnectMember(FindType_MSElement::msStairG, FIND_DNFLOOR);
	MSStairPart* pCurDownPart = GetLowerPart();

	if (pCurDownPart->IsLanding() == true) // 랜딩인 경우 0 , 스텝인 경우 윗층 계단의 랜딩의 두께를 넘겨준다.
		return 0;

	MSStairPart* pFindDownStairUpPart = nullptr;
	for (int i = 0; i < DownStairArr.size(); i++)
	{
		MSGenStairG* pDownStair = (MSGenStairG*)DownStairArr[i];
		MSStairPart* pUpPart = pDownStair->GetUpperPart();

		if (pUpPart->IsLanding() == false)
			continue;

		if (pCurDownPart->IsConnect(pUpPart) == true)
		{
			pFindDownStairUpPart = pUpPart;
			break;
		}
	}

	if (pFindDownStairUpPart != nullptr)
		return pFindDownStairUpPart->GetThick();

	if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Formwork)
	{
		MSCompoG* downSlab = GetBottomConnectSlab();
		if (downSlab != nullptr)
			return downSlab->GetThick();
	}
	else
	{
		vector<MSCompoG*> DownSlabArr = MSCompoGUtil::FindCompoGInCrossQTArr(pCurDownPart->m_pPolyline, ma_CrossQT, FindType_MSElement::msSlabG, false, true, DTOL_GM, true);
		for (int i = 0; i < DownSlabArr.size(); i++)
		{
			CheckModelOpt CheckFlag;
			if (IsLevelConnectedOverLap(DownSlabArr[i], CheckFlag, true) == false)
				continue;
			return DownSlabArr[i]->GetThick();
		}
	}
	
	return 0;
}

MSCompoG* MSGenStairG::GetTopConnectSlab()
{
	GM2DPolyline* pBoundary = mp_Boundary;

	vector<MSCompoG*> slabArr = this->FindConnectMember(FindType_MSElement::msSlabG, 0);

	double dTopLevel = 0;
	double dBottomLevel = 0;
	this->GetZLevels(dTopLevel, dBottomLevel);

	MSCompoG* pTopSlab = nullptr;
	// 간혹 동일한 슬래브가 2개 생성되어 먼저 만들어진 슬래브의 ACIS Shape이 Empty인 경우가 있기 때문에 뒤에서부터 찾는다.
	for (int iSlab = slabArr.size() - 1; iSlab >= 0; iSlab--)
	{
		double dSlabTopLevel = 0;
		double dSlabBottomLevel = 0;

		slabArr[iSlab]->GetZLevels(dSlabTopLevel, dSlabBottomLevel);
		if (dTopLevel - DTOL_GM <= dSlabTopLevel &&
			dTopLevel + DTOL_GM >= dSlabBottomLevel)
		{
			// 계단의 Boundary 안에 있는지 계산한다.
			bool isInside = false;
			GM2DPolyline* slabBoundary = slabArr[iSlab]->GetTrimmedBoundary();
			for (int i = 0; i < slabBoundary->ma_Point.GetSize(); i++)
			{
				long num = pBoundary->Contains(slabBoundary->ma_Point[i]);
				isInside = num == -1 || num == 0;
				if (isInside == false)
					break;
			}
			if (isInside)
			{
				pTopSlab = slabArr[iSlab];
				break;
			}
		}
	}

	if (pTopSlab == nullptr)
		return nullptr;

	return dynamic_cast<MSSlabG*>(pTopSlab);
}

MSCompoG* MSGenStairG::GetBottomConnectSlab()
{
	GM2DPolyline* pBoundary = mp_Boundary;

	vector<MSCompoG*> slabArr = this->FindConnectMember(FindType_MSElement::msSlabG, 0);

	double dTopLevel = 0;
	double dBottomLevel = 0;
	this->GetZLevels(dTopLevel, dBottomLevel);

	MSCompoG* pBottomSlab = nullptr;
	// 간혹 동일한 슬래브가 2개 생성되어 먼저 만들어진 슬래브의 ACIS Shape이 Empty인 경우가 있기 때문에 뒤에서부터 찾는다.
	for (int iSlab = slabArr.size() - 1; iSlab >= 0; iSlab--)
	{
		double dSlabTopLevel = 0;
		double dSlabBottomLevel = 0;

		slabArr[iSlab]->GetZLevels(dSlabTopLevel, dSlabBottomLevel);
		if (dBottomLevel - DTOL_GM <= dSlabTopLevel &&
			dBottomLevel + DTOL_GM >= dSlabBottomLevel)
		{
			// 계단의 Boundary 안에 있는지 계산한다.
			bool isInside = false;
			GM2DPolyline* slabBoundary = slabArr[iSlab]->GetTrimmedBoundary();
			for (int i = 0; i < slabBoundary->ma_Point.GetSize(); i++)
			{
				long num = pBoundary->Contains(slabBoundary->ma_Point[i]);
				isInside = num == -1 || num == 0;
				if (isInside == false)
					break;
			}
			if (isInside)
			{
				pBottomSlab = slabArr[iSlab];
				break;
			}
		}
	}

	if (pBottomSlab == nullptr)
		return nullptr;

	return dynamic_cast<MSSlabG*>(pBottomSlab);
}

vector<GM2DCurve*> MSGenStairG::GetPanelLineArr()
{
	SetPartSequenceNum();
	vector<GM2DCurve*> panelLineArr;
	if (m_eStairType == MSGenStairG::StairType_General || m_eStairType == MSGenStairG::StairType_Double)
	{
		// 첫번째 계단부터 시작해서 계단의 외각선의 4개의 선분을 찾는다.

		MSStairPart* pFirstStep = GetFirstStep();
		if (pFirstStep == nullptr)
			return panelLineArr;
		int nFirstSequence = pFirstStep->GetSequence();
		double dTol = DTOL_GM;

		MSStairPart* pNextPart1 = GetStairPartbySequence(nFirstSequence + 1);
		MSStairPart* pNextPart2 = GetStairPartbySequence(nFirstSequence + 2);
		
		if (pNextPart1 == nullptr || pNextPart2 == nullptr)
			return panelLineArr;

		GM2DCurve* pCurve = GetStairOutLine(pFirstStep, true);
		GM2DCurve* pCurve1 = GetStairOutLine(pNextPart1, false);
		GM2DCurve* pCurve2 = GetStairOutLine(pNextPart2, true);

		if (pCurve == nullptr || pCurve1 == nullptr || pCurve2 == nullptr)
			return panelLineArr;
		
		vector<GM2DVector> CurCrossVecArr;
		vector<double> MyLocArr, UrLocArr;
		vector<long> ResutlArr;

		int nRval = pCurve->IntersectEnd2(pCurve1, CurCrossVecArr, MyLocArr, UrLocArr, ResutlArr, true);
		if (nRval == 1)
		{
			if (CurCrossVecArr[0].SameVector4DTOL(pCurve->m_P0, dTol))
				pCurve->ReverseDirection();
			if (CurCrossVecArr[0].SameVector4DTOL(pCurve1->m_P1, dTol))
				pCurve1->ReverseDirection();
		}

		nRval = pCurve1->IntersectEnd2(pCurve2, CurCrossVecArr, MyLocArr, UrLocArr, ResutlArr, true);
		if (nRval == 1)
		{
			if (CurCrossVecArr[0].SameVector4DTOL(pCurve2->m_P1, dTol))
				pCurve2->ReverseDirection();
		}

		GM2DLineVector* pLastLine = new GM2DLineVector();
		pLastLine->m_P0 = pCurve2->m_P1;
		pLastLine->m_P1 = pCurve->m_P0;

		panelLineArr.push_back(pCurve);
		panelLineArr.push_back(pCurve1);
		panelLineArr.push_back(pCurve2);
		panelLineArr.push_back(pLastLine);
	}

	return panelLineArr;
}

GM2DCurve* MSGenStairG::GetStairOutLine(MSStairPart* pStairPart, bool bFindLongCurve)
{
	if (pStairPart == nullptr)
		return nullptr;

	GM2DPolyline* pStartPoly = new GM2DPolyline(true);
	for (int idx = 0; idx < ma_Vec.size(); idx++)
		pStartPoly->AddCopy(*ma_Vec[idx]);
	pStartPoly->MakeClosedPolyline(DTOL_GM);

	vector<GM2DCurve*> ColinearCurveArr = pStartPoly->GetColinearCurve(pStairPart->m_pPolyline, DTOL_GM, DTOL_GM);
	if (ColinearCurveArr.size() == 0)
		return nullptr;
	//else if (ColinearCurveArr.size() == 1 && pStairPart->IsLanding())
	//{
	//	// 앞뒤의 선을 하나씩 더 넣어준다.
	//	int idx = pStartPoly->FindEdgeIndex(ColinearCurveArr[0]);
	//	int numCruve = pStartPoly->GetNumLines();
	//	int preIdx = (idx - 1 + numCruve) % numCruve;
	//	int nextIdx = (idx + 1 + numCruve) % numCruve;

	//	GM2DCurve* pPreCurve = pStartPoly->GetCurve(preIdx);
	//	GM2DCurve* pNextCurve = pStartPoly->GetCurve(nextIdx);

	//	GM2DVector cenVec = pStairPart->m_pPolyline->MassCenter();
	//	double preDist = pPreCurve->Distance(cenVec);
	//	double nextDist = pNextCurve->Distance(cenVec);

	//	if (preDist < nextDist)
	//	{
	//		ColinearCurveArr.push_back(pPreCurve);
	//		delete pNextCurve;
	//	}
	//	else
	//	{
	//		ColinearCurveArr.push_back(pNextCurve);
	//		delete pPreCurve;
	//	}
	//}

	GM2DCurve* pStairOutLine = ColinearCurveArr[0];
	double dFirstPanelLineLen = pStairOutLine->Length();
	
	if (ColinearCurveArr.size() > 0)
	{
		for (int idx = 0; idx < ColinearCurveArr.size(); idx++)
		{
			double dCurLen = ColinearCurveArr[idx]->Length();
			if (bFindLongCurve == true)
			{
				if (dCurLen > dFirstPanelLineLen)
				{
					dFirstPanelLineLen = dCurLen;
					pStairOutLine = ColinearCurveArr[idx];
				}
			}
			else
			{
				if (dCurLen < dFirstPanelLineLen)
				{
					dFirstPanelLineLen = dCurLen;
					pStairOutLine = ColinearCurveArr[idx];
				}
			}
		}
	}

	pStairOutLine = pStairOutLine->Copy();
	GMObject::DeleteGMObject(ColinearCurveArr);
	delete pStartPoly;

	double dDeg = pStairOutLine->GetDegree();
	if (dDeg > 90 + DTOL_GM && dDeg <= 270 + DTOL_GM)
		pStairOutLine->ReverseDirection();

	return pStairOutLine;
}

MSStairPart* MSGenStairG::GetStairPartbySequence(int nFindSequence)
{
	for (long i = 0; i < ma_Step.GetSize(); i++)
	{
		if (ma_Step[i]->GetSequence() == nFindSequence)
			return ma_Step[i];
	}

	for (long i = 0; i < ma_Landing.GetSize(); i++)
	{
		if (ma_Landing[i]->GetSequence() == nFindSequence)
			return ma_Landing[i];
	}

	return nullptr;
}

bool MSGenStairG::CalculatePartHeight_General_Formwork()
{
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return false;

	int NumLanding = ma_Landing.GetSize();
	double dStartLevel = m_dBotOffset;
	double dEndLevel = pCurFloor->GetSlabHeight() + m_dTopOffset;

	// Cli에서 받는 계단의 갯수 2 , 랜딩의 갯수 1~3
	/*
	if (ma_Step.GetSize() != 2 || ma_LandingHeight.size() != 1)
	return false;

	if (m_bRemoveStartLanding == false)
	{
	ma_Landing[0]->SetBottomLevel(dStartLevel);
	ma_Landing[0]->SetTopLevel(dStartLevel);
	ma_Landing[1]->SetBottomLevel(ma_LandingHeight[0]);
	ma_Landing[1]->SetTopLevel(ma_LandingHeight[0]);
	}
	else
	{
	ma_Landing[0]->SetBottomLevel(ma_LandingHeight[0]);
	ma_Landing[0]->SetTopLevel(ma_LandingHeight[0]);
	}

	if (m_bRemoveEndLanding == false)
	{
	ma_Landing[NumLanding - 1]->SetBottomLevel(dEndLevel);
	ma_Landing[NumLanding - 1]->SetTopLevel(dEndLevel);
	}

	ma_Step[0]->SetBottomLevel(dStartLevel);
	ma_Step[0]->SetTopLevel(ma_LandingHeight[0]);

	ma_Step[1]->SetBottomLevel(ma_LandingHeight[0]);
	ma_Step[1]->SetTopLevel(dEndLevel);
	*/

	SetStepsInfo();
	SetPartSequenceNum();
	return true;

}
bool MSGenStairG::CalculatePartHeight_Double_Formwork()
{
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == nullptr)
		return false;

	int NumLanding = ma_Landing.GetSize();
	double dStartLevel = m_dBotOffset;
	double dEndLevel = pCurFloor->GetSlabHeight() + m_dTopOffset;

	// Cli에서 받는 계단의 갯수 4 , 랜딩의 갯수 3~5
	/*
	if (ma_Step.GetSize() != 4 && ma_LandingHeight.size() != 3)
	return false;

	int nIdx = 0;
	if (m_bRemoveStartLanding == false)
	{
	ma_Landing[0]->SetBottomLevel(dStartLevel);
	ma_Landing[0]->SetTopLevel(dStartLevel);
	nIdx++;
	}
	for (int i = 0; i < ma_LandingHeight.size(); i++)
	{
	ma_Landing[nIdx + i]->SetBottomLevel(ma_LandingHeight[i]);
	ma_Landing[nIdx + i]->SetTopLevel(ma_LandingHeight[i]);
	}

	if (m_bRemoveEndLanding == false)
	{
	ma_Landing[NumLanding - 1]->SetBottomLevel(dEndLevel);
	ma_Landing[NumLanding - 1]->SetTopLevel(dEndLevel);
	}

	if (ma_Step.GetSize() == 2)
	{
	AddStep((MSStairPart*)ma_Step[0]->Copy());
	AddStep((MSStairPart*)ma_Step[1]->Copy());
	}

	ma_Step[0]->SetBottomLevel(dStartLevel);
	ma_Step[0]->SetTopLevel(ma_LandingHeight[0]);

	ma_Step[1]->SetBottomLevel(ma_LandingHeight[0]);
	ma_Step[1]->SetTopLevel(ma_LandingHeight[1]);

	ma_Step[2]->SetBottomLevel(ma_LandingHeight[1]);
	ma_Step[2]->SetTopLevel(ma_LandingHeight[2]);

	ma_Step[3]->SetBottomLevel(ma_LandingHeight[2]);
	ma_Step[3]->SetTopLevel(dEndLevel);
	*/

	SetStepsInfo();

	SetPartSequenceNum();
	return true;

}
bool MSGenStairG::CalculatePartHeight_Line_Formwork()
{
	return CalculatePartHeight_Line();
}
