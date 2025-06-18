#include "StdAfx.h"
#include "MSConnectivity.h"

#include "GMLib\GM2DVector.h"
#include "GMLib\GM2DLineVector.h"
#include "GMLib\GMVector.h"
#include "GMLib\GMText.h"
#include "GMLib\GMQuadTree.h"
#include "GMLib\GMLine.h"
#include "GMLib\GM2DArc.h"
#include "GMLib\GM2DBound.h"

#include "MSElement.h"
#include "MSLinkMemberG.h"
#include "MSWallMemberG.h"
#include "MSColumnG.h"
#include "MSIsoFootG.h"
#include "MSBeamG.h"
#include "MSSlabG.h"
#include "MSGridLine.h"
#include "MSLinkMemPart.h"
#include "MSPolySection.h"
#include "MSFloor.h"
#include "MSFloorQT.h"
#include "MSJointMemberG.h"
#include "MSConnLevelZone.h"
#include "MSBuilding.h"
#include "MSLineLevelDifG.h"
#include "MSLevelZone.h"
#include "MSPolylineObject.h"
#include "CmdProcess\CmdNewModifyVector.h"
#include "CmdProcess\CmdNewModifyVariable.h"
#include "MSVersionInfo.h"

#include "Logger.h"
#include <math.h>

IMPLEMENT_SERIAL(MSConnectivity, MSObject, VERSIONABLE_SCHEMA | 1)
long MSConnectivity::ms_LastID = 0;
double MSConnectivity::ms_OverlapDistTol = 200.;

// NEW JOINT WORK START
map<MSConnectivity*, bool> MSConnectivity::_deleted_map;
MSConnectivity::MSConnectivity()
{
    m_ID = 0;
	m_nFloorNum = 0;
	m_dXc = 0.;
	m_dYc = 0.;

    mp_Source = NULL;

    mp_UpFloorJoint = NULL;
    mp_DownFloorJoint = NULL;
// 	mp_UpLinkJoint = NULL;
// 	mp_DownLinkJoint = NULL;
//	mp_UppperFloorHitVerJoit = NULL;

	mp_Column = NULL;
	mp_IsoFoot = NULL;
	mp_JointMember = NULL;
    mp_Floor = NULL;
    mp_DefaultSection = new GM2DPolyline(TRUE);

	m_bDeleted = false;
}

MSConnectivity::~MSConnectivity()
{
	DeleteLevelZone();
    if(mp_DefaultSection)
        delete mp_DefaultSection;
}

// m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
long MSConnectivity::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSConnectivity::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

MSConnectivity::operator GMVector()
{
	GMVector AVec(m_dXc, m_dYc, 0);
	return AVec;
}

MSConnectivity::operator GM2DVector()
{
	GM2DVector aVec(m_dXc, m_dYc);
	return aVec;
}

void MSConnectivity::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_nFloorNum;
		ar << m_dXc;
		ar << m_dYc;
		//mp_DefaultSection->Serialize(ar);
	//	ar << m_dZc;

		long ID = 0;
		int count = 0;

		ID = (mp_Source != NULL ? mp_Source->m_ID : 0);
		ar << ID;
		
		count = ma_Clone.size();
		ar << count;
		for (int i=0 ; i<count ; ++i)
		{
			ID = (ma_Clone[i] != NULL ? ma_Clone[i]->m_ID : 0);
			ar << ID;
		}
		ar << m_ID;

		ID = (mp_Floor != NULL ? mp_Floor->m_ID : 0);
		ar << ID;

		Serialize_AsID(ar, mp_UpFloorJoint);
		Serialize_AsID(ar, mp_DownFloorJoint);
		long NumUpLinkJoint = ma_UpLinkJoint.size();
		ar << NumUpLinkJoint;
		for(long iUpLinkJoint = 0 ; iUpLinkJoint < NumUpLinkJoint ; iUpLinkJoint++)
			Serialize_AsID(ar, ma_UpLinkJoint[iUpLinkJoint]);

		long NumDownLinkJoint = ma_DownLinkJoint.size();
		ar << NumDownLinkJoint;
		for(long iDownLinkJoint = 0 ;iDownLinkJoint < NumDownLinkJoint ; iDownLinkJoint++)
			Serialize_AsID(ar, ma_DownLinkJoint[iDownLinkJoint]);

		CTypedPtrArray<CObArray, MSFloorQT*>& temp = ma_CrossQT;

		count = ma_LevelZone.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i)
			ma_LevelZone[i]->Serialize(ar);
		count = ma_LevelZoneFloor.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i)
			ma_LevelZoneFloor[i]->Serialize(ar);

		count = ma_MemPart.size();
		ar << count;
		for (int i=0 ; i<count ; ++i)
		{
			long dummy = 0;
			MSLinkMemPart::Serialize_AsID(ar, ma_MemPart[i], dummy);
		}

		count = ma_MemPartDegree.size();
		ar << count;
		for (int i=0 ; i<count ; ++i) { double value = ma_MemPartDegree[i]; ar << value; }

		count = ma_SptMember.size();
		ar << count;
		for (int i=0 ; i<count ; ++i)
		{
			ID = (ma_SptMember[i] == NULL ? 0 : ma_SptMember[i]->m_ID);
			ar << ID;
		}

		count = ma_Degree.size();
		ar << count;
		for (int i=0 ; i<count ; ++i) { double value = ma_Degree[i]; ar << value; }

		count = ma_BuriedLinkMember.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i)
		{
			ID = (ma_BuriedLinkMember[i] == NULL ? 0 : ma_BuriedLinkMember[i]->m_ID);
			ar << ID;
		}

		ID = (mp_Column == NULL ? 0 : mp_Column->m_ID);
		ar << ID;
		ID = (mp_IsoFoot == NULL ? 0 : mp_IsoFoot->m_ID);
		ar << ID;
		ID = (mp_JointMember == NULL ? 0 : mp_JointMember->m_ID);
		ar << ID;

		count = ma_MidJointMember.size();
		ar << count;
		for (int i=0 ; i<count ; ++i)
		{
			ID = (ma_MidJointMember[i] == NULL ? 0 : ma_MidJointMember[i]->m_ID);
			ar << ID;
		}
		count = ma_SlabMember.size();
		ar << count;
		for (int i=0 ; i<count ; ++i)
		{
			ID = (ma_SlabMember[i] == NULL ? 0 : ma_SlabMember[i]->m_ID);
			ar << ID;
		}
		count = ma_StairMember.size();
		ar << count;
		for (int i=0 ; i<count ; ++i)
		{
			ID = (ma_StairMember[i] == NULL ? 0 : ma_StairMember[i]->m_ID);
			ar << ID;
		}
		GM2DPolyline::SerializeWithNullFlag(ar, mp_DefaultSection);
		ar << m_bDeleted;

		
	}
	else
	{
		ar >> m_nFloorNum;
		ar >> m_dXc;
		ar >> m_dYc;
		if (MSVersionInfo::GetCurrentVersion() >= 20141030) { } // skip
		else
		{
			mp_DefaultSection->Serialize(ar);
		}
	//	ar >> m_dZc;
		

		long ID = 0;
		int count = 0;
		if (MSVersionInfo::GetCurrentVersion() >= 20141003)
		{
			ar >> ID;
			mp_Source = (MSConnectivity*)ID;
			ar >> count;
			for (int i=0 ; i<count ; ++i)
			{
				ar >> ID;
				ma_Clone.push_back((MSConnectivity*)ID);
			}
			ar >> m_ID;

			ar >> ID;
			mp_Floor = (MSFloor*)ID;

			Serialize_AsID(ar, mp_UpFloorJoint);
			Serialize_AsID(ar, mp_DownFloorJoint);
			
			long NumUpLinkJoint = 0;
			ar >> NumUpLinkJoint;
			for(long iUpLinkJoint = 0 ; iUpLinkJoint < NumUpLinkJoint ; iUpLinkJoint++)
			{
				long nUpLinkJointID = 0;
				ar >> nUpLinkJointID;
				ma_UpLinkJoint_Idx.push_back(nUpLinkJointID);
			}

			long NumDownLinkJoint = 0;
			ar >> NumDownLinkJoint;
			for(long iDownLinkJoint = 0 ; iDownLinkJoint < NumDownLinkJoint ; iDownLinkJoint++)
			{
				long nDownLinkJointID = 0;
				ar >> nDownLinkJointID;
				ma_DownLinkJoint_Idx.push_back(nDownLinkJointID);
			}

			CTypedPtrArray<CObArray, MSFloorQT*>& temp = ma_CrossQT;

			ar >> count;
			ma_LevelZone.RemoveAll();
			for (int i=0 ; i<count ; ++i)
			{
				MSConnLevelZone* cur = new MSConnLevelZone(NULL, 0, 0);
				cur->Serialize(ar);
				ma_LevelZone.Add(cur);
			}
			ar >> count;
			ma_LevelZoneFloor.RemoveAll();
			for (int i=0 ; i<count ; ++i)
			{
				MSConnLevelZone* cur = new MSConnLevelZone(NULL, 0, 0);
				cur->Serialize(ar);
				ma_LevelZoneFloor.Add(cur);
			}

			ar >> count;
			ma_MemPart.clear();
			ma_MemPart_Idx.clear();
			for (int i=0 ; i<count ; ++i)
			{
				long idx = 0;
				MSLinkMemPart* part = NULL;
				MSLinkMemPart::Serialize_AsID(ar, part, idx); // NEED TO CONVERT ID->POINTER
				ma_MemPart.push_back(part);
				ma_MemPart_Idx.push_back(idx);
			}

			ar >> count;
			ma_MemPartDegree.clear();
			for (int i=0 ; i<count ; ++i) { double value = 0.0; ar >> value; ma_MemPartDegree.push_back(value); }

			ar >> count;
			ma_SptMember.clear();
			for (int i=0 ; i<count ; ++i)
			{
				ar >> ID;
				ma_SptMember.push_back((MSLinkMemberG*)ID);
			}

			ar >> count;
			ma_Degree.clear();
			for (int i=0 ; i<count ; ++i) { double value = 0.0; ar >> value; ma_Degree.push_back(value); }

			ar >> count;
			ma_BuriedLinkMember.RemoveAll();
			for (int i=0 ; i<count ; ++i)
			{
				ar >> ID;
				ma_BuriedLinkMember.Add((MSLinkMemberG*)ID);
			}

			ar >> ID;
			mp_Column = (MSColumnG*)ID;
			ar >> ID;
			mp_IsoFoot = (MSIsoFootG*)ID;
			ar >> ID;
			mp_JointMember = (MSJointMemberG*)ID;

			ar >> count;
			ma_MidJointMember.clear();
			for (int i=0 ; i<count ; ++i)
			{
				ar >> ID;
				ma_MidJointMember.push_back((MSLinkMemberG*)ID);
			}
			ar >> count;
			ma_SlabMember.clear();
			for (int i=0 ; i<count ; ++i)
			{
				ar >> ID;
				ma_SlabMember.push_back((MSSlabG*)ID);
			}
			ar >> count;
			ma_StairMember.clear();
			for (int i=0 ; i<count ; ++i)
			{
				ar >> ID;
				ma_StairMember.push_back((MSGenStairG*)ID);
			}

			GM2DPolyline::SerializeWithNullFlag(ar, mp_DefaultSection);

			ar >> m_bDeleted;
			m_bDeleted = false;

			if (!MSConnectivity::JOINT_SERIALIZE())
			{
				mp_Source = NULL;
				ma_Clone.clear();
				mp_Floor = NULL;

				ma_MemPart.clear();
				ma_MemPart_Idx.clear();
				ma_MemPartDegree.clear();
				ma_SptMember.clear();
				ma_Degree.clear();
				ma_BuriedLinkMember.RemoveAll();

				mp_Column = NULL;
				mp_IsoFoot = NULL;
				mp_JointMember = NULL;

				ma_MidJointMember.clear();
				ma_SlabMember.clear();
				ma_StairMember.clear();
			}

		}

		SetLastID(m_ID);
	}
}

void MSConnectivity::RecoverPointerAfterSerialize(MSFloor* floor)
{
	long ID = 0;
	long idx = 0;

	ID = (long)mp_Source;
	mp_Source = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);

	for (int i=0 ; i<ma_Clone.size() ; ++i)
	{
		ID = (long)ma_Clone[i];
		ma_Clone[i] = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);
	}

	ID = (long)mp_Floor;
	mp_Floor = floor;
	//mp_Floor = (floor->GetOwner()->mm_Floor.count(ID) == 0 ? NULL : floor->GetOwner()->mm_Floor[ID]);

	ID = (long)mp_UpFloorJoint; mp_UpFloorJoint = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);
	ID = (long)mp_DownFloorJoint; mp_DownFloorJoint = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);
	
	long NumUpLinkJoint = ma_UpLinkJoint_Idx.size();
	for(long iUpIdx = 0 ; iUpIdx < NumUpLinkJoint ; iUpIdx++)
	{
		ID = ma_UpLinkJoint_Idx[iUpIdx];
		MSConnectivity* pUpLinkJoint = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);
		if(pUpLinkJoint != NULL)
			ma_UpLinkJoint.push_back(pUpLinkJoint);
	}
	long NumDownLinkJoint = ma_DownLinkJoint_Idx.size();
	for(long iDownIdx = 0 ; iDownIdx < NumDownLinkJoint ; iDownIdx++)
	{
		ID = ma_DownLinkJoint_Idx[iDownIdx];
		MSConnectivity* pDownLinkJoint = (floor->mm_Joint.count(ID) == 0 ? NULL : floor->mm_Joint[ID]);
		if(pDownLinkJoint != NULL)
			ma_DownLinkJoint.push_back(pDownLinkJoint);
	}
	
	for (int i=0 ; i<ma_LevelZone.GetSize() ; ++i)
		ma_LevelZone[i]->RecoverPointerAfterSerialize(floor);
	for (int i=0 ; i<ma_LevelZoneFloor.GetSize() ; ++i)
		ma_LevelZoneFloor[i]->RecoverPointerAfterSerialize(floor);

	for (int i=0 ; i<ma_MemPart.size() ; ++i)
	{
		ID = (long)ma_MemPart[i];
		idx = ma_MemPart_Idx[i];
		ma_MemPart[i] = NULL;
		if (floor->mm_CompoG.count(ID) > 0)
		{
			MSCompoG* compoG = (floor->mm_CompoG[ID]);
			MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(compoG);
// 			if (link != NULL && idx < link->ma_MemPart.size())
// 				ma_MemPart[i] = link->ma_MemPart[idx];
		}
		if (ma_MemPart[i] == NULL) ASSERT(0);
	}
	ma_MemPart_Idx.clear();

	for (int i=0 ; i<ma_SptMember.size() ; ++i)
	{
		ID = (long)ma_SptMember[i];
		ma_SptMember[i] = (MSLinkMemberG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);
	}
	for (int i=0 ; i<ma_BuriedLinkMember.GetSize() ; ++i)
	{
		ID = (long)ma_BuriedLinkMember[i];
		ma_BuriedLinkMember[i] = (MSLinkMemberG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);
	}

	ID = (long)mp_Column;
	mp_Column = (MSColumnG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);
	ID = (long)mp_IsoFoot;
	mp_IsoFoot = (MSIsoFootG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);
	ID = (long)mp_JointMember;
	mp_JointMember = (MSJointMemberG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);

	for (int i=0 ; i<ma_MidJointMember.size() ; ++i)
	{
		ID = (long)ma_MidJointMember[i];
		ma_MidJointMember[i] = (MSLinkMemberG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);
	}

	for (int i=0 ; i<ma_SlabMember.size() ; ++i)
	{
		ID = (long)ma_SlabMember[i];
		ma_SlabMember[i] = (MSSlabG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);
	}

	for (int i=0 ; i<ma_StairMember.size() ; ++i)
	{
		ID = (long)ma_StairMember[i];
		ma_StairMember[i] = (MSGenStairG*)(floor->mm_CompoG.count(ID) == 0 ? NULL : floor->mm_CompoG[ID]);
	}

	//floor->mp_QT->AddToLeaves(this, true, true);
}

void MSConnectivity::Serialize_AsID(CArchive& ar, MSConnectivity*& con)
{
	long ID = 0;
	if(ar.IsStoring())
	{
		ID = (con == NULL ? 0 : con->m_ID);
		ar << ID;
	}
	else
	{
		ar >> ID;
		con = (MSConnectivity*)ID;
	}
}

