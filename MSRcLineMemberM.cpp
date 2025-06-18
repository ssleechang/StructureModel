#include "StdAfx.h"
#include "StructureModelEnum.h"
#include "MSRcLineMemberM.h"
#include "GMLib/GM2DRectangle.h"
#include "GMLib/GM2DCircle.h"
#include "GMLib/MSVersionInfo.h"
#include "T_BarLoc.h"
#include "RADFCore_CLI/StatusReporterWrapper.h"

#include "MSRcColumnM.h"
#include "MSSrcColumnM.h"
#include "MDSrcColumnM.h"
#include "MRColumnM.h"
#include "MRRcSection.h"
#include "MREdge.h"
#include "MRVBar.h"
#include "MSSteelSection.h"
#include "StructureModel\MSDeformedBar.h"
#include "MSRebarBuilding.h"
#include "MSColumnHeaderM.h"

IMPLEMENT_SERIAL(MSRcLineMemberM, MSLineMemberM, VERSIONABLE_SCHEMA | 1)
MSRcLineMemberM::MSRcLineMemberM(void)
{
	me_Shape = RECT;
	m_dWidth = 500;
	m_dDepth = GetDepthDefaultValue();
	m_ColumnHeaderMID = 0;
}

MSRcLineMemberM::~MSRcLineMemberM(void)
{
}

void MSRcLineMemberM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSRcLineMemberM* pSourceLineM = (MSRcLineMemberM*)pSource;
	me_Shape = pSourceLineM->me_Shape;
	m_dWidth = pSourceLineM->m_dWidth;
	m_dDepth = pSourceLineM->m_dDepth;
	m_ColumnHeaderMID = pSourceLineM->m_ColumnHeaderMID;
	MSLineMemberM::CopyFromMe(pSource);
}

double MSRcLineMemberM::GetCover()
{
	if(mp_RebarM)
		return mp_RebarM->GetCover();
	return 0.0;
}
void MSRcLineMemberM::SetCover(double dCover)
{
	if(mp_RebarM)
		mp_RebarM->SetCover(dCover);
}

double MSRcLineMemberM::GetDepth()
{
	if (me_Shape == CIRCLE)
		return GetWidth();

	return m_dDepth;
}

bool MSRcLineMemberM::IsGeometryDifferent(MSCompoM* compoM)
{
	MSRcLineMemberM* pUrCompoM = dynamic_cast<MSRcLineMemberM*>(compoM);
	if(pUrCompoM != NULL)
	{
		if(m_ColumnHeaderMID != pUrCompoM->m_ColumnHeaderMID)
			return true;
	}
	return MSLineMemberM::IsGeometryDifferent(compoM);
}

void MSRcLineMemberM::Dump(EFS::EFS_Buffer& buffer)
{
	MSLineMemberM::Dump(buffer);
	int shape = (int)me_Shape;
	buffer << m_dWidth;
	buffer << m_dDepth;
	buffer << shape;
	buffer << m_ColumnHeaderMID;
}

void MSRcLineMemberM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSLineMemberM::Recovery(buffer);
	int shape = 0;
	buffer >> m_dWidth;
	buffer >> m_dDepth;
	buffer >> shape;
	me_Shape = (SECT_SHAPE)shape;
	buffer >> m_ColumnHeaderMID;
}

void MSRcLineMemberM::Serialize(CArchive &ar)
{
	MSLineMemberM::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << (int)me_Shape;
		ar << m_dWidth;
		ar << m_dDepth;
		ar << m_ColumnHeaderMID;
	}
	else
	{
		int Shape;
		ar >> Shape;
		me_Shape = (SECT_SHAPE)Shape;
		ar >> m_dWidth;
		ar >> m_dDepth;
		if(MSVersionInfo::GetCurrentVersion() >= 20160330)
			ar >> m_ColumnHeaderMID;
	}
}

void MSRcLineMemberM::GetSecPolyline(GM2DPolyline& TheGon )
{
	if(me_Shape == RECT)
	{
		double dXc1 = -GetWidth()/2;
		double dYc1 = -GetDepth()/2;
		double dXc2 = +GetWidth()/2;
		double dYc2 = +GetDepth()/2;
		GM2DVector* pVec1 = new GM2DVector(dXc1, dYc1);
		GM2DVector* pVec2 = new GM2DVector(dXc2, dYc1);
		GM2DVector* pVec3 = new GM2DVector(dXc2, dYc2);
		GM2DVector* pVec4 = new GM2DVector(dXc1, dYc2);

		TheGon.AddPointAndBulge(pVec1);
		TheGon.AddPointAndBulge(pVec2);
		TheGon.AddPointAndBulge(pVec3);
		TheGon.AddPointAndBulge(pVec4);
	}
	else if(me_Shape == CIRCLE)
	{
        int resolution = 24;
		GM2DVector Pnt;
		GMCircle SecCircle(GetWidth()/2., 0., 0.);
		double RadStep = 2.0*PI/resolution;
		for(long PntNum=0; PntNum<resolution; PntNum++)
		{
			SecCircle.GetGlobalXY(PntNum*RadStep, Pnt.m_X, Pnt.m_Y);
			TheGon.AddPointAndBulge((GM2DVector*)Pnt.Copy());
		}
	}
	else if(me_Shape == 2)
	{
		ASSERT(0);
	}
	else
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Not Support POLYGON, POLYGONINPLAN SectionType Name: %s(%d)"), GetName(), m_ID);
		StatusReporterWrapper::OutputMessage(strErrorMsg);
	}
	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
}

