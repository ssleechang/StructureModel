#include "StdAfx.h"
#include "MSColumnG.h"
#include "MSRcLineMemberM.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSPitG.h"
#include "MSIsoFootG.h"
#include "MSLinkMemberG.h"
#include "MSSlabG.h"
#include "MSRcColumnM.h"
#include "MDSrcColumnM.h"
#include "MSSrcColumnM.h"
#include "MSSteelColumnM.h"
#include "MSColumnHeaderM.h"

#include "MSSteelUtility.h"
#include "MSSteelSection.h"
#include "MSRcFWColumnM.h"

#include <math.h>

#include "GMLib/MSVersionInfo.h"
#include "RADFCore_CLI/AppGlobalOptionWrapper.h"

IMPLEMENT_SERIAL(MSColumnG, MSVerticalLineMemberG, VERSIONABLE_SCHEMA | 1)
MSColumnG::MSColumnG()
{
	Init(true);
}

MSColumnG::~MSColumnG(void)
{
	
}

void MSColumnG::Dump(EFS::EFS_Buffer& buffer)
{
	MSVerticalLineMemberG::Dump(buffer);

	m_StartCondition.Dump(buffer);
	m_EndCondition.Dump(buffer);
	m_SteelCenter.Dump(buffer);
	
	buffer << m_dColumnHeaderOffset;
	buffer << m_dColumnHeaderSubOffset;
}

void MSColumnG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSVerticalLineMemberG::Recovery(buffer);

	m_StartCondition.Recovery(buffer);
	m_EndCondition.Recovery(buffer);
	m_SteelCenter.Recovery(buffer);
	
	buffer >> m_dColumnHeaderOffset;
	buffer >> m_dColumnHeaderSubOffset;
}

void MSColumnG::Serialize(CArchive &ar)
{
	MSVerticalLineMemberG::Serialize(ar);
	bool bEachCompoM = false;
	if(ar.IsStoring())
	{
		if(mp_CompoM)
		{
			if(mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if(bEachCompoM == true)
			{
				Serialize_EachCompoM(ar);
			}
		}
		m_StartCondition.Serialize(ar);
		m_EndCondition.Serialize(ar);
		m_SteelCenter.Serialize(ar);
		ar << m_nLocationCalculationType;
		ar << m_nLocationCalculationResult;
		ar << m_dColumnHeaderOffset;
		ar << m_dColumnHeaderSubOffset;
	}
	else
	{
		ar >> bEachCompoM;
		if(bEachCompoM == true)
		{
			Serialize_EachCompoM(ar);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150722)
		{
			m_StartCondition.Serialize(ar);
			m_EndCondition.Serialize(ar);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150727)
			m_SteelCenter.Serialize(ar);

		if(IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20220509)
		{
			ar >> m_nLocationCalculationType;
			ar >> m_nLocationCalculationResult;
		}

 		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20221011)
 			ar >> m_dColumnHeaderOffset;
		
		if (MSVersionInfo::GetCurrentVersion() >= 20240626)
			ar >> m_dColumnHeaderSubOffset;
	}
}


