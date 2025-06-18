#include "StdAfx.h"
#include "MSConnLevelZone.h"

#include "MSConnectivity.h"
#include "MSLinkMemberG.h"
#include "MSLinkMemPart.h"
#include "MSLineLevelDifG.h"

#include "MSColumnG.h"
#include "MSIsoFootG.h"
#include "MSFloor.h"

map<MSConnLevelZone*, bool> MSConnLevelZone::_deleted_map;
MSConnLevelZone::MSConnLevelZone( MSConnectivity* pConn, double TopLevel, double BotLevel ) : m_Section(true), m_DrawSection(TRUE)
{
    mp_Conn = pConn;
    m_TopLevel = TopLevel;
    m_BotLevel = BotLevel;

    mp_Column = NULL;
    mp_IsoFoot = NULL;

	mp_DownLinkJointBoundry = NULL;
	m_bUsedDownLinkJoint = FALSE;
	m_isBaseSec = false;
	m_hasColinearDownMemPart = false;
	m_UseTramInOut = false;
	m_hasBuriedLinkMember = false;
	m_nNumMemberG = 0;
	m_HasLevelDifG = false;
	m_hasArcLinkMember = false;
	m_hasArcLinkMemPartCnt = 0;

	m_bDeleted = false;

}

MSConnLevelZone* MSConnLevelZone::CopyOnlyTopBot()
{
	MSConnLevelZone* copy = new MSConnLevelZone(mp_Conn, m_TopLevel, m_BotLevel);
	return copy;
}

MSConnLevelZone::~MSConnLevelZone(void)
{
	/*long nMemPart = ma_MemPart.GetSize();
	for(long iMemPart=0; iMemPart<nMemPart; iMemPart++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[iMemPart];
		if(pMemPart->GetSConn() == mp_Conn)
			pMemPart->DeleteEndJointPoly(this, 0);
		else
			pMemPart->DeleteEndJointPoly(this, 1);
	}*/


	DeleteDownLinkJointPoly();
}

void MSConnLevelZone::Serialize( CArchive& ar )
{
	MSObject::Serialize(ar);

	if(ar.IsStoring())
	{
		long ID = (mp_Conn == NULL ? 0 : mp_Conn->m_ID);
		ar << ID;
		ar << m_TopLevel;
		ar << m_BotLevel;
		m_Section.Serialize(ar);
		m_DrawSection.Serialize(ar);
		ar << m_isBaseSec;
		ar << m_UseTramInOut;
		ar << m_hasBuriedLinkMember;
		ar << m_hasArcLinkMember;

		int count = m_ColinerMemMap.size();
		ar << count;
		for (map<long, long>::iterator itr = m_ColinerMemMap.begin() ; itr != m_ColinerMemMap.end() ; ++itr)
		{
			long key = itr->first;
			long value = itr->second;
			ar << key << value;
		}

		count = ma_MemPart.GetSize();
		ar << count;
		for (int i=0 ; i<ma_MemPart.GetSize() ; ++i)
		{
			long idx_temp = 0;
			MSLinkMemPart::Serialize_AsID(ar, ma_MemPart[i], idx_temp);
		}

		count = ma_BuriedMemPart.GetSize();
		ar << count;
		for (int i=0 ; i<ma_BuriedMemPart.GetSize() ; ++i)
		{
			long idx_temp = 0;
			MSLinkMemPart::Serialize_AsID(ar, ma_BuriedMemPart[i], idx_temp);
		}

		count = ma_SegList.size();
		ar << count;
		for (int i=0 ; i<ma_SegList.size() ; ++i)
			ma_SegList[i].Serialize(ar);

		ID = (mp_Column == NULL ? 0 : mp_Column->m_ID);
		ar << ID;
		ID = (mp_IsoFoot == NULL ? 0 : mp_IsoFoot->m_ID);
		ar << ID;

		ar << m_HasLevelDifG;
		ar << m_bUsedDownLinkJoint;
		ar << m_hasColinearDownMemPart;
		GM2DPolyline::SerializeWithNullFlag(ar, mp_DownLinkJointBoundry);

		ar << m_nNumMemberG;
	}
	else
	{
		long ID = 0;
		ar >> ID;
		mp_Conn = (MSConnectivity*)ID; // NEED TO CONVERT ID->POINTER
		ar >> m_TopLevel;
		ar >> m_BotLevel;
		m_Section.Serialize(ar);
		m_DrawSection.Serialize(ar);
		ar >> m_isBaseSec;
		ar >> m_UseTramInOut;
		ar >> m_hasBuriedLinkMember;
		ar >> m_hasArcLinkMember;

		int count = 0;
		ar >> count;
		m_ColinerMemMap.clear();
		for (int i=0 ; i<count ; ++i)
		{
			long key = 0, value = 0;
			ar >> key;
			ar >> value;
			m_ColinerMemMap[key] = value;
		}

		ar >> count;
		ma_MemPart.RemoveAll(); ma_MemPart_idx.clear();
		for (int i=0 ; i<count ; ++i)
		{
			long part_idx = 0;
			MSLinkMemPart* part = NULL;
			MSLinkMemPart::Serialize_AsID(ar, part, part_idx);
			ma_MemPart.Add(part); // NEED TO CONVERT ID->POINTER
			ma_MemPart_idx.push_back(part_idx);
		}

		ar >> count;
		ma_BuriedMemPart.RemoveAll(); ma_BuriedMemPart_idx.clear();
		for (int i=0 ; i<count ; ++i)
		{
			long part_idx = 0;
			MSLinkMemPart* part = NULL;
			MSLinkMemPart::Serialize_AsID(ar, part, part_idx);
			ma_BuriedMemPart.Add(part); // NEED TO CONVERT ID->POINTER
			ma_BuriedMemPart_idx.push_back(part_idx);
		}

		ar >> count;
		ma_SegList.resize(count);
		for (int i=0 ; i<count ; ++i)
			ma_SegList[i].Serialize(ar);

		ar >> ID;
		mp_Column = (MSColumnG*)ID; // NEED TO CONVERT ID->POINTER
		ar >> ID;
		mp_IsoFoot = (MSIsoFootG*)ID; // NEED TO CONVERT ID->POINTER

		ar >> m_HasLevelDifG;
		ar >> m_bUsedDownLinkJoint;
		ar >> m_hasColinearDownMemPart;
		GM2DPolyline::SerializeWithNullFlag(ar, mp_DownLinkJointBoundry);

		ar >> m_nNumMemberG;

		if (!MSConnectivity::JOINT_SERIALIZE())
		{
			mp_Conn = NULL;
			m_ColinerMemMap.clear();
			ma_MemPart.RemoveAll(); ma_MemPart_idx.clear();
			ma_BuriedMemPart.RemoveAll(); ma_BuriedMemPart_idx.clear();
			ma_SegList.clear();
			mp_Column = NULL;
			mp_IsoFoot = NULL;
		}
	}
}

void MSConnLevelZone::RecoverPointerAfterSerialize(MSFloor* floor)
{
	long ID = 0;
	long idx = 0;

	ID = (long)mp_Conn;
	mp_Conn = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);

	for (int i=0 ; i<ma_MemPart.GetSize() ; ++i)
	{
		ID = (long)ma_MemPart[i];
		idx = ma_MemPart_idx[i];
		ma_MemPart[i] = NULL;
		if (floor->mm_CompoG.count(ID) > 0)
		{
			MSCompoG* compoG = (floor->mm_CompoG[ID]);
			MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(compoG);
// 			if (link != NULL && idx < link->ma_MemPart.size())
// 				ma_MemPart[i] = link->ma_MemPart[idx];
		}
		//if (ma_MemPart[i] == NULL) ASSERT(0);
	}
	ma_MemPart_idx.clear();

	for (int i=0 ; i<ma_BuriedMemPart.GetSize() ; ++i)
	{
		ID = (long)ma_BuriedMemPart[i];
		idx = ma_BuriedMemPart_idx[i];
		ma_BuriedMemPart[i] = NULL;
		if (floor->mm_CompoG.count(ID) > 0)
		{
			MSCompoG* compoG = (floor->mm_CompoG[ID]);
			MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(compoG);
// 			if (link != NULL && idx < link->ma_MemPart.size())
// 				ma_BuriedMemPart[i] = link->ma_MemPart[idx];
		}
		if (ma_BuriedMemPart[i] == NULL) ASSERT(0);
	}
	ma_BuriedMemPart_idx.clear();

	ID = (long)mp_Column;
	mp_Column = (MSColumnG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);

	ID = (long)mp_IsoFoot;
	mp_IsoFoot = (MSIsoFootG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);

}

bool MSConnLevelZone::TrimByAMemPart(MSLinkMemPart* pMyPart, GM2DPolyline& TheSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
    GM2DCurve* pMyLine = NULL;
    GM2DCurve* pUrLine = NULL;
    GM2DVector* pMyEndPoint = NULL, *pUrEndPoint = NULL;
    GM2DLineVector* MyEndLine=NULL, *UrEndLine=NULL;
    long MyEndNum, UrEndNum;

    GM2DVector MyCrossVec, UrCrossVec;

    MSLinkMemPart* pMyMemPart = pMyPart;
    if(pMyMemPart->GetEConn()== mp_Conn)
    {
        pMyEndPoint = &(pMyMemPart->mp_LLine->m_P1);
        MyEndNum = 1;
    }
    else if(pMyMemPart->GetSConn()== mp_Conn)
    {
        pMyEndPoint = &(pMyMemPart->mp_LLine->m_P0);
        MyEndNum = 0;
    }
    else
        ASSERT(0);

    long nMem = MemPartArr.GetSize();
    if(nMem==0)
        return false;
    long MyIndex=nMem-1;

    bool bLCutWhenShorten = false;
    bool bRCutWhenShorten = false;
    long nEdge = m_Section.GetSize();
    for(long iEdge=0; iEdge<nEdge; iEdge++)
    {
        GM2DLineVector& TheEdge = m_Section.GetLine(iEdge);
        if(CutLineByEndLine(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum, &TheEdge, bLCutWhenShorten)==1)
            bLCutWhenShorten = true;
        if(CutLineByEndLine(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum, &TheEdge, bRCutWhenShorten)==1)
            bRCutWhenShorten = true;
        if(nMem==3)  // 2개 부재 트림 시 외부교차에 의해 연장된 라인을 절점 폴리곤에 의해 잘리도록 선행 조치
        {
            for(long iMem=0; iMem<3; iMem++)
            {
                if(pMyMemPart==MemPartArr[iMem]) continue;
                long UrEndNum=0;
                if(MemPartArr[iMem]->GetEConn()== mp_Conn)
                    UrEndNum = 1;
                CutLineByEndLine(MemPartArr[iMem], 'L', MemPartArr[iMem]->mp_LLine, UrEndNum, &TheEdge, true);
                CutLineByEndLine(MemPartArr[iMem], 'R', MemPartArr[iMem]->mp_RLine, UrEndNum, &TheEdge, true);
            }
        }
    }


    bool bLLineCut = false;
    bool bRLineCut = false;

    for(int j = 0; j< nMem ;j++)
    {
        MSLinkMemPart* pUrMemPart = MemPartArr[j];
        if(pMyMemPart==pUrMemPart) continue;
        if(pUrMemPart->GetEConn()== mp_Conn)
        {
            pUrEndPoint = &(pUrMemPart->mp_LLine->m_P1);
            UrEndNum = 1;
        }
        else if(pUrMemPart->GetSConn()== mp_Conn)
        {
            pUrEndPoint = &(pUrMemPart->mp_LLine->m_P0);
            UrEndNum = 0;
        }
        else
            //             ASSERT(0);
            continue;

        ///////////////////////////////////////////////////////////////////////////////
        // this벡터와  ALineVec벡터의 교차점 벡터 CrossVec와 절대 위치(0.~1.) MyLoc, YourLoc 계산
        // return : -1 : Colinear 
        //          -2 : Parallel
        //           1 : Intersect at 1 point
        //           2 : not Parallel and not intersection, 외부 교차
        //           0 : 길이 제로 라인벡터 포함 에러
        ///////////////////////////////////////////////////////////////////////////////

        //if(nMem==2 && pMyMemPart->mp_LinkMem->mp_Curve==NULL && pUrMemPart->mp_LinkMem->mp_Curve==NULL)
		if(nMem==2 && pMyMemPart->mp_LinkMem->isArc() == false && pUrMemPart->mp_LinkMem->isArc() == false)
        {
            if(MyEndNum==UrEndNum)
            {
                CutLineWhenShortening(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum,
                    pUrMemPart, 'R', pUrMemPart->mp_RLine, UrEndNum, nMem);
                CutLineWhenShortening(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum,
                    pUrMemPart, 'L', pUrMemPart->mp_LLine, UrEndNum, nMem);
            }
            else
            {
                CutLineWhenShortening(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum,
                    pUrMemPart, 'L', pUrMemPart->mp_LLine, UrEndNum, nMem);
                CutLineWhenShortening(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum,
                    pUrMemPart, 'R', pUrMemPart->mp_RLine, UrEndNum, nMem);
            }
        }
        else
        {
            CutLineWhenShortening(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum, pUrMemPart, 'L', pUrMemPart->mp_LLine, UrEndNum, nMem);
            CutLineWhenShortening(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum, pUrMemPart, 'R', pUrMemPart->mp_RLine, UrEndNum, nMem);
            CutLineWhenShortening(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum, pUrMemPart, 'L', pUrMemPart->mp_LLine, UrEndNum, nMem);
            CutLineWhenShortening(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum, pUrMemPart, 'R', pUrMemPart->mp_RLine, UrEndNum, nMem);
        }
    }
    return BuildSectionOnGivenMemPart(TheSection, m_DrawSection, MemPartArr);
}

bool MSConnLevelZone::BuildSection(bool bCheck)
{

	ma_MemPart.RemoveAll();
	if(bCheck == false)
	{
		ma_BuriedMemPart.RemoveAll();
		m_hasBuriedLinkMember = false;
	}

	m_HasLevelDifG = mp_Conn->HasMemberType(MSElement::msLineLevelDifG);

    if(mp_Conn->mp_Column && CheckLevelOverlap(mp_Conn->mp_Column))
        mp_Column = mp_Conn->mp_Column;
    if(mp_Conn->mp_IsoFoot && CheckLevelOverlap(mp_Conn->mp_IsoFoot))
        mp_IsoFoot = mp_Conn->mp_IsoFoot;

	double dMaxWidth = mp_Conn->GetMaxWidthByLineMemberG();
	vector<GM2DVector> VecArr;
	CTypedPtrArray<CObArray, CTypedPtrArray<CObArray, MSLinkMemPart*>*> MemPartArr;
	CTypedPtrArray<CObArray, MSLinkMemPart*>* MemPart;
	//MemPartArr.push_back(MemPart);

    long nLineMemPart = mp_Conn->ma_MemPart.size();

	if(nLineMemPart == 0)
	{
		MSPointMemberG* pPointMem=NULL;
		if(mp_Column)
			pPointMem = mp_Column;
		if(mp_IsoFoot)
			pPointMem = mp_IsoFoot;

		if(pPointMem != NULL)
		{
			GM2DPolyline PolyLine;
			pPointMem->GetBoundary(PolyLine);

			m_Section.CopyFrom(&PolyLine);
			m_DrawSection.CopyFrom(&PolyLine);
			return true;
		}
	}

    for(long iMemPart=0; iMemPart<nLineMemPart; iMemPart++)
	{
		MSLinkMemPart* pMemPart = mp_Conn->ma_MemPart[iMemPart];
		// Buried...
// 		if(MSFloor::ms_TrimType == 0)
// 			if(pMemPart == NULL || pMemPart->isBuriedLinkMember()) continue;

        if(CheckLevelOverlap(mp_Conn->ma_MemPart[iMemPart]->mp_LinkMem))
        {
			// 절점에 부재가 추가되거나 삭제되어 다시 트림해야 하므로 양라인 원상복귀
			// MemPart의 끝선 정보 입력(기본은  NULL)
			GM2DVector CheckVec;
			if(mp_Conn == pMemPart->GetSConn())
			{
				pMemPart->AddEndJointPoly(this, 0, NULL);
				pMemPart->SetLinesOfAMemPart(true , false);
				CheckVec = pMemPart->mp_CenLine->m_P0;
			}
			if(mp_Conn == pMemPart->GetEConn())
			{
				pMemPart->AddEndJointPoly(this, 1, NULL);
				pMemPart->SetLinesOfAMemPart(false, true);
				CheckVec = pMemPart->mp_CenLine->m_P1;
			}
// 			else
// 				ASSERT(0);

			if(pMemPart->isBuriedLinkMember())
			{
				if(bCheck == false)
				{
					m_hasBuriedLinkMember = true;
					ma_BuriedMemPart.Add(pMemPart);
				}
				else
					continue;
			}

			if(pMemPart->mp_LinkMem->isArc())
			{
				m_hasArcLinkMember = true;
				m_hasArcLinkMemPartCnt++;
			}

			ma_MemPart.Add(pMemPart);

			if(!m_HasLevelDifG)
			{
				long NumMemPartArr = MemPartArr.GetSize();
				if(NumMemPartArr == 0)
				{
					VecArr.push_back(CheckVec);
					MemPart = new CTypedPtrArray<CObArray, MSLinkMemPart*>;
					MemPart->Add(pMemPart);
					MemPartArr.Add(MemPart);
				}
				else
				{
					bool bFind = false;
					for(long i = 0 ; i < VecArr.size() ; i++)
					{
						GM2DVector TheVec = VecArr[i];
						BOOL bRval = TheVec.SameVector4DTOL(CheckVec, dMaxWidth * 2);
						if(bRval == TRUE)
						{
							bFind = true;
							MemPartArr[i]->Add(pMemPart);
							break;
						}
					}

					if(bFind == false)
					{
						MemPart = new CTypedPtrArray<CObArray, MSLinkMemPart*>;
						VecArr.push_back(CheckVec);
						MemPart->Add(pMemPart);
						MemPartArr.Add(MemPart);
					}
				}
			}
        }
	}

	BuildSectionByDownJoint();
	if(mp_DownLinkJointBoundry != NULL && mp_DownLinkJointBoundry->GetNumLines() > 2)
		m_bUsedDownLinkJoint = TRUE;
	
	bool bSplitTrim = true;
	long NumMemPartArr = MemPartArr.GetSize();
	if(NumMemPartArr == 0)
		bSplitTrim = false;

	for(long i = 0 ; i < NumMemPartArr ; i++)
	{
		for(long j = i+1 ; j < NumMemPartArr ; j++)
		{
			CTypedPtrArray<CObArray, MSLinkMemPart*>* iMemPart = MemPartArr[i];
			CTypedPtrArray<CObArray, MSLinkMemPart*>* jMemPart = MemPartArr[j];
			long Numi = iMemPart->GetSize();
			long Numj = jMemPart->GetSize();
			if(Numi == 1 || Numj == 1)
				bSplitTrim = false;
			if(Numi > Numj)
			{
				MemPartArr[i] = jMemPart;
				MemPartArr[j] = iMemPart;
			}
		}
	}

	m_Section.ResetPolyline();
	m_DrawSection.ResetPolyline();

	bool bRval = false;

	if(m_HasLevelDifG)
		bSplitTrim = false;

	if(bSplitTrim)
	{
		for(long i = 0 ; i < NumMemPartArr ; i++)
		{
			GM2DPolyline Section;
			GM2DPolyline DrawSection;
			CTypedPtrArray<CObArray, MSLinkMemPart*>* pMemPartArr = MemPartArr[i];

			if(MSFloor::ms_TrimType == 0)
				bRval = TrimSection(Section, DrawSection, *pMemPartArr, true);
			else if(MSFloor::ms_TrimType == 1) 
				bRval = TrimSection2(Section, DrawSection, *pMemPartArr, true);
			else if(MSFloor::ms_TrimType == 2) 
				bRval = TrimSection3(Section, DrawSection, *pMemPartArr);
			else if(MSFloor::ms_TrimType == 3)
				bRval = TrimSection4(Section, DrawSection, *pMemPartArr);

			ReSetMemPartVec(*pMemPartArr);

//  			Section.MergeVectorArr(1);
//  			DrawSection.MergeVectorArr(1);
// 			Section.CheckVecterArr(true, 1);
// 			DrawSection.CheckVecterArr(true, 1);
			
			Section.CheckVecterArr(true, 1);
			DrawSection.MergeVectorArr(1);
			

			if(m_Section.GetNumLines() <= 2)
				m_Section.CopyFrom(&Section);
			else if(Section.GetNumLines() > 2)
			{
				CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
				m_Section.BoolOperation('U', &Section, JoinedPolys, DTOL_GM);
				long NumJoined = JoinedPolys.GetSize();
				if(NumJoined >= 1)
				{
					m_Section.CopyFrom(JoinedPolys[0]);
				}
				else
					ASSERT(0);

				for(long i = 0 ; i < NumJoined ; i++)
					delete JoinedPolys[i];
				JoinedPolys.RemoveAll();
			}

			if(m_DrawSection.GetNumLines() <= 2)
				m_DrawSection.CopyFrom(&DrawSection);
			else if(DrawSection.GetNumLines() > 2)
			{
				CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
				m_DrawSection.BoolOperation('U', &DrawSection, JoinedPolys, DTOL_GM);
				long NumJoined = JoinedPolys.GetSize();
				if(NumJoined >= 1)
				{
					m_DrawSection.CopyFrom(JoinedPolys[0]);
				}
				else
					ASSERT(0);

				for(long i = 0 ; i < NumJoined ; i++)
					delete JoinedPolys[i];
				JoinedPolys.RemoveAll();
			}

			if(NumMemPartArr > 1)
			{
				long NumPart = pMemPartArr->GetSize();
				for(long iPart = 0 ; iPart < NumPart ; iPart++)
				{
					MSLinkMemPart* pPart = pMemPartArr->GetAt(iPart);
					if(pPart->isBuriedLinkMember())
						continue;
					long StartOrEnd = pPart->GetSConn()==mp_Conn ? 0 : 1;
					if(MSFloor::ms_DrawHitJoint == true)
					{
						m_Section.Trim(pPart->mp_LLine, StartOrEnd, false);
						m_Section.Trim(pPart->mp_RLine, StartOrEnd, false);
					}
					else
					{
						m_DrawSection.Trim(pPart->mp_LLine, StartOrEnd, false);
						m_DrawSection.Trim(pPart->mp_RLine, StartOrEnd, false);
					}
				}
			}

			delete MemPartArr[i];
		}
		MemPartArr.RemoveAll();

// 		if(bRval && ma_MemPart.GetSize() > 1 /*&& (m_ColinerMemMap.size() > 1 || m_hasBuriedLinkMember)*/)
// 			return MakeMemPartEndJointPoly2(m_Section, ma_MemPart);

//  		m_Section.MergeVectorArr(1);
//  		m_DrawSection.MergeVectorArr(1);
// 		m_Section.CheckVecterArr(true, 1);
// 		m_DrawSection.CheckVecterArr(true, 1);
		
		m_Section.CheckVecterArr(true, 1);
		m_DrawSection.MergeVectorArr(1);
		return bRval;
	}
	else
	{
		if(MSFloor::ms_TrimType == 0)
			bRval = TrimSection(m_Section, m_DrawSection, ma_MemPart, true);
		else if(MSFloor::ms_TrimType == 1) 
			bRval = TrimSection2(m_Section, m_DrawSection, ma_MemPart, true);
		else if(MSFloor::ms_TrimType == 2) 
			bRval = TrimSection3(m_Section, m_DrawSection, ma_MemPart);
		else if(MSFloor::ms_TrimType == 3)
			bRval = TrimSection4(m_Section, m_DrawSection, ma_MemPart);
		ReSetMemPartVec(ma_MemPart);

// 		if(bRval && ma_MemPart.GetSize() > 1 /*&& (m_ColinerMemMap.size() > 1 || m_hasBuriedLinkMember)*/)
// 			return MakeMemPartEndJointPoly2(m_Section, ma_MemPart);

// 		m_Section.MergeVectorArr(1);
// 		m_DrawSection.MergeVectorArr(1);
// 		m_Section.CheckVecterArr(true, 1);
//		m_DrawSection.CheckVecterArr(true, 1);
		
		m_Section.CheckVecterArr(true, 1);
		m_DrawSection.MergeVectorArr(1);

		//LevelDifG일 경우
		long NumPart = ma_MemPart.GetSize();
		long nOnEdgeLevelDifGCnt = 0;
		bool HasOnEdgeLevelDifG = false;
		for(long iPart = 0 ; iPart < NumPart ; iPart++)
		{
			MSLinkMemPart* pPart = ma_MemPart[iPart];
			HasOnEdgeLevelDifG = ((MSLineLevelDifG*)pPart->mp_LinkMem)->IsOnEdgeLevelDifG();
			if(HasOnEdgeLevelDifG == true)
				nOnEdgeLevelDifGCnt++;
		}
		if(nOnEdgeLevelDifGCnt > 0 && nOnEdgeLevelDifGCnt != NumPart)
		{
			for(long iPart = 0 ; iPart < NumPart ; iPart++)
			{
				MSLinkMemPart* pPart = ma_MemPart[iPart];
				bool isLongExtend = true;
				if(((MSLineLevelDifG*)pPart->mp_LinkMem)->IsOnEdgeLevelDifG())
					isLongExtend = true;
				else
					isLongExtend = false;

				if(pPart->mp_LLine->Length() > pPart->mp_RLine->Length()) // 왼쪽이 길다.
				{
					if(isLongExtend == true)
					{
						GM2DVector SVec = pPart->mp_RLine->OrthoBaseVector(pPart->mp_LLine->m_P0);
						GM2DVector EVec = pPart->mp_RLine->OrthoBaseVector(pPart->mp_LLine->m_P1);
						pPart->mp_RLine->SetP0(SVec);
						pPart->mp_RLine->SetP1(EVec);
					}
					else
					{
						GM2DVector SVec = pPart->mp_LLine->OrthoBaseVector(pPart->mp_RLine->m_P0);
						GM2DVector EVec = pPart->mp_LLine->OrthoBaseVector(pPart->mp_RLine->m_P1);
						pPart->mp_LLine->SetP0(SVec);
						pPart->mp_LLine->SetP1(EVec);
					}
				}
				else // 오른쪽이 길다.
				{
					if(isLongExtend == true)
					{
						GM2DVector SVec = pPart->mp_LLine->OrthoBaseVector(pPart->mp_RLine->m_P0);
						GM2DVector EVec = pPart->mp_LLine->OrthoBaseVector(pPart->mp_RLine->m_P1);
						pPart->mp_LLine->SetP0(SVec);
						pPart->mp_LLine->SetP1(EVec);
					}
					else
					{
						GM2DVector SVec = pPart->mp_RLine->OrthoBaseVector(pPart->mp_LLine->m_P0);
						GM2DVector EVec = pPart->mp_RLine->OrthoBaseVector(pPart->mp_LLine->m_P1);
						pPart->mp_RLine->SetP0(SVec);
						pPart->mp_RLine->SetP1(EVec);						
					}
				}

				if(isLongExtend)
				{
					long StartOrEnd = pPart->GetSConn()==mp_Conn ? 0 : 1;
					GM2DPolyline* pEndPoly = new GM2DPolyline(TRUE);
					GM2DLineVector EndLine = pPart->GetEndLine(StartOrEnd, FALSE);

					MSLineLevelDifG* pDifG = (MSLineLevelDifG*)pPart->mp_LinkMem;
					if(pDifG->IsOnEdgeLevelDifG())
					{
						pEndPoly->ResetPolyline();
						pEndPoly->AddLine(EndLine);
						pPart->AddEndJointPoly(this, StartOrEnd, pEndPoly);
					}
					continue;

				}
			}
		}

		return bRval;
	}
// 	if(MSFloor::ms_TrimType == 0)
// 		return TrimSection(m_Section, ma_MemPart, true);
// 	else if(MSFloor::ms_TrimType == 1) 
// 		return TrimSection2(m_Section, ma_MemPart, true);
// 	else if(MSFloor::ms_TrimType == 2) 
//         return TrimSection3(m_Section, ma_MemPart);
//     else if(MSFloor::ms_TrimType == 3)
//         return TrimSection4(m_Section, ma_MemPart);
}

void MSConnLevelZone::ReSetMemPartVec(CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
	if(MSFloor::ms_FixLineMemVec == false)
		return;

	double dMinWidth = GetMemPartByMinWidth();

	long NumMemPart = MemPartArr.GetSize();
	for(long iPart = 0 ; iPart < NumMemPart ; iPart++)
	{
		MSLinkMemPart* pPart = MemPartArr[iPart];
		MSLinkMemberG* pLinkMemberG = pPart->mp_LinkMem;
		long StartOrEnd = -1;
// 		if(mp_Conn->IsSameOrUpDown(pLinkMemberG->GetSConn()))
// 			StartOrEnd = 0;
// 		else if(mp_Conn->IsSameOrUpDown(pLinkMemberG->GetEConn()))
// 			StartOrEnd = 1;

		GM2DVector TheVec;
		GM2DVector OrgVec;
		if(StartOrEnd == 0)
		{
			//GM2DCurve* pCurve = pPart->GetShortLRLine(StartOrEnd);
			//OrgVec = pPart->mp_CenLine->OrthoBaseVector(pCurve->m_P0);
			TheVec = pPart->mp_CenLine->m_P0;
			OrgVec = *pLinkMemberG->GetSVec();
			
			double dDist = TheVec.Distance(OrgVec) - dMinWidth/2;
			if(dDist > MSFloor::ms_DefLineHitDistTol)
				pLinkMemberG->ma_Vec[0]->SetXY(TheVec.m_X, TheVec.m_Y);
		}
		else if(StartOrEnd == 1)
		{
			//GM2DCurve* pCurve = pPart->GetShortLRLine(StartOrEnd);
			//OrgVec = pPart->mp_CenLine->OrthoBaseVector(pCurve->m_P1);
			TheVec = pPart->mp_CenLine->m_P1;
			OrgVec = *pLinkMemberG->GetEVec();

			double dDist = TheVec.Distance(OrgVec) - dMinWidth/2;
			if(dDist > MSFloor::ms_DefLineHitDistTol)
				pLinkMemberG->ma_Vec[1]->SetXY(TheVec.m_X, TheVec.m_Y);
		}
	}

}