MSObject* MSConnectivity::Copy( bool bNewID /*= true*/ )
{
	MSConnectivity* pNewObj = new MSConnectivity();
    if(bNewID)
        pNewObj->GetID();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}


void MSConnectivity::CopyFromMe(MSConnectivity* pSource, bool bNewID)
{
	if(pSource==NULL)
		return;

	MSObject::CopyFromMe(pSource, bNewID);

	m_dXc = pSource->m_dXc;
	m_dYc = pSource->m_dYc;
	m_nFloorNum = pSource->m_nFloorNum;
    mp_Floor = pSource->mp_Floor;

	if(bNewID)
		GetID();
}

void MSConnectivity::Delete()
{
	m_bDeleted = true;
	DeleteLevelZone();
	_deleted_map[this] = true;
}

bool MSConnectivity::IsDeleted()
{
	return m_bDeleted;
}

MSLinkMemPart* MSConnectivity::FindMemPart(MSConnectivity* pOtherJoint)
{
	if(pOtherJoint == this) return NULL;
	vector<MSLinkMemPart*> MemParts = ma_MemPart;

	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
		MemParts.reserve(MemParts.size() + pJoint->ma_MemPart.size());
		MemParts.insert(MemParts.end(), pJoint->ma_MemPart.begin(), pJoint->ma_MemPart.end());
	}
// 	if(mp_UpLinkJoint != NULL)
// 	{
// 		MemParts.reserve(MemParts.size() + mp_UpLinkJoint->ma_MemPart.size());
// 		MemParts.insert(MemParts.end(), mp_UpLinkJoint->ma_MemPart.begin(),mp_UpLinkJoint->ma_MemPart.end());
// 	}

	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
		MemParts.reserve(MemParts.size() + pJoint->ma_MemPart.size());
		MemParts.insert(MemParts.end(), pJoint->ma_MemPart.begin(), pJoint->ma_MemPart.end());
	}
// 	if(mp_DownLinkJoint != NULL)
// 	{
// 		MemParts.reserve(MemParts.size() + mp_DownLinkJoint->ma_MemPart.size());
// 		MemParts.insert(MemParts.end(), mp_DownLinkJoint->ma_MemPart.begin(),mp_DownLinkJoint->ma_MemPart.end());
// 	}


	long nPart = MemParts.size();
	for(long iPart=0; iPart<nPart; iPart++)
	{
		MSLinkMemPart* pUrPart = MemParts[iPart];
		if(pUrPart->isBuriedLinkMember()) continue;

		if(pUrPart->mp_SConn->IsConnected(pOtherJoint) || pUrPart->mp_EConn->IsConnected(pOtherJoint))
			return pUrPart;
	}
	return NULL;
}

MSLinkMemPart* MSConnectivity::FindMemPart(double MyDegree)
{
	vector<MSLinkMemPart*> MemParts = ma_MemPart;
	vector<double> MemPartDegrees = ma_MemPartDegree;
	
	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
		MemParts.reserve(MemParts.size() + pJoint->ma_MemPart.size());
		MemParts.insert(MemParts.end(), pJoint->ma_MemPart.begin(), pJoint->ma_MemPart.end());

		MemPartDegrees.reserve(MemPartDegrees.size() + pJoint->ma_MemPartDegree.size());
		MemPartDegrees.insert(MemPartDegrees.end(), pJoint->ma_MemPartDegree.begin(),pJoint->ma_MemPartDegree.end());
	}

// 	if(mp_UpLinkJoint != NULL)
// 	{
// 		MemParts.reserve(MemParts.size() + mp_UpLinkJoint->ma_MemPart.size());
// 		MemParts.insert(MemParts.end(), mp_UpLinkJoint->ma_MemPart.begin(),mp_UpLinkJoint->ma_MemPart.end());
// 		
// 		MemPartDegrees.reserve(MemPartDegrees.size() + mp_UpLinkJoint->ma_MemPartDegree.size());
// 		MemPartDegrees.insert(MemPartDegrees.end(), mp_UpLinkJoint->ma_MemPartDegree.begin(),mp_UpLinkJoint->ma_MemPartDegree.end());
// 	}

	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
		MemParts.reserve(MemParts.size() + pJoint->ma_MemPart.size());
		MemParts.insert(MemParts.end(), pJoint->ma_MemPart.begin(), pJoint->ma_MemPart.end());

		MemPartDegrees.reserve(MemPartDegrees.size() + pJoint->ma_MemPartDegree.size());
		MemPartDegrees.insert(MemPartDegrees.end(), pJoint->ma_MemPartDegree.begin(),pJoint->ma_MemPartDegree.end());
	}

// 	if(mp_DownLinkJoint != NULL)
// 	{
// 		MemParts.reserve(MemParts.size() + mp_DownLinkJoint->ma_MemPart.size());
// 		MemParts.insert(MemParts.end(), mp_DownLinkJoint->ma_MemPart.begin(),mp_DownLinkJoint->ma_MemPart.end());
// 
// 		MemPartDegrees.reserve(MemPartDegrees.size() + mp_DownLinkJoint->ma_MemPartDegree.size());
// 		MemPartDegrees.insert(MemPartDegrees.end(), mp_DownLinkJoint->ma_MemPartDegree.begin(),mp_DownLinkJoint->ma_MemPartDegree.end());
// 	}


	long nPart = MemParts.size();
	for(long iPart=0; iPart<nPart; iPart++)
	{
		MSLinkMemPart* pUrPart = MemParts[iPart];
		if(pUrPart->isBuriedLinkMember()) continue;

		double UrAngle = MemPartDegrees[iPart];
// 뒤의 조건은 0도 멤파트가 수치오류에 의하여 359.99999로 되어 있는 경우 처리
		if(fabs(MyDegree-UrAngle) < MSFloor::ms_ColinearDegreeTol*2. || fabs(fabs(MyDegree-UrAngle)-360.) < MSFloor::ms_ColinearDegreeTol*2.)
        {
            if(pUrPart->mp_FloorPartUp)  // pUrPart가 벽체이고 상단에 보가 있는 경우 보를 리턴. pUrPart가 보일 경우 상단파트가 항상 NULL임
                return pUrPart->mp_FloorPartUp;
			return pUrPart;
        }
	}
	return NULL;
}

long MSConnectivity::AddPartMember(long StartOrEnd, MSLinkMemPart* pLinkPart, bool bIsMidJoint)
{
	long NumMem = ma_MemPart.size();
	if(pLinkPart == NULL)
		return NumMem;

// 트림을 위하여 부재를 조인트에 연결되는 각도별로 저장하여야 한다.
// 그런데 동일 방향, 동일 레벨에 동일 부재가 연결될 때 -1을 리턴해야 하므로 실제 기능은 방향/레벨 테스트 후 시작된다.
// 그러므로 곡선 부재일 때는 this점으로 LLine, RLine 끝점을 변경해야 GetDegree 함수로 각도를 알 수 있는데,
// 원본이 변경되지 않도록 tempPart를 만들어서 각도를 구한다.
    double MyAngle;
	
	if(pLinkPart->mp_LinkMem->mp_Curve==NULL)
		MyAngle = pLinkPart->GetDegree(StartOrEnd); 
	else
	{
		MSLinkMemPart  tempPart;
		tempPart.CopyFromMe(pLinkPart);
		GM2DVector Pnt(m_dXc, m_dYc);

		if(StartOrEnd==0)
		{
			tempPart.mp_LLine->SetP0(Pnt);
			tempPart.mp_RLine->SetP0(Pnt);
		}
		else
		{
			tempPart.mp_LLine->SetP1(Pnt);
			tempPart.mp_RLine->SetP1(Pnt);
		}
		MyAngle =  tempPart.GetDegree(StartOrEnd); 
	}

    for(long MemNum=0; MemNum<NumMem; MemNum++)
    {
        MSLinkMemPart* UrMem = ma_MemPart[MemNum];
        if(pLinkPart == UrMem)
            return ma_MemPart.size()-1;

        double UrAngle = ma_MemPartDegree[MemNum];

        double InnerDegree = fabs(MyAngle-UrAngle);
        //if(InnerDegree<MSFloor::ms_ColinearDegreeTol && UrMem->mp_LinkMem->mp_Curve==NULL && pLinkPart->mp_LinkMem->mp_Curve==NULL)
		if( InnerDegree <= MSFloor::ms_ColinearDegreeTol   
			&& UrMem->mp_LinkMem->isArc() && pLinkPart->mp_LinkMem->isArc() )
        {
            // -1 : 보-벽 조합으로 레벨 겹침.
            // 0 : 레벨 불일치
            // 1 : 한 부재 하단에 한 부재 상단이 접함
            // 2 : 상호 어긋나게 교차
            // 3 : my가 ur에 포함됨
            // 4 : my가 ur를 포함함
            // 5 : 완전 일치
            long LevelStatus = mp_Floor->LevelOverlap(pLinkPart->mp_LinkMem, UrMem->mp_LinkMem);
            // LevelOverlap이 -1을 리턴하는 것은 두 부재가 플로어(수평)와 벽체(수직)로 구성되어 있고 레벨이 겹친다는 의미이므로 서로 연결해 줌
            // 이후 트림할 때 두 부재 사이에서는 트림하지 않고 스킾
            if(LevelStatus==-1)
            {
                // 내가 수평이므로 너는 벽체
                if(pLinkPart->mp_LinkMem->IsLineSupportMember())
                {
                    pLinkPart->mp_WallPartBelow = UrMem;
                    UrMem->mp_FloorPartUp = pLinkPart;
					//ASSERT(0);
					break;
                }
                // 내가 벽체이므로 너는 수직
                if(pLinkPart->mp_LinkMem->IsWallMember())
                {
                    pLinkPart->mp_FloorPartUp = UrMem;
                    UrMem->mp_WallPartBelow = pLinkPart;
					//ASSERT(0);
					break;
                }
            }
            // 같은 유형의 부재가 겹치는 상황이므로 입력되지 않음
            if(LevelStatus>1)
			{

				//ASSERT(0);
				//pLinkPart->mp_LinkMem->AddErrorObject(ADD_MEMBER_ERROR_INS_SAME_LOC, MESSAGE_LEVEL_ERROR, ADD_MEMBER_GROUP, ADD_MEMBER_ERROR_INS_SAME_LOC_MSG);

				GM2DCurve* pUrFirstLineTo0Degree = UrMem->mp_SConn == this ? UrMem->mp_RLine : UrMem->mp_LLine;
				GM2DVector& UrSecondEnd = UrMem->mp_SConn == this ? UrMem->mp_LLine->m_P0 : UrMem->mp_RLine->m_P0;
				GM2DLineVector MyLVec;
				GM2DVector MyEnd;
				if(pLinkPart->mp_LinkMem->mp_Curve)
					MyEnd = pLinkPart->mp_LinkMem->mp_Curve->OrthoBaseVector(*this);
				else
				{
					GM2DLineVector MyLVec = pLinkPart->mp_LinkMem->GetCenLVec();
					MyEnd = MyLVec.OrthoBaseVector(*this);
				}
				if(pUrFirstLineTo0Degree->WhichSide(UrSecondEnd) * pUrFirstLineTo0Degree->WhichSide(MyEnd) == 1)
				{
					ma_MemPart.insert(ma_MemPart.begin()+MemNum+1, pLinkPart);
					ma_MemPartDegree.insert(ma_MemPartDegree.begin()+MemNum+1, MyAngle);
					break;
				}
				else 
				{
					ma_MemPart.insert(ma_MemPart.begin()+MemNum, pLinkPart);
					ma_MemPartDegree.insert(ma_MemPartDegree.begin()+MemNum, MyAngle);
					break;
				}

//                return -1;
			}
            // LevelStatus==0 인 경우는 두 부재 사이 레벨이 불일치하므로 무조건 입력
        }

        if(MyAngle<(UrAngle+DTOL_GM))
        {
            ma_MemPart.insert(ma_MemPart.begin()+MemNum, pLinkPart);
            ma_MemPartDegree.insert(ma_MemPartDegree.begin()+MemNum, MyAngle);
            break;
        }
    }
    if(MemNum==NumMem)
    {
        ma_MemPart.push_back(pLinkPart);
        ma_MemPartDegree.push_back(MyAngle);
    }

	if(StartOrEnd == 0)
	{
		pLinkPart->mp_SConn = this;
		if(bIsMidJoint == true)
        {
			pLinkPart->SetLinesOfAMemPart(true, false);
            pLinkPart->ma_SSegments.clear();
            pLinkPart->mp_SPolyLine->ResetPolyline();
        }
	}
	else if(StartOrEnd == 1)
	{
		pLinkPart->mp_EConn = this;
		if(bIsMidJoint == true)
        {
			pLinkPart->SetLinesOfAMemPart(false, true);
            pLinkPart->ma_ESegments.clear();
            pLinkPart->mp_EPolyLine->ResetPolyline();
        }
	}

	return ma_MemPart.size()-1;
}


void MSConnectivity::RemoveMemPart( MSLinkMemPart* pRemoved )
{
	long NumMem = ma_MemPart.size();

	for(long MemNum=0; MemNum<NumMem; MemNum++)
	{
		MSLinkMemPart* UrMem = ma_MemPart[MemNum];
		if(UrMem == pRemoved)
		{
			ma_MemPart.erase(ma_MemPart.begin()+MemNum);
			ma_MemPartDegree.erase(ma_MemPartDegree.begin()+MemNum);
            if(pRemoved->mp_FloorPartUp)
                pRemoved->mp_FloorPartUp->mp_WallPartBelow = NULL;
            if(pRemoved->mp_WallPartBelow)
                pRemoved->mp_WallPartBelow->mp_FloorPartUp = NULL;
			break;
		}
	}
}


//by LSS
long MSConnectivity::AddSptMember(long StartOrEnd, MSLinkMemberG* pLink)
{
//     if(StartOrEnd == 0)
//         AddPartMember(StartOrEnd,pLink->ma_MemPart.front(), false);
//     else if(StartOrEnd == 1)
//         AddPartMember(StartOrEnd,pLink->ma_MemPart.back(), false);

	long NumMem = ma_SptMember.size();
	double MyAngle = pLink->GetDegree(StartOrEnd); 

	for(long MemNum=0; MemNum<NumMem; MemNum++)
	{
		MSLinkMemberG* UrMem = ma_SptMember[MemNum];
		if(UrMem == pLink)
			return -MemNum;
		double UrAngle = ma_Degree[MemNum];
		
		if(MyAngle<UrAngle)
		{
				ma_SptMember.insert(ma_SptMember.begin()+MemNum, pLink);
				ma_Degree.insert(ma_Degree.begin()+MemNum, MyAngle);
						
			return MemNum;
		}
	}
	
	ma_SptMember.push_back(pLink);
	ma_Degree.push_back(MyAngle);

	return ma_SptMember.size()-1;
}

void MSConnectivity::RemoveSptMembers()
{
	ma_SptMember.clear();
	ma_Degree.clear();
	mp_Column = NULL;
	ma_SlabMember.clear();
	ma_StairMember.clear();
}