void MSColumnG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
		long nType = mp_CompoM->m_nType;
		ar << nType;
		mp_CompoM->Serialize(ar);
	}
	else
	{
		if ((MSObject::IsBHQorBHCEMode() && MSVersionInfo::GetCurrentVersion() < 20220321)
			|| (MSObject::IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() < 20240307))
		//if(MSVersionInfo::GetCurrentVersion() < 20220321)
		{
			MSBaseBuilding* pBldg = GetBuilding();
			long MType = 0;
			ar >> MType;

			MSCompoM* pCompoM = pBldg->CreateNewCompoM(MType);
			pCompoM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pCompoM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pCompoM->m_Name;

			if (pCompoM->GetType() == MSCompoM::msColumnM || pCompoM->GetType() == MSCompoM::msSrcColumnM)
			{
				MSRcLineMemberM* pLineM = (MSRcLineMemberM*)pCompoM;
				int nShape = 0;
				ar >> nShape;
				pLineM->SetShape(nShape);

				if (nShape == MSRcLineMemberM::RECT || nShape == MSRcLineMemberM::CIRCLE)
				{
					double dWidth = 0, dDepth = 0;
					ar >> dWidth;
					ar >> dDepth;

					pLineM->SetWidth(dWidth);
					pLineM->SetDepth(dDepth);
				}
				else if (nShape == MSRcLineMemberM::POLYGON)
				{
					int nVector = 0;
					ar >> nVector;
					for (int iVec = 0; iVec < nVector; iVec++)
					{
						GMVector* pVec = new GMVector();
						pVec->Serialize(ar);
						pLineM->m_SecPGon.Add(pVec);
					}
				}

				if (pCompoM->GetType() == MSCompoM::msSrcColumnM)
				{
					long sectionType = 0, sectionID = 0;
					ar >> sectionType;
					ar >> sectionID;
					MSSrcColumnM* pSrcColumnM = (MSSrcColumnM*)pCompoM;
					pSrcColumnM->m_nSecType = sectionType;
					MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(GetBuilding());
					if(pRebarBuilding != nullptr)
						pSrcColumnM->m_pSection = pRebarBuilding->GetSteelUtili()->GetSection(sectionType, sectionID);
				}
			}
			else if (pCompoM->GetType() == MSCompoM::msSteelColumnM)
			{
				long sectionType = 0, sectionID = 0;
				ar >> sectionType;
				ar >> sectionID;
				MSSteelColumnM* pSteelColumnM = (MSSteelColumnM*)pCompoM;
				pSteelColumnM->m_nSecType = sectionType;
				MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(GetBuilding());
				if (pRebarBuilding != nullptr)
					pSteelColumnM->m_pSection = pRebarBuilding->GetSteelUtili()->GetSection(sectionType, sectionID);
			}
			pCompoM->mp_Owner = pBldg;
			mp_CompoM = pCompoM;
		}
		else
		{
			long nType = 0;
			ar >> nType;
			mp_CompoM = GetBuilding()->CreateNewCompoM(nType);
			mp_CompoM->Serialize(ar);
		}
	}
}

MSObject* MSColumnG::Copy( bool bNewID /*= true*/ )
{
	MSColumnG* pNewObj;

	pNewObj = new MSColumnG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSColumnG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msColumnG) { ASSERT(0); return; }

	MSColumnG* pSource = (MSColumnG*)pSource_in;
	m_StartCondition.CopyFromMe(&pSource->m_StartCondition);
	m_EndCondition.CopyFromMe(&pSource->m_EndCondition);
	m_SteelCenter.CopyFrom(&pSource->GetSteelCenter());
	MSVerticalLineMemberG::CopyFromMe(pSource_in, bNewID);
	this->SetLocationCalculationType(pSource->GetLocationCalculationType());
	this->SetLocationCalculationResult(pSource->GetLocationCalculationResult());
	
	m_dColumnHeaderOffset = pSource->GetColumnHeaderOffset();
	m_dColumnHeaderSubOffset = pSource->GetColumnHeaderSubOffset();
}

void MSColumnG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSVerticalLineMemberG::Init(bInConstructor);

	m_nLocationCalculationType = 0;
	m_nLocationCalculationResult = 0;
	m_dColumnHeaderOffset = 0;
	m_dColumnHeaderSubOffset = 0;
}

double MSColumnG::GetDepth()
{
	if(mp_CompoM)
	{
		MSRcFWColumnM* pRcFWLineM = dynamic_cast<MSRcFWColumnM*>(mp_CompoM);
		MSRcLineMemberM* pLineM = dynamic_cast<MSRcLineMemberM*>(mp_CompoM);
		if (pRcFWLineM != nullptr)
		{
			return mp_CompoM->GetDepth();
		}
		else if (pLineM != nullptr)
		{
			if (pLineM->GetShape() == MSRcLineMemberM::RECT)
				return mp_CompoM->GetDepth();
			else
				return 0;
		}
		else
		{
			return mp_CompoM->GetDepth();
		}
	}
	return 0;
}

