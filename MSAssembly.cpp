#include "StdAfx.h"
#include "MSAssembly.h"

#include "MSRebarBuilding.h"
#include "MSFloorQT.h"
#include "MSCompoG.h"
#include "MSLinkMemberG.h"
#include "MSColumnG.h"
#include "MSPitG.h"
#include "MSIsoFootG.h"
#include "MSLevelZone.h"
#include "MSSectionView.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSAssembly, MSContainer, VERSIONABLE_SCHEMA | 1)

MSAssembly::MSAssembly()
{
	Init(true);
}


MSAssembly::~MSAssembly(void)
{
	DeleteLevelZones(); // Floor에서 먼저 삭제 해줘야함 LevelZone 소멸자에서 Floor 사용함 2번 호출 하나 괜찬음
	DeleteCompoGs();
}

void MSAssembly::DeleteCompoGs()
{
	map<long, MSCompoG*>::iterator it;
	for (it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
		delete it->second;
	mm_CompoG.clear();
}

void MSAssembly::DeleteLevelZones()
{
	vector<MSLevelZone*>::iterator it;
	for(it = ma_LevelZone.begin(); it != ma_LevelZone.end(); it++)
		delete *it;
	ma_LevelZone.clear();
}
void MSAssembly::Serialize( CArchive& ar)
{
	MSContainer::Serialize(ar);

	if(ar.IsStoring())
	{
		m_SiteRect.Serialize(ar);

		int LevelZoneCount = ma_LevelZone.size();
		ar << LevelZoneCount;

	    vector<MSLevelZone*>::iterator itZone;
		for(itZone = ma_LevelZone.begin(); itZone != ma_LevelZone.end(); itZone++)
		{
			MSLevelZone* pZone = *itZone;
			pZone->Serialize(ar);
		}
		int count = ma_SectionView.size();
		ar << count;
		for (int i = 0; i < ma_SectionView.size(); ++i)
			ma_SectionView[i]->Serialize(ar);
	}
	else
	{
		m_SiteRect.Serialize(ar);

		int LevelZoneCount = 0;
		ar >> LevelZoneCount;
		for( int iZone = 0; iZone < LevelZoneCount; iZone++)
		{
			MSLevelZone* pZone = new MSLevelZone;
			pZone->Serialize(ar);
			pZone->SetOwner(this);
			// 내부 동작은 Building Serializing 끝난 후에 처리 (MSBuilding::AddLevelZoneAfterSerialize)
			MSAssembly::Add(pZone); // 이 함수에서는 ma_LevelZone 에 단순 Add 만 처리함
			// ma_LevelZone.push_back(pZone);
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20150708)
		{
			int count = 0;
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				MSSectionView* view = new MSSectionView();
				view->Serialize(ar);
				view->SetOwner(this);
				ma_SectionView.push_back(view);
			}
		}
	}

	// 레벨존에 포함된 Pit 의 이전버전 처리
	if (!ar.IsStoring() && MSVersionInfo::GetCurrentVersion() < 20150714)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == MSElement::msPitG)
			{
				MSPitG* pit = (MSPitG*)(itr->second);
				double offsetByLevelZone = pit->GetOffsetByContainingLevelZone();
				pit->m_dOffset -= offsetByLevelZone;
			}
		}
	}
}

MSObject* MSAssembly::Copy( bool bNewID /*= true*/ )
{
	MSAssembly* pNewObj = new MSAssembly();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSAssembly::CopyFromMe(MSAssembly* pSource, bool bNewID)
{

}

void MSAssembly::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSContainer::Init(bInConstructor);
	mp_Building = NULL;
}

/*
long MSAssembly::InsertNewCompoG( MSCompoG* pCompoG )
{
	MergeJoints(pCompoG);
	mp_QT->AddToLeaves(pCompoG);
	Add(pCompoG,true);
	return 0;
}

long MSAssembly::DeleteCompoG( MSCompoG* pCompoG )
{

}
*/

void MSAssembly::Add( MSCompoG* pCompoG, bool bChangeOwner /*= false*/, bool bUpdateTrimmedBoundary /*= true*/ )
{
	MSContainer::Add(pCompoG, bChangeOwner);
}

bool MSAssembly::Remove( MSCompoG* pCompoG )
{
	long nID = pCompoG->m_ID;
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
	if(it == mm_CompoG.end())	return false;
	mm_CompoG.erase(it);
	return true;
}

bool MSAssembly::Delete( MSCompoG* pCompoG )
{
	if(!Remove(pCompoG))
		return false;
	delete pCompoG;
	return true;
}

/*
long MSAssembly::Modify( MSCompoG* pCompoG )
{
	long nID = pCompoG->m_ID;
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
	if(it == mm_CompoG.end())	return 0;

	if(ms_bCloneMode)  // Source 층일 경우
	{
		if(!mp_Source)
		{
			MSCompoG* pCloneMem;
			long nClone = pCompoG->ma_Clone.size();
			for(long iClone=0; iClone<nClone; iClone++)
			{
				pCloneMem = pCompoG->ma_Clone[iClone];
				pCloneMem = (MSCompoG*)pCompoG->Copy();

				pCloneMem->mp_Owner->Remove(pCloneMem);
			}
		}
	}

	delete pCompoG;
	mm_CompoG.erase(it);
	return nID;
}*/

void MSAssembly::Add( MSLevelZone* pZone, bool isDoit /*= true*/ )
{
	ma_LevelZone.push_back(pZone);
}

bool MSAssembly::Remove( MSLevelZone* pRemoveZone )
{
	long nZone = ma_LevelZone.size();
	for(long iZone=0; iZone<nZone; iZone++)
	{
		MSLevelZone* pZone = ma_LevelZone[iZone];
		if(pZone==pRemoveZone)
		{
			ma_LevelZone.erase(ma_LevelZone.begin()+iZone);
			return true;
		}
	}
	return false;
}

void MSAssembly::Add(MSSectionView* pZone, bool isDoit /*= true*/)
{
	ma_SectionView.push_back(pZone);
}

bool MSAssembly::Remove(MSSectionView* pRemoveZone)
{
	long nZone = ma_SectionView.size();
	for (long iZone = 0; iZone<nZone; iZone++)
	{
		MSSectionView* pZone = ma_SectionView[iZone];
		if (pZone == pRemoveZone)
		{
			ma_SectionView.erase(ma_SectionView.begin() + iZone);
			return true;
		}
	}
	return false;
}

/*
bool MSAssembly::Delete( MSLevelZone* pRemoveZone )
{
	if(Remove(pRemoveZone) == false)
		return false;
	delete pRemoveZone;
	return true;
}
*/

//void MSAssembly::Add( MSOpening* pOpening )
//{
//}
//
//bool MSAssembly::Remove( MSOpening* pOpening )
//{
//    return false;
//}


// -1 : 보-벽 조합으로 레벨 겹침.
// 0 : 레벨 불일치
// 1 : 한 부재 하단에 한 부재 상단이 접함
// 2 : 상호 어긋나게 교차
// 3 : my가 ur에 포함됨
// 4 : my가 ur를 포함함
// 5 : 완전 일치
long MSAssembly::LevelOverlap(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink)
{
	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	pMyLink->GetZLevels(MyTopLevel, MyBotLevel);
	pUrLink->GetZLevels(UrTopLevel, UrBotLevel);

	// 벽-보 false
	if((pMyLink->IsLineSupportMember() && pUrLink->IsWallMember() ||
		(pMyLink->IsWallMember() && pUrLink->IsLineSupportMember())))
	{
		if(MyTopLevel <= UrBotLevel+DTOL1)
			return 0;
		if(UrTopLevel <= MyBotLevel+DTOL1)
			return 0;
		return -1;
	}

	if(fabs(MyTopLevel-UrTopLevel) < DTOL1 && fabs(MyBotLevel-UrBotLevel) < DTOL1)
		return 5;
	if(MyTopLevel >= UrTopLevel-DTOL1 && MyBotLevel <= UrBotLevel+DTOL1)
		return 4;
	if(MyTopLevel <= UrTopLevel+DTOL1 && MyBotLevel >= UrBotLevel-DTOL1)
		return 3;

	if(fabs(MyTopLevel-UrBotLevel)<DTOL1)
		return 1;
	if(fabs(MyBotLevel-UrTopLevel)<DTOL1)
		return 1;

	if(MyTopLevel <= UrBotLevel+DTOL1)
		return 0;
	if(UrTopLevel <= MyBotLevel+DTOL1)
		return 0;

	return 2;
}

void MSAssembly::AddCrossMemberError(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrMem, XErrorObject* pMsgObject, CString strGroup, CString& strMsg)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	if(pMsgObject)
	{
		pMsgObject->SetGroupName(strGroup);
		pMsgObject->m_Message.Add(strMsg);
		pMsgObject->AddCompoGID(pMyLink->m_ID);
		pMsgObject->AddCompoGID(pUrMem->m_ID);
		pMsgObject->SetShowSelectCompoG(TRUE);
		pMessageContainer->AddErrorObject(pMsgObject);
	}
}

void MSAssembly::Dump( EFS::EFS_Buffer& buffer )
{
	MSContainer::Dump(buffer);
	buffer << &m_SiteRect;
}

void MSAssembly::Recovery( EFS::EFS_Buffer& buffer )
{
	MSContainer::Recovery(buffer);
	buffer >> &m_SiteRect;
}

vector<MSLevelZone*> MSAssembly::GetLevelZone( double Xc, double Yc )
{
	vector<MSLevelZone*> LevelZoneArr;
	vector<MSLevelZone*>::iterator it;
	for(it = ma_LevelZone.begin();it!=ma_LevelZone.end();it++)
	{
		MSLevelZone* pLevelZone = *it;
		if(pLevelZone == NULL)	continue;
		if(pLevelZone->Contains(Xc,Yc,1)<=0)//포함
			LevelZoneArr.push_back(pLevelZone);
	}

	return LevelZoneArr;
}

vector<MSLevelZone*> MSAssembly::GetLevelZone(GM2DCurve* pCurve )
{
	vector<MSLevelZone*> LevelZoneArr;
	vector<MSLevelZone*>::iterator it;
	for(it = ma_LevelZone.begin();it!=ma_LevelZone.end();it++)
	{
		MSLevelZone* pLevelZone = *it;
		if(pLevelZone == NULL)	continue;

		GM2DVector TheVec = pCurve->PointAtParam(0.5);
		if(pLevelZone->Contains(TheVec.m_X, TheVec.m_Y, 1) <= 0)
			LevelZoneArr.push_back(pLevelZone);
		else if(pLevelZone->OverLap(pCurve,1))
			LevelZoneArr.push_back(pLevelZone);
	}

	return LevelZoneArr;
}

vector<MSLevelZone*> MSAssembly::GetLevelZonebyCompoG(MSCompoG* pCompoG)
{
	vector<MSLevelZone*> LevelZoneArr;

	if(pCompoG->IsPointMember())
	{
		MSPointMemberG* pPointG = (MSPointMemberG*)pCompoG;
		GM2DVector* pSVec = pPointG->GetSVec();
		return GetLevelZone(pSVec->m_X, pSVec->m_Y);
	}
	else if(pCompoG->IsLinkMember())
	{
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
		shared_ptr<GM2DCurve> pCurve = pLinkG->GetCenCurve();
		GM2DLineVector TheLine;

		return GetLevelZone(&*pCurve);
	}
	else
	{
		GM2DPolyline* pPoly = pCompoG->GetProfile();
		return GetLevelZone(pPoly);
	}

	return LevelZoneArr;
}

double MSAssembly::GetLevel4LevelZone( double Xc, double Yc, bool isMax)
{
	vector<MSLevelZone*> LevelZoneArr = GetLevelZone(Xc,Yc);
	long NumLevelZone = LevelZoneArr.size();
	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		for(long jZone = iZone+1 ; jZone < NumLevelZone ; jZone++)
		{
			MSLevelZone* pLevelZonei = LevelZoneArr[iZone];
			MSLevelZone* pLevelZonej = LevelZoneArr[jZone];
			if(isMax)
			{
				if(pLevelZonei->m_Level > pLevelZonej->m_Level)
				{
					LevelZoneArr[iZone] = pLevelZonej;
					LevelZoneArr[jZone] = pLevelZonei;
				}
			}
			else
			{
				if(pLevelZonei->m_Level < pLevelZonej->m_Level)
				{
					LevelZoneArr[iZone] = pLevelZonej;
					LevelZoneArr[jZone] = pLevelZonei;
				}
			}
		}
	}
	
	if(NumLevelZone > 0)
		return LevelZoneArr[0]->m_Level;

	return 0;
}

double MSAssembly::GetLevel4LevelZone(GM2DCurve* pCurve, bool isMax)
{
	vector<MSLevelZone*> LevelZoneArr = GetLevelZone(pCurve);
	long NumLevelZone = LevelZoneArr.size();
	if(NumLevelZone == 0)
		return 0;

	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		for(long jZone = iZone+1 ; jZone < NumLevelZone ; jZone++)
		{
			MSLevelZone* pLevelZonei = LevelZoneArr[iZone];
			MSLevelZone* pLevelZonej = LevelZoneArr[jZone];
			if(isMax)
			{
				if(pLevelZonei->m_Level > pLevelZonej->m_Level)
				{
					LevelZoneArr[iZone] = pLevelZonej;
					LevelZoneArr[jZone] = pLevelZonei;
				}
			}
			else
			{
				if(pLevelZonei->m_Level < pLevelZonej->m_Level)
				{
					LevelZoneArr[iZone] = pLevelZonej;
					LevelZoneArr[jZone] = pLevelZonei;
				}
			}
		}
	}

	if(!isMax)
	{
		if(LevelZoneArr[0]->m_Level > 0)
			return 0;
		else
			return LevelZoneArr[0]->m_Level;
	}

	return LevelZoneArr[0]->m_Level;
}

double MSAssembly::GetLevel4LevelZone(MSLinkMemberG* pLinkG, bool isMax )
{
	GM2DVector CVec = pLinkG->GetCenCurve()->PointAtParam(0.5);

	return GetLevel4LevelZone(CVec.m_X, CVec.m_Y);

// 	long NumDif = pLinkG->ma_MemberLevelDif.GetSize();
// 	if(NumDif == 0)
// 	{
// 		long nZone = ma_LevelZone.size();
// 		for(long iZone=0; iZone<nZone; iZone++)
// 		{
// 			MSLevelZone* pZone = ma_LevelZone[iZone];
// 			long nLSVal = 1;
// 			long nLEVal = 1;
// 			long nRSVal = 1;
// 			long nREVal = 1;
// 			if(pLinkG->mp_LLine != NULL)
// 			{
// 				nLSVal = pZone->Contains(pLinkG->mp_LLine->m_P0.m_X, pLinkG->mp_LLine->m_P0.m_Y, 1);
// 				nLEVal = pZone->Contains(pLinkG->mp_LLine->m_P1.m_X, pLinkG->mp_LLine->m_P1.m_Y, 1);
// 			}
// 			if(pLinkG->mp_RLine != NULL)
// 			{
// 				nRSVal = pZone->Contains(pLinkG->mp_RLine->m_P0.m_X, pLinkG->mp_RLine->m_P0.m_Y, 1);
// 				nREVal = pZone->Contains(pLinkG->mp_RLine->m_P1.m_X, pLinkG->mp_RLine->m_P1.m_Y, 1);
// 			}
// 			if(nLSVal < 1 && nLEVal < 1 && nRSVal < 1 && nREVal < 1)
// 				return pZone->m_Level;
// 		}
// 
// 		return 0;
// 	}
// 	else
// 		return GetLevel4LevelZone(pCurve, isMax);
}

double MSAssembly::GetLevel4LevelZone(MSPointMemberG* pPointG)
{
	GM2DVector* pVec = pPointG->GetSVec();
	double dZLevel = GetLevel4LevelZone(pVec->m_X, pVec->m_Y);
	return dZLevel;
}

vector<MSLevelZone*> MSAssembly::GetLevelZone(GM2DPolyline* pPolyline)
{
	vector<MSLevelZone*> LevelZoneArr;
	long nCnt = 1;
	if(pPolyline->IsClosed())
		nCnt++;
	long NumVec = pPolyline->GetSize() - nCnt;

	map<long, MSLevelZone*> LevelZoneMap;

	for(long iVec = 0 ; iVec < NumVec ; iVec++)
	{
		GM2DVector* pVec = pPolyline->ma_Point[iVec];
		vector<MSLevelZone*> LevelZoneArr = GetLevelZone(pVec->m_X, pVec->m_Y);
		for(long iLevelZone = 0 ; iLevelZone < LevelZoneArr.size() ;iLevelZone++)
		{
			MSLevelZone* pLevelZone = LevelZoneArr[iLevelZone];
			if(pLevelZone != NULL)
			{
				map<long, MSLevelZone*>::iterator it = LevelZoneMap.find(pLevelZone->m_ID);
				if(it == LevelZoneMap.end())
				{
					LevelZoneArr.push_back(pLevelZone);
					LevelZoneMap.insert(make_pair(pLevelZone->m_ID, pLevelZone));
				}
			}
		}
	}

	return LevelZoneArr;
}