long MSConnectivity::MergeToMeAndDelete( MSAssembly* pOwner, MSConnectivity* DeletedJoint, bool bDoTrim )
{
	if(this == DeletedJoint)
		return 1;
	MSPointMemberG *pMyPointMem = NULL;
	if(mp_Column)
		pMyPointMem = mp_Column;
	if(mp_IsoFoot)
		pMyPointMem = mp_IsoFoot;
	
	MSPointMemberG *pUrPointMem = NULL;
	if(DeletedJoint->mp_Column)
		pUrPointMem = DeletedJoint->mp_Column;
	if(DeletedJoint->mp_IsoFoot)
		pUrPointMem = DeletedJoint->mp_IsoFoot;
	
	// My,Ur에 포인트 부재가 있을 경우 머지 시키면 안된다.
	if(pMyPointMem != NULL && pUrPointMem != NULL)
		return 1;

	if (HasNoFloorMemPart() && DeletedJoint->HasNoVerticalMemPart())
		return 1;
	if(HasNoVerticalMemPart() && DeletedJoint->HasNoFloorMemPart())
		return 1;
	
//	long Result = 0;

// 	if(DeletedJoint->mp_DownLinkJoint != NULL && mp_DownLinkJoint != NULL)
// 	{
// 		Result = DeletedJoint->mp_DownLinkJoint->MergeTo(mp_DownLinkJoint);
// 		if(Result == 0)
// 		{	pOwner->Remove(DeletedJoint->mp_DownLinkJoint);
// 			DeletedJoint->mp_DownLinkJoint = NULL;
// 		}
// 	}
// 
// 	if(DeletedJoint->mp_UpLinkJoint != NULL && mp_UpLinkJoint != NULL)
// 	{
// 		Result = DeletedJoint->mp_UpLinkJoint->MergeTo(mp_UpLinkJoint);
// 		if(Result == 0)
// 		{	
// 			pOwner->Remove(DeletedJoint->mp_UpLinkJoint);
// 			DeletedJoint->mp_UpLinkJoint = NULL;
// 		}
// 	}

    long Result = DeletedJoint->MergeTo(this, bDoTrim);
	if(Result==1) // 같은 조인트
	  return 1;
	//pOwner->Remove(DeletedJoint);

    return Result;
}

long MSConnectivity::MergeTo( MSConnectivity* Target, bool bDoTrim)
{
	if( this== Target )
		return 1;
	/*
	Target->DeleteLevelZone();
	DeleteLevelZone();

    long nMem = ma_SptMember.size();
	
	// Buried...
// 	if(MSFloor::ms_TrimType == 0)
// 	{
// 		for(long iMem=nMem-1; iMem>=0; iMem--)
// 		{
// 			MSLinkMemberG* pMem = ma_SptMember[iMem];
// 			if((pMem->GetSConn() == this && pMem->GetEConn()==Target) ||
// 				(pMem->GetEConn() == this && pMem->GetSConn()==Target))
// 			{
// 				//ASSERT(0);
// 				RemoveSptMember(pMem);
// 				Target->RemoveSptMember(pMem);
// 				long nMidJoint = pMem->ma_MidJoint.size();
// 				for(long iMidJoint=0; iMidJoint<nMidJoint; iMidJoint++)
// 				{
// 					if(pMem->ma_MidJoint.size() >iMidJoint)
// 						pMem->RemoveMidJoint(pMem->ma_MidJoint[iMidJoint]);
// 					else
// 						ASSERT(0);
// 				}
// 				pMem->ma_Joint[0] = Target;
// 				pMem->ma_Joint[1] = Target;
// 				pMem->ma_MemPart[0]->mp_SConn = Target;
// 				pMem->ma_MemPart[0]->mp_EConn = Target;
// 				Target->ma_BuriedLinkMember.Add(pMem);
// 				pMem->SetBuriedLinkMember();
// 				pMem->SetLines_MemPart();
// 			}
// 		}
// 	}
// 	

	// 동일한  MemPart가 존재할시 삭제 해준다.
	CheckMemPart();

	// 삭제될 미드노드를 라인부재에서 삭제한후 MemPart를 다시 생성한다.
// 	long idx = 0;
// 	long CheckCnt = 0;
// 	while(idx != ma_MemPart.size())
// 	{
// 		if(ma_MemPart.size() == 0)
// 			break;
// 
// 		MSLinkMemPart* pPart = ma_MemPart[idx];
// 		MSLinkMemberG* pLinkMem = ma_MemPart[idx]->mp_LinkMem;
// 
// 		Target->RemoveMemPart(pPart);
// 		if(pLinkMem->RemoveMidJoint(this))
// 		{
// 			idx = 0;
// 			continue;
// 		}
// 		idx++;
// 	}

	long numPart = ma_MemPart.size();
	for(long iPart = numPart-1; iPart >= 0; iPart--)
	{
		//if(ma_MemPart.size() <= iPart) continue;
        MSLinkMemberG* pLinkMem = ma_MemPart[iPart]->mp_LinkMem;
        MSConnectivity* pSConn = ma_MemPart[iPart]->GetSConn();
		MSConnectivity* pEConn = ma_MemPart[iPart]->GetEConn();

		//SConn과 EConn이 Merge될 두 joint에 연결된 경우
		//MemPart를 삭제하고 MemPart를 소유하고있는 LinkMember에서 MemPart와 MidJoint를 빼줘야한다.
		if(pEConn == Target)// || ma_MemPart[iPart]->GetSConn() == Target)
		{
// 			Target->RemoveMemPart(ma_MemPart[iPart]);//Target에서도 Mempart가 있을수있을수 있어 먼저 빼준다.

			// pSConn 와 pEConn 이 this, Target 인  MSLinkMemPart 가 존재할경우 Target에서는 삭제해 주면 안된다..
   			if(pLinkMem->RemoveMidJoint(this)==true)
            {
                iPart--;  // 링크멤에서 조인트 리무브에 성공하면 디스 조인트 앞뒤에서 두개의 멤파트가 제거되므로 
				continue;
            }
            if(Target == pLinkMem->GetEConn())
                continue;

            Target->RemoveMemPart(ma_MemPart[iPart]);//Target에서도 Mempart가 있을수있을수 있어 먼저 빼준다.
            //kkw test
			if(pLinkMem->ma_MemPart.size() > 0)
				pLinkMem->ma_MemPart.erase(pLinkMem->ma_MemPart.begin());
            //delete ma_MemPart[iPart];
			ma_MemPart[iPart]->Delete();
            if(pLinkMem->ma_MidJoint.size() > 0)
				pLinkMem->ma_MidJoint.erase(pLinkMem->ma_MidJoint.begin());
			continue;
		}
        else if(pSConn == Target)
        {
            if(pLinkMem->RemoveMidJoint(this)==true)
            {
                iPart--;  // 링크멤에서 조인트 리무브에 성공하면 디스 조인트 앞뒤에서 두개의 멤파트가 제거되므로 
                continue;
            }
            if(Target == pLinkMem->GetSConn())
                continue;

            Target->RemoveMemPart(ma_MemPart[iPart]);//Target에서도 Mempart가 있을수있을수 있어 먼저 빼준다.
            
			if(pLinkMem->ma_MemPart.size() > 0)
				pLinkMem->ma_MemPart.erase(pLinkMem->ma_MemPart.end()-1);
            //delete ma_MemPart[iPart];
			ma_MemPart[iPart]->Delete();
            if(pLinkMem->ma_MidJoint.size() > 0)
				pLinkMem->ma_MidJoint.erase(pLinkMem->ma_MidJoint.end()-1);

			continue;
        }
    }

	// 삭제될 조인트에 있는 MemPart를 합쳐질 조인트에 Add 시킨다., 위 로직과 합쳐지면 안됨( 시작,끝 조인트의 관계가 꼬이는경우 발생함 )
	numPart = ma_MemPart.size();
	for(long iPart = numPart-1; iPart >= 0; iPart--)
	{
		if(ma_MemPart[iPart]->isDelete())
			continue;
		MSLinkMemberG* pLinkMem = ma_MemPart[iPart]->mp_LinkMem;
		MSConnectivity* pSConn = ma_MemPart[iPart]->GetSConn();
		MSConnectivity* pEConn = ma_MemPart[iPart]->GetEConn();

		bool bHasInfilledMemPart=false;
		long nOwnerPart = pLinkMem->ma_MemPart.size();
		for(long iOwnerPart=0; iOwnerPart<nOwnerPart; iOwnerPart++)
		{
			if(pLinkMem->ma_MemPart[iOwnerPart]->GetEConn() == Target || pLinkMem->ma_MemPart[iOwnerPart]->GetSConn() == Target)
				bHasInfilledMemPart = true;                
		}

		if(bHasInfilledMemPart==false)
			ma_MemPart[iPart]->mp_LinkMem->ReplaceMidJoint(this, Target);
		if(pSConn == this)
			Target->AddPartMember(0,ma_MemPart[iPart], pSConn!=pLinkMem->GetSConn());
		else if(pEConn == this)
			Target->AddPartMember(1,ma_MemPart[iPart], pEConn!=pLinkMem->GetEConn());
		else
			ASSERT(0);
	}

	ma_MemPart.clear();		
	
	nMem = ma_SptMember.size();
	for(long iMem=nMem-1; iMem>=0; iMem--)
	{
		MSLinkMemberG* pMem = ma_SptMember[iMem];
		if(pMem->GetSConn() == this)
			pMem->SetJoint(0,Target);
		else if(pMem->GetEConn() == this)
			pMem->SetJoint(1,Target);
		else//Mid에 연결된 경우 
			ASSERT(0);
		//Target->AddSptMember(pMem);
	}
	if(mp_Column)
		mp_Column->SetJoint(0,Target);
		//Target->AddSptMember(mp_Column);
	if(mp_IsoFoot)
		mp_IsoFoot->SetJoint(0,Target);
		//Target->AddSptMember(mp_IsoFoot);
	if(mp_JointMember)
		mp_JointMember->SetJoint(0,Target);


	ma_SptMember.clear();
	mp_Column = NULL;
	mp_IsoFoot = NULL;
	mp_JointMember = NULL;

	Target->CheckMemPart();

	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;
		Target->AddUpLinkJoint(pJoint);
		pJoint->AddDownLinkJoint(Target);
	}

	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;
		Target->AddDownLinkJoint(pJoint);
		pJoint->AddUpLinkJoint(Target);
	}

// 
// 	Target->AddUpLinkJoint(mp_UpLinkJoint);
// 	if(mp_UpLinkJoint != NULL)
// 		mp_UpLinkJoint->AddDownLinkJoint(Target);
// 	Target->AddDownLinkJoint(mp_DownLinkJoint);
// 	if(mp_DownLinkJoint != NULL)
// 		mp_DownLinkJoint->AddUpLinkJoint(Target);

	ma_UpLinkJoint.clear();
	ma_DownLinkJoint.clear();

// 	mp_UpLinkJoint = NULL;
// 	mp_DownLinkJoint = NULL;

// 		//MSLinkMemPart의 SConn, EConn 이 같으면 삭제 해준다.
// 	long NumMemPart = Target->ma_MemPart.size();
// 	for(long iPart = NumMemPart -1 ; iPart >= 0 ; iPart--)
// 	{
// 		MSLinkMemPart* pPart = Target->ma_MemPart[iPart];
// 		if(pPart == NULL) continue;
// 		MSLinkMemberG* pLinkMem = pPart->mp_LinkMem;
// 		if(pPart->mp_SConn == pPart->mp_EConn)
// 		{
// 			long Num = pLinkMem->ma_MemPart.size();
// 			if(Num > 1 && !pLinkMem->m_isBuriedLinkMember)
// 			{
// 				for(long i = Num - 1 ; i >= 0 ; i--)
// 				{
// 					if(pLinkMem->ma_MemPart[i] != pPart)
// 						continue;
// 					delete pLinkMem->ma_MemPart[i];
// 					pLinkMem->ma_MemPart.erase(pLinkMem->ma_MemPart.begin() + i);
// 				}
// 				Target->ma_MemPart.erase(Target->ma_MemPart.begin() + iPart);
// 			}
// 		}
// 	}
// 	

    if(bDoTrim)
        Target->TrimMembersEx(true);
	*/
	return 0;
}
 
void MSConnectivity::RemoveSptMember( MSCompoG* pRemove )
{
	long nMem = ma_SptMember.size();
	for(long iMem=nMem-1; iMem>=0; iMem--)
	{
		MSLinkMemberG* pMem = ma_SptMember[iMem];
		if(pMem==pRemove)
		{
			if(IS_USE_NATIVE_CMD)
			{
				CmdNewModifyVector<MSLinkMemberG*>::Erase(&ma_SptMember, iMem);
				CmdNewModifyVector<double>::Erase(&ma_Degree, iMem);
			}
			else
			{
				ma_SptMember.erase(ma_SptMember.begin()+iMem);
				ma_Degree.erase(ma_Degree.begin()+iMem);
			}
		}
	}
	long nMemPart = ma_MemPart.size();
	for(long iMem=nMemPart-1; iMem>=0; iMem--)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[iMem];
		if(pMemPart->mp_LinkMem==pRemove)
		{
			if(IS_USE_NATIVE_CMD)
			{
				CmdNewModifyVector<MSLinkMemPart*>::Erase(&ma_MemPart, iMem);
				CmdNewModifyVector<double>::Erase(&ma_MemPartDegree, iMem);
			}
			else
			{
				ma_MemPart.erase(ma_MemPart.begin()+iMem);
				ma_MemPartDegree.erase(ma_MemPartDegree.begin()+iMem);
			}
		}
	}

	if(mp_Column == pRemove)
    {
		if(IS_USE_NATIVE_CMD)
			CmdNewModifyVariable<MSColumnG*>::Set(&mp_Column, NULL);
		else
			mp_Column = NULL;
    }
	if(mp_IsoFoot == pRemove)
    {
        mp_IsoFoot = NULL;
    }
	if(mp_JointMember == pRemove)
		mp_JointMember = NULL;
}

BOOL MSConnectivity::VisibleInRect( GM2DRectangle* pRect )
{
	if((pRect->m_Origin->m_X-m_dXc)*(pRect->m_Corner->m_X-m_dXc)<=0)
		if((pRect->m_Origin->m_Y-m_dYc)*(pRect->m_Corner->m_Y-m_dYc)<=0)
			return TRUE;

	GM2DPolyline* pSection;
	pSection = GetSection_Hit();
	if(pSection == NULL)
		return FALSE;
	
	GM2DBound MyBound;
	GM2DBound UrBound;
	pSection->Get2DBound(MyBound);
	pRect->GetCorners(UrBound);

	if(!MyBound.OverLapTo(&UrBound))
		return FALSE;
	
	long NumCurve = pSection->GetNumLines();
	for(long iCurve = 0 ; iCurve < NumCurve ; iCurve++)
	{
		GM2DCurve* pCurve = pSection->GetCurve(iCurve);
		if(pCurve->VisibleInRect(*pRect))
		{
			delete pCurve;
			return TRUE;
		}
		delete pCurve;
	}

	return FALSE;
}

// HCChoi1308 -> 조인트 엘리먼트 포리곤 테스트로 바꿀 것
long MSConnectivity::HitTest( MSCompoG* pCompoG, long JointIndex  )
{
	MSConnectivity* pCurJoint = NULL; //pCompoG->GetJoint(JointIndex);

	if(pCurJoint)  // 이미 부재 조인트가 있는 경우
	{
		if(pCurJoint==this) // 부재조인트와 동일 조인트
			return 0;
		else if(HitTest(pCurJoint)>0) // 부재조인트와 다른 조인트이지만, 히트테스트 성공. 조인트 폴리곤 테스트로 수정요
			return 2;
		else
			return -2;
	}

	if(pCompoG->IsLinkMember())
	{
		GM2DLineVector EndLVec;
		EndLVec = ((MSLinkMemberG*)pCompoG)->GetEndLine4Joint2(JointIndex, false);
//         if(pCompoG->IsFloorMember() && mp_DownFloorJoint && mp_DownFloorJoint->mp_Column)


        if(HitTest(EndLVec))		
            return 1;
 		long nMemPart = ma_MemPart.size();
 		if(nMemPart==1)
 		{
 			GM2DLineVector JointEndLVec;
 			MSLinkMemPart* pMemPart = ma_MemPart[0];
 			long StartOrEnd = pMemPart->mp_SConn == this ? 0 : 1;
 			JointEndLVec = pMemPart->GetEndLine(StartOrEnd, 0);

            GM2DVector CrossVec;
            double MyLoc, UrLoc;
            // ms_OverlapDistTol : 두 선이 5도 이내로 평행할 때 일직선상으로 보는 떨어진 거리
            long Result = EndLVec.IntersectEndWithLine(JointEndLVec, CrossVec, &MyLoc, &UrLoc, MSFloor::ms_LineHitDistTol, 5.);

            if(Result==-1) // ms_OverlapDistTol 거리 이내로 일직선
            {
                double Loc0 = EndLVec.OrthoBaseLocValue(JointEndLVec.m_P0);
                double Loc1 = EndLVec.OrthoBaseLocValue(JointEndLVec.m_P1);
                if((Loc0<-DTOL_GM && Loc1<-DTOL_GM) || (Loc0>1.+DTOL_GM && Loc1>1.+DTOL_GM))
                    return -1;
                else
                    return 1;
            }
 			if(Result==1)
 				return 1;
 			return -1;
 		}
		return -1;
	}
	else if(pCompoG->IsPointMember())
	{
        GM2DPolyline SecPoly(true);
        pCompoG->GetBoundary(SecPoly);
        GM2DVector JointVec(m_dXc, m_dYc);
        if(SecPoly.Contains(&JointVec, MSFloor::ms_LineHitDistTol)<=0)
            return 1;

        GM2DPolyline* pSection;
		pSection =	GetSection_Hit();
        if(pSection)
        {
            if(pSection->Contains(pCompoG->GetVec(0), MSFloor::ms_LineHitDistTol)<=0)
                return 1;
            if(pSection->OverlapEx(SecPoly))
                return 1;
        }
        return -1;
	}
    else
    {
        GM2DVector* pVec = pCompoG->GetVec(JointIndex);
        if(pVec==NULL)
            return 0;
        if(HitTest(*pVec))
            return 1;
        else
            return -1;
    }

	return 0;
}

