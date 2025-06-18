#include "StdAfx.h"
#include "MSCompoGUtil.h"
#include "MSCompoG.h"
#include "MSLinkMemberG.h"
#include "MSSubHaunchG.h"
#include "MSSlabG.h"
#include "MSFloor.h"
#include "MSFloorQT.h"
#include "MSConstructionZone.h"
#include "MSLevelZone.h"
#include "GMLib/GM2DVector.h"
#include "GMLib/GM2DCurve.h"


MSCompoGUtil::MSCompoGUtil()
{

}

MSCompoGUtil::~MSCompoGUtil()
{

}

GM2DCurve* MSCompoGUtil::GetCurveOnProfile(MSCompoG* pCompoG, double x, double y, double x2, double y2)
{
	GM2DPolyline* profile = pCompoG->GetTrimmedBoundary_Acis();
	GM2DVector Vec1(x, y);
	GM2DVector Vec2(x2, y2);
	GM2DCurve* pOnCurve = profile->GetCurve(Vec1, Vec2);
	if (pOnCurve == nullptr)
		return nullptr;

	double dLoc1 = pOnCurve->OrthoBaseLocValue(Vec1);
	double dLoc2 = pOnCurve->OrthoBaseLocValue(Vec2);

	if (pOnCurve->m_ObjType == GT_ARC)
	{
		double dLocToLenDist = DTOL_MODELING / pOnCurve->Length();
		double loc1 = pOnCurve->OrthoBaseLocValue(GM2DVector(x, y));
		double loc2 = pOnCurve->OrthoBaseLocValue(GM2DVector(x2, y2));
		if ((loc1 < 0 - dLocToLenDist || loc1 > 1 + dLocToLenDist) &&
			(loc2 < 0 - dLocToLenDist || loc2 > 1 + dLocToLenDist))
		{
			delete pOnCurve;
			return nullptr;
		}
	}

	if (dLoc1 > dLoc2)
		std::swap(dLoc1, dLoc2);
	GM2DCurve* pAddCurve = pOnCurve->GetSegment(dLoc1, dLoc2);

	delete pOnCurve;
	return pAddCurve;
}

vector<MSSubHaunchG*> MSCompoGUtil::MakeHaunchG(MSCompoG* pCompoG, double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	vector<MSSubHaunchG*> HaunchGArr;
	GM2DCurve* pHaunchCurve = MSCompoGUtil::GetCurveOnProfile(pCompoG, x, y, x2, y2);
	if (pHaunchCurve == nullptr)
		return HaunchGArr;

	vector<GM2DVector> start_list, end_list;
	vector<double> haunch_y_list;
	if (bTopAuto)
		MSCompoGUtil::FindHaunchTopZ_Auto(pCompoG, pHaunchCurve, start_list, end_list, haunch_y_list);
	else
	{
		start_list.push_back(GM2DVector(x, y));
		end_list.push_back(GM2DVector(x2, y2));
		haunch_y_list.push_back(haunchY_Top);
	}

	for (int i = 0; i < start_list.size(); ++i)
	{
		x = start_list[i].m_X; y = start_list[i].m_Y;
		x2 = end_list[i].m_X; y2 = end_list[i].m_Y;
		haunchY_Top = haunch_y_list[i];

		if (haunchY_Top <= -pCompoG->GetThick() || haunchY_Top > 0)
		{
			CString groupName(_T("Haunch"));
			CString msg = LocalDic::GetLocal(_T("상부 오프셋 값이 슬래브 두께를 초과하였습니다. 하단 오프셋 값이 낮은 슬래브에 헌치를 모델링해야 합니다."), _T("CheckData"));
			pCompoG->mp_MsgObject = NULL;
			pCompoG->AddErrorObject(0, -1, groupName, msg);
			continue;
		}

		/////////////////////////////////////////////////////////////////////////////
		// MAKE Center-Line

		MSSubHaunchG* haunch = new MSSubHaunchG();
		haunch->mp_OwnerCompoG = pCompoG;
		haunch->SetOwner(pCompoG->mp_Owner);
		// haunch->SetCenCurve() 함수에서 pAddCurve 주소 그대로 할당한다.
		GM2DCurve* pNewCurve = pHaunchCurve->Copy();
		pNewCurve->SetP0(start_list[i]);
		pNewCurve->SetP1(end_list[i]);
		haunch->SetCenCurve(pNewCurve);
		haunch->m_dHaunchAngle = haunchAngle;
		haunch->m_dHaunchY = haunchY;
		haunch->m_dHaunchY_Top = haunchY_Top;

		/////////////////////////////////////////////////////////////////////////////
		// MAKE LR Line
		haunch->SetLines(); // mylee 20150122
							 /////////////////////////////////////////////////////////////////////////////
							 // Trim
		HaunchGArr.push_back(haunch);
	}

	delete pHaunchCurve;

	return HaunchGArr;
}

vector<MSSubHaunchG*> MSCompoGUtil::MakeHaunchGAllProfile(MSCompoG* pCompoG, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	vector<MSSubHaunchG*> HaunchGArr;
	GM2DPolyline* pPoly = nullptr;
	if (pCompoG->IsLinkMember())
		pPoly = pCompoG->GetTrimmedBoundary();
	else
		pPoly = pCompoG->GetProfile();

	for (int i = 0; i < pPoly->ma_Point.GetSize() - 1; ++i)
	{
		if (pCompoG->GetTrimmedBoundaryVisibility(i) == FALSE)
			continue;
		GM2DVector* p1 = pPoly->ma_Point[i];
		GM2DVector* p2 = pPoly->ma_Point[i + 1];
		vector<MSSubHaunchG*> CurHaunchGArr = MSCompoGUtil::MakeHaunchG(pCompoG, p1->m_X, p1->m_Y, p2->m_X, p2->m_Y, haunchAngle, haunchY, haunchY_Top, bTopAuto);
		HaunchGArr.insert(HaunchGArr.end(), CurHaunchGArr.begin(), CurHaunchGArr.end());
	}
	return HaunchGArr;
}