bool MSColumnG::CheckHeight(bool IsUseGroupName)
{
	bool isValid = true;
	// Height Check
	if (GetFloor() == NULL || GetFloor()->IsBlock()) { } // check skip
	else
	{

		CString groupName(_T("Column"));
		/*
		if (m_SOffsetZ < 0)
		{
			CString msg(_T("Error : Bottom Offset >= 0"));
			mp_MsgObject = NULL;
			AddErrorObject(0, -1, groupName, msg);
			return false;
		}
		if (m_EOffsetZ > 0)
		{
			CString msg(_T("Error : Top Offset <= 0"));
			mp_MsgObject = NULL;
			AddErrorObject(0, -1, groupName, msg);
			return false;
		}
		*/
		double bot = GetStartLevel();
		double top = GetEndLevel();
		if (bot >= top)
		{
			CString strGroupName = _T("");
			CString msg = LocalDic::GetLocal(_T("오프셋과 레벨존을 적용한 수직부재의 높이가 0보다 작습니다."), _T("CheckData"));
			if (IsUseGroupName == true)
			{
				if (m_ID != 0)
					strGroupName.Format(_T("[%s_%s(ID:%d)]: "), GetFloor()->GetName(), GetGroupName(), m_ID);
			}

			mp_MsgObject = NULL;
			AddErrorObject(0, -1, groupName, strGroupName + msg);
			isValid = false;
		}

		if (HasColumnHeaderM())
		{
			MSColumnHeaderM* pColumnHeaderM = dynamic_cast<MSColumnHeaderM*>(GetColumnHeaderM());
			if (pColumnHeaderM != nullptr)
			{
				double dMaxHeaderHeight = max(pColumnHeaderM->GetT1(), pColumnHeaderM->GetT3());

				double dHeaderTopLevel = top + m_dColumnHeaderOffset;
				double dHeaderBotLevel = dHeaderTopLevel - dMaxHeaderHeight;

				GM2DPolyline headerProfile;
				GetHeaderPolylineByInsertVec(headerProfile);

				vector<MSCompoG*> partnerGArr = GetPartnerCompoGs();
				for (int iPartner = 0; iPartner < partnerGArr.size(); iPartner++)
				{
					MSCompoG* pPartnerG = partnerGArr[iPartner];
					MSBeamG* pBeamG = dynamic_cast<MSBeamG*>(pPartnerG);
					if (pBeamG != nullptr)
					{
						double dBeamTopLevel = 0, dBeamBotLevel = 0;
						pBeamG->GetZLevels(dBeamTopLevel, dBeamBotLevel);

						bool isZOverLap = false;
						if (dHeaderBotLevel <= dBeamTopLevel && dHeaderTopLevel >= dBeamTopLevel)
							isZOverLap = true;
						if (isZOverLap)
						{

							shared_ptr<GM2DCurve> cenCurve = pBeamG->GetLongCenCurveTrimmedByTRBoundary();
							bool isOverlap = headerProfile.OverlapEx(&*cenCurve, DTOL_MODELING, false);
							if (isOverlap)
							{
								CString strGroupName = _T("");
								CString msg = LocalDic::GetLocal(_T("주두부와 겹쳐진 Beam이 존재 합니다."));
								if (IsUseGroupName == true)
								{
									if (m_ID != 0)
										strGroupName.Format(_T("[%s_%s(ID:%d)]: "), GetFloor()->GetName(), GetGroupName(), m_ID);
								}

								mp_MsgObject = NULL;
								AddErrorObject(0, -1, groupName, strGroupName + msg);
								isValid = false;
							}
						}
					}
				}
			}
		}
	}

	return isValid;
}