bool MSConnectivity::HitTestAsMidJoint( MSLinkMemberG* pLink)
{
//     if(this == pLink->ma_Joint[0] || this==pLink->ma_Joint[1])
//         return false;

    GM2DPolyline* pSection;
    pSection = GetSection_Hit();
    if(!pSection) return false;

    // jissi / polyline 2占쏙옙 intersect 체크占쏙옙 占쏙옙占쏙옙

    long nPoint = pSection->GetNumLines();
    for(long iPoint=0; iPoint<nPoint; iPoint++)
    {
        if(pLink->HitTestWithVector(*(*pSection)[iPoint]))
            return true;
    }

    for(long iPoint=0; iPoint<nPoint; iPoint++)
    {
        GM2DVector CrossVec;
        double MyLoc, UrLoc;
        if(pLink->mp_LLine->IntersectEndWithLine(pSection->GetLine(iPoint), CrossVec, &MyLoc, &UrLoc)==1)
            return true;
        if(pLink->mp_RLine->IntersectEndWithLine(pSection->GetLine(iPoint), CrossVec, &MyLoc, &UrLoc)==1)
            return true;
    }
    return false;
}

long MSConnectivity::HitTest( MSConnectivity* pJoint )
{
	if(pJoint==NULL)
		return -1;
	if(pJoint==this)
		return 2;

	GM2DVector MyVec, UrVec;
    GM2DPolyline* pSection;
    pSection = GetSection_Hit();
    //if(!pSection) return 0;
	if(pSection == NULL)
		MyVec.SetXY(m_dXc, m_dYc);

	GM2DPolyline* pUrSection;
	pUrSection = pJoint->GetSection_Hit();
    //if(!pUrSection) return 0;
	if(pUrSection == NULL)
		UrVec.SetXY(pJoint->m_dXc, pJoint->m_dYc);

	if(pSection != NULL && pUrSection != NULL)
	{
		if(pSection->OverlapEx(*pUrSection, MSFloor::ms_LineHitDistTol, TRUE))
			return 2;
	}
	else
	{
		if(pSection != NULL && pUrSection == NULL)
		{
			if(pSection->Contains(&UrVec, MSFloor::ms_LineHitDistTol) < 1)
				return 2;
		}
		else if(pUrSection != NULL && pSection == NULL)
		{
			if(pUrSection->Contains(&MyVec, MSFloor::ms_LineHitDistTol) < 1)
				return 2;
		}
		else
		{
			if(MyVec.Distance(UrVec) < MSFloor::ms_LineHitDistTol)
				return 2;
		}
	}
	return 0;
}

long MSConnectivity::HitTest( GM2DLineVector& EndLVec )
{
    GM2DPolyline* pSection;
    pSection = GetSection_Hit();
    if(!pSection) return 0;

	return pSection->OverlapEx(&EndLVec, MSFloor::ms_LineHitDistTol);
}

long MSConnectivity::HitTest( GM2DVector& PosVec )
{
    GM2DPolyline* pSection;
    pSection = GetSection_Hit();
    if(!pSection) return 0;

	if(pSection->Contains(&PosVec, MSFloor::ms_LineHitDistTol) <= 0)
		return 1;

	return 0;
}

void MSConnectivity::MakeDefSection()
{
	mp_DefaultSection->ResetPolyline();


	long NumLevelZone = ma_LevelZone.GetSize();
	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		MSConnLevelZone* pConZone = ma_LevelZone[iZone];
		if(pConZone == NULL || pConZone->m_Section.ma_Point.GetCount() == 0) continue;

		GM2DPolyline* pCurPoly = &ma_LevelZone[iZone]->m_Section;
		if(mp_DefaultSection->ma_Point.GetCount() == 0)
		{
			mp_DefaultSection->CopyFrom(pCurPoly);
			continue;
		}

		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;

		mp_DefaultSection->BoolOperation('U', pCurPoly, JoinedPolys, 1);

		long NumJoined = JoinedPolys.GetCount();
		if(NumJoined == 1)
		{
			mp_DefaultSection->CopyFrom(JoinedPolys[0]);
		}

		for(long iJoined = 0 ; iJoined < NumJoined ; iJoined++)
			delete JoinedPolys[iJoined];

		JoinedPolys.RemoveAll();		
	}

	mp_DefaultSection->MakeClosedPolyline(DTOL_GM*1E3);

	MSPointMemberG* pPointG = NULL;
	if(mp_IsoFoot != NULL)
		pPointG = mp_IsoFoot;
	else if(mp_Column != NULL)
		pPointG = mp_Column;

	if(pPointG != NULL)
	{
		pPointG->GetBoundary(*mp_DefaultSection);
	}
}

GM2DPolyline* MSConnectivity::GetSection_Hit()
{
	/*
	if(mp_DefaultSection == NULL)
		MakeDefSection();

	long NumLevelZone = ma_LevelZone.GetSize();
	if(NumLevelZone == 0 || mp_DefaultSection->GetNumLines() < 2)
		return NULL;


    if(mp_DefaultSection)
        return mp_DefaultSection;
		*/
/*

	long NumLevelZone = ma_LevelZone.GetSize();
	if(NumLevelZone == 0)
		return NULL;

	double dMaxArea = 0.0;
	int nMaxAreaIdx = 0;

	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		double dCurArea = ma_LevelZone[iZone]->m_Section.Area();
		if(dCurArea > dMaxArea)
		{
			dMaxArea = dCurArea;
			nMaxAreaIdx = iZone;
		}
	}

	return ma_LevelZone[nMaxAreaIdx]->GetSection();*/

    if(ma_LevelZone.GetSize()>0)
        return ma_LevelZone[0]->GetSection();

	if(mp_DefaultSection)
		return mp_DefaultSection;

	return NULL;
}

GM2DPolyline* MSConnectivity::GetTopSection()
{
    if(ma_LevelZone.GetSize()>0)
        return ma_LevelZone[0]->GetSection();

    return NULL;
}

GM2DPolyline* MSConnectivity::GetBottomSection()
{
    int count = ma_LevelZone.GetSize();
    if(count>0)
        return ma_LevelZone[count-1]->GetSection();

    return NULL;
}

GM2DPolyline* MSConnectivity::GetDrawSection(bool current)
{
	GM2DPolyline* pPoly = NULL;

	long NumZone = ma_LevelZone.GetSize();
	if(current)
	{
		for(long iZone = NumZone -1 ; iZone >= 0 ; iZone--)
		{
			MSConnLevelZone* pConLevelZone = ma_LevelZone[iZone];
			if(pConLevelZone->m_isBaseSec == true) continue;

			if(MSFloor::ms_DrawHitJoint == true)
				pPoly = pConLevelZone->GetSection();
			else
				pPoly = pConLevelZone->GetDrawSection();
			break;
		}
	}
	else
	{
		for(long iZone = 0 ; iZone < NumZone ; iZone++)
		{
			MSConnLevelZone* pConLevelZone = ma_LevelZone[iZone];
			if(pConLevelZone->m_isBaseSec == true) continue;

			if(MSFloor::ms_DrawHitJoint == true)
				pPoly = pConLevelZone->GetSection();
			else
				pPoly = pConLevelZone->GetDrawSection();
			break;
		}
	}

	return pPoly;
}

void MSConnectivity::GetMergedSection(GM2DPolyline &Poly)
{
	long nCnt = 0;
	long NumLevelZone = ma_LevelZone.GetSize();
	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		MSConnLevelZone* pConZone = ma_LevelZone[iZone];
		if(pConZone == NULL || pConZone->IsDeleted())
			continue;

		if(nCnt == 0)
			Poly.CopyFrom(&pConZone->m_DrawSection);
		else
		{
			vector<GM2DPolyline*> JointPolys;
			Poly.BoolOperation('U', &pConZone->m_DrawSection, JointPolys, DTOL_GM);
			if(JointPolys.size() > 0)
				Poly.CopyFrom(JointPolys[0]);
		}

		nCnt++;
	}
}

void MSConnectivity::GetMergedSectionAll(GM2DPolyline &Poly)
{
	long nCnt = 0;
	GM2DPolyline CurPoly(TRUE);
	GetMergedSection(CurPoly);
	if(CurPoly.GetNumLines() > 2)
	{
		Poly.CopyFrom(&CurPoly);
		nCnt++;
	}

	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pUpJoint = ma_UpLinkJoint[iJoint];
		if(pUpJoint == NULL || pUpJoint->IsDeleted()) 
			continue;
		GM2DPolyline UpPoly(TRUE) ;
		pUpJoint->GetMergedSection(UpPoly);
		if(UpPoly.GetNumLines() < 3)
			continue;

		if(nCnt == 0)
			Poly.CopyFrom(&UpPoly);
		else
		{
			vector<GM2DPolyline*> JointPolys;
			Poly.BoolOperation('U', &UpPoly, JointPolys, 1);
			if(JointPolys.size() > 0)
				Poly.CopyFrom(JointPolys[0]);

			for(long iPoly = 0 ; iPoly < JointPolys.size() ; iPoly++)
				delete JointPolys[iPoly];
		}
		nCnt++;
	}

	long NumDnLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDnLinkJoint ; iJoint++)
	{
		MSConnectivity* pDownJoint = ma_DownLinkJoint[iJoint];
		if(pDownJoint == NULL || pDownJoint->IsDeleted()) 
			continue;
		GM2DPolyline DownPoly(TRUE) ;
		pDownJoint->GetMergedSection(DownPoly);
		if(DownPoly.GetNumLines() < 3)
			continue;

		if(nCnt == 0)
			Poly.CopyFrom(&DownPoly);
		else
		{
			vector<GM2DPolyline*> JointPolys;
			Poly.BoolOperation('U', &DownPoly, JointPolys, 1);
			if(JointPolys.size() > 0)
				Poly.CopyFrom(JointPolys[0]);

			for(long iPoly = 0 ; iPoly < JointPolys.size() ; iPoly++)
				delete JointPolys[iPoly];
		}
		nCnt++;
	}
}

vector<long> MSConnectivity::GetLinkJointID()
{
	vector<long> IdArr;
	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pUpJoint = ma_UpLinkJoint[iJoint];
		if(pUpJoint == NULL || pUpJoint->IsDeleted()) 
			continue;
		IdArr.push_back(pUpJoint->m_ID);
	}

	long NumDnLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDnLinkJoint ; iJoint++)
	{
		MSConnectivity* pDownJoint = ma_DownLinkJoint[iJoint];
		if(pDownJoint == NULL || pDownJoint->IsDeleted()) 
			continue;
		IdArr.push_back(pDownJoint->m_ID);
	}

	sort(IdArr.begin(), IdArr.end(), 
		[](const int& a, const int& b)->bool {return a < b;}
	);

	vector<long>::iterator new_end;
	new_end = unique(IdArr.begin(), IdArr.end());
	IdArr.erase(new_end, IdArr.end());

	return IdArr;
}

bool MSConnectivity::AddSptMember(MSCompoG* pMem)
{
	long StartOrEnd;

	MSCompoG::Type_MSCompoG MemType = pMem->GetType();
	if(MemType==MSCompoG::msColumnG)
	{
		mp_Column = (MSColumnG*)pMem;
		return true;
	}
	if(MemType==MSCompoG::msIsoFootG)
	{
		mp_IsoFoot = (MSIsoFootG*)pMem;
		return true;
	}

	if(MemType==MSCompoG::msWallG || MemType==MSCompoG::msBeamG || MemType==MSCompoG::msHandRailG ||
	   MemType==MSCompoG::msWallFootG || MemType==MSCompoG::msLineLevelDifG)
	{
// 이 경우 Retrim(pMem)은 AddPartMember에서 처리한다.

		MSLinkMemberG* pLink = (MSLinkMemberG*)pMem;

// 		if(pLink->GetSConn()==this)
// 			StartOrEnd = 0;		
// 		else if(pLink->GetEConn()==this)
// 			StartOrEnd = 1;
// 		else
		{
			//ma_MidJointMember 필요시 추가
// 			long numMid = pLink->ma_MidJoint.size();
// 			for(long iMid = 0;iMid<numMid;iMid++) // 언제 이곳으로 들어오나
// 			{
// 				if(pLink->ma_MidJoint[iMid] == this)
// 				{
// 					AddPartMember(1,pLink->ma_MemPart[iMid]);
// 					// 					AddPartMember(1,pLink->ma_MemPart[iMid+1]);
// 					AddPartMember(0,pLink->ma_MemPart[iMid+1]);   // 확인 필요
// 					return true;
// 				}
// 			}
			return false;
		}
		AddSptMember(StartOrEnd, pLink);

		return true;
	}

	return false;
}

static long SortDoubleList(CArray<double, double>& LevelArr, double NewValue)
{
    long nValue = LevelArr.GetSize();
    for(long iValue=0; iValue<nValue; iValue++)
    {
        if(fabs(NewValue-LevelArr[iValue])<DTOL_GM)
            return iValue;
        if(NewValue>LevelArr[iValue])
        {
            LevelArr.InsertAt(iValue, NewValue);
            return iValue;
        }
    }
    if(iValue==nValue)
    {
        LevelArr.Add(NewValue);
        return nValue;
    }
    return -1;
}

void MSConnectivity::ResetMemPartLines()
{
    long nPart = ma_MemPart.size();
    for(long iPart=0; iPart<nPart; iPart++)
    {
        GM2DVector pPoint;
        MSLinkMemPart* pMemPart = ma_MemPart[iPart];
		if(pMemPart->isBuriedLinkMember()) continue;

//         if(!pMemPart->mp_LinkMem->IsLineSupportMember())
//             continue;
        if(pMemPart->mp_SConn == this)
        {
//             if(pMemPart->mp_LinkMem->GetSConn()==pMemPart->mp_SConn)
//                 pPoint = *(pMemPart->mp_LinkMem->GetSVec());
//             else
            {
                pPoint = *(pMemPart->mp_SConn);
                // reset start cap line
                pMemPart->ma_SSegments.clear();
            }
            pMemPart->mp_LLine->SetP0(pMemPart->mp_LLine->OrthoBaseVector(pPoint));
            pMemPart->mp_RLine->SetP0(pMemPart->mp_RLine->OrthoBaseVector(pPoint));
        }
        else
        {
//             if(pMemPart->mp_LinkMem->GetEConn()==pMemPart->mp_EConn)
//                 pPoint = *(pMemPart->mp_LinkMem->GetEVec());
//             else
            {
                pPoint = *(pMemPart->mp_EConn);
                // reset end cap line
                pMemPart->ma_ESegments.clear();
            }
            pMemPart->mp_LLine->SetP1(pMemPart->mp_LLine->OrthoBaseVector(pPoint));
            pMemPart->mp_RLine->SetP1(pMemPart->mp_RLine->OrthoBaseVector(pPoint));
        }
    }
}