void MSCompoGUtil::FindHaunchTopZ_Auto(MSCompoG* pCompoG, GM2DCurve* pCurve, vector<GM2DVector>& start_list, vector<GM2DVector>& end_list, vector<double>& haunch_top_y_list)
{
	vector<GM2DCurve*> EdgeArr;
	// 슬래브들 edge들 중 현재 edge에 수직되는 엣지로 나눔
	map<double, double> mapDivPos;

	// 엣지에 포함되는 슬래브 구함
	vector<MSCompoG*> SlabList = MSCompoGUtil::FindCompoG((MSFloor*)pCompoG->mp_Owner, pCurve, msSlabG, true);
	for (int i = 0; i < SlabList.size(); i++)
	{
		MSSlabG* pSlab = (MSSlabG*)SlabList[i];
		pSlab->GetProfile()->GetCurveInPolygon(pCurve, EdgeArr);

		for (int j = 0; j < EdgeArr.size(); j++)
		{
			double dSLoc = pCurve->OrthoBaseLocValue(EdgeArr[j]->m_P0);
			double dELoc = pCurve->OrthoBaseLocValue(EdgeArr[j]->m_P1);

			mapDivPos[dSLoc] = dSLoc;
			mapDivPos[dELoc] = dELoc;
		}
	}

	vector<double> endPos;
	endPos.push_back(0.0);
	for (map<double, double>::iterator itr = mapDivPos.begin(); itr != mapDivPos.end(); ++itr)
		endPos.push_back(itr->first);
	endPos.push_back(1.0);

	for (int i = (int)(endPos.size()) - 1; i >= 0; --i)
	{
		if (endPos[i] < 0.0 || endPos[i] > 1.0)
			endPos.erase(endPos.begin() + i);
	}
	for (int i = (int)(endPos.size()) - 1; i >= 1; --i) // 중복 삭제
	{
		if (fabs(endPos[i] - endPos[i - 1]) < 0.01)
			endPos.erase(endPos.begin() + i);
	}

	start_list.clear();
	end_list.clear();
	haunch_top_y_list.clear();
	for (int i = 1; i < endPos.size(); ++i)
	{
		double pos1 = endPos[i - 1];
		double pos2 = endPos[i];
		double midPos = (pos1 + pos2) / 2.0;
		GM2DVector mid = pCurve->PointAtParam(midPos);
		double cur = 0;
		bool bExist = false;
		for (int k = 0; k < SlabList.size(); ++k)
		{
			if (SlabList[k]->m_ID == pCompoG->m_ID) continue;
			MSSlabG* slab = dynamic_cast<MSSlabG*>(SlabList[k]);
			if (slab == NULL) continue;
			GM2DPolyline* profile = slab->GetProfile();
			if (profile->Contains(&mid, DTOL_MODELING) < 1)
			{
				cur = SlabList[k]->GetThick() + pCompoG->GetBotOffSet() - ((MSSlabG*)SlabList[k])->m_dOffset;
				cur = -cur;
				if (fabs(cur) < 1.e-5) cur = 0;
				bExist = true;
				break;
			}
		}
		//[BH-3036] TopOffset이 0인 경우에도 헌치 생성으로 변경
		//if (bAll && !bExist) continue; // 슬래브에 접하지 않은 엣지는 통과 (유승호)

		start_list.push_back(pCurve->PointAtParam(pos1));
		end_list.push_back(pCurve->PointAtParam(pos2));
		haunch_top_y_list.push_back(cur);
	}
}

void MSCompoGUtil::TrimHaunches(vector<MSSubHaunchG*> Haunches, MSFloor* pFloor)
{
	long NumHaunch = Haunches.size();
	for (long iHaunch = 0; iHaunch < NumHaunch; iHaunch++)
	{
		MSSubHaunchG* pMyHaunch = Haunches[iHaunch];
		vector<MSCompoG*> OverLapCompoGArr;

		pMyHaunch->GetID();
		GM2DPolyline MyPoly(TRUE);
		pMyHaunch->GetBoundary(MyPoly);

		for (long jHaunch = iHaunch + 1; jHaunch < NumHaunch; jHaunch++)
		{
			MSSubHaunchG* pUrHaunchG = Haunches[jHaunch];
			if (pMyHaunch == pUrHaunchG)
				continue;

			CheckModelOpt tmpOpt;
			if (pMyHaunch->IsLevelConnectedOverLap(pUrHaunchG, tmpOpt) == false)
				continue;

			pUrHaunchG->GetID();
			GM2DPolyline UrPoly(TRUE);
			pUrHaunchG->GetBoundary(UrPoly);

			BOOL bOverlaped = MyPoly.OverlapEx(UrPoly, MSFloor::ms_DefLineHitDistTol);
			if (bOverlaped)
				OverLapCompoGArr.push_back(pUrHaunchG);
		}

		// 입력 순서대로 정렬
		sort(OverLapCompoGArr.begin(), OverLapCompoGArr.end(),
			//[](const MSCompoG* a, const MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
			[](MSCompoG* a, MSCompoG* b)->bool {return a->m_nCmdCnt < b->m_nCmdCnt; }
		);

		for (long i = 0; i < OverLapCompoGArr.size(); i++)
			pMyHaunch->AddConnectionWithMember(OverLapCompoGArr[i]);

		pMyHaunch->UpdateTrimmedBoundary();
	}

	for (long iHaunch = 0; iHaunch < NumHaunch; iHaunch++)
	{
		MSSubHaunchG* pMyLevelDif = Haunches[iHaunch];
		pMyLevelDif->UpdateBoundaryVisibilities();
		if(pFloor != nullptr)
			pFloor->CalculateUpdatedCompoGArray(pMyLevelDif, pFloor->CompoGIDList_NeedRegen_CurTR());
	}
}

