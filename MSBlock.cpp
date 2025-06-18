#include "StdAfx.h"
#include "MSBlock.h"

#include "MSRebarBuilding.h"
#include "MSLevelZone.h"
#include "MSCompoG.h"
#include "MSBlockInstance.h"
#include "MSPolylineObject.h"
#include "MSRcLineMemberM.h"
#include "MSRcHandRailM.h"
#include "MSRcFaceMemberM.h"
#include "MSStairM.h"
#include "MSWindowM.h"
#include "MSDoorM.h"
#include "MSRcIsoFootM.h"
#include "MSRcWallFoot.h"
#include "MSWell.h"
#include "MSTrench.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSBlock, MSFloor, VERSIONABLE_SCHEMA | 1)

MSBlock::MSBlock(void)
{
	this->mp_PolylineInfo = new MSPolylineObject();
	m_dHeight = 2800;
	m_RefFloorNum = 0;
}


MSBlock::~MSBlock(void)
{
	DeleteBlockInstances();
}

void MSBlock::DeleteBlockInstances()
{
	int NumInstance = ma_Instance.size();
	for( int iInstance = 0; iInstance < NumInstance; iInstance++)
		delete ma_Instance[iInstance];
	ma_Instance.clear();
}

void MSBlock::Dump( EFS::EFS_Buffer& buffer )
{
	MSFloor::Dump(buffer);
	buffer << m_RefFloorNum;
}

void MSBlock::Recovery( EFS::EFS_Buffer& buffer )
{
	MSFloor::Recovery(buffer);
	buffer >> m_RefFloorNum;
}

MSObject* MSBlock::Copy(bool bNewID /*= true*/)
{
	MSBlock* pNewObj = new MSBlock();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBlock::CopyFromMe(MSBlock* pSource, bool bNewID/*=true*/)
{
	if (pSource == NULL) return;
	m_RefFloorNum = pSource->m_RefFloorNum;

	MSFloor::CopyFromMe(pSource, bNewID);
}
void MSBlock::Serialize(CArchive &ar)
{
	MSFloor::Serialize(ar);
	if (ar.IsStoring())
	{
		mp_PolylineInfo->Serialize(ar);
		ar << m_RefFloorNum;
	}
	else
	{
		mp_PolylineInfo->Serialize(ar);
		if(MSVersionInfo::GetCurrentVersion() >= 20140813)
			ar >> m_RefFloorNum;
	}
}

MSBlockInstance* MSBlock::MakeInstance( GM2DVector& FirstPoint, GM2DVector& SecondPoint, bool bReverse )
{
	MSBlockInstance* pInstance = new MSBlockInstance;
	GM2DPolyline* pProfile = GetProfile();
	if (pProfile == nullptr)
		return pInstance;

	if(pProfile->GetSize() == 0)
		pProfile->AddLine(GM2DLineVector(0,0,10000,0));
	GM2DLineVector BlockFirstLine = pProfile->GetLine(0);
	GM2DLineVector InstanceFirstLine(FirstPoint, SecondPoint);
	double AxisRotRadian = (InstanceFirstLine.GetDegree() - BlockFirstLine.GetDegree())*Deg2Rad;
	GM2DVector DeltaOfBasePoint = FirstPoint - *pProfile->ma_Point[0];

	MSLevelZone* pNewZone;
	long nZone = ma_LevelZone.size();
	for (long iZone = 0; iZone < nZone; iZone++)
	{
		MSLevelZone* pZone = ma_LevelZone[iZone];
		pNewZone = (MSLevelZone*)pZone->Copy();
		// 		pNewZone->mp_Source = pZone;
		// 		pNewZone->mp_Owner = this;
		// 		pZone->ma_Clone.push_back(pNewZone);
		pInstance->ma_LevelZone.push_back(pNewZone);

		GM2DPolyline* pProfile = pNewZone->GetProfile();
		pProfile->Translate(DeltaOfBasePoint);
		pProfile->RotateZ(AxisRotRadian, &FirstPoint);
		if(bReverse)
			pProfile->MirrorByAxis(InstanceFirstLine);
	}

	MSCompoG* pNewMem;
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = mm_CompoG.begin(); itMem != mm_CompoG.end(); itMem++)
	{
		MSCompoG* pMem = itMem->second;
		pNewMem = (MSCompoG*)pMem->Copy();
		pInstance->ma_CompoG.push_back(pNewMem);

		pNewMem->Translate(DeltaOfBasePoint);
		pNewMem->RotateZ(AxisRotRadian, &FirstPoint);
		if(bReverse)
			pNewMem->MirrorByAxis(InstanceFirstLine);
	}

	MSSlabRebar* pNewSlabRebar;
	map<long, MSSlabRebar*>::iterator itSlabRebar;
	for (itSlabRebar = mm_Rebar.begin(); itSlabRebar!= mm_Rebar.end(); itSlabRebar++)
	{
		MSSlabRebar* pMem = itSlabRebar->second;
		pNewSlabRebar= (MSSlabRebar*)pMem->Copy();
		pInstance->ma_SlabRebar.push_back(pNewSlabRebar);

		pNewSlabRebar->Translate(DeltaOfBasePoint);
		pNewSlabRebar->RotateZ(AxisRotRadian, &FirstPoint);
		if(bReverse)
			pNewSlabRebar->MirrorByAxis(InstanceFirstLine);
	}

	return pInstance;
}