void MSConnectivity::TrimMembersEx( bool bResetMemPartLines, MSLevelZone* pMyLevelZone, bool bLinkJointTrim)
{
// 	if(mp_Floor->m_bIsSerializing 
// 		&& MSVersionInfo::GetCurrentVersion() >= 20140804)
// 		return;

	CheckMemPart();

    if(bResetMemPartLines)
        ResetMemPartLines();
	// 구간을 레벨별로 만들어서 ma_LevelZone에 생성

    BuildSectionZone(pMyLevelZone);

    long nZone = ma_LevelZone.GetSize();
    for(long iZone=nZone-1; iZone>=0; iZone--)
    {
		// 각 구간 별로 연결된 부재를 다시 찾아서 트림하고 m_Section을 완성
		MSConnLevelZone* pConnZone = ma_LevelZone[iZone];
		bool Result = pConnZone->BuildSection();
		pConnZone->m_DrawSection.CheckAndFixFoldedEdges(DTOL_GM);
		pConnZone->m_Section.CheckAndFixFoldedEdges(DTOL_GM);
		pConnZone->CheckBasePoly();
		if(!Result)
		{
			pConnZone->DeleteEndJointPoly();
			delete pConnZone;
			ma_LevelZone.RemoveAt(iZone);
			continue;
		}
		pConnZone->CheckBuriedMember();
    }
	
	ResetXYbySection();
	MakeDefSection();

	if(bLinkJointTrim == true)
	{
		long NumUpLinkJoint = ma_UpLinkJoint.size();
		for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
		{
			MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
			pJoint->TrimMembersEx(bResetMemPartLines, pMyLevelZone, false);
		}
// 		if(mp_UpLinkJoint != NULL)
// 			mp_UpLinkJoint->TrimMembersEx(bResetMemPartLines, pMyLevelZone, false);
	}
	
	LowFloorJointHandRailTrim();
}

void MSConnectivity::BuildSectionZone(MSLevelZone* pMyLevelZone)
{
    CArray<double, double> LevelArr;
    double BotLevel = 0.0, TopLevel =0.0;
    long BotIndex, TopIndex;
	double dLevel = 0;
	
	//Joint 가 포함된 LevelZone을 찾는다.
	MSLevelZone *pZone = NULL;
	if(pMyLevelZone == NULL)
		pZone = GetLevelZone();
	else
		pZone = pMyLevelZone;
	
	if(pZone != NULL)
		dLevel = pZone->m_Level;

    if(mp_IsoFoot)
    {
		//mp_IsoFoot->GetZLevelsbyJoint(TopLevel, BotLevel, this);
        TopIndex = SortDoubleList(LevelArr, TopLevel);
        BotIndex = SortDoubleList(LevelArr, BotLevel);
    }

    if(mp_Column)
    {
		//mp_Column->GetZLevelsbyJoint(TopLevel, BotLevel, this);
        TopIndex = SortDoubleList(LevelArr, TopLevel);
        BotIndex = SortDoubleList(LevelArr, BotLevel);
    }

	/*
	// 수직부재 수평 부재 조인트 존재시 ZLevel을 내려줄 필요가 없다.

	// kkw Add 20140317 Start
	double dFloorLevel = mp_Floor->GetFloorLevel();
	BOOL bUseMaxTopLevel = FALSE;
	double dMaxTopLevel = 0;
	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted() || mp_Column != NULL || mp_IsoFoot != NULL || pJoint->GetNumMemPart() == 1)
			continue;
		double dCurTopLevel = 0;
		if(pJoint->GetMemPartMinBotLevel(dCurTopLevel))
		{
			if(fabs(dFloorLevel - dCurTopLevel) > DTOL_GM)
			{
				if(bUseMaxTopLevel == FALSE)
					dMaxTopLevel = dCurTopLevel;
				else
					dMaxTopLevel = max(dMaxTopLevel, dCurTopLevel);

				bUseMaxTopLevel = TRUE;
			}
		}
	}

	BOOL bUseMinBotLevel = FALSE;
	double dMinBotLevel = 0;
	// kkw Add 20140317 End

    long nMemPart = ma_MemPart.size();
    for(long iMemPart=0; iMemPart<nMemPart; iMemPart++)
    {
        MSLinkMemPart* pMemPart = ma_MemPart[iMemPart];

		// kkw Add 20140317 Start
		pMemPart->GetZLevelsbyJoint(TopLevel, BotLevel, this);
		if(bUseMaxTopLevel && TopLevel > dMaxTopLevel)
			TopLevel = dMaxTopLevel;
		
		if(fabs(dFloorLevel - BotLevel) > DTOL_GM)
		{
			if(iMemPart == 0)
			{
				dMinBotLevel = BotLevel;
				bUseMinBotLevel = TRUE;
			}
			else if(dMinBotLevel > BotLevel)
				dMinBotLevel = BotLevel;
		}
		// kkw Add 20140317 End

        TopIndex = SortDoubleList(LevelArr, TopLevel);
        BotIndex = SortDoubleList(LevelArr, BotLevel);
    }

	//kkw add 20140317 start
	// DownLinkJoit의 ConnLevelZone TopLevel를 마춰줌
	
	if(bUseMinBotLevel)
	{
		long NumDownJoint = ma_DownLinkJoint.size();
		for(long iJoint = 0 ; iJoint < NumDownJoint ; iJoint++)
		{
			MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
			if(pJoint == NULL || pJoint->IsDeleted()|| pJoint->mp_Column != NULL || pJoint->mp_IsoFoot != NULL || GetNumMemPart() == 1)
				continue;

			long NumLevelZone = pJoint->ma_LevelZone.GetSize();
			for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
			{
				MSConnLevelZone* pConZone = pJoint->ma_LevelZone[iZone];
				if(pConZone->m_TopLevel > dMinBotLevel)
					pConZone->m_TopLevel = dMinBotLevel;

				if(pConZone->m_BotLevel > pConZone->m_TopLevel)
				{
					double dTmp = pConZone->m_BotLevel;
					pConZone->m_BotLevel = pConZone->m_TopLevel;
					pConZone->m_TopLevel = dTmp;
					//ASSERT(0);
				}
			}
		}
	}

// 	if(mp_DownLinkJoint != NULL && bUseMinBotLevel == TRUE)
// 	{
// 		long NumLevelZone = mp_DownLinkJoint->ma_LevelZone.GetSize();
// 		for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
// 		{
// 			MSConnLevelZone* pConZone = mp_DownLinkJoint->ma_LevelZone[iZone];
// 			if(pConZone->m_TopLevel > dMinBotLevel)
// 				pConZone->m_TopLevel = dMinBotLevel;
// 
// 			if(pConZone->m_BotLevel > pConZone->m_TopLevel)
// 			{
// 				double dTmp = pConZone->m_BotLevel;
// 				pConZone->m_BotLevel = pConZone->m_TopLevel;
// 				pConZone->m_TopLevel = dTmp;
// 				//ASSERT(0);
// 			}
// 		}
// 	}
*/

	// Up,DownLink 조인트 반영 안함
	long nMemPart = ma_MemPart.size();
	for(long iMemPart=0; iMemPart<nMemPart; iMemPart++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[iMemPart];
		//pMemPart->GetZLevelsbyJoint(TopLevel, BotLevel, this);

		TopIndex = SortDoubleList(LevelArr, TopLevel);
		BotIndex = SortDoubleList(LevelArr, BotLevel);
	}

	if(HasHandRail())
	{
		vector<MSConnectivity*> UpperVerJoint = GetUppperFloorHitVerticalJoint();
		if(UpperVerJoint.size() > 0)
		{
			double dFloorZLevel = 0;
			MSFloor* pUpperFloor = mp_Floor->GetUpperFloor();
			if(pUpperFloor != NULL)
			{
				dFloorZLevel = pUpperFloor->GetZLevel(m_dXc, m_dYc);
				SortDoubleList(LevelArr, dFloorZLevel);
			}
		}
	}

	//kkw add 20140317 endi

    DeleteLevelZone();
    long nSectionZone = LevelArr.GetSize();
    for(long iZone=0; iZone<nSectionZone-1; iZone++)
        ma_LevelZone.Add(new MSConnLevelZone(this, LevelArr[iZone], LevelArr[iZone+1]));
}

// 0 : 레벨 불일치

long MSConnectivity::LevelOverlap(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink)
{
	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;

// 	if(!pMyLink->GetZLevelsbyJoint(MyTopLevel, MyBotLevel, this))
// 		ASSERT(0);
// 	if(!pUrLink->GetZLevelsbyJoint(UrTopLevel, UrBotLevel, this))
// 		ASSERT(0);

	if(MyTopLevel <= UrBotLevel+DTOL1)
		return 0;
	if(UrTopLevel <= MyBotLevel+DTOL1)
		return 0;

	if(fabs(MyTopLevel-UrTopLevel) < DTOL1)
	{
		if(fabs(MyBotLevel-UrBotLevel) < DTOL1)
			return 1;
		else if(MyBotLevel>UrBotLevel)
			return 2;
		else if(MyBotLevel<UrBotLevel)
			return 3;
	}
	else if(MyTopLevel > UrTopLevel)
	{
		//트림을 하지 않음.
		if(fabs(MyBotLevel-UrBotLevel) < DTOL1)
			return -1;
		else if(MyBotLevel>UrBotLevel)
			return -2;
		else if(MyBotLevel<UrBotLevel)
			return -3;
	}
	else 
	{
		if(fabs(MyBotLevel-UrBotLevel) < DTOL1)
			return 4;
		else if(MyBotLevel>UrBotLevel)
			return 5;
		else if(MyBotLevel<UrBotLevel)
			return 6;
	}
	return 0;
}
bool MSConnectivity::IsNullJoint()
{
    if(mp_Column || mp_IsoFoot)
        return false;
	if(ma_SptMember.size() > 0 || ma_SlabMember.size() > 0 || ma_StairMember.size() > 0 || ma_MemPart.size() > 0)
		return false;

    return true;
}

void MSConnectivity::Init( bool bInConstructor/*=false*/ )
{
    if(!bInConstructor)
        MSObject::Init(bInConstructor);

    m_bDeleted = false;

    m_ID = 0;
    m_nFloorNum = 0;
    m_dXc = 0.;
    m_dYc = 0.;

    mp_UpFloorJoint = NULL;
    mp_DownFloorJoint = NULL;

    mp_Source = NULL;
    mp_Column = NULL;
    mp_IsoFoot = NULL;
    mp_JointMember = NULL;
}

void MSConnectivity::UnlinkUpdownFloorJoint()
{
    if(mp_UpFloorJoint)
    {
        mp_UpFloorJoint->mp_DownFloorJoint = NULL;
        mp_UpFloorJoint = NULL;
    }
    if(mp_DownFloorJoint)
    {
        mp_DownFloorJoint->mp_UpFloorJoint = NULL;
        mp_DownFloorJoint = NULL;
    }
}

bool MSConnectivity::HasOneLineMember()
{
    long nLineMember = 0;
    long nMem = ma_SptMember.size();
    for(long iMem=0; iMem<nMem; iMem++)
    {
        if(ma_SptMember[iMem]->IsLineSupportMember())
            nLineMember++;
        if(nLineMember>1) return false;
    }
    if(nLineMember==1) return true;
    return false;
}

bool MSConnectivity::HasOnlyOneMemPart()
{
    if (this != NULL)
        return GetNumMemPart()==1;
    return false;
}

bool MSConnectivity::HasOneVerticalSupportMember()
{
    long nVerticalMember = 0;
    if(mp_Column)
        nVerticalMember++;
    long nMem = ma_SptMember.size();
    for(long iMem=0; iMem<nMem; iMem++)
    {
        if(ma_SptMember[iMem]->IsWallMember())
            nVerticalMember++;
        if(nVerticalMember>1) return false;
    }
    if(nVerticalMember==1) return true;
    return false;
}

bool MSConnectivity::HasOneWallMember()
{
    long nWall = 0;
    long nMem = ma_MemPart.size();
    for(long iMem=0; iMem<nMem; iMem++)
    {
        if(ma_MemPart[iMem]->mp_LinkMem->IsWallMember())
        {
            long jMem;
            for(jMem=0; jMem<iMem; jMem++)
                if(ma_MemPart[iMem]->mp_LinkMem==ma_MemPart[jMem]->mp_LinkMem)
                    break;
            if(jMem==iMem)
                nWall++;
        }
        if(nWall>1) return false;
    }
    if(nWall==1) return true;
    return false;
}

bool MSConnectivity::HasWallFootGPartCnt(long NumWall)
{
	long WallPartCnt = 0;
	long nMem = ma_MemPart.size();
	for(long iMem=0; iMem<nMem; iMem++)
	{
		if(ma_MemPart[iMem]->mp_LinkMem->GetType() == MSElement::msWallFootG)
			WallPartCnt++;
	}

	if(WallPartCnt >= NumWall)
		return true;
	else
		return false;

	long NumSptMem = ma_SptMember.size();
	for(long iSptMem = 0 ; iSptMem < NumSptMem ; iSptMem++)
	{
		if(ma_SptMember[iSptMem]->GetType() == MSElement::msWallFootG)
			return true;
	}

	return false;
}

long MSConnectivity::GetMaxThickWallFootGID()
{
	long nGID = 0;

	double MaxThick = 0;
	long nMem = ma_MemPart.size();
	for(long iMem=0; iMem<nMem; iMem++)
	{
		if(ma_MemPart[iMem]->mp_LinkMem->GetType() == MSElement::msWallFootG)
		{
			MSLinkMemberG* pWallFootG = (MSLinkMemberG*)ma_MemPart[iMem]->mp_LinkMem;
			MSCompoM* pCompoM = pWallFootG->GetCompoM();
			if(pCompoM == NULL) continue;
			double CurThick = pCompoM->GetThick();
			if(MaxThick < CurThick )
			{
				MaxThick = CurThick;
				nGID = pWallFootG->m_ID;
			}
		}
	}

	long NumSptMem = ma_SptMember.size();
	for(long iSptMem = 0 ; iSptMem < NumSptMem ; iSptMem++)
	{
		if(ma_SptMember[iSptMem]->GetType() == MSElement::msWallFootG)
		{
			MSLinkMemberG* pWallFootG = (MSLinkMemberG*)ma_SptMember[iSptMem];
			MSCompoM* pCompoM = pWallFootG->GetCompoM();
			if(pCompoM == NULL) continue;
			double CurThick = pCompoM->GetThick();
			if(MaxThick < CurThick )
			{
				MaxThick = CurThick;
				nGID = pWallFootG->m_ID;
			}
		}	
	}

	return nGID;
}

double MSConnectivity::GetMaxWidthByLineMemberG()
{
	double MaxWidth = 0;
	long nMem = ma_MemPart.size();
	for(long iMem=0; iMem<nMem; iMem++)
	{
		MSLinkMemberG* pLinkMemberG = (MSLinkMemberG*)ma_MemPart[iMem]->mp_LinkMem;
		double CurWidth = pLinkMemberG->GetWidth();
		MaxWidth = max(MaxWidth, CurWidth);
	}

	long NumSptMem = ma_SptMember.size();
	for(long iSptMem = 0 ; iSptMem < NumSptMem ; iSptMem++)
	{
		MSLinkMemberG* pLinkMemberG = (MSLinkMemberG*)ma_SptMember[iSptMem];
		double CurWidth = pLinkMemberG->GetWidth();
		MaxWidth = max(MaxWidth, CurWidth);
	}

	return MaxWidth;
}

bool MSConnectivity::HasNoVerticalMemPart()
{
    if(mp_Column)
        return false;

    long nMem = ma_MemPart.size();
    for(long iMem=0; iMem<nMem; iMem++)
    {
        if(ma_MemPart[iMem]->mp_LinkMem->IsVerticalMember())
            return false;
    }

    return true;
}

bool MSConnectivity::HasHorizontalMemPart()
{
    long nMem = ma_MemPart.size();
    for(long iMem=0; iMem<nMem; iMem++)
    {
        if(!ma_MemPart[iMem]->mp_LinkMem->IsVerticalMember())
            return true;
    }

    return false;
}

bool MSConnectivity::HasNoFloorMemPart()
{
    long nMem = ma_MemPart.size();
    for(long iMem=0; iMem<nMem; iMem++)
    {
        MSLinkMemberG* pLink = ma_MemPart[iMem]->mp_LinkMem;
        if(ma_MemPart[iMem]->mp_LinkMem->IsLineSupportMember())
            return false;
    }

    return true;
}

