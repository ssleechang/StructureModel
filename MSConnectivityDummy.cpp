#include "StdAfx.h"
#include "MSConnectivityDummy.h"

#include "MSFloor.h"

#include "MSLinkMemberG.h"
#include "MSColumnG.h"
#include "MSIsoFootG.h"
#include "MSJointMemberG.h"

#include "MSSlabG.h"
#include "MSGenStairG.h"

MSConnectivityDummy::MSConnectivityDummy(void)
{
	mp_DefaultSection = new GM2DPolyline(TRUE);
}


MSConnectivityDummy::~MSConnectivityDummy(void)
{
	delete mp_DefaultSection;
}

void MSConnectivityDummy::Serialize(CArchive& ar)
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
			MSLinkMemPartDummy::Serialize_AsID(ar, ma_MemPart[i], dummy);
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
			mp_Source = (MSConnectivityDummy*)ID;
			ar >> count;
			for (int i=0 ; i<count ; ++i)
			{
				ar >> ID;
				ma_Clone.push_back((MSConnectivityDummy*)ID);
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
				//ma_UpLinkJoint_Idx.push_back(nUpLinkJointID);
			}

			long NumDownLinkJoint = 0;
			ar >> NumDownLinkJoint;
			for(long iDownLinkJoint = 0 ; iDownLinkJoint < NumDownLinkJoint ; iDownLinkJoint++)
			{
				long nDownLinkJointID = 0;
				ar >> nDownLinkJointID;
				//ma_DownLinkJoint_Idx.push_back(nDownLinkJointID);
			}

			CTypedPtrArray<CObArray, MSFloorQT*>& temp = ma_CrossQT;

			ar >> count;
			ma_LevelZone.RemoveAll();
			for (int i=0 ; i<count ; ++i)
			{
				MSConnLevelZoneDummy* cur = new MSConnLevelZoneDummy(NULL, 0, 0);
				cur->Serialize(ar);
				ma_LevelZone.Add(cur);
			}
			ar >> count;
			ma_LevelZoneFloor.RemoveAll();
			for (int i=0 ; i<count ; ++i)
			{
				MSConnLevelZoneDummy* cur = new MSConnLevelZoneDummy(NULL, 0, 0);
				cur->Serialize(ar);
				ma_LevelZoneFloor.Add(cur);
			}

			ar >> count;
			ma_MemPart.clear();
			ma_MemPart_Idx.clear();
			for (int i=0 ; i<count ; ++i)
			{
				long idx = 0;
				MSLinkMemPartDummy* part = NULL;
				MSLinkMemPartDummy::Serialize_AsID(ar, part, idx); // NEED TO CONVERT ID->POINTER
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

			if (!MSConnectivityDummy::JOINT_SERIALIZE())
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

		//SetLastID(m_ID);
	}
}

void MSConnectivityDummy::Serialize_AsID(CArchive& ar, MSConnectivityDummy*& con)
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
		con = (MSConnectivityDummy*)ID;
	}
}

bool MSConnectivityDummy::JOINT_SERIALIZE()
{
	return false;
	//return true; // mylee - 안정화 될때까지 막는다
}

MSConnLevelZoneDummy::MSConnLevelZoneDummy(MSConnectivityDummy* pConn, double TopLevel, double BotLevel)
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

MSConnLevelZoneDummy::~MSConnLevelZoneDummy(void)
{
	if(mp_DownLinkJointBoundry != NULL)
		delete mp_DownLinkJointBoundry;
	mp_DownLinkJointBoundry = NULL;
}