void MSRcLineMemberM::GetHeaderPolyline(GM2DPolyline& TheGon)
{
	long nCHMID = GetColumnHeaderMID();
	if (nCHMID == 0)
		return;
	MSColumnHeaderM* pColumnHeaderM = dynamic_cast<MSColumnHeaderM*>(GetBuilding()->GetCompoM(nCHMID));
	if (pColumnHeaderM == NULL || pColumnHeaderM->GetType() != MSCompoM::msColumnHeaderM)
		return;

	if (me_Shape == RECT)
	{
		GM2DRectangle TheRect;
		double dMaxVal = m_dWidth > m_dDepth ? m_dWidth : m_dDepth;

		double dXc1 = -m_dWidth / 2;
		double dYc1 = -m_dDepth / 2;
		double dXc2 = +m_dWidth / 2;
		double dYc2 = +m_dDepth / 2;

		if (pColumnHeaderM->GetHeadType() == 0) // 한쪽
		{
			if (pColumnHeaderM->GetDirection() == 0) // X방향
				dXc2 += pColumnHeaderM->GetL1();
			else
				dYc2 += pColumnHeaderM->GetL1();
		}
		else if (pColumnHeaderM->GetHeadType() == 1) // 양쪽
		{
			if (pColumnHeaderM->GetDirection() == 0) // X방향
			{
				dXc1 -= pColumnHeaderM->GetL3(); 
				dXc2 += pColumnHeaderM->GetL1();
			}
			else
			{
				dYc2 += pColumnHeaderM->GetL1();
				dYc1 -= pColumnHeaderM->GetL3();
			}

		}
		else if (pColumnHeaderM->GetHeadType() == 2) // 직사각형
		{
			if (pColumnHeaderM->GetDirection() == 0) // X방향
			{
				dXc1 = -pColumnHeaderM->GetL1()/2;
				dXc2 = pColumnHeaderM->GetL1()/2;
				dYc2 = pColumnHeaderM->GetL3()/2;
				dYc1 = -pColumnHeaderM->GetL3()/2;
			}
			else
			{
				dXc1 = -pColumnHeaderM->GetL3()/2;
				dXc2 = pColumnHeaderM->GetL3()/2;
				dYc2 = pColumnHeaderM->GetL1()/2;
				dYc1 = -pColumnHeaderM->GetL1()/2;
			}
		}
		else if (pColumnHeaderM->GetHeadType() == 3) // 삼방향
		{
			if (pColumnHeaderM->GetDirection() == 0) // X방향
			{
				dXc2 = pColumnHeaderM->GetL1() + dXc1;
				dYc2 = pColumnHeaderM->GetL3() / 2;
				dYc1 = -pColumnHeaderM->GetL3() / 2;
			}
			else
			{
				dXc1 = -pColumnHeaderM->GetL3() / 2;
				dXc2 = pColumnHeaderM->GetL3() / 2;
				dYc2 = pColumnHeaderM->GetL1() + dYc1;
			}
		}

		GM2DVector* pVec1 = new GM2DVector(dXc1, dYc1);
		GM2DVector* pVec2 = new GM2DVector(dXc2, dYc1);
		GM2DVector* pVec3 = new GM2DVector(dXc2, dYc2);
		GM2DVector* pVec4 = new GM2DVector(dXc1, dYc2);

		TheGon.AddPointAndBulge(pVec1);
		TheGon.AddPointAndBulge(pVec2);
		TheGon.AddPointAndBulge(pVec3);
		TheGon.AddPointAndBulge(pVec4);
		TheGon.MakeClosedPolyline(DTOL_GM);
	}
	else if (me_Shape == CIRCLE)
	{
		int resolution = 24;
		GM2DVector Pnt;
		GMCircle SecCircle(m_dWidth / 2., 0., 0.);
		double RadStep = 2.0*PI / resolution;
		for (long PntNum = 0; PntNum < resolution; PntNum++)
		{
			SecCircle.GetGlobalXY(PntNum*RadStep, Pnt.m_X, Pnt.m_Y);
			TheGon.AddCopy(Pnt);
		}
	}
	else if (me_Shape == 2)
	{
		ASSERT(0);
	}
	else
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Not Support POLYGON, POLYGONINPLAN SectionType Name: %s(%d)"), GetName(), m_ID);
		StatusReporterWrapper::OutputMessage(strErrorMsg);
	}
	TheGon.MakeCCW();
}

vector<GM2DPolyline> MSRcLineMemberM::GetSubHeaderPolyline()
{
	vector<GM2DPolyline> subProfileArr;

	long nCHMID = GetColumnHeaderMID();
	if (nCHMID == 0)
		return subProfileArr;
	MSColumnHeaderM* pColumnHeaderM = dynamic_cast<MSColumnHeaderM*>(GetBuilding()->GetCompoM(nCHMID));
	if (pColumnHeaderM == NULL || pColumnHeaderM->GetType() != MSCompoM::msColumnHeaderM)
		return subProfileArr;

	if (me_Shape == RECT)
	{
		GM2DRectangle TheRect;
		double dMaxVal = m_dWidth > m_dDepth ? m_dWidth : m_dDepth;

		double dXc1 = -m_dWidth / 2;
		double dYc1 = -m_dDepth / 2;
		double dXc2 = +m_dWidth / 2;
		double dYc2 = +m_dDepth / 2;

		double dL5 = pColumnHeaderM->GetL5();
		double dT5 = pColumnHeaderM->GetT5();

		if (pColumnHeaderM->GetHeadType() == 1) // 양쪽
		{
			if (pColumnHeaderM->GetDirection() == 0) // X방향
			{
				{// TopSubProfile
					GM2DPolyline subPolyline;
					subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc2));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc2, dYc2));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc2, dYc2 + dL5));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc2 + dL5));
					subPolyline.MakeClosedPolyline(DTOL_GM);
					subPolyline.MakeCCW();

					subProfileArr.push_back(subPolyline);
				}
				{// BotSubProfile
					GM2DPolyline subPolyline;
					subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc1 - dL5));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc2, dYc1 - dL5));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc2, dYc1));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc1));
					subPolyline.MakeClosedPolyline(DTOL_GM);
					subPolyline.MakeCCW();

					subProfileArr.push_back(subPolyline);
				}
			}
			else
			{
				{// LeftSubProfile
					GM2DPolyline subPolyline;
					subPolyline.AddPointAndBulge(new GM2DVector(dXc1 - dL5, dYc1));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc1));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc2));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc1 - dL5, dYc2));
					subPolyline.MakeClosedPolyline(DTOL_GM);
					subPolyline.MakeCCW();

					subProfileArr.push_back(subPolyline);
				}
				{// RightSubProfile
					GM2DPolyline subPolyline;
					subPolyline.AddPointAndBulge(new GM2DVector(dXc2, dYc1));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc2 + dL5, dYc1));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc2 + dL5, dYc2));
					subPolyline.AddPointAndBulge(new GM2DVector(dXc2, dYc2));
					subPolyline.MakeClosedPolyline(DTOL_GM);
					subPolyline.MakeCCW();

					subProfileArr.push_back(subPolyline);
				}
			}
		}
		else if (pColumnHeaderM->GetHeadType() == 3) // 삼방향
		{
			if (pColumnHeaderM->GetDirection() == 0) // X방향
			{
				// LeftSubProfile
				GM2DPolyline subPolyline;
				subPolyline.AddPointAndBulge(new GM2DVector(dXc1 - dL5, dYc1));
				subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc1));
				subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc2));
				subPolyline.AddPointAndBulge(new GM2DVector(dXc1 - dL5, dYc2));
				subPolyline.MakeClosedPolyline(DTOL_GM);
				subPolyline.MakeCCW();

				subProfileArr.push_back(subPolyline);
			}
			else
			{
				// BotSubProfile
				GM2DPolyline subPolyline;
				subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc1 - dL5));
				subPolyline.AddPointAndBulge(new GM2DVector(dXc2, dYc1 - dL5));
				subPolyline.AddPointAndBulge(new GM2DVector(dXc2, dYc1));
				subPolyline.AddPointAndBulge(new GM2DVector(dXc1, dYc1));
				subPolyline.MakeClosedPolyline(DTOL_GM);
				subPolyline.MakeCCW();

				subProfileArr.push_back(subPolyline);
			}
		}
	}
	else if (me_Shape == CIRCLE)
	{
		
	}
	else if (me_Shape == 2)
	{
		ASSERT(0);
	}
	else
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Not Support POLYGON, POLYGONINPLAN SectionType Name: %s(%d)"), GetName(), m_ID);
		StatusReporterWrapper::OutputMessage(strErrorMsg);
	}

	return subProfileArr;
}