void MSConnectivity::DeleteLevelZone()
{

	long nMemPart = ma_MemPart.size();
	for(long iMemPart=0; iMemPart<nMemPart; iMemPart++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[iMemPart];
		if(pMemPart->GetSConn()==this)
			pMemPart->DeleteEndJointPolyAll(0);
		else
			pMemPart->DeleteEndJointPolyAll(1);
	}


    long nLevelZone = ma_LevelZone.GetSize();
    for(long iLevelZone=0; iLevelZone<nLevelZone; iLevelZone++)
        delete ma_LevelZone[iLevelZone];
    ma_LevelZone.RemoveAll();
}

void MSConnectivity::GetZLevels( double& TopLevel, double& BotLevel)
{
	if(mp_Floor == NULL)
		return;

    BotLevel = mp_Floor->GetZLevel(m_dXc, m_dYc);
    MSFloor* pUpFloor = mp_Floor->GetUpperFloor();
    TopLevel = pUpFloor->GetZLevel(m_dXc, m_dYc);
}

bool MSConnectivity::IsSameOrUpDown( MSConnectivity* pConn )
{
    if(pConn==NULL)
        return false;
	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;
		if(pJoint == pConn)
			return true;
	}
// 	if(mp_UpLinkJoint != NULL && mp_UpLinkJoint == pConn)
// 		return true;

	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;
		if(pJoint == pConn)
			return true;
	}
// 	if(mp_DownLinkJoint != NULL && mp_DownLinkJoint == pConn)
// 		return true;
    if(this==pConn || mp_DownFloorJoint==pConn || mp_UpFloorJoint==pConn)
        return true;
    return false;
}

MSLinkMemPart* MSConnectivity::GetNextMember( MSLinkMemPart* pMyMem, char Side, MSConnectivity** NextJoint, bool CheckMinDeg)
{
	vector<MSLinkMemPart*> MemParts;
	vector<double> MemPartDegrees;

	vector<MSLinkMemPart*> OtherMemParts;
	vector<double> OtherMemPartDegrees;
	MSConnectivity* OtherJoint;

	MemParts = ma_MemPart;
	MemPartDegrees = ma_MemPartDegree;

	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;
		for(long i = 0 ; i < pJoint->ma_MemPart.size() ;i++)
			OtherMemParts.push_back(pJoint->ma_MemPart[i]);
		for(long i = 0 ; i < pJoint->ma_MemPart.size() ;i++)
			OtherMemPartDegrees.push_back(pJoint->ma_MemPartDegree[i]);
	}

	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;
		for(long i = 0 ; i < pJoint->ma_MemPart.size() ;i++)
			OtherMemParts.push_back(pJoint->ma_MemPart[i]);
		for(long i = 0 ; i < pJoint->ma_MemPart.size() ;i++)
			OtherMemPartDegrees.push_back(pJoint->ma_MemPartDegree[i]);
	}

// 	if(mp_UpLinkJoint != NULL)
// 	{
// 		OtherMemParts = mp_UpLinkJoint->ma_MemPart;
// 		OtherMemPartDegrees = mp_UpLinkJoint->ma_MemPartDegree;
// 		OtherJoint = mp_UpLinkJoint;
// 	}
// 	else if(mp_DownLinkJoint != NULL)
// 	{
// 		OtherMemParts = mp_DownLinkJoint->ma_MemPart;
// 		OtherMemPartDegrees = mp_DownLinkJoint->ma_MemPartDegree;
// 		OtherJoint = mp_DownLinkJoint;
// 	}

	// Merge OtherParts to MemParts
	if(OtherMemParts.size()>0)
	{
		for(int i=0; i<OtherMemParts.size(); i++)
		{
			double degree = OtherMemPartDegrees[i];
			
			auto iter = MemPartDegrees.begin();
			auto iter2 = MemParts.begin();
			
			bool bProcessed = false;
			for(;iter != MemPartDegrees.end(); iter++, iter2++)
			{
				if(degree < *iter)
				{
					MemPartDegrees.insert(iter,degree);
					MemParts.insert(iter2,OtherMemParts[i]);
					bProcessed = true;
					break;
				}
			}
			if(!bProcessed)
			{
				MemPartDegrees.push_back(degree);
				MemParts.push_back(OtherMemParts[i]);
			}

			
		}
	}

	long nMem = MemParts.size();
	if(nMem == 1)
		return NULL;

	MSLinkMemPart* pNextMem = NULL;
	for(long iMem=0; iMem<nMem; iMem++)
	{
		if(pMyMem==MemParts[iMem])
			break;
	}

	if(iMem==nMem)
		return NULL;

	if(Side == 'R')
	{
		if(iMem == nMem-1)
			pNextMem = MemParts[0];
		else
			pNextMem = MemParts[iMem+1];
	}
	else
	{
		if(iMem == 0)
			pNextMem = MemParts[nMem-1];
		else
			pNextMem = MemParts[iMem-1];
	}

	if(IsConnected(pNextMem->GetSConn()))
		*NextJoint = pNextMem->GetEConn();
	else
		*NextJoint = pNextMem->GetSConn();

	return pNextMem;
}

double MSConnectivity::DistanceOnXY( MSConnectivity* pJoint )
{
    return sqrt((pJoint->m_dXc-m_dXc)*(pJoint->m_dXc-m_dXc)
        +(pJoint->m_dYc-m_dYc)*(pJoint->m_dYc-m_dYc));
}

double MSConnectivity::DistanceOnXY( GM2DVector* pJoint )
{
    return sqrt((pJoint->m_X-m_dXc)*(pJoint->m_X-m_dXc)
        +(pJoint->m_Y-m_dYc)*(pJoint->m_Y-m_dYc));
}

MSLinkMemberG* MSConnectivity::IsLevelOverlapOfWithSameTypeMember( MSLinkMemberG* pMyLinkG )
{
	MSConnectivity* pMyEConn = NULL; //pMyLinkG->GetEConn();
	if(pMyEConn==this) // buried member라는 뜻
		return NULL;
	long nSptMem = ma_SptMember.size();
	for(long iSptMem=0; iSptMem<nSptMem; iSptMem++)
	{
		MSLinkMemberG* pUrLinkG = ma_SptMember[iSptMem];
// 		if(pUrLinkG->GetSConn()!=pMyEConn && pUrLinkG->GetEConn()!=pMyEConn)
// 			continue;
		BOOL bImWall = pMyLinkG->IsWallMember();
		BOOL bUrWall = pUrLinkG->IsWallMember();
		if(bImWall != bUrWall)
			continue;
		//if(pMyLinkG->IsLevelOverlap(pUrLinkG)==false)
		//if(pMyLinkG->IsLevelOverlapbyJoint(pUrLinkG, this)==false)
			continue;
		return pUrLinkG;
	}
	return NULL;
}

bool MSConnectivity::IsHasOnlyCompoG(MSCompoG* pCompoG)
{
	// MSConnectivity에 pCompoG만 있을 경우 TRUE
	bool bHas = true;
	long NumMemPart = ma_MemPart.size();
	for(long iPart = 0 ; iPart < NumMemPart ; iPart++)
	{
		MSLinkMemPart* pLinkMemPart = ma_MemPart[iPart];
		if(pLinkMemPart->mp_LinkMem != NULL && pLinkMemPart->mp_LinkMem != pCompoG)
			bHas = false;
	}

	long NumSptMember = ma_SptMember.size();
	for(long iSptMem = 0 ; iSptMem < NumSptMember ; iSptMem++)
	{
		MSLinkMemberG* pLinkMemberG = ma_SptMember[iSptMem];
		if(pLinkMemberG != NULL && pLinkMemberG != pCompoG)
			bHas = false;
	}
	
	long NumMidMem = ma_MidJointMember.size();
	for(long iMid = 0 ; iMid < NumMidMem ; iMid++)
	{
		MSLinkMemberG* pLinkMemberG = ma_MidJointMember[iSptMem];
		if(pLinkMemberG != NULL && pLinkMemberG != pCompoG)
			bHas = false;
	}

	long NumSlab = ma_SlabMember.size();
	for(long iSlab = 0 ; iSlab < NumSlab ; iSlab++)
	{
		MSSlabG* pSlab = ma_SlabMember[iSlab];
		if(pSlab != NULL && pSlab != pCompoG)
			bHas = false;
	}

	long NumStair = ma_StairMember.size();
	for(long iStar = 0 ; iStar < NumStair ; iStar++)
	{
		MSGenStairG* pStart = ma_StairMember[iStar];
		if(pStart != NULL && pStart != pCompoG)
			bHas = false;
	}

	if( mp_Column!= NULL && mp_Column != pCompoG)
		bHas = false;

	if( mp_IsoFoot!= NULL && mp_IsoFoot != pCompoG)
		bHas = false;

	return bHas;
}

MSLevelZone* MSConnectivity::GetLevelZone()
{
	if(mp_Floor == NULL)
		return NULL;

	GM2DVector MyVec(m_dXc, m_dYc);

	long NumLevelZone = mp_Floor->ma_LevelZone.size();
	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		MSLevelZone* pZone = mp_Floor->ma_LevelZone[iZone];
		if(pZone == NULL)	continue;
		GM2DPolyline* pPoly = pZone->GetProfile();
		if(pPoly == NULL) continue;
		long nRval = pPoly->Contains(&MyVec);
		if( nRval < 1)
			return pZone;
	}

	return NULL;
}

map<long, MSCompoG*> MSConnectivity::GetAllCompoGMap()
{
	map<long, MSCompoG*> CompoGMap;

	bool bHas = true;
	long NumMemPart = ma_MemPart.size();
	for(long iPart = 0 ; iPart < NumMemPart ; iPart++)
	{
		MSLinkMemPart* pLinkMemPart = ma_MemPart[iPart];
		if(pLinkMemPart != NULL && pLinkMemPart->mp_LinkMem != NULL)
			CompoGMap.insert(make_pair(pLinkMemPart->mp_LinkMem->m_ID, pLinkMemPart->mp_LinkMem));
	}

	long NumSptMember = ma_SptMember.size();
	for(long iSptMem = 0 ; iSptMem < NumSptMember ; iSptMem++)
	{
		MSLinkMemberG* pLinkMemberG = ma_SptMember[iSptMem];
		if(pLinkMemberG != NULL )
			CompoGMap.insert(make_pair(pLinkMemberG->m_ID, pLinkMemberG));
	}

	long NumMidMem = ma_MidJointMember.size();
	for(long iMid = 0 ; iMid < NumMidMem ; iMid++)
	{
		MSLinkMemberG* pLinkMemberG = ma_MidJointMember[iSptMem];
		if(pLinkMemberG != NULL)
			CompoGMap.insert(make_pair(pLinkMemberG->m_ID, pLinkMemberG));
	}

	long NumSlab = ma_SlabMember.size();
	for(long iSlab = 0 ; iSlab < NumSlab ; iSlab++)
	{
		MSSlabG* pSlab = ma_SlabMember[iSlab];
		if(pSlab != NULL)
			CompoGMap.insert(make_pair(pSlab->m_ID, pSlab));
	}

	long NumStair = ma_StairMember.size();
	for(long iStar = 0 ; iStar < NumStair ; iStar++)
	{
		MSGenStairG* pStart = ma_StairMember[iStar];
		if(pStart != NULL)
			CompoGMap.insert(make_pair(pStart->m_ID, pStart));
	}

	if( mp_Column!= NULL)
		CompoGMap.insert(make_pair(mp_Column->m_ID, mp_Column));

	if( mp_IsoFoot!= NULL)
		CompoGMap.insert(make_pair(mp_IsoFoot->m_ID, mp_IsoFoot));



	return CompoGMap;
}

MSConnectivity* MSConnectivity::GetCloneJoint(MSFloor* pFloor)
{
	long NumClone = ma_Clone.size();
	for(long iClone = 0 ; iClone < NumClone ; iClone++)
	{
		MSConnectivity* pCloneJoint = ma_Clone[iClone];
		if(pCloneJoint == NULL) continue;
		if(pCloneJoint->mp_Floor == pFloor)
			return pCloneJoint;
	}

	return NULL;
}

void MSConnectivity::CheckMemPart()
{
	long numPart = ma_MemPart.size();
	for(long iPart = numPart-1; iPart >= 0; iPart--)
	{
		for(long jPart = 0 ; jPart < ma_MemPart.size() ; jPart++)
		{
			if(iPart == jPart) continue;
			MSLinkMemPart* iMemPart = ma_MemPart[iPart];
			MSLinkMemPart* jMemPart = ma_MemPart[jPart];
// 			if(iMemPart->isBuriedLinkMember())
// 				continue;
			if(iMemPart == jMemPart)
			{
				ma_MemPart.erase(ma_MemPart.begin() + iPart);
				ma_MemPartDegree.erase(ma_MemPartDegree.begin() + iPart);
				break;
			}
		}
	}

	//MSLinkMemPart의 SConn, EConn 이 같으면 삭제 해준다.
	long NumMemPart = ma_MemPart.size();
	for(long iPart = NumMemPart -1 ; iPart >= 0 ; iPart--)
	{
		MSLinkMemPart* pPart = ma_MemPart[iPart];
		if(pPart == NULL) continue;
		MSLinkMemberG* pLinkMem = pPart->mp_LinkMem;
		if(pPart->mp_SConn == pPart->mp_EConn)
		{
// 			long Num = pLinkMem->ma_MemPart.size();
// 			if(Num > 1 && !pLinkMem->m_isBuriedLinkMember)
// 			{
// 				for(long i = Num - 1 ; i >= 0 ; i--)
// 				{
// 					if(pLinkMem->ma_MemPart[i] != pPart)
// 						continue;
// // 					delete pLinkMem->ma_MemPart[i];
// // 					pLinkMem->ma_MemPart.erase(pLinkMem->ma_MemPart.begin() + i);
// 					//pLinkMem->RemoveMidJoint(pPart->mp_EConn);
// 				}
// 				if(ma_MemPart.size() > iPart)
// 					ma_MemPart.erase(ma_MemPart.begin() + iPart);
// 			}
		}
	}
}

bool MSConnectivity::GetMemPartMinBotLevel(double &dMinBotLevel)
{
	long NumMemPart = ma_MemPart.size();
	if(NumMemPart == 0)
		return FALSE;
	double dTopLevel, dBotLevel;
	//ma_MemPart[0]->GetZLevels(dTopLevel, dMinBotLevel);
	//ma_MemPart[0]->GetZLevelsbyJoint(dTopLevel, dMinBotLevel, this);
	for(long iPart = 1 ; iPart < NumMemPart ; iPart++)
	{
		//ma_MemPart[iPart]->GetZLevels(dTopLevel, dBotLevel);
		//ma_MemPart[iPart]->GetZLevelsbyJoint(dTopLevel, dBotLevel, this);
		if(dMinBotLevel > dBotLevel)
			dMinBotLevel = dBotLevel;
	}
	return TRUE;
}

bool MSConnectivity::IsMidConn()
{
	//링크 부재 중에서 하나라도 시작 및 끝 조인트가 아니면 미드 조인트가 아니다.
	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[iMemPart];
		if(pMemPart == NULL) continue;
		MSLinkMemberG* pLinkMemberG = pMemPart->mp_LinkMem;
// 		if(pLinkMemberG->GetSConn() != this && pLinkMemberG->GetEConn() != this)
// 			return true;
	}

	return false;
}

bool MSConnectivity::HasOnlyFloorMember()
{
	if(IsNullJoint())
		return false;

	if(mp_Column != NULL)
		return false;

	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemberG* pLink = ma_MemPart[iMemPart]->mp_LinkMem;
		if(ma_MemPart[iMemPart]->mp_LinkMem->IsVerticalMember())
			return false;
	}
		
	return true;
}

bool MSConnectivity::HasOnlyVerticalMember()
{
	if(IsNullJoint())
		return false;

	if(mp_IsoFoot != NULL)
		return false;

	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemberG* pLink = ma_MemPart[iMemPart]->mp_LinkMem;
		if(ma_MemPart[iMemPart]->mp_LinkMem->IsFloorMember())
			return false;
	}

	return true;
}

bool MSConnectivity::HasOnlyFloorLineMember()
{
	if(mp_Column != NULL)
		return false;

	if(mp_IsoFoot != NULL)
		return false;

	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemberG* pLink = ma_MemPart[iMemPart]->mp_LinkMem;
		if(ma_MemPart[iMemPart]->mp_LinkMem->IsVerticalMember())
			return false;
	}

	return true;
}