void MSConnLevelZoneDummy::Serialize(CArchive& ar)
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
			MSLinkMemPartDummy::Serialize_AsID(ar, ma_MemPart[i], idx_temp);
		}

		count = ma_BuriedMemPart.GetSize();
		ar << count;
		for (int i=0 ; i<ma_BuriedMemPart.GetSize() ; ++i)
		{
			long idx_temp = 0;
			MSLinkMemPartDummy::Serialize_AsID(ar, ma_BuriedMemPart[i], idx_temp);
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
		mp_Conn = (MSConnectivityDummy*)ID; // NEED TO CONVERT ID->POINTER
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
			MSLinkMemPartDummy* part = NULL;
			MSLinkMemPartDummy::Serialize_AsID(ar, part, part_idx);
			ma_MemPart.Add(part); // NEED TO CONVERT ID->POINTER
			ma_MemPart_idx.push_back(part_idx);
		}

		ar >> count;
		ma_BuriedMemPart.RemoveAll(); ma_BuriedMemPart_idx.clear();
		for (int i=0 ; i<count ; ++i)
		{
			long part_idx = 0;
			MSLinkMemPartDummy* part = NULL;
			MSLinkMemPartDummy::Serialize_AsID(ar, part, part_idx);
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

		if (!MSConnectivityDummy::JOINT_SERIALIZE())
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



MSLinkMemPartDummy::MSLinkMemPartDummy(MSLinkMemberG* pLineMember /*= NULL*/)
{
	mp_LinkMem = pLineMember;

	mp_SConn = NULL;
	mp_EConn = NULL;

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
}

MSLinkMemPartDummy::~MSLinkMemPartDummy(void)
{

}

void MSLinkMemPartDummy::Serialize_AsID(CArchive& ar, MSLinkMemPartDummy*& part, long& rIdx)
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
		part = (MSLinkMemPartDummy*)ID; // NEED TO CONVERT ID->POINTER
		rIdx = idx;
	}
}

void MSLinkMemPartDummy::Serialize(CArchive& ar)
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
		MSLinkMemPartDummy::Serialize_AsID(ar, mp_FloorPartUp, idx_dummy);
		MSLinkMemPartDummy::Serialize_AsID(ar, mp_WallPartBelow, idx_dummy);

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
		mp_SConn = (MSConnectivityDummy*)ID; // NEED TO CONVERT ID->POINTER
		ar >> ID;
		mp_EConn = (MSConnectivityDummy*)ID; // NEED TO CONVERT ID->POINTER

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
		for (int i=0 ; i<count ; ++i) ma_StartJointLevel.Add(new MSConnLevelZoneDummy(NULL, 0, 0));
		for (int i=0 ; i<count ; ++i) ma_StartJointLevel[i]->Serialize(ar);

		ar >> count;
		ma_StartJointEndPoly.RemoveAll();
		for (int i=0 ; i<count ; ++i) ma_StartJointEndPoly.Add(NULL);
		for (int i=0 ; i<count ; ++i) GM2DPolyline::SerializeWithNullFlag(ar, ma_StartJointEndPoly[i]);

		ar >> count;
		ma_EndJointLevel.RemoveAll();
		for (int i=0 ; i<count ; ++i) ma_EndJointLevel.Add(new MSConnLevelZoneDummy(NULL, 0, 0));
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

		MSLinkMemPartDummy::Serialize_AsID(ar, mp_FloorPartUp, mp_FloorPartUp_Idx); // NEED TO CONVERT ID->POINTER
		MSLinkMemPartDummy::Serialize_AsID(ar, mp_WallPartBelow, mp_WallPartBelow_Idx); // NEED TO CONVERT ID->POINTER
		ar >> ID;
		mp_LSlab = (MSSlabG*)ID; // NEED TO CONVERT ID->POINTER
		ar >> ID;
		mp_RSlab = (MSSlabG*)ID; // NEED TO CONVERT ID->POINTER

		if(mp_Boundary != NULL) { delete mp_Boundary; mp_Boundary = NULL; }
		if(mp_TrimmedBoundary != NULL) { delete mp_TrimmedBoundary; mp_TrimmedBoundary = NULL; }
		GM2DPolyline::SerializeWithNullFlag(ar, mp_Boundary);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_TrimmedBoundary);

		if (!MSConnectivityDummy::JOINT_SERIALIZE())
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