bool MSConnLevelZone::BuildSectionByDownJoint()
{
	long nLineMemPart = mp_Conn->ma_MemPart.size();
	long nMyColinearArcCnt = 0;
	long nMakeDownMemCnt = 0;
	long nDownArcCnt = 0;
	vector<MSConnectivity*> DownJointArr = GetDownLinkJoint();
	long NumDownLinkJoint = DownJointArr.size();
	if(NumDownLinkJoint > 0)
		DeleteDownLinkJointPoly();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{

		MSConnectivity* pDownJoint = DownJointArr[iJoint];
		if(pDownJoint != NULL)
		{
			bool isMidJoint = pDownJoint->IsMidConn();

			if(mp_Column == NULL)
			{
				if(pDownJoint->mp_Column != NULL && CheckLevelOverlap(pDownJoint->mp_Column))
					mp_Column = pDownJoint->mp_Column;
			}
			if(mp_IsoFoot == NULL)
			{
				if(pDownJoint->mp_IsoFoot != NULL && CheckLevelOverlap(pDownJoint->mp_IsoFoot))
					mp_IsoFoot = pDownJoint->mp_IsoFoot;
			}

			// 현재 Joint의 부재가 1개 이고 DownJoint의 중 1개라도 평행 한것이 있으면 생성하지 않는다 
			//long NumCurPart = ma_MemPart.GetCount();
			long NumCurMemberG = GetNumMemberG();
			long NumDownPart = pDownJoint->GetNumMemPart();

			long NumDownMemberG = pDownJoint->GetNumMemberG();

			/*
			if(NumCurMemberG == 1 )
			{
			MSLinkMemPart* pCurPart = ma_MemPart[0];
			long MyEndNum = 0;
			if(mp_Conn->IsSameOrUpDown(pCurPart->GetEConn()))
			MyEndNum = 1;
			else
			MyEndNum = 0;

			double MyDegree = pCurPart->GetDegree(MyEndNum);

			for(long iDownPart = 0 ; iDownPart < NumDownPart ; iDownPart++)
			{
			MSLinkMemPart* pDownPart = pDownJoint->ma_MemPart[iDownPart];
			long UrEndNum = 0;
			if(mp_Conn->IsSameOrUpDown(pDownPart->GetEConn()))
			UrEndNum = 1;
			else
			UrEndNum = 0;

			double UrDegree = pDownPart->GetDegree(UrEndNum);
			double InnerDegree = fabs(UrDegree - MyDegree);

			//if( ( InnerDegree <= MSFloor::ms_ColinearDegreeTol )  || (fabs( InnerDegree -180.) <= MSFloor::ms_ColinearDegreeTol ) )
			if( ( InnerDegree <= MSFloor::ms_MaxInnerDegreeTol )  || (fabs( InnerDegree -180.) <= MSFloor::ms_MaxInnerDegreeTol ) )
			{
			bUsedDownLinkJoint = FALSE;
			return TrimSection(m_Section, ma_MemPart, true);
			}
			}
			}
			*/

			MSPointMemberG* pPointMem=NULL;
			if(mp_Column)
				pPointMem = mp_Column;
			if(mp_IsoFoot)
				pPointMem = mp_IsoFoot;


			if(pPointMem != NULL)
			{
				GM2DPolyline PolyLine;
				pPointMem->GetBoundary(PolyLine);

				mp_DownLinkJointBoundry = new GM2DPolyline(TRUE);
				mp_DownLinkJointBoundry->CopyFrom(&PolyLine);
			}

			CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
			bool UseJointPoly = false;
			long NumDownMemPart = pDownJoint->ma_MemPart.size();
			if(NumDownMemPart > 1 && ! pDownJoint->HasBaseSec())
			{
				GM2DPolyline* pPoly = pDownJoint->GetSection_Hit();
				if(pPoly!= NULL && pPoly->CheckPolygon())
				{
					mp_DownLinkJointBoundry = new GM2DPolyline(TRUE);
					mp_DownLinkJointBoundry->CopyFrom(pPoly);
					UseJointPoly = true;
				}
			}
			
			double dMaxWidth = mp_Conn->GetMaxWidthByLineMemberG();

			m_hasColinearDownMemPart = false;
			for(long iDownPart = 0 ; iDownPart < NumDownMemPart ; iDownPart++)
			{
				MSLinkMemPart* pDownPart = pDownJoint->ma_MemPart[iDownPart];
				if(CheckLevelOverlap(pDownPart->mp_LinkMem))
				{
					bool isContinue = false;

					long UrEndNum = 0;
					if(mp_Conn->IsSameOrUpDown(pDownPart->GetEConn()))
						UrEndNum = 1;
					else
						UrEndNum = 0;

					double UrDegree = pDownPart->GetDegree(UrEndNum);

					//GM2DPolyline* pDownBoundry = pDownPart->GetBoundary(FALSE);
					GM2DPolyline* pDownBoundry = new GM2DPolyline(TRUE);
					pDownPart->mp_LinkMem->GetBoundary(*pDownBoundry);

					for(long iMemPart=0; iMemPart<nLineMemPart; iMemPart++)
					{
						MSLinkMemPart* pCurPart = mp_Conn->ma_MemPart[iMemPart];

						long MyEndNum = 0;
						if(mp_Conn->IsSameOrUpDown(pCurPart->GetEConn()))
							MyEndNum = 1;
						else
							MyEndNum = 0;

						double MyDegree = pCurPart->GetDegree(MyEndNum);

						// 벽과 보의 수평 판단.
						double InnerDegree = fabs(UrDegree - MyDegree);
						if(InnerDegree > 180.0) // 예각으로 계산한다.(180 넘으면 예각을 가지고옴)
							InnerDegree = 360.0 - fabs(InnerDegree);

						if(( fabs(InnerDegree) <= MSFloor::ms_ColinearDegreeTol )  || (fabs( InnerDegree -180.) <= MSFloor::ms_ColinearDegreeTol ) )
						{	
							GM2DVector ChkVec;
							double dLenDist = 0;
							double dCenLength = pCurPart->mp_CenLine->Length();
							if(dCenLength < dMaxWidth)
								dLenDist = (dMaxWidth/2) / dCenLength;
							else
								dLenDist = dMaxWidth / dCenLength;

							if(MyEndNum == 0)
								ChkVec = pCurPart->mp_CenLine->PointAtParam(dLenDist);
							else if(MyEndNum == 1)
								ChkVec = pCurPart->mp_CenLine->PointAtParam(1-dLenDist);
							
// 							GM2DLineVector TheLine;
// 							TheLine.m_P0 = pCurPart->mp_LLine->OrthoBaseVector(ChkVec);
// 							TheLine.m_P1 = pCurPart->mp_RLine->OrthoBaseVector(ChkVec);
							
							GM2DPolyline* pCurPoly = pCurPart->GetBoundary();
							bool bOverLap = pDownBoundry->OverlapEx(*pCurPoly, pDownPart->mp_LinkMem->GetWidth()/2 -1);
							if(bOverLap)
							{
								isContinue = true;
								m_hasColinearDownMemPart = true;

								if(pCurPart->mp_LinkMem->isArc())
									nMyColinearArcCnt++;
								break;
							}
						}
					}

					if(isContinue == true) 
					{
						delete pDownBoundry;
						continue;
					}

					if(pDownPart->mp_LinkMem->isArc())
						nDownArcCnt++;

					

					if(mp_DownLinkJointBoundry == NULL)
					{
						mp_DownLinkJointBoundry = new GM2DPolyline(TRUE);
						mp_DownLinkJointBoundry->CopyFrom(pDownBoundry);
						nMakeDownMemCnt++;
						delete pDownBoundry;
						continue;
					}

					mp_DownLinkJointBoundry->BoolOperation('U', pDownBoundry, JoinedPolys, 1);
					long NumJoindPoly = JoinedPolys.GetSize();
					if(NumJoindPoly != 1)
					{
						for(long iPoly = 0 ; iPoly < NumJoindPoly ; iPoly++)
							delete JoinedPolys[iPoly];
						JoinedPolys.RemoveAll();

						mp_DownLinkJointBoundry->BoolOperation('R', pDownBoundry, JoinedPolys, 1);
						NumJoindPoly = JoinedPolys.GetSize();
					}

					if(NumJoindPoly == 1)
					{
						DeleteDownLinkJointPoly();
						mp_DownLinkJointBoundry = new GM2DPolyline(TRUE);
						mp_DownLinkJointBoundry->CopyFrom(JoinedPolys[0]);
					}

					delete pDownBoundry;

					for(long iPoly = 0 ; iPoly < NumJoindPoly ; iPoly++)
						delete JoinedPolys[iPoly];
					JoinedPolys.RemoveAll();

					mp_DownLinkJointBoundry->MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5

					nMakeDownMemCnt++;
					/*
					MSConnectivity* pOthderJoint = NULL;
					if(UrEndNum == 0)
					pOthderJoint = pDownPart->GetEConn();
					else
					pOthderJoint = pDownPart->GetSConn();
					if(pOthderJoint != NULL)
					{
					GM2DPolyline* pOtherBoundry = pOthderJoint->GetBottomSection();
					if(pOtherBoundry != NULL)
					{
					mp_DownLinkJointBoundry->BoolOperation('U', pOtherBoundry, JoinedPolys, 1);
					NumJoindPoly = JoinedPolys.GetSize();
					if(NumJoindPoly != 1)
					{
					for(long iPoly = 0 ; iPoly < NumJoindPoly ; iPoly++)
					delete JoinedPolys[iPoly];
					JoinedPolys.RemoveAll();

					mp_DownLinkJointBoundry->BoolOperation('R', pOtherBoundry, JoinedPolys, 1);
					NumJoindPoly = JoinedPolys.GetSize();
					}

					if(NumJoindPoly == 1)
					{
					DeleteDownLinkJointPoly();
					mp_DownLinkJointBoundry = new GM2DPolyline(TRUE);
					mp_DownLinkJointBoundry->CopyFrom(JoinedPolys[0]);
					}

					for(long iPoly = 0 ; iPoly < NumJoindPoly ; iPoly++)
					delete JoinedPolys[iPoly];
					JoinedPolys.RemoveAll();
					}
					}
					*/
				}
			}

			if(NumCurMemberG == 1 )
			{
				/*
				MSLinkMemPart* pCurPart = ma_MemPart[0];
				if(mp_DownLinkJointBoundry != NULL)
				{
				long nSVal = mp_DownLinkJointBoundry->Contains(pCurPart->mp_LinkMem->ma_Vec[0], 1);
				long nEVal = mp_DownLinkJointBoundry->Contains(pCurPart->mp_LinkMem->ma_Vec[1], 1);
				if(nSVal < 1 && nEVal < 1)
				{
				if(mp_DownLinkJointBoundry != NULL)
				delete mp_DownLinkJointBoundry;
				mp_DownLinkJointBoundry = NULL;
				bUsedDownLinkJoint = FALSE;
				}
				}
				if(mp_DownLinkJointBoundry != NULL)
				{
				long nSVal = mp_DownLinkJointBoundry->Contains(&pCurPart->mp_LLine->m_P0, 1);
				long nEVal = mp_DownLinkJointBoundry->Contains(&pCurPart->mp_LLine->m_P1, 1);
				if(nSVal < 1 && nEVal < 1)
				{
				if(mp_DownLinkJointBoundry != NULL)
				delete mp_DownLinkJointBoundry;
				mp_DownLinkJointBoundry = NULL;
				bUsedDownLinkJoint = FALSE;
				}
				}
				if(mp_DownLinkJointBoundry != NULL)
				{
				long nSVal = mp_DownLinkJointBoundry->Contains(&pCurPart->mp_RLine->m_P0, 1);
				long nEVal = mp_DownLinkJointBoundry->Contains(&pCurPart->mp_RLine->m_P1, 1);
				if(nSVal < 1 && nEVal < 1)
				{
				if(mp_DownLinkJointBoundry != NULL)
				delete mp_DownLinkJointBoundry;
				mp_DownLinkJointBoundry = NULL;
				bUsedDownLinkJoint = FALSE;
				}

				}
				*/
			}

			if( (nMyColinearArcCnt > 0 && nDownArcCnt > 0 ) ||
				(NumDownMemPart == nMyColinearArcCnt) ||
				nMakeDownMemCnt == 0)
			{
				MSPointMemberG* pPointMem=NULL;
				if(mp_Column)
					pPointMem = mp_Column;
				if(mp_IsoFoot)
					pPointMem = mp_IsoFoot;

				if(pPointMem != NULL)
				{
					DeleteDownLinkJointPoly();
					GM2DPolyline PolyLine;
					pPointMem->GetBoundary(PolyLine);

					mp_DownLinkJointBoundry = new GM2DPolyline(TRUE);
					mp_DownLinkJointBoundry->CopyFrom(&PolyLine);
				}
			}

		}
	}
	return true;
}

GM2DPolyline* MSConnLevelZone::GetSection()
{
    //if(m_Section.GetSize() == 0)
        //BuildSection();
    if(m_Section.GetSize() > 0)
        return &m_Section;
    else 
        return NULL;
}

GM2DPolyline* MSConnLevelZone::GetDrawSection()
{
	//if(m_Section.GetSize() == 0)
	//BuildSection();
	if(m_DrawSection.GetSize() > 0)
		return &m_DrawSection;
	else 
		return NULL;
}

bool MSConnLevelZone::CheckLevelOverlap(MSCompoG* pCompoG)
{
    double MyTopLevel, MyBotLevel;
    //pCompoG->GetZLevels(MyTopLevel, MyBotLevel);
	//pCompoG->GetZLevelsbyJoint(MyTopLevel, MyBotLevel, mp_Conn);

    if((MyTopLevel <= m_BotLevel + DTOL1) || (m_TopLevel <= MyBotLevel + DTOL1))
        return false;
    return true;
}

void MSConnLevelZone::TrimMemPartSegment(vector<GM2DCurve*>& PartSegments, GM2DVector MyCrossVec, MSLinkMemPart* pMyMemPart, MSLinkMemPart* pUrMemPart)
{
    GM2DPolyline* pUrPoly = pUrMemPart->GetTrimmedBoundary();

    // check cross vector valid
	//GM2DVector OrgVec;
	//GM2DVector JointCenVec;
	//if(OrgVec.SameVector4DTOL(MyCrossVec))
	//	JointCenVec.SetXY(mp_Conn->m_dXc, mp_Conn->m_dYc);
	//else
	//	JointCenVec = MyCrossVec;

	vector<GM2DCurve*> Segments;
	
    long NumLSeg = PartSegments.size();
	for(long iSeg = 0 ; iSeg < NumLSeg ; iSeg++)
	{
		GM2DCurve* pCurve = PartSegments[iSeg];
		
        CTypedPtrArray<CObArray, GM2DCurve*> MySegments;
        vector<GM2DCurve*> MySegments2;

        // MySegments get divided segments with UrMemPart Boundary
		pCurve->GetSegments(MySegments, pUrPoly);

        // Segments2 gathers segments outside of UrMemPart Boundary temporarily in loop
		long NumMySeg = MySegments.GetSize();
		for(long i = 0; i < NumMySeg ; i++)
		{
			GM2DCurve* pSeg = MySegments[i];
			GM2DVector MidVec = pSeg->PointAtParam(0.5);

			long nRval = pUrPoly->Contains(&MidVec, 1);

			if(nRval > 0)
				MySegments2.push_back(pSeg);
			else
				delete pSeg;
		}
		delete PartSegments[iSeg];
        
		long NumSeg = MySegments2.size();
		if(NumSeg == 2)
		{
            // in case of need to delete an edge triangle
            FilletInsideSegment(MySegments2, pUrMemPart->mp_LinkMem);
            FilletTriangularProtrusion(MySegments2, pMyMemPart->mp_LinkMem, pUrPoly);
		}

        // Segments gathers newly trimmed segments
		NumSeg = MySegments2.size();
		for(long i = 0 ; i < NumSeg ; i++)
			Segments.push_back(MySegments2[i]);
	}

    // return Trimmed Segments after loop
	PartSegments.clear();
	PartSegments = Segments;
}

void MSConnLevelZone::FilletInsideSegment( vector<GM2DCurve*>& Segments, MSLinkMemberG* pUrLinkMem )
{
    vector<long> index;
    long NumSeg = Segments.size();
    for (long iSeg=0; iSeg<NumSeg; iSeg++)
    {
        GM2DCurve* pSeg = Segments[iSeg];
        GM2DVector SVec = pSeg->GetP0();
        GM2DVector EVec = pSeg->GetP1();

        // in case of which the segment is included in the extended UrLinkMember
        if (((pUrLinkMem->mp_LLine->WhichSide(SVec))*(pUrLinkMem->mp_RLine->WhichSide(SVec)) == -1)
            ||((pUrLinkMem->mp_LLine->WhichSide(EVec))*(pUrLinkMem->mp_RLine->WhichSide(EVec)) == -1))
        {
            index.push_back(iSeg);
            continue;
        }
    }

    // check index and delete segment
    long count = index.size();
    if (count>0)
        DeleteSegmentByIndex(Segments, index);
}

void MSConnLevelZone::FilletTriangularProtrusion( vector<GM2DCurve*>& Segments, MSLinkMemberG* pLinkMem, GM2DPolyline* pUrPoly)
{
    // get end points of the link member
    vector<GM2DVector> endVecs;
    endVecs.push_back(pLinkMem->mp_LLine->GetP0());
    endVecs.push_back(pLinkMem->mp_LLine->GetP1());
    endVecs.push_back(pLinkMem->mp_RLine->GetP1());
    endVecs.push_back(pLinkMem->mp_RLine->GetP0());

    vector<long> index;
    long NumSeg = Segments.size();
    for (long iSeg=0; iSeg<NumSeg; iSeg++)
    {
        GM2DCurve* pSeg = Segments[iSeg];
        GM2DVector SVec = pSeg->GetP0();
        GM2DVector EVec = pSeg->GetP1();

        // in case of which the segment makes a triangular protrusion
        long result = CheckEndVectors(SVec, endVecs);
        if ((result > -1)&&(pUrPoly->Contains(&endVecs[3-result])<1))
        {
            index.push_back(iSeg);
            continue;
        }
        result = CheckEndVectors(EVec, endVecs);
        if ((result > -1)&&(pUrPoly->Contains(&endVecs[3-result])<1))
        {
            index.push_back(iSeg);
            continue;
        }
    }

    // check index and delete segment
    long count = index.size();
    if (count>0)
        DeleteSegmentByIndex(Segments, index);
}

void MSConnLevelZone::DeleteSegmentByIndex( vector<GM2DCurve*>& Segments, vector<long> index)
{
    // check index and delete segment
    long count = index.size();
    long NumSeg = Segments.size();

    if (count>0)
    {
        long i=0;
        vector<GM2DCurve*> NewSegments;
        for (long iSeg=0; iSeg<NumSeg; iSeg++)
        {
            GM2DCurve* pSeg = Segments[iSeg];
            if((i<count)&&(iSeg == index[i]))
            {
                delete pSeg;
                i++;
            }
            else
                NewSegments.push_back(pSeg->Copy());
        }
        Segments.clear();
        Segments = NewSegments;
    }
}


long MSConnLevelZone::CheckEndVectors( GM2DVector Vec, vector<GM2DVector> endVecs )
{
    long NumVecs = endVecs.size();
    for(long iVec = 0 ; iVec < NumVecs ;iVec++)
        if (Vec.SameVector4DTOL(endVecs[iVec], 1))
            return iVec;

    return -1;
}

bool SegStructCompare(SegStruct &a, SegStruct &b)
{
	return a.dDeg < b.dDeg;
}

bool MSConnLevelZone::TrimSection4( GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
    long NumMemPart = MemPartArr.GetSize();

    // initialize trimmed polyline
    for(long iPart = 0 ; iPart < NumMemPart ;iPart++)
    {
        MSLinkMemPart* pMyMemPart = MemPartArr[iPart];

        // SetSegments (for initial condition)
        pMyMemPart->SetSegments();

        // make a closed trimmed polyline with segments
        //pMyMemPart->MakeTrimmedPolyline();
    }

    if (NumMemPart>1)
    {
        GM2DVector Center = CalculateSegmentList(MemPartArr);
        SortSegments(Center);

        m_DrawSection.ResetPolyline();
        m_Section.ResetPolyline();

        long NumSegList = ma_SegList.size();
        for(long iSeg = 0 ; iSeg < NumSegList ; iSeg++)
        {
            SegStruct MySegData = ma_SegList[iSeg];
            SegStruct UrSegData = ma_SegList[(iSeg+1)%NumSegList];

            if(MySegData.MemPartIdx == UrSegData.MemPartIdx)
            {
                if(MySegData.isEndLine||UrSegData.isEndLine)
                    continue;
                else if (MySegData.isLLine != UrSegData.isLLine)
                    continue;
            }

            // in case of colinear
            long nCLVal = MySegData.pCurve->IsColinear4DTol(UrSegData.pCurve);
            if(nCLVal != 0)
            {
                MakeConnProfileWhenCoLinear(&MySegData, &UrSegData, MemPartArr);
            }
            else // in case of parallel
            {
                long nPRVal = MySegData.pCurve->IsParallel4DTol(UrSegData.pCurve);
                if (nPRVal !=0)
                    MakeConnProfileWhenParallel(&MySegData, &UrSegData, Center, MemPartArr);
            }

            // intersect (not parallel/not colinear)
            MakeConnProfileWhenCross(&MySegData, &UrSegData);
        }

        m_DrawSection.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5
        m_Section.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5

    }

    for(long iPart = 0 ; iPart < NumMemPart ;iPart++)
    {
        MSLinkMemPart* pMyMemPart = MemPartArr[iPart];

        pMyMemPart->mp_LPolyLine->AddSegments(pMyMemPart->ma_LSegments);
        pMyMemPart->mp_RPolyLine->AddSegments(pMyMemPart->ma_RSegments);

        if (pMyMemPart->IsStart())
            FilterEndLines(pMyMemPart, true);
        
        if (pMyMemPart->IsEnd())
            FilterEndLines(pMyMemPart, false);

        //if(pMyMemPart->ma_SSegments.size() < 1)
        //    pMyMemPart->mp_SPolyLine->ResetPolyline();
        //else
        //    pMyMemPart->mp_SPolyLine->AddSegments(pMyMemPart->ma_SSegments, true);
        //
        //if(pMyMemPart->ma_ESegments.size() < 1)
        //    pMyMemPart->mp_EPolyLine->ResetPolyline();
        //else
        //    pMyMemPart->mp_EPolyLine->AddSegments(pMyMemPart->ma_ESegments, true);
    }

    return true;
}

bool MSConnLevelZone::TrimSection3( GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
	long NumMemPart = MemPartArr.GetSize();
	vector<GM2DVector> CrossVectors;

    // initialize trimmed polyline
    for(long iPart = 0 ; iPart < NumMemPart ;iPart++)
    {
        MSLinkMemPart* pMyMemPart = MemPartArr[iPart];

        // check to initialize trimmed polyline by part default boundary
        // for LSegments
        if(pMyMemPart->ma_LSegments.size() == 0)
            pMyMemPart->ma_LSegments.push_back(pMyMemPart->mp_LLine->Copy());
        // for RSegments
        if(pMyMemPart->ma_RSegments.size() == 0)
            pMyMemPart->ma_RSegments.push_back(pMyMemPart->mp_RLine->Copy());
        //// for ESegments
        //if(pMyMemPart->ma_ESegments.size() == 0)
        //{
        //    pMyMemPart->ma_ESegments.push_back(new GM2DLineVector(pMyMemPart->mp_LLine->m_P0, pMyMemPart->mp_RLine->m_P0));
        //    pMyMemPart->ma_ESegments.push_back(new GM2DLineVector(pMyMemPart->mp_LLine->m_P1, pMyMemPart->mp_RLine->m_P1));
        //}

        // make a closed trimmed polyline with segments
//        pMyMemPart->MakeTrimmedPolyline();
    }

    if (NumMemPart>1)
    {
        for(long iPart = 0 ; iPart < NumMemPart ;iPart++)
        {
            MSLinkMemPart* pMyMemPart = MemPartArr[iPart];
            GM2DCurve* pMyCenCurve =  pMyMemPart->mp_CenLine;

            for(long jPart = 0 ; jPart < NumMemPart ;jPart++)
            {
                if(iPart == jPart) continue;
                MSLinkMemPart* pUrMemPart = MemPartArr[jPart];
                if(pMyMemPart->mp_LinkMem == pUrMemPart->mp_LinkMem) continue;

                GM2DCurve* pUrCenCurve =  pUrMemPart->mp_CenLine;

                vector<GM2DVector> CrossVecs;
                vector<double> MyLocArr, UrLocArr;
                vector<long> ResutlArr;
                long ResultIdx = 0;
                GM2DVector MyCrossVec;

                long nRval = pMyCenCurve->IntersectEnd2(pUrCenCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true);
                if(abs(nRval%10)==1)
                {
                    for(long i = 0 ; i < ResutlArr.size() ; i++)
                    {
                        if(ResutlArr[i] == 1)
                        {
                            ResultIdx = i;
                            break;
                        }
                    }

                    MyCrossVec = CrossVecs[ResultIdx];

                    if(pMyMemPart->mp_LinkMem != pUrMemPart->mp_LinkMem)
                    {
                        long NumCross = CrossVectors.size();
                        bool exist = false;
                        for(long iNum = 0 ; iNum < NumCross ; iNum++)
                        {
                            if (MyCrossVec.SameVector4DTOL(CrossVectors[iNum]))
                            {
                                exist = true;
                                break;
                            }
                        }
                        if(!exist)
                            CrossVectors.push_back(MyCrossVec);
                    }
                }
                else if(abs(nRval%10)==2)
                {

                }

                TrimMemPartSegment(pMyMemPart->ma_LSegments, MyCrossVec, pMyMemPart, pUrMemPart);
                TrimMemPartSegment(pMyMemPart->ma_RSegments, MyCrossVec, pMyMemPart, pUrMemPart);
            }

            long NumLSeg = pMyMemPart->ma_LSegments.size();
            for(long iSeg = 0 ; iSeg < NumLSeg ; iSeg++)
            {
                GM2DCurve* pCurve = pMyMemPart->ma_LSegments[iSeg];
                SegStruct SegData;
                SegData.dDeg = 0.0;
                SegData.MemPartIdx = iPart;
                SegData.pCurve = pCurve;
                SegData.isLLine = true;
                ma_SegList.push_back(SegData);
            }

            long NumRSeg = pMyMemPart->ma_RSegments.size();
            for(long iSeg = 0 ; iSeg < NumRSeg ; iSeg++)
            {
                GM2DCurve* pCurve = pMyMemPart->ma_RSegments[iSeg];
                SegStruct SegData;
                SegData.dDeg = 0.0;
                SegData.MemPartIdx = iPart;
                SegData.pCurve = pCurve;
                SegData.isLLine = false;
                ma_SegList.push_back(SegData);
            }
        }

        GM2DVector Center;
        long NumVec = CrossVectors.size();
        if (NumVec>0)
        {
            for(long iVec = 0 ; iVec < NumVec ; iVec++)
            {
                Center += CrossVectors[iVec];
            }
            Center = Center * (1./NumVec);
        }
        else
            Center.SetXY(mp_Conn->m_dXc, mp_Conn->m_dYc);

        // get the angle from Center to MidPoint
        long NumSeg = ma_SegList.size();
        for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
        {
            SegStruct& SegData = ma_SegList[iSeg];
            GM2DCurve* pCurve = SegData.pCurve;
            double length = pCurve->Length();

            GM2DVector MidVec = pCurve->PointAtParam(0.5);
            if (length > 600)
            {
                double location = pCurve->OrthoBaseLocValue(Center);
                if (pCurve->m_ObjectType == "GM2DArc")
                {
                    if (location < 0.5)
                        MidVec = pCurve->PointAtParam(0.01);
                    else
                        MidVec = pCurve->PointAtParam(0.09);
                }
                else
                {
                    if (location < 0.5)
                        MidVec = pCurve->PointAtParam(0.2);
                    else
                        MidVec = pCurve->PointAtParam(0.8);
                }
            }
            SegData.dDeg = Center.GetDegree(MidVec);
        }
        // sort according to angles
        sort(ma_SegList.begin(), ma_SegList.end(), SegStructCompare);

        m_DrawSection.ResetPolyline();
        m_Section.ResetPolyline();

        long NumSegList = ma_SegList.size();
        for(long iSeg = 0 ; iSeg < NumSegList ; iSeg++)
        {
            SegStruct MySegData = ma_SegList[iSeg];
            SegStruct UrSegData = ma_SegList[(iSeg+1)%NumSegList];

            if(MySegData.MemPartIdx == UrSegData.MemPartIdx)
                if ((MySegData.isLLine != UrSegData.isLLine)
                    ||!(MySegData.pCurve->DirVector().SameVector4DTOL(UrSegData.pCurve->DirVector())))
                    continue;

            // in case of colinear
            long nCLVal = MySegData.pCurve->IsColinear4DTol(UrSegData.pCurve);
            if(nCLVal != 0)
            {
                double UrP0Loc = 0;
                double UrP1Loc = 0;
                long nOLVal = MySegData.pCurve->Overlap4DTolBase(UrSegData.pCurve, UrP0Loc, UrP1Loc);
                GM2DVector CrossVec;

                switch (nOLVal)
                {
                case 1: // UrSegment is over the start point of MySegment
                    if(UrP0Loc<UrP1Loc)
                        CrossVec = UrSegData.pCurve->GetP1();
                    else
                        CrossVec = UrSegData.pCurve->GetP0();
                    MySegData.pCurve->SetP0(CrossVec);
                    break;
                case 25: // UrSegment is over the end point of MySegment
                    if(UrP0Loc<UrP1Loc)
                        CrossVec = UrSegData.pCurve->GetP0();
                    else
                        CrossVec = UrSegData.pCurve->GetP1();
                    MySegData.pCurve->SetP1(CrossVec);
                    break;
                case 2:
                case 6:  // UrSegment meets MySegment at the start point of MySegment.
                    CrossVec = MySegData.pCurve->GetP0();
                    break;
                case 20:
                case 24: // UrSegment meets MySegment at the end point of MySegment.
                    CrossVec = MySegData.pCurve->GetP1();
                    break;
                }

                if(MySegData.MemPartIdx != UrSegData.MemPartIdx)
                {
                    // add point to Connection
                    if (MemPartArr[MySegData.MemPartIdx]->mp_LinkMem != MemPartArr[UrSegData.MemPartIdx]->mp_LinkMem)
                    {
                        m_DrawSection.AddCopy(CrossVec);
                        m_DrawSection.ma_Bulge.Add(0.);
                    }
                    m_Section.AddCopy(CrossVec);
                    m_Section.ma_Bulge.Add(0.);
                    continue;
                }
                continue;
            }
            else // in case of parallel
            {
                long nPRVal = MySegData.pCurve->IsParallel4DTol(UrSegData.pCurve);
                if (nPRVal !=0)
                {
                    double UrP0Loc = 0;
                    double UrP1Loc = 0;
                    long nOLVal = MySegData.pCurve->Overlap4DTolBase(UrSegData.pCurve, UrP0Loc, UrP1Loc);
                    GM2DVector CrossVec;
                    GM2DVector EndVec;

                    double Dist2Me = MySegData.pCurve->Distance(Center);
                    double Dist2U = UrSegData.pCurve->Distance(Center);
                    bool NearMe = true;
                    if (Dist2Me>Dist2U)
                        NearMe = false;

                    switch (nOLVal)
                    {
                    case 1: // UrSegment is over the start point of MySegment
                        if (NearMe)
                        {
                            if(UrSegData.isLLine)
                            {
                                if(UrP0Loc<UrP1Loc)
                                {
                                    EndVec = MemPartArr[UrSegData.MemPartIdx]->mp_LLine->GetP1();
                                    UrSegData.pCurve->SetP1(EndVec);
                                }
                                else
                                {
                                    EndVec = MemPartArr[UrSegData.MemPartIdx]->mp_LLine->GetP0();
                                    UrSegData.pCurve->SetP0(EndVec);
                                }
                            }
                            else
                            {
                                if(UrP0Loc<UrP1Loc)
                                {
                                    EndVec = MemPartArr[UrSegData.MemPartIdx]->mp_RLine->GetP1();
                                    UrSegData.pCurve->SetP1(EndVec);
                                }
                                else
                                {
                                    EndVec = MemPartArr[UrSegData.MemPartIdx]->mp_RLine->GetP0();
                                    UrSegData.pCurve->SetP0(EndVec);
                                }
                            }
                            double loc = MySegData.pCurve->OrthoBaseLocValue(EndVec);
                            CrossVec = MySegData.pCurve->PointAtParam(loc);
                            MySegData.pCurve->SetP0(CrossVec);
                        }
                        else
                        {
                            if(MySegData.isLLine)
                                EndVec = MemPartArr[MySegData.MemPartIdx]->mp_LLine->GetP0();
                            else
                                EndVec = MemPartArr[MySegData.MemPartIdx]->mp_RLine->GetP0();
                            MySegData.pCurve->SetP0(EndVec);
                            double loc = UrSegData.pCurve->OrthoBaseLocValue(EndVec);
                            CrossVec = UrSegData.pCurve->PointAtParam(loc);
                            if(loc<0.5)
                                UrSegData.pCurve->SetP0(CrossVec);
                            else
                                UrSegData.pCurve->SetP1(CrossVec);
                        }
                        break;

                    case 25: // UrSegment is over the end point of MySegment
                        if (NearMe)
                        {
                            if(UrSegData.isLLine)
                            {
                                if(UrP0Loc<UrP1Loc)
                                {
                                    EndVec = MemPartArr[UrSegData.MemPartIdx]->mp_LLine->GetP0();
                                    UrSegData.pCurve->SetP0(EndVec);
                                }
                                else
                                {
                                    EndVec = MemPartArr[UrSegData.MemPartIdx]->mp_LLine->GetP1();
                                    UrSegData.pCurve->SetP1(EndVec);
                                }
                            }
                            else
                            {
                                if(UrP0Loc<UrP1Loc)
                                {
                                    EndVec = MemPartArr[UrSegData.MemPartIdx]->mp_RLine->GetP0();
                                    UrSegData.pCurve->SetP0(EndVec);
                                }
                                else
                                {
                                    EndVec = MemPartArr[UrSegData.MemPartIdx]->mp_RLine->GetP1();
                                    UrSegData.pCurve->SetP1(EndVec);
                                }
                            }
                            double loc = MySegData.pCurve->OrthoBaseLocValue(EndVec);
                            CrossVec = MySegData.pCurve->PointAtParam(loc);
                            MySegData.pCurve->SetP1(CrossVec);
                        }
                        else
                        {
                            if(MySegData.isLLine)
                                EndVec = MemPartArr[MySegData.MemPartIdx]->mp_LLine->GetP1();
                            else
                                EndVec = MemPartArr[MySegData.MemPartIdx]->mp_RLine->GetP1();
                            MySegData.pCurve->SetP1(EndVec);
                            double loc = UrSegData.pCurve->OrthoBaseLocValue(EndVec);
                            CrossVec = UrSegData.pCurve->PointAtParam(loc);
                            if(loc<0.5)
                                UrSegData.pCurve->SetP0(CrossVec);
                            else
                                UrSegData.pCurve->SetP1(CrossVec);
                        }
                        break;

                    case 2:
                        if (NearMe)
                        {
                            CrossVec = MySegData.pCurve->GetP0();
                            EndVec = UrSegData.pCurve->GetP1();
                        }
                        else
                        {
                            CrossVec = UrSegData.pCurve->GetP1();
                            EndVec = MySegData.pCurve->GetP0();
                        }
                        break;

                    case 6:  // UrSegment meets MySegment at the start point of MySegment.
                        if (NearMe)
                        {
                            CrossVec = MySegData.pCurve->GetP0();
                            EndVec = UrSegData.pCurve->GetP0();
                        }
                        else
                        {
                            CrossVec = UrSegData.pCurve->GetP0();
                            EndVec = MySegData.pCurve->GetP0();
                        }
                        break;

                    case 20:
                        if (NearMe)
                        {
                            CrossVec = MySegData.pCurve->GetP1();
                            EndVec = UrSegData.pCurve->GetP0();
                        }
                        else
                        {
                            CrossVec = UrSegData.pCurve->GetP0();
                            EndVec = MySegData.pCurve->GetP1();
                        }
                        break;

                    case 24: // UrSegment meets MySegment at the end point of MySegment.
                        if (NearMe)
                        {
                            CrossVec = MySegData.pCurve->GetP1();
                            EndVec = UrSegData.pCurve->GetP1();
                        }
                        else
                        {
                            CrossVec = UrSegData.pCurve->GetP1();
                            EndVec = MySegData.pCurve->GetP1();
                        }
                        break;
                    }

                    GM2DCurve* pLine = new GM2DLineVector(CrossVec, EndVec);

                    if (NearMe)
                    {
                        if(UrSegData.isLLine)
                            MemPartArr[UrSegData.MemPartIdx]->ma_LSegments.push_back(pLine);
                        else
                            MemPartArr[UrSegData.MemPartIdx]->ma_RSegments.push_back(pLine);
                    }
                    else
                    {
                        if(MySegData.isLLine)
                            MemPartArr[MySegData.MemPartIdx]->ma_LSegments.push_back(pLine);
                        else
                            MemPartArr[MySegData.MemPartIdx]->ma_RSegments.push_back(pLine);
                    }
                    // add point to Connection
                    m_DrawSection.AddCopy(CrossVec);
                    m_DrawSection.ma_Bulge.Add(0.);
                    m_Section.AddCopy(CrossVec);
                    m_Section.ma_Bulge.Add(0.);
                    continue;
                }
            }

            // intersect (not parallel/not colinear)
            vector<GM2DVector> CrossVecs;
            vector<double> MyLocArr, UrLocArr;
            vector<long> ResutlArr;
            long ResultIdx = -1;

            long nRval = MySegData.pCurve->IntersectEnd2(UrSegData.pCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true);

            switch (nRval)
            {
            case 1:
            case -1:
            case 11:
                for(long i = 0 ; i < ResutlArr.size() ; i++)
                    if(ResutlArr[i] == 1 || ResutlArr[i] == 2)
                    {
                        ResultIdx = i;
                        break;
                    }
                    break;

            case -2:
                double temp0 = min(fabs(MyLocArr[0]-0.0), fabs(MyLocArr[0]-1.0));
                double temp1 = min(fabs(MyLocArr[1]-0.0), fabs(MyLocArr[1]-1.0));

                if(temp0<temp1)
                    ResultIdx = 0;
                else
                    ResultIdx = 1;
                break;

            }

            if (ResultIdx>-1)
            {
                GM2DVector MyCrossVec = CrossVecs[ResultIdx];
                double MyLoc = MyLocArr[ResultIdx];
                double UrLoc = UrLocArr[ResultIdx];

                m_DrawSection.AddCopy(MyCrossVec);
                m_DrawSection.ma_Bulge.Add(0.);
                m_Section.AddCopy(MyCrossVec);
                m_Section.ma_Bulge.Add(0.);

                if(fabs(MyLoc) > DTOL_GM)
                {
                    if(fabs(MyLoc -1) > DTOL_GM )
                    {
                        if(MyLoc > 0.5)
                            MySegData.pCurve->SetP1(MyCrossVec);
                        else
                            MySegData.pCurve->SetP0(MyCrossVec);
                    }
                }

                if(fabs(UrLoc) > DTOL_GM)
                {
                    if(fabs(UrLoc -1) > DTOL_GM )
                    {
                        if(UrLoc > 0.5)
                            UrSegData.pCurve->SetP1(MyCrossVec);
                        else
                            UrSegData.pCurve->SetP0(MyCrossVec);
                    }
                }
            }
        }

        m_DrawSection.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5
        m_Section.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5

    }

	for(long iPart = 0 ; iPart < NumMemPart ;iPart++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[iPart];

		pMyMemPart->mp_LPolyLine->AddSegments(pMyMemPart->ma_LSegments);
		pMyMemPart->mp_RPolyLine->AddSegments(pMyMemPart->ma_RSegments);
	}

	return true;
}