bool MSColumnG::CheckData()
{
	if (MSElement::CheckData() == false)
		return false;

	// Height Check
	if (!CheckHeight())
		return false;

	// 부재 중첩 체크 (각도 돌린 경우는 여기서 체크해야만 함)
	MSFloor* floor = GetFloor();
	if (floor != NULL && floor->CheckCompoGOverlap_WithUpDownFloor(this, this->m_ID))
		return true;
		//return false;

	// Check add...

	return true;
}

bool MSColumnG::IsConnectedBelowColumn(MSColumnG* pUpColumn, bool bCheckOverLap)
{
	if(pUpColumn == NULL)
		return false;
	
	double dZLevel = pUpColumn->GetStartLevel() - GetEndLevel();
	if(bCheckOverLap)
	{
		if(fabs(dZLevel) > DTOL1)
			return false;		
	}
	else
	{
		if(dZLevel < -DTOL1 || dZLevel > DTOL1)
			return false;
	}

	return true;
}

MSColumnG* MSColumnG::GetUpperColumn(bool bCheckButtress)
{
	vector<MSCompoG*> columns = FindConnectMember(FindType_MSElement::msColumnG, FIND_ALLFLOOR);
	bool isButtress = IsButtress();
	
	double dMinDist = 0;
	MSColumnG* pRCol = NULL;
	GM2DVector* pSVec = GetSVec();
	double dCurTopLevel = 0;
	double dCurBotLevel = 0;
	GetZLevels(dCurTopLevel, dCurBotLevel);
	for(long i = 0 ; i < columns.size() ; i++)
	{
		MSColumnG* pColumn = (MSColumnG*)columns[i];
		if(bCheckButtress = true && isButtress != pColumn->IsButtress())
			continue;
		//Check level
		double dTopLevel = 0;
		double dBotLevel = 0;
		pColumn->GetZLevels(dTopLevel, dBotLevel);
		if(dCurBotLevel > dBotLevel)	continue;
		double dDist = pSVec->Distance(*pColumn->GetSVec());
		if(pRCol == NULL)
		{
			pRCol = pColumn;
			dMinDist = dDist;
			continue;
		}
		
		if(dMinDist > dDist)
		{
			pRCol = pColumn;
			dMinDist = dDist;
		}

	}

	return pRCol;
}

MSColumnG* MSColumnG::GetLowerColumn(bool bCheckButtress)
{
	vector<MSCompoG*> columns = FindConnectMember(FindType_MSElement::msColumnG, FIND_ALLFLOOR);
	bool isButtress = IsButtress();

	double dMinDist = 0;
	MSColumnG* pRCol = NULL;
	GM2DVector* pSVec = GetSVec();
	double dCurTopLevel = 0;
	double dCurBotLevel = 0;
	GetZLevels(dCurTopLevel, dCurBotLevel);

	for(long i = 0 ; i < columns.size() ; i++)
	{
		MSColumnG* pColumn = (MSColumnG*)columns[i];
		if(bCheckButtress = true && isButtress != pColumn->IsButtress())
			continue;
		double dTopLevel = 0;
		double dBotLevel = 0;
		pColumn->GetZLevels(dTopLevel, dBotLevel);
		if (dCurTopLevel < dTopLevel)	continue;
		double dDist = pSVec->Distance(*pColumn->GetSVec());
		if(pRCol == NULL)
		{
			pRCol = pColumn;
			dMinDist = dDist;
			continue;
		}

		if(dMinDist > dDist)
		{
			pRCol = pColumn;
			dMinDist = dDist;
		}

	}

	return pRCol;
}

bool MSColumnG::IsContainSlabinLowFloor()
{
	MSFloor* pCurFloor = GetFloor();
	if(pCurFloor == NULL)
		return false;
	MSFloor* pLowFloor = pCurFloor->GetLowerFloor();
	if(pLowFloor == NULL)
		return false;
	return pLowFloor->IsContainSlab(this);
}