void MSCompoGUtil::Sort(vector<double>& LocArr, bool isASC)
{
	if(isASC)
		sort(LocArr.begin(), LocArr.end(), [](const double& a, const double& b)->bool {return a < b; });
	else
		sort(LocArr.begin(), LocArr.end(), [](const double& a, const double& b)->bool {return a > b; });
}

vector<MSCompoG*> MSCompoGUtil::FindCompoGInCrossQTArr(GMObject* pGeometry, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, FindType_MSElement memberType, bool bFindOnlyOnLine, bool bCheckOverlap, double dTol, bool bIncludeOnLine)
{
	GM2DVector* pVec = dynamic_cast<GM2DVector*>(pGeometry);
	GM2DCurve* pCurve = dynamic_cast<GM2DCurve*>(pGeometry);
	GM2DPolyline* pPoly = dynamic_cast<GM2DPolyline*>(pGeometry);

	vector<MSCompoG*> CompoGArr;

	map<MSFloorQT*, MSFloorQT*> UsedFloorQTMap;
	map<long, MSCompoG*> CompoGMap;

	long nQT = CrossQTArr.GetSize();
	for (long iQT = 0; iQT < nQT; iQT++)
	{
		MSFloorQT* pCurQT = CrossQTArr[iQT];
		if (UsedFloorQTMap.find(pCurQT) != UsedFloorQTMap.end())
			continue;

		UsedFloorQTMap.insert(make_pair(pCurQT, pCurQT));

		long NumVerMem = pCurQT->ma_VerticalMem.GetSize();
		for (long iMem = 0; iMem < NumVerMem; iMem++)
		{
			MSCompoG* pCompoG = pCurQT->ma_VerticalMem[iMem];
			if (pCompoG == NULL) continue;

			if (memberType == 0 || pCompoG->IsSameFindType(memberType))
				CompoGMap.insert(make_pair(pCompoG->m_ID, pCompoG));
		}

		long NumFloorMem = pCurQT->ma_FloorMem.GetSize();
		for (long iMem = 0; iMem < NumFloorMem; iMem++)
		{
			MSCompoG* pCompoG = pCurQT->ma_FloorMem[iMem];
			if (pCompoG == NULL) continue;

			if (memberType == 0 || pCompoG->IsSameFindType(memberType))
				CompoGMap.insert(make_pair(pCompoG->m_ID, pCompoG));
		}
	}

	// 쿼드 트리에 존재하는 모든 CompoG를 모아둠
	map<long, MSCompoG*>::iterator it;
	for (it = CompoGMap.begin(); it != CompoGMap.end(); it++)
		CompoGArr.push_back(it->second);

	if (bCheckOverlap == true)
	{
		for (long iCompoG = CompoGArr.size() - 1; iCompoG >= 0; iCompoG--)
		{
			MSCompoG* pCompoG = CompoGArr[iCompoG];
			GM2DPolyline* pCompoGPoly = pCompoG->GetTrimmedBoundary();

			if (pVec != NULL)
			{
				if (pCompoGPoly->Contains(pVec, dTol) >= 1)
					CompoGArr.erase(CompoGArr.begin() + iCompoG);
			}
			else if (pCurve != NULL)
			{
				if (pCompoGPoly->OverlapEx(pCurve, dTol, bIncludeOnLine) == FALSE)
					CompoGArr.erase(CompoGArr.begin() + iCompoG);
			}
			else if (pPoly != NULL)
			{
				if (pPoly == pCompoGPoly)
					continue;

				if (bFindOnlyOnLine == true)
				{
					//최초로직: 라인부재의 경우 폴리라인중 라인부재의 센터라인과  선상에 있는 부재만 CompoGArr에 남겨뒀음
					//수정 [BH-677]
					//수정 [BH-985] 슬래브의 InnerProfile 외에도 다른 곳에서 사용할수 있는 경우 발생(LevelZone 등 등)
					//최종로직: 부재의 폴리라인과 찾고자하는 폴리라인이 교차하는 경우에만 CompoGArr에 남겨둠
					if (pPoly->Overlap(*pCompoGPoly, dTol, bIncludeOnLine) == false)
						CompoGArr.erase(CompoGArr.begin() + iCompoG);
				}
				else
				{
					if (pCompoGPoly->OverlapEx(*pPoly, dTol, bIncludeOnLine) == FALSE)
						CompoGArr.erase(CompoGArr.begin() + iCompoG);
				}
			}
		}
	}

	return CompoGArr;
}

//memberType이 msNULL이면 모든 부재 리턴
vector<MSCompoG*> MSCompoGUtil::FindCompoG(MSFloor* pFloor, GM2DVector* pVec, FindType_MSElement memberType, bool bCheckOverlap, double dTol)
{
	vector<MSCompoG*> CompoGArr;
	if (pFloor == nullptr)
		return CompoGArr;

	CompoGArr = pFloor->mp_QT->FindCompoG(pVec, memberType, true, bCheckOverlap, dTol);
	return CompoGArr;
}