bool MSConnLevelZone::TrimSection2_3( GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool IsFloorZone/*=true*/ )
{
	GM2DCurve *pMyLineIn = NULL, *pMyFullLineIn=NULL;
	GM2DCurve *pUrLineIn = NULL, *pUrFullLineIn=NULL;
	GM2DCurve *pMyLineOut = NULL, *pMyFullLineOut=NULL;
	GM2DCurve *pUrLineOut = NULL, *pUrFullLineOut=NULL;

	GM2DVector* pMyEndPoint = NULL, *pUrEndPoint = NULL;
	long MyEndNum, UrEndNum;
	bool bMyEndIsMidPoint, bUrEndIsMidPoint;

	long NumMemPart = MemPartArr.GetSize();

	long NumLinkMember = mp_Conn->GetNumMemberG();

	for(long i = 0 ; i < NumMemPart ; i++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[i];
		GM2DCurve* pMyCenCurve = pMyMemPart->mp_LinkMem->GetLongLVec();


		//for(long j = i+1 ; j < NumMemPart ; j++)
		//for(int j = ( i == NumMemPart-1 ? 0 : i+1 ) ; j < ( i == NumMemPart-1 ? 1 : NumMemPart ) ;j++)
		for(long j = 0 ; j < NumMemPart; j++)
		{
			MSLinkMemPart* pUrMemPart = MemPartArr[j];
			// 같은 링크부재의 파트일 경우 트림에서 제외
			if(pMyMemPart == pUrMemPart || pMyMemPart->mp_LinkMem==pUrMemPart->mp_LinkMem)
				continue;

			if(pMyMemPart->IsWallAndGirderWith(pUrMemPart))
				continue;

			GM2DCurve* pUrCenCurve = pUrMemPart->mp_LinkMem->GetLongLVec();

			long NumLCurve = pMyMemPart->mp_LPolyLine->GetNumLines();
			for(long iCurve = 0 ;iCurve < NumLCurve ; iCurve++)
			{
				GM2DCurve* pCurCurve = pMyMemPart->mp_LPolyLine->GetCurve(iCurve);
				if(pCurCurve == NULL) continue;
				GM2DVector CenVec = pCurCurve->PointAtParam(0.5);

				long LSide = pUrMemPart->mp_LLine->WhichSide(CenVec);
				long RSide = pUrMemPart->mp_RLine->WhichSide(CenVec);
				double dLoc = pUrCenCurve->OrthoBaseLocValue(CenVec);
				double dSLoc = pUrCenCurve->OrthoBaseLocValue(pCurCurve->m_P0);
				double dELoc = pUrCenCurve->OrthoBaseLocValue(pCurCurve->m_P1);
				if(!(LSide * RSide > 0) && (dLoc > 0 && dLoc < 1 ))
				{
					pMyMemPart->mp_LPolyLine->SetVisibility(iCurve, false);
				}
				delete pCurCurve;
			}

			long NumRCurve = pMyMemPart->mp_RPolyLine->GetNumLines();
			for(long iCurve = 0 ;iCurve < NumRCurve ; iCurve++)
			{
				GM2DCurve* pCurCurve = pMyMemPart->mp_RPolyLine->GetCurve(iCurve);
				if(pCurCurve == NULL) continue;
				GM2DVector CenVec = pCurCurve->PointAtParam(0.5);

				long LSide = pUrMemPart->mp_LLine->WhichSide(CenVec);
				long RSide = pUrMemPart->mp_RLine->WhichSide(CenVec);
				double dLoc = pUrCenCurve->OrthoBaseLocValue(CenVec);
				double dSLoc = pUrCenCurve->OrthoBaseLocValue(pCurCurve->m_P0);
				double dELoc = pUrCenCurve->OrthoBaseLocValue(pCurCurve->m_P1);
				if(!(LSide * RSide > 0) && (dLoc > 0 && dLoc < 1 ))
				{
					pMyMemPart->mp_RPolyLine->SetVisibility(iCurve, false);
				}
				delete pCurCurve;
			}
			delete pUrCenCurve;
		}
		delete pMyCenCurve;
	}
	return true;
}

bool MSConnLevelZone::TrimSection2_2( GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool IsFloorZone/*=true*/ )
{
	GM2DCurve *pMyLineIn = NULL, *pMyFullLineIn=NULL;
	GM2DCurve *pUrLineIn = NULL, *pUrFullLineIn=NULL;
	GM2DCurve *pMyLineOut = NULL, *pMyFullLineOut=NULL;
	GM2DCurve *pUrLineOut = NULL, *pUrFullLineOut=NULL;

	GM2DVector* pMyEndPoint = NULL, *pUrEndPoint = NULL;
	long MyEndNum, UrEndNum;
	bool bMyEndIsMidPoint, bUrEndIsMidPoint;
	bool isMyArc = false;
	bool isUrArc = false;

	double dTol = DTOL_GM;
	double dTol_Line = DTOL_GM;
	double dTol_Arc = DTOL1;

	long NumMemPart = MemPartArr.GetSize();
	long NumLinkMember = mp_Conn->GetNumMemberG();

	for(long i = 0 ; i < NumMemPart ; i++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[i];
		pMyMemPart->SetLRPolyLine(mp_Conn);
	}

	for(long i = 0 ; i < NumMemPart ; i++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[i];
		
		/*CTypedPtrArray<CObArray, GM2DCurve*> UrSegments;
		for(long j = 0 ; j < NumMemPart; j++)
		{
			MSLinkMemPart* pUrMemPart = MemPartArr[j];
			if(pMyMemPart == pUrMemPart) continue;
			
			long NumLCurve = pUrMemPart->mp_LPolyLine->GetNumLines();
			for(long iCurve = 0 ; iCurve < NumLCurve ; iCurve++)
			{
				GM2DCurve* pCurve = pUrMemPart->mp_LPolyLine->GetCurve(iCurve);
				if(pCurve == NULL) continue;
				UrSegments.Add(pCurve);
			}
			long NumRCurve = pUrMemPart->mp_RPolyLine->GetNumLines();
			for(long iCurve = 0 ; iCurve < NumRCurve ; iCurve++)
			{
				GM2DCurve* pCurve = pUrMemPart->mp_RPolyLine->GetCurve(iCurve);
				if(pCurve == NULL) continue;
				UrSegments.Add(pCurve);
			}
		}

		CTypedPtrArray<CObArray, GM2DCurve*> MySegments;
		pMyMemPart->mp_LPolyLine->GetSegments(MySegments, UrSegments, 1);
		long NumSeg = MySegments.GetSize();
		if(NumSeg > 1)
			pMyMemPart->mp_LPolyLine->AddSegments(MySegments, true);

		for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
			delete MySegments[iSeg];
		MySegments.RemoveAll();

		pMyMemPart->mp_RPolyLine->GetSegments(MySegments, UrSegments, 1);
		NumSeg = MySegments.GetSize();
		if(NumSeg > 1)
			pMyMemPart->mp_RPolyLine->AddSegments(MySegments, true);

		for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
			delete MySegments[iSeg];
		MySegments.RemoveAll();
*/

				for(long j = 0 ; j < NumMemPart; j++)
		{
			MSLinkMemPart* pUrMemPart = MemPartArr[j];
			if(pMyMemPart == pUrMemPart) continue;

			CArray<long, long> UrRegion;
			CTypedPtrArray<CObArray, GM2DCurve*> MySegments;
			pMyMemPart->mp_LPolyLine->GetSegments(MySegments, pUrMemPart->mp_LPolyLine, UrRegion, 1);
			long NumSeg = MySegments.GetSize();
			if(NumSeg > 1)
				pMyMemPart->mp_LPolyLine->AddSegments(MySegments);
			
			for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				delete MySegments[iSeg];
			MySegments.RemoveAll();

			pMyMemPart->mp_LPolyLine->GetSegments(MySegments, pUrMemPart->mp_RPolyLine, UrRegion, 1);
			NumSeg = MySegments.GetSize();
			if(NumSeg > 1)
				pMyMemPart->mp_LPolyLine->AddSegments(MySegments);

			for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				delete MySegments[iSeg];
			MySegments.RemoveAll();

			pMyMemPart->mp_RPolyLine->GetSegments(MySegments, pUrMemPart->mp_LPolyLine, UrRegion, 1);
			NumSeg = MySegments.GetSize();
			if(NumSeg > 1)
				pMyMemPart->mp_RPolyLine->AddSegments(MySegments);

			for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				delete MySegments[iSeg];
			MySegments.RemoveAll();

			pMyMemPart->mp_RPolyLine->GetSegments(MySegments, pUrMemPart->mp_RPolyLine, UrRegion, 1);
			NumSeg = MySegments.GetSize();
			if(NumSeg > 1)
				pMyMemPart->mp_RPolyLine->AddSegments(MySegments);

			for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				delete MySegments[iSeg];
			MySegments.RemoveAll();
		}
		
	}


	for(long i = 0 ; i < NumMemPart ; i++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[i];
		GM2DPolyline* pMyPartPolyLine = pMyMemPart->GetBoundary2();
// 		GM2DLineVector TheLine1 = pMyMemPart->mp_LinkMem->GetCenLVec();
// 		GM2DLineVector* pMyCenCurve = &TheLine1;
		
		isMyArc = pMyMemPart->mp_LinkMem->isArc();
		

		if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
		{
			pMyEndPoint = &(pMyMemPart->mp_LLine->m_P1);
			MyEndNum = 1;
			mp_Conn->IsMidConn();
// 			if(mp_Conn->IsSameOrUpDown(pMyMemPart->mp_LinkMem->GetEConn()))
// 				bMyEndIsMidPoint = false;
// 			else
				bMyEndIsMidPoint = true;
		}
		else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
		{
			pMyEndPoint = &(pMyMemPart->mp_LLine->m_P0);
			MyEndNum = 0;
// 			if(mp_Conn->IsSameOrUpDown(pMyMemPart->mp_LinkMem->GetSConn()))
// 				bMyEndIsMidPoint = false;
// 			else
				bMyEndIsMidPoint = true;
		}
		else
		{
			continue;
			ASSERT(0);
		}

		double MyDegree = pMyMemPart->GetDegree(MyEndNum);
		bool isMyColiner = false;
		map<long, long>::iterator itID = m_ColinerMemMap.find(pMyMemPart->mp_LinkMem->m_ID);
		if(itID != m_ColinerMemMap.end())
			isMyColiner = true;
		else
			isMyColiner = false;

		GM2DCurve* pMyCenCurve = NULL;
		if(isMyColiner == true)
			pMyCenCurve = pMyMemPart->mp_LinkMem->GetLongLVec();
		else
			pMyCenCurve = pMyMemPart->mp_LinkMem->GetCenLineVec();

		pMyCenCurve = pMyMemPart->mp_CenLine;

		GM2DCurve* pOldMyCenCurve = NULL;

		vector<int> MyLLineCnt;
		vector<int> MyRLineCnt;

		long NumLCurve = pMyMemPart->mp_LPolyLine->GetNumLines();
		for(long iCurve = 0 ;iCurve < NumLCurve ; iCurve++)
			MyLLineCnt.push_back(0);

		long NumRCurve = pMyMemPart->mp_RPolyLine->GetNumLines();
		for(long iCurve = 0 ;iCurve < NumRCurve ; iCurve++)
			MyRLineCnt.push_back(0);

		//for(long j = i+1 ; j < NumMemPart ; j++)
		//for(int j = ( i == NumMemPart-1 ? 0 : i+1 ) ; j < ( i == NumMemPart-1 ? 1 : NumMemPart ) ;j++)
		for(long j = 0 ; j < NumMemPart; j++)
		{
			MSLinkMemPart* pUrMemPart = MemPartArr[j];
			// 같은 링크부재의 파트일 경우 트림에서 제외
			if(pMyMemPart == pUrMemPart || pMyMemPart->mp_LinkMem==pUrMemPart->mp_LinkMem)
				continue;

			if(pMyMemPart->IsWallAndGirderWith(pUrMemPart))
				continue;

			bool isUrColiner = false;
			map<long, long>::iterator itID = m_ColinerMemMap.find(pUrMemPart->mp_LinkMem->m_ID);
			if(itID != m_ColinerMemMap.end())
				isUrColiner = true;
			else
				isUrColiner = false;

			GM2DCurve* pUrCenCurve = NULL;
			if(isUrColiner)
				pUrCenCurve = pUrMemPart->mp_LinkMem->GetLongLVec();
			else
				pUrCenCurve = pUrMemPart->mp_LinkMem->GetCenLineVec();

			pUrCenCurve = pUrMemPart->mp_CenLine;

// 			GM2DLineVector TheLine2 = pUrMemPart->mp_LinkMem->GetCenLVec();
// 			GM2DLineVector* pUrCenCurve = &TheLine2;
			isUrArc = pUrMemPart->mp_LinkMem->isArc();
			//GM2DPolyline* pUrPartPolyLine = pUrMemPart->GetBoundary2();

			if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetEConn()))
			{
				pUrEndPoint = &(pUrMemPart->mp_LLine->m_P1);
				UrEndNum = 1;
// 				if(mp_Conn->IsSameOrUpDown(pUrMemPart->mp_LinkMem->GetEConn()))
// 					bUrEndIsMidPoint = false;
// 				else
					bUrEndIsMidPoint = true;
			}
			else if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetSConn()))
			{
				pUrEndPoint = &(pUrMemPart->mp_LLine->m_P0);
				UrEndNum = 0;
// 				if(mp_Conn->IsSameOrUpDown(pUrMemPart->mp_LinkMem->GetSConn()))
// 					bUrEndIsMidPoint = false;
// 				else
					bUrEndIsMidPoint = true;
			}
			else
			{
				continue;
				ASSERT(0);
			}

			double UrDegree = pUrMemPart->GetDegree(UrEndNum);
			double InnerDegree = fabs(UrDegree - MyDegree);

			bool isColiner = false;
			if(fabs(InnerDegree - 180) < MSFloor::ms_ColinearDegreeTol)
				isColiner = true;

// 			if(isColiner)
// 			{
// 				pOldMyCenCurve = pMyCenCurve;
// 				pMyCenCurve = pMyMemPart->mp_LinkMem->GetLongLVec();
// 				delete pUrCenCurve;
// 				pUrCenCurve = pUrMemPart->mp_LinkMem->GetLongLVec();
// 			}

			//if(!isUrArc && !isUrColiner)

			/*if(!isMyArc && !isUrArc && !isUrColiner)
			{
				vector<GM2DVector> CrossVecs;
				vector<double> MyLocArr, UrLocArr;
				vector<long> ResutlArr;
				long ResultIdx = 0;

				long nRval = pUrCenCurve->IntersectEnd2(pMyCenCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, MSFloor::ms_LineHitDistTol);
				if(nRval != 0)
				{
					if(UrEndNum == 0)
						pUrCenCurve->SetP0(CrossVecs[0]);
					else if(UrEndNum == 1)
						pUrCenCurve->SetP1(CrossVecs[0]);
					if(MyEndNum == 0)
						pMyCenCurve->SetP0(CrossVecs[0]);
					else if(MyEndNum == 1)
						pMyCenCurve->SetP1(CrossVecs[0]);
				}
			}*/

			GM2DPolyline* pUrPartPolyLine = pUrMemPart->GetBoundary2(pUrCenCurve);

			if(isMyArc || isUrArc)
				dTol = dTol_Arc;
			else
				dTol = dTol_Line;
			if(isColiner && (isMyArc || isUrArc))
				dTol = 0;
			//if(NumMemPart == 2 || (InnerDegree > 88 && InnerDegree < 92 ) || (InnerDegree > 180 && InnerDegree < 180 ) || (InnerDegree > 268 && InnerDegree < 272 ))
// 			if(NumMemPart == 2)
// 				dTol = 0;

			/*CArray<long, long> UrRegion;
			CTypedPtrArray<CObArray, GM2DCurve*> MySegments;
			pMyMemPart->mp_LPolyLine->GetSegments(MySegments, pUrMemPart->mp_LPolyLine, UrRegion, dTol);
			long NumSeg = MySegments.GetSize();
			if(NumSeg > 1)
			{
				//pMyMemPart->mp_LPolyLine->ResetPolyline();
				pMyMemPart->mp_LPolyLine->AddSegments(MySegments, true);
			}
			for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				delete MySegments[iSeg];
			MySegments.RemoveAll();

			pMyMemPart->mp_LPolyLine->GetSegments(MySegments, pUrMemPart->mp_RPolyLine, UrRegion, dTol);
			NumSeg = MySegments.GetSize();
			if(NumSeg > 1)
			{
				//pMyMemPart->mp_LPolyLine->ResetPolyline();
				pMyMemPart->mp_LPolyLine->AddSegments(MySegments, true);
			}
			for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				delete MySegments[iSeg];
			MySegments.RemoveAll();

			pMyMemPart->mp_RPolyLine->GetSegments(MySegments, pUrMemPart->mp_LPolyLine, UrRegion, dTol);
			NumSeg = MySegments.GetSize();
			if(NumSeg > 1)
			{
				//pMyMemPart->mp_RPolyLine->ResetPolyline();
				pMyMemPart->mp_RPolyLine->AddSegments(MySegments, true);
			}
			for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				delete MySegments[iSeg];
			MySegments.RemoveAll();

			pMyMemPart->mp_RPolyLine->GetSegments(MySegments, pUrMemPart->mp_RPolyLine, UrRegion, dTol);
			NumSeg = MySegments.GetSize();
			if(NumSeg > 1)
			{
				//pMyMemPart->mp_RPolyLine->ResetPolyline();
				pMyMemPart->mp_RPolyLine->AddSegments(MySegments, true);
			}
			for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				delete MySegments[iSeg];
			MySegments.RemoveAll();*/

			//continue;
			double dLLineLoc = 0.;
			double dRLineLoc = 0.;
			double dLTol = dTol;
			double dRTol = dTol;
			if(MyEndNum == 0)
			{
				dLLineLoc = pUrCenCurve->OrthoBaseLocValue(pMyMemPart->mp_LLine->m_P0);
				dRLineLoc = pUrCenCurve->OrthoBaseLocValue(pMyMemPart->mp_RLine->m_P0);
			}
			else if(MyEndNum == 1)
			{
				dLLineLoc = pUrCenCurve->OrthoBaseLocValue(pMyMemPart->mp_LLine->m_P1);
				dRLineLoc = pUrCenCurve->OrthoBaseLocValue(pMyMemPart->mp_RLine->m_P1);
			}

			if(dLLineLoc > 0 - dTol && dLLineLoc < 1 + dTol && dRLineLoc > 0 - dTol && dRLineLoc < 1 + dTol)
			{
				if(UrEndNum == 0)
				{
					if(dLLineLoc > dRLineLoc)
						dRTol = 0;
					else
						dLTol = 0;
				}
				else if(UrEndNum == 1)
				{
					if(dLLineLoc > dRLineLoc)
						dLTol = 0;
					else
						dRTol = 0;
				}
			}

			if(pUrMemPart->m_IsParallelCenLine)
			{
				dLTol = 0.1;
				dRTol = 0.1;
			}

			long NumLCurve = pMyMemPart->mp_LPolyLine->GetNumLines();
			for(long iCurve = 0 ;iCurve < NumLCurve ; iCurve++)
			{
				GM2DCurve* pCurCurve = pMyMemPart->mp_LPolyLine->GetCurve(iCurve);
				if(pCurCurve == NULL || (pCurCurve->IsVisible() == false && MyLLineCnt[iCurve] == 0)) continue;
				GM2DVector CenVec = pCurCurve->PointAtParam(0.5);
				
				
		/*		long nSVal = pMyPartPolyLine->Contains(&pCurCurve->m_P0);
				long nCVal = pUrPartPolyLine->Contains(&CenVec);
				long nEVal = pMyPartPolyLine->Contains(&pCurCurve->m_P1);

				if(nCVal < 0 && nSVal < 0 && nEVal < 0)
					pMyMemPart->mp_LPolyLine->SetVisibility(iCurve, false);
				*/

				long SLSide = pUrMemPart->mp_LLine->WhichSide(pCurCurve->m_P0);
				long SRSide = pUrMemPart->mp_RLine->WhichSide(pCurCurve->m_P0);
				long ELSide = pUrMemPart->mp_LLine->WhichSide(pCurCurve->m_P1);
				long ERSide = pUrMemPart->mp_RLine->WhichSide(pCurCurve->m_P1);
				long MLSide = pUrMemPart->mp_LLine->WhichSide(CenVec);
				long MRSide = pUrMemPart->mp_RLine->WhichSide(CenVec);
				double dLoc = pUrCenCurve->OrthoBaseLocValue(CenVec);

// 				vector<GM2DVector> CrossVecs;
// 				vector<double> MyLocArr, UrLocArr;
// 				vector<long> ResutlArr;
// 				long res = pCurCurve->IntersectEnd2(pUrCenCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, 0, 0, 0);

				double dSLoc = pUrCenCurve->OrthoBaseLocValue(pCurCurve->m_P0);
				double dELoc = pUrCenCurve->OrthoBaseLocValue(pCurCurve->m_P1);

				double dMySLoc = pMyCenCurve->OrthoBaseLocValue(pCurCurve->m_P0);
				double dMyELoc = pMyCenCurve->OrthoBaseLocValue(pCurCurve->m_P1);

				//if(!(LSide * RSide > 0) && (dLoc > 0 && dLoc < 1 ))
				// 부재 안에 포함되는선 안그리기

// 				if(pUrMemPart->GetBoundary2()->Contains(&CenVec) == -1)
// 					pMyMemPart->mp_LPolyLine->SetVisibility(iCurve, false);

				//if(!(LSide * RSide > 0) && ( (dSLoc > 0 + DTOL_GM && dSLoc < 1 - DTOL_GM ) && (dELoc > 0 + DTOL_GM && dELoc < 1 - DTOL_GM )) )
				if(!(MLSide * MRSide > 0) && !(SLSide * SRSide > 0) && !(ELSide * ERSide > 0) && ( (dSLoc > 0 - dLTol && dSLoc < 1 + dLTol ) && (dELoc > 0 - dLTol && dELoc < 1 + dLTol )) )
				{
					pMyMemPart->mp_LPolyLine->SetVisibility(iCurve, false);
				}
// 				else if(MyLLineCnt[iCurve] > 0)
// 				{
// 					pMyMemPart->mp_LPolyLine->SetVisibility(iCurve, true);
// 				}
				// 조인트에서 첫번째 선이 다른 부재 바깥에 있으면 안그리기
				if(/*!isMyArc && */!pMyMemPart->isBuriedLinkMember() /* && !bMyEndIsMidPoint*/)
				{
					if(!isMyColiner && NumLCurve > 1 && (MyEndNum == 0)/* && (iCurve == 0) */&& (SLSide * SRSide > 0) && dMySLoc < 0 + DTOL_GM )
					{
						if(pMyMemPart->mp_LPolyLine->GetVisibilityCount(true) > 1)
						{
							pMyMemPart->mp_LPolyLine->SetVisibility(iCurve, false);
							//MyLLineCnt[iCurve]++;
						}
					}
					else if(!isMyColiner &&  NumLCurve > 1 && (MyEndNum == 1) /*&& (iCurve == NumLCurve -1)*/ && (ELSide * ERSide > 0) && dMyELoc > 1 - DTOL_GM )
					{
						if(pMyMemPart->mp_LPolyLine->GetVisibilityCount(true) > 1)
						{
							pMyMemPart->mp_LPolyLine->SetVisibility(iCurve, false);
							//MyLLineCnt[iCurve]++;
						}
					}
				}

			

				delete pCurCurve;
			}

			long NumRCurve = pMyMemPart->mp_RPolyLine->GetNumLines();
			for(long iCurve = 0 ;iCurve < NumRCurve ; iCurve++)
			{
				GM2DCurve* pCurCurve = pMyMemPart->mp_RPolyLine->GetCurve(iCurve);
				if(pCurCurve == NULL || ( pCurCurve->IsVisible() == false && MyRLineCnt[iCurve] == 0) ) continue;
				GM2DVector CenVec = pCurCurve->PointAtParam(0.5);

				
				/*long nSVal = pMyPartPolyLine->Contains(&pCurCurve->m_P0);
				long nCVal = pUrPartPolyLine->Contains(&CenVec);
				long nEVal = pMyPartPolyLine->Contains(&pCurCurve->m_P1);

				if(nCVal < 0 && nSVal < 0 && nEVal < 0)
					pMyMemPart->mp_RPolyLine->SetVisibility(iCurve, false);
				*/


				long SLSide = pUrMemPart->mp_LLine->WhichSide(pCurCurve->m_P0);
				long SRSide = pUrMemPart->mp_RLine->WhichSide(pCurCurve->m_P0);
				long ELSide = pUrMemPart->mp_LLine->WhichSide(pCurCurve->m_P1);
				long ERSide = pUrMemPart->mp_RLine->WhichSide(pCurCurve->m_P1);
				long MLSide = pUrMemPart->mp_LLine->WhichSide(CenVec);
				long MRSide = pUrMemPart->mp_RLine->WhichSide(CenVec);
				double dLoc = pUrCenCurve->OrthoBaseLocValue(CenVec);
				
// 				vector<GM2DVector> CrossVecs;
// 				vector<double> MyLocArr, UrLocArr;
// 				vector<long> ResutlArr;
// 				long res = pCurCurve->IntersectEnd2(pUrCenCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, 0, 0, 0);

				double dSLoc = pUrCenCurve->OrthoBaseLocValue(pCurCurve->m_P0);
				double dELoc = pUrCenCurve->OrthoBaseLocValue(pCurCurve->m_P1);

				double dMySLoc = pMyCenCurve->OrthoBaseLocValue(pCurCurve->m_P0);
				double dMyELoc = pMyCenCurve->OrthoBaseLocValue(pCurCurve->m_P1);

				//if(!(LSide * RSide > 0) && (dLoc > 0 && dLoc < 1 ))
				// 부재 안에 포함되는선 안그리기
				
// 				if(pUrMemPart->GetBoundary2()->Contains(&CenVec) == -1)
// 					pMyMemPart->mp_RPolyLine->SetVisibility(iCurve, false);

				
				//if(!(LSide * RSide > 0) && ( (dSLoc > 0 + DTOL_GM && dSLoc < 1 - DTOL_GM ) && (dELoc > 0 + DTOL_GM && dELoc < 1 - DTOL_GM )) )
				if(!(MLSide * MRSide > 0) && !(SLSide * SRSide > 0) && !(ELSide * ERSide > 0) && ( (dSLoc > 0 - dRTol && dSLoc < 1 + dRTol ) && (dELoc > 0 - dRTol && dELoc < 1 + dRTol )) )
				{
					pMyMemPart->mp_RPolyLine->SetVisibility(iCurve, false);
				}
// 				else if(MyRLineCnt[iCurve] > 0)
// 				{
// 					pMyMemPart->mp_RPolyLine->SetVisibility(iCurve, true);
// 				}

				// 조인트에서 첫번째 선이 다른 부재 바깥에 있으면 안그리기
				if(/*!isMyArc && */!pMyMemPart->isBuriedLinkMember()/* && !bMyEndIsMidPoint*/)
				{
					if(!isMyColiner && NumRCurve > 1 && (MyEndNum == 0)/* && (iCurve == 0)*/ && (SLSide * SRSide > 0)  && dMySLoc < 0 + DTOL_GM  )
					{
						if(pMyMemPart->mp_RPolyLine->GetVisibilityCount(true) > 1)
						{
							pMyMemPart->mp_RPolyLine->SetVisibility(iCurve, false);
							//MyRLineCnt[iCurve]++;
						}
					}
					else if(!isMyColiner &&  NumRCurve > 1 && (MyEndNum == 1) /*&& (iCurve == NumRCurve -1)*/ && (ELSide * ERSide > 0) && dMyELoc > 1 - DTOL_GM )
					{
						if(pMyMemPart->mp_RPolyLine->GetVisibilityCount(true) > 1)
						{
							pMyMemPart->mp_RPolyLine->SetVisibility(iCurve, false);
							//MyRLineCnt[iCurve]++;
						}
					}
				}

				delete pCurCurve;
			}

			//if(isColiner)
				//pMyCenCurve = pOldMyCenCurve;

			//delete pUrCenCurve;

		}
		//delete pMyCenCurve;
	}

	return BuildSectionOnGivenMemPart(TheSection, DrawSection, MemPartArr);
}