bool MSConnectivity::HasOnlyVerticalLineMember()
{
	if(mp_Column != NULL)
		return false;

	if(mp_IsoFoot != NULL)
		return false;

	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemberG* pLink = ma_MemPart[iMemPart]->mp_LinkMem;
		if(ma_MemPart[iMemPart]->mp_LinkMem->IsFloorMember())
			return false;
	}

	return true;
}

bool MSConnectivity::HasArcMember()
{
	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemberG* pLink = ma_MemPart[iMemPart]->mp_LinkMem;
		if(pLink->isArc())
			return true;
	}
	return false;
}

long MSConnectivity::GetNumMemberG()
{
	long NumLinkMember = 0;
	map<long, MSCompoG*> LinkMemMap;
	long nMem = ma_MemPart.size();
	for(int i=0; i< nMem ; i++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[i];
		LinkMemMap.insert(make_pair(pMemPart->mp_LinkMem->m_ID, pMemPart->mp_LinkMem));
	}
	return LinkMemMap.size();
}

long MSConnectivity::GetNumMemPart()
{
	long Count = 0;
	long nMem = ma_MemPart.size();
	for(int i=0; i< nMem ; i++)
	{
		MSLinkMemPart* pMemPart = ma_MemPart[i];
		if(pMemPart->isBuriedLinkMember()) continue;
		Count++;
	}
	return Count;
}

bool MSConnectivity::HasBaseSec()
{
	long NumZone = ma_LevelZone.GetSize();
	for(long iZone = 0 ; iZone < NumZone ; iZone++)
	{
		MSConnLevelZone* pZone = ma_LevelZone[iZone];
		if(pZone == NULL) continue;
		if(pZone->m_isBaseSec)
			return true;
	}
	return false;
}

MSConnectivity* MSConnectivity::AddUpLinkJoint(MSConnectivity* pJoint)
{
	if(pJoint == NULL)
		return NULL;

	MSConnectivity* pMergeJoint = NULL;
	bool isMerge = false;

	long NumJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumJoint ; iJoint++)
	{
		MSConnectivity* pCurJoint = ma_UpLinkJoint[iJoint];
		if(pCurJoint == NULL || pCurJoint->IsDeleted()) 
			continue;
		if(pCurJoint == pJoint)
			return NULL;
		if(pCurJoint->HitTest(pJoint))
		{
			long nRval = pCurJoint->MergeToMeAndDelete(mp_Floor, pJoint);
			if(nRval == 0)
			{
				isMerge = true;
				return pCurJoint;
			}
		}
	}

	if(!isMerge)
		ma_UpLinkJoint.push_back(pJoint);

	return NULL;
}

MSConnectivity* MSConnectivity::AddDownLinkJoint(MSConnectivity* pJoint)
{
	if(pJoint == NULL)
		return NULL;
	
	MSConnectivity* pMergeJoint = NULL;
	bool isMerge = false;
	
	long NumJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumJoint ; iJoint++)
	{
		MSConnectivity* pCurJoint = ma_DownLinkJoint[iJoint];
		if(pCurJoint == NULL || pCurJoint->IsDeleted()) 
			continue;
		if(pCurJoint == pJoint)
			return NULL;
		if(pCurJoint->HitTest(pJoint))
		{
			long nRval = pCurJoint->MergeToMeAndDelete(mp_Floor, pJoint);
			if(nRval == 0)
			{
				isMerge = true;
				return pCurJoint;
			}
		}
	}

	if(!isMerge)
		ma_DownLinkJoint.push_back(pJoint);

	return NULL;
}

bool MSConnectivity::RemoveUpLinkJoint(MSConnectivity* pJoint)
{
	bool bRval = false;
	long NumJoint = ma_UpLinkJoint.size();
	for(long iJoint = NumJoint -1 ; iJoint >= 0 ; iJoint--)
	{
		MSConnectivity* pCurJoint = ma_UpLinkJoint[iJoint];
		if(pCurJoint->IsDeleted() || pCurJoint == pJoint)
		{
			ma_UpLinkJoint.erase(ma_UpLinkJoint.begin() + iJoint);
			bRval = true;
		}
	}
	return bRval;
}
bool MSConnectivity::RemoveDownLinkJoint(MSConnectivity* pJoint)
{
	bool bRval = false;
	long NumJoint = ma_DownLinkJoint.size();
	for(long iJoint = NumJoint -1 ; iJoint >= 0 ; iJoint--)
	{
		MSConnectivity* pCurJoint = ma_DownLinkJoint[iJoint];
		if(pCurJoint->IsDeleted() || pCurJoint == pJoint)
		{
			ma_DownLinkJoint.erase(ma_DownLinkJoint.begin() + iJoint);
			bRval = true;
		}
	}
	return bRval;	
}

void MSConnectivity::RemoveUpLinkJointsByMe(MSConnectivity* pJoint)
{
	long NumJoint = ma_UpLinkJoint.size();
	for(long iJoint = NumJoint -1 ; iJoint >= 0 ; iJoint--)
	{
		MSConnectivity* pCurJoint = ma_UpLinkJoint[iJoint];
		if(pCurJoint == NULL) continue;
		bool bDnRval = pCurJoint->RemoveDownLinkJoint(pJoint);
		bool bUpRval = pCurJoint->RemoveUpLinkJoint(pJoint);
		if(pCurJoint->IsDeleted() || pCurJoint == pJoint)
			ma_UpLinkJoint.erase(ma_UpLinkJoint.begin() + iJoint);
	}
}

void MSConnectivity::RemoveDownLinkJointsByMe(MSConnectivity* pJoint)
{
	long NumJoint = ma_DownLinkJoint.size();
	for(long iJoint = NumJoint -1 ; iJoint >= 0 ; iJoint--)
	{
		MSConnectivity* pCurJoint = ma_DownLinkJoint[iJoint];
		if(pCurJoint == NULL) continue;
		bool bDnRval = pCurJoint->RemoveDownLinkJoint(pJoint);
		bool bUpRval = pCurJoint->RemoveUpLinkJoint(pJoint);
		if(pCurJoint->IsDeleted() || pCurJoint == pJoint)
			ma_DownLinkJoint.erase(ma_DownLinkJoint.begin() + iJoint);
	}
}

void MSConnectivity::ResetXYbySection()
{
	if(HasMSType(MSElement::msLineLevelDifG))
		return;

	GM2DPolyline* pPoly = GetSection_Hit();
	if(pPoly == NULL || pPoly->CheckPolygon() == FALSE)
		return;

	GM2DVector OrgVec;
	GM2DVector Vec = pPoly->MassCenter();
	if(OrgVec.SameVector4DTOL(Vec, DTOL_GM))
		return;
	m_dXc = Vec.m_X;
	m_dYc = Vec.m_Y;
}

MSConnLevelZone* MSConnectivity::GetConLevelZoneByZC(double dZc)
{
	long NumZone = ma_LevelZone.GetSize();
	if(NumZone == 0)
		return NULL;
	for(long iZone = NumZone -1 ; iZone >= 0 ; iZone--)
	{
		MSConnLevelZone* pZone = ma_LevelZone[iZone];
		if(pZone->m_TopLevel > dZc && pZone->m_BotLevel <= dZc)
			return pZone;
	}

	return NULL;
}

MSConnLevelZone* MSConnectivity::GetTopConLevelZone()
{
	long NumZone = ma_LevelZone.GetSize();
	if(NumZone == 0)
		return NULL;

	return ma_LevelZone[0];
}

MSConnLevelZone* MSConnectivity::GetBotConLevelZone()
{
	long NumZone = ma_LevelZone.GetSize();
	if(NumZone == 0)
		return NULL;

	return ma_LevelZone[NumZone -1];
}

bool MSConnectivity::HasHandRail()
{
	long NumPart = ma_MemPart.size();
	for(long iPart = 0 ; iPart < NumPart ; iPart++)
	{
		if(ma_MemPart[iPart]->mp_LinkMem->GetType() == MSElement::msHandRailG)
			return true;
	}
	return false;
}

bool MSConnectivity::HasCompoG()
{
	if(mp_Column != NULL)
		return true;
	if(mp_IsoFoot != NULL)
		return true;
	long NumMemPart = ma_MemPart.size();
	if(NumMemPart > 0)
		return true;
	long NumSptMember = ma_SptMember.size();
	if(NumSptMember > 0)
		return true;
	long NumBuriedLinkMember = ma_BuriedLinkMember.GetSize();
	if(NumBuriedLinkMember > 0)
		return true;

	return false;
}

bool MSConnectivity::IsConnectedBelowColumn(MSColumnG* pUpColumn)
{
	if(mp_Column == NULL || pUpColumn == NULL)
		return false;
	
	if(fabs(pUpColumn->GetStartLevel() - mp_Column->GetEndLevel()) > DTOL1)
		return false;
	return true;
}

bool MSConnectivity::IsConnectedVMember(double dZc, double dDepth)
{
	if(mp_Column)
	{
		if(fabs(mp_Column->GetEndLevel() - dZc) < DTOL1)
			return true;
	}

	MSConnLevelZone* pTopLevelZone = GetTopConLevelZone();
	MSConnLevelZone* pBotLevelZone = GetBotConLevelZone();
	if(pTopLevelZone == NULL || pBotLevelZone == NULL)
		return false;

	double minZc = pBotLevelZone->m_BotLevel;
	double maxZc = pTopLevelZone->m_TopLevel;

	if( minZc <= dZc - dDepth && dZc - dDepth <= maxZc)
		return true;

	return false;
}

bool MSConnectivity::IsConnectedVMemberInDownLinkJoint(double dZc, double dDepth)
{
	long NumJoint = ma_DownLinkJoint.size();

	for(long iJoint = 0 ; iJoint < NumJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
		if(pJoint == NULL)
			continue;
		if(pJoint->IsConnectedVMember(dZc, dDepth))
			return true;
	}

	return false;
}

bool MSConnectivity::HasColumn()
{
	if(mp_Column == NULL)
		return false;
	return true;
}

bool MSConnectivity::HasIsoFoot()
{
	if(mp_IsoFoot == NULL)
		return false;
	return true;
}

bool MSConnectivity::HasFloorLineMember()
{
	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemberG* pLink = ma_MemPart[iMemPart]->mp_LinkMem;
		if(ma_MemPart[iMemPart]->mp_LinkMem->IsFloorMember())
			return true;
	}

	return false;
}

bool MSConnectivity::HasWallMember()
{
	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemberG* pLink = ma_MemPart[iMemPart]->mp_LinkMem;
		if(ma_MemPart[iMemPart]->mp_LinkMem->IsVerticalMember())
			return true;
	}

	return false;
}

bool MSConnectivity::HasMemberType(int eType)
{
	if(mp_Column != NULL && mp_Column->GetType() == eType)
		return true;
	
	if(mp_IsoFoot != NULL && mp_IsoFoot->GetType() == eType)
		return true;

	long NumMemPart = ma_MemPart.size();
	for(long iMemPart = 0 ; iMemPart < NumMemPart ; iMemPart++)
	{
		MSLinkMemberG* pLink = ma_MemPart[iMemPart]->mp_LinkMem;
		if(pLink->GetType() == eType)
			return true;
	}

	return false;
}

bool MSConnectivity::HasMidJointBelow()
{
	if(HasFloorLineMember())
	{
		long NumDownLinkJoint = ma_DownLinkJoint.size();
		for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
		{
			MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
			if(pJoint == NULL || pJoint->IsDeleted())
				continue;
			bool bRval = pJoint->HasMidJointBelow();
			if(bRval == true)
				return true;
		}
// 		if(mp_DownLinkJoint != NULL)
// 		{
// 			return mp_DownLinkJoint->HasMidJointBelow();
// 		}
	}
	else if(HasWallMember())
	{
		return IsMidConn();
	}

	return false;
}

bool MSConnectivity::HasMidJointOn()
{
	if(HasFloorLineMember())
	{
		return IsMidConn();
	}
	else if(HasWallMember())
	{
		long NumUpLinkJoint = ma_UpLinkJoint.size();
		for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
		{
			MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
			if(pJoint == NULL || pJoint->IsDeleted())
				continue;
			bool bRval = pJoint->HasMidJointOn();
			if(bRval)
				return true;
		}
// 		if(mp_UpLinkJoint != NULL)
// 		{
// 			return mp_UpLinkJoint->HasMidJointOn();
// 		}
	}

	return false;
}

vector<MSConnectivity*> MSConnectivity::GetUppperFloorHitVerticalJoint()
{
	//MSConnectivity* pUpJoint = NULL;
	if(mp_Floor != NULL)
	{
		MSFloor* pUpFloor = mp_Floor->GetUpperFloor();
		if(pUpFloor == NULL) return ma_UppperFloorHitVerJoit;

		//VerticalMemPart의 조인트만 찾는다.
		//ma_UppperFloorHitVerJoit = pUpFloor->FindHitJoints(this, -1);
		//mp_UppperFloorHitVerJoit = pUpFloor->FindHitJoint(this, -1);

	}
	return ma_UppperFloorHitVerJoit;
}

MSConnectivity* MSConnectivity::GetLowerFloorHitFloorJoint()
{
	MSConnectivity* pFloorJoint = NULL;
	if(mp_Floor != NULL)
	{
		MSFloor* pLowerFloor = mp_Floor->GetLowerFloor();
		if(pLowerFloor == NULL) return NULL;

		//FloorMemPart의 조인트만 찾는다.
		//pFloorJoint = pLowerFloor->FindHitJoint(this, 1);
	}
	return pFloorJoint;
}

void MSConnectivity::LowFloorJointHandRailTrim()
{
	// 수직 부재 조인트이면 아래 층의 LowWall이 포함된 조인트 Level를 찾아서 다시 트림해준다..
	if(HasNoFloorMemPart())
	{
		MSConnectivity* pFloorJoint = GetLowerFloorHitFloorJoint();
		if(pFloorJoint != NULL)
		{
			if(pFloorJoint->HasHandRail())
				pFloorJoint->TrimMembersEx(false);
		}
	}
}

void MSConnectivity::ClearAllDeletedJoint()
{
	// 각 층마다 호출되지만 _deleted_map 이 clear() 되므로 두번째는 효력 없음
	map<MSConnectivity*, bool>::iterator it;
	for(it = _deleted_map.begin(); it != _deleted_map.end(); it++)
		delete it->first;
	_deleted_map.clear();
}

bool MSConnectivity::CheckProfile()
{
	GM2DPolyline* pDefPoly = GetSection_Hit();
	GM2DPolyline* pDrawPoly = GetDrawSection(true);
	
	bool bDefSec = false;
	bool bDrawSec = false;
	
	if(pDefPoly)
		//bDefSec = pDefPoly->CheckVecterArr(false);
		bDefSec = pDefPoly->CheckPolygon(false);
	else
		bDefSec = true;
	
	if(pDrawPoly)
		//bDrawSec = pDrawPoly->CheckVecterArr(false);
	{
		long NumLine = pDrawPoly->GetNumLines();
		if(NumLine > 2)
			bDrawSec = pDrawPoly->CheckPolygon(false);
		else
			bDrawSec = true;
	}
	else
		bDrawSec = true;
	
	if(bDefSec == false || bDrawSec == false)
		return false;
	return true;
}

bool MSConnectivity::Contains(MSCompoG* pUrCompoG, double dTol /*= DTOL_GM*/, bool bIncludeOnLine /*= TRUE */)
{
// 	if(!pUrCompoG->IsLevelOverlap(this))
// 		return false;

	GM2DPolyline* pMyBoundary;
	pMyBoundary = GetSection_Hit();
	if(pMyBoundary == NULL)
		return false;

	GM2DPolyline UrJointPoly(TRUE);
	pUrCompoG->GetBoundary(UrJointPoly);
	
	long nConVal = 1;
	if(bIncludeOnLine)
		nConVal = 1;
	else
		nConVal = 0;

	MSPointMemberG* pPointG = NULL;
	if(mp_Column != NULL)
		pPointG = mp_Column;
	if(mp_IsoFoot != NULL)
		pPointG = mp_IsoFoot;

	if(pUrCompoG->IsLinkMember() && pPointG != NULL)
	{
		MSLinkMemberG* pLinnkMemberG = (MSLinkMemberG*)pUrCompoG;

		long nLRval0 = pMyBoundary->Contains(&pLinnkMemberG->mp_LLine->m_P0);
		long nLRval1 = pMyBoundary->Contains(&pLinnkMemberG->mp_LLine->m_P1);

		if(nLRval0 < nConVal && nLRval1 < nConVal)
			return true;

		long nRRval0 = pMyBoundary->Contains(&pLinnkMemberG->mp_RLine->m_P0);
		long nRRval1 = pMyBoundary->Contains(&pLinnkMemberG->mp_RLine->m_P1);

		if(nRRval0 < nConVal && nRRval1 < nConVal)
			return true;

	}

	long nContainCnt = 0;
	bool isMyAllContain = true;
	long NumMyVec = UrJointPoly.GetSize() -1;
	for(long iVec = 0 ; iVec < NumMyVec ; iVec++)
	{
		long nRval = pMyBoundary->Contains(UrJointPoly.ma_Point[iVec], dTol);
		if(nRval < nConVal)
			nContainCnt++;
		else
			isMyAllContain = false;
	}
	if(NumMyVec > 1 && isMyAllContain == true)
		return true;

	return false;

}