void MSRcLineMemberM::GetMainBarLoc(vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation, eColumnBarType eType)
{
	MSRcLineMemberM* pCompoM = this;
	if (pCompoM == NULL) return;

	if (pCompoM->GetShape() == MSRcLineMemberM::RECT)
	{
		GetMainBarLoc_Rect(MainBarLocArr, dAxisRotation, eType);
	}
	else if (pCompoM->GetShape() == MSRcLineMemberM::CIRCLE)
	{
		GetMainBarLoc_Circle(MainBarLocArr, dAxisRotation);
	}
	else if (pCompoM->GetShape() == MSRcLineMemberM::POLYGON)
	{
		GetMainBarLoc_Polygon(MainBarLocArr, dAxisRotation);
	}
}

void MSRcLineMemberM::GetMainBarLoc_Circle(vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation)
{
	GM2DVector BaseVec(0, 0);
	MSMainBarSet* pTBars = NULL;
	double dHoopBarDia = 0;
	double dMaxDia = 0;
	vector<CString> DiaArr;
	double dCover = 0;
	bool isCornerPlacing = false;
	CString sInitial = _T("T");
	CString sBarName = _T("");
	bool bUserSteelGradeS = false;

	if (IsRC())
	{
		MSRcColumnM* pRcColumnM = (MSRcColumnM*)GetRebarM();
		if (pRcColumnM == NULL) return;

		pTBars = &pRcColumnM->m_TBars;

		dHoopBarDia = pRcColumnM->m_CenHoopBar.GetDeformedBarDiameter();
		dMaxDia = pRcColumnM->m_TBars.GetMaxDeformedBarDiameterOnSet(0);
		bUserSteelGradeS = pRcColumnM->m_TBars.GetUserSteelGradeS();
		dCover = pRcColumnM->GetCover();
		isCornerPlacing = pRcColumnM->GetIsCornerPlacing();
	}
	else if (IsSRC())
	{
		MDSrcColumnM* pSrcColumnM = (MDSrcColumnM*)GetRebarM();
		if (pSrcColumnM == NULL) return;

		pTBars = &pSrcColumnM->m_TBars;

		dHoopBarDia = pSrcColumnM->m_CenHoopBar.GetDeformedBarDiameter();
		dMaxDia = pSrcColumnM->m_TBars.GetMaxDeformedBarDiameterOnSet(0);
		bUserSteelGradeS = pSrcColumnM->m_TBars.GetUserSteelGradeS();
		dCover = pSrcColumnM->GetCover();
		isCornerPlacing = pSrcColumnM->GetIsCornerPlacing();
	}
	double dCurDia = 0;

	if (isCornerPlacing)
	{
		GM2DVector Pnt;
		
		double dWidth = GetWidth() / 2 - dCover - dHoopBarDia - dMaxDia / 2;

		//long nMainBarNum = pTBars->GetNumBarsOnSet(0);
		double RadStartStep = PI / 4 + dAxisRotation*Deg2Rad;
		double RadStep = PI / 2;
		double dDiaDist = 40 + dMaxDia;
		double RadDiaStep = 0;
		long nBarNumLayer = pTBars->GetNumSets();
		for (long index = 0; index < nBarNumLayer; index++)
		{
			DiaArr = pTBars->MakeLayerDiaArr(index);
			dWidth -= (40 + dMaxDia)*index;
			GM2DVector* SVec = &BaseVec;
			GM2DCircle CircleBarLine(dWidth, SVec->m_X, SVec->m_Y);
			double dCircleLen = PI * 2 * dWidth;
			double dDiaStep = dDiaDist / dCircleLen;
			RadDiaStep = PI * 2 * dDiaStep;

			long nBarNum = pTBars->GetNumBarsOnSet(index);
			long nMultipleCnt = ceil(nBarNum / 4.);
			bool IsOddNumber = false;
			if (nMultipleCnt % 2 == 1)
				IsOddNumber = true;

			long nSIdx = MainBarLocArr.size();

			for (long PntNum = 0; PntNum < nBarNum; PntNum++)
			{
				double dTmp = PntNum*RadStep + RadStartStep;

				int nCurMultipleCnt = floor(PntNum / 4.);
				if (IsOddNumber)
				{
					if (nCurMultipleCnt == 0)
					{

					}
					else if (nCurMultipleCnt % 2 == 0) // 짝수
						dTmp -= RadDiaStep *  ceil(nCurMultipleCnt / 2.);
					else // 홀수
						dTmp += RadDiaStep * ceil(nCurMultipleCnt / 2.);
				}
				else
				{
					if (nCurMultipleCnt == 0)
						dTmp -= RadDiaStep / 2;
					else if (nCurMultipleCnt % 2 == 0) // 짝수
					{
						dTmp -= RadDiaStep / 2;
						dTmp -= RadDiaStep *  ceil(nCurMultipleCnt / 2.);
					}
					else // 홀수
					{
						dTmp += RadDiaStep / 2;
						dTmp += RadDiaStep * floor(nCurMultipleCnt / 2.);
					}
				}

				if (dTmp > PI * 2)
					dTmp -= PI * 2;

				CircleBarLine.GetGlobalXY(dTmp, Pnt.m_X, Pnt.m_Y);
				MSDeformedBar bar(DiaArr[PntNum]);
				dCurDia = bar.m_Dia;
				T_BarLoc* pBarLoc = new T_BarLoc(Pnt, Rad2Deg*dTmp + 90, dCurDia, bUserSteelGradeS);
				pBarLoc->SetRootType(COLUMN_MAIN_TOP);
				MainBarLocArr.push_back(pBarLoc);
			}

			for (long iLoc = nSIdx ; iLoc < MainBarLocArr.size(); iLoc++)
			{
				for (long jLoc = iLoc + 1; jLoc < MainBarLocArr.size(); jLoc++)
				{
					T_BarLoc* pIBarLoc = MainBarLocArr[iLoc];
					T_BarLoc* pJBarLoc = MainBarLocArr[jLoc];
					double dIDeg = BaseVec.GetDegree(pIBarLoc->GetLoc());
					double dJDeg = BaseVec.GetDegree(pJBarLoc->GetLoc());

					if (dIDeg > dJDeg)
					{
						MainBarLocArr[iLoc] = pJBarLoc;
						MainBarLocArr[jLoc] = pIBarLoc;
					}
				}
			}

			if (index == 0) // 1단 레이어에만 정보를 넣어줌
			{
				for (long iLoc = nSIdx; iLoc < MainBarLocArr.size(); iLoc++)
				{
					T_BarLoc* pIBarLoc = MainBarLocArr[iLoc];
					sBarName.Format(_T("%s%ld"), sInitial, iLoc);
					pIBarLoc->SetBarName(sBarName);
				}
			}
		}
	}
	else
	{
		GM2DVector Pnt;
		T_BarLoc* pBarLoc = NULL;
		double dWidth = GetWidth() / 2 - dCover - dHoopBarDia - dMaxDia / 2;

		long nMainBarNum = pTBars->GetNumBarsOnSet(0);
		if (nMainBarNum == 0)	return;
		double RadStep = (PI * 2) / nMainBarNum;
		long nBarNumLayer = pTBars->GetNumSets();
		for (long index = 0; index < nBarNumLayer; index++)
		{
			dWidth -= (40 + dMaxDia)*index;
			GM2DVector* SVec = &BaseVec;
			GM2DCircle CircleBarLine(dWidth, SVec->m_X, SVec->m_Y);
			long nBarNum = pTBars->GetNumBarsOnSet(index);
			DiaArr = pTBars->MakeLayerDiaArr(index);
			for (long PntNum = 0; PntNum < nBarNum; PntNum++)
			{
				long space = nMainBarNum / nBarNum;

				CircleBarLine.GetGlobalXY(PntNum*space*RadStep + dAxisRotation*Deg2Rad, Pnt.m_X, Pnt.m_Y);
				MSDeformedBar bar(DiaArr[PntNum]);
				dCurDia = bar.m_Dia;
				pBarLoc = new T_BarLoc(Pnt, Rad2Deg*PntNum*space*RadStep + dAxisRotation + 90, dCurDia, bUserSteelGradeS);
				
				if (index == 0)  // 타이바는 1단 레이어에만 정보는 넣어줌
				{
					sBarName.Format(_T("%s%ld"), sInitial, PntNum);
					pBarLoc->SetBarName(sBarName);

				}
				pBarLoc->SetRootType(COLUMN_MAIN_TOP);
				MainBarLocArr.push_back(pBarLoc);
			}
			if(index == 0)
				SetCornerBarInfo(MainBarLocArr);
		}
	}
}
void MSRcLineMemberM::SetCornerBarInfo(vector<T_BarLoc*>& MainBarLocArr)
{
	int numBar = MainBarLocArr.size();
	int space = numBar / 4;
	int nConer = 0;
	double dRatio = 0;
	for (int i = 0; i < numBar; i = i+space)
	{
		MainBarLocArr[i]->SetIsCornerBar(TRUE);
		nConer++;
		if(nConer==4)	break;
	}
}
void MSRcLineMemberM::GetMainBarLoc_Rect(vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation, eColumnBarType eType)
{
	GM2DVector BaseVec(0, 0);
	double dWidth = GetWidth();
	double dDepth = GetDepth();
	double dCover = 0;

	MSMainBarSet* pTBars = NULL;
	MSMainBarSet* pBBars = NULL;
	MSMainBarSet* pLBars = NULL;
	MSMainBarSet* pRBars = NULL;

	double dMaxHoopBarDia = 0;
	double dMaxMainBarDia = 0;
	bool isCornerPlacing = false;

	if (IsRC())
	{
		MSRcColumnM* pRcColumnM = (MSRcColumnM*)GetRebarM();
		if (pRcColumnM == NULL) return;

		dMaxHoopBarDia = pRcColumnM->m_CenHoopBar.GetDeformedBarDiameter();
		dMaxMainBarDia = pRcColumnM->m_TBars.GetMaxDeformedBarDiameterOnSet(0);
		dCover = pRcColumnM->GetCover() + dMaxHoopBarDia + dMaxMainBarDia / 2;

		pTBars = &pRcColumnM->m_TBars;
		pBBars = &pRcColumnM->m_BBars;
		pLBars = &pRcColumnM->m_LBars;
		pRBars = &pRcColumnM->m_RBars;

		isCornerPlacing = pRcColumnM->GetIsCornerPlacing();
	}
	else if (IsSRC())
	{

		//SRC의 경우 주근을 코너로 몰아서 배치시킨다.
		MSRcLineMemberM* pCompoM = (MSRcLineMemberM*)this;
		if (pCompoM == NULL) return;
		MDSrcColumnM* pSrcColumnM = (MDSrcColumnM*)this->GetRebarM();
		if (pSrcColumnM == NULL) return;
		
		dMaxHoopBarDia = pSrcColumnM->m_CenHoopBar.GetDeformedBarDiameter();
		dMaxMainBarDia = pSrcColumnM->m_TBars.GetMaxDeformedBarDiameterOnSet(0);
		dCover = pSrcColumnM->GetCover() + dMaxHoopBarDia + dMaxMainBarDia / 2;

		pTBars = &pSrcColumnM->m_TBars;
		pBBars = &pSrcColumnM->m_BBars;
		pLBars = &pSrcColumnM->m_LBars;
		pRBars = &pSrcColumnM->m_RBars;

		isCornerPlacing = pSrcColumnM->GetIsCornerPlacing();
	}
	else
		return;

	GM2DPolyline SectPoly(TRUE);
	GetSecPolyline(SectPoly);

	for (long iVec = 0; iVec < SectPoly.GetSize(); iVec++)
		SectPoly[iVec]->RotateZ(dAxisRotation*Deg2Rad, &BaseVec);

	GM2DPolyline BarLines(TRUE);
	SectPoly.OffSet_Acis(dCover, FALSE, BarLines);

	long numLine = BarLines.ma_Point.GetSize();
	int nTopNumLayer = pTBars->GetNumSets();
	int nLeftNumLayer = pLBars->GetNumSets();

	GM2DLineVector BLine = BarLines.GetLine(0);
	GM2DLineVector RLine = BarLines.GetLine(1);
	GM2DLineVector TLine = BarLines.GetLine(2);
	GM2DLineVector LLine = BarLines.GetLine(3);
	long LNumBars = pLBars->GetNumBarsOnSet(0);
	long TNumBars = pTBars->GetNumBarsOnSet(0);
	if (LNumBars <= 0)	LNumBars = 0;
	if (TNumBars <= 1)	TNumBars = 2;

	double dTBLayerDist = LLine.Length() / (LNumBars + 1);
	double dLRLayerDist = TLine.Length() / (TNumBars - 1);

	if (isCornerPlacing)
	{
		if (eType == ColumnBarType_AllBar || eType == ColumnBarType_TopBar)
			GetMainBarLoc_BarLineByDist(TLine, *pTBars, true, MainBarLocArr, ColumnBarType_TopBar, dTBLayerDist);
		if (eType == ColumnBarType_AllBar || eType == ColumnBarType_BotBar)
			GetMainBarLoc_BarLineByDist(BLine, *pBBars, true, MainBarLocArr, ColumnBarType_BotBar, dTBLayerDist);
		if (eType == ColumnBarType_AllBar || eType == ColumnBarType_LeftBar)
			GetMainBarLoc_BarLineByDist(LLine, *pLBars, false, MainBarLocArr, ColumnBarType_LeftBar, dLRLayerDist);
		if (eType == ColumnBarType_AllBar || eType == ColumnBarType_RightBar)
			GetMainBarLoc_BarLineByDist(RLine, *pRBars, false, MainBarLocArr, ColumnBarType_RightBar, dLRLayerDist);
	}
	else
	{
		if (eType == ColumnBarType_AllBar || eType == ColumnBarType_TopBar)
			GetMainBarLoc_BarLine(TLine, *pTBars, true, MainBarLocArr, ColumnBarType_TopBar, dTBLayerDist, nLeftNumLayer);
		if (eType == ColumnBarType_AllBar || eType == ColumnBarType_BotBar)
			GetMainBarLoc_BarLine(BLine, *pBBars, true, MainBarLocArr, ColumnBarType_BotBar, dTBLayerDist, nLeftNumLayer);
		if (eType == ColumnBarType_AllBar || eType == ColumnBarType_LeftBar)
			GetMainBarLoc_BarLine(LLine, *pLBars, false, MainBarLocArr, ColumnBarType_LeftBar, dLRLayerDist, nTopNumLayer);
		if (eType == ColumnBarType_AllBar || eType == ColumnBarType_RightBar)
			GetMainBarLoc_BarLine(RLine, *pRBars, false, MainBarLocArr, ColumnBarType_RightBar, dLRLayerDist, nTopNumLayer);
	}
	return;
}