bool MSConnLevelZone::TrimSection2( GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool IsFloorZone/*=true*/ )
{
	GM2DCurve *pMyLineIn = NULL, *pMyFullLineIn=NULL;
	GM2DCurve *pUrLineIn = NULL, *pUrFullLineIn=NULL;
	GM2DCurve *pMyLineOut = NULL, *pMyFullLineOut=NULL;
	GM2DCurve *pUrLineOut = NULL, *pUrFullLineOut=NULL;

	GM2DVector* pMyEndPoint = NULL, *pUrEndPoint = NULL;
	long MyEndNum, UrEndNum;
	bool bMyEndIsMidPoint, bUrEndIsMidPoint;

	double DefELongDeg = 60.;
	double ELongDeg = 0.;
	bool isLineLevelDifG = false;
	bool bTrimFail = false;

	bool bOnlyInCross = false;
	bool bOnlyShorten = false;
	CArray<long, long> bLLineCutArr;
	CArray<long, long> bRLineCutArr;
	long *pMyInCut, *pMyOutCut, *pUrInCut, *pUrOutCut;

	m_ColinerMemMap.clear();
	long NumMemPart = MemPartArr.GetSize();
	long NumLinkMember = mp_Conn->GetNumMemberG();

	if(NumMemPart == 3 && NumLinkMember == 2)
		m_UseTramInOut = false;

	double dMaxWidth = 0;

	for(int i=0; i< NumMemPart ; i++)
	{
		bLLineCutArr.Add(0);
		bRLineCutArr.Add(0);
		MSLinkMemPart* pMyMemPart = MemPartArr[i];
		pMyMemPart->m_CenLineSetCnt = 0;

		double dWidth = pMyMemPart->mp_LinkMem->GetWidth();
		dMaxWidth = max(dMaxWidth, dWidth);
	}

	for(int i=0; i< NumMemPart ; i++)
    {
        MSLinkMemPart* pMyMemPart = MemPartArr[i];
        if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
        {
            pMyEndPoint = &(pMyMemPart->mp_LLine->m_P1);
            MyEndNum = 1;
//             if(mp_Conn->IsSameOrUpDown(pMyMemPart->mp_LinkMem->GetEConn()))
//                 bMyEndIsMidPoint = false;
//             else
                bMyEndIsMidPoint = true;
        }
        else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
        {
            pMyEndPoint = &(pMyMemPart->mp_LLine->m_P0);
            MyEndNum = 0;
//             if(mp_Conn->IsSameOrUpDown(pMyMemPart->mp_LinkMem->GetSConn()))
//                 bMyEndIsMidPoint = false;
//             else
                bMyEndIsMidPoint = true;
        }
        else
            ASSERT(0);

        double MyDegree = pMyMemPart->GetDegree(MyEndNum);

		//if(pMyMemPart->mp_LinkMem->m_hasDelPart == true)
			//m_ColinerMemMap.insert(make_pair(pMyMemPart->mp_LinkMem->m_ID, pMyMemPart->mp_LinkMem->m_ID));

        //for(int j = i+1; j< nMem ;j++)
		//for(int j = i+1 ; j < NumMemPart ;j++)
		//for(int j = ( i == NumMemPart-1 ? 0 : i+1 ) ; j < ( i == NumMemPart-1 ? 1 : NumMemPart ) ;j++)
        {
			int j = (i+1)%NumMemPart;
			MSLinkMemPart* pUrMemPart = MemPartArr[j];
			//부재가 2개일경우 처리 하지 않는다.(ex J = 0, i = 1 ) 
			//둘중 하나가 아크 부재이면 처리한다.
			//if(nMem == 2 && j == 0 && !pMyMemPart->mp_LinkMem->isArc() && !pUrMemPart->mp_LinkMem->isArc() ) 
			//	break;

            // 같은 링크부재의 파트일 경우 트림에서 제외
            if(pMyMemPart == pUrMemPart || pMyMemPart->mp_LinkMem==pUrMemPart->mp_LinkMem)
            {
				m_ColinerMemMap.insert(make_pair(pMyMemPart->mp_LinkMem->m_ID, pMyMemPart->mp_LinkMem->m_ID));
				m_ColinerMemMap.insert(make_pair(pUrMemPart->mp_LinkMem->m_ID, pUrMemPart->mp_LinkMem->m_ID));
				bLLineCutArr[i] = 1;
				bRLineCutArr[i] = 1;
				bLLineCutArr[j] = 1;
				bRLineCutArr[j] = 1;
				continue;
			}

            if(pMyMemPart->IsWallAndGirderWith(pUrMemPart))
               continue;

            if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetEConn()))
            {
                pUrEndPoint = &(pUrMemPart->mp_LLine->m_P1);
                UrEndNum = 1;
//                 if(mp_Conn->IsSameOrUpDown(pUrMemPart->mp_LinkMem->GetEConn()))
//                     bUrEndIsMidPoint = false;
//                 else
                    bUrEndIsMidPoint = true;
            }
            else if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetSConn()))
            {
                pUrEndPoint = &(pUrMemPart->mp_LLine->m_P0);
                UrEndNum = 0;
//                 if(mp_Conn->IsSameOrUpDown(pUrMemPart->mp_LinkMem->GetSConn()))
//                     bUrEndIsMidPoint = false;
//                 else
                    bUrEndIsMidPoint = true;
            }
            else
                ASSERT(0);

            double UrDegree = pUrMemPart->GetDegree(UrEndNum);
            double InnerDegree = UrDegree - MyDegree;
			
			if(InnerDegree < 0)
				InnerDegree += 360;

			//if( ( InnerDegree <= MSFloor::ms_ColinearDegreeTol )  || (fabs( InnerDegree -180.) <= MSFloor::ms_ColinearDegreeTol )
			if( fabs(fabs(InnerDegree)-180.) <= MSFloor::ms_ColinearDegreeTol 
				&& pMyMemPart->mp_LinkMem->isArc() == false && pUrMemPart->mp_LinkMem->isArc() == false)
            {
				//if(j == 0) continue;
				bool nInOnlyShort = true;
				bool nOutOnlyShort = true;
				if(abs(j-i) == 1)
					nInOnlyShort = false;
				if((j+1)%NumMemPart == i)
					nOutOnlyShort = false;

				m_ColinerMemMap.insert(make_pair(pMyMemPart->mp_LinkMem->m_ID, pMyMemPart->mp_LinkMem->m_ID));
				m_ColinerMemMap.insert(make_pair(pUrMemPart->mp_LinkMem->m_ID, pUrMemPart->mp_LinkMem->m_ID));

				if(!m_hasBuriedLinkMember)
				{
					if(MyEndNum!=UrEndNum)
					{
						CutParallelLine2(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum, pUrMemPart, 'L', pUrMemPart->mp_LLine, UrEndNum, nInOnlyShort);
						CutParallelLine2(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum, pUrMemPart, 'R', pUrMemPart->mp_RLine, UrEndNum, nOutOnlyShort);
						bLLineCutArr[i] = 1;
						bRLineCutArr[i] = 1;
						bLLineCutArr[j] = 1;
						bRLineCutArr[j] = 1;
					}
					else
					{
						CutParallelLine2(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum, pUrMemPart, 'R', pUrMemPart->mp_RLine, UrEndNum, nOutOnlyShort);
						CutParallelLine2(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum, pUrMemPart, 'L', pUrMemPart->mp_LLine, UrEndNum, nInOnlyShort);
						bLLineCutArr[i] = 1;
						bRLineCutArr[i] = 1;
						bLLineCutArr[j] = 1;
						bRLineCutArr[j] = 1;
					}
				}
            }
            else
            {
                char MyInSide, MyOutSide, UrInSide, UrOutSide;

					if(MyEndNum==0)
					{
						pMyLineIn = pMyMemPart->mp_LLine;
						pMyFullLineIn = pMyMemPart->mp_LinkMem->mp_LLine;
						pMyLineOut = pMyMemPart->mp_RLine;
						pMyFullLineOut = pMyMemPart->mp_LinkMem->mp_RLine;
						MyInSide = 'L';
						MyOutSide = 'R';
						pMyInCut = &bLLineCutArr[i];
						pMyOutCut = &bRLineCutArr[i];
					}
					else
					{
						pMyLineIn = pMyMemPart->mp_RLine;
						pMyFullLineIn = pMyMemPart->mp_LinkMem->mp_RLine;
						pMyLineOut = pMyMemPart->mp_LLine;
						pMyFullLineOut = pMyMemPart->mp_LinkMem->mp_LLine;
						MyInSide = 'R';
						MyOutSide = 'L';
						pMyInCut = &bRLineCutArr[i];
						pMyOutCut = &bLLineCutArr[i];
					}
					if(UrEndNum==0)
					{
						pUrLineIn = pUrMemPart->mp_RLine;
						pUrFullLineIn = pUrMemPart->mp_LinkMem->mp_RLine;
						pUrLineOut = pUrMemPart->mp_LLine;
						pUrFullLineOut = pUrMemPart->mp_LinkMem->mp_LLine;
						UrInSide = 'R';
						UrOutSide = 'L';
						pUrInCut = &bRLineCutArr[j];
						pUrOutCut = &bLLineCutArr[j];
					}
					else
					{
						pUrLineIn = pUrMemPart->mp_LLine;
						pUrFullLineIn = pUrMemPart->mp_LinkMem->mp_LLine;
						pUrLineOut = pUrMemPart->mp_RLine;
						pUrFullLineOut = pUrMemPart->mp_LinkMem->mp_RLine;
						UrInSide = 'L';
						UrOutSide = 'R';
						pUrInCut = &bLLineCutArr[j];
						pUrOutCut = &bRLineCutArr[j];
					}
				
				if(pMyMemPart->mp_LinkMem->GetType() == MSElement::msLineLevelDifG && pUrMemPart->mp_LinkMem->GetType() == MSElement::msLineLevelDifG)
				{
					ELongDeg = 0.;
					isLineLevelDifG = true;
				}
				else
				{
					ELongDeg = DefELongDeg;
					isLineLevelDifG = false;
				}

				bOnlyInCross = false;
				bOnlyShorten = false;

				if( fabs(InnerDegree) <= MSFloor::ms_ColinearDegreeTol 
					&& pMyMemPart->mp_LinkMem->isArc() != pUrMemPart->mp_LinkMem->isArc() )
				{
					// 추후에 처리 함
					bTrimFail = true;
				}
				else if(pMyMemPart->mp_LinkMem->isArc() == false && pUrMemPart->mp_LinkMem->isArc() == false)
				{
					long MyInUrIn;
					if(isLineLevelDifG == true)
					{
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
					}
					else
					{
						if(pMyMemPart->mp_LinkMem->m_isBuriedLinkMember || pUrMemPart->mp_LinkMem->m_isBuriedLinkMember)
						{
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0);
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0);
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0);
						}
						else
						{
							if(j != 0 || NumMemPart > 2)
								ELongDeg = 0;
							else
								ELongDeg = DefELongDeg;
							//if(*pMyInCut == 0 || *pUrInCut == 0 || j == 0)
								MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
							//MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
							//if( (*pMyOutCut == 0 || *pUrOutCut == 0) && (NumMemPart == 2/* || m_UseTramInOut*/))
								if(j == 0 || NumMemPart > 2)
									ELongDeg = 0;
								else
									ELongDeg = DefELongDeg;
// 							if(MyInUrIn == 2)
// 								MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
							//MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						}
					}
				}
				else
				{
					long MyInUrIn;
					if(isLineLevelDifG == true)
					{
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
					}
					else
					{
						//if(*pMyInCut == 0 || *pUrInCut == 0|| j == 0)
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						//MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						//if((*pMyOutCut == 0 || *pUrOutCut == 0) /*&& (NumMemPart == 2 || m_UseTramInOut)*/ )
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
						//MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
					}
					if( !(NumLinkMember >= 2 && NumMemPart >= 4 && bMyEndIsMidPoint == true && bUrEndIsMidPoint == true)) // 2개의 아크 부재가 미드 조인트에서 겹칠경우 아래 로직 안들어감
					{

					}
					else
					{

					}
				}
            }
        }
    }

	for(int i=0; i< NumMemPart ; i++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[i];

		if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
			MyEndNum = 1;
		else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
			MyEndNum = 0;
		else
			ASSERT(0);

		MSPointMemberG* pPointMem=NULL;
		if(mp_Column)
			pPointMem = mp_Column;
		if(mp_IsoFoot)
			pPointMem = mp_IsoFoot;

		if(pPointMem && mp_DownLinkJointBoundry == NULL)
		{
			GM2DPolyline MyPoly(TRUE);
			pPointMem->GetBoundary(MyPoly);
			//MyPoly.Trim(pMyMemPart->mp_LLine, MyEndNum, !bLLineCutArr[i]);
			//MyPoly.Trim(pMyMemPart->mp_RLine, MyEndNum, !bRLineCutArr[i]);
			MyPoly.Trim(pMyMemPart->mp_LLine, MyEndNum, false);
			MyPoly.Trim(pMyMemPart->mp_RLine, MyEndNum, false);
		}

		if(mp_DownLinkJointBoundry != NULL)
		{
			bool bAllowExtend = false;

			//if(!bLLineCutArr[i])
				bool bLLineRVal = mp_DownLinkJointBoundry->Trim(pMyMemPart->mp_LLine, MyEndNum, bAllowExtend);
			//if(!bRLineCutArr[i])
				bool bRLineRVal = mp_DownLinkJointBoundry->Trim(pMyMemPart->mp_RLine, MyEndNum, bAllowExtend);
		}
	}

	//2개의 라인 부재일 경우 LLine과 RLine의 길이를 비교하여 라인부재의 최대 두깨 1.5보다 클경우 최대1.5배 길이만큼 연장되게 한다. 아크 부재는 안한다.
	if(NumMemPart == 2)
	{
		double dMaxExtendLen = dMaxWidth * 1.5;
		for(int i=0; i< NumMemPart ; i++)
		{
			MSLinkMemPart* pMyMemPart = MemPartArr[i];
			if(pMyMemPart->isBuriedLinkMember() || pMyMemPart->m_IsParallelCenLine == true || pMyMemPart->mp_CenLine->m_ObjType == GT_ARC) continue;

			if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
				MyEndNum = 1;
			else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
				MyEndNum = 0;
			else
				ASSERT(0);
			if(MyEndNum == 0)
			{
				double dLLoc = pMyMemPart->mp_LLine->OrthoBaseLocValue(pMyMemPart->mp_RLine->m_P0);
				double dRLoc = pMyMemPart->mp_RLine->OrthoBaseLocValue(pMyMemPart->mp_LLine->m_P0);

				if(dLLoc > 0)
				{
					GM2DVector TheLVec =  pMyMemPart->mp_LLine->PointAtParam(dLLoc);
					double dLLen = TheLVec.Distance(pMyMemPart->mp_LLine->m_P0);
					if(dMaxExtendLen < dLLen)
						pMyMemPart->mp_LLine->ExtendLineVector(TRUE, dLLen - dMaxExtendLen, TRUE);					
				}
				else if(dRLoc > 0)
				{
					GM2DVector TheRVec =  pMyMemPart->mp_RLine->PointAtParam(dRLoc);
					double dRLen = TheRVec.Distance(pMyMemPart->mp_RLine->m_P0);
					if(dMaxExtendLen < dRLen)
						pMyMemPart->mp_RLine->ExtendLineVector(TRUE, dRLen - dMaxExtendLen, TRUE);
				}
			}
			else if(MyEndNum == 1)
			{
				double dLLoc = pMyMemPart->mp_LLine->OrthoBaseLocValue(pMyMemPart->mp_RLine->m_P1);
				double dRLoc = pMyMemPart->mp_RLine->OrthoBaseLocValue(pMyMemPart->mp_LLine->m_P1);

				if(dLLoc < 1)
				{
					GM2DVector TheLVec =  pMyMemPart->mp_LLine->PointAtParam(dLLoc);
					double dLLen = TheLVec.Distance(pMyMemPart->mp_LLine->m_P1);
					if(dMaxExtendLen < dLLen)
						pMyMemPart->mp_LLine->ExtendLineVector(FALSE, dLLen - dMaxExtendLen, FALSE);
				}
				else if(dRLoc < 1)
				{
					GM2DVector TheRVec =  pMyMemPart->mp_RLine->PointAtParam(dRLoc);
					double dRLen = TheRVec.Distance(pMyMemPart->mp_RLine->m_P1);
					if(dMaxExtendLen < dRLen)
						pMyMemPart->mp_RLine->ExtendLineVector(FALSE, dRLen - dMaxExtendLen, FALSE);										
				}
			}
		}
	}

	if(bTrimFail == true)
	{
		for(int i=0; i< NumMemPart ; i++)
		{
			MSLinkMemPart *pLink = ma_MemPart[i];
			pLink->SetLinesOfAMemPart(pLink->GetSConn() == mp_Conn, pLink->GetEConn() == mp_Conn);

			/*
			// 끝선 그리기...
			if(pLink->mp_LinkMem->GetSConn() == mp_Conn || pLink->mp_LinkMem->GetEConn() == mp_Conn)
			{
				long StartOrEnd = pLink->GetSConn()==mp_Conn ? 0 : 1;
				GM2DPolyline* pEndPoly = new GM2DPolyline(TRUE);
				GM2DPolyline* pLinkPoly = pLink->GetBoundary(FALSE);
				GM2DLineVector EndLine = pLink->GetEndLine(StartOrEnd, FALSE);
				pEndPoly->AddLine(EndLine);
				pLink->AddEndJointPoly(this, StartOrEnd, pEndPoly);
			}
			*/
		}
		return false;
	}

	return TrimSection2_2(TheSection, DrawSection, MemPartArr);
	//return BuildSectionOnGivenMemPart(TheSection, MemPartArr);
	//return true;
}

long MSConnLevelZone::CutLineMemPart(GM2DCurve* pMyLine, GM2DCurve* pUrLine, MSLinkMemPart* pMyMember, MSLinkMemPart* pUrMember, long MyEndNum, long UrEndNum, double ElongDegree)
{
	GM2DVector MyCrossVec = MyEndNum==0 ? pMyLine->GetP0() : pMyLine->GetP1();
	GM2DVector UrCrossVec = UrEndNum==0 ? pUrLine->GetP0() : pUrLine->GetP1();

	GM2DLineVector MyCrossLine(MyCrossVec, MyEndNum==0 ? pMyLine->m_P1 : pMyLine->m_P0);
	GM2DLineVector UrCrossLine(MyCrossVec, UrEndNum==0 ? pUrLine->m_P1 : pUrLine->m_P0);
	double Degree = MyCrossLine.GetDegree(UrCrossLine);
	if(Degree<ElongDegree)
		return -1;

	double MyLoc, UrLoc;
	
	vector<GM2DVector> CrossVecs;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResutlArr;
	long ResultIdx = 0;
	long res = pMyLine->IntersectEnd2(pUrLine, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, MSFloor::ms_LineHitDistTol);
	
	if(abs(res%10)==1) // 내부 교차
	{
		MyCrossVec = CrossVecs[ResultIdx];
		MyLoc = MyLocArr[ResultIdx];
		UrLoc = UrLocArr[ResultIdx];
		double dLoc1 = pMyLine->OrthoBaseLocValue(MyCrossVec);
		double dLoc2 = pUrLine->OrthoBaseLocValue(MyCrossVec);

		if(MyEndNum == 0 && MyLoc > 1 - DTOL_GM)
			return -1;
		else if(MyEndNum == 1 && MyLoc < 0 - DTOL_GM)
			return -1;

		if(UrEndNum == 0 && UrLoc > 1 - DTOL_GM)
			return -1;
		else if(UrEndNum == 1 && UrLoc < 0 - DTOL_GM)
			return -1;

		if(MyEndNum == 0 && ( dLoc1 < 0 || pMyMember->m_CenLineSetCnt == 0) )
		{
			pMyLine->m_P0.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
			if(pMyLine == pMyMember->mp_CenLine)
				pMyMember->m_CenLineSetCnt++;
		}
		else if(MyEndNum == 1 && ( dLoc1 > 1 || pMyMember->m_CenLineSetCnt == 0) )
		{
			pMyLine->m_P1.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
			if(pMyLine == pMyMember->mp_CenLine)
				pMyMember->m_CenLineSetCnt++;
		}

		if(UrEndNum == 0 && ( dLoc2 < 0 || pUrMember->m_CenLineSetCnt == 0) )
		{
			pUrLine->m_P0.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
			if(pUrLine == pUrMember->mp_CenLine)
				pUrMember->m_CenLineSetCnt++;
		}
		else if(UrEndNum == 1 && ( dLoc2 > 1 || pUrMember->m_CenLineSetCnt == 0) )
		{
			pUrLine->m_P1.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
			if(pUrLine == pUrMember->mp_CenLine)
				pUrMember->m_CenLineSetCnt++;
		}

	}
	else if(abs(res%10)==2)
	{

	}

}

bool MSConnLevelZone::TrimSection22( GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool IsFloorZone/*=true*/ )
{
	GM2DCurve *pMyLineIn = NULL, *pMyFullLineIn=NULL;
	GM2DCurve *pUrLineIn = NULL, *pUrFullLineIn=NULL;
	GM2DCurve *pMyLineOut = NULL, *pMyFullLineOut=NULL;
	GM2DCurve *pUrLineOut = NULL, *pUrFullLineOut=NULL;

	GM2DVector* pMyEndPoint = NULL, *pUrEndPoint = NULL;
	long MyEndNum, UrEndNum;
	bool bMyEndIsMidPoint, bUrEndIsMidPoint;

	double DefELongDeg = 60.;
	double ELongDeg = 0.;
	bool isLineLevelDifG = false;
	bool bTrimFail = false;

	bool bOnlyInCross = false;
	bool bOnlyShorten = false;
	CArray<long, long> bLLineCutArr;
	CArray<long, long> bRLineCutArr;
	long *pMyInCut, *pMyOutCut, *pUrInCut, *pUrOutCut;

	m_ColinerMemMap.clear();
	long NumMemPart = MemPartArr.GetSize();
	long NumLinkMember = mp_Conn->GetNumMemberG();

	if(NumMemPart == 3 && NumLinkMember == 2)
		m_UseTramInOut = false;

	for(int i=0; i< NumMemPart ; i++)
	{
		bLLineCutArr.Add(0);
		bRLineCutArr.Add(0);
		MSLinkMemPart* pMyMemPart = MemPartArr[i];
		pMyMemPart->m_CenLineSetCnt = 0;
	}

	for(int i=0; i< NumMemPart ; i++)
    {
        MSLinkMemPart* pMyMemPart = MemPartArr[i];
        if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
        {
            pMyEndPoint = &(pMyMemPart->mp_LLine->m_P1);
            MyEndNum = 1;
//             if(mp_Conn->IsSameOrUpDown(pMyMemPart->mp_LinkMem->GetEConn()))
//                 bMyEndIsMidPoint = false;
//             else
                bMyEndIsMidPoint = true;
        }
        else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
        {
            pMyEndPoint = &(pMyMemPart->mp_LLine->m_P0);
            MyEndNum = 0;
//             if(mp_Conn->IsSameOrUpDown(pMyMemPart->mp_LinkMem->GetSConn()))
//                 bMyEndIsMidPoint = false;
//             else
                bMyEndIsMidPoint = true;
        }
        else
            ASSERT(0);

        double MyDegree = pMyMemPart->GetDegree(MyEndNum);

		//if(pMyMemPart->mp_LinkMem->m_hasDelPart == true)
			//m_ColinerMemMap.insert(make_pair(pMyMemPart->mp_LinkMem->m_ID, pMyMemPart->mp_LinkMem->m_ID));

        //for(int j = i+1; j< nMem ;j++)
		//for(int j = i+1 ; j < NumMemPart ;j++)

		//vector<GM2DCurve*> UrLCurveArr;
		//vector<GM2DCurve*> UrRCurveArr;

		for(int j = ( i == NumMemPart-1 ? 0 : i+1 ) ; j < ( i == NumMemPart-1 ? 1 : NumMemPart ) ;j++)
        {
			MSLinkMemPart* pUrMemPart = MemPartArr[j];
			//부재가 2개일경우 처리 하지 않는다.(ex J = 0, i = 1 ) 
			//둘중 하나가 아크 부재이면 처리한다.
			//if(nMem == 2 && j == 0 && !pMyMemPart->mp_LinkMem->isArc() && !pUrMemPart->mp_LinkMem->isArc() ) 
			//	break;

            // 같은 링크부재의 파트일 경우 트림에서 제외
            if(pMyMemPart == pUrMemPart || pMyMemPart->mp_LinkMem==pUrMemPart->mp_LinkMem)
            {
				m_ColinerMemMap.insert(make_pair(pMyMemPart->mp_LinkMem->m_ID, pMyMemPart->mp_LinkMem->m_ID));
				m_ColinerMemMap.insert(make_pair(pUrMemPart->mp_LinkMem->m_ID, pUrMemPart->mp_LinkMem->m_ID));
				bLLineCutArr[i] = 1;
				bRLineCutArr[i] = 1;
				bLLineCutArr[j] = 1;
				bRLineCutArr[j] = 1;
				continue;
			}

            if(pMyMemPart->IsWallAndGirderWith(pUrMemPart))
               continue;

            if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetEConn()))
            {
                pUrEndPoint = &(pUrMemPart->mp_LLine->m_P1);
                UrEndNum = 1;
//                 if(mp_Conn->IsSameOrUpDown(pUrMemPart->mp_LinkMem->GetEConn()))
//                     bUrEndIsMidPoint = false;
//                 else
                    bUrEndIsMidPoint = true;
            }
            else if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetSConn()))
            {
                pUrEndPoint = &(pUrMemPart->mp_LLine->m_P0);
                UrEndNum = 0;
//                 if(mp_Conn->IsSameOrUpDown(pUrMemPart->mp_LinkMem->GetSConn()))
//                     bUrEndIsMidPoint = false;
//                 else
                    bUrEndIsMidPoint = true;
            }
            else
                ASSERT(0);

			// 교차 검사
			


            double UrDegree = pUrMemPart->GetDegree(UrEndNum);
            double InnerDegree = UrDegree - MyDegree;
			
			if(InnerDegree < 0)
				InnerDegree += 360;


            {
				char MyInSide, MyOutSide, UrInSide, UrOutSide;

				if(MyEndNum==0)
				{
					pMyLineIn = pMyMemPart->mp_LLine;
					pMyFullLineIn = pMyMemPart->mp_LinkMem->mp_LLine;
					pMyLineOut = pMyMemPart->mp_RLine;
					pMyFullLineOut = pMyMemPart->mp_LinkMem->mp_RLine;
					MyInSide = 'L';
					MyOutSide = 'R';
					pMyInCut = &bLLineCutArr[i];
					pMyOutCut = &bRLineCutArr[i];
				}
				else
				{
					pMyLineIn = pMyMemPart->mp_RLine;
					pMyFullLineIn = pMyMemPart->mp_LinkMem->mp_RLine;
					pMyLineOut = pMyMemPart->mp_LLine;
					pMyFullLineOut = pMyMemPart->mp_LinkMem->mp_LLine;
					MyInSide = 'R';
					MyOutSide = 'L';
					pMyInCut = &bRLineCutArr[i];
					pMyOutCut = &bLLineCutArr[i];
				}
				if(UrEndNum==0)
				{
					pUrLineIn = pUrMemPart->mp_RLine;
					pUrFullLineIn = pUrMemPart->mp_LinkMem->mp_RLine;
					pUrLineOut = pUrMemPart->mp_LLine;
					pUrFullLineOut = pUrMemPart->mp_LinkMem->mp_LLine;
					UrInSide = 'R';
					UrOutSide = 'L';
					pUrInCut = &bRLineCutArr[j];
					pUrOutCut = &bLLineCutArr[j];
				}
				else
				{
					pUrLineIn = pUrMemPart->mp_LLine;
					pUrFullLineIn = pUrMemPart->mp_LinkMem->mp_LLine;
					pUrLineOut = pUrMemPart->mp_RLine;
					pUrFullLineOut = pUrMemPart->mp_LinkMem->mp_RLine;
					UrInSide = 'L';
					UrOutSide = 'R';
					pUrInCut = &bLLineCutArr[j];
					pUrOutCut = &bRLineCutArr[j];
				}


				CutLineMemPart(pMyLineIn, pUrLineIn, pMyMemPart, pUrMemPart, MyEndNum, UrEndNum, 0);
				CutLineMemPart(pMyLineIn, pUrLineOut, pMyMemPart, pUrMemPart, MyEndNum, UrEndNum, 0);
				CutLineMemPart(pMyLineOut, pUrLineIn, pMyMemPart, pUrMemPart, MyEndNum, UrEndNum, 0);
				CutLineMemPart(pMyLineOut, pUrLineOut, pMyMemPart, pUrMemPart, MyEndNum, UrEndNum, 60);
				CutLineMemPart(pMyMemPart->mp_CenLine, pUrMemPart->mp_CenLine, pMyMemPart, pUrMemPart, MyEndNum, UrEndNum, InnerDegree);

				continue;

				if(pMyMemPart->mp_LinkMem->GetType() == MSElement::msLineLevelDifG && pUrMemPart->mp_LinkMem->GetType() == MSElement::msLineLevelDifG)
				{
					ELongDeg = 0.;
					isLineLevelDifG = true;
				}
				else
				{
					ELongDeg = DefELongDeg;
					isLineLevelDifG = false;
				}

				bOnlyInCross = false;
				bOnlyShorten = false;

				if( fabs(InnerDegree) <= MSFloor::ms_ColinearDegreeTol 
					&& pMyMemPart->mp_LinkMem->isArc() != pUrMemPart->mp_LinkMem->isArc() )
				{
					// 추후에 처리 함
					bTrimFail = true;
				}
				else if(pMyMemPart->mp_LinkMem->isArc() == false && pUrMemPart->mp_LinkMem->isArc() == false)
				{
					long MyInUrIn;
					if(isLineLevelDifG == true)
					{
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
					}
					else
					{
						if(pMyMemPart->mp_LinkMem->m_isBuriedLinkMember || pUrMemPart->mp_LinkMem->m_isBuriedLinkMember)
						{
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
						}
						else
						{
							if(*pMyInCut == 0 || *pUrInCut == 0)
								MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
							//MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
							if( (*pMyOutCut == 0 || *pUrOutCut == 0) && (NumMemPart == 2/* || m_UseTramInOut*/))
								MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
							//MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						}
					}
				}
				else
				{
					long MyInUrIn;
					if(isLineLevelDifG == true)
					{
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
					}
					else
					{
						if(*pMyInCut == 0 || *pUrInCut == 0)
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						//MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
						if((*pMyOutCut == 0 || *pUrOutCut == 0) /*&& (NumMemPart == 2 || m_UseTramInOut)*/ )
							MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, bOnlyInCross, bOnlyShorten, ELongDeg);
						//MyInUrIn = CutLineWhenShortening2(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, bOnlyInCross, bOnlyShorten, 0.);
					}
					if( !(NumLinkMember >= 2 && NumMemPart >= 4 && bMyEndIsMidPoint == true && bUrEndIsMidPoint == true)) // 2개의 아크 부재가 미드 조인트에서 겹칠경우 아래 로직 안들어감
					{

					}
					else
					{

					}
				}
            }
        }
    }

	for(int i=0; i< NumMemPart ; i++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[i];

// 		if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
// 			MyEndNum = 1;
// 		else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
// 			MyEndNum = 0;
// 		else
// 			ASSERT(0);

		MSPointMemberG* pPointMem=NULL;
		if(mp_Column)
			pPointMem = mp_Column;
		if(mp_IsoFoot)
			pPointMem = mp_IsoFoot;

		if(pPointMem && mp_DownLinkJointBoundry == NULL)
		{
			GM2DPolyline MyPoly(TRUE);
			pPointMem->GetBoundary(MyPoly);
			//MyPoly.Trim(pMyMemPart->mp_LLine, MyEndNum, !bLLineCutArr[i]);
			//MyPoly.Trim(pMyMemPart->mp_RLine, MyEndNum, !bRLineCutArr[i]);
			MyPoly.Trim(pMyMemPart->mp_LLine, MyEndNum, false);
			MyPoly.Trim(pMyMemPart->mp_RLine, MyEndNum, false);
		}

		if(mp_DownLinkJointBoundry != NULL)
		{
			bool bAllowExtend = false;

			//if(!bLLineCutArr[i])
				bool bLLineRVal = mp_DownLinkJointBoundry->Trim(pMyMemPart->mp_LLine, MyEndNum, bAllowExtend);
			//if(!bRLineCutArr[i])
				bool bRLineRVal = mp_DownLinkJointBoundry->Trim(pMyMemPart->mp_RLine, MyEndNum, bAllowExtend);
		}
	}

	if(bTrimFail == true)
	{
		for(int i=0; i< NumMemPart ; i++)
		{
			MSLinkMemPart *pLink = ma_MemPart[i];
			//pLink->SetLinesOfAMemPart(pLink->GetSConn() == mp_Conn, pLink->GetEConn() == mp_Conn);

			/*
			// 끝선 그리기...
			if(pLink->mp_LinkMem->GetSConn() == mp_Conn || pLink->mp_LinkMem->GetEConn() == mp_Conn)
			{
				long StartOrEnd = pLink->GetSConn()==mp_Conn ? 0 : 1;
				GM2DPolyline* pEndPoly = new GM2DPolyline(TRUE);
				GM2DPolyline* pLinkPoly = pLink->GetBoundary(FALSE);
				GM2DLineVector EndLine = pLink->GetEndLine(StartOrEnd, FALSE);
				pEndPoly->AddLine(EndLine);
				pLink->AddEndJointPoly(this, StartOrEnd, pEndPoly);
			}
			*/
		}
		return false;
	}

	return TrimSection2_2(TheSection, DrawSection, MemPartArr);
	//return BuildSectionOnGivenMemPart(TheSection, MemPartArr);
	//return true;
}