MSCompoG* MSConnectivity::GetFirstCompoG()
{
	if(mp_Column != NULL)
		return mp_Column;
	if(mp_IsoFoot != NULL)
		return mp_IsoFoot;

	long NumMemPart = ma_MemPart.size();
	for(long iPart = 0 ; iPart < NumMemPart ; iPart++)
	{
		MSLinkMemPart* pPart =  ma_MemPart[iPart];
		if(pPart == NULL) continue;
		return pPart->mp_LinkMem;
	}
	return NULL;
}

int MSConnectivity::GetOverlapZoneID()
{
	return mp_Floor->GetOverlapCZoneID(m_dXc, m_dYc);
}

vector<GM2DPolyline*> MSConnectivity::JointPolybyLevelZone()
{
	vector<GM2DPolyline*> PolyArr;

	GM2DPolyline* pJointPoly = new GM2DPolyline(TRUE);
	pJointPoly->CopyFrom(GetSection_Hit());

	MSAssembly* pAssembly = mp_Floor;
	vector<MSLevelZone*> LevelZoneArr = pAssembly->GetLevelZone(pJointPoly);

	long NumLevelZone = LevelZoneArr.size();
	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		MSLevelZone* pLevelZone = LevelZoneArr[iZone];
		GM2DPolyline* pLevelZonePoly = pLevelZone->GetProfile();

		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
		pJointPoly->BoolOperation('I', pLevelZonePoly, JoinedPolys, DTOL_GM);
		long NumJoind = JoinedPolys.GetSize();

		if(NumJoind == 1)
			PolyArr.push_back(JoinedPolys[0]);

		pJointPoly->BoolOperation('A', pLevelZonePoly, JoinedPolys, DTOL_GM);

		NumJoind = JoinedPolys.GetSize();
		if(NumJoind == 1)
			pJointPoly->CopyFrom(JoinedPolys[0]);
	}

	bool HasSamePoly = false;
	long NumPoly = PolyArr.size();
	for(long iPoly = 0 ; iPoly < NumPoly ; iPoly++)
	{
		GM2DPolyline* pPoly = PolyArr[iPoly];
		if(pPoly->SamePolyLine(pJointPoly))
		{
			HasSamePoly = true;
			break;
		}
	}
	if(HasSamePoly == false)
		PolyArr.push_back(pJointPoly);

	return PolyArr;
}

bool MSConnectivity::IsExist_VerticalMember()
{
	if(HasColumn() || HasWallMember())
		return true;
	return false;
}

bool MSConnectivity::IsExist_HorizontalMember()
{
	if(HasIsoFoot() || HasFloorLineMember())
		return true;
	return false;
}

bool MSConnectivity::HasMSType(int msType)
{
	if(mp_Column != NULL)
	{
		int nType = mp_Column->GetType();
		if(msType == nType)
			return true;
	}

	if(mp_IsoFoot != NULL)
	{
		int nType = mp_IsoFoot->GetType();
		if(msType == nType)
			return true;
	}

	long NumMemPart = ma_MemPart.size();
	for(long iPart = 0 ; iPart < NumMemPart ; iPart++)
	{
		MSLinkMemPart* pPart = ma_MemPart[iPart];
		int PartType = pPart->mp_LinkMem->GetType();

		if(msType == PartType)
			return true;

	}

	return false;
}

bool MSConnectivity::IsConnected( MSConnectivity* pOtherConnectivity )
{
	if(this == pOtherConnectivity) return true;
	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_UpLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;
		if(pJoint == pOtherConnectivity)
			return true;
	}

	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;
		if(pJoint == pOtherConnectivity)
			return true;
	}

	return false;
}

MSLinkMemberG* MSConnectivity::GetColinerLinkMember(MSLinkMemberG* pMyLink, double dTolDeg)
{
	long nMyStartOrEnd = 0;/*pMyLink->GetSConn() == this ? 0 : 1;*/
	GM2DCurve* pMyCurve = NULL;
	GM2DCurve* pUrCurve = NULL;
	GM2DLineVector MyLine;
	GM2DLineVector UrLine;

	pMyCurve = pMyLink->GetCenLineVec();
	
	bool bMyLinkArc = false;
	if(pMyCurve->m_ObjType == GT_ARC)
		bMyLinkArc = true;

	double dMyWidth = pMyLink->GetWidth();

	MSElement::Type_MSCompoG eMyType = pMyLink->GetType();

	long NumMemPart = ma_MemPart.size();
	for(long iPart = 0 ; iPart < NumMemPart ; iPart++)
	{
		MSLinkMemPart* pPart = ma_MemPart[iPart];
		if(pPart == NULL) continue;
		MSLinkMemberG* pUrLink = pPart->mp_LinkMem;

		if(pMyLink == pUrLink) continue;

		MSElement::Type_MSCompoG eUrType = pUrLink->GetType();
		if(eMyType != eUrType)
			continue;

		long nUrStartOrEnd = 0;//pUrLink->GetSConn() == this ? 0 : 1;
		pUrCurve = pUrLink->GetCenLineVec();

		bool bUrLinkArc = false;
		if(pUrCurve->m_ObjType == GT_ARC)
			bUrLinkArc = true;

		double dUrWidth = pUrLink->GetWidth();
		double dLineDist = 0;
		GM2DVector TheVec;
		if(nMyStartOrEnd == 0)
			TheVec = pUrCurve->OrthoBaseVector(pMyCurve->m_P0);
		else if(nMyStartOrEnd == 1)
			TheVec = pUrCurve->OrthoBaseVector(pMyCurve->m_P1);
		else
		{
			delete pUrCurve;
			continue;
		}

		if(nUrStartOrEnd == 0)
		{
			pUrCurve->SetP0(TheVec);
			dLineDist = TheVec.Distance(pUrCurve->m_P0);
		}
		else if(nUrStartOrEnd == 1)
		{
			pUrCurve->SetP1(TheVec);
			dLineDist = TheVec.Distance(pUrCurve->m_P1);
		}
		else
		{
			delete pUrCurve;
			continue;
		}

		double dMaxWidth = dMyWidth/2 + dUrWidth/2;
	
		if(bMyLinkArc == bUrLinkArc)
		{
			long nRval = pMyCurve->IsColinear4DTol(pUrCurve, dTolDeg, dMaxWidth);
			if(nRval != 0)
			{
				delete pMyCurve;
				delete pUrCurve;

				return pUrLink;
			}
		}
		else if(bMyLinkArc)
		{
// 			GM2DArc* pArc = (GM2DArc*) pMyCurve;
// 			double dUrDeg = pUrCurve->GetDegree();
// 			double dMyDeg = 0;
// 			if(nMyStartOrEnd == 0)
// 				dMyDeg = pArc->GetStartAngle() * Rad2Deg;
// 			else if(nMyStartOrEnd == 1)
// 				dMyDeg = pArc->GetEndAngle() * Rad2Deg;
// 			else
// 				ASSERT(0);
// 			
// 			double dRad = fabs(dMyDeg - dUrDeg);
// 			if( fabs(dRad - 360) <= dTolDeg || fabs(dRad) <= dTolDeg &&
// 
// 			//if(fabs(fabs(dMyDeg - dUrDeg) - 180) <= dTolDeg &&
// 				dLineDist < dMaxWidth)
// 			{
// 				delete pMyCurve;
// 				delete pUrCurve;
// 
// 				return pUrLink;
//			}
		}
		else if(bUrLinkArc)
		{
// 			GM2DArc* pArc = (GM2DArc*) pUrCurve;
// 			double dMyDeg = pMyCurve->GetDegree();
// 			double dUrDeg = 0;
// 			if(nUrStartOrEnd == 0)
// 				dUrDeg = pArc->GetStartAngle() * Rad2Deg;
// 			else
// 				dUrDeg = pArc->GetEndAngle() * Rad2Deg;
// 			
// 			double dRad = fabs(dMyDeg - dUrDeg);
// 			if( fabs(dRad - 360) <= dTolDeg || fabs(dRad) <= dTolDeg &&
// 			//if(fabs(fabs(dMyDeg - dUrDeg) - 180) <= dTolDeg &&
// 				dLineDist < dMaxWidth)
// 			{
// 				delete pMyCurve;
// 				delete pUrCurve;
// 
// 				return pUrLink;
// 			}
		}

		delete pUrCurve;

	}

	delete pMyCurve;
	return NULL;
}

vector<MSCompoG*> MSConnectivity::GetLinkMemberG(long eGetType)
{
	vector<MSCompoG*> CompoGArr;

	long NumPart = ma_MemPart.size();
	for(long iPart = 0 ; iPart < NumPart ; iPart++)
	{
		MSLinkMemPart* pPart = ma_MemPart[iPart];
		if(pPart == NULL) continue;
		if(eGetType != 0 && pPart->mp_LinkMem->GetType() != eGetType)
			continue;

		bool bHasCompoG = false;
		long NumCompoG = CompoGArr.size();
		for(long iCompoG = 0 ;iCompoG < NumCompoG ; iCompoG++)
		{
			if(CompoGArr[iCompoG] == pPart->mp_LinkMem)
			{
				bHasCompoG = true;
				break;
			}
		}
		if(bHasCompoG == false)
			CompoGArr.push_back(pPart->mp_LinkMem);
	}


	return CompoGArr;
}

bool MSConnectivity::isNotContainColumnColinerLinkMember()
{
	MSColumnG* pColumn = NULL;

	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pJoint = ma_DownLinkJoint[iJoint];
		if(pJoint == NULL || pJoint->IsDeleted())
			continue;

		if(pJoint->mp_Column != NULL)
		{
			pColumn= pJoint->mp_Column;
			break;
		}
	}

	if(pColumn == NULL)
		return false;

	GM2DPolyline ColumnPoly(TRUE);
	pColumn->GetBoundary(ColumnPoly);

	vector<MSCompoG*> BeamGArr = GetLinkMemberG(MSElement::msBeamG);
	long NumBeam = BeamGArr.size();

	bool bRval = false;
	map<long, long> UsedBeamMap;
	map<long, long>::iterator it;
	
	long NumConnLevelZone = ma_LevelZone.GetSize();
	for(long iConLevel = 0 ; iConLevel < NumConnLevelZone ; iConLevel++)
	{
		MSConnLevelZone* pConn = ma_LevelZone[iConLevel];
		UsedBeamMap.insert(pConn->m_ColinerMemMap.begin(), pConn->m_ColinerMemMap.end());
	}

	for(long i = 0 ; i < NumBeam ; i++)
	{
		MSBeamG* pCurBeam = (MSBeamG*)BeamGArr[i];
// 		if(pCurBeam->GetSConn() != this && pCurBeam->GetEConn() != this)
// 			continue;

		it = UsedBeamMap.find(pCurBeam->m_ID);
		if(it != UsedBeamMap.end())
			continue;

		UsedBeamMap.insert(make_pair(pCurBeam->m_ID, pCurBeam->m_ID));

		long nCurLVal = ColumnPoly.OverlapEx(pCurBeam->mp_LLine);
		long nCurRVal = ColumnPoly.OverlapEx(pCurBeam->mp_RLine);

		if(nCurLVal == 0 || nCurRVal == 0/* && nNextLVal > 0 && nNextRVal > 0*/)
			return true;
	}

	return bRval;
}

void MSConnectivity::MakeMemPartEndJointPoly()
{
	long nZone = ma_LevelZone.GetSize();
	for(long iZone=nZone-1; iZone>=0; iZone--)
	{
		MSConnLevelZone* pConnZone = ma_LevelZone[iZone];
		pConnZone->MakeMemPartEndJointPoly2(pConnZone->m_Section, pConnZone->ma_MemPart);
	}
}

bool MSConnectivity::HasUpLinkJoint(MSConnectivity* pJoint)
{
	long NumUpLinkJoint = ma_UpLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumUpLinkJoint ; iJoint++)
	{
		MSConnectivity* pCurJoint = ma_UpLinkJoint[iJoint];
		if(pCurJoint == NULL || pCurJoint->IsDeleted())
			continue;
		if(pCurJoint == pJoint)
			return true;
	}
	return false;
}

bool MSConnectivity::HasDownLinkJoint(MSConnectivity* pJoint)
{
	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pCurJoint = ma_DownLinkJoint[iJoint];
		if(pCurJoint == NULL || pCurJoint->IsDeleted())
			continue;
		if(pCurJoint == pJoint)
			return true;
	}
	return false;
}

MSColumnG* MSConnectivity::GetColumn()
{
	if(mp_Column != NULL)
		return mp_Column;

	long NumDownLinkJoint = ma_DownLinkJoint.size();
	for(long iJoint = 0 ; iJoint < NumDownLinkJoint ; iJoint++)
	{
		MSConnectivity* pDownLinkJoint = ma_DownLinkJoint[iJoint];
		if(pDownLinkJoint == NULL || pDownLinkJoint->IsDeleted())
			continue;
		if(pDownLinkJoint->mp_Column != NULL)
			return pDownLinkJoint->mp_Column;
	}


	return NULL;
}

bool MSConnectivity::JOINT_SERIALIZE()
{
	return false;
	//return true; // mylee - 안정화 될때까지 막는다
}

void MSConnectivity::AddFloorQT(MSFloorQT* pQT)
{
	//pQT->AddJoint(this);

	long NumQT = ma_CrossQT.GetSize();
	for(long iQT = 0 ; iQT < NumQT ; iQT++)
	{
		MSFloorQT* pCurQT = ma_CrossQT[iQT];
		if(pCurQT == pQT)
			return;
	}

	ma_CrossQT.Add(pQT);
}

void MSConnectivity::UpdateZLevel(double dChangZLevel, bool bUpdateTop, bool bUpdateBot)
{
	long NumLevelZone = ma_LevelZone.GetSize();
	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		MSConnLevelZone* pConnZone = ma_LevelZone[iZone];
		if(pConnZone == NULL) continue;
		
		if(HasOnlyFloorMember())
		{
			if(mp_Floor->m_nFloorNum == -1)
			{
				//pConnZone->m_TopLevel += dChangZLevel;
				//pConnZone->m_BotLevel += dChangZLevel;
			}
			else if(mp_Floor->m_nFloorNum == 1)
			{
				pConnZone->m_TopLevel += dChangZLevel;
				pConnZone->m_BotLevel += dChangZLevel;
			}
			else
			{
				if(bUpdateTop == bUpdateBot || mp_Floor->m_nFloorNum > 0) // bUpdateTop == bUpdateBot 경우는 처음 ZLevel이 아닐경우임 또는 1층 이상
				{
					pConnZone->m_TopLevel += dChangZLevel;
					pConnZone->m_BotLevel += dChangZLevel;
				}
			}
		}
		else
		{
			if(mp_Floor->m_nFloorNum == -1)
			{
				//pConnZone->m_TopLevel += dChangZLevel;
				pConnZone->m_BotLevel += dChangZLevel;
			}
			else if(mp_Floor->m_nFloorNum == 1)
			{
				pConnZone->m_TopLevel += dChangZLevel;
				//pConnZone->m_BotLevel += dChangZLevel;
			}
			else
			{
				if(bUpdateTop)
					pConnZone->m_TopLevel += dChangZLevel;
				if(bUpdateBot)
					pConnZone->m_BotLevel += dChangZLevel;
			}
		}
	}
}