bool MSColumnG::IsButtress()
{
	if(GetCompoM() == NULL)
		AfxThrowUserException();
	if(GetCompoM()->GetType() == MSCompoM::msButtressM )
		return true;
	return false;
}

bool MSColumnG::IsPC()
{
	if (GetCompoM() == NULL)
		return false;
	if (GetCompoM()->GetType() == MSCompoM::msPcColumnM)
		return true;
	return false;
}


MSIsoFootG* MSColumnG::GetLowerIsoFooting()
{
	MSIsoFootG* pRIsoFoot = NULL;
	GM2DVector* pSVec =GetSVec();
	double dMinDist = 0;

	MSFloor* pFloor = GetFloor();
	if(pFloor)
	{
		vector<MSCompoG*> IsoFootArr = FindConnectMember(FindType_MSElement::msIsoFootG, -1);

		for(long i = 0 ; i < IsoFootArr.size() ; i++)
		{
			MSIsoFootG* pIsoFoot = (MSIsoFootG*)IsoFootArr[i];

			double dDist = pSVec->Distance(*pIsoFoot->GetSVec());
			if(pRIsoFoot == NULL)
			{
				pRIsoFoot = pIsoFoot;
				dMinDist = dDist;
				continue;
			}

			if(dMinDist > dDist)
			{
				pRIsoFoot = pIsoFoot;
				dMinDist = dDist;
			}

		}

		for(long i = 0 ; i < IsoFootArr.size() ; i++)
			return (MSIsoFootG*)IsoFootArr[i];
	}
	return NULL;
}

vector<MSLinkMemberG*> MSColumnG::GetLowerSptLinkMember()
{
	map<long,long> memberIDmap;
	vector<MSLinkMemberG*> memberArr;
	MSFloor* pFloor = GetFloor();
	if(pFloor)
	{
		vector<MSCompoG*> CompoGArr = FindConnectMember(FindType_MSElement::msNULL, -1);
		for(long i = 0 ; i < CompoGArr.size() ; i++)
		{
			MSCompoG* pCompoG = CompoGArr[i];
			if(pCompoG->IsLinkMember() == FALSE)
				continue;
			memberArr.push_back((MSLinkMemberG*)pCompoG);
		}
	}
	return memberArr;
}

MSPitG* MSColumnG::GetLowerPit()
{
	MSPitG* pPitG = NULL;
	GM2DVector vec = *GetSVec();
	MSFloor* pFloor = GetFloor();
	if(pFloor)
	{
		MSFloor* pLowerFloor = pFloor->GetLowerFloor();
		if(pLowerFloor)
			pPitG = (MSPitG*)pLowerFloor->GetMSCompoG(&vec,MSCompoG::msPitG);
	}
	return pPitG;
}
MSSlabG* MSColumnG::GetLowerSlab()
{
	double dColumnBotLevel = GetStartLevel();
	vector<MSCompoG*> slabGs = FindConnectMember(FindType_MSElement::msSlabG,-1);
	GM2DVector vec = *GetSVec();
	double dBotLevel = 0, dTopLevel = 0;
	MSSlabG* pLowerSlab= NULL;
	for(auto it = slabGs.begin();it!=slabGs.end();it++)
	{
		MSSlabG* pSlabG = (MSSlabG*)*it;
		if(pSlabG == NULL) continue;
		pSlabG->GetZLevels(vec,dTopLevel,dBotLevel);
		if(dColumnBotLevel > dTopLevel+10 || dColumnBotLevel<dBotLevel-10)
			continue;
		pLowerSlab = pSlabG;
		break;

	}

	return pLowerSlab;
//	MSFloor* pFloor = GetFloor();
//	if(pFloor)
//	{
//		MSFloor* pLowerFloor = pFloor->GetLowerFloor();
//		if(pLowerFloor)
//			pSlab = (MSSlabG*)pLowerFloor->GetMSCompoG(&vec,MSCompoG::msSlabG);
//	}
//	return pSlab;
}