bool MSConnLevelZone::TrimSection( GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool IsFloorZone/*=true*/ )
{
    GM2DCurve *pMyLineIn = NULL, *pMyFullLineIn=NULL;
    GM2DCurve *pUrLineIn = NULL, *pUrFullLineIn=NULL;
    GM2DCurve *pMyLineOut = NULL, *pMyFullLineOut=NULL;
    GM2DCurve *pUrLineOut = NULL, *pUrFullLineOut=NULL;
    long *pMyInCut = 0, *pMyOutCut = 0, *pUrInCut = 0, *pUrOutCut = 0;
    GM2DVector* pMyEndPoint = NULL, *pUrEndPoint = NULL;
    GM2DLineVector* MyEndLine=NULL, *UrEndLine=NULL;
    long MyEndNum = 0, UrEndNum = 0;
    bool bMyEndIsMidPoint = false, bUrEndIsMidPoint = false;
	double DefELongDeg = 60.;
	double ELongDeg = 0.;
	bool isLineLevelDifG = false;
	
	long NumLinkMember = GetNumMemberG();
    CArray<long, long> bLLineCutArr;
    CArray<long, long> bRLineCutArr;
    long nMem = MemPartArr.GetSize();
	m_ColinerMemMap.clear();

	bool bTrimFail = false;
    for(int i=0; i< nMem ; i++)
    {
        bLLineCutArr.Add(0);
        bRLineCutArr.Add(0);
		MSLinkMemPart* pMemPart = MemPartArr[i];
		pMemPart->m_CenLineSetCnt = 0;
    }

	// 평행한거 트림
    for(int i=0; i< nMem ; i++)
    {
        MSLinkMemPart* pMyMemPart = MemPartArr[i];
        if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
        {
            pMyEndPoint = &(pMyMemPart->mp_LLine->m_P1);
            MyEndNum = 1;
        }
        else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
        {
            pMyEndPoint = &(pMyMemPart->mp_LLine->m_P0);
            MyEndNum = 0;
        }
        else
            ASSERT(0);

        double MyDegree = pMyMemPart->GetDegree(MyEndNum);

        for(int j = i+1; j< nMem ;j++)
		//for(int j = ( i == nMem-1 ? 0 : i+1 ) ; j < ( i == nMem-1 ? 1 : nMem ) ;j++) // 마지막 부재와 첫부재 트림
        {
			MSLinkMemPart* pUrMemPart = MemPartArr[j];
			//부재가 2개일경우 처리 하지 않는다.(ex J = 0, i = 1 ) 
			//둘중 하나가 아크 부재이면 처리한다.
			if(nMem == 2 && j == 0 && !pMyMemPart->mp_LinkMem->isArc() && !pUrMemPart->mp_LinkMem->isArc() ) // 마지막 부재와 첫부재 트림
				break;

            // 같은 링크부재의 파트일 경우 트림에서 제외
            if(pMyMemPart == pUrMemPart || pMyMemPart->mp_LinkMem==pUrMemPart->mp_LinkMem)
            {
				m_ColinerMemMap.insert(make_pair(pMyMemPart->mp_LinkMem->m_ID, pMyMemPart->mp_LinkMem->m_ID));
				continue;
			}

            if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetEConn()))
            {
                pUrEndPoint = &(pUrMemPart->mp_LLine->m_P1);
                UrEndNum = 1;
            }
            else if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetSConn()))
            {
                pUrEndPoint = &(pUrMemPart->mp_LLine->m_P0);
                UrEndNum = 0;
            }
            else
                ASSERT(0);

            double UrDegree = pUrMemPart->GetDegree(UrEndNum);
            double InnerDegree = UrDegree - MyDegree;

			//if( ( InnerDegree <= MSFloor::ms_ColinearDegreeTol )  || (fabs( InnerDegree -180.) <= MSFloor::ms_ColinearDegreeTol )
			if( fabs(fabs(InnerDegree)-180.) <= MSFloor::ms_ColinearDegreeTol 
				&& pMyMemPart->mp_LinkMem->isArc() == false && pUrMemPart->mp_LinkMem->isArc() == false)
            {
				m_ColinerMemMap.insert(make_pair(pMyMemPart->mp_LinkMem->m_ID, pMyMemPart->mp_LinkMem->m_ID));
				m_ColinerMemMap.insert(make_pair(pUrMemPart->mp_LinkMem->m_ID, pUrMemPart->mp_LinkMem->m_ID));

				bool nInOnlyShort = false;
				bool nOutOnlyShort = false;

				if(mp_Column != NULL || mp_IsoFoot != NULL)
				{
					nInOnlyShort = true;
					nOutOnlyShort = true;
				}

                if(MyEndNum!=UrEndNum)
                {
                    CutParallelLine(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum, pUrMemPart, 'L', pUrMemPart->mp_LLine, UrEndNum, nInOnlyShort);
                    CutParallelLine(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum, pUrMemPart, 'R', pUrMemPart->mp_RLine, UrEndNum, nOutOnlyShort);
                }
                else
                {
                    CutParallelLine(pMyMemPart, 'L', pMyMemPart->mp_LLine, MyEndNum, pUrMemPart, 'R', pUrMemPart->mp_RLine, UrEndNum, nOutOnlyShort);
                    CutParallelLine(pMyMemPart, 'R', pMyMemPart->mp_RLine, MyEndNum, pUrMemPart, 'L', pUrMemPart->mp_LLine, UrEndNum, nInOnlyShort);
                }
                bLLineCutArr[i]=1;
                bLLineCutArr[j]=1;
                bRLineCutArr[i]=1;
                bRLineCutArr[j]=1;
            }
            else
            {
				continue;
            }
        }
    }

	// 평행하지 않은거 트림
	for(int i=0; i< nMem ; i++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[i];
		if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
		{
			pMyEndPoint = &(pMyMemPart->mp_LLine->m_P1);
			MyEndNum = 1;
// 			if(mp_Conn->IsSameOrUpDown(pMyMemPart->mp_LinkMem->GetEConn()))
// 				bMyEndIsMidPoint = false;
// 			else
				bMyEndIsMidPoint = true;
		}
		else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
		{
			pMyEndPoint = &(pMyMemPart->mp_LLine->m_P0);
			MyEndNum = 0;
// 			if(mp_Conn->IsSameOrUpDown(pMyMemPart->mp_LinkMem->GetSConn()))
// 				bMyEndIsMidPoint = false;
// 			else
				bMyEndIsMidPoint = true;
		}
		else
			ASSERT(0);

		double MyDegree = pMyMemPart->GetDegree(MyEndNum);

		//for(int j = i+1; j< nMem ;j++)
		for(int j = ( i == nMem-1 ? 0 : i+1 ) ; j < ( i == nMem-1 ? 1 : nMem ) ;j++) // 마지막 부재와 첫부재 트림
		{
			MSLinkMemPart* pUrMemPart = MemPartArr[j];
			//부재가 2개일경우 처리 하지 않는다.(ex J = 0, i = 1 ) 
			//둘중 하나가 아크 부재이면 처리한다.
			if(nMem == 2 && j == 0 && !pMyMemPart->mp_LinkMem->isArc() && !pUrMemPart->mp_LinkMem->isArc() ) // 마지막 부재와 첫부재 트림
				break;

			// 같은 링크부재의 파트일 경우 트림에서 제외
			if(pMyMemPart == pUrMemPart || pMyMemPart->mp_LinkMem==pUrMemPart->mp_LinkMem)
			{
				m_ColinerMemMap.insert(make_pair(pMyMemPart->mp_LinkMem->m_ID, pMyMemPart->mp_LinkMem->m_ID));
				continue;
			}

			if(pMyMemPart->IsWallAndGirderWith(pUrMemPart))
				continue;

			if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetEConn()))
			{
				pUrEndPoint = &(pUrMemPart->mp_LLine->m_P1);
				UrEndNum = 1;
// 				if(mp_Conn->IsSameOrUpDown(pUrMemPart->mp_LinkMem->GetEConn()))
// 					bUrEndIsMidPoint = false;
// 				else
					bUrEndIsMidPoint = true;
			}
			else if(mp_Conn->IsSameOrUpDown(pUrMemPart->GetSConn()))
			{
				pUrEndPoint = &(pUrMemPart->mp_LLine->m_P0);
				UrEndNum = 0;
// 				if(mp_Conn->IsSameOrUpDown(pUrMemPart->mp_LinkMem->GetSConn()))
// 					bUrEndIsMidPoint = false;
// 				else
					bUrEndIsMidPoint = true;
			}
			else
				ASSERT(0);

			double UrDegree = pUrMemPart->GetDegree(UrEndNum);
			double InnerDegree = UrDegree - MyDegree;

			//if( ( InnerDegree <= MSFloor::ms_ColinearDegreeTol )  || (fabs( InnerDegree -180.) <= MSFloor::ms_ColinearDegreeTol )
			if( fabs(fabs(InnerDegree)-180.) <= MSFloor::ms_ColinearDegreeTol 
				&& pMyMemPart->mp_LinkMem->isArc() == false && pUrMemPart->mp_LinkMem->isArc() == false)
			{
				continue;
			}
			else
			{
				if(pMyMemPart->isBuriedLinkMember() || pUrMemPart->isBuriedLinkMember())
				{
					double dMinDist = MAXINT;

					double dDist00 = pMyMemPart->mp_CenLine->m_P0.Distance(pUrMemPart->mp_CenLine->m_P0);
					double dDist01 = pMyMemPart->mp_CenLine->m_P0.Distance(pUrMemPart->mp_CenLine->m_P1);
					double dDist10 = pMyMemPart->mp_CenLine->m_P1.Distance(pUrMemPart->mp_CenLine->m_P0);
					double dDist11 = pMyMemPart->mp_CenLine->m_P1.Distance(pUrMemPart->mp_CenLine->m_P1);

					dMinDist = min(dMinDist, dDist00);
					dMinDist = min(dMinDist, dDist01);
					dMinDist = min(dMinDist, dDist10);
					dMinDist = min(dMinDist, dDist11);

					if(fabs(dMinDist - dDist00) < DTOL_GM)
					{
						MyEndNum = 0;
						UrEndNum = 0;
					}
					else if(fabs(dMinDist - dDist01) < DTOL_GM)
					{
						MyEndNum = 0;
						UrEndNum = 1;
					}
					else if(fabs(dMinDist - dDist10) < DTOL_GM)
					{
						MyEndNum = 1;
						UrEndNum = 0;
					}
					else if(fabs(dMinDist - dDist11) < DTOL_GM)
					{
						MyEndNum = 1;
						UrEndNum = 1;
					}
				}

				char MyInSide, MyOutSide, UrInSide, UrOutSide;
				if(InnerDegree < (180.-MSFloor::ms_ColinearDegreeTol))
				{
					if(MyEndNum==0)
					{
						pMyLineIn = pMyMemPart->mp_LLine;
						pMyFullLineIn = pMyMemPart->mp_LinkMem->mp_LLine;
						pMyLineOut = pMyMemPart->mp_RLine;
						pMyFullLineOut = pMyMemPart->mp_LinkMem->mp_RLine;
						MyInSide = 'L';
						MyOutSide = 'R';
						pMyInCut = &bLLineCutArr[i];
						pMyOutCut = &bRLineCutArr[i];
					}
					else
					{
						pMyLineIn = pMyMemPart->mp_RLine;
						pMyFullLineIn = pMyMemPart->mp_LinkMem->mp_RLine;
						pMyLineOut = pMyMemPart->mp_LLine;
						pMyFullLineOut = pMyMemPart->mp_LinkMem->mp_LLine;
						MyInSide = 'R';
						MyOutSide = 'L';
						pMyInCut = &bRLineCutArr[i];
						pMyOutCut = &bLLineCutArr[i];
					}
					if(UrEndNum==0)
					{
						pUrLineIn = pUrMemPart->mp_RLine;
						pUrFullLineIn = pUrMemPart->mp_LinkMem->mp_RLine;
						pUrLineOut = pUrMemPart->mp_LLine;
						pUrFullLineOut = pUrMemPart->mp_LinkMem->mp_LLine;
						UrInSide = 'R';
						UrOutSide = 'L';
						pUrInCut = &bRLineCutArr[j];
						pUrOutCut = &bLLineCutArr[j];
					}
					else
					{
						pUrLineIn = pUrMemPart->mp_LLine;
						pUrFullLineIn = pUrMemPart->mp_LinkMem->mp_LLine;
						pUrLineOut = pUrMemPart->mp_RLine;
						pUrFullLineOut = pUrMemPart->mp_LinkMem->mp_RLine;
						UrInSide = 'L';
						UrOutSide = 'R';
						pUrInCut = &bLLineCutArr[j];
						pUrOutCut = &bRLineCutArr[j];
					}
				}
				else
				{
					if(MyEndNum==0)
					{
						pMyLineIn = pMyMemPart->mp_RLine;
						pMyFullLineIn = pMyMemPart->mp_LinkMem->mp_RLine;
						pMyLineOut = pMyMemPart->mp_LLine;
						pMyFullLineOut = pMyMemPart->mp_LinkMem->mp_LLine;
						MyInSide = 'R';
						MyOutSide = 'L';
						pMyInCut = &bRLineCutArr[i];
						pMyOutCut = &bLLineCutArr[i];
					}
					else
					{
						pMyLineIn = pMyMemPart->mp_LLine;
						pMyFullLineIn = pMyMemPart->mp_LinkMem->mp_LLine;
						pMyLineOut = pMyMemPart->mp_RLine;
						pMyFullLineOut = pMyMemPart->mp_LinkMem->mp_RLine;
						MyInSide = 'L';
						MyOutSide = 'R';
						pMyInCut = &bLLineCutArr[i];
						pMyOutCut = &bRLineCutArr[i];
					}
					if(UrEndNum==0)
					{
						pUrLineIn = pUrMemPart->mp_LLine;
						pUrFullLineIn = pUrMemPart->mp_LinkMem->mp_LLine;
						pUrLineOut = pUrMemPart->mp_RLine;
						pUrFullLineOut = pUrMemPart->mp_LinkMem->mp_RLine;
						UrInSide = 'L';
						UrOutSide = 'R';
						pUrInCut = &bLLineCutArr[j];
						pUrOutCut = &bRLineCutArr[j];
					}
					else
					{
						pUrLineIn = pUrMemPart->mp_RLine;
						pUrFullLineIn = pUrMemPart->mp_LinkMem->mp_RLine;
						pUrLineOut = pUrMemPart->mp_LLine;
						pUrFullLineOut = pUrMemPart->mp_LinkMem->mp_LLine;
						UrInSide = 'R';
						UrOutSide = 'L';
						pUrInCut = &bRLineCutArr[j];
						pUrOutCut = &bLLineCutArr[j];
					}
				}

				if(pMyMemPart->mp_LinkMem->GetType() == MSElement::msLineLevelDifG && pUrMemPart->mp_LinkMem->GetType() == MSElement::msLineLevelDifG)
				{
					ELongDeg = 0.;
					isLineLevelDifG = true;
				}
				else
				{
					ELongDeg = DefELongDeg;
					isLineLevelDifG = false;
				}

				if( fabs(InnerDegree) <= MSFloor::ms_ColinearDegreeTol 
					&& pMyMemPart->mp_LinkMem->isArc() != pUrMemPart->mp_LinkMem->isArc() )
				{
					// 추후에 처리 함
					bTrimFail = true;
				}
				else if(pMyMemPart->mp_LinkMem->isArc() == false && pUrMemPart->mp_LinkMem->isArc() == false)
				{
					long MyInUrIn = 0;
					if(isLineLevelDifG == true)
					{
						MyInUrIn = CutLineWhenShortening(pMyMemPart, MyInSide, pMyFullLineIn, MyEndNum, pUrMemPart, UrInSide, pUrFullLineIn, UrEndNum, false, false, 0.);
						
						if(MyInUrIn == 1)
						{
							*pMyInCut = 1;
							*pUrInCut = 1;

							if(nMem==2 || (*pMyOutCut==0 && *pUrOutCut==0 && i+1 == j))
							{
								long MyOutUrOut = CutLineWhenShortening(pMyMemPart, MyOutSide, pMyFullLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrFullLineOut, UrEndNum, false, false, ELongDeg);
								if(MyOutUrOut==1 || MyOutUrOut==2)
								{
									*pMyOutCut = 1;
									*pUrOutCut = 1;
								}
							}
						}
					}
					else
					{
						if(pMyMemPart->isBuriedLinkMember() || pUrMemPart->isBuriedLinkMember())
						{
							MyInUrIn = CutLineWhenShorteningByBuried(pMyMemPart, pUrMemPart);
							continue;
						}

						if(m_hasBuriedLinkMember && NumLinkMember == 3)
							continue;

						//if(i+1==j || (i==0 && j== nMem-1))
						if(i+1==j || (i==0 && j== nMem-1) || j == 0) // 마지막 부재와 첫부재 트림
							//if(abs(i - j) == 1 || (i==0 && j== nMem-1))
								if(*pMyInCut==0 && *pUrInCut==0)
									//if(*pMyInCut==0 && *pUrInCut==0)
										MyInUrIn = CutLineWhenShortening(pMyMemPart, MyInSide, pMyFullLineIn, MyEndNum, pUrMemPart, UrInSide, pUrFullLineIn, UrEndNum, false, false, 0.);
								else
									MyInUrIn = CutLineWhenShortening(pMyMemPart, MyInSide, pMyFullLineIn, MyEndNum, pUrMemPart, UrInSide, pUrFullLineIn, UrEndNum, false, true, 0.);
						else
							MyInUrIn = CutLineWhenShortening(pMyMemPart, MyInSide, pMyFullLineIn, MyEndNum, pUrMemPart, UrInSide, pUrFullLineIn, UrEndNum, true, true);

						if(MyInUrIn==1)
						{
							*pMyInCut = 1;
							*pUrInCut = 1;
							long MyInUrOut = CutLineWhenShortening(pMyMemPart, MyInSide, pMyFullLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrFullLineOut, UrEndNum, true, true);
							if(MyInUrOut==1)
								*pUrOutCut = 1;
							long UrInMyOut = CutLineWhenShortening(pUrMemPart, UrInSide, pUrFullLineIn, UrEndNum, pMyMemPart, MyOutSide, pMyFullLineOut, MyEndNum, true, true);
							if(UrInMyOut==1)
								*pMyOutCut = 1;
							//if(nMem==2 || (*pMyOutCut==0 && *pUrOutCut==0 && i+1 == j)) // 정확한 이유를 모르겠음. 2번째 라인이 아닌 마지막 라인하고 해야하지 않은지..
							//if( nMem==2 || ( ( nMem == j + 1 ) && i == 0 ) || (*pMyOutCut==0 && *pUrOutCut==0 && i+1 == j))
							//if(nMem==2 )
							if(nMem==2 || (*pMyOutCut==0 && *pUrOutCut==0 && i+1 == j))
							{
								long MyOutUrOut = CutLineWhenShortening(pMyMemPart, MyOutSide, pMyFullLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrFullLineOut, UrEndNum, false, false, ELongDeg);
								if(MyOutUrOut==1 || MyOutUrOut==2)
								{
									*pMyOutCut = 1;
									*pUrOutCut = 1;
								}
							}
						}
					}
					
				}
				else
				{
					long MyInUrIn = 0;
					if(isLineLevelDifG == true)
					{
						MyInUrIn = CutLineWhenShortening(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, false, false, 0.);
					}
					else
					{
						//if(i+1==j || (i==0 && j== nMem-1))
						if(i+1==j || (i==0 && j== nMem-1) || j == 0) // 마지막 부재와 첫부재 트림
							//if(abs(i - j) == 1 || (i==0 && j== nMem-1))
								if(*pMyInCut==0 && *pUrInCut==0)
									//if(*pMyInCut==0 && *pUrInCut==0)
										MyInUrIn = CutLineWhenShortening(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, false, false, 0.);
								else
									MyInUrIn = CutLineWhenShortening(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, false, true, 0.);
						else
							MyInUrIn = CutLineWhenShortening(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrInSide, pUrLineIn, UrEndNum, true, true);
					}
					if( !(NumLinkMember >= 2 && nMem >= 4 && bMyEndIsMidPoint == true && bUrEndIsMidPoint == true)) // 2개의 아크 부재가 미드 조인트에서 겹칠경우 아래 로직 안들어감
					{
						if(MyInUrIn==1)
						{
							*pMyInCut = 1;
							*pUrInCut = 1;
							// 수정 필요??
							long MyInUrOut = CutLineWhenShortening(pMyMemPart, MyInSide, pMyLineIn, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, true, true);
							if(MyInUrOut==1)
								*pUrOutCut = 1;
							//
							long UrInMyOut = CutLineWhenShortening(pUrMemPart, UrInSide, pUrLineIn, UrEndNum, pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, true, true);
							if(UrInMyOut==1)
								*pMyOutCut = 1;
							//if(nMem==2) 
							//if(nMem==2 || (*pMyOutCut==0 && *pUrOutCut==0 && i+1 == j))
							//if( nMem==2 || ( ( nMem == j + 1 ) && i == 0 ) || (*pMyOutCut==0 && *pUrOutCut==0 && i+1 == j))
							if(nMem==2 || (*pMyOutCut==0 && *pUrOutCut==0 && i+1 == j))
							{
								long MyOutUrOut = CutLineWhenShortening(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, false, false, ELongDeg);
								if(MyOutUrOut==1 || MyOutUrOut==2)
								{
									*pMyOutCut = 1;
									*pUrOutCut = 1;
								}
							}
						}
					}
					else
					{
						if(MyInUrIn == 0) // pMyLineIn pUrLineIn 이 서로 교차 되지 않을경우에 처리함
						{
							long MyOutUrOut = CutLineWhenShortening(pMyMemPart, MyOutSide, pMyLineOut, MyEndNum, pUrMemPart, UrOutSide, pUrLineOut, UrEndNum, true, true);
							if(MyOutUrOut==1 || MyOutUrOut==2)
							{
								*pMyOutCut = 1;
								*pUrOutCut = 1;
							}
						}
					}
				}
			}
		}
	}

// 	bool isTrim = false;
// 	if(mp_DownLinkJointBoundry != NULL && !mp_DownLinkJointBoundry->Contains(&TheSection) && !mp_DownLinkJointBoundry->Contains(&DrawSection))
// 		isTrim = true;

	MSPointMemberG* pPointMem=NULL;
	if(mp_Column)
		pPointMem = mp_Column;
	if(mp_IsoFoot)
		pPointMem = mp_IsoFoot;

	// 수직 부재와 트림하기 전에 조인트 생성 이후 수직부재와 조인트 머지
// 	if(pPointMem) // 1: 기둥 있으면 조인트 먼저 생성
// 		bool bRval = BuildSectionOnGivenMemPart1(TheSection, DrawSection, MemPartArr);

	for(int i=0; i< nMem ; i++)
	{
		MSLinkMemPart* pMyMemPart = MemPartArr[i];

		if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetEConn()))
			MyEndNum = 1;
		else if(mp_Conn->IsSameOrUpDown(pMyMemPart->GetSConn()))
			MyEndNum = 0;
		else
			ASSERT(0);

		if(pPointMem)
		{
			GM2DPolyline MyPoly(TRUE);
			pPointMem->GetBoundary(MyPoly);
			MyPoly.Trim(pMyMemPart->mp_LLine, MyEndNum, false);
			MyPoly.Trim(pMyMemPart->mp_RLine, MyEndNum, false);
		}

		if(mp_DownLinkJointBoundry != NULL)
		{
			bool bAllowExtend = false;
			bool bLLineRVal = mp_DownLinkJointBoundry->Trim(pMyMemPart->mp_LLine, MyEndNum, bAllowExtend);
			bool bRLineRVal = mp_DownLinkJointBoundry->Trim(pMyMemPart->mp_RLine, MyEndNum, bAllowExtend);
		}
	}

	if(bTrimFail == true)
	{
		for(int i=0; i< nMem ; i++)
		{
			MSLinkMemPart *pLink = ma_MemPart[i];
			pLink->SetLinesOfAMemPart(pLink->GetSConn() == mp_Conn, pLink->GetEConn() == mp_Conn);

			/*
			// 끝선 그리기...
			if(pLink->mp_LinkMem->GetSConn() == mp_Conn || pLink->mp_LinkMem->GetEConn() == mp_Conn)
			{
				long StartOrEnd = pLink->GetSConn()==mp_Conn ? 0 : 1;
				GM2DPolyline* pEndPoly = new GM2DPolyline(TRUE);
				GM2DPolyline* pLinkPoly = pLink->GetBoundary(FALSE);
				GM2DLineVector EndLine = pLink->GetEndLine(StartOrEnd, FALSE);
				pEndPoly->AddLine(EndLine);
				pLink->AddEndJointPoly(this, StartOrEnd, pEndPoly);
			}
			*/
		}
		MakeBasePoly(TheSection, true);
		MakeBasePoly(DrawSection, true);
		return true;
	}

// 	if(pPointMem)
// 		return BuildSectionOnGivenMemPart2(TheSection, DrawSection, MemPartArr);
// 	else
		return BuildSectionOnGivenMemPart(TheSection, DrawSection, MemPartArr);
}

long MSConnLevelZone::CutParallelLine(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, 
                                      MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, 
                                      bool nOnlyShort)
{
    double MyLoc, UrLoc;
    GM2DVector MyCrossVec = MyEndNum==0 ? pMyLine->GetP0() : pMyLine->GetP1();
    GM2DVector UrCrossVec = UrEndNum==0 ? pUrLine->GetP0() : pUrLine->GetP1();
	// 평행할 경우 입력된 부재 정보가 변경되기 때문에 MSFloor::ms_LineHitDistTol를 사용 하지 않는다. 
	long res = pMyLine->IntersectEnd(pUrLine, MyCrossVec, UrCrossVec, &MyLoc, &UrLoc, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, 
		MSFloor::ms_LineHitDistTol);

	if(pMyMember->mp_LinkMem->GetType() == MSElement::msLineLevelDifG)
	{
		GM2DVector CrossVec = pMyLine->OrthoBaseVector(GM2DVector(mp_Conn->m_dXc, mp_Conn->m_dYc));
		// 교차점 까지 무조건 연장함
		pMyMember->SetLineXY(MyEndNum, MyLineSide, CrossVec, false);

		CrossVec = pUrLine->OrthoBaseVector(GM2DVector(mp_Conn->m_dXc, mp_Conn->m_dYc));
		pUrMember->SetLineXY(UrEndNum, UrLineSide, CrossVec, false);
		return res;
	}

    if(res == -2)  // 평행하지만 동일 선상 아닐 때
    {
        //         bool bSameDir = MyEndNum==UrEndNum ? false : true;
        if(MyEndNum!=UrEndNum && MyLineSide==UrLineSide)
        {
            bool bIContainYou = pMyMember->mp_LinkMem->HitTestWithVector(UrEndNum==0 ? pUrLine->m_P0 : pUrLine->m_P1, 1., false);
            if(bIContainYou)
            {
                GM2DVector CrossVec = pUrLine->OrthoBaseVector(MyCrossVec);
                pUrMember->SetLineXY(UrEndNum, UrLineSide, CrossVec, nOnlyShort);

				double dLoc = pUrMember->mp_CenLine->OrthoBaseLocValue(CrossVec);

				if(UrEndNum == 0 && dLoc < 0)
					pUrMember->mp_CenLine->SetP0(pUrMember->mp_CenLine->OrthoBaseVector(CrossVec));
				else if(UrEndNum == 1 && dLoc > 1)
					pUrMember->mp_CenLine->SetP1(pUrMember->mp_CenLine->OrthoBaseVector(CrossVec));
            }
            else
            {
                GM2DVector CrossVec = pMyLine->OrthoBaseVector(UrCrossVec);
                pMyMember->SetLineXY(MyEndNum, MyLineSide, CrossVec, nOnlyShort);

				double dLoc = pMyMember->mp_CenLine->OrthoBaseLocValue(CrossVec);

				if(MyEndNum == 0 && dLoc < 0)
					pMyMember->mp_CenLine->SetP0(pMyMember->mp_CenLine->OrthoBaseVector(CrossVec));
				else if(MyEndNum == 1 && dLoc > 1)
					pMyMember->mp_CenLine->SetP1(pMyMember->mp_CenLine->OrthoBaseVector(CrossVec));
            }
        }
        if(MyEndNum==UrEndNum && MyLineSide!=UrLineSide)
        {
            bool bIContainYou = pMyMember->mp_LinkMem->HitTestWithVector(UrEndNum==0 ? pUrLine->m_P0 : pUrLine->m_P1, 1., false);
            if(bIContainYou)
            {
                GM2DVector CrossVec = pUrLine->OrthoBaseVector(MyCrossVec);
                pUrMember->SetLineXY(UrEndNum, UrLineSide, CrossVec, nOnlyShort);

				double dLoc = pUrMember->mp_CenLine->OrthoBaseLocValue(CrossVec);

				if(UrEndNum == 0 && dLoc < 0)
					pUrMember->mp_CenLine->SetP0(pUrMember->mp_CenLine->OrthoBaseVector(CrossVec));
				else if(UrEndNum == 1 && dLoc > 1)
					pUrMember->mp_CenLine->SetP1(pUrMember->mp_CenLine->OrthoBaseVector(CrossVec));
            }
            else
            {
                GM2DVector CrossVec = pMyLine->OrthoBaseVector(UrCrossVec);
                pMyMember->SetLineXY(MyEndNum, MyLineSide, CrossVec, nOnlyShort);

				double dLoc = pMyMember->mp_CenLine->OrthoBaseLocValue(CrossVec);

				if(MyEndNum == 0 && dLoc < 0)
					pMyMember->mp_CenLine->SetP0(pMyMember->mp_CenLine->OrthoBaseVector(CrossVec));
				else if(MyEndNum == 1 && dLoc > 1)
					pMyMember->mp_CenLine->SetP1(pMyMember->mp_CenLine->OrthoBaseVector(CrossVec));
            }
        }

    }
    else if(res==-1)//Overlap
    {
		GM2DLineVector TheLine;
		TheLine.m_P0 = MyCrossVec;
		TheLine.m_P1 = UrCrossVec;
		GM2DVector CenVec = TheLine.MassCenter();

		double dLoc1 = pMyMember->mp_CenLine->OrthoBaseLocValue(CenVec);
		double dLoc2 = pUrMember->mp_CenLine->OrthoBaseLocValue(CenVec);

        //GM2DVector MyCrossVec = pMyLine->OrthoBaseVector(GM2DVector(mp_Conn->m_dXc, mp_Conn->m_dYc));
		GM2DVector MyCrossVec = pMyLine->OrthoBaseVector(GM2DVector(CenVec.m_X, CenVec.m_Y));
        pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, nOnlyShort);
		//GM2DVector UrCrossVec = pUrLine->OrthoBaseVector(GM2DVector(mp_Conn->m_dXc, mp_Conn->m_dYc));
		GM2DVector UrCrossVec = pUrLine->OrthoBaseVector(GM2DVector(CenVec.m_X, CenVec.m_Y));
        pUrMember->SetLineXY(UrEndNum, UrLineSide, UrCrossVec, nOnlyShort);

		if(MyEndNum == 0 )//&& ( dLoc1 < 0/* || pMyMember->m_CenLineSetCnt == 0*/) )
		{
			pMyMember->mp_CenLine->SetP0(pMyMember->mp_CenLine->OrthoBaseVector(CenVec));
			pMyMember->m_IsParallelCenLine = true;
		}
		else if(MyEndNum == 1 )//&& ( dLoc1 > 1/* || pMyMember->m_CenLineSetCnt == 0*/) )
		{
			pMyMember->mp_CenLine->SetP1(pMyMember->mp_CenLine->OrthoBaseVector(CenVec));
			pMyMember->m_IsParallelCenLine = true;
		}

		if(UrEndNum == 0 )//&& ( dLoc2 < 0/* || pUrMember->m_CenLineSetCnt == 0*/) )
		{
			pUrMember->mp_CenLine->SetP0(pUrMember->mp_CenLine->OrthoBaseVector(CenVec));
			pUrMember->m_IsParallelCenLine = true;
		}
		else if(UrEndNum == 1 )//&& ( dLoc2 > 1/* || pUrMember->m_CenLineSetCnt == 0*/) )
		{
			pUrMember->mp_CenLine->SetP1(pUrMember->mp_CenLine->OrthoBaseVector(CenVec));
			pUrMember->m_IsParallelCenLine = true;
		}
    }
    return res;
}

long MSConnLevelZone::CutLineWhenShorteningByBuried(MSLinkMemPart* pMyMember, MSLinkMemPart* pUrMember)
{
	GM2DPolyline* pMyPoly = pMyMember->GetBoundary2();
	GM2DPolyline* pUrPoly = pUrMember->GetBoundary2();

	long MyStartOrEnd = -1;
	long UrStartOrEnd = -1;

	double dMinDist = MAXINT;

	double dDist00 = pMyMember->mp_CenLine->m_P0.Distance(pUrMember->mp_CenLine->m_P0);
	double dDist01 = pMyMember->mp_CenLine->m_P0.Distance(pUrMember->mp_CenLine->m_P1);
	double dDist10 = pMyMember->mp_CenLine->m_P1.Distance(pUrMember->mp_CenLine->m_P0);
	double dDist11 = pMyMember->mp_CenLine->m_P1.Distance(pUrMember->mp_CenLine->m_P1);

	dMinDist = min(dMinDist, dDist00);
	dMinDist = min(dMinDist, dDist01);
	dMinDist = min(dMinDist, dDist10);
	dMinDist = min(dMinDist, dDist11);

	GM2DVector MyCenMid = pMyMember->mp_CenLine->PointAtParam(0.5);
	GM2DVector UrCenMid = pUrMember->mp_CenLine->PointAtParam(0.5);

	if(fabs(dMinDist - dDist00) < DTOL_GM)
	{
		MyStartOrEnd = 0;
		UrStartOrEnd = 0;
	}
	else if(fabs(dMinDist - dDist01) < DTOL_GM)
	{
		MyStartOrEnd = 0;
		UrStartOrEnd = 1;
	}
	else if(fabs(dMinDist - dDist10) < DTOL_GM)
	{
		MyStartOrEnd = 1;
		UrStartOrEnd = 0;
	}
	else if(fabs(dMinDist - dDist11) < DTOL_GM)
	{
		MyStartOrEnd = 1;
		UrStartOrEnd = 1;
	}

	double dUrLMyCDist = pUrMember->mp_LinkMem->mp_LLine->PointAtParam(0.5).Distance(MyCenMid);
	double dUrRMyCDist = pUrMember->mp_LinkMem->mp_RLine->PointAtParam(0.5).Distance(MyCenMid);
	double dMyLUrCDist = pMyMember->mp_LinkMem->mp_LLine->PointAtParam(0.5).Distance(UrCenMid);
	double dMyRUrCDist = pMyMember->mp_LinkMem->mp_RLine->PointAtParam(0.5).Distance(UrCenMid);

	GM2DCurve* pMyLongDistCurve = NULL;
	GM2DCurve* pMyShotDistCurve = NULL;
	GM2DCurve* pUrLongDistCurve = NULL;
	GM2DCurve* pUrShotDistCurve = NULL;

	if(dUrLMyCDist > dUrRMyCDist)
	{
		pUrLongDistCurve = pUrMember->mp_LLine;
		pUrShotDistCurve = pUrMember->mp_RLine;
	}
	else
	{
		pUrLongDistCurve = pUrMember->mp_RLine;
		pUrShotDistCurve = pUrMember->mp_LLine;
	}

	if(dMyLUrCDist > dMyRUrCDist)
	{
		pMyLongDistCurve = pMyMember->mp_LLine;
		pMyShotDistCurve = pMyMember->mp_RLine;
	}
	else
	{
		pMyLongDistCurve = pMyMember->mp_RLine;
		pMyShotDistCurve = pMyMember->mp_LLine;
	}

	vector<GM2DVector> CrossVecs;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResutlArr;

	// 멀리 떨어져 있는 선끼리
	long nRval = pMyLongDistCurve->IntersectEnd2(pUrLongDistCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, MSFloor::ms_LineHitDistTol);
	if(abs(nRval%10) == 1)
	{
		if(UrStartOrEnd == 0)
		{	
			if(!pUrLongDistCurve->m_P0.SameVector4DTOL(CrossVecs[0]))
				pUrLongDistCurve->m_P0 = CrossVecs[0];
		}
		else
		{
			if(!pUrLongDistCurve->m_P1.SameVector4DTOL(CrossVecs[0]))
				pUrLongDistCurve->m_P1 = CrossVecs[0];
		}
		if(MyStartOrEnd == 0)
		{	
			if(!pMyLongDistCurve->m_P0.SameVector4DTOL(CrossVecs[0]))
				pMyLongDistCurve->m_P0 = CrossVecs[0];
		}
		else
		{
			if(!pMyLongDistCurve->m_P1.SameVector4DTOL(CrossVecs[0]))
				pMyLongDistCurve->m_P1 = CrossVecs[0];

		}
	}

	// 가까운 선끼리
	nRval = pMyShotDistCurve->IntersectEnd2(pUrShotDistCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, MSFloor::ms_LineHitDistTol);
	if(abs(nRval%10) == 1)
	{
		if(UrStartOrEnd == 0)
		{
			if(!pUrShotDistCurve->m_P0.SameVector4DTOL(CrossVecs[0]))
				pUrShotDistCurve->m_P0 = CrossVecs[0];
		}
		else
		{
			if(!pUrShotDistCurve->m_P1.SameVector4DTOL(CrossVecs[0]))
				pUrShotDistCurve->m_P1 = CrossVecs[0];
		}

		if(MyStartOrEnd == 0)
		{
			if(!pMyShotDistCurve->m_P0.SameVector4DTOL(CrossVecs[0]))
				pMyShotDistCurve->m_P0 = CrossVecs[0];
		}
		else
		{
			if(!pMyShotDistCurve->m_P1.SameVector4DTOL(CrossVecs[0]))
				pMyShotDistCurve->m_P1 = CrossVecs[0];
		}
	}
	return 1;
}