void MSRcLineMemberM::GetMainBarLoc_Polygon(vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation)
{
	MRColumnM* pMRM = (MRColumnM*)mp_MRM;
	if (pMRM == NULL)
		return;
	MRRcSection* pSec = (MRRcSection*)pMRM->ma_Section.GetAt(0);//Default이기때문에 첫번째 section에서
	if (pSec == NULL)
		return;
	SetMainBarLoc_Sec(pSec, MainBarLocArr, dAxisRotation);//주근위치결정
}
CString MSRcLineMemberM::GetInitial(eColumnBarType eType)
{
	switch(eType)
	{
	case ColumnBarType_TopBar:			return _T("T");
	case ColumnBarType_BotBar:			return _T("B");
	case ColumnBarType_LeftBar:			return _T("L");
	case ColumnBarType_RightBar:			return _T("R");
	default:										return _T("A");
	}
}
int MSRcLineMemberM::GetMainBarLoc_BarLine(GM2DLineVector& barLine, MSMainBarSet& mainBars, bool placingConer, vector<T_BarLoc*>& MainBarLocArr, eColumnBarType eType, double dLayerDist, int nOrthoNumLayer)
{
	CString sInitial = GetInitial(eType);
	long numLoc = 0;
	T_BarLoc* pBarLoc = NULL;
	vector<map<int,T_BarLoc*>*> LocArr;
	map<int,T_BarLoc*>* Layer1 = new map<int,T_BarLoc*>;

	long nBarNumLayer1 = mainBars.GetNumBarsOnSet(0);
	double dBarSpace = 100;
	if (placingConer)
		nBarNumLayer1 -= 2;

	double dConerBarDeg = 45;
	vector<CString> DiaArr = mainBars.MakeLayerDiaArr(0);
	bool bUserSteelGradeS = mainBars.GetUserSteelGradeS();
	double dBarDeg = barLine.GetDegree();
	if(eType == ColumnBarType_TopBar || eType == ColumnBarType_RightBar)
	{
		dBarDeg = barLine.GetDegree();
		barLine.ReverseDirection();
		dConerBarDeg = -dConerBarDeg;
		dLayerDist = -dLayerDist;
	}
	if (nBarNumLayer1 < 0) return 0;
	dBarSpace = barLine.Length() / (nBarNumLayer1 + 1);
	CString sBarName = _T("");
	
	long numLayer = mainBars.GetNumSets();
	int index = 0;
	int StartLoc = 0;
	int iDia = 0;
	if(eType == ColumnBarType_LeftBar || eType == ColumnBarType_RightBar)
	{
		if (numLayer > nOrthoNumLayer)
			StartLoc = nOrthoNumLayer - 1;
		else
			StartLoc = 1;
	}
	if (placingConer)
	{
		CString sDia = DiaArr[index];		
		MSDeformedBar bar(sDia);
		StartLoc++;
		pBarLoc = new T_BarLoc(barLine.m_P0, dBarDeg - dConerBarDeg,bar.m_Dia, bUserSteelGradeS);
		pBarLoc->SetIsCornerBar(TRUE);
		sBarName.Format(_T("%s%ld"), sInitial, index);
		pBarLoc->SetBarName(sBarName);
		Layer1->insert(make_pair(index,pBarLoc));
		index++;
		iDia++;
	}
	for (long iLoc = 0; iLoc < nBarNumLayer1; iLoc++)
	{
		CString sDia = DiaArr[iDia];
		MSDeformedBar bar(sDia);
		pBarLoc = new T_BarLoc(barLine.PointVector(dBarSpace*(iLoc + 1) / barLine.Length()), dBarDeg,bar.m_Dia, bUserSteelGradeS);
		pBarLoc->SetIsCornerBar(FALSE);
		if(eType == ColumnBarType_LeftBar || eType == ColumnBarType_RightBar)
			sBarName.Format(_T("%s%ld"), sInitial, index+1);
		else
			sBarName.Format(_T("%s%ld"), sInitial, index);

		pBarLoc->SetBarName(sBarName);
		Layer1->insert(make_pair(index,pBarLoc));
		index++;
		iDia++;

	}
	if (placingConer)
	{
		CString sDia = DiaArr[iDia];
		bool isUserSteelGradeS = sDia.Find('S') != -1;
		MSDeformedBar bar(sDia);
		pBarLoc = new T_BarLoc(barLine.m_P1, dBarDeg + dConerBarDeg,bar.m_Dia, bUserSteelGradeS);
		pBarLoc->SetIsCornerBar(TRUE);
		sBarName.Format(_T("%s%ld"), sInitial, index);
		pBarLoc->SetBarName(sBarName);
		Layer1->insert(make_pair(index,pBarLoc));
		index++;
		iDia++;
	}
	LocArr.push_back(Layer1);

	if (numLayer > 1)
	{
		GM2DVector DirVec  = barLine.DirVector();
		GM2DVector Origin;
		DirVec.RotateZ(0.5*PI, &Origin);
		GM2DVector TransVec = DirVec.UnitVector()*dLayerDist;

		map<int,T_BarLoc*>* preLayer = Layer1;
		for (int iL = 1; iL < numLayer; iL++)
		{
			long nCurBarNum = mainBars.GetNumBarsOnSet(iL);
			vector<CString> DiaArr1 = mainBars.MakeLayerDiaArr(iL);

			long nPreBarNum = preLayer->size();
			int nStartLocStep = (iL - 1);
			if (iL > nOrthoNumLayer)
				nStartLocStep = 0;
			if (nPreBarNum - 2 >= nCurBarNum)
			{
				numLoc = 0;
				int iTurn = 0;
				map<int,T_BarLoc*>* newLayer = new map<int,T_BarLoc*>;
				for (int iLoc = StartLoc + nStartLocStep; iLoc < Layer1->size(); iLoc++)
				{
					T_BarLoc* pLoc = Layer1->at(iLoc);
					if (numLoc >= nCurBarNum)
						break;
					pBarLoc = pLoc->Copy();
					MSDeformedBar barL(DiaArr1[iTurn]);
					pBarLoc->SetDia(barL.m_Dia);
					pBarLoc->SetIsCornerBar(FALSE);
					GM2DVector LocVec = pBarLoc->GetLoc();
					LocVec.Translate(TransVec * iL);
					pBarLoc->SetLoc(LocVec);
					newLayer->insert(make_pair(iLoc,pBarLoc));
					numLoc++;
					if (numLoc >= nPreBarNum || numLoc >= nCurBarNum)
						break;
					pLoc = Layer1->at(Layer1->size() - iLoc - 1);
					pBarLoc = pLoc->Copy();
					MSDeformedBar barR(DiaArr1[nCurBarNum-1-iTurn]);
					pBarLoc->SetDia(barR.m_Dia);
					pBarLoc->SetIsCornerBar(FALSE);
					LocVec = pBarLoc->GetLoc();
					LocVec.Translate(TransVec * iL);
					pBarLoc->SetLoc(LocVec);
					int key = Layer1->size() - iLoc - 1;
					newLayer->insert(make_pair(key, pBarLoc));
					numLoc++;
					iTurn++;
				}
				preLayer = newLayer;
				LocArr.push_back(newLayer);
			}

		}
	}
	long nRootType = GetRootType(eType);
	for (auto it = LocArr.begin(); it != LocArr.end(); it++)
	{
		map<int,T_BarLoc*>* pLayer = *it;
		for(auto itL = pLayer->begin();itL!=pLayer->end();itL++)
		{
			itL->second->SetRootType(nRootType);
			MainBarLocArr.push_back(itL->second);
		}
		delete pLayer;
	}
	return MainBarLocArr.size();
}
long MSRcLineMemberM::GetRootType(eColumnBarType eType)
{
	switch(eType)
	{
	case ColumnBarType_AllBar:				return COLUMN_MAIN;
	case ColumnBarType_TopBar:			return COLUMN_MAIN_TOP;
	case ColumnBarType_BotBar:			return COLUMN_MAIN_BOTTOM;
	case ColumnBarType_LeftBar:			return COLUMN_MAIN_LEFT;
	case ColumnBarType_RightBar:			return COLUMN_MAIN_RIGHT;
	default:										return -1;
	}
}
int MSRcLineMemberM::GetMainBarLoc_BarLineByDist(GM2DLineVector& barLine, MSMainBarSet& mainBars, bool placingConer, vector<T_BarLoc*>& MainBarLocArr, eColumnBarType eType, double dLayerDist)
{
	CString sInitial = GetInitial(eType);
	long numLoc = 0;
	T_BarLoc* pBarLoc = NULL;
	vector<vector<T_BarLoc*>*> LocArr;
	vector<T_BarLoc*>* Layer1 = new vector<T_BarLoc*>;
	long nBarNumLayer1 = mainBars.GetNumBarsOnSet(0);
	bool bUserSteelGradeS = mainBars.GetUserSteelGradeS();
	vector<CString> DiaArr = mainBars.MakeLayerDiaArr(0);
	double dMaxDia = mainBars.GetMaxDeformedBarDiameterOnSet(0);

	if (placingConer)
		nBarNumLayer1 -= 2;

	bool IsOddNumber = false;
	if (nBarNumLayer1 % 2 == 1)
		IsOddNumber = true;

	double dConerBarDeg = 45;
	double dBarDeg = barLine.GetDegree();
	
	if(eType == ColumnBarType_TopBar || eType == ColumnBarType_RightBar)
	{
		dBarDeg = barLine.GetDegree();
		barLine.ReverseDirection();
		dConerBarDeg = -dConerBarDeg;
		dLayerDist = -dLayerDist;
	}

	double dBarDist = 40 + dMaxDia;
	double dBarSpace = dBarDist / barLine.Length();
	double dCurLoc = 0;
	int iDia = 0;

	CString sBarName = _T("");
	int index = 0;

	if (placingConer)
	{
		MSDeformedBar bar(DiaArr[iDia]);
		pBarLoc = new T_BarLoc(barLine.m_P0, dBarDeg - dConerBarDeg,bar.m_Dia, bUserSteelGradeS);
		pBarLoc->SetIsCornerBar(TRUE);
		sBarName.Format(_T("%s%ld"), sInitial, index);
		pBarLoc->SetBarName(sBarName);
		Layer1->push_back(pBarLoc);
		iDia++;
	}
	for (long iLoc = 0; iLoc < nBarNumLayer1 / 2; iLoc++)
	{
		dCurLoc = dBarSpace*(iLoc + 1);
		MSDeformedBar bar(DiaArr[iDia]);
		pBarLoc = new T_BarLoc(barLine.PointVector(dCurLoc), dBarDeg,bar.m_Dia, bUserSteelGradeS);
		pBarLoc->SetIsCornerBar(FALSE);
		index++;
		sBarName.Format(_T("%s%ld"), sInitial, index);
		pBarLoc->SetBarName(sBarName);
		Layer1->push_back(pBarLoc);
		iDia++;
	}

	if (IsOddNumber)
	{
		//pBarLoc = new T_BarLoc(barLine.PointVector(0.5), barLine.GetDegree());
		MSDeformedBar bar(DiaArr[iDia]);
		pBarLoc = new T_BarLoc(barLine.PointVector(dCurLoc + dBarSpace), dBarDeg,bar.m_Dia, bUserSteelGradeS);
		pBarLoc->SetIsCornerBar(FALSE);
		index++;
		sBarName.Format(_T("%s%ld"), sInitial, index);
		pBarLoc->SetBarName(sBarName);
		Layer1->push_back(pBarLoc);
		iDia++;
	}
	dCurLoc = 1 - dCurLoc;
	for (long iLoc = index; iLoc < nBarNumLayer1; iLoc++)
	{
		MSDeformedBar bar(DiaArr[iDia]);
		pBarLoc = new T_BarLoc(barLine.PointVector(dCurLoc), dBarDeg,bar.m_Dia, bUserSteelGradeS);
		pBarLoc->SetIsCornerBar(FALSE);
		index++;
		sBarName.Format(_T("%s%ld"), sInitial, index);
		pBarLoc->SetBarName(sBarName);
		Layer1->push_back(pBarLoc);
		iDia++;
		dCurLoc += dBarSpace;
	}

	if (placingConer)
	{
		MSDeformedBar bar(DiaArr[iDia]);
		pBarLoc = new T_BarLoc(barLine.m_P1, dBarDeg + dConerBarDeg,bar.m_Dia, bUserSteelGradeS);
		pBarLoc->SetIsCornerBar(TRUE);
		index++;
		sBarName.Format(_T("%s%ld"), sInitial, index);
		pBarLoc->SetBarName(sBarName);
		Layer1->push_back(pBarLoc);
	}
	LocArr.push_back(Layer1);
	long numLayer = mainBars.GetNumSets();
	if (numLayer > 1)
	{
		double dBarDist = 40 + mainBars.GetMaxDeformedBarDiameterOnSet(1);
		if(eType == ColumnBarType_TopBar || eType == ColumnBarType_RightBar)
			dBarDist = dBarDist*-1;

		vector<T_BarLoc*>* preLayer = Layer1;
		for(int iL = 1;iL < numLayer;iL++)
		{
			long nCurBarNum = mainBars.GetNumBarsOnSet(iL);
			long nPreBarNum = preLayer->size();

			GM2DLineVector moveLine = barLine.NormalTranslate(-dBarDist*iL);
			GM2DVector TransVec = barLine.m_P0 - moveLine.m_P0;

			if (nPreBarNum - 2 >= nCurBarNum)
			{
				numLoc = 0;
				vector<T_BarLoc*>* newLayer = new vector<T_BarLoc*>;
				for (int iLoc = 1; iLoc < preLayer->size(); iLoc++)
				{
					T_BarLoc* pLoc = preLayer->at(iLoc);
					if (numLoc >= nCurBarNum)
						break;
					pBarLoc = pLoc->Copy();
					pBarLoc->SetIsCornerBar(FALSE);
					GM2DVector LocVec = pBarLoc->GetLoc();
					LocVec.Translate(TransVec);
					pBarLoc->SetLoc(LocVec);
					newLayer->push_back(pBarLoc);
					numLoc++;
					if (numLoc >= nPreBarNum)
						break;
					pLoc = preLayer->at(nPreBarNum - iLoc - 1);
					pBarLoc = pLoc->Copy();
					pBarLoc->SetIsCornerBar(FALSE);					
					LocVec = pBarLoc->GetLoc();
					LocVec.Translate(TransVec);
					pBarLoc->SetLoc(LocVec);
					newLayer->push_back(pBarLoc);
					numLoc++;
				}
				preLayer = newLayer;
				LocArr.push_back(newLayer);
			}
		}
	}
	long nRootType = GetRootType(eType);
	for (auto it = LocArr.begin(); it != LocArr.end(); it++)
	{
		vector<T_BarLoc*>* pLayer = *it;
		for(auto itL = pLayer->begin();itL!=pLayer->end();itL++)
		{
			(*itL)->SetRootType(nRootType);
			MainBarLocArr.push_back(*itL);
		}
		delete pLayer;
	}

	return MainBarLocArr.size();
}