bool MSColumnG::hasBoundaryOffsetBeamMembers()
{
	
	GM2DPolyline ColumnPoly(TRUE);
	this->GetBoundary(ColumnPoly);

	vector<MSCompoG*> BeamGArr = FindConnectMember(FindType_MSElement::msBeamG, 0);
	long NumBeam = BeamGArr.size();

	bool bRval = false;
	map<long, long> UsedBeamMap;
	map<long, long>::iterator it;

	
	for (long i = 0; i < NumBeam; i++)
	{
		MSBeamG* pCurBeam = (MSBeamG*)BeamGArr[i];
		
		//TODO 중간에 기둥이 끼어있을때 ???
		//if (pCurBeam->GetSConn() != this && pCurBeam->GetEConn() != this)
		//	continue;
// 		shared_ptr<GM2DCurve> lLine, rLine;
// 		pCurBeam->GetTrimmedLRLines(lLine, rLine);

		long nCurLVal = ColumnPoly.OverlapEx(pCurBeam->mp_LLineTR);
		long nCurRVal = ColumnPoly.OverlapEx(pCurBeam->mp_RLineTR);

		if (nCurLVal == 0 || nCurRVal == 0/* && nNextLVal > 0 && nNextRVal > 0*/)
			return true;
	}

	return bRval;
}

MSBoundaryCondition::Type_Condition MSColumnG::GetStartBoundaryCondition()
{
	return m_StartCondition.GetCondition();
}

MSBoundaryCondition::Type_Condition MSColumnG::GetEndBoundaryCondition()
{
	return m_EndCondition.GetCondition();
}

void MSColumnG::SetStartBoundaryCondition(MSBoundaryCondition::Type_Condition condition)
{
	m_StartCondition.SetCondition(condition);
}
void MSColumnG::SetEndBoundaryCondition(MSBoundaryCondition::Type_Condition condition)
{
	m_EndCondition.SetCondition(condition);
}

bool MSColumnG::IsCornerPlacing()
{
	if (IsRC())
	{
		MSRcColumnM* pCompoM = (MSRcColumnM*)GetRebarM();
		return pCompoM->GetIsCornerPlacing();
	}
	else if (IsSRC())
	{
		MDSrcColumnM* pCompoM = (MDSrcColumnM*)GetRebarM();
		return pCompoM->GetIsCornerPlacing();
	}

	return false;
}

bool MSColumnG::HasColumnHeaderM()
{
	if (IsRectSecProfile() == false)
		return false;

	if (GetCompoM()->IsSteel())
		return false;

	MSBaseBuilding* pBuilding = GetBuilding();
	MSCompoM* pHeaderM = pBuilding->GetCompoM(GetColumnHeaderMID());
	if(pHeaderM == NULL)
		return false;

	return true;
}

int MSColumnG::GetColumnHeaderM_T1()
{
	if (IsRectSecProfile() == false)
		return 0;

	if (GetCompoM()->IsSteel())
		return 0;

	MSBaseBuilding* pBuilding = GetBuilding();
	MSColumnHeaderM* pHeaderM = dynamic_cast<MSColumnHeaderM*>(pBuilding->GetCompoM(GetColumnHeaderMID()));
	if(pHeaderM == NULL)
		return 0;

	return pHeaderM->GetT1();
}
int MSColumnG::GetColumnHeaderM_T3()
{
	if (IsRectSecProfile() == false)
		return 0;

	if (GetCompoM()->IsSteel())
		return 0;

	MSBaseBuilding* pBuilding = GetBuilding();
	MSColumnHeaderM* pHeaderM = dynamic_cast<MSColumnHeaderM*>(pBuilding->GetCompoM(GetColumnHeaderMID()));
	if(pHeaderM == NULL)
		return 0;

	return pHeaderM->GetT3();
}