bool MSBlock::ExportBlock(CString fileName)
{
	CFile file;
	CFileException e;
	if(file.Open(fileName, CFile::modeCreate | CFile::modeWrite, &e)==FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);
	Serialize_Block(ar);
	return true;
}

bool MSBlock::ImportBlock(CString fileName)
{
	CFile file;
	CFileException e;
	if(file.Open(fileName, CFile::modeRead, &e)==FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	if(file.GetLength() <= 0)	return false;
	CArchive ar(&file, CArchive::load);
	Serialize_Block(ar);

	return true;
}

void MSBlock::Init_ImportData()
{
	ma_ImportCompoG.clear();
	ma_ImportCompoM.clear();
	ma_ImportLevelZone.clear();
	ma_ImportFloorRebar.clear();
}

void MSBlock::Serialize_Block(CArchive& ar)
{
	Init_ImportData();
	if(ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		mp_PolylineInfo->Serialize(ar);
		ar << m_Name;
		int NumCompoG = mm_CompoG.size();
		ar << NumCompoG;
		map<long, MSCompoG*>::iterator itCompoG;
		for( itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
		{
			MSCompoG* pCompoG = itCompoG->second;
			int GType = pCompoG->GetType();
			ar << GType;
			pCompoG->Serialize(ar);

			bool HasOpening = false;
			if(GType == MSCompoG::msWallG)
			{
				HasOpening = true;
				ar << HasOpening;
				MSWallMemberG* pWallG = (MSWallMemberG*)pCompoG;
				int OpeningCount = pWallG->mm_WallOpening.size();
				ar << OpeningCount;
				map<long, MSWallOpeningG*>::iterator itOpening;
				for(itOpening = pWallG->mm_WallOpening.begin(); itOpening != pWallG->mm_WallOpening.end(); itOpening++)
				{
					MSWallOpeningG* pOpening = itOpening->second;
					int OpeningMType = pOpening->mp_CompoM->GetType();
					ar << pOpening->m_ID;
					ar << OpeningMType;
					Serialize_Block_CompoM(ar, pOpening->mp_CompoM);
				}
			}
			else if(GType == MSCompoG::msSlabG)
			{
				HasOpening = true;
				ar << HasOpening;
				MSSlabG* pSlabG = (MSSlabG*)pCompoG;
				int OpeningCount = pSlabG->mp_PolylineInfo->ma_Opening.size();
				ar << OpeningCount;
				for(int iOpening = 0; iOpening < OpeningCount; iOpening++)
				{
					MSOpening* pOpening = pSlabG->mp_PolylineInfo->ma_Opening[iOpening];
					pOpening->Serialize(ar);
				}

				int WellCount = pSlabG->mm_Well.size();
				ar << WellCount;
				map<long, MSWell*>::iterator itWell;
				for(itWell = pSlabG->mm_Well.begin(); itWell != pSlabG->mm_Well.end(); itWell++)
				{
					MSWell* pWell = itWell->second;
					pWell->Serialize(ar);
				}

				int TrenchCount = pSlabG->mm_Trench.size();
				ar << TrenchCount;
				map<long, MSTrench*>::iterator itTrench;
				for(itTrench = pSlabG->mm_Trench.begin(); itTrench != pSlabG->mm_Trench.end(); itTrench++)
				{
					MSTrench* pTrench = itTrench->second;
					pTrench->Serialize(ar);
				}
			}
			else
				ar << HasOpening;

			int MType = pCompoG->mp_CompoM->GetType();
			ar << MType;
			Serialize_Block_CompoM(ar, pCompoG->mp_CompoM);
		}

		int LevelZoneCount = ma_LevelZone.size();
		ar << LevelZoneCount;

		vector<MSLevelZone*>::iterator itZone;
		for(itZone = ma_LevelZone.begin(); itZone != ma_LevelZone.end(); itZone++)
		{
			MSLevelZone* pZone = *itZone;
			pZone->Serialize(ar);
		}

		int RebarCount = mm_Rebar.size();
		ar << RebarCount;
		map<long, MSSlabRebar*>::iterator itRebar;
		for(itRebar = mm_Rebar.begin(); itRebar != mm_Rebar.end(); itRebar++)
		{
			MSSlabRebar* pRebar = itRebar->second;
			pRebar->Serialize(ar);
		}
	}
	else
	{
		int version;
		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);

		mp_PolylineInfo->Serialize(ar);
		ar >> m_Name;
		int NumCompoG = 0;
		ar >> NumCompoG;
		for(int iCompoG =  0; iCompoG < NumCompoG; iCompoG++)
		{
			int GType = 0;
			ar >> GType;
			MSCompoG* pCompoG = mp_Building->CreateNewCompoG(GType);
			pCompoG->SetOwner(this);
			pCompoG->Serialize(ar);
			if(pCompoG->mp_CompoM && pCompoG->mp_CompoM->m_ID==0)
				delete pCompoG->mp_CompoM;

			bool HasOpening = false;
			ar >> HasOpening;

			if(HasOpening)
			{
				if(GType == MSCompoG::msWallG)
				{
					MSWallMemberG* pWallG = (MSWallMemberG*)pCompoG;
					int OpeningCount = 0;
					ar >> OpeningCount; 
					if(OpeningCount != pWallG->mm_WallOpening.size())
					{
						ASSERT(0);
					}
					for(int iOpening = 0; iOpening < OpeningCount; iOpening++)
					{
						long OpeningID = 0;
						int OpeningMType = 0;
						ar >> OpeningID;
						ar >> OpeningMType;
						map<long, MSWallOpeningG*>::iterator itOpening = pWallG->mm_WallOpening.find(OpeningID);
						if(itOpening != pWallG->mm_WallOpening.end())
						{
							MSWallOpeningG* pOpening = itOpening->second;
							MSCompoM* pCompoM = mp_Building->CreateNewCompoM(OpeningMType);
							pOpening->mp_CompoM = Serialize_Block_CompoM(ar, pCompoM);
						}
					}
					map<long, MSWallOpeningG*> oldIDtoWallOpeningMap;
					oldIDtoWallOpeningMap.insert(pWallG->mm_WallOpening.begin(), pWallG->mm_WallOpening.end());
					pWallG->mm_WallOpening.clear();
					map<long, MSWallOpeningG*>::iterator itOpening;
					for(itOpening = oldIDtoWallOpeningMap.begin(); itOpening != oldIDtoWallOpeningMap.end(); itOpening++)
					{
						MSWallOpeningG* pOpening = itOpening->second;
						pOpening->m_ID = 0;
						pWallG->mm_WallOpening.insert(make_pair(pOpening->GetID(), pOpening));
					}
				}
				else if(GType == MSCompoG::msSlabG)
				{
					MSSlabG* pSlabG = (MSSlabG*)pCompoG;
					int OpeningCount = 0;
					ar >> OpeningCount; 
					if(OpeningCount != pSlabG->mp_PolylineInfo->ma_Opening.size())
					{
						ASSERT(0);
					}
					pSlabG->mp_PolylineInfo->DeleteOpenings();
					for(int iOpening = 0; iOpening < OpeningCount; iOpening++)
					{
						MSOpening* pOpening = new MSOpening();
						pOpening->Serialize(ar);
						pOpening->m_ID = 0;
						pOpening->GetID();
						pSlabG->mp_PolylineInfo->ma_Opening.push_back(pOpening);
						pOpening->SetOwner(pSlabG->mp_PolylineInfo);
					}

					if(MSVersionInfo::GetCurrentVersion() >= 20140627)
					{
						int WellCount = 0;
						ar >> WellCount;
						if(WellCount != pSlabG->mm_Well.size())
						{
							ASSERT(0);
						}
						pSlabG->DeleteWells();
						for(int iWell = 0; iWell < WellCount; iWell++)
						{
							MSWell* pWell = new MSWell();
							pWell->SetOwner(pSlabG);
							pWell->Serialize(ar);
							pWell->m_ID = 0;
							pWell->GetID();
							pSlabG->mm_Well.insert(make_pair(pWell->m_ID, pWell));
						}

						int TrenchCount = 0;
						ar >> TrenchCount;
						if(TrenchCount != pSlabG->mm_Trench.size())
						{
							ASSERT(0);
						}
						pSlabG->DeleteTrenchs();
						for(int iTrench = 0; iTrench < TrenchCount; iTrench++)
						{
							MSTrench* pTrench = new MSTrench();
							pTrench->SetOwner(pSlabG);
							pTrench->Serialize(ar);
							pTrench->m_ID = 0;
							pTrench->GetID();
							pSlabG->mm_Trench.insert(make_pair(pTrench->m_ID, pTrench));
						}
					}
				}
			}
			int MType = 0;
			ar >> MType;
			MSCompoM* pCompoM = mp_Building->CreateNewCompoM(MType);
			pCompoG->mp_CompoM = Serialize_Block_CompoM(ar, pCompoM);

			pCompoG->m_ID = 0;
			//			ma_ImportCompoG.push_back(pCompoG);
			pCompoG->GetID();
			Add(pCompoG);
		}

		int LevelZoneCount = 0;
		ar >> LevelZoneCount;

		for(int iZone = 0; iZone < LevelZoneCount; iZone++)
		{
			MSLevelZone* pZone = new MSLevelZone();
			pZone->SetOwner(this);
			pZone->Serialize(ar);

			pZone->m_ID = 0;
			//			ma_ImportLevelZone.push_back(pZone);
			pZone->GetID();
			Add(pZone);
		}

		int RebarCount = 0;
		ar >> RebarCount;
		for(int iRebar = 0; iRebar < RebarCount; iRebar++)
		{
			MSSlabRebar* pRebar = new MSSlabRebar();
			pRebar->Serialize(ar);
			pRebar->m_ID = 0;
			//			ma_ImportFloorRebar.push_back(pRebar);
			pRebar->GetID();
			Add(pRebar);
		}

		MSVersionInfo::PopCurrentVersion();
	}
}

MSCompoM* MSBlock::Serialize_Block_CompoM(CArchive& ar, MSCompoM* pCompoM)
{
	if(ar.IsStoring())
	{
		ar << pCompoM->m_ID;
		ar << pCompoM->m_Name;
		ar << pCompoM->GetMaterial()->m_Name;
		int MType = pCompoM->GetType();

		switch(MType)
		{
		case MSCompoM::msBeamM:
		case MSCompoM::msBraceM:
		case MSCompoM::msColumnM:
		case MSCompoM::msButtressM:
		case MSCompoM::msWallFootM:
		case MSCompoM::msLevelDifM:
			{
				MSRcLineMemberM* pLineM = (MSRcLineMemberM*)pCompoM;
				ar << pLineM->GetWidth();
				ar << pLineM->GetDepth();
				int Shape = pLineM->GetShape();
				ar << Shape;
				break;
			}
		case MSCompoM::msHandRailM:
			{
				MSRcHandRailM* pHandM = (MSRcHandRailM*)pCompoM;
				ar << pHandM->GetWidth();
				ar << pHandM->GetDepth();
				ar << pHandM->m_nHandRailType;//0:상부벽만 있음, 1:하부벽도 있음.
				ar << pHandM->m_dTopDepth;//상부벽 높이
				ar << pHandM->m_dBotDepth;//하부벽 높이

				ar << pHandM->m_nParapetType;//0:없음, 1:Defalt parapet

				ar << pHandM->m_dParapetLengthA;
				ar << pHandM->m_dParapetLengthB;
				ar << pHandM->m_dParapetLengthC;
				ar << pHandM->m_dParapetLengthD;
				break;
			}
			//face
		case MSCompoM::msRampM:
		case MSCompoM::msBWallM:
		case MSCompoM::msRWallM:
		case MSCompoM::msSlabM:
		case MSCompoM::msPitM:
			{
				MSRcFaceMemberM* pFaceM = (MSRcFaceMemberM*)pCompoM;
				ar << pFaceM->GetThick();
				break;
			}
		case MSCompoM::msStairLandingM:
		case MSCompoM::msStairStepM:
			{
				MSStairM* pStairM = (MSStairM*)pCompoM;
				break;
			}
		case MSCompoM::msIsoFootM:
			{
				MSRcIsoFootM* pIsoM = (MSRcIsoFootM*)pCompoM;
				ar << pIsoM->GetWidth();
				ar << pIsoM->GetDepth();
				ar << pIsoM->GetThick();
				ar << pIsoM->m_Haunch;
				ar << pIsoM->m_nShape;
				break;
			}
		case MSCompoM::msWindowM:
			{
				MSWindowM* pWindowM = (MSWindowM*)pCompoM;
				ar << pWindowM->GetWidth();
				ar << pWindowM->GetHeight();
				break;
			}
		case MSCompoM::msDoorM:
			{
				MSDoorM* pDoorM = (MSDoorM*)pCompoM;
				ar << pDoorM->GetWidth();
				ar << pDoorM->GetHeight();
				break;
			}
		case MSCompoM::msWallOpeningM:
			{
				MSOpeningM* pOpeningM = (MSOpeningM*)pCompoM;
				ar << pOpeningM->GetWidth();
				ar << pOpeningM->GetHeight();
				break;
			}
		}
	}
	else
	{
		long MID = 0;
		CString Name = _T("");
		CString MateName = _T("");
		ar >> MID;
		ar >> Name;
		ar >> MateName;

		pCompoM->m_Name = Name;
		int MType = pCompoM->GetType();
		double dWidth = 0, dDepth = 0, dThick = 0;

		switch(MType)
		{
		case MSCompoM::msBeamM:
		case MSCompoM::msBraceM:
		case MSCompoM::msColumnM:
		case MSCompoM::msButtressM:
		case MSCompoM::msWallFootM:
		case MSCompoM::msLevelDifM:
			{
				MSRcLineMemberM* pLineM = (MSRcLineMemberM*)pCompoM;
				ar >> dWidth;
				ar >> dDepth;
				int Shape = 0;
				ar >> Shape;
				pLineM->SetWidth(dWidth);
				pLineM->SetDepth(dDepth);
				pLineM->SetShape(Shape);
				break;
			}
		case MSCompoM::msHandRailM:
			{
				MSRcHandRailM* pHandM = (MSRcHandRailM*)pCompoM;
				ar >> dWidth;
				ar >> dDepth;
				ar >> pHandM->m_nHandRailType;//0:상부벽만 있음, 1:하부벽도 있음.
				ar >> pHandM->m_dTopDepth;//상부벽 높이
				ar >> pHandM->m_dBotDepth;//하부벽 높이

				ar >> pHandM->m_nParapetType;//0:없음, 1:Defalt parapet

				ar >> pHandM->m_dParapetLengthA;
				ar >> pHandM->m_dParapetLengthB;
				ar >> pHandM->m_dParapetLengthC;
				ar >> pHandM->m_dParapetLengthD;
				pHandM->SetWidth(dWidth);
				pHandM->SetDepth(dDepth);
				break;
			}
			//face
		case MSCompoM::msRampM:
		case MSCompoM::msBWallM:
		case MSCompoM::msRWallM:
		case MSCompoM::msSlabM:
		case MSCompoM::msPitM:
			{
				MSRcFaceMemberM* pFaceM = (MSRcFaceMemberM*)pCompoM;
				ar >> dThick;
				pFaceM->SetThick(dThick);
				break;
			}
		case MSCompoM::msStairLandingM:
		case MSCompoM::msStairStepM:
			{
				MSStairM* pStairM = (MSStairM*)pCompoM;
				break;
			}
		case MSCompoM::msIsoFootM:
			{
				MSRcIsoFootM* pIsoM = (MSRcIsoFootM*)pCompoM;
				ar >> dWidth;
				ar >> dDepth;
				ar >> dThick;
				ar >> pIsoM->m_Haunch;
				ar >> pIsoM->m_nShape;
				pIsoM->SetWidth(dWidth);
				pIsoM->SetDepth(dDepth);
				pIsoM->SetThick(dThick);
				break;
			}
		case MSCompoM::msWindowM:
			{
				MSWindowM* pWindowM = (MSWindowM*)pCompoM;
				ar >> dWidth;
				ar >> dDepth;
				pWindowM->SetWidth(dWidth);
				pWindowM->SetHeight(dDepth);
				break;
			}
		case MSCompoM::msDoorM:
			{
				MSDoorM* pDoorM = (MSDoorM*)pCompoM;
				ar >> dWidth;
				ar >> dDepth;
				pDoorM->SetWidth(dWidth);
				pDoorM->SetHeight(dDepth);
				break;
			}
		case MSCompoM::msWallOpeningM:
			{
				MSOpeningM* pOpeningM = (MSOpeningM*)pCompoM;
				ar >> dWidth;
				ar >> dDepth;
				pOpeningM->SetWidth(dWidth);
				pOpeningM->SetHeight(dDepth);
				break;
			}
		}

		MSCompoM* pExistM = mp_Building->GetCompoM(Name, MType);
		if(pExistM && (pExistM->m_nFromStory <= m_RefFloorNum && m_RefFloorNum <= pExistM->m_nToStory))
		{
			delete pCompoM;
			return pExistM;
		}
		else
		{
			long nFrom = -(mp_Building->GetNumUnderStory()-1);
			long nTo = mp_Building->GetNumAboveStory()-1;
			pCompoM->m_nFromStory = nFrom;
			pCompoM->m_nToStory = nTo;
			MSMaterial* pMate = mp_Building->GetMaterial(MateName);
			if(pMate)
				pCompoM->SetMaterial(pMate);
			else
				pCompoM->SetMaterial(mp_Building->GetDefMaterial());

			if(MID != 0)
			{
				pCompoM->m_ID = 0;
				ma_ImportCompoM.push_back(pCompoM);
			}
		}
	}
	return pCompoM;
}