long MSRcLineMemberM::SetMainBarLoc_Sec(MRRcSection* pSec, vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation)
{
	long numEdge = pSec->ma_Edge.GetSize();

	for (int iEdge = 0; iEdge < numEdge; iEdge++)
	{
		MREdge* pEdge = pSec->ma_Edge[iEdge];
		if (pEdge == NULL)	continue;
		SetMainBarLoc_Edge(pEdge, MainBarLocArr, dAxisRotation);
	}
	return MainBarLocArr.size();

}

long MSRcLineMemberM::SetMainBarLoc_Edge(MREdge* pEdge, vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation)
{
	GM2DVector BaseVec(0, 0);
	GM2DLineVector* BarLine = (GM2DLineVector*)pEdge->mp_StrpCurve;//mp_Curve;//mp_VBarCurve;
	GM2DLineVector VbarLine = *BarLine;//->NormalTranslate(80);
	
	VbarLine.RotateZ(dAxisRotation*Deg2Rad, &BaseVec);
	GM2DLineVector VBarLine = VbarLine;//.NormalTranslate(80);
	VBarLine.ReverseDirection();

	long numVbar = pEdge->ma_VBarSet.GetSize();
	double dDeg = 0;
	for (int iBar = 0; iBar < numVbar; iBar++)
	{
		MRVBar*  pBar = pEdge->ma_VBarSet.GetAt(iBar);
		if (pBar == NULL)	continue;

		//if(pBar->m_LayerNum>1)
		//	VBarLine = VbarLine.NormalTranslate(minBarDist()*(pBar->m_LayerNum-1));

		int numLoc = pBar->ma_VBarLoc.GetSize();
		for (int iLoc = 0; iLoc < numLoc; iLoc++)
		{
			dDeg = VBarLine.GetDegree();
			MSDeformedBar bar(pBar->GetDia());
			T_BarLoc* pBarLoc = new T_BarLoc(VBarLine.PointVector(pBar->ma_VBarLoc[iLoc]), dDeg,bar.m_Dia, false);
			//if(iLoc%2==0 && (iLoc ==0 || iLoc == numLoc-1)&&iBar == 0)
			//{
			//	if(iLoc == 0)
			//		pBarLoc->m_dBarDeg-=45;
			//	else if(iLoc == numLoc-1)
			//		pBarLoc->m_dBarDeg+=45;

			//	pBarLoc->m_IsConerBar = TRUE;
			//}
			MainBarLocArr.push_back(pBarLoc);
		}

		//vector<double> BarLocArr;
		//int numLoc = pBar->GetBarLocArr(BarLocArr);
		//for(int iLoc = 0;iLoc<numLoc;iLoc++)
		//{
		//	dDeg =  VbarLine.GetDegree();
		//	T_BarLoc* pBarLoc = new T_BarLoc(VBarLine.PointVector(BarLocArr[iLoc]),dDeg);
		//	pBarLoc->m_nDia = pBar->GetDia();
		//	if((iLoc ==0 || iLoc == numLoc-1)&&pBar->Get_bHasCornerBar() &&iBar == 0)
		//	{
		//		if(iLoc == 0)
		//			pBarLoc->m_dBarDeg-=45;
		//		else if(iLoc == numLoc-1)
		//			pBarLoc->m_dBarDeg+=45;

		//		pBarLoc->m_IsConerBar = TRUE;
		//	}
		//	MainBarLocArr.push_back(pBarLoc);
		//}
	}
	return MainBarLocArr.size();

}