long MSConnLevelZone::CutLineWhenShortening(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, 
                                            MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, 
                                            bool bOnlyInCross, bool bOnlyShorten, double ElongDegree)
{
	MSElement::Type_MSCompoG eMemType = pMyMember->mp_LinkMem->GetType();

// 	if(pMyMember->isBuriedLinkMember() || pUrMember->isBuriedLinkMember())
// 		return CutLineWhenShorteningByBuried(pMyMember, pUrMember);

	long NumMember = GetNumMemberG();
    double MyLoc, UrLoc;
    GM2DVector MyCrossVec = MyEndNum==0 ? pMyLine->GetP0() : pMyLine->GetP1();
    GM2DVector UrCrossVec = UrEndNum==0 ? pUrLine->GetP0() : pUrLine->GetP1();

	vector<GM2DVector> CrossVecs;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResutlArr;
	long ResultIdx = 0;

	long nRval = pMyMember->mp_CenLine->IntersectEnd2(pUrMember->mp_CenLine, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, 
		MSFloor::ms_LineHitDistTol);

	if(abs(nRval%10)==1) // 내부 교차
	{
		for(long i = 0 ; i < ResutlArr.size() ; i++)
		{
			if(ResutlArr[i] == 1)
			{
				ResultIdx = i;
				break;
			}
		}

		MyCrossVec = CrossVecs[ResultIdx];
		MyLoc = MyLocArr[ResultIdx];
		UrLoc = UrLocArr[ResultIdx];
		double dLoc1 = pMyMember->mp_CenLine->OrthoBaseLocValue(MyCrossVec);
		double dLoc2 = pUrMember->mp_CenLine->OrthoBaseLocValue(MyCrossVec);

		if(MyEndNum == 0 && MyLoc > 1 - DTOL_GM)
			return -1;
		else if(MyEndNum == 1 && MyLoc < 0 - DTOL_GM)
			return -1;

		if(UrEndNum == 0 && UrLoc > 1 - DTOL_GM)
			return -1;
		else if(UrEndNum == 1 && UrLoc < 0 - DTOL_GM)
			return -1;

		if(pMyMember->m_IsParallelCenLine == false)
		{
			if(MyEndNum == 0 && ( dLoc1 < 0 || pMyMember->m_CenLineSetCnt == 0) )
			{
				pMyMember->mp_CenLine->m_P0.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
				pMyMember->m_CenLineSetCnt++;
			}
			else if(MyEndNum == 1 && ( dLoc1 > 1 || pMyMember->m_CenLineSetCnt == 0) )
			{
				pMyMember->mp_CenLine->m_P1.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
				pMyMember->m_CenLineSetCnt++;
			}
		}

		if(pUrMember->m_IsParallelCenLine == false)
		{
			if(UrEndNum == 0 && ( dLoc2 < 0 || pUrMember->m_CenLineSetCnt == 0) )
			{
				pUrMember->mp_CenLine->m_P0.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
				pUrMember->m_CenLineSetCnt++;
			}
			else if(UrEndNum == 1 && ( dLoc2 > 1 || pUrMember->m_CenLineSetCnt == 0) )
			{
				pUrMember->mp_CenLine->m_P1.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
				pUrMember->m_CenLineSetCnt++;
			}
		}

		// 		if( (MyEndNum == 0 && MyLoc > 0 ) || (UrEndNum == 0 && UrLoc > 0) ||
		// 			(MyEndNum == 1 && MyLoc < 1 ) || (UrEndNum == 1 && UrLoc < 1) )
		if( (MyEndNum == 0 && MyLoc < 0 ) || (UrEndNum == 0 && UrLoc < 0) ||
			(MyEndNum == 1 && MyLoc > 1 ) || (UrEndNum == 1 && UrLoc > 1) )
		{

		}
	}
	else if(abs(nRval%10)==2)
	{

	}


//     long res = pMyLine->IntersectEnd(pUrLine, MyCrossVec, UrCrossVec, &MyLoc, &UrLoc, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, 
// 		                             max(pMyMember->mp_LinkMem->GetThick(), pUrMember->mp_LinkMem->GetThick()));
	long res = pMyLine->IntersectEnd(pUrLine, MyCrossVec, UrCrossVec, &MyLoc, &UrLoc, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, 
		MSFloor::ms_LineHitDistTol);

    if(res==1) // 내부 교차
    {
		if(eMemType != MSElement::msLineLevelDifG)
		{
			if(MyEndNum == 0 && MyLoc > 1 - DTOL_GM)
				return -1;
			else if(MyEndNum == 1 && MyLoc < 0 - DTOL_GM)
				return -1;

			if(UrEndNum == 0 && UrLoc > 1 - DTOL_GM)
				return -1;
			else if(UrEndNum == 1 && UrLoc < 0 - DTOL_GM)
				return -1;
		}

        if(!bOnlyInCross)
		{
			bool nRVal1 = pMyMember->PreCheckSetLine(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
			bool nRVal2 = pUrMember->PreCheckSetLine(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
			if(nRVal1 == true && nRVal2 == true || NumMember < 3 || eMemType == MSElement::msLineLevelDifG)
			{
				pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
				pUrMember->SetLineXY(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
			}
		}
		else if(MyLoc>0.+DTOL_GM && MyLoc<1.-DTOL_GM && UrLoc>0.+DTOL_GM && UrLoc<1.-DTOL_GM)
		{
			bool nRVal1 = pMyMember->PreCheckSetLine(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
			bool nRVal2 = pUrMember->PreCheckSetLine(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
			if(nRVal1 == true && nRVal2 == true || NumMember < 3 || eMemType == MSElement::msLineLevelDifG)
			{
				pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
				pUrMember->SetLineXY(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
			}
		}
    }
    else if(res==2 && !bOnlyInCross)
    {
		if(eMemType != MSElement::msLineLevelDifG)
		{
			if(MyEndNum == 0 && MyLoc > 1 - DTOL_GM)
				return -1;
			else if(MyEndNum == 1 && MyLoc < 0 - DTOL_GM)
				return -1;

			if(UrEndNum == 0 && UrLoc > 1 - DTOL_GM)
				return -1;
			else if(UrEndNum == 1 && UrLoc < 0 - DTOL_GM)
				return -1;
		}

		// 아크 부재와 트림할때 끝점이 포함되지 않으면 트림을 안한다. ,LineLevelDifG는 떨어져 있다.
		if(pMyMember->mp_LinkMem->GetType() != MSElement::msLineLevelDifG)
		{
			bool isMyArc = pMyMember->mp_LinkMem->isArc();
			bool isUrArc = pUrMember->mp_LinkMem->isArc();
			if(isMyArc != isUrArc)
			{
				if(isMyArc)
				{
					GM2DPolyline* pPoly = pMyMember->GetBoundary();
					if(pPoly->Contains(&UrCrossVec, MSFloor::ms_LineHitDistTol) == 1)
					{
						delete pPoly;
						return res;
					}
				}
				else if(isUrArc)
				{
					GM2DPolyline* pPoly = pUrMember->GetBoundary();
					if(pPoly->Contains(&MyCrossVec, MSFloor::ms_LineHitDistTol) == 1)
					{
						delete pPoly;
						return res;
					}
				}
			}
		}

        GM2DLineVector MyCrossLine(MyCrossVec, MyEndNum==0 ? pMyLine->m_P1 : pMyLine->m_P0);
        GM2DLineVector UrCrossLine(MyCrossVec, UrEndNum==0 ? pUrLine->m_P1 : pUrLine->m_P0);
        double Degree = MyCrossLine.GetDegree(UrCrossLine);
        if(Degree>ElongDegree)
        {
			bool nRVal1 = pMyMember->PreCheckSetLine(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
			bool nRVal2 = pUrMember->PreCheckSetLine(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
			if(nRVal1 == true && nRVal2 == true || NumMember < 3 || eMemType == MSElement::msLineLevelDifG)
			{
				pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
				pUrMember->SetLineXY(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
			}
            return 1;
        }
    }
    return res;
}

long MSConnLevelZone::CutParallelLine2(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, 
                                      MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, 
                                      bool nOnlyShort)
{
    double MyLoc, UrLoc;
    GM2DVector MyCrossVec = MyEndNum==0 ? pMyLine->GetP0() : pMyLine->GetP1();
    GM2DVector UrCrossVec = UrEndNum==0 ? pUrLine->GetP0() : pUrLine->GetP1();
	// 평행할 경우 입력된 부재 정보가 변경되기 때문에 MSFloor::ms_LineHitDistTol를 사용 하지 않는다. 
	long res = pMyLine->IntersectEnd(pUrLine, MyCrossVec, UrCrossVec, &MyLoc, &UrLoc, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, 
		MSFloor::ms_LineHitDistTol);
/*

	if(pMyMember->mp_LinkMem->GetType() == MSElement::msLineLevelDifG)
	{
		GM2DVector CrossVec = pMyLine->OrthoBaseVector(GM2DVector(mp_Conn->m_dXc, mp_Conn->m_dYc));
		// 교차점 까지 무조건 연장함
		pMyMember->SetLineXY(MyEndNum, MyLineSide, CrossVec, false);
		pUrMember->SetLineXY(UrEndNum, UrLineSide, CrossVec, false);
		return res;
	}*/

    if(res == -2)  // 평행하지만 동일 선상 아닐 때
    {
        //         bool bSameDir = MyEndNum==UrEndNum ? false : true;
        if(MyEndNum!=UrEndNum && MyLineSide==UrLineSide)
        {
            bool bIContainYou = pMyMember->mp_LinkMem->HitTestWithVector(UrEndNum==0 ? pUrLine->m_P0 : pUrLine->m_P1, 1., false);
            if(bIContainYou)
            {
                GM2DVector CrossVec = pUrLine->OrthoBaseVector(MyCrossVec);
                pUrMember->SetLineXY(UrEndNum, UrLineSide, CrossVec, true);
				pUrMember->m_IsParallelCenLine = true;

				double dLoc = pUrMember->mp_CenLine->OrthoBaseLocValue(CrossVec);

				if(UrEndNum == 0 && dLoc < 0)
					pUrMember->mp_CenLine->SetP0(pUrMember->mp_CenLine->OrthoBaseVector(CrossVec));
				else if(UrEndNum == 1 && dLoc > 1)
					pUrMember->mp_CenLine->SetP1(pUrMember->mp_CenLine->OrthoBaseVector(CrossVec));
            }
            else
            {
                GM2DVector CrossVec = pMyLine->OrthoBaseVector(UrCrossVec);
                pMyMember->SetLineXY(MyEndNum, MyLineSide, CrossVec, true);
				pMyMember->m_IsParallelCenLine = true;

				double dLoc = pMyMember->mp_CenLine->OrthoBaseLocValue(CrossVec);

				if(MyEndNum == 0 && dLoc < 0)
					pMyMember->mp_CenLine->SetP0(pMyMember->mp_CenLine->OrthoBaseVector(CrossVec));
				else if(MyEndNum == 1 && dLoc > 1)
					pMyMember->mp_CenLine->SetP1(pMyMember->mp_CenLine->OrthoBaseVector(CrossVec));
            }
        }
        if(MyEndNum==UrEndNum && MyLineSide!=UrLineSide)
        {
            bool bIContainYou = pMyMember->mp_LinkMem->HitTestWithVector(UrEndNum==0 ? pUrLine->m_P0 : pUrLine->m_P1, 1., false);
            if(bIContainYou)
            {
                GM2DVector CrossVec = pUrLine->OrthoBaseVector(MyCrossVec);
                pUrMember->SetLineXY(UrEndNum, UrLineSide, CrossVec, true);
				pUrMember->m_IsParallelCenLine = true;

				double dLoc = pUrMember->mp_CenLine->OrthoBaseLocValue(CrossVec);

				if(UrEndNum == 0 && dLoc < 0)
					pUrMember->mp_CenLine->SetP0(pUrMember->mp_CenLine->OrthoBaseVector(CrossVec));
				else if(UrEndNum == 1 && dLoc > 1)
					pUrMember->mp_CenLine->SetP1(pUrMember->mp_CenLine->OrthoBaseVector(CrossVec));
            }
            else
            {
                GM2DVector CrossVec = pMyLine->OrthoBaseVector(UrCrossVec);
                pMyMember->SetLineXY(MyEndNum, MyLineSide, CrossVec, true);
				pMyMember->m_IsParallelCenLine = true;

				double dLoc = pMyMember->mp_CenLine->OrthoBaseLocValue(CrossVec);

				if(MyEndNum == 0 && dLoc < 0)
					pMyMember->mp_CenLine->SetP0(pMyMember->mp_CenLine->OrthoBaseVector(CrossVec));
				else if(MyEndNum == 1 && dLoc > 1)
					pMyMember->mp_CenLine->SetP1(pMyMember->mp_CenLine->OrthoBaseVector(CrossVec));
            }
        }

    }
    else if(res==-1)//Overlap
    {
		GM2DLineVector TheLine;
		TheLine.m_P0 = MyCrossVec;
		TheLine.m_P1 = UrCrossVec;
		GM2DVector CenVec = TheLine.MassCenter();

		double dLoc1 = pMyMember->mp_CenLine->OrthoBaseLocValue(CenVec);
		double dLoc2 = pUrMember->mp_CenLine->OrthoBaseLocValue(CenVec);

		pMyMember->SetLineXY(MyEndNum, MyLineSide, CenVec, false);
		pUrMember->SetLineXY(UrEndNum, UrLineSide, CenVec, false);

		if(MyEndNum == 0 && ( dLoc1 < 0/* || pMyMember->m_CenLineSetCnt == 0*/) )
		{
			pMyMember->mp_CenLine->SetP0(pMyMember->mp_CenLine->OrthoBaseVector(CenVec));
			pMyMember->m_IsParallelCenLine = true;
		}
		else if(MyEndNum == 1 && ( dLoc1 > 1/* || pMyMember->m_CenLineSetCnt == 0*/) )
		{
			pMyMember->mp_CenLine->SetP1(pMyMember->mp_CenLine->OrthoBaseVector(CenVec));
			pMyMember->m_IsParallelCenLine = true;
		}

		if(UrEndNum == 0 && ( dLoc2 < 0/* || pUrMember->m_CenLineSetCnt == 0*/) )
		{
			pUrMember->mp_CenLine->SetP0(pUrMember->mp_CenLine->OrthoBaseVector(CenVec));
			pUrMember->m_IsParallelCenLine = true;
		}
		else if(UrEndNum == 1 && ( dLoc2 > 1/* || pUrMember->m_CenLineSetCnt == 0*/) )
		{
			pUrMember->mp_CenLine->SetP1(pUrMember->mp_CenLine->OrthoBaseVector(CenVec));
			pUrMember->m_IsParallelCenLine = true;
		}
		/*
		if(MyEndNum == 0)
			pMyMember->mp_CenLine->SetP0(pMyMember->mp_CenLine->OrthoBaseVector(CenVec));
		else
			pMyMember->mp_CenLine->SetP1(pMyMember->mp_CenLine->OrthoBaseVector(CenVec));

		if(UrEndNum == 0)
			pUrMember->mp_CenLine->SetP0(pUrMember->mp_CenLine->OrthoBaseVector(CenVec));
		else
			pUrMember->mp_CenLine->SetP1(pUrMember->mp_CenLine->OrthoBaseVector(CenVec));
			*/
    }
    return res;
}

long MSConnLevelZone::CutLineWhenShortening2(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, 
											MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, 
											bool bOnlyInCross, bool bOnlyShorten, double ElongDegree)
{
	long NumMember = GetNumMemberG();
	double MyLoc, UrLoc;
	GM2DVector MyCrossVec = MyEndNum==0 ? pMyLine->GetP0() : pMyLine->GetP1();
	GM2DVector UrCrossVec = UrEndNum==0 ? pUrLine->GetP0() : pUrLine->GetP1();

	GM2DLineVector MyCrossLine(MyCrossVec, MyEndNum==0 ? pMyLine->m_P1 : pMyLine->m_P0);
	GM2DLineVector UrCrossLine(MyCrossVec, UrEndNum==0 ? pUrLine->m_P1 : pUrLine->m_P0);
	double Degree = MyCrossLine.GetDegree(UrCrossLine);
	if(Degree<ElongDegree)
		return -1;

	bool IsMyColinerMemPart = false;
	bool IsUrColinerMemPart = false;

	map<long, long>::iterator itID;
	itID = m_ColinerMemMap.find(pMyMember->mp_LinkMem->m_ID);
	if(itID != m_ColinerMemMap.end())
		IsMyColinerMemPart = true;

	itID = m_ColinerMemMap.find(pUrMember->mp_LinkMem->m_ID);
	if(itID != m_ColinerMemMap.end())
		IsUrColinerMemPart = true;

	bool IsMyBuriedLinkMember = pMyMember->isBuriedLinkMember();
	bool IsUrBuriedLinkMember = pUrMember->isBuriedLinkMember();
	

	vector<GM2DVector> CrossVecs;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResutlArr;
	long ResultIdx = 0;
	long res = pMyLine->IntersectEnd2(pUrLine, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, 
		MSFloor::ms_LineHitDistTol);

	long nArcCnt = 0;
	if(pMyLine->m_ObjType == GT_ARC)
		nArcCnt++;
	if(pUrLine->m_ObjType == GT_ARC)
		nArcCnt++;

	if(abs(res%10)==1) // 내부 교차
	{
		for(long i = 0 ; i < ResutlArr.size() ; i++)
		{
			if(ResutlArr[i] == 1)
			{
				ResultIdx = i;
				break;
			}
		}
		
		MyCrossVec = CrossVecs[ResultIdx];
		MyLoc = MyLocArr[ResultIdx];
		UrLoc = UrLocArr[ResultIdx];
		double dLoc1 = pMyLine->OrthoBaseLocValue(MyCrossVec);
		double dLoc2 = pUrLine->OrthoBaseLocValue(MyCrossVec);

		if(!IsMyBuriedLinkMember)
		{
			if(MyEndNum == 0 && MyLoc > 1 - DTOL_GM)
				return -1;
			else if(MyEndNum == 1 && MyLoc < 0 - DTOL_GM)
				return -1;
		}

		if(!IsUrBuriedLinkMember)
		{
			if(UrEndNum == 0 && UrLoc > 1 - DTOL_GM)
				return -1;
			else if(UrEndNum == 1 && UrLoc < 0 - DTOL_GM)
				return -1;
		}

		if(nArcCnt == 1)
		{
			if(!bOnlyInCross)
			{
				if(!pMyMember->mp_LinkMem->m_isBuriedLinkMember)
					pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, false);

				if(!pUrMember->mp_LinkMem->m_isBuriedLinkMember)
					pUrMember->SetLineXY2(UrEndNum, UrLineSide, MyCrossVec, false);
			}
			else if(MyLoc>0.+DTOL_GM && MyLoc<1.-DTOL_GM && UrLoc>0.+DTOL_GM && UrLoc<1.-DTOL_GM)
			{
				if(!pMyMember->mp_LinkMem->m_isBuriedLinkMember)
					pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, false);

				if(!pUrMember->mp_LinkMem->m_isBuriedLinkMember)
					pUrMember->SetLineXY(UrEndNum, UrLineSide, MyCrossVec, false);
			}
		}
		else
		{
			if(!bOnlyInCross)
			{
				bool nRVal1 = pMyMember->PreCheckSetLine(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
				bool nRVal2 = pUrMember->PreCheckSetLine(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
				if(nRVal1 == true && nRVal2 == true || NumMember < 3)
				{
					if(IsMyColinerMemPart == false || (IsMyColinerMemPart == true && IsUrBuriedLinkMember == true) )
					{
						if(pMyMember->mp_LinkMem->m_isBuriedLinkMember)
						{
							pMyMember->SetLineXY2(0, MyLineSide, MyCrossVec, true, bOnlyShorten);
							pMyMember->SetLineXY2(1, MyLineSide, MyCrossVec, true, bOnlyShorten);
						}
						else
							pMyMember->SetLineXY2(MyEndNum, MyLineSide, MyCrossVec, true, bOnlyShorten);
					}

					if(IsUrColinerMemPart == false || (IsUrColinerMemPart == true && IsMyBuriedLinkMember == true) )
					{
						if(pUrMember->mp_LinkMem->m_isBuriedLinkMember)
						{
							pUrMember->SetLineXY2(0, UrLineSide, MyCrossVec, true, bOnlyShorten);
							pUrMember->SetLineXY2(1, UrLineSide, MyCrossVec, true, bOnlyShorten);
						}
						else
							pUrMember->SetLineXY2(UrEndNum, UrLineSide, MyCrossVec, true, bOnlyShorten);
					}
				}
			}
			else if(MyLoc>0.+DTOL_GM && MyLoc<1.-DTOL_GM && UrLoc>0.+DTOL_GM && UrLoc<1.-DTOL_GM)
			{
				bool nRVal1 = pMyMember->PreCheckSetLine(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
				bool nRVal2 = pUrMember->PreCheckSetLine(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
				if(nRVal1 == true && nRVal2 == true || NumMember < 3)
				{
					if(IsMyColinerMemPart == false || (IsMyColinerMemPart == true && IsUrBuriedLinkMember == true) )
					{
						if(pMyMember->mp_LinkMem->m_isBuriedLinkMember)
						{
							pMyMember->SetLineXY2(0, MyLineSide, MyCrossVec, true, bOnlyShorten);
							pMyMember->SetLineXY2(1, MyLineSide, MyCrossVec, true, bOnlyShorten);
						}
						else
							pMyMember->SetLineXY2(MyEndNum, MyLineSide, MyCrossVec, true, bOnlyShorten);
					}

					if(IsUrColinerMemPart == false || (IsUrColinerMemPart == true && IsMyBuriedLinkMember == true) )
					{
						if(pUrMember->mp_LinkMem->m_isBuriedLinkMember)
						{
							pUrMember->SetLineXY2(0, UrLineSide, MyCrossVec, true, bOnlyShorten);
							pUrMember->SetLineXY2(1, UrLineSide, MyCrossVec, true, bOnlyShorten);
						}
						else
							pUrMember->SetLineXY2(UrEndNum, UrLineSide, MyCrossVec, true, bOnlyShorten);
					}
				}
			}
		}
	}
	else if(abs(res%10)==2 && !bOnlyInCross)
	{
		double dMinDist = 0;
		double dDist0 = CrossVecs[0].Distance(MyCrossVec);
		double dDist1 = CrossVecs[1].Distance(MyCrossVec);
		if(dDist0 > dDist1)
			ResultIdx = 1;
		else
			ResultIdx = 0;

		MyCrossVec = CrossVecs[ResultIdx];
		MyLoc = MyLocArr[ResultIdx];
		UrLoc = UrLocArr[ResultIdx];

		if(MyEndNum == 0 && MyLoc > 1 - DTOL_GM)
			return -1;
		else if(MyEndNum == 1 && MyLoc < 0 - DTOL_GM)
			return -1;

		if(UrEndNum == 0 && UrLoc > 1 - DTOL_GM)
			return -1;
		else if(UrEndNum == 1 && UrLoc < 0 - DTOL_GM)
			return -1;

		GM2DLineVector MyCrossLine(MyCrossVec, MyEndNum==0 ? pMyLine->m_P1 : pMyLine->m_P0);
		GM2DLineVector UrCrossLine(MyCrossVec, UrEndNum==0 ? pUrLine->m_P1 : pUrLine->m_P0);
		double Degree = MyCrossLine.GetDegree(UrCrossLine);
		if(Degree>ElongDegree)
		{
			if(nArcCnt == 1)
			{
				if(!pMyMember->mp_LinkMem->m_isBuriedLinkMember)
					pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, false);

				if(!pUrMember->mp_LinkMem->m_isBuriedLinkMember)
					pUrMember->SetLineXY(UrEndNum, UrLineSide, MyCrossVec, false);
			}
			else
			{
				bool nRVal1 = pMyMember->PreCheckSetLine(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);
				bool nRVal2 = pUrMember->PreCheckSetLine(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
				if(nRVal1 == true && nRVal2 == true || NumMember < 3)
				{
					if(pMyMember->mp_LinkMem->m_isBuriedLinkMember)
					{
						pMyMember->SetLineXY(0, MyLineSide, MyCrossVec, bOnlyShorten);
						pMyMember->SetLineXY(1, MyLineSide, MyCrossVec, bOnlyShorten);
					}
					else
						pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, bOnlyShorten);

					if(pUrMember->mp_LinkMem->m_isBuriedLinkMember)
					{
						pUrMember->SetLineXY(0, UrLineSide, MyCrossVec, bOnlyShorten);
						pUrMember->SetLineXY(1, UrLineSide, MyCrossVec, bOnlyShorten);
					}
					else
						pUrMember->SetLineXY(UrEndNum, UrLineSide, MyCrossVec, bOnlyShorten);
				}
			}
			return 1;
		}
	}
	else if(res == 0)
	{
		if(nArcCnt == 1)
		{
			if(pMyLine->m_ObjType == GT_ARC)
			{
				GM2DLineVector EndLine = pUrMember->GetEndLine(UrEndNum, FALSE);
				
				long nVal = EndLine.IntersectEnd2(pMyLine, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true);

				ResultIdx = -1;
				for(long i = 0 ; i < ResutlArr.size() ; i++)
				{
					if(ResutlArr[i] == 1)
					{
						ResultIdx = i;
						break;
					}
				}
				if(ResultIdx == -1)
				{
					double dMinLoc = DBL_MAX;
					for(long i = 0 ; i < ResutlArr.size() ; i++)
					{
						if(dMinLoc > fabs(MyLocArr[i]))
						{
							dMinLoc = fabs(MyLocArr[i]);
							ResultIdx = i;
						}
					}
				}
				if(ResultIdx != -1)
					pMyMember->SetLineXY(MyEndNum, MyLineSide, CrossVecs[ResultIdx], false);
			}

			if(pUrLine->m_ObjType == GT_ARC)
			{
				GM2DLineVector EndLine = pMyMember->GetEndLine(MyEndNum, FALSE);
				long nVal = EndLine.IntersectEnd2(pUrLine, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true);

				ResultIdx = -1;
				for(long i = 0 ; i < ResutlArr.size() ; i++)
				{
					if(ResutlArr[i] == 1)
					{
						ResultIdx = i;
						break;
					}
				}
				if(ResultIdx == -1)
				{
					double dMinLoc = DBL_MAX;
					for(long i = 0 ; i < ResutlArr.size() ; i++)
					{
						if(dMinLoc > fabs(MyLocArr[i]))
						{
							dMinLoc = fabs(MyLocArr[i]);
							ResultIdx = i;
						}
					}
				}
				if(ResultIdx != -1)
					pUrMember->SetLineXY(MyEndNum, MyLineSide, CrossVecs[ResultIdx], false);
			}
		}

	}

	long nRval = pMyMember->mp_CenLine->IntersectEnd2(pUrMember->mp_CenLine, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true, MSFloor::ms_LineHitDistTol, MSFloor::ms_ColinearDegreeTol, 
		MSFloor::ms_LineHitDistTol);

	if(abs(nRval%10)==1) // 내부 교차
	{
		for(long i = 0 ; i < ResutlArr.size() ; i++)
		{
			if(ResutlArr[i] == 1)
			{
				ResultIdx = i;
				break;
			}
		}

		MyCrossVec = CrossVecs[ResultIdx];
		MyLoc = MyLocArr[ResultIdx];
		UrLoc = UrLocArr[ResultIdx];
		double dLoc1 = pMyMember->mp_CenLine->OrthoBaseLocValue(MyCrossVec);
		double dLoc2 = pUrMember->mp_CenLine->OrthoBaseLocValue(MyCrossVec);

		if(MyEndNum == 0 && MyLoc > 1 - DTOL_GM)
			return -1;
		else if(MyEndNum == 1 && MyLoc < 0 - DTOL_GM)
			return -1;

		if(UrEndNum == 0 && UrLoc > 1 - DTOL_GM)
			return -1;
		else if(UrEndNum == 1 && UrLoc < 0 - DTOL_GM)
			return -1;

		if(pMyMember->m_IsParallelCenLine == false)
		{
			if(MyEndNum == 0 && ( dLoc1 < 0 || pMyMember->m_CenLineSetCnt == 0) )
			{
				pMyMember->mp_CenLine->m_P0.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
				pMyMember->m_CenLineSetCnt++;
			}
			else if(MyEndNum == 1 && ( dLoc1 > 1 || pMyMember->m_CenLineSetCnt == 0) )
			{
				pMyMember->mp_CenLine->m_P1.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
				pMyMember->m_CenLineSetCnt++;
			}
		}

		if(pUrMember->m_IsParallelCenLine == false)
		{
			if(UrEndNum == 0 && ( dLoc2 < 0 || pUrMember->m_CenLineSetCnt == 0) )
			{
				pUrMember->mp_CenLine->m_P0.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
				pUrMember->m_CenLineSetCnt++;
			}
			else if(UrEndNum == 1 && ( dLoc2 > 1 || pUrMember->m_CenLineSetCnt == 0) )
			{
				pUrMember->mp_CenLine->m_P1.SetXY(MyCrossVec.m_X, MyCrossVec.m_Y);
				pUrMember->m_CenLineSetCnt++;
			}
		}

// 		if( (MyEndNum == 0 && MyLoc > 0 ) || (UrEndNum == 0 && UrLoc > 0) ||
// 			(MyEndNum == 1 && MyLoc < 1 ) || (UrEndNum == 1 && UrLoc < 1) )
		if( (MyEndNum == 0 && MyLoc < 0 ) || (UrEndNum == 0 && UrLoc < 0) ||
			(MyEndNum == 1 && MyLoc > 1 ) || (UrEndNum == 1 && UrLoc > 1) )
			return 2;
	}
	else if(abs(nRval%10)==2)
	{

	}

	return res;
}

long MSConnLevelZone::CutLineByEndLine(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, GM2DCurve* pUrEndLine, bool bCutWhenShorten)
{
    double MyLoc, UrLoc;
    //     GM2DVector MyCrossVec = MyEndNum==0 ? pMyLine->GetP0() : pMyLine->GetP1();
    //     GM2DVector UrCrossVec = UrEndNum==0 ? pUrLine->GetP0() : pUrLine->GetP1();
    GM2DVector MyCrossVec = MyEndNum==0 ? pMyLine->GetP0() : pMyLine->GetP1();
    GM2DVector UrCrossVec;
    if(pUrEndLine==NULL)
        return 0;
    long res = pMyLine->IntersectEnd(pUrEndLine, MyCrossVec, UrCrossVec, &MyLoc, &UrLoc, true);
    if(res==1 || (UrLoc>-DTOL_GM && UrLoc<1.+DTOL_GM)) // 내부 교차
    {
        pMyMember->SetLineXY(MyEndNum, MyLineSide, MyCrossVec, bCutWhenShorten);
        return 1;
    }
    return 0;
}

void MSConnLevelZone::MakeBasePoly(GM2DPolyline& TheSection, bool isBaseSec, long MemPartIdx)
{
	long nMem = ma_MemPart.GetSize();
	MSLinkMemPart* pLink = ma_MemPart[MemPartIdx];
	if(nMem==2 && pLink->mp_FloorPartUp)
		pLink = ma_MemPart[MemPartIdx+1];

	long StartOrEnd = mp_Conn->IsSameOrUpDown(pLink->GetSConn()) ? 0 : 1;
	
	GM2DLineVector EndLVec, LVec;
	long NumDownLinkJoint = mp_Conn->ma_DownLinkJoint.size();

	if(NumDownLinkJoint > 0)
		EndLVec = pLink->GetEndLine(StartOrEnd, false);
	else
		EndLVec = pLink->GetEndLine(StartOrEnd, true);
	
	bool isColinerMember = false;
	if(m_ColinerMemMap.size() == 2 && nMem == 2 || nMem == 2 && GetNumMemberG() == 1)
		isColinerMember = true;

	long NumUpLinkJoint = mp_Conn->ma_UpLinkJoint.size();

	if(isColinerMember && NumUpLinkJoint > 0)
	{
		double dMaxWidth = 0;
		for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
		{
			MSConnectivity* pJoint = mp_Conn->ma_UpLinkJoint[iJoint];
			if(pJoint == NULL || pJoint->IsDeleted())
				continue;
			double dWidth = pJoint->GetMaxWidthByLineMemberG();
			dMaxWidth = max(dMaxWidth, dWidth);
		}
		LVec = EndLVec.NormalTranslate(dMaxWidth/2);
		EndLVec = EndLVec.NormalTranslate(-dMaxWidth/2);
	}
	else
	{
		if(StartOrEnd)
			LVec = EndLVec.NormalTranslate(-5);
		else
			LVec = EndLVec.NormalTranslate(5);
	}
	
	TheSection.ResetPolyline();

	TheSection.AddCopy(EndLVec.m_P0);
	TheSection.ma_Bulge.Add(0.0);
	TheSection.AddCopy(EndLVec.m_P1);
	TheSection.ma_Bulge.Add(0.0);
	TheSection.AddCopy(LVec.m_P1);
	TheSection.ma_Bulge.Add(0.0);
	TheSection.AddCopy(LVec.m_P0);
	TheSection.ma_Bulge.Add(0.0);
	TheSection.AddCopy(EndLVec.m_P0);
	m_isBaseSec = isBaseSec;

	TheSection.MakeCCW();
}

void MSConnLevelZone::BuildSectionOnGivenMemPartByDownJoint(GM2DPolyline& TheSection)
{
	MSLinkMemPart* pLink = ma_MemPart[0];
	long nMem = ma_MemPart.GetSize();

	if(nMem==2 && pLink->mp_FloorPartUp)
		pLink = ma_MemPart[1];

	long StartOrEnd = mp_Conn->IsSameOrUpDown(pLink->GetSConn()) ? 0 : 1;

	vector<MSConnectivity*> DownLinkJointArr = GetDownLinkJoint();

	long NumDownLinkJoint = DownLinkJointArr.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pDownJoint = DownLinkJointArr[iJoint];

		long NumDownMemberG = 0;
		bool isDownMidJoint = false;
		if(pDownJoint != NULL)
		{
			NumDownMemberG = pDownJoint->GetNumMemberG();
			isDownMidJoint = pDownJoint->IsMidConn();
		}

		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
		GM2DPolyline* pBound = NULL;

		double dExtendRatio= 2;
		if(pLink->mp_LinkMem->isArc())
			dExtendRatio= 1.5;

		//if(NumDownMemberG == 1 && isDownMidJoint)
		if(isDownMidJoint)
		{
			double dMaxWidth = pDownJoint->GetMaxWidthByLineMemberG();

			//mp_DownLinkJointBoundry->Trim(pLink->mp_LLine, StartOrEnd, true);
			//mp_DownLinkJointBoundry->Trim(pLink->mp_RLine, StartOrEnd, true);

			mp_DownLinkJointBoundry->Trim2(pLink->mp_LLine, StartOrEnd, true, true, dMaxWidth * dExtendRatio);
			mp_DownLinkJointBoundry->Trim2(pLink->mp_RLine, StartOrEnd, true, true, dMaxWidth * dExtendRatio);

			GM2DCurve* pOldLLine = pLink->mp_LLine->Copy();
			GM2DCurve* pOldRLine = pLink->mp_RLine->Copy();

			mp_DownLinkJointBoundry->Trim2(pLink->mp_LLine, StartOrEnd, true, false, dMaxWidth * dExtendRatio);
			mp_DownLinkJointBoundry->Trim2(pLink->mp_RLine, StartOrEnd, true, false, dMaxWidth * dExtendRatio);

			//mp_DownLinkJointBoundry->ExtendUrCurve(pLink->mp_LLine, StartOrEnd, true, dMaxWidth);
			//mp_DownLinkJointBoundry->ExtendUrCurve(pLink->mp_RLine, StartOrEnd, true, dMaxWidth);
			pBound = pLink->GetBoundary(false);

			delete pLink->mp_LLine;
			delete pLink->mp_RLine;

			pLink->mp_LLine = pOldLLine->Copy();
			pLink->mp_RLine = pOldRLine->Copy();
		}
		else
			pBound = pLink->GetBoundary(false);


		pBound->BoolOperation('I', mp_DownLinkJointBoundry, JoinedPolys, 10);

		delete pBound;

		long NumJoinedPoly = JoinedPolys.GetSize();

		if(TheSection.GetNumLines() > 0)
		{
			if(NumJoinedPoly == 1)
			{
				CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys2;

				GM2DPolyline* pJoindPoly = JoinedPolys[0];
				pJoindPoly->BoolOperation('U', &TheSection, JoinedPolys2, 1);

				long NumJoindPoly2 = JoinedPolys2.GetSize();

				if(NumJoindPoly2 == 1)
					TheSection.CopyFrom(JoinedPolys2[0]);

				for(long iJoind = 0 ; iJoind < NumJoindPoly2 ; iJoind++)
					delete JoinedPolys2[iJoind];
				JoinedPolys2.RemoveAll();

			}
			else if(NumJoinedPoly > 1) // 가장 큰 영역을 선택함
			{
				long Maxidx = -1;
				double MaxArea = 0;
				for(long i = 0 ; i < NumJoinedPoly ; i++)
				{
					GM2DPolyline* pJoindPoly = JoinedPolys[i];
					if(pJoindPoly->GetNumLines() <= 2) continue;

					double dArea = pJoindPoly->Area();
					if(dArea > MaxArea )
					{
						MaxArea  = dArea;
						Maxidx = i;
					}
				}
				if(Maxidx == -1)
					MakeBasePoly(TheSection, false);
				else
					TheSection.CopyFrom(JoinedPolys[Maxidx]);
			}
		}
		else
		{
			if(NumJoinedPoly == 1)
			{
				if(JoinedPolys[0]->GetNumLines() > 2)
				{
					TheSection.CopyFrom(JoinedPolys[0]);
				}
				else
				{
					MakeBasePoly(TheSection, false);
				}
			}
			else if(NumJoinedPoly > 1) // 가장 큰 영역을 선택함
			{
				long Maxidx = -1;
				double MaxArea = 0;
				for(long i = 0 ; i < NumJoinedPoly ; i++)
				{
					GM2DPolyline* pJoindPoly = JoinedPolys[i];
					if(pJoindPoly->GetNumLines() <= 2) continue;

					double dArea = pJoindPoly->Area();
					if(dArea > MaxArea )
					{
						MaxArea  = dArea;
						Maxidx = i;
					}
				}
				if(Maxidx == -1)
					MakeBasePoly(TheSection, false);
				else
					TheSection.CopyFrom(JoinedPolys[Maxidx]);
			}

		}

		for(long iJoind = 0 ; iJoind < NumJoinedPoly ; iJoind++)
			delete JoinedPolys[iJoind];
		JoinedPolys.RemoveAll();
	}
}

bool MSConnLevelZone::BuildSectionOnGivenMemPart(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
    TheSection.ResetPolyline();
	DrawSection.ResetPolyline();
	m_isBaseSec = false;

	MSPointMemberG* pPointMem=NULL;
	if(mp_Column)
		pPointMem = mp_Column;
	if(mp_IsoFoot)
		pPointMem = mp_IsoFoot;

	vector<MSConnectivity*> DownLinkJointArr = GetDownLinkJoint();
	long NumDownLinkJoint = DownLinkJointArr.size();

	//MSConnectivity* pDownJoint = GetDownLinkJoint();
	long NumDownMemberG = 0;
	bool isDownMidJoint = false;
	for(long iDownJoint = 0; iDownJoint < NumDownLinkJoint ; iDownJoint++)
	{
		NumDownMemberG += DownLinkJointArr[iDownJoint]->GetNumMemberG();
		if(DownLinkJointArr[iDownJoint]->IsMidConn())
			isDownMidJoint = true;
	}
// 	if(pDownJoint != NULL)
// 	{
// 		NumDownMemberG = pDownJoint->GetNumMemberG();
// 		isDownMidJoint = pDownJoint->IsMidConn();
// 	}

    GM2DLineVector EndLVec;
    long nMem = MemPartArr.GetSize();
    if(nMem==0 && pPointMem == NULL)
        return false;
	else if(nMem == 0 && pPointMem != NULL)
	{
		GM2DPolyline MyPoly(TRUE);
		pPointMem->GetBoundary(TheSection);
		return true;
	}
    else if(nMem==1 || (nMem==2 && MemPartArr[0]->IsWallAndGirderWith(MemPartArr[1])))
    {
        MSLinkMemPart* pLink = MemPartArr[0];
        if(nMem==2 && pLink->mp_FloorPartUp)
            pLink = MemPartArr[1];

		long StartOrEnd = mp_Conn->IsSameOrUpDown(pLink->GetSConn()) ? 0 : 1;

		if(m_bUsedDownLinkJoint == FALSE)
		{
			MakeBasePoly(TheSection, true);
		}

		if(pPointMem != NULL)
		{
			GM2DPolyline PointPoly(TRUE);
			pPointMem->GetBoundary(PointPoly);
			TheSection.CopyFrom(&PointPoly);
			DrawSection.CopyFrom(&TheSection);
		}
		else if(m_bUsedDownLinkJoint == TRUE && mp_DownLinkJointBoundry != NULL)
		{
			BuildSectionOnGivenMemPartByDownJoint(TheSection);
		}

		//나중에 수정 kkw
		MakeMemPartEndJointPoly(TheSection, DrawSection, MemPartArr);

		for(long iDownJoint = 0; iDownJoint < NumDownLinkJoint ; iDownJoint++)
		{
			MSConnectivity* pDownJoint = DownLinkJointArr[iDownJoint];

			if(pDownJoint != NULL)
			{
				GM2DPolyline* pDownPoly = pDownJoint->GetSection_Hit();
				if(pDownPoly != NULL)
				{
					CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
					TheSection.BoolOperation('U', pDownPoly, JoinedPolys, 1.);
					long NumJoined = JoinedPolys.GetSize();

					if(NumJoined == 1)
						TheSection.CopyFrom(JoinedPolys[0]);

					for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
						delete JoinedPolys[iPoly];
					JoinedPolys.RemoveAll();
				}
			}
		}
		TheSection.MakeClosedPolyline(DTOL_GM*1E3);
        return true;
    }

	long nCount = 0;
	long NumMemberG = GetNumMemberG();
    for(long iMem=0; iMem<nMem; iMem++)
    {
        MSLinkMemPart* pLink = MemPartArr[iMem];
		if(pLink->isBuriedLinkMember())
			continue;

		if( (NumDownLinkJoint > 0) && nMem == 2 && NumMemberG == 1 && isDownMidJoint && NumDownMemberG == 1 && mp_DownLinkJointBoundry != NULL)
		{
			long StartOrEnd = mp_Conn->IsSameOrUpDown(pLink->GetSConn()) ? 0 : 1;
			mp_DownLinkJointBoundry->Trim(pLink->mp_LLine, StartOrEnd, true);
			mp_DownLinkJointBoundry->Trim(pLink->mp_RLine, StartOrEnd, true);
		}

		if(MSFloor::ms_TrimType == 0)
		{
			//pLink->SetLRPolyLine(mp_Conn);
			pLink->AddEndJointLine(this,TheSection, NumMemberG);
			if(nMem == 3 && NumMemberG == 2)
			{
// 				map<long, long>::iterator itID = m_ColinerMemMap.find(pLink->mp_LinkMem->m_ID);
// 				if(itID == m_ColinerMemMap.end())
					pLink->AddEndJointLine2(this, DrawSection, 0, 0);
			}
			else
				pLink->AddEndJointLine2(this, DrawSection, 0, 0);
		}
		else if(MSFloor::ms_TrimType == 1)
		{
			pLink->AddEndJointLine2(this,TheSection, nMem, NumMemberG);

			if(nMem == 3 && NumMemberG == 2)
			{
				map<long, long>::iterator itID = m_ColinerMemMap.find(pLink->mp_LinkMem->m_ID);
				if(itID == m_ColinerMemMap.end())
					pLink->AddEndJointLine2(this, DrawSection, 0, 0);
			}
			else
				pLink->AddEndJointLine2(this, DrawSection, 0, 0);
		}
        
    }

	TheSection.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5
	DrawSection.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5

	if(TheSection.ma_Point.GetSize() == TheSection.ma_Bulge.GetSize())
	{
		if(TheSection.ma_Bulge.GetSize() > 0)
			TheSection.ma_Bulge.RemoveAt(TheSection.ma_Bulge.GetSize()-1);
	}
	
	GM2DVector CenVec(mp_Conn->m_dXc, mp_Conn->m_dYc);

	// Hit조인트는 형상 유지 Draw조인트는 꼬인거 풀어줌
// 	if(TheSection.CheckPolygon() == FALSE)
// 		TheSection.MakeSaftPolygon(&CenVec);
	if(DrawSection.CheckPolygon(false) == FALSE)
		DrawSection.MakeSaftPolygon(&CenVec);
	
	if(TheSection.GetNumLines() < 3)
	{
		MakeBasePoly(TheSection, true);
		MakeBasePoly(DrawSection, true);
		//return false;
		return true;
	}
	
    if(pPointMem)
    {
//         double Xc = pPointMem->ma_Joint[0]->m_dXc;
//         double Yc = pPointMem->ma_Joint[0]->m_dYc;
//         long JointID = pPointMem->ma_Joint[0]->m_ID;
        GM2DPolyline MyPoly(TRUE);
        pPointMem->GetBoundary(MyPoly);
		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
        TheSection.BoolOperation('U', &MyPoly, JoinedPolys, 1.);
        
		if(JoinedPolys.GetSize() > 0)
			TheSection.CopyFrom(JoinedPolys[0]);
		else
			TheSection.CopyFrom(&MyPoly);
        //else
        //    ASSERT(0);

		for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
		{
			delete JoinedPolys[iPoly];
		}
		JoinedPolys.RemoveAll();

		DrawSection.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5

		DrawSection.BoolOperation('U', &MyPoly, JoinedPolys, 1.);

		if(JoinedPolys.GetSize() > 0)
			DrawSection.CopyFrom(JoinedPolys[0]);
		else
			DrawSection.CopyFrom(&MyPoly);
		//else
		//    ASSERT(0);

		for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
		{
			delete JoinedPolys[iPoly];
		}
		JoinedPolys.RemoveAll();			
    }

	/*
    long nBuriedMem = mp_Conn->ma_BuriedLinkMember.GetSize();
    for(long iBuried=0; iBuried<nBuriedMem; iBuried++)
    {
        MSLinkMemberG* pLink = mp_Conn->ma_BuriedLinkMember[iBuried];
        if(!CheckLevelOverlap(pLink))
            continue;
        GM2DPolyline MyPoly(TRUE);
        pLink->GetBoundary(MyPoly);
        //         GM2DPolygon* pMergedPGon = TheSection.Union(&MyPoly, MSFloor::ms_LineHitDistTol);
		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
		TheSection.BoolOperation('U', &MyPoly, JoinedPolys, 1.);
		if(JoinedPolys.GetSize()==1)
			TheSection.CopyFrom(JoinedPolys[0]);
			//TheSection = *JoinedPolys[0];
		//else
			//ASSERT(0);

		for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
		{
			delete JoinedPolys[iPoly];
		}
		JoinedPolys.RemoveAll();

    }
	if(nBuriedMem > 0)
		TheSection.MakeClosedPolyline();
*/

	// 나중에 수정 kkw 
	MakeMemPartEndJointPoly(TheSection, DrawSection, MemPartArr);
	
// 	for(long iDownJoint = 0; iDownJoint < NumDownLinkJoint ; iDownJoint++)
// 	{
// 		MSConnectivity* pDownJoint = DownLinkJointArr[iDownJoint];
// 
// 		if(pDownJoint != NULL)
// 		{
// 			GM2DPolyline* pDownPoly = pDownJoint->GetSection_Hit();
// 			if(pDownPoly != NULL)
// 			{
// 				CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
// 				TheSection.BoolOperation('U', pDownPoly, JoinedPolys, 1.);
// 				long NumJoined = JoinedPolys.GetSize();
// 
// 				if(NumJoined == 1)
// 					TheSection.CopyFrom(JoinedPolys[0]);
// 
// 				for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
// 					delete JoinedPolys[iPoly];
// 				JoinedPolys.RemoveAll();
// 			}
// 		}
// 	}

    return true;
}

bool MSConnLevelZone::BuildSectionOnGivenMemPart1(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
    TheSection.ResetPolyline();
	DrawSection.ResetPolyline();
	m_isBaseSec = false;

	vector<MSConnectivity*> DownLinkJointArr = GetDownLinkJoint();
	long NumDownLinkJoint = DownLinkJointArr.size();

	long NumDownMemberG = 0;
	bool isDownMidJoint = false;
	for(long iDownJoint = 0; iDownJoint < NumDownLinkJoint ; iDownJoint++)
	{
		NumDownMemberG += DownLinkJointArr[iDownJoint]->GetNumMemberG();
		if(DownLinkJointArr[iDownJoint]->IsMidConn())
			isDownMidJoint = true;
	}

    GM2DLineVector EndLVec;
    long nMem = MemPartArr.GetSize();
    if(nMem==0)
        return false;
    else if(nMem==1 )
    {
        MSLinkMemPart* pLink = MemPartArr[0];
        if(nMem==2 && pLink->mp_FloorPartUp)
            pLink = MemPartArr[1];

		long StartOrEnd = mp_Conn->IsSameOrUpDown(pLink->GetSConn()) ? 0 : 1;

		if(m_bUsedDownLinkJoint == FALSE)
		{
			MakeBasePoly(TheSection, true);
		}

		if(m_bUsedDownLinkJoint == TRUE && mp_DownLinkJointBoundry != NULL)
		{
			BuildSectionOnGivenMemPartByDownJoint(TheSection);
		}

        return true;
    }

	long nCount = 0;
	long NumMemberG = GetNumMemberG();
    for(long iMem=0; iMem < nMem; iMem++)
    {
        MSLinkMemPart* pLink = MemPartArr[(iMem)%nMem];
		MSLinkMemPart* pPreLink = MemPartArr[(iMem+nMem-1)%nMem];
		if(pLink->isBuriedLinkMember())
			continue;

		if(MSFloor::ms_TrimType == 0)
		{
			//pLink->SetLRPolyLine(mp_Conn);
			pLink->AddEndJointLine(this,TheSection, NumMemberG);
			if(nMem == 3 && NumMemberG == 2)
			{
// 				map<long, long>::iterator itID = m_ColinerMemMap.find(pLink->mp_LinkMem->m_ID);
// 				if(itID == m_ColinerMemMap.end())
					pLink->AddEndJointLine2(this, DrawSection, 0, 0);
			}
			else
				pLink->AddEndJointLine2(this, DrawSection, 0, 0);
		}
		else if(MSFloor::ms_TrimType == 1)
		{
			pLink->AddEndJointLine2(this,TheSection, nMem, NumMemberG);

			if(nMem == 3 && NumMemberG == 2)
			{
				map<long, long>::iterator itID = m_ColinerMemMap.find(pLink->mp_LinkMem->m_ID);
				if(itID == m_ColinerMemMap.end())
					pLink->AddEndJointLine2(this, DrawSection, 0, 0);
			}
			else
				pLink->AddEndJointLine2(this, DrawSection, 0, 0);
		}
        pPreLink = pLink;
    }

	TheSection.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5
	DrawSection.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5

	if(TheSection.ma_Point.GetSize() == TheSection.ma_Bulge.GetSize())
	{
		if(TheSection.ma_Bulge.GetSize() > 0)
			TheSection.ma_Bulge.RemoveAt(TheSection.ma_Bulge.GetSize()-1);
	}
	
	GM2DVector CenVec(mp_Conn->m_dXc, mp_Conn->m_dYc);
	if(TheSection.CheckPolygon() == FALSE)
		TheSection.MakeSaftPolygon(&CenVec);
	if(DrawSection.CheckPolygon(false) == FALSE)
		DrawSection.MakeSaftPolygon(&CenVec);
	
	if(TheSection.GetNumLines() < 3)
	{
		MakeBasePoly(TheSection, true);
		MakeBasePoly(DrawSection, true);
		//return false;
		return true;
	}

	// 나중에 수정 kkw 
	//MakeMemPartEndJointPoly(TheSection, DrawSection, MemPartArr);
    return true;
}

bool MSConnLevelZone::BuildSectionOnGivenMemPart2(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
	MSPointMemberG* pPointMem=NULL;
	if(mp_Column)
		pPointMem = mp_Column;
	if(mp_IsoFoot)
		pPointMem = mp_IsoFoot;

	vector<MSConnectivity*> DownLinkJointArr = GetDownLinkJoint();
	long NumDownLinkJoint = DownLinkJointArr.size();

	long NumDownMemberG = 0;
	bool isDownMidJoint = false;
	for(long iDownJoint = 0; iDownJoint < NumDownLinkJoint ; iDownJoint++)
	{
		NumDownMemberG += DownLinkJointArr[iDownJoint]->GetNumMemberG();
		if(DownLinkJointArr[iDownJoint]->IsMidConn())
			isDownMidJoint = true;
	}

	GM2DLineVector EndLVec;
    long nMem = MemPartArr.GetSize();
    if(nMem==0 && pPointMem == NULL)
        return false;
	else if(nMem == 0 && pPointMem != NULL)
	{
		GM2DPolyline MyPoly(TRUE);
		pPointMem->GetBoundary(TheSection);
		return true;
	}
    else if(nMem==1)
    {
		//나중에 수정 kkw
		MakeMemPartEndJointPoly(TheSection, DrawSection, MemPartArr);

		for(long iDownJoint = 0; iDownJoint < NumDownLinkJoint ; iDownJoint++)
		{
			MSConnectivity* pDownJoint = DownLinkJointArr[iDownJoint];

			if(pDownJoint != NULL)
			{
				GM2DPolyline* pDownPoly = pDownJoint->GetSection_Hit();
				if(pDownPoly != NULL)
				{
					CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
					TheSection.BoolOperation('U', pDownPoly, JoinedPolys, 1.);
					long NumJoined = JoinedPolys.GetSize();

					if(NumJoined >= 1)
						TheSection.CopyFrom(JoinedPolys[0]);

					for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
						delete JoinedPolys[iPoly];
					JoinedPolys.RemoveAll();
				}
			}
		}

        return true;
    }

	long nCount = 0;
	long NumMemberG = GetNumMemberG();
    for(long iMem=0; iMem<nMem; iMem++)
    {
        MSLinkMemPart* pLink = MemPartArr[iMem];
		if(pLink->isBuriedLinkMember())
			continue;

		if( (NumDownLinkJoint > 0) && nMem == 2 && NumMemberG == 1 && isDownMidJoint && NumDownMemberG == 1 && mp_DownLinkJointBoundry != NULL)
		{
			long StartOrEnd = mp_Conn->IsSameOrUpDown(pLink->GetSConn()) ? 0 : 1;
			mp_DownLinkJointBoundry->Trim(pLink->mp_LLine, StartOrEnd, true);
			mp_DownLinkJointBoundry->Trim(pLink->mp_RLine, StartOrEnd, true);
		}
    }
	
    if(pPointMem)
    {
//         double Xc = pPointMem->ma_Joint[0]->m_dXc;
//         double Yc = pPointMem->ma_Joint[0]->m_dYc;
//         long JointID = pPointMem->ma_Joint[0]->m_ID;
        GM2DPolyline MyPoly(TRUE);
        pPointMem->GetBoundary(MyPoly);
		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
        TheSection.BoolOperation('U', &MyPoly, JoinedPolys, 1.);
        
		if(JoinedPolys.GetSize()==1)
			TheSection.CopyFrom(JoinedPolys[0]);
		else
			TheSection.CopyFrom(&MyPoly);
        //else
        //    ASSERT(0);

		for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
		{
			delete JoinedPolys[iPoly];
		}
		JoinedPolys.RemoveAll();

		DrawSection.MakeClosedPolyline(DTOL_GM*1E3);//ACIS기준 1E-5

		DrawSection.BoolOperation('U', &MyPoly, JoinedPolys, 1.);

		if(JoinedPolys.GetSize()==1)
			DrawSection.CopyFrom(JoinedPolys[0]);
		else
			DrawSection.CopyFrom(&MyPoly);
		//else
		//    ASSERT(0);

		for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
		{
			delete JoinedPolys[iPoly];
		}
		JoinedPolys.RemoveAll();			
    }

	/*
    long nBuriedMem = mp_Conn->ma_BuriedLinkMember.GetSize();
    for(long iBuried=0; iBuried<nBuriedMem; iBuried++)
    {
        MSLinkMemberG* pLink = mp_Conn->ma_BuriedLinkMember[iBuried];
        if(!CheckLevelOverlap(pLink))
            continue;
        GM2DPolyline MyPoly(TRUE);
        pLink->GetBoundary(MyPoly);
        //         GM2DPolygon* pMergedPGon = TheSection.Union(&MyPoly, MSFloor::ms_LineHitDistTol);
		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
		TheSection.BoolOperation('U', &MyPoly, JoinedPolys, 1.);
		if(JoinedPolys.GetSize()==1)
			TheSection.CopyFrom(JoinedPolys[0]);
			//TheSection = *JoinedPolys[0];
		//else
			//ASSERT(0);

		for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
		{
			delete JoinedPolys[iPoly];
		}
		JoinedPolys.RemoveAll();

    }
	if(nBuriedMem > 0)
		TheSection.MakeClosedPolyline();
*/

	// 나중에 수정 kkw 
	MakeMemPartEndJointPoly(TheSection, DrawSection, MemPartArr);
	
// 	for(long iDownJoint = 0; iDownJoint < NumDownLinkJoint ; iDownJoint++)
// 	{
// 		MSConnectivity* pDownJoint = DownLinkJointArr[iDownJoint];
// 
// 		if(pDownJoint != NULL)
// 		{
// 			GM2DPolyline* pDownPoly = pDownJoint->GetSection_Hit();
// 			if(pDownPoly != NULL)
// 			{
// 				CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
// 				TheSection.BoolOperation('U', pDownPoly, JoinedPolys, 1.);
// 				long NumJoined = JoinedPolys.GetSize();
// 
// 				if(NumJoined == 1)
// 					TheSection.CopyFrom(JoinedPolys[0]);
// 
// 				for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
// 					delete JoinedPolys[iPoly];
// 				JoinedPolys.RemoveAll();
// 			}
// 		}
// 	}

    return true;
}

void MSConnLevelZone::GetSectionPolyLine(GM2DPolyline& APolyLine)
{
	/*
	APolyLine.RemoveAll();
	long NumVec = m_Section.ma_Vector.GetSize();
	for(long iVec = 0 ; iVec < NumVec ; iVec++)
	{
		APolyLine.AddCopy(*m_Section.ma_Vector[iVec]);
		APolyLine.ma_Bulge.Add(0);
	}
	APolyLine.AddCopy(*m_Section.ma_Vector[0]);
	*/
	APolyLine.CopyFrom(&m_Section);
}

bool MSConnLevelZone::MakeMemPartEndJointPoly2(GM2DPolyline& TheSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
	bool bRval = false;

	if(TheSection.GetNumLines() < 2)
		return bRval;

	long NumDownJoint = mp_Conn->ma_DownLinkJoint.size();
	for(long iDownJoint = 0 ; iDownJoint < NumDownJoint ; iDownJoint++)
	{
		MSConnectivity* pDownJoint = mp_Conn->ma_DownLinkJoint[iDownJoint];
		if(pDownJoint == NULL) continue;
		if(pDownJoint->GetNumMemPart() > 0)
			return bRval;
	}

	if(m_hasArcLinkMemPartCnt > 1 || m_hasArcLinkMemPartCnt == 1 && ma_MemPart.GetCount() > 2)
		return bRval;

// 	if(m_hasArcLinkMember)
// 		return bRval;
	
// 	if(mp_Column != NULL || mp_IsoFoot != NULL || mp_Conn->ma_DownLinkJoint.size() > 0)
// 		return bRval;

// 	bool isSectionContainDnBoundry = false;
// 	if(mp_DownLinkJointBoundry != NULL)
// 	{
// 		if(TheSection.Contains(mp_DownLinkJointBoundry))
// 			isSectionContainDnBoundry = true;
// 		else
// 			isSectionContainDnBoundry = false;
// 	}
// 
// 	if(!isSectionContainDnBoundry && mp_DownLinkJointBoundry != NULL)
// 		return bRval;
	

	GM2DPolyline* pSectionPoly = new GM2DPolyline(TRUE);
	pSectionPoly->CopyFrom(&TheSection);

	GM2DPolyline BoundPoly(TRUE);
	BoundPoly.CopyFrom(&TheSection);

	long nMem = MemPartArr.GetSize();
	for(long iMem=0; iMem<nMem; iMem++)
	{
		MSLinkMemPart* pLink = MemPartArr[iMem];
		GM2DPolyline* pLinkBound = pLink->GetBoundary2();
		
		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
		BoundPoly.BoolOperation('U', pLinkBound, JoinedPolys, 1);
		long NumJoindPoly = JoinedPolys.GetSize();
		if(NumJoindPoly != 1)
		{
			for(long iPoly = 0 ; iPoly < NumJoindPoly ; iPoly++)
				delete JoinedPolys[iPoly];
			JoinedPolys.RemoveAll();

			BoundPoly.BoolOperation('R', pLinkBound, JoinedPolys, 1);
			NumJoindPoly = JoinedPolys.GetSize();
		}

		if(NumJoindPoly > 0)
			BoundPoly.CopyFrom(JoinedPolys[0]);

		for(long iPoly = 0 ; iPoly < JoinedPolys.GetSize() ; iPoly++)
			delete JoinedPolys[iPoly];
		JoinedPolys.RemoveAll();
	}

	pSectionPoly->SegmentstoMe(&BoundPoly);
	long NumLine = pSectionPoly->GetNumLines();
	for(long iLine = 0 ; iLine < NumLine ; iLine++)
		pSectionPoly->SetVisibility(iLine, false);

	for(long iLine = 0 ; iLine < NumLine ; iLine++)
	{
		GM2DCurve* pCurve =  pSectionPoly->GetCurve(iLine);

		GM2DVector CenVec = pCurve->PointAtParam(0.5);

		long nSVal = BoundPoly.Contains(&pCurve->m_P0);
		long nEVal = BoundPoly.Contains(&pCurve->m_P1);
		long nCVal = BoundPoly.Contains(&CenVec);
		
		if(nSVal == 0 && nEVal == 0 && nCVal == 0)
		{
			bool bHasOnLine = false;
			for(long iMem=0; iMem<nMem; iMem++)
			{
				MSLinkMemPart* pLink = MemPartArr[iMem];
				bool bLSVal = pLink->mp_LLine->HasOnLine(pCurve->m_P0, DTOL10);
				bool bLEVal = pLink->mp_LLine->HasOnLine(pCurve->m_P1, DTOL10);
				bool bLCVal = pLink->mp_LLine->HasOnLine(CenVec, DTOL10);
				
				if(bLSVal && bLEVal && bLCVal)
					bHasOnLine = true;

				if(bHasOnLine)
					break;

				bool bRSVal = pLink->mp_RLine->HasOnLine(pCurve->m_P0, DTOL10);
				bool bREVal = pLink->mp_RLine->HasOnLine(pCurve->m_P1, DTOL10);
				bool bRCVal = pLink->mp_RLine->HasOnLine(CenVec, DTOL10);

				if(bRSVal && bREVal && bRCVal)
					bHasOnLine = true;

				if(bHasOnLine)
					break;
			}

			if(!bHasOnLine)
				pSectionPoly->SetVisibility(iLine, true);
		}

		if(mp_DownLinkJointBoundry != NULL)
		{
			long NumDownLine = mp_DownLinkJointBoundry->GetNumLines();
			for(long jLine = 0 ; jLine < NumDownLine ; jLine++)
			{
				GM2DCurve* pDownCurve =  mp_DownLinkJointBoundry->GetCurve(jLine);

				bool bHasOnLine = false;

				bool bRSVal = pDownCurve->HasOnLine(pCurve->m_P0, DTOL10);
				bool bREVal = pDownCurve->HasOnLine(pCurve->m_P1, DTOL10);
				bool bRCVal = pDownCurve->HasOnLine(CenVec, DTOL10);

				if(bRSVal && bREVal && bRCVal)
					bHasOnLine = true;

				if(bHasOnLine)
					pSectionPoly->SetVisibility(iLine, false);

				delete pDownCurve;
			}
		}

		delete pCurve;
	}

	for(long iMem=0; iMem<nMem; iMem++)
	{
		MSLinkMemPart* pLink = MemPartArr[iMem];
		long StartOrEnd = pLink->GetSConn()==mp_Conn ? 0 : 1;
		pLink->AddEndJointPoly(this, StartOrEnd, pSectionPoly);
		bRval = true;
		break;
	}
	if(bRval == false)
		delete pSectionPoly;

	return bRval;
}

bool MSConnLevelZone::MakeMemPartEndJointPoly(GM2DPolyline& TheSection,GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
	double dLineHitDist = 10.;

	long nMem = MemPartArr.GetSize();

	
 	if(nMem != 1 /*&& (m_ColinerMemMap.size() > 1 || m_hasBuriedLinkMember)*/)
 		return MakeMemPartEndJointPoly2(TheSection, MemPartArr);

	if(nMem != 1 && m_isBaseSec != true)
		//if(nMem != 1 || (m_hasColinearDownMemPart != true && isDownMidJoint == true && m_isBaseSec != true)) // 0529
			//if(nMem != 1 || mp_Column != NULL || mp_IsoFoot != NULL || (m_hasColinearDownMemPart != true && isDownMidJoint == true && m_isBaseSec != true))
				return false;
	

	if(mp_Conn->GetTopConLevelZone() != this)
		return false;

	GM2DPolyline pSectionPoly(TRUE);

	MSPointMemberG* pPointMem=NULL;
	if(mp_Column)
		pPointMem = mp_Column;
	if(mp_IsoFoot)
		pPointMem = mp_IsoFoot;

	/*
	if(pPointMem && nMem == 1)
		pPointMem->GetBoundary(pSectionPoly);
	else
		GetSectionPolyLine(pSectionPoly);
		*/
	if(mp_DownLinkJointBoundry != NULL)
		pSectionPoly.CopyFrom(mp_DownLinkJointBoundry);
	else if(pPointMem && nMem == 1)
		pPointMem->GetBoundary(pSectionPoly);
	else
		pSectionPoly.CopyFrom(&TheSection);
		//GetSectionPolyLine(pSectionPoly);
	
	for(long iMem=0; iMem<nMem; iMem++)
	{
		MSLinkMemPart* pLink = MemPartArr[iMem];
// 		if(pLink->mp_LinkMem->GetType() == MSElement::msLineLevelDifG)
// 			continue;

		long StartOrEnd = pLink->GetSConn()==mp_Conn ? 0 : 1;
		GM2DPolyline* pEndPoly = new GM2DPolyline(TRUE);
		GM2DPolyline* pLinkPoly = NULL;
		GM2DLineVector EndLine;


		// 추후에 구현 벽에 수평부재 라인 늘려주기
// 		if(pLink->mp_LinkMem->ma_MemPart.size() == 1)
// 		{
// 			GM2DPolyline ThePoly(TRUE);
// 			pLink->mp_LinkMem->GetBoundary(ThePoly);
// 			pLinkPoly = new GM2DPolyline(TRUE);
// 			pLinkPoly->CopyFrom(&ThePoly);
// 			
// 			EndLine = pLink->GetEndLine(StartOrEnd, TRUE);
// 		}
// 		else
		{
			pLinkPoly = pLink->GetBoundary(FALSE);
			EndLine = pLink->GetEndLine(StartOrEnd, FALSE);
		}
		double dEndLineLen = EndLine.Length();
		//GM2DPolyline* pLinkPoly = pLink->GetMemberBoundary();
		//GM2DLineVector EndLine = pLink->GetMemberEndLine(StartOrEnd);
		/*
		if(mp_DownLinkJointBoundry != NULL)
		{
			long nRavl0 = mp_DownLinkJointBoundry->Contains(&EndLine.m_P0, MSFloor::ms_LineHitDistTol);
			long nRavl1 = mp_DownLinkJointBoundry->Contains(&EndLine.m_P1, MSFloor::ms_LineHitDistTol);
			if(nRavl0 < 1 && nRavl1 < 1)
				continue;
		}
		*/

		//CTypedPtrArray<CObArray, GM2DVector*> CrossVecArr;
		vector<GM2DVector> CrossVecArr;

		if(pSectionPoly.GetNumLines() < 1)
			return false;

		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
		pSectionPoly.BoolOperation('E', pLinkPoly, JoinedPolys, DTOL_GM);

		long NumJoinedPoly = JoinedPolys.GetCount();

		if(NumJoinedPoly == 1 )
		{
			pEndPoly->CopyFrom(JoinedPolys[0]);
			
			//long NumCrossVec = pEndPoly->Intersect(EndLine, CrossVecArr);
			CrossVecArr = pEndPoly->IntersectEx(&EndLine);
			long NumCrossVec = CrossVecArr.size();

			if(NumCrossVec == 1)
			{
				GM2DVector* pCrossVec = &CrossVecArr[0];
				//long nRval0 = pSectionPoly.Contains(&EndLine.m_P0, MSFloor::ms_LineHitDistTol);
				long nRval0 = pSectionPoly.Contains(&EndLine.m_P0, dLineHitDist);
				if(nRval0 == -1 || nRval0 == 0)
				{
					EndLine.m_P0.SetXY(pCrossVec->m_X, pCrossVec->m_Y);
				}
				//long nRval1 = pSectionPoly.Contains(&EndLine.m_P1, MSFloor::ms_LineHitDistTol);
				long nRval1 = pSectionPoly.Contains(&EndLine.m_P1, dLineHitDist);
				if(nRval1 == -1 || nRval1 == 0)
				{
					EndLine.m_P1.SetXY(pCrossVec->m_X, pCrossVec->m_Y);
				}
				double dLen = EndLine.Length();
				if( ( fabs(dEndLineLen - dLen) > DTOL_GM)  &&dLen > DTOL_GM) // 부재의 끝선이 그대로이거나 매우 짧아지면 추가 안한다.
					pEndPoly->AddLine(EndLine);
			}
			else if(NumCrossVec > 1)
			{
				GM2DVector* pCrossVec = &CrossVecArr[0];
				GM2DVector* pCrossVec2 = &CrossVecArr[CrossVecArr.size()-1];
				GM2DLineVector tmpLine, tmpLine2;
				double dLoc = 0 , dLoc2 = 0;
				EndLine.LocValue(*pCrossVec, &dLoc);
				EndLine.LocValue(*pCrossVec2, &dLoc2);

				if(dLoc < dLoc2)
				{
					tmpLine.m_P0.SetXY(EndLine.m_P0.m_X, EndLine.m_P0.m_Y);
					tmpLine.m_P1.SetXY(pCrossVec->m_X, pCrossVec->m_Y);

					tmpLine2.m_P0.SetXY(pCrossVec2->m_X, pCrossVec2->m_Y);
					tmpLine2.m_P1.SetXY(EndLine.m_P1.m_X, EndLine.m_P1.m_Y);
				}
				else
				{
					tmpLine.m_P0.SetXY(EndLine.m_P0.m_X, EndLine.m_P0.m_Y);
					tmpLine.m_P1.SetXY(pCrossVec2->m_X, pCrossVec2->m_Y);

					tmpLine2.m_P0.SetXY(pCrossVec->m_X, pCrossVec->m_Y);
					tmpLine2.m_P1.SetXY(EndLine.m_P1.m_X, EndLine.m_P1.m_Y);
				}
				if(tmpLine.Length() > DTOL10)
					pEndPoly->AddLine(tmpLine);
				if(tmpLine2.Length() > DTOL10)
					pEndPoly->AddLine(tmpLine2);
			}
			else if(NumCrossVec == 0)
			{
				pEndPoly->ResetPolyline();
				pEndPoly->AddLine(EndLine);
			}

			pLink->AddEndJointPoly(this, StartOrEnd, pEndPoly);
		}
		else
		{
			pEndPoly->ResetPolyline();
			pEndPoly->AddLine(EndLine);
			pLink->AddEndJointPoly(this, StartOrEnd, pEndPoly);
			//pEndPoly->AddLine(EndLine);
			//pLink->AddEndJointPoly(this, StartOrEnd, pEndPoly);
			//ASSERT(0);
		}
		
		delete pLinkPoly;
		
		long NumJoind = JoinedPolys.GetSize();
		for(long iJoind = 0 ; iJoind < NumJoind ; iJoind++)
		{
			delete JoinedPolys[iJoind];
		}
		JoinedPolys.RemoveAll();
	}
	return true;
}

double MSConnLevelZone::GetMemPartByWidth(bool isMax)
{
	double dWidth = 0;
	long NumPart = ma_MemPart.GetCount();
	for(int iPart = 0 ; iPart < NumPart ; iPart++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[iPart];
		if(pMemPart->isBuriedLinkMember()) continue;
		if(pMemPart == NULL || pMemPart->mp_LinkMem == NULL || pMemPart->mp_LinkMem->mp_CompoM == NULL)
			continue;
		if(iPart == 0)
			dWidth = pMemPart->mp_LinkMem->mp_CompoM->GetWidth();
		else
		{
			if(isMax)
				dWidth = max(dWidth,  pMemPart->mp_LinkMem->mp_CompoM->GetWidth());
			else
				dWidth = min(dWidth,  pMemPart->mp_LinkMem->mp_CompoM->GetWidth());
		}
	}

	return dWidth;
}

double MSConnLevelZone::GetMemPartByMaxWidth()
{
	return GetMemPartByWidth(true);
}

double MSConnLevelZone::GetMemPartByMinWidth()
{
	return GetMemPartByWidth(false);
}

long MSConnLevelZone::GetNumMemberG()
{
	if(m_nNumMemberG == 0)
		SetNumMemberG();
	return m_nNumMemberG;
}

void MSConnLevelZone::SetNumMemberG()
{
	long NumLinkMember = 0;
	map<long, MSCompoG*> LinkMemMap;
	long nMem = ma_MemPart.GetCount();
	for(int i=0; i< nMem ; i++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[i];
		//if(pMemPart->isBuriedLinkMember()) continue;

		LinkMemMap.insert(make_pair(pMemPart->mp_LinkMem->m_ID, pMemPart->mp_LinkMem));
	}
	m_nNumMemberG = LinkMemMap.size();
}

vector<MSConnectivity*> MSConnLevelZone::GetDownLinkJoint()
{
	vector<MSConnectivity*> DownLinkJointArr;
	if(mp_Conn == NULL)
		return DownLinkJointArr;

	vector<MSCompoG*> LowWallArr = mp_Conn->GetLinkMemberG(MSElement::msHandRailG);
	long NumLowWall = LowWallArr.size();

	if(NumLowWall == 0)
		return mp_Conn->ma_DownLinkJoint;

	double dFloorZLevel = 0.;
	MSFloor* pUpperFloor = mp_Conn->mp_Floor->GetUpperFloor();
	if(pUpperFloor != NULL)
	{
		dFloorZLevel = pUpperFloor->GetZLevel(mp_Conn->m_dXc, mp_Conn->m_dYc);

		if(dFloorZLevel < m_TopLevel)
			return mp_Conn->ma_UppperFloorHitVerJoit;
		if(dFloorZLevel > m_BotLevel)
			return mp_Conn->ma_DownLinkJoint;
	}
	
	return DownLinkJointArr;
}

void MSConnLevelZone::DeleteDownLinkJointPoly()
{
	if(mp_DownLinkJointBoundry != NULL)
		delete mp_DownLinkJointBoundry;
	mp_DownLinkJointBoundry = NULL;
}

bool MSConnLevelZone::HasHandRail()
{
	long NumPart = ma_MemPart.GetSize();
	for(long iPart = 0 ; iPart < NumPart ; iPart++)
	{
		if(ma_MemPart[iPart]->mp_LinkMem->GetType() == MSElement::msHandRailG)
			return true;
	}
	return false;
}

GM2DVector MSConnLevelZone::CalculateSegmentList( CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr)
{
    long NumMemPart = MemPartArr.GetCount();
    vector<GM2DVector> CrossVectors;

    for(long iPart = 0 ; iPart < NumMemPart ;iPart++)
    {
        MSLinkMemPart* pMyMemPart = MemPartArr[iPart];
        GM2DCurve* pMyCenCurve =  pMyMemPart->mp_CenLine;

        for(long jPart = 0 ; jPart < NumMemPart ;jPart++)
        {
            // skip for same parts
            if(iPart == jPart) continue; 
            
            MSLinkMemPart* pUrMemPart = MemPartArr[jPart];
            // skip for same linkmembers
            if(pMyMemPart->mp_LinkMem == pUrMemPart->mp_LinkMem) continue; 

            GM2DCurve* pUrCenCurve =  pUrMemPart->mp_CenLine;

            vector<GM2DVector> CrossVecs;
            vector<double> MyLocArr, UrLocArr;
            vector<long> ResutlArr;
            long ResultIdx = 0;
            GM2DVector MyCrossVec;

            long nRval = pMyCenCurve->IntersectEnd2(pUrCenCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true);

            // in case of intersection at 1 point
            if(abs(nRval%10)==1)
            {
                for(long i = 0 ; i < ResutlArr.size() ; i++)
                {
                    if(ResutlArr[i] == 1)
                    {
                        // get index of the intersection point in CrossVecs
                        ResultIdx = i;
                        break;
                    }
                }

                // get the intersection point
                MyCrossVec = CrossVecs[ResultIdx]; // check later when result index is not reset the former loop

                //if(pMyMemPart->mp_LinkMem != pUrMemPart->mp_LinkMem)
                {
                    long NumCross = CrossVectors.size();
                    bool exist = false;
                    for(long iNum = 0 ; iNum < NumCross ; iNum++)
                    {
                        if (MyCrossVec.SameVector4DTOL(CrossVectors[iNum]))
                        {
                            exist = true;
                            break;
                        }
                    }
                    // gather CrossVectors for calculating a real center point later
                    if(!exist)
                        CrossVectors.push_back(MyCrossVec);
                }
            }
            else if(abs(nRval%10)==2)
            {
                MyCrossVec = (CrossVecs[0]+CrossVecs[1])*0.5;
            }

            // trim MyMemPart Segments with UrMemPart Boundary
            TrimMemPartSegment(pMyMemPart->ma_LSegments, MyCrossVec, pMyMemPart, pUrMemPart);
            TrimMemPartSegment(pMyMemPart->ma_RSegments, MyCrossVec, pMyMemPart, pUrMemPart);
            
            GM2DPolyline* pUrPoly = pUrMemPart->GetTrimmedBoundary();
            if (pMyMemPart->IsStart())
                FilletTriangularProtrusion(pMyMemPart->ma_SSegments, pMyMemPart->mp_LinkMem, pUrPoly);
            if (pMyMemPart->IsEnd())
                FilletTriangularProtrusion(pMyMemPart->ma_ESegments, pMyMemPart->mp_LinkMem, pUrPoly);
        }

        GatherSegmentList(pMyMemPart->ma_LSegments, iPart, true, false);
        GatherSegmentList(pMyMemPart->ma_RSegments, iPart, false, false);
        //GatherSegmentList(pMyMemPart->ma_SSegments, iPart, NULL, true);
        //GatherSegmentList(pMyMemPart->ma_ESegments, iPart, NULL, true);
    }

    // calculate a real center point
    GM2DVector Center;
    long NumVec = CrossVectors.size();
    if (NumVec>0)
    {
        for(long iVec = 0 ; iVec < NumVec ; iVec++)
            Center += CrossVectors[iVec];
        Center = Center * (1./NumVec);
    }
    else
        Center.SetXY(mp_Conn->m_dXc, mp_Conn->m_dYc);

    return Center;
}

void MSConnLevelZone::GatherSegmentList( vector<GM2DCurve*> Segments, long iPart, bool LLine, bool EndLine )
{
    long NumLSeg = Segments.size();
    for(long iSeg = 0 ; iSeg < NumLSeg ; iSeg++)
    {
        GM2DCurve* pCurve = Segments[iSeg];
        SegStruct SegData;
        SegData.dDeg = 0.0;
        SegData.MemPartIdx = iPart;
        SegData.pCurve = pCurve;
        SegData.isLLine = LLine;
        SegData.isEndLine = EndLine;
        ma_SegList.push_back(SegData);
    }
}

void MSConnLevelZone::SortSegments( GM2DVector Center )
{
    // get the angle from Center to MidVec
    long NumSeg = ma_SegList.size();
    for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
    {
        SegStruct& SegData = ma_SegList[iSeg];
        GM2DCurve* pCurve = SegData.pCurve;
        double length = pCurve->Length();

        GM2DVector MidVec = pCurve->PointAtParam(0.5);

        // tuning for exact trim
        /*        if (SegData.isEndLine)
        {
        GM2DVector start = pCurve->OrthoBaseVector(Center);
        double distance = pCurve->Distance(Center);
        double angle = Center.GetRadian(start);
        if (distance > 3000)
        {
        GM2DVector direction(start, distance - 2000, angle);
        MidVec += direction;
        }
        }
        else */ if (length > 600)
        {
            double location = pCurve->OrthoBaseLocValue(Center);
            if (pCurve->m_ObjectType == "GM2DArc")
            {
                if (location < 0.5)
                    MidVec = pCurve->PointAtParam(0.01);
                else
                    MidVec = pCurve->PointAtParam(0.09);
            }
            else
            {
                if (location < 0.5)
                    MidVec = pCurve->PointAtParam(0.2);
                else
                    MidVec = pCurve->PointAtParam(0.8);
            }
        }
        SegData.dDeg = Center.GetDegree(MidVec);
    }

    // sort according to angles
    sort(ma_SegList.begin(), ma_SegList.end(), SegStructCompare);
}

void MSConnLevelZone::MakeConnProfileWhenCoLinear( SegStruct* MySegData, SegStruct * UrSegData, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr )
{
    double UrP0Loc = 0;
    double UrP1Loc = 0;
    long nOLVal = MySegData->pCurve->Overlap4DTolBase(UrSegData->pCurve, UrP0Loc, UrP1Loc);
    GM2DVector CrossVec;

    switch (nOLVal)
    {
    case 1: // UrSegment is over the start point of MySegment
        if(UrP0Loc<UrP1Loc)
            CrossVec = UrSegData->pCurve->GetP1();
        else
            CrossVec = UrSegData->pCurve->GetP0();
        MySegData->pCurve->SetP0(CrossVec);
        break;
    case 25: // UrSegment is over the end point of MySegment
        if(UrP0Loc<UrP1Loc)
            CrossVec = UrSegData->pCurve->GetP0();
        else
            CrossVec = UrSegData->pCurve->GetP1();
        MySegData->pCurve->SetP1(CrossVec);
        break;
    case 2:
    case 6:  // UrSegment meets MySegment at the start point of MySegment.
        CrossVec = MySegData->pCurve->GetP0();
        break;
    case 20:
    case 24: // UrSegment meets MySegment at the end point of MySegment.
        CrossVec = MySegData->pCurve->GetP1();
        break;
    }

    if(MySegData->MemPartIdx != UrSegData->MemPartIdx)
    {
        // add point to Connection
        if (MemPartArr[MySegData->MemPartIdx]->mp_LinkMem != MemPartArr[UrSegData->MemPartIdx]->mp_LinkMem)
        {
            m_DrawSection.AddCopy(CrossVec);
            m_DrawSection.ma_Bulge.Add(0.);
        }
        m_Section.AddCopy(CrossVec);
        m_Section.ma_Bulge.Add(0.);
        return;
    }
    return;
}

void MSConnLevelZone::MakeConnProfileWhenParallel( SegStruct * MySegData, SegStruct * UrSegData, GM2DVector Center, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr )
{
    double UrP0Loc = 0;
    double UrP1Loc = 0;
    long nOLVal = MySegData->pCurve->Overlap4DTolBase(UrSegData->pCurve, UrP0Loc, UrP1Loc);
    GM2DVector CrossVec;
    GM2DVector EndVec;

    double Dist2Me = MySegData->pCurve->Distance(Center);
    double Dist2U = UrSegData->pCurve->Distance(Center);
    bool NearMe = true;
    if (Dist2Me>Dist2U)
        NearMe = false;

    switch (nOLVal)
    {
    case 1: // UrSegment is over the start point of MySegment
        if (NearMe)
        {
            if(UrSegData->isLLine)
            {
                if(UrP0Loc<UrP1Loc)
                {
                    EndVec = MemPartArr[UrSegData->MemPartIdx]->mp_LLine->GetP1();
                    UrSegData->pCurve->SetP1(EndVec);
                }
                else
                {
                    EndVec = MemPartArr[UrSegData->MemPartIdx]->mp_LLine->GetP0();
                    UrSegData->pCurve->SetP0(EndVec);
                }
            }
            else
            {
                if(UrP0Loc<UrP1Loc)
                {
                    EndVec = MemPartArr[UrSegData->MemPartIdx]->mp_RLine->GetP1();
                    UrSegData->pCurve->SetP1(EndVec);
                }
                else
                {
                    EndVec = MemPartArr[UrSegData->MemPartIdx]->mp_RLine->GetP0();
                    UrSegData->pCurve->SetP0(EndVec);
                }
            }
            double loc = MySegData->pCurve->OrthoBaseLocValue(EndVec);
            CrossVec = MySegData->pCurve->PointAtParam(loc);
            MySegData->pCurve->SetP0(CrossVec);
        }
        else
        {
            if(MySegData->isLLine)
                EndVec = MemPartArr[MySegData->MemPartIdx]->mp_LLine->GetP0();
            else
                EndVec = MemPartArr[MySegData->MemPartIdx]->mp_RLine->GetP0();
            MySegData->pCurve->SetP0(EndVec);
            double loc = UrSegData->pCurve->OrthoBaseLocValue(EndVec);
            CrossVec = UrSegData->pCurve->PointAtParam(loc);
            if(loc<0.5)
                UrSegData->pCurve->SetP0(CrossVec);
            else
                UrSegData->pCurve->SetP1(CrossVec);
        }
        break;

    case 25: // UrSegment is over the end point of MySegment
        if (NearMe)
        {
            if(UrSegData->isLLine)
            {
                if(UrP0Loc<UrP1Loc)
                {
                    EndVec = MemPartArr[UrSegData->MemPartIdx]->mp_LLine->GetP0();
                    UrSegData->pCurve->SetP0(EndVec);
                }
                else
                {
                    EndVec = MemPartArr[UrSegData->MemPartIdx]->mp_LLine->GetP1();
                    UrSegData->pCurve->SetP1(EndVec);
                }
            }
            else
            {
                if(UrP0Loc<UrP1Loc)
                {
                    EndVec = MemPartArr[UrSegData->MemPartIdx]->mp_RLine->GetP0();
                    UrSegData->pCurve->SetP0(EndVec);
                }
                else
                {
                    EndVec = MemPartArr[UrSegData->MemPartIdx]->mp_RLine->GetP1();
                    UrSegData->pCurve->SetP1(EndVec);
                }
            }
            double loc = MySegData->pCurve->OrthoBaseLocValue(EndVec);
            CrossVec = MySegData->pCurve->PointAtParam(loc);
            MySegData->pCurve->SetP1(CrossVec);
        }
        else
        {
            if(MySegData->isLLine)
                EndVec = MemPartArr[MySegData->MemPartIdx]->mp_LLine->GetP1();
            else
                EndVec = MemPartArr[MySegData->MemPartIdx]->mp_RLine->GetP1();
            MySegData->pCurve->SetP1(EndVec);
            double loc = UrSegData->pCurve->OrthoBaseLocValue(EndVec);
            CrossVec = UrSegData->pCurve->PointAtParam(loc);
            if(loc<0.5)
                UrSegData->pCurve->SetP0(CrossVec);
            else
                UrSegData->pCurve->SetP1(CrossVec);
        }
        break;

    case 2:
        if (NearMe)
        {
            CrossVec = MySegData->pCurve->GetP0();
            EndVec = UrSegData->pCurve->GetP1();
        }
        else
        {
            CrossVec = UrSegData->pCurve->GetP1();
            EndVec = MySegData->pCurve->GetP0();
        }
        break;

    case 6:  // UrSegment meets MySegment at the start point of MySegment.
        if (NearMe)
        {
            CrossVec = MySegData->pCurve->GetP0();
            EndVec = UrSegData->pCurve->GetP0();
        }
        else
        {
            CrossVec = UrSegData->pCurve->GetP0();
            EndVec = MySegData->pCurve->GetP0();
        }
        break;

    case 20:
        if (NearMe)
        {
            CrossVec = MySegData->pCurve->GetP1();
            EndVec = UrSegData->pCurve->GetP0();
        }
        else
        {
            CrossVec = UrSegData->pCurve->GetP0();
            EndVec = MySegData->pCurve->GetP1();
        }
        break;

    case 24: // UrSegment meets MySegment at the end point of MySegment.
        if (NearMe)
        {
            CrossVec = MySegData->pCurve->GetP1();
            EndVec = UrSegData->pCurve->GetP1();
        }
        else
        {
            CrossVec = UrSegData->pCurve->GetP1();
            EndVec = MySegData->pCurve->GetP1();
        }
        break;
    }

    GM2DCurve* pLine = new GM2DLineVector(CrossVec, EndVec);

    if (NearMe)
    {
        MSLinkMemPart* pPart = MemPartArr[UrSegData->MemPartIdx];
//         if (pPart->mp_LinkMem->GetSConn() == this->mp_Conn)
//             pPart->ma_SSegments.push_back(pLine);
//         else if (pPart->mp_LinkMem->GetEConn() == this->mp_Conn)
            pPart->ma_ESegments.push_back(pLine);
    }
    else
    {
        MSLinkMemPart* pPart = MemPartArr[MySegData->MemPartIdx];
//         if (pPart->mp_LinkMem->GetSConn() == this->mp_Conn)
//             pPart->ma_SSegments.push_back(pLine);
//         else if (pPart->mp_LinkMem->GetEConn() == this->mp_Conn)
            pPart->ma_ESegments.push_back(pLine);
    }
    // add point to Connection
    m_DrawSection.AddCopy(CrossVec);
    m_DrawSection.ma_Bulge.Add(0.);
    m_Section.AddCopy(CrossVec);
    m_Section.ma_Bulge.Add(0.);
    return;
}

void MSConnLevelZone::MakeConnProfileWhenCross( SegStruct * MySegData, SegStruct * UrSegData )
{
    vector<GM2DVector> CrossVecs;
    vector<double> MyLocArr, UrLocArr;
    vector<long> ResutlArr;
    long ResultIdx = -1;

    long nRval = MySegData->pCurve->IntersectEnd2(UrSegData->pCurve, CrossVecs, MyLocArr, UrLocArr, ResutlArr, true);

    switch (nRval)
    {
    case 1:
    case -1:
    case 11:
        for(long i = 0 ; i < ResutlArr.size() ; i++)
            if(ResutlArr[i] == 1)// || ResutlArr[i] == 2)
            {
                ResultIdx = i;
                break;
            }
            break;

    case -2:
        double temp0 = min(fabs(MyLocArr[0]-0.0), fabs(MyLocArr[0]-1.0));
        double temp1 = min(fabs(MyLocArr[1]-0.0), fabs(MyLocArr[1]-1.0));

        if(temp0<temp1)
            ResultIdx = 0;
        else
            ResultIdx = 1;
        break;

    }

    if (ResultIdx>-1)
    {
        GM2DVector MyCrossVec = CrossVecs[ResultIdx];
        double MyLoc = MyLocArr[ResultIdx];
        double UrLoc = UrLocArr[ResultIdx];

        m_DrawSection.AddCopy(MyCrossVec);
        m_DrawSection.ma_Bulge.Add(0.);
        m_Section.AddCopy(MyCrossVec);
        m_Section.ma_Bulge.Add(0.);

        if(fabs(MyLoc) > DTOL_GM)
        {
            if(fabs(MyLoc -1) > DTOL_GM )
            {
                if(MyLoc > 0.5)
                    MySegData->pCurve->SetP1(MyCrossVec);
                else
                    MySegData->pCurve->SetP0(MyCrossVec);
            }
        }

        if(fabs(UrLoc) > DTOL_GM)
        {
            if(fabs(UrLoc -1) > DTOL_GM )
            {
                if(UrLoc > 0.5)
                    UrSegData->pCurve->SetP1(MyCrossVec);
                else
                    UrSegData->pCurve->SetP0(MyCrossVec);
            }
        }
    }
}

void MSConnLevelZone::FilterEndLines( MSLinkMemPart* pMyMemPart, bool start )
{
    GM2DPolyline* pLPoly = pMyMemPart->mp_LPolyLine;
    GM2DPolyline* pRPoly = pMyMemPart->mp_RPolyLine;
    MSLinkMemberG* pLink = pMyMemPart->mp_LinkMem;
    // get end points of the link member
    vector<GM2DVector> endVecs;
    
    if (start)
    {
        endVecs.push_back(pLPoly->FirstPnt());
        endVecs.push_back(pRPoly->FirstPnt());

        long result1 = CheckEndVectors(pLink->mp_LLine->GetP0(), endVecs);
        long result2 = CheckEndVectors(pLink->mp_RLine->GetP0(), endVecs);
        if ((result1<0)&&(result2<0))
            pMyMemPart->ma_SSegments.clear();

        if(pMyMemPart->ma_SSegments.size() < 1)
            pMyMemPart->mp_SPolyLine->ResetPolyline();
        else
            pMyMemPart->mp_SPolyLine->AddSegments(pMyMemPart->ma_SSegments);
    }
    else
    {
        endVecs.push_back(pLPoly->LastPnt());
        endVecs.push_back(pRPoly->LastPnt());

        long result1 = CheckEndVectors(pLink->mp_LLine->GetP1(), endVecs);
        long result2 = CheckEndVectors(pLink->mp_RLine->GetP1(), endVecs);
        if ((result1<0)&&(result2<0))
            pMyMemPart->ma_ESegments.clear();

        if(pMyMemPart->ma_ESegments.size() < 1)
            pMyMemPart->mp_EPolyLine->ResetPolyline();
        else
            pMyMemPart->mp_EPolyLine->AddSegments(pMyMemPart->ma_ESegments);
    }
}

void MSConnLevelZone::CheckBuriedMember()
{
	if(m_hasBuriedLinkMember)
	{
		GM2DPolyline* pPoly = GetSection();
		if(pPoly != NULL)
		{
			bool bCheck = false;
			long NumBuriedPart = ma_BuriedMemPart.GetSize();
			for(long iPart = 0 ; iPart < NumBuriedPart ; iPart++)
			{
				MSLinkMemPart* pPart = ma_BuriedMemPart[iPart];
				if(pPart->mp_LLine != NULL && pPart->mp_RLine != NULL)
				{
					GM2DVector LCenVec = pPart->mp_LLine->PointAtParam(0.5);
					GM2DVector RCenVec = pPart->mp_RLine->PointAtParam(0.5);
					if(pPoly->Contains(&LCenVec) == -1 || pPoly->Contains(&RCenVec) == -1)
					{
						bCheck = true;
						break;
					}

					if(pPoly->Contains(&pPart->mp_LLine->m_P0) == -1 ||
						pPoly->Contains(&pPart->mp_LLine->m_P1) == -1 ||
						pPoly->Contains(&pPart->mp_RLine->m_P0) == -1 ||
						pPoly->Contains(&pPart->mp_RLine->m_P1) == -1)
					{
						bCheck = true;
						break;
					}
				}
				
				MSLinkMemberG* pLinkG = pPart->mp_LinkMem;
				if(pLinkG->mp_LLine != NULL && pLinkG->mp_RLine != NULL)
				{
					GM2DVector LCenVec = pLinkG->mp_LLine->PointAtParam(0.5);
					GM2DVector RCenVec = pLinkG->mp_RLine->PointAtParam(0.5);
					if(pPoly->Contains(&LCenVec) == -1 || pPoly->Contains(&RCenVec) == -1)
					{
						bCheck = true;
						break;
					}

					if(pPoly->Contains(&pLinkG->mp_LLine->m_P0) == -1 ||
						pPoly->Contains(&pLinkG->mp_LLine->m_P1) == -1 ||
						pPoly->Contains(&pLinkG->mp_RLine->m_P0) == -1 ||
						pPoly->Contains(&pLinkG->mp_RLine->m_P1) == -1)
					{
						bCheck = true;
						break;
					}
				}

			}
			if(bCheck == true)
			{
				BuildSection(true);
				for(long iPart = 0 ; iPart < NumBuriedPart ; iPart++)
				{
					MSLinkMemPart* pPart = ma_BuriedMemPart[iPart];
					GM2DVector OrgVec(mp_Conn->m_dXc, mp_Conn->m_dYc);
					pPart->mp_LLine->m_P0 = OrgVec;
					pPart->mp_LLine->m_P1 = OrgVec;
					pPart->mp_RLine->m_P0 = OrgVec;
					pPart->mp_RLine->m_P1 = OrgVec;
				}
			}
		}
	}
}

void MSConnLevelZone::DeleteEndJointPoly()
{
	long nMemPart = ma_MemPart.GetSize();
	for(long iMemPart=0; iMemPart<nMemPart; iMemPart++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[iMemPart];
		if(pMemPart->GetSConn() == mp_Conn)
			pMemPart->DeleteEndJointPoly(this, 0);
		else
			pMemPart->DeleteEndJointPoly(this, 1);
	}
}

void MSConnLevelZone::Delete()
{
	m_bDeleted = true;
}

void MSConnLevelZone::ClearAllDeleted()
{
	map<MSConnLevelZone*, bool>::iterator it;
	for(it = _deleted_map.begin(); it != _deleted_map.end(); it++)
		delete it->first;
	_deleted_map.clear();
}

void MSConnLevelZone::CheckBasePoly()
{
	if(GetNumMemberG() != 2)
		return;
	GM2DPolyline* pPoly = GetSection();
	if(pPoly == NULL)
	{
		m_isBaseSec = true;
		return;
	}
	if(pPoly->GetNumLines() < 3)
		return;

	double dArea = pPoly->Area();
	double dWidth = GetMemPartByMaxWidth();
	if(dArea < dWidth * 7)
		m_isBaseSec = true;
}