//memberType이 msNULL이면 모든 부재 리턴
vector<MSCompoG*> MSCompoGUtil::FindCompoG(MSFloor* pFloor, GM2DPolyline* pPoly, FindType_MSElement memberType, bool bFindOnlyOnLine, bool bCheckOverlap, double dTol, bool bIncludeOnLine)
{
	vector<MSCompoG*> CompoGArr;
	if (pFloor == nullptr)
		return CompoGArr;

	CompoGArr = pFloor->mp_QT->FindCompoG(pPoly, memberType, bFindOnlyOnLine, bCheckOverlap, dTol, bIncludeOnLine);
	return CompoGArr;
}

//memberType이 msNULL이면 모든 부재 리턴
vector<MSCompoG*> MSCompoGUtil::FindCompoG(MSFloor* pFloor, GM2DCurve* pCurve, FindType_MSElement memberType, bool bCheckOverlap, double dTol)
{
	vector<MSCompoG*> CompoGArr;
	if (pFloor == nullptr)
		return CompoGArr;

	CompoGArr = pFloor->mp_QT->FindCompoG(pCurve, memberType, true, bCheckOverlap, dTol);
	return CompoGArr;
}

vector<MSElement*> MSCompoGUtil::FindPolyElement(MSFloor* pFloor, GM2DPolyline* pPoly, bool bFindOnlyOnLine, bool bCheckOverlap, double dTol)
{
	vector<MSElement*> ElementArr;
	vector<MSCompoG*> SlabArr = MSCompoGUtil::FindCompoG(pFloor, pPoly, msPolyMember, bFindOnlyOnLine, bCheckOverlap, dTol);
	for(long i = 0 ; i < SlabArr.size() ; i++)
		ElementArr.push_back(SlabArr[i]);

	for(map<long, MSConstructionZone*>::iterator itr = pFloor->mm_CZone.begin() ; itr != pFloor->mm_CZone.end() ; ++itr)
	{
		MSConstructionZone* pCZone = itr->second;
		if(pCZone->GetProfile()->OverlapEx(*pPoly, dTol))
			ElementArr.push_back(pCZone);
	}

	for(long i = 0 ; i < pFloor->ma_LevelZone.size() ; i++)
	{
		MSLevelZone* pLevelZone = pFloor->ma_LevelZone[i];
		if(pLevelZone->GetProfile()->OverlapEx(*pPoly, dTol))
			ElementArr.push_back(pLevelZone);
	}

	return ElementArr;
}

vector<MSFloor*> MSCompoGUtil::GetFloorArrByFindFloorType(MSCompoG* pCurCompoG, eFIND_FLOOR eFindFloorType)
{
	vector<MSFloor*> FloorArr;

	if (pCurCompoG == nullptr || pCurCompoG->GetFloor() == nullptr)
		return FloorArr;
	
	MSFloor* pCurFloor = pCurCompoG->GetFloor();
	if (eFindFloorType == FIND_ALLFLOOR)
		FloorArr = pCurCompoG->GetOverlapFloor();
	else if (eFindFloorType == FIND_CURFLOOR)
		FloorArr.push_back(pCurFloor);
	else if (eFindFloorType == FIND_DNFLOOR)
		FloorArr.push_back(pCurFloor->GetLowerFloor());
	else if (eFindFloorType == FIND_UPFLOOR)
		FloorArr.push_back(pCurFloor->GetUpperFloor());
	
	return FloorArr;
}

vector<MSCompoG*> MSCompoGUtil::FindConnectMember(MSCompoG* pCurCompoG, FindType_MSElement memberType, long nFloorType, double dTol, bool bUseTrimedBoundry, bool bFindPolyMemberOnlyOnLine, bool bIncludeOnLine)
{
	if (pCurCompoG->IsLinkMember())
		return MSCompoGUtil::FindConnectMember((MSLinkMemberG*)pCurCompoG, CON_ALL, memberType, nFloorType, false, dTol, bUseTrimedBoundry, bFindPolyMemberOnlyOnLine, bIncludeOnLine);

	GM2DPolyline* pMyBoundary = NULL;
	if (bUseTrimedBoundry)
		pMyBoundary = pCurCompoG->GetTrimmedBoundary();
	else
		pMyBoundary = pCurCompoG->GetProfile();

	vector<MSCompoG*> CompoGArr;
	vector<MSCompoG*> FindCompoGArr;

	MSFloor* pCurFloor = pCurCompoG->GetFloor();
	vector<MSFloor*> FloorArr = MSCompoGUtil::GetFloorArrByFindFloorType(pCurCompoG, (eFIND_FLOOR)nFloorType);

	for (long iFloor = 0; iFloor < FloorArr.size(); iFloor++)
	{
		MSFloor* pFloor = FloorArr[iFloor];
		if (pFloor == NULL)
			continue;

		if (pFloor == pCurFloor)
		{
			if (pCurCompoG->IsPointMember())
			{
				long NumConn = pCurCompoG->ma_Connections.size();
				for (long iConn = 0; iConn < NumConn; iConn++)
				{
					MSConnection* pConn = pCurCompoG->ma_Connections[iConn];
					MSCompoG* pPartnerG = pConn->GetPartner(pCurCompoG);
					if(pPartnerG == nullptr)
						continue;

					if (memberType == msNULL || pPartnerG->IsSameFindType(memberType))
						CompoGArr.push_back(pPartnerG);
				}

				if (memberType == MSElement::msSlabG || memberType == MSElement::msNULL)
					FindCompoGArr = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, FindType_MSElement::msSlabG, bFindPolyMemberOnlyOnLine, true, dTol, bIncludeOnLine);
			}
			else if (pCurCompoG->IsPolyMember() && pCurCompoG->ma_CrossQT.GetSize() > 0)
				FindCompoGArr = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, memberType, bFindPolyMemberOnlyOnLine, true, dTol, bIncludeOnLine);
			else
				FindCompoGArr = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, (FindType_MSElement)memberType, bFindPolyMemberOnlyOnLine, true, dTol, bIncludeOnLine);
		}
		else
			FindCompoGArr = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, (FindType_MSElement)memberType, bFindPolyMemberOnlyOnLine, true, dTol, bIncludeOnLine);

		for (long iCompoG = 0; iCompoG < FindCompoGArr.size(); iCompoG++)
		{
			MSCompoG* pCompoG = FindCompoGArr[iCompoG];
			CheckModelOpt CheckFlag;
			if (pCompoG == pCurCompoG || pCurCompoG->IsLevelConnectedOverLap(pCompoG, CheckFlag, true) == false)
				continue;

			// 기둥인 경우 바로 옆에 붙어 있는 기둥을 찾지 않는다
			if (pCurCompoG->GetType() == MSElement::msColumnG && pCompoG->GetType() == MSElement::msColumnG)
			{
				if (pCurCompoG->GetProfile()->OverlapEx(*pCompoG->GetProfile(), MSFloor::ms_DefLineHitDistTol, false) == false)
					continue;
			}

			CompoGArr.push_back(pCompoG);
		}
	}

	sort(CompoGArr.begin(), CompoGArr.end(),
		//[](MSCompoG* a, MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
		[](MSCompoG* a, MSCompoG* b)->bool {return a->m_nCmdCnt < b->m_nCmdCnt; }
	);

	vector<MSCompoG*>::iterator new_end;
	new_end = unique(CompoGArr.begin(), CompoGArr.end());
	CompoGArr.erase(new_end, CompoGArr.end());

	return CompoGArr;
}

vector<MSCompoG*> MSCompoGUtil::FindConnectMemberByCurve(MSSlabG* pCurSlabG, GM2DCurve* pFindCurve, FindType_MSElement memberType, long nFloorType, double dTol, bool bUseInnerBoundry, bool bFindPolyMemberOnlyOnLine, bool bIncludeOnLine)
{
	GM2DPolyline* pMyBoundary = NULL;
	if (bUseInnerBoundry)
		pMyBoundary = &pCurSlabG->GetInnerProfile();
	else
		pMyBoundary = pCurSlabG->GetTrimmedBoundary();

	vector<MSCompoG*> CompoGArr;
	vector<MSCompoG*> FindCompoGArr;

	MSFloor* pCurFloor = pCurSlabG->GetFloor();
	vector<MSFloor*> FloorArr = MSCompoGUtil::GetFloorArrByFindFloorType(pCurSlabG, (eFIND_FLOOR)nFloorType);

	for (long iFloor = 0; iFloor < FloorArr.size(); iFloor++)
	{
		MSFloor* pFloor = FloorArr[iFloor];
		if (pFloor == NULL)
			continue;

		FindCompoGArr = MSCompoGUtil::FindCompoG(pFloor, pFindCurve, memberType, true, dTol);
		for (long iCompoG = 0; iCompoG < FindCompoGArr.size(); iCompoG++)
		{
			MSCompoG* pCompoG = FindCompoGArr[iCompoG];
			CheckModelOpt CheckFlag;
			if (pCompoG == pCurSlabG || pCurSlabG->IsLevelConnectedOverLap(pCompoG, CheckFlag, true) == false)
				continue;

			CompoGArr.push_back(pCompoG);
		}
	}

	sort(CompoGArr.begin(), CompoGArr.end(),
		//[](MSCompoG* a, MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
		[](MSCompoG* a, MSCompoG* b)->bool {return a->m_nCmdCnt < b->m_nCmdCnt; }
	);

	vector<MSCompoG*>::iterator new_end;
	new_end = unique(CompoGArr.begin(), CompoGArr.end());
	CompoGArr.erase(new_end, CompoGArr.end());

	return CompoGArr;
}

vector<MSCompoG*> MSCompoGUtil::FindConnectMember(MSLinkMemberG* pMyLinkG, CON_POS eConPos, FindType_MSElement memberType, long nFloorType, bool isReverse, double dTol, bool bUseTrimedBoundry, bool bFindPolyMemberOnlyOnLine, bool bIncludeOnLine)
{
	CON_POS ePos = eConPos;
	if (isReverse)
	{
		if (eConPos == CON_START)	ePos = CON_END;
		else if (eConPos == CON_END) ePos = CON_START;
	}

	shared_ptr<GM2DCurve> pMyCurve = pMyLinkG->GetCenCurve();
	GM2DPolyline* pMyBoundary = NULL;
	if (bUseTrimedBoundry)
		pMyBoundary = pMyLinkG->GetTrimmedBoundary();
	else
		pMyBoundary = pMyLinkG->GetProfile();

	vector<MSCompoG*> CompoGArr;
	vector<MSCompoG*> FindCompoGArr;

	MSFloor* pCurFloor = pMyLinkG->GetFloor();
	vector<MSFloor*> FloorArr = MSCompoGUtil::GetFloorArrByFindFloorType(pMyLinkG, (eFIND_FLOOR)nFloorType);

	for (long iFloor = 0; iFloor < FloorArr.size(); iFloor++)
	{
		MSFloor* pFloor = FloorArr[iFloor];
		if (pFloor == NULL)
			continue;

		MSElement::Type_MSCompoG compoGType = pMyLinkG->GetType();
		if (pFloor == pCurFloor && pMyLinkG->GetType() != MSElement::msLineLevelDifG)
		{
			bool checkSameFloor = compoGType == MSElement::msHandRailG ? false : true;
			vector<MSConnection*> ConnArr = pMyLinkG->GetConnectionByPos(ePos, checkSameFloor);
			long NumConn = ConnArr.size();
			for (long iConn = 0; iConn < NumConn; iConn++)
			{
				MSConnection* pConn = ConnArr[iConn];
				MSCompoG* pPartnerG = pConn->GetPartner(pMyLinkG);
				// 슬래브는 MSCompoGUtil::FindCompoG로 검토해야 한다.
				if (pPartnerG == nullptr || pPartnerG->GetType() == MSElement::msSlabG)
					continue;

				if (memberType == msNULL || pPartnerG->IsSameFindType(memberType))
					CompoGArr.push_back(pPartnerG);
			}

			if (memberType == MSElement::msSlabG || memberType == MSElement::msNULL)
			{
				GM2DPolyline* pMyTrimmedBoundary = pMyLinkG->GetTrimmedBoundary();
				MSFloor* pFloor = pMyLinkG->GetFloor();

				FindCompoGArr = MSCompoGUtil::FindCompoG(pFloor, pMyTrimmedBoundary, FindType_MSElement::msSlabG, bFindPolyMemberOnlyOnLine, true, dTol, bIncludeOnLine);

				for (long iCompoG = 0; iCompoG < FindCompoGArr.size(); iCompoG++)
				{
					MSCompoG* pCompoG = FindCompoGArr[iCompoG];
					CheckModelOpt CheckFlag;
					if (pMyLinkG->IsLevelConnectedOverLap(pCompoG, CheckFlag, true) == false)
						continue;

					CompoGArr.push_back(pCompoG);
				}
			}
		}
		else
		{
			if (pFloor == pCurFloor)
				FindCompoGArr = MSCompoGUtil::FindCompoGInCrossQTArr(pMyBoundary, pMyLinkG->ma_CrossQT, (FindType_MSElement)memberType, bFindPolyMemberOnlyOnLine, true, dTol, bIncludeOnLine);
			else
				FindCompoGArr = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, (FindType_MSElement)memberType, bFindPolyMemberOnlyOnLine, true, dTol, bIncludeOnLine);

			for (long iCompoG = 0; iCompoG < FindCompoGArr.size(); iCompoG++)
			{
				MSCompoG* pCompoG = FindCompoGArr[iCompoG];
				CheckModelOpt CheckFlag;
				if (pMyLinkG->IsLevelConnectedOverLap(pCompoG, CheckFlag, true) == false)
					continue;

				if (pCompoG->IsLinkMember())
				{
					MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pCompoG;
					if (ePos != CON_ALL)
					{
						GM2DVector CrossVec;
						double dMyLoc, dUrLoc, dMyLocTol, dUrLocTol;
						long nRval = pMyLinkG->CheckIntersectLinkMember(pUrLinkG, CrossVec, dMyLoc, dUrLoc, dMyLocTol, dUrLocTol);
						if (nRval == 1)
						{
							CON_POS eMyConPos;
							MSConnection::SetConnPos(eMyConPos, dMyLoc, dMyLocTol, dUrLoc, pMyLinkG, pUrLinkG);

							if (ePos != eMyConPos)
								continue;
						}
						else if (nRval == 2) // 교차점이 2개 인경우(라인+라인 = 평행), (라인+아크/아크+아크) ?? 
						{
							if (ePos == CON_START && dMyLoc > 0.5)
								continue;
							else if (ePos == CON_END && dMyLoc < 0.5)
								continue;
						}
					}
				}
				else if (pCompoG->IsPointMember())
				{
					if (ePos != CON_ALL)
					{
						GM2DVector* pVec = pCompoG->GetSVec();

						double dLen = pMyCurve->Length();
						double dSecondWidth = min(pCompoG->GetWidth(), pCompoG->GetDepth());
						double dLoc = pMyCurve->OrthoBaseLocValue(*pVec);
						double dLocTol = 0;
						if (dLen)
							dLocTol = (dSecondWidth*0.5) / dLen + MSFloor::ms_DefLineHitDistTol / dLen;

						GM2DPolyline Poly(TRUE);
						pCompoG->GetBoundary(Poly);

						CON_POS eMyConPos;

						GM2DLineVector FirstSLine(pMyLinkG->mp_LLine->m_P0, pMyLinkG->mp_RLine->m_P0);
						GM2DLineVector FirstELine(pMyLinkG->mp_LLine->m_P1, pMyLinkG->mp_RLine->m_P1);
						MSConnection::SetConnPos(eMyConPos, dLoc, dLocTol, FirstSLine, FirstELine, &Poly);

						if (ePos != eMyConPos)
							continue;
					}
				}

				CompoGArr.push_back(pCompoG);
			}
		}
	}

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		if (pCompoG->GetType() == MSElement::msSlabG && (memberType == MSElement::msNULL || memberType == MSElement::msLineLevelDifG))
		{
			MSSlabG* pSlab = (MSSlabG*)pCompoG;

			vector<MSLinkMemberG*> LevelDifArr = pSlab->GetLevelDifGAll(true);
			for (long iDif = 0; iDif < LevelDifArr.size(); iDif++)
			{
				MSLinkMemberG* pDif = LevelDifArr[iDif];
				CheckModelOpt CheckFlag;
				if (pMyLinkG->IsParallel(pDif, DTOL_MODELING) && pMyLinkG->IsLevelConnectedOverLap(pDif, CheckFlag))
					CompoGArr.push_back(pDif);
			}
		}
	}

	sort(CompoGArr.begin(), CompoGArr.end(),
		//[](MSCompoG* a, MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
		[](MSCompoG* a, MSCompoG* b)->bool {return a->m_nCmdCnt < b->m_nCmdCnt; }
	);

	vector<MSCompoG*>::iterator new_end;
	new_end = unique(CompoGArr.begin(), CompoGArr.end());
	CompoGArr.erase(new_end, CompoGArr.end());

	return CompoGArr;
}

vector<MSCompoG*> MSCompoGUtil::FindColinerLinkMember(MSLinkMemberG* pLinkG, CON_POS eConPos, FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool isReverse, bool bUseTrimedBoundry)
{
	vector<MSCompoG*> CompoGArr;

	shared_ptr<GM2DCurve> pMyCenCurve = pLinkG->GetCenCurve();
	double dUrP0Loc, dUrP1Loc;

	vector<MSCompoG*> ConnectLinkCompoGArr = MSCompoGUtil::FindConnectMember(pLinkG, eConPos, memberType, nFloorType, isReverse, DistTol, bUseTrimedBoundry);
	// PC 부재인 경우 포인트 부재를 찾아 연결된 부재를 다시 검토하도록 한다.
	if (pLinkG->IsPC())
	{
		MSCompoG* pPointMember = nullptr;
		vector<MSCompoG*> ConnectPointMemberGArr = MSCompoGUtil::FindConnectMember(pLinkG, eConPos, FindType_MSElement::msPointMember, nFloorType, isReverse, DistTol, bUseTrimedBoundry);
		for (int iConCompoG = 0; iConCompoG < ConnectPointMemberGArr.size(); iConCompoG++)
		{
			if (ConnectPointMemberGArr[iConCompoG]->IsPointMember())
			{
				pPointMember = ConnectPointMemberGArr[iConCompoG];
				break;
			}
		}
		if (pPointMember != nullptr)
		{
			for (int iConn = 0; iConn < pPointMember->ma_Connections.size(); iConn++)
			{
				MSCompoG* pPartnerG = pPointMember->ma_Connections[iConn]->GetPartner(pPointMember);
				if(pPartnerG == nullptr || pPartnerG == pLinkG)	continue;
				if (memberType == msNULL || pPartnerG->IsSameFindType(memberType))
					ConnectLinkCompoGArr.push_back(pPartnerG);
			}
		}
	}

	for (long iCompoG = 0; iCompoG < ConnectLinkCompoGArr.size(); iCompoG++)
	{
		if (ConnectLinkCompoGArr[iCompoG]->IsLinkMember() == false)
			continue;
		MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)ConnectLinkCompoGArr[iCompoG];
		shared_ptr<GM2DCurve> pUrCenCurve = pUrLinkG->GetCenCurve();

		if (pLinkG->isArc() && pUrLinkG->isArc())
			DistTol = max(pLinkG->GetWidth(), pUrLinkG->GetWidth()) + DTOL_MODELING;

		long res = pMyCenCurve->IsParallel4DTol(&*pUrCenCurve, dTolDeg);
		if (res != 0)
		{
			//DistTol 변경
			if (pLinkG->isArc() && pUrLinkG->isArc())
				DistTol = max(pLinkG->GetWidth(), pUrLinkG->GetWidth()) + DTOL_MODELING;
			else
				DistTol = pLinkG->GetWidth() / 2 + pUrLinkG->GetWidth() / 2;
			//김하나-20150604
			//나란한 부재를 찾을 경우 상대편 부재의 길이가 LinkTestLength보다 작은 경우에 대한 예외처리-임시
			//LinkTestLength를 지정한 부분에 대한 검토후 수정 필요
			double dUrLinkTestLength = pUrCenCurve->Length()*0.2;
			if (dUrLinkTestLength < LinkTestLength)
				LinkTestLength = dUrLinkTestLength;
			long res = pMyCenCurve->Overlap4DTol(&*pUrCenCurve, dUrP0Loc, dUrP1Loc, dTolDeg, DistTol, LinkTestLength);
			if (res == 1 || res == 2 || res == 6 || res == 7 || res == 19 || res == 20 || res == 24 || res == 25)
				CompoGArr.push_back(pUrLinkG);
		}
	}

	// 같은 층 동일한 위치에 오프셋 처리하여 2개의 라인객체가 모델링되는 경우가 존재한다 ZLevel을 체크하여 터치 되는 부재는 제거한다.
	for (int idx = CompoGArr.size() - 1; idx >= 0; idx--)
	{
		MSCompoG* pFindG = CompoGArr[idx];
		if (pLinkG->IsLevelTouch(pFindG))
			CompoGArr.erase(CompoGArr.begin() + idx);
	}

	return CompoGArr;
}

vector<MSCompoG*> MSCompoGUtil::FindOverlapLinkMember(MSLinkMemberG* pLinkG, FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool bCheckMemberWidth, bool bUseTrimedBoundry, bool bIncludeOnLine)
{
	vector<MSCompoG*> CompoGArr;

	shared_ptr<GM2DCurve> pMyCenCurve = pLinkG->GetCenCurve();

	double dMyHalfWidth = pLinkG->GetWidth() / 2;
	double dUrP0Loc, dUrP1Loc;
	vector<MSCompoG*> ConnectLinkCompoGArr = MSCompoGUtil::FindConnectMember(pLinkG, CON_ALL, memberType, nFloorType, false, DistTol, bUseTrimedBoundry);
	for (long iCompoG = 0; iCompoG < ConnectLinkCompoGArr.size(); iCompoG++)
	{
		if (ConnectLinkCompoGArr[iCompoG]->IsLinkMember() == false)
			continue;
		MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)ConnectLinkCompoGArr[iCompoG];
		shared_ptr<GM2DCurve> pUrCenCurve = pUrLinkG->GetCenCurve();

		//if(isArc() && pUrLinkG->isArc())
		DistTol = max(pLinkG->GetWidth(), pUrLinkG->GetWidth()) + DTOL_MODELING;

		long res = pMyCenCurve->IsParallel4DTol(&*pUrCenCurve, dTolDeg);// , DistTol);
		if (res != 0)
		{
			//DistTol 변경
			if (pLinkG->isArc() && pUrLinkG->isArc())
				DistTol = max(pLinkG->GetWidth(), pUrLinkG->GetWidth()) + DTOL_MODELING;
			else if (bCheckMemberWidth)
				DistTol = dMyHalfWidth + pUrLinkG->GetWidth() / 2;

			if (bIncludeOnLine == false)
			{
				if (bCheckMemberWidth)
					DistTol = fabs(DistTol - DTOL_MODELING);
				else
					DistTol = fabs(DistTol - DTOL_MODELING * 2);
			}

			long res = pMyCenCurve->Overlap4DTol(&*pUrCenCurve, dUrP0Loc, dUrP1Loc, dTolDeg, DistTol, LinkTestLength);
			if (res == 0 || res == 1 || res == 2 || res == 6 || res == 7 || res == 19 || res == 20 || res == 24 || res == 25)
				continue;
			CompoGArr.push_back(pUrLinkG);
		}
	}

	return CompoGArr;
}