int MSRcLineMemberM::GetLayerNumbyBarType(eColumnBarType eType)
{
	int nLayerNum = 0;

	MSMainBarSet* pTBars = NULL;
	MSMainBarSet* pBBars = NULL;
	MSMainBarSet* pLBars = NULL;
	MSMainBarSet* pRBars = NULL;

	if (IsRC())
	{
		MSRcColumnM* pRcColumnM = (MSRcColumnM*)GetRebarM();
		if (pRcColumnM == NULL) return 0;

		pTBars = &pRcColumnM->m_TBars;
		pBBars = &pRcColumnM->m_BBars;
		pLBars = &pRcColumnM->m_LBars;
		pRBars = &pRcColumnM->m_RBars;
	}
	else if (IsSRC())
	{
		MDSrcColumnM* pSrcColumnM = (MDSrcColumnM*)this->GetRebarM();
		if (pSrcColumnM == NULL) return 0;

		pTBars = &pSrcColumnM->m_TBars;
		pBBars = &pSrcColumnM->m_BBars;
		pLBars = &pSrcColumnM->m_LBars;
		pRBars = &pSrcColumnM->m_RBars;
	}

	if (GetShape() == MSRcLineMemberM::RECT)
	{
		if (eType == eColumnBarType::ColumnBarType_AllBar)
		{
			nLayerNum = max(nLayerNum, pTBars->GetNumSets());
			nLayerNum = max(nLayerNum, pBBars->GetNumSets());
			nLayerNum = max(nLayerNum, pLBars->GetNumSets());
			nLayerNum = max(nLayerNum, pRBars->GetNumSets());
		}
		else if (eType == eColumnBarType::ColumnBarType_TopBar)
			nLayerNum = pTBars->GetNumSets();
		else if (eType == eColumnBarType::ColumnBarType_BotBar)
			nLayerNum = pBBars->GetNumSets();
		else if (eType == eColumnBarType::ColumnBarType_LeftBar)
			nLayerNum = pLBars->GetNumSets();
		else if (eType == eColumnBarType::ColumnBarType_RightBar)
			nLayerNum = pRBars->GetNumSets();
	}
	else if (GetShape() == MSRcLineMemberM::CIRCLE)
	{
		nLayerNum += pTBars->GetNumSets();
	}
	else if (GetShape() == MSRcLineMemberM::POLYGON)
	{
		nLayerNum += pTBars->GetNumSets();
	}

	return nLayerNum;
}