int MSColumnG::GetColumnHeaderM_T5()
{
	if (IsRectSecProfile() == false)
		return 0;

	if (GetCompoM()->IsSteel())
		return 0;

	MSBaseBuilding* pBuilding = GetBuilding();
	MSColumnHeaderM* pHeaderM = dynamic_cast<MSColumnHeaderM*>(pBuilding->GetCompoM(GetColumnHeaderMID()));
	if (pHeaderM == NULL)
		return 0;

	return pHeaderM->GetT5();
}

MSCompoM* MSColumnG::GetColumnHeaderM()
{
	if (IsRectSecProfile() == false)
		return nullptr;

	if (GetCompoM()->IsSteel())
		return nullptr;

	MSBaseBuilding* pBuilding = GetBuilding();
	MSColumnHeaderM* pHeaderM = dynamic_cast<MSColumnHeaderM*>(pBuilding->GetCompoM(GetColumnHeaderMID()));
	if(pHeaderM == nullptr)
		return nullptr;

	return pHeaderM;
}

void MSColumnG::SetColumnHeaderM(CString columnHeadName)
{
	if (IsRectSecProfile() == false)
		return;

	if (GetCompoM()->IsSteel())
		return;

	MSRcLineMemberM* pColumnM = (MSRcLineMemberM*)GetCompoM();

	if (columnHeadName == _T("None"))
	{
		pColumnM->SetColumnHeaderMID(0);
		return;
	}
		
	MSCompoM* pCompoM = pColumnM->mp_Owner->GetCompoM(columnHeadName, MSCompoM::msColumnHeaderM);
	if (pCompoM != NULL)
		pColumnM->SetColumnHeaderMID(pCompoM->m_ID);
}

double MSColumnG::GetColumnHeaderOffset()
{
	return m_dColumnHeaderOffset;
}

void MSColumnG::SetColumnHeaderOffset(double value)
{
	if (m_EOffsetZ < value)
		return;

	if (value < 0)
	{
		double dHeight = -GetHeight();
		int t1 = GetColumnHeaderM_T1(); // 주두부 높이
		if (value < dHeight + t1)
			value = dHeight + t1;
	}
	m_dColumnHeaderOffset = value;
}

double MSColumnG::GetColumnHeaderSubOffset()
{
	return m_dColumnHeaderSubOffset;
}

void MSColumnG::SetColumnHeaderSubOffset(double value)
{
	// [Q24-545] 양수 지원 안함
	if (value > 0)
		return;
	if (value < 0)
	{
		double dHeight = -GetHeight();
		int t5 = GetColumnHeaderM_T5();
		if (value < dHeight + t5)
			value = dHeight + t5;
	}
	m_dColumnHeaderSubOffset = value;
}

bool MSColumnG::IsRectSecProfile()
{
	MSRcLineMemberM* pColumnM = dynamic_cast<MSRcLineMemberM*>(GetCompoM());
	if (pColumnM == nullptr)
		return false;

	if (pColumnM->GetShape() == MSRcLineMemberM::RECT)
		return true;

	return false;
}

bool MSColumnG::IsCircleSecProfile()
{
	MSRcLineMemberM* pColumnM = dynamic_cast<MSRcLineMemberM*>(GetCompoM());
	if (pColumnM == nullptr)
		return false;

	if (pColumnM->GetShape() == MSRcLineMemberM::CIRCLE)
		return true;

	return false;
}
long MSColumnG::GetLocationCalculationType()
{
	return m_nLocationCalculationType;
}
void MSColumnG::SetLocationCalculationType(long nType)
{
	m_nLocationCalculationType = nType;
}
long MSColumnG::GetLocationCalculationResult()
{
	return m_nLocationCalculationResult;
}
void MSColumnG::SetLocationCalculationResult(long nType)
{
	m_nLocationCalculationResult = nType;
}