CON_POS MSCompoGUtil::ToConPos(char cLoc)
{
	CON_POS eConPos = CON_UNKNOWN;
	if (cLoc == 'S')
		eConPos = CON_START;
	else if (cLoc == 'E')
		eConPos = CON_END;
	else if (cLoc == 'M')
		eConPos = CON_MID;
	else if (cLoc == 'A')
		eConPos = CON_ALL;

	return eConPos;
}

bool MSCompoGUtil::IsContainCurveinLinkMember(MSFloor* pFloor, GM2DCurve* pCurve, vector<MSElement::Type_MSCompoG> findTypeArr, double dRatio)
{
	vector<MSCompoG*> tempCompoGArr = FindCompoG(pFloor, pCurve, FindType_MSElement::msNULL);

	vector<MSCompoG*>findLinkGArr;
	for (int idx = 0; idx < tempCompoGArr.size(); idx++)
	{
		MSCompoG* pCompoG = tempCompoGArr[idx];
		if (pCompoG->IsLinkMember() == false)
			continue;

		MSLinkMemberG* pLinkG = dynamic_cast<MSLinkMemberG*>(pCompoG);
		if (pLinkG == nullptr)
			continue;

		shared_ptr<GM2DCurve> pCenCurve = pLinkG->GetCenCurve();
		if (pCenCurve->IsParallel4DTol(pCurve) == false)
			continue;

		MSElement::Type_MSCompoG curType = pLinkG->GetType();

		if (find(findTypeArr.begin(), findTypeArr.end(), curType) != findTypeArr.end())
			findLinkGArr.push_back(pCompoG);
	}

	bool isStartVecContain = false;
	bool isEndVecContain = false;
	double dTol = DTOL_MODELING;
	vector<GM2DVector> CrossVecArr;
	for (int idx = 0; idx < findLinkGArr.size(); idx++)
	{
		MSLinkMemberG* pLinkG = dynamic_cast<MSLinkMemberG*>(findLinkGArr[idx]);
		GM2DPolyline* pPoly = pLinkG->GetTrimmedBoundary_Acis();

		vector<GM2DVector> tempVecArr = pPoly->IntersectEx(pCurve, 0, dTol);
		CrossVecArr.insert(CrossVecArr.end(), tempVecArr.begin(), tempVecArr.end());
	}

	vector<double> crossLocArr;
	for (int idx = 0; idx < CrossVecArr.size(); idx++)
	{
		double dLoc = pCurve->OrthoBaseLocValue(CrossVecArr[idx]);
		if (dLoc > 0 && dLoc < 1)
			crossLocArr.push_back(dLoc);
	}
	crossLocArr.insert(crossLocArr.begin(), 0);
	crossLocArr.push_back(1);

	sort(crossLocArr.begin(), crossLocArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(crossLocArr.begin(), crossLocArr.end());
	crossLocArr.erase(new_end, crossLocArr.end());

	vector<GM2DCurve*> containCurveArr;
	vector<GM2DCurve*> notContainCurveArr;

	for (int idx = 0; idx < crossLocArr.size() - 1; idx++)
	{
		double sLoc = crossLocArr[idx];
		double eLoc = crossLocArr[idx + 1];
		GM2DCurve* pNewCurve = pCurve->GetSegment(sLoc, eLoc);
		GM2DVector midVec = pNewCurve->PointAtParam(0.5);
		bool isContain = false;
		for (int idx = 0; idx < findLinkGArr.size(); idx++)
		{
			MSLinkMemberG* pLinkG = dynamic_cast<MSLinkMemberG*>(findLinkGArr[idx]);
			GM2DPolyline* pPoly = pLinkG->GetTrimmedBoundary_Acis();

			if (pPoly->Contains(pNewCurve, true) == true)
			{
				containCurveArr.push_back(pNewCurve);
				isContain = true;
				break;
			}
		}
		
		if(isContain == false)
			notContainCurveArr.push_back(pNewCurve);
	}

	double curCurveLen = pCurve->Length();
	double containCurveLen = 0;
	for (int idx = 0; idx < containCurveArr.size(); idx++)
	{
		double dLen = containCurveArr[idx]->Length();
		containCurveLen += dLen;
	}

	double notContainCurveLen = 0;
	for (int idx = 0; idx < notContainCurveArr.size(); idx++)
	{
		double dLen = notContainCurveArr[idx]->Length();
		notContainCurveLen += dLen;
	}

	bool rVal = false;

	double containRatio = containCurveLen / curCurveLen;
	if (containRatio >= dRatio)
		rVal = true;

	GMObject::DeleteGMObject(containCurveArr);
	GMObject::DeleteGMObject(notContainCurveArr);

	return rVal;
}