int MSRcLineMemberM::GetLayerBarCnt(int nLayerIdx, eColumnBarType eType)
{
	int nBarCnt = 0;

	MSMainBarSet* pTBars = NULL;
	MSMainBarSet* pBBars = NULL;
	MSMainBarSet* pLBars = NULL;
	MSMainBarSet* pRBars = NULL;

	if (IsRC())
	{
		MSRcColumnM* pRcColumnM = (MSRcColumnM*)GetRebarM();
		if (pRcColumnM == NULL) return 0;

		pTBars = &pRcColumnM->m_TBars;
		pBBars = &pRcColumnM->m_BBars;
		pLBars = &pRcColumnM->m_LBars;
		pRBars = &pRcColumnM->m_RBars;
	}
	else if (IsSRC())
	{
		MDSrcColumnM* pSrcColumnM = (MDSrcColumnM*)this->GetRebarM();
		if (pSrcColumnM == NULL) return 0;

		pTBars = &pSrcColumnM->m_TBars;
		pBBars = &pSrcColumnM->m_BBars;
		pLBars = &pSrcColumnM->m_LBars;
		pRBars = &pSrcColumnM->m_RBars;
	}

	if (GetShape() == MSRcLineMemberM::RECT)
	{
		if (eType == eColumnBarType::ColumnBarType_AllBar)
		{
			nBarCnt += pTBars->GetNumBarsOnSet(nLayerIdx);
			nBarCnt += pBBars->GetNumBarsOnSet(nLayerIdx);
			nBarCnt += pLBars->GetNumBarsOnSet(nLayerIdx);
			nBarCnt += pRBars->GetNumBarsOnSet(nLayerIdx);
		}
		else if (eType == eColumnBarType::ColumnBarType_TopBar)
			nBarCnt += pTBars->GetNumBarsOnSet(nLayerIdx);
		else if (eType == eColumnBarType::ColumnBarType_BotBar)
			nBarCnt += pBBars->GetNumBarsOnSet(nLayerIdx);
		else if (eType == eColumnBarType::ColumnBarType_LeftBar)
			nBarCnt += pLBars->GetNumBarsOnSet(nLayerIdx);
		else if (eType == eColumnBarType::ColumnBarType_RightBar)
			nBarCnt += pRBars->GetNumBarsOnSet(nLayerIdx);
	}
	else if (GetShape() == MSRcLineMemberM::CIRCLE)
	{
		nBarCnt += pTBars->GetNumBarsOnSet(nLayerIdx);
	}
	else if (GetShape() == MSRcLineMemberM::POLYGON)
	{
		
	}

	return nBarCnt;
}

double MSRcLineMemberM::GetSpecificTieHeight(CString sInitial, GM2DLineVector* TheLine)
{
	//현재 Rect일때만
	double dLen = 0;
	double dCover = GetCover();

	if (IsRC())
	{
		if (GetShape() == MSRcLineMemberM::RECT)
		{
			if (sInitial == _T("T") || sInitial == _T("B"))
				dLen = GetDepth() - 2 * dCover;
			else
				dLen = (GetWidth() - 2 * dCover);
		}
		else if (GetShape() == MSRcLineMemberM::CIRCLE)
		{
			if (TheLine != NULL && sInitial == _T("C"))
			{
				GM2DVector BaseVec(0, 0);
				dLen = TheLine->Distance(BaseVec) * 2;
			}
			else
				dLen = 0;
		}
	}
	else if (IsSRC())
	{
		MSSteelSection* pSec = GetSteelSection();

		double dDist = 20;//Steel 과 rebar 사이간격
		double dWebThick = pSec->GetWThk();
		//차후 편심고려시켜야함.

		bool bUsedHor = false;
		bool bUsedVer = false;

		double dDeg = TheLine->GetDegree();
		double dTolDeg = 2;
		if ( (dDeg < dTolDeg || (dDeg > 180 - dTolDeg && dDeg < 180 + dTolDeg) || dDeg > 360 - dTolDeg)
			&& (sInitial == _T("T") || sInitial == _T("B") || sInitial == _T("C"))
			)
			bUsedHor = true;

		if ( ( (dDeg > 90 - dTolDeg && dDeg <90 + dTolDeg) || (dDeg > 270 - dTolDeg && dDeg < 270 + dTolDeg) )
			&& (sInitial == _T("L") || sInitial == _T("R") || sInitial == _T("C"))
			)
			bUsedVer = true;

		if (bUsedHor == true && GetShape() == MSRcLineMemberM::RECT)
		{
			dLen = GetDepth() / 2 - GetCover() - pSec->GetH() / 2 - dDist;
		}
		else if (bUsedVer == true)
		{
			if (GetShape() == MSRcLineMemberM::RECT)
				dLen = GetWidth() / 2 - GetCover() - dWebThick / 2 - dDist;
			else
			{
				GM2DVector BaseVec(0, 0);
				dLen = TheLine->Distance(BaseVec);
				dLen -= dWebThick / 2;
			}
		}
		else
		{
			GM2DPolyline TheGon(TRUE);
			pSec->GetSecPolyline(TheGon);

			double dMinDist = (min(GetWidth(), GetDepth()) - 2 * dCover);
			for (long iVec = 0; iVec < TheGon.ma_Point.GetSize(); iVec++)
			{
				GM2DVector* pVec = TheGon.ma_Point[iVec];
				GM2DVector TheVec(pVec->m_X, pVec->m_Y);
				double dDist = TheLine->Distance(TheVec);
				dMinDist = min(dMinDist, dDist);
			}
			dLen = dMinDist;
		}

// 		if (sInitial == _T("T"))
// 		{
// 			dLen = GetDepth() / 2 - GetCover() - pSec->GetH() / 2 - dDist;
// 		}
// 		else if (sInitial == _T("B"))
// 		{
// 			dLen = GetDepth() / 2 - GetCover() - pSec->GetH() / 2 - dDist;
// 		}
// 		else if (sInitial == _T("L"))
// 		{
// 			dLen = GetWidth() / 2 - GetCover() - dWebThick / 2 - dDist;
// 		}
// 		else if (sInitial == _T("R"))
// 		{
// 			dLen = GetWidth() / 2 - GetCover() - dWebThick / 2 - dDist;
// 		}
// 		else if (sInitial == _T("C"))
// 		{
// 			GMPolygon TheGon(TRUE);
// 			pSec->GetSecPolygon(TheGon);
// 
// 			double dMinDist = (GetWidth() - 2 * dCover);
// 			for (long iVec = 0; iVec < TheGon.ma_Vector.GetSize(); iVec++)
// 			{
// 				GMVector* pVec = TheGon.ma_Vector[iVec];
// 				GM2DVector TheVec(pVec->m_X, pVec->m_Y);
// 				double dDist = TheLine->Distance(TheVec);
// 				dMinDist = min(dMinDist, dDist);
// 			}
// 			dLen = dMinDist;
// 		}
	}
	
	return dLen;
}

double MSRcLineMemberM::GetDepthDefaultValue()
{
	return 500;
}



CString MSRcLineMemberM::ToString()
{
	auto str = MSLineMemberM::ToString();
	str.AppendFormat(L" ShapeType:%d, Width:%f, Depth:%f", GetShape(), GetWidth(), GetDepth());
	return str;
}
