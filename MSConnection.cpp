#include "StdAfx.h"
#include "MSConnection.h"

#include "MSCompoG.h"
#include "MSLinkMemberG.h"
#include "MSFloor.h"
#include "GMLib/GMObject.h"
#include "CmdProcess/CmdProcessHelper.h"
#include "GMLib/AcisGeomHelper.h"
#include "MSLinkMemberG.h"
#include "MSBaseBuilding.h"
#include "MSHandRailG.h"
#include "MSSlabG.h"
#include "MSOpening.h"
#include "MSPolylineObject.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

//
//MSConnection::MSConnection()
//{
//	mp_FirstCompoG = NULL;
//	mp_SecondCompoG = NULL;
//	m_nFirstCompoGID = 0;
//	m_nSecondCompoGID = 0;
//	m_dFirstLoc = 0;
//	m_dSecondeLoc = 0;
//	m_dFirstLocTol = 0;
//	m_dSecondeLocTol = 0;
//	m_dInnerDeg = 0;
//	m_dIgnoreDeg = 35;
//}

bool MSConnection::CalculateFirstSecondCompoG(MSCompoG*& pFirstCompoG, MSCompoG*& pSecondCompoG)
{
	if (pFirstCompoG == nullptr || pSecondCompoG == nullptr)
		return false;

	bool isFirstRigidMember = pFirstCompoG->GetRigidMember();
	bool isSecondRigidMember = pSecondCompoG->GetRigidMember();
	if (isFirstRigidMember == isSecondRigidMember)
	{
		if (pFirstCompoG->IsPolyMember() == false && pSecondCompoG->IsPolyMember() == true)
		{
			std::swap(pFirstCompoG, pSecondCompoG);
			return true;
		}
		else if (pFirstCompoG->GetType() == MSElement::msSlabG && pSecondCompoG->GetType() == MSElement::msSlabG)
		{
			GM2DPolyline* pFirstPoly = pFirstCompoG->GetProfile();
			GM2DPolyline* pSecondPoly = pSecondCompoG->GetProfile();
			
			// 안에 포함된 부재가 First야 한다
			if (pFirstPoly->Contains(pSecondPoly))
			{
				std::swap(pFirstCompoG, pSecondCompoG);
				return true;
			}

			if (pFirstCompoG->m_ID < pSecondCompoG->m_ID)
			{
				std::swap(pFirstCompoG, pSecondCompoG);
				return true;
			}
		}
		else
		{
			if (pFirstCompoG->m_ID > pSecondCompoG->m_ID)
			{
				std::swap(pFirstCompoG, pSecondCompoG);
				return true;
			}
		}
	}
	else
	{
		if (isSecondRigidMember == true)
		{
			std::swap(pFirstCompoG, pSecondCompoG);
			return true;
		}
	}
	return false;
}

MSConnection* MSConnection::GetConnection(MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG)
{
	MSConnection* pConnection = NULL;
	CString id = MSConnection::HistoryID(pFirstCompoG, pSecondCompoG);

	pConnection = dynamic_cast<MSConnection*>(CmdProcessHelper::GetHistoryManager()->GetHistoryObject(id));

	if ( (*IGNORE_HISTORY()) )
		pConnection = NULL;

	if (pConnection == NULL)
	{
		MSConnection::CalculateFirstSecondCompoG(pFirstCompoG, pSecondCompoG);
		pConnection = new MSConnection(id, pFirstCompoG, pSecondCompoG);
		CmdProcessHelper::GetHistoryManager()->AddHistoryObject(pConnection);
	}
	else
	{
		if ((pConnection->m_nFirstCompoGID == pFirstCompoG->m_ID) &&
			(pConnection->m_nSecondCompoGID == pSecondCompoG->m_ID))
		{
			pConnection->SetFirstCompoG(pFirstCompoG);
			pConnection->SetSecondCompoG(pSecondCompoG);
		}
		else
		{
			pConnection->SetFirstCompoG(pSecondCompoG);
			pConnection->SetSecondCompoG(pFirstCompoG);
		}
		pFirstCompoG = pConnection->GetFirstCompoG();
		pSecondCompoG = pConnection->GetSecondCompoG();
		if (pConnection->m_SetByUser == false)
		{
			bool isChang = MSConnection::CalculateFirstSecondCompoG(pFirstCompoG, pSecondCompoG);
			if (isChang)
				pConnection->SwitchFirstSecond();
		}

	}
	return pConnection;


}

MSConnection* MSConnection::GetEmptyConnection()
{
	return new MSConnection(CString(), NULL, NULL);
}

MSConnection::MSConnection(CString& historyID, MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG) :MSHistoryObject(historyID)
{
	Initialize(pFirstCompoG, pSecondCompoG);

}

MSConnection::MSConnection(MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG) :MSHistoryObject(CString())
{
	Initialize(pFirstCompoG, pSecondCompoG);
}

MSConnection::~MSConnection()
{
	DeleteIntersects();
	DeleteFiller();
}

void MSConnection::Serialize(CArchive& ar)
{
	MSObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << mp_FirstCompoG->m_ID;
		ar << mp_SecondCompoG->m_ID;
		ar << (int)m_FirstTrimOption;
		ar << (int)m_SecondTrimOption;
		ar << m_ConAngle;
		ar << m_IsParallel;
		ar << m_dFirstLoc;
		ar << m_dSecondeLoc;
		ar << m_dFirstLocTol;
		ar << m_dSecondeLocTol;
		ar << m_dInnerDeg;
		ar << m_dIgnoreDeg;

		int nFirstConPos = m_eFirstConPos;
		int nSecondeConPos = m_eSecondeConPos;
		ar << nFirstConPos;
		ar << nSecondeConPos;
		int count = ma_Intersect.size();
		ar << count;
		for (int i = 0; i < count; ++i)
			GM2DPolyline::SerializeWithNullFlag(ar, ma_Intersect[i]);
		ar << m_SetByUser;
	}
	else
	{
		ar >> m_nFirstCompoGID;
		ar >> m_nSecondCompoGID;
		int opt1 = 0;
		int opt2 = 0;
		ar >> opt1;
		ar >> opt2;
		m_FirstTrimOption = (TRIM_OPTION)opt1;
		m_SecondTrimOption = (TRIM_OPTION)opt2;
		ar >> m_ConAngle;
		ar >> m_IsParallel;
		ar >> m_dFirstLoc;
		ar >> m_dSecondeLoc;
		ar >> m_dFirstLocTol;
		ar >> m_dSecondeLocTol;
		ar >> m_dInnerDeg;
		ar >> m_dIgnoreDeg;
		if(MSVersionInfo::GetCurrentVersion() >= 20150112)
		{
			int nFirstConPos = 0;
			int nSecondeConPos = 0;
			ar >> nFirstConPos;
			ar >> nSecondeConPos;
			m_eFirstConPos = (CON_POS)nFirstConPos;
			m_eSecondeConPos = (CON_POS)nSecondeConPos;
		}

		int count = 0;
		ar >> count;
		for (int i = 0; i < count; ++i)
		{
			GM2DPolyline* poly = NULL;
			GM2DPolyline::SerializeWithNullFlag(ar, poly);
			ma_Intersect.push_back(poly);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150401)
		{
			ar >> m_SetByUser;
			if (m_SetByUser == true)
			{
				if (GetFirstTrimOption() == TO_DEFAULT || GetSecondTrimOption() == TO_DEFAULT)
					m_SetByUser = false;
			}
		}
	}
}

bool MSConnection::RecoverPointerAfterSerialize_ForSubMembers(vector<MSCompoG*>& aSubMember)
{
	map<long, MSCompoG*> mm_CompoG;
	for (int i=0 ; i<aSubMember.size() ; ++i)
		mm_CompoG[aSubMember[i]->m_ID] = aSubMember[i];

	map<long, MSCompoG*>::iterator itr;
	itr = mm_CompoG.find(m_nFirstCompoGID);
	if (itr != mm_CompoG.end())
		mp_FirstCompoG = itr->second;
	else
		return false;

	itr = mm_CompoG.find(m_nSecondCompoGID);
	if (itr != mm_CompoG.end())
		mp_SecondCompoG = itr->second;
	else
		return false;

	// Replace other connection pointer with this
	bool bFirstOne = false;
	for (int ii = 0; ii < 2; ++ii)
	{
		// First / Second 의 Connection 들 중에서 자신과 같은 정보를 가지고 포인터가 다른 것을 자신으로 대체한다
		// 두번째는 같아졌으므로 this == other 에서 continue 된다
		vector<MSConnection*>& list = (ii == 0 ? mp_FirstCompoG->ma_Connections : mp_SecondCompoG->ma_Connections);
		for (int i = 0; i < list.size(); ++i)
		{
			MSConnection* other = list[i];
			if (this == other) continue;
			if (m_nFirstCompoGID == other->m_nFirstCompoGID && m_nSecondCompoGID == other->m_nSecondCompoGID &&
				fabs(m_dFirstLoc - other->m_dFirstLoc) < 1.e-3)
			{
				list[i] = this;
				bool isRemove = CmdProcessHelper::GetHistoryManager()->RemoveHistoryObject(other);
				delete other;
				bFirstOne = true; break;
			}
		}
		if (bFirstOne) break;
	}

	if(MSVersionInfo::GetCurrentVersion() < 20150112)
		CalculateContainedAngle();

	// AddHistoryObject 
	if (bFirstOne)
	{
		CString id = MSConnection::HistoryID(mp_FirstCompoG, mp_SecondCompoG);
		_historyID = id;
		CmdProcessHelper::GetHistoryManager()->AddHistoryObject(this);
	}

	return true;
}

void MSConnection::RecoverPointerAfterCopyNPaste(map<long, MSCompoG*>& OldCompoGMap)
{
	map<long, MSCompoG*>::iterator itr;
	itr = OldCompoGMap.find(m_nFirstCompoGID);
	if (itr != OldCompoGMap.end())
		mp_FirstCompoG = itr->second;
	else
		return;

	itr = OldCompoGMap.find(m_nSecondCompoGID);
	if (itr != OldCompoGMap.end())
		mp_SecondCompoG = itr->second;
	else
		return;

	// Replace other connection pointer with this
	bool bFirstOne = false;
	for (int ii = 0; ii < 2; ++ii)
	{
		// First / Second 의 Connection 들 중에서 자신과 같은 정보를 가지고 포인터가 다른 것을 자신으로 대체한다
		// 두번째는 같아졌으므로 this == other 에서 continue 된다
		vector<MSConnection*>& list = (ii == 0 ? mp_FirstCompoG->ma_Connections : mp_SecondCompoG->ma_Connections);
		for (int i = 0; i < list.size(); ++i)
		{
			MSConnection* other = list[i];
			if (this == other) continue;
			if (m_nFirstCompoGID == other->m_nFirstCompoGID && m_nSecondCompoGID == other->m_nSecondCompoGID &&
				fabs(m_dFirstLoc - other->m_dFirstLoc) < 1.e-3)
			{
				list[i] = this;
				delete other;
				bFirstOne = true; break;
			}
		}
		if (bFirstOne) break;
	}

	if(MSVersionInfo::GetCurrentVersion() < 20150112)
		CalculateContainedAngle();

	// AddHistoryObject 
	if (bFirstOne)
	{
		CString id = MSConnection::HistoryID(mp_FirstCompoG, mp_SecondCompoG);
		_historyID = id;
		CmdProcessHelper::GetHistoryManager()->AddHistoryObject(this);
	}
}

void MSConnection::RecoverPointerAfterSerialize(MSFloor* floor)
{
	if (m_nFirstCompoGID == m_nSecondCompoGID || floor == NULL)
	{
		mp_FirstCompoG = mp_SecondCompoG = NULL;
		return;
	}

	MSBaseBuilding* pBuild = floor->GetBuilding();
	map<long, MSCompoG*>::iterator itr;
	itr = floor->mm_CompoG.find(m_nFirstCompoGID);
	if (itr != floor->mm_CompoG.end())
		mp_FirstCompoG = itr->second;
	else
		mp_FirstCompoG = pBuild->GetCompoGByFloors(m_nFirstCompoGID);

	itr = floor->mm_CompoG.find(m_nSecondCompoGID);
	if (itr != floor->mm_CompoG.end())
		mp_SecondCompoG = itr->second;
	else
		mp_SecondCompoG = pBuild->GetCompoGByFloors(m_nSecondCompoGID);

	if (mp_FirstCompoG == NULL || mp_SecondCompoG == NULL)
	{
		mp_FirstCompoG = mp_SecondCompoG = NULL;
		return;
	}

	// Replace other connection pointer with this
	bool bFirstOne = false;
	for (int ii = 0; ii < 2; ++ii)
	{
		// First / Second 의 Connection 들 중에서 자신과 같은 정보를 가지고 포인터가 다른 것을 자신으로 대체한다
		// 두번째는 같아졌으므로 this == other 에서 continue 된다
		vector<MSConnection*>& list = (ii == 0 ? mp_FirstCompoG->ma_Connections : mp_SecondCompoG->ma_Connections);
		for (int i = 0; i < list.size(); ++i)
		{
			MSConnection* other = list[i];
			if (this == other) continue;
			if (m_nFirstCompoGID == other->m_nFirstCompoGID && m_nSecondCompoGID == other->m_nSecondCompoGID &&
				fabs(m_dFirstLoc - other->m_dFirstLoc) < 1.e-3)
			{
				list[i] = this;
				bool isRemove = CmdProcessHelper::GetHistoryManager()->RemoveHistoryObject(other);
				delete other;
				bFirstOne = true; break;
			}
		}
		if (bFirstOne) break;
	}

	//MSAssembly::Serialize() 함수에서 처리함
// 	if (MSVersionInfo::GetCurrentVersion() < 20160324)
// 		CalculateContainedAngle();

// [BH-1358] m_dInnerDeg 음수인 경우 CalculateContainedAngle(); 호출 하여 다시 계산해야함
//	MSAssembly::Serialize() 함수에서 처리함
// MSCompoG::RecoverPointerAfterSerialize() 함수에서 포인터 셋팅후 재검토함

	// AddHistoryObject 
	if (bFirstOne)
	{
		CString id = MSConnection::HistoryID(mp_FirstCompoG, mp_SecondCompoG);
		_historyID = id;
		CmdProcessHelper::GetHistoryManager()->AddHistoryObject(this);
	}
}

void MSConnection::Dump(EFS::EFS_Buffer& buffer)
{
	MSCompoG* pFirstCompoG = GetFirstCompoG();
	MSCompoG* pSecondCompoG = GetSecondCompoG();
	buffer << &pFirstCompoG;
	buffer << &pSecondCompoG;
	buffer << m_FirstTrimOption;
	buffer << m_SecondTrimOption;
	buffer << m_dFirstLoc;
	buffer << m_dSecondeLoc;
	buffer << m_dFirstLocTol;
	buffer << m_dSecondeLocTol;
	buffer << m_dInnerDeg;
	buffer << m_dIgnoreDeg;
	buffer << m_ConAngle;
	buffer << m_IsParallel;
	buffer << m_SetByUser;

	int NumIntersect = ma_Intersect.size();
	buffer << NumIntersect;
	for (int iIntersect = 0; iIntersect < NumIntersect; iIntersect++)
		ma_Intersect[iIntersect]->Dump(buffer);

	buffer >> m_dPointMemberArea;
}

void MSConnection::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCompoG* pFirstCompoG = nullptr;
	MSCompoG* pSecondCompoG = nullptr;
	buffer >> &pFirstCompoG;
	buffer >> &pSecondCompoG;
	SetFirstCompoG(pFirstCompoG);
	SetSecondCompoG(pSecondCompoG);
	buffer >> m_FirstTrimOption;
	buffer >> m_SecondTrimOption;
	buffer >> m_dFirstLoc;
	buffer >> m_dSecondeLoc;
	buffer >> m_dFirstLocTol;
	buffer >> m_dSecondeLocTol;
	buffer >> m_dInnerDeg;
	buffer >> m_dIgnoreDeg;
	buffer >> m_ConAngle;
	buffer >> m_IsParallel;
	buffer >> m_SetByUser;

	DeleteIntersects();
	int NumIntersect = 0;
	buffer >> NumIntersect;
	for (int iIntersect = 0; iIntersect < NumIntersect; iIntersect++)
	{
		GM2DPolyline* pPoly = new GM2DPolyline();
		pPoly->Recovery(buffer);
		ma_Intersect.push_back(pPoly);
	}

	buffer >> m_dPointMemberArea;
}

void MSConnection::CopyFromMe( MSObject* _pSource, bool bNewID/*=true*/ )
{
	if(_pSource)
	{
		MSConnection* pSource = (MSConnection*)_pSource;
		mp_FirstCompoG = pSource->GetFirstCompoG();
		mp_SecondCompoG = pSource->GetSecondCompoG();
		m_nFirstCompoGID = pSource->m_nFirstCompoGID;
		m_nSecondCompoGID = pSource->m_nSecondCompoGID;
		m_FirstTrimOption = pSource->GetFirstTrimOption();
		m_SecondTrimOption = pSource->GetSecondTrimOption();
		m_dFirstLoc = pSource->m_dFirstLoc;
		m_dSecondeLoc = pSource->m_dSecondeLoc;
		m_dFirstLocTol = pSource->m_dFirstLocTol;
		m_dSecondeLocTol = pSource->m_dSecondeLocTol;
		m_dInnerDeg = pSource->m_dInnerDeg;
		m_dIgnoreDeg = pSource->m_dIgnoreDeg;
		m_IsParallel = pSource->m_IsParallel;
		m_SetByUser = pSource->m_SetByUser;
	}
}

MSConnection::TRIM_OPTION MSConnection::GetTrimOption(MSCompoG* pCompoG)
{
	MSCompoG* pOwnerCompoG = dynamic_cast<MSCompoG*>(pCompoG->GetOwnerCompoG());

	if (mp_FirstCompoG == pCompoG || mp_FirstCompoG == pOwnerCompoG)
		return m_FirstTrimOption;
	else if (mp_SecondCompoG == pCompoG || mp_SecondCompoG == pOwnerCompoG)
		return m_SecondTrimOption;

	return ms_DefaultLinkGOption;
}

MSConnection::TRIM_OPTION MSConnection::GetFirstTrimOption()
{
	return m_FirstTrimOption;
}

MSConnection::TRIM_OPTION MSConnection::GetSecondTrimOption()
{
	return m_SecondTrimOption;
}

void MSConnection::SetFirstTrimOption(TRIM_OPTION Opt)
{
	if (!CheckTrimOption(Opt, m_eFirstConPos, mp_FirstCompoG)) return;

	if(m_FirstTrimOption != Opt) mp_FirstCompoG->m_DoUpdateTrimmedProfile = true;
	
	m_FirstTrimOption = Opt;
}


void MSConnection::SetSecondTrimOption(TRIM_OPTION Opt)
{
	if (!CheckTrimOption(Opt, m_eSecondeConPos, mp_SecondCompoG)) return;
	
	if (m_SecondTrimOption != Opt) mp_SecondCompoG->m_DoUpdateTrimmedProfile = true;

	m_SecondTrimOption = Opt;
}

bool MSConnection::CheckTrimOption(TRIM_OPTION Opt, CON_POS conPos, MSCompoG* trimCompoG)
{
	if (conPos == CON_START || conPos == CON_END)
	{
		if (Opt == TO_ENDCUT)
		{
			// 시작 끝 위치에 Miter,SQUAREOFF가 있는경우 EndCut옵션을 사용할수 없다.
			if (trimCompoG->HasTrimOption(TO_MITER, conPos) || trimCompoG->HasTrimOption(TO_SQUAREOFF, conPos))
				return false;

			// [BH25-90] 시작 끝 위치에 점 부재가 있는 경우 EndCut옵션을 사용할 수 없도록 변경
			vector<MSConnection*> connArr = trimCompoG->GetConnectionByPos(conPos);
			for (int iConn = 0; iConn < connArr.size(); iConn++)
			{
				MSConnection* pConn = connArr[iConn];
				if (pConn != nullptr && pConn->GetPartner(trimCompoG)->IsPointMember())
					return false;
			}
		}
		if (Opt == TO_MITER || Opt == TO_SQUAREOFF)
		{
			vector<MSConnection*> ConArr = trimCompoG->GetConnectionByPos(conPos);
			for (int iConn = 0; iConn < ConArr.size(); iConn++)
			{
				if (this == ConArr[iConn])
					continue;
				if (ConArr[iConn]->GetFirstTrimOption() == TO_ENDCUT)
					ConArr[iConn]->SetFirstTrimOption(TO_ORIGIN);
				if (ConArr[iConn]->GetSecondTrimOption() == TO_ENDCUT)
					ConArr[iConn]->SetSecondTrimOption(TO_ORIGIN);
			}
		}
	}
	return true;
}


double MSConnection::GetLinkMemberLoc(MSCompoG* pCompoG)
{
	if (mp_FirstCompoG == pCompoG)
		return m_dFirstLoc;
	else if (mp_SecondCompoG == pCompoG)
		return m_dSecondeLoc;

	return -100;
}

void MSConnection::SetLinkMemberLoc(MSCompoG* pCompoG, double loc)
{
	if (mp_FirstCompoG == pCompoG)
		m_dFirstLoc = loc;
	else if (mp_SecondCompoG == pCompoG)
		m_dSecondeLoc = loc;
}

double MSConnection::GetLinkMemberLocTol(MSCompoG* pCompoG)
{
	if (mp_FirstCompoG == pCompoG)
		return m_dFirstLocTol;
	else if (mp_SecondCompoG == pCompoG)
		return m_dSecondeLocTol;
	
	return 0;
}

CON_POS MSConnection::GetLinkMemberConPos(MSCompoG* pCompoG)
{
	if (mp_FirstCompoG == pCompoG)
		return m_eFirstConPos;
	else if (mp_SecondCompoG == pCompoG)
		return m_eSecondeConPos;
	
	return CON_UNKNOWN;
}

void MSConnection::SetLinkMemberConPos(MSCompoG* pCompoG, CON_POS pos)
{
	if (mp_FirstCompoG == pCompoG)
		m_eFirstConPos = pos;
	else if (mp_SecondCompoG == pCompoG)
		m_eSecondeConPos = pos;
}

bool MSConnection::IsEndConnection(MSCompoG* pCompoG)
{
// 	bool bRVal = false;
// 	double dMyLoc = GetLinkMemberLoc(pCompoG);
// 	double dMyLocTol = GetLinkMemberLocTol(pCompoG);
// 
// 	
// 	if(fabs(dMyLoc - 0) <= dMyLocTol) // 시작 위치에서 Connection 생성
// 		bRVal = true;
// 	else if(fabs(dMyLoc - 1) <= dMyLocTol) // 끝 위치에서 Connection 생성
// 		bRVal = true;
// 
// 	return bRVal;

	bool bRVal = false;

	if(mp_FirstCompoG == pCompoG)
	{
		if(m_eFirstConPos == CON_START || m_eFirstConPos == CON_END)
			bRVal = true;
	}
	else if(mp_SecondCompoG == pCompoG)
	{
		if(m_eSecondeConPos == CON_START || m_eSecondeConPos == CON_END)
			bRVal = true;
	}

	return bRVal;
}

void MSConnection::AdjustTrimOptionAfter()
{
	if (m_SetByUser) return;

	// Slope member -> All Org setting
	{
		MSLinkMemberG* link1 = dynamic_cast<MSLinkMemberG*>(mp_FirstCompoG);
		MSLinkMemberG* link2 = dynamic_cast<MSLinkMemberG*>(mp_SecondCompoG);
		vector<MSLinkMemberG*> list;
		if (link1 != NULL) list.push_back( link1 );
		if (link2 != NULL) list.push_back( link2 );
		
		bool bSlopeExist = false;
		for (int i=0 ; i<list.size() ; ++i)
		{
			MSLinkMemberG* link = list[i];
			MSWallMemberG* wall = dynamic_cast<MSWallMemberG*>(link);
			if (wall != NULL)
			{
				if ( fabs(wall->m_SOffsetZ - wall->m_SOffsetZ_End) > DTOL_GM ) bSlopeExist = true;
				if ( fabs(wall->m_EOffsetZ - wall->m_EOffsetZ_End) > DTOL_GM ) bSlopeExist = true;
			}
			else
			{
				if ( fabs(link->m_SOffsetZ - link->m_EOffsetZ) > DTOL_GM ) bSlopeExist = true;
			}
		}

		if (bSlopeExist && !IsParallel())
		{
			if (!m_SetByUser)
			{ 
				if (GetFirstTrimOption() != TO_IGNORE)
					SetFirstTrimOption(TO_ORIGIN);
				if (GetSecondTrimOption() != TO_IGNORE)
					SetSecondTrimOption(TO_ORIGIN);
			return;
			}
		}
	}

	if (mp_FirstCompoG->IsPointMember() && mp_SecondCompoG->IsPointMember())	// 기둥 + 기둥
	{
	}
	else // 기둥 + 라인부재 or 라인부재 라인부재
	{
		if (mp_FirstCompoG->IsPointMember() && mp_SecondCompoG->IsLinkMember()) // 기둥 + 라인부재
		{
		}
		else if (mp_SecondCompoG->IsPointMember() && mp_FirstCompoG->IsLinkMember()) // 라인부재 + 기둥
		{
		}
		else if(mp_FirstCompoG->IsLinkMember() && mp_SecondCompoG->IsLinkMember())// 라인 + 라인 부재
		{
			MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
			MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;

			if (pFirstLinkG->IsParallel(pSecondLinkG, 0.5)) // 라인부재 + 라인부재 평행함
			{
			}
			else // 평행하지 않음
			{
				if (mp_FirstCompoG->IsVerticalMember() == mp_SecondCompoG->IsVerticalMember()) // 벽 + 벽 or 보 + 보
				{
				}
				else
				{
					MSCompoG* beam = NULL;
					MSCompoG* wall = NULL;
					TRIM_OPTION* beamOpt = NULL;
					double beam_loc = 0.0, wall_loc = 0.0;
					CON_POS beam_pos=CON_POS::CON_END, wall_pos=CON_POS::CON_END;
					if (mp_FirstCompoG->IsVerticalMember() == FALSE)
					{
						beam = mp_FirstCompoG; wall = mp_SecondCompoG; beamOpt = &m_FirstTrimOption;
						beam_loc = m_dFirstLoc; wall_loc = m_dSecondeLoc; beam_pos = m_eFirstConPos; wall_pos = m_eSecondeConPos;
					}
					else if (mp_SecondCompoG->IsVerticalMember() == FALSE)
					{
						wall = mp_FirstCompoG; beam = mp_SecondCompoG; beamOpt = &m_SecondTrimOption;
						beam_loc = m_dSecondeLoc; wall_loc = m_dFirstLoc; beam_pos = m_eSecondeConPos; wall_pos = m_eFirstConPos;
					}

					if (beam->HasTrimOption(TO_MITER) || beam->HasTrimOption(TO_SQUAREOFF))
						return;

					double dOrthoTol = 30.0;
					bool bWallLineExist = false;
					if (wall_pos == CON_POS::CON_MID)
					{
						int beam_count = 0;
						vector<MSCompoG*> target_beams;
						target_beams.push_back(beam);
						for (int i=0 ; i<beam->ma_Connections.size() ; ++i)
						{
							MSConnection* cur = beam->ma_Connections[i];
							if ( fabs(cur->GetLinkMemberLoc(beam) - beam_loc) > 0.25 ) continue;
							MSCompoG* pPartnerG = cur->GetPartner(beam);
							if (pPartnerG == nullptr) continue;
							if (pPartnerG->GetType() == MSElement::msBeamG)
							{
								target_beams.push_back(pPartnerG);
								double dParallelTol = 10.0;
								if (cur->m_dInnerDeg <= dParallelTol || cur->m_dInnerDeg >= 180 - dParallelTol) { }
								else { beam_count++; break; }
							}
						}
						if (beam_count > 0)
						{
							/////////////////////////////////////////////////////////////////////////////////////////////
							// 해당 위치에서 보와 수직으로 만난 보가 존재하면 : 해당 보들을 벽과 모두 IGNORE
							for (int i=0 ; i<target_beams.size() ; ++i)
							{
								for (int k=0 ; k<target_beams[i]->ma_Connections.size() ; ++k)
								{
									MSConnection* cur = target_beams[i]->ma_Connections[k];
									if (cur->m_SetByUser) continue;
									MSCompoG* pPartnerG = cur->GetPartner(target_beams[i]);
									if (pPartnerG == nullptr) continue;
									if (pPartnerG->GetType() == MSElement::msWallG)
									{
										if (cur->GetFirstCompoG() == target_beams[i]) cur->SetFirstTrimOption(TO_IGNORE);
										if (cur->GetSecondCompoG() == target_beams[i]) cur->SetSecondTrimOption(TO_IGNORE);
									}
								}
							}
							/////////////////////////////////////////////////////////////////////////////////////////
						}
						else
						{
							long eZLevelInfo = pFirstLinkG->GetZLevelInfo(pSecondLinkG, this);
							if(!(eZLevelInfo & ZLevelConnection))
							{
								//if (dOrthoTol <= m_dInnerDeg && m_dInnerDeg <= 180.0 - dOrthoTol)
								//20160126 RH-1658 벽과 보가 중간에서 만났을 경우 보를 EndCut 하지 않는다.
								if (beam_pos != CON_POS::CON_MID)
								{
									if (dOrthoTol <= m_dInnerDeg && m_dInnerDeg <= 180.0 - dOrthoTol && beam_pos != CON_MID)
									{
										if (mp_FirstCompoG == beam)
											SetFirstTrimOption(TO_ENDCUT);
										else if (mp_SecondCompoG == beam)
											SetSecondTrimOption(TO_ENDCUT);
									}
								}
								bWallLineExist = true;
 							}
						}

					}
					else
					{
						// 해당 위치에서 벽과 만난 다른 벽이 1개 있고 Parallel 이면 (톨러런스 10도)
						int wall_count = 0;
						for (int i=0 ; i<wall->ma_Connections.size() ; ++i)
						{
							if (wall_count >= 2) break;
							MSConnection* cur = wall->ma_Connections[i];
							if (cur == this) continue;
							if ( fabs(cur->GetLinkMemberLoc(wall) - wall_loc) > 0.25 ) continue;
							MSCompoG* pPartnerG = cur->GetPartner(wall);
							if (pPartnerG == nullptr) continue;
							if (pPartnerG->GetType() == MSElement::msWallG)
							{
								double dParallelTol = 10.0;
								if (cur->m_dInnerDeg <= dParallelTol || cur->m_dInnerDeg >= 180 - dParallelTol)
									wall_count++;
							}
						}
						if (wall_count == 1)
						{
							long eZLevelInfo = pFirstLinkG->GetZLevelInfo(pSecondLinkG, this);
							if(!(eZLevelInfo & ZLevelConnection))
							{
								if (beam_pos != CON_POS::CON_MID)
								{
									if (dOrthoTol <= m_dInnerDeg && m_dInnerDeg <= 180.0 - dOrthoTol)
									{
										if (mp_FirstCompoG == beam)
											SetFirstTrimOption(TO_ENDCUT);
										else if (mp_SecondCompoG == beam)
											SetSecondTrimOption(TO_ENDCUT);
									}
								}
							}
							bWallLineExist = true;
						}
					}

					////////////////////////////////////////////////////////////////////////////
					// 이 위치에 붙은 보끼리 옵션 처리 (위에서 보 옵션이 변경된 경우만)
					// (Parallel 이 없는 경우는 서로 Org, 하나라도 있으면 서로 Default)
					if (bWallLineExist)
					{
						/*
						int nWallGirderCount = 0;
						vector<MSCompoG*> vBeamConnected;
						for (int i=0 ; i<wall->ma_Connections.size() ; ++i)
						{
							if (nWallGirderCount > 0) break;
							MSConnection* cur = wall->ma_Connections[i];
							if (cur == this) continue;
							if ( ! (cur->m_IsParallel || fabs(cur->GetLinkMemberLoc(wall) - wall_loc) <= 0.25) ) continue;
							if (cur->GetPartner(wall)->GetType() == MSElement::msBeamG)
							{
								if (cur->m_dInnerDeg <= dOrthoTol || cur->m_dInnerDeg >= 180 - dOrthoTol)
									nWallGirderCount++;
								if (dOrthoTol <= m_dInnerDeg && m_dInnerDeg <= 180.0 - dOrthoTol)
									vBeamConnected.push_back( cur->GetPartner(wall) );
							}
						}

						for (int i=0 ; i<vBeamConnected.size() ; ++i)
						{
							MSCompoG* tgt = vBeamConnected[i];
							for (int k=0 ; k<beam->ma_Connections.size() ; ++k)
							{
								if (beam->ma_Connections[k]->GetPartner(beam) == tgt)
								{
									TRIM_OPTION oldOpt = (TRIM_OPTION)beam->ma_Connections[k]->GetTrimOption(tgt);
									beam->ma_Connections[k]->SetFirstTrimOption((nWallGirderCount==0 ? TO_ORIGIN : TO_EXTENDCUT));
									beam->ma_Connections[k]->SetSecondTrimOption((nWallGirderCount==0 ? TO_ORIGIN : TO_EXTENDCUT));
									TRIM_OPTION newOpt = (TRIM_OPTION)beam->ma_Connections[k]->GetTrimOption(tgt);
									bool bChanged = (oldOpt != newOpt);
									if (bChanged)
									{
										tgt->UpdateTrimmedBoundary();
										tgt->UpdateBoundaryVisibilities();
									}
								}
							}
						}
						*/
					}
					////////////////////////////////////////////////////////////////////////////

				}

			}
		}
	}
}

void MSConnection::CalculateContainedAngle()
{
	if (mp_FirstCompoG->IsPolyMember() || mp_SecondCompoG->IsPolyMember())
	{
		m_eFirstConPos = CON_ALL;
		m_eSecondeConPos = CON_ALL;

#pragma region 전이슬래브 + 일반 보 관계
		// 전이 슬래브와 보의 경우 보를 ExtendCut 해줘야한다
		MSSlabG* pSlabG = nullptr;
		MSBeamG* pBeamG = nullptr;
		if (mp_FirstCompoG->GetType() == MSElement::msSlabG)
			pSlabG = dynamic_cast<MSSlabG*>(mp_FirstCompoG);
		if (mp_SecondCompoG->GetType() == MSElement::msSlabG)
			pSlabG = dynamic_cast<MSSlabG*>(mp_SecondCompoG);

		if (mp_FirstCompoG->GetType() == MSElement::msBeamG)
			pBeamG = dynamic_cast<MSBeamG*>(mp_FirstCompoG);
		if (mp_SecondCompoG->GetType() == MSElement::msBeamG)
			pBeamG = dynamic_cast<MSBeamG*>(mp_SecondCompoG);

		if (pSlabG != nullptr && pSlabG->IsTransfer() && pBeamG != nullptr && pBeamG->IsGeneral())
		{
			//pBeamG->GetLongCenCurveTrimmedByTRBoundary();
			auto cenCurve = pBeamG->GetCenCurve();
			GM2DLineVector sLine = pBeamG->GetStartEndLine();
			GM2DLineVector eLine = pBeamG->GetEndEndLine();
			GM2DPolyline* pSlabProfile = pSlabG->GetTrimmedBoundary_Acis();

			long nSVal = pSlabProfile->Contains(&cenCurve->m_P0);
			long nEVal = pSlabProfile->Contains(&cenCurve->m_P1);
			long nLSVal = pSlabProfile->Contains(&pBeamG->mp_LLine->m_P0);
			long nLEVal = pSlabProfile->Contains(&pBeamG->mp_LLine->m_P1);
			long nRSVal = pSlabProfile->Contains(&pBeamG->mp_RLine->m_P0);
			long nREVal = pSlabProfile->Contains(&pBeamG->mp_RLine->m_P1);

			if (nLSVal < 1)
				nSVal = nLSVal;
			if (nLEVal < 1)
				nEVal = nLEVal;
			if (nRSVal < 1)
				nSVal = nRSVal;
			if (nREVal < 1)
				nEVal = nREVal;

			if (pBeamG == mp_FirstCompoG)
			{
				m_eSecondeConPos = CON_ALL;
				if (nSVal < 1 && nEVal < 1)
					m_eFirstConPos = CON_ALL;
				else if (nSVal < 1)
					m_eFirstConPos = CON_START;
				else if ( nEVal < 1)
					m_eFirstConPos = CON_END;
			}
			else if (pBeamG == mp_SecondCompoG)
			{
				m_eFirstConPos = CON_ALL;
				if (nSVal < 1 && nEVal < 1)
					m_eSecondeConPos = CON_ALL;
				else if (nSVal < 1)
					m_eSecondeConPos = CON_START;
				else if (nEVal < 1)
					m_eSecondeConPos = CON_END;
			}
		}
#pragma endregion
	}
	else if (mp_FirstCompoG->IsLinkMember() && mp_SecondCompoG->IsLinkMember())
	{
		MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
		MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;
		CalculateContainedAngle(pFirstLinkG, pSecondLinkG);
	}
	else
	{
		if (mp_FirstCompoG->IsLinkMember())
		{
			MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
			shared_ptr<GM2DCurve> pFirstCurve = pFirstLinkG->GetCenCurve();
			GM2DVector* pVec = mp_SecondCompoG->GetSVec();

			double dLen = pFirstCurve->Length();
			double dSecondWidth = max(mp_SecondCompoG->GetWidth(), mp_SecondCompoG->GetDepth());
			double dLoc = pFirstCurve->OrthoBaseLocValue(*pVec);
			m_dFirstLoc = dLoc;
			m_dFirstLocTol = 0;
			if(dLen)
				m_dFirstLocTol = (dSecondWidth*0.5)/dLen + MSFloor::ms_DefLineHitDistTol/dLen;

			GM2DPolyline Poly(TRUE);
			mp_SecondCompoG->GetBoundary(Poly);

			GM2DLineVector FirstSLine(pFirstLinkG->mp_LLine->m_P0, pFirstLinkG->mp_RLine->m_P0);
			GM2DLineVector FirstELine(pFirstLinkG->mp_LLine->m_P1, pFirstLinkG->mp_RLine->m_P1);
			MSConnection::SetConnPos(m_eFirstConPos, m_dFirstLoc, m_dFirstLocTol, FirstSLine, FirstELine, &Poly);
			m_eSecondeConPos = CON_ALL;
		}
		else if (mp_SecondCompoG->IsLinkMember())
		{
			MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;
			shared_ptr<GM2DCurve> pSecondCurve = pSecondLinkG->GetCenCurve();
			GM2DVector* pVec = mp_FirstCompoG->GetSVec();

			double dLen = pSecondCurve->Length();
			double dFirstWidth = max(mp_FirstCompoG->GetWidth(), mp_FirstCompoG->GetDepth());
			double dLoc = pSecondCurve->OrthoBaseLocValue(*pVec);
			m_dSecondeLoc = dLoc;
			if(dLen > 0)
				m_dSecondeLocTol = (dFirstWidth*0.5) / dLen + MSFloor::ms_DefLineHitDistTol/dLen;

			GM2DPolyline Poly(TRUE);
			mp_FirstCompoG->GetBoundary(Poly);

			GM2DLineVector SecondSLine(pSecondLinkG->mp_LLine->m_P0, pSecondLinkG->mp_RLine->m_P0);
			GM2DLineVector SecondELine(pSecondLinkG->mp_LLine->m_P1, pSecondLinkG->mp_RLine->m_P1);
			MSConnection::SetConnPos(m_eSecondeConPos, m_dSecondeLoc, m_dSecondeLocTol, SecondSLine, SecondELine, &Poly);
			m_eFirstConPos = CON_ALL;
		}
		else
		{
			m_eFirstConPos = CON_ALL;
			m_eSecondeConPos = CON_ALL;
		}
	}
}

void MSConnection::CalculateLinkMemberTrimOption()
{
	if (m_SetByUser) 
		return;

	if (SetSteelMemberTrimOption())
		return;

	// [BH-2978] 평행 하지 않은 헌치 옵션 강제 변경 무조건 Miter로 셋팅함, 헌치가 한곳에 3개 붙는 경우는 발생하지 않는다
	if (IsParallel() == false &&
		(mp_FirstCompoG->GetType() == MSElement::msSubHaunchG || mp_SecondCompoG->GetType() == MSElement::msSubHaunchG))
	{
		MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
		MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;
		if (pFirstLinkG->isArc() == false && pSecondLinkG->isArc() == false)
		{
			SetFirstTrimOption(MSConnection::TO_MITER);
			SetSecondTrimOption(MSConnection::TO_MITER);
			return;
		}
	}

	if (!mp_FirstCompoG->IsLinkMember() || !mp_SecondCompoG->IsLinkMember())
	{
		SetTrimOption(m_FirstTrimOption, m_SecondTrimOption);
		return;
	}

	//[A22-1457] #comment 마감벽체 트림로직 변경
	if ((mp_FirstCompoG->IsLinkMember() && mp_SecondCompoG->IsLinkMember())
		&& (m_eFirstConPos == CON_START || m_eFirstConPos == CON_END) 
		&& (m_eSecondeConPos == CON_START || m_eSecondeConPos == CON_END))
	{
		MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
		MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;

		double firstLen = pFirstLinkG->GetLength();
		double secondLen = pSecondLinkG->GetLength();
		double maxWidth = max(pFirstLinkG->GetWidth(), pSecondLinkG->GetWidth());
		double minLineLen = min(firstLen, secondLen);

		// 두라인 객체의 두께의 3배보다 작은 길이를 가지고 있는 경우에 
		// 짧은 라인객체의 우선순위를 높게 변경한다.
		if (minLineLen < maxWidth * 3 && secondLen < firstLen)
			SwitchFirstSecond();
	}

	TRIM_OPTION FirstTrimOption = m_FirstTrimOption;
	TRIM_OPTION SecondTrimOption = m_SecondTrimOption;

	MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
	MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;

	bool isFirstCustomM = pFirstLinkG->IsCustomM();
	bool isSecondCustomM = pSecondLinkG->IsCustomM();
	
	bool hasFirstCompoGMiterOpt = pFirstLinkG->HasTrimOption(MSConnection::TO_MITER);
	bool hasSecondCompoGMiterOpt = pSecondLinkG->HasTrimOption(MSConnection::TO_MITER);

	if (isFirstCustomM == true || isSecondCustomM == true) // 이형 라인부재가 존재하는 경우
	{
		if (isFirstCustomM == true && isSecondCustomM == true
			&& ((hasFirstCompoGMiterOpt == false && hasSecondCompoGMiterOpt == false) 
				|| (FirstTrimOption == MSConnection::TO_MITER && SecondTrimOption == MSConnection::TO_MITER))
			)
		{
			if (m_eFirstConPos != CON_MID && m_eSecondeConPos != CON_MID && IsParallel() == false)
			{
				SetFirstTrimOption(MSConnection::TO_MITER);
				SetSecondTrimOption(MSConnection::TO_MITER);
			}
			else
			{
				SetFirstTrimOption(MSConnection::TO_ORIGIN);
				SetSecondTrimOption(MSConnection::TO_ORIGIN);
			}
		}
		else
		{
			if (isFirstCustomM == true && isSecondCustomM == false)
			{
				SetFirstTrimOption(MSConnection::TO_IGNORE);
				if(m_eSecondeConPos == CON_MID)
					SetSecondTrimOption(MSConnection::TO_ORIGIN);
				else
					SetSecondTrimOption(MSConnection::TO_ENDCUT);
			}
			else if (isSecondCustomM == true && isFirstCustomM == false)
			{
				SetSecondTrimOption(MSConnection::TO_IGNORE);
				if(m_eFirstConPos == CON_MID)
					SetFirstTrimOption(MSConnection::TO_ORIGIN);
				else
					SetFirstTrimOption(MSConnection::TO_ENDCUT);
			}
			else
			{
				if (m_eFirstConPos == CON_MID)
					SetFirstTrimOption(MSConnection::TO_ORIGIN);
				else
				{
					if (hasFirstCompoGMiterOpt == true)
						SetFirstTrimOption(MSConnection::TO_IGNORE);
					else
						SetFirstTrimOption(MSConnection::TO_ENDCUT);
				}

				if (m_eSecondeConPos == CON_MID)
				{
					SetSecondTrimOption(MSConnection::TO_ORIGIN);
				}
				else
				{
					if (hasSecondCompoGMiterOpt == true)
						SetSecondTrimOption(MSConnection::TO_IGNORE);
					else
						SetSecondTrimOption(MSConnection::TO_ENDCUT);
				}
			}
		}
	}
	else
	{
		// PC 부재 Ignore 처리 (수평/수평, 수직/수직인 경우만 처리하고 리턴)
		if (mp_FirstCompoG->GetRigidMember() || mp_SecondCompoG->GetRigidMember())
		{
			if (mp_FirstCompoG->IsVerticalMember() == mp_SecondCompoG->IsVerticalMember())
			{
				if (mp_FirstCompoG->GetRigidMember() && GetFirstTrimOption() == TO_DEFAULT)
					SetFirstTrimOption(TO_ORIGIN);
				else if (FirstTrimOption == TO_DEFAULT)
					SetFirstTrimOption(ms_DefaultLinkGOption);

			if (mp_SecondCompoG->GetRigidMember() && GetSecondTrimOption() == TO_DEFAULT)
				SetSecondTrimOption(TO_ORIGIN);
			else if (SecondTrimOption == TO_DEFAULT) 
				SetSecondTrimOption(ms_DefaultLinkGOption);
		}
		else
		{
			if (mp_FirstCompoG->GetRigidMember() == true && mp_SecondCompoG->GetRigidMember() == true)
			{
				SetFirstTrimOption(TO_IGNORE);
				SetSecondTrimOption(TO_IGNORE);
			}
			else
			{
				if (mp_FirstCompoG->GetRigidMember())
				{
					SetFirstTrimOption(TO_IGNORE);
					if(IsParallel())
						SetSecondTrimOption(TO_IGNORE);
					else
						SetSecondTrimOption(TO_ORIGIN);
				}
				if (mp_SecondCompoG->GetRigidMember())
				{
					SetSecondTrimOption(TO_IGNORE);
					if (IsParallel())
						SetFirstTrimOption(TO_IGNORE);
					else
						SetFirstTrimOption(TO_ORIGIN);
					
				}
			}
		}
		return;
	}

		if (pFirstLinkG->isArc() || pSecondLinkG->isArc()) // 1) 둘중 하나라도 아크인 경우
		{
			if (mp_FirstCompoG->IsVerticalMember() != mp_SecondCompoG->IsVerticalMember()) // 1-1) 수직 + 수평부재 만남
			{
				// 아크벽에 직선보가 붙는 경우 아니면 Ignore
				SetFirstTrimOption(TO_IGNORE);
				SetSecondTrimOption(TO_IGNORE);

				if (mp_FirstCompoG->IsVerticalMember() && (pFirstLinkG->isArc() && !pSecondLinkG->isArc()))
				{
					if (IsCase_LineExtendToArc(pSecondLinkG, pFirstLinkG))
					{
						SetFirstTrimOption(TO_IGNORE);
						SetSecondTrimOption(TO_ENDCUT);
					}
				}
				if (mp_SecondCompoG->IsVerticalMember() && (pSecondLinkG->isArc() && !pFirstLinkG->isArc()))
				{
					if (IsCase_LineExtendToArc(pFirstLinkG, pSecondLinkG))
					{
						SetFirstTrimOption(TO_ENDCUT);
						SetSecondTrimOption(TO_IGNORE);
					}
				}
			}
			else // 1-2) 수직+수직, 수평+수평
			{
				if (pFirstLinkG->isArc() && pSecondLinkG->isArc())
				{
					SetFirstTrimOption(TO_ORIGIN);
					SetSecondTrimOption(TO_ORIGIN);
				}
				else if (pFirstLinkG->isArc() && !pSecondLinkG->isArc())
				{
					SetFirstTrimOption(TO_ORIGIN);
					if (IsCase_LineExtendToArc(pSecondLinkG, pFirstLinkG))
						SetSecondTrimOption(TO_EXTENDCUT);
					else
						SetSecondTrimOption(TO_ORIGIN);
				}
				else if (pSecondLinkG->isArc() && !pFirstLinkG->isArc())
				{
					SetSecondTrimOption(TO_ORIGIN);
					if (IsCase_LineExtendToArc(pFirstLinkG, pSecondLinkG))
						SetFirstTrimOption(TO_EXTENDCUT);
					else
						SetFirstTrimOption(TO_ORIGIN);
				}
			}
		}
		else if (!pSecondLinkG->isArc() && !pFirstLinkG->isArc()) // 2) 모두 직선인 경우
		{
			bool miter_case = false;
			if (mp_FirstCompoG->IsVerticalMember() == mp_SecondCompoG->IsVerticalMember())
			{
				if (mp_FirstCompoG->HasRightAngleConnection() == false && mp_SecondCompoG->HasRightAngleConnection() == false)
				{
					GM2DVector v1 = pFirstLinkG->GetCenLVec().UnitVector();
					GM2DVector v2 = pSecondLinkG->GetCenLVec().UnitVector();
					double dist1 = __min(pFirstLinkG->GetSVec()->Distance(*pSecondLinkG->GetSVec()), pFirstLinkG->GetEVec()->Distance(*pSecondLinkG->GetEVec()));
					double dist2 = __min(pFirstLinkG->GetSVec()->Distance(*pSecondLinkG->GetEVec()), pFirstLinkG->GetEVec()->Distance(*pSecondLinkG->GetSVec()));
					bool same_dir = (dist2 < dist1);
					double inner = v1.m_X*v2.m_X + v1.m_Y*v2.m_Y;
					double TOL = 30;
					if (same_dir && inner > cos(TOL / 180.0*PI))
						miter_case = true;
					if (!same_dir && inner < -cos(TOL / 180.0*PI))
						miter_case = true;
					//if ( fabs(m_dInnerDeg) < TOL || (180-TOL < m_dInnerDeg && m_dInnerDeg < (180+TOL)) )
					//	miter_case = true;
				}
			}

			if (pFirstLinkG->IsParallel(pSecondLinkG, 0.5)) // 라인부재 + 라인부재 평행함
			{

				if ((mp_FirstCompoG->IsVerticalMember() == mp_SecondCompoG->IsVerticalMember()) &&
					mp_FirstCompoG->GetType() == mp_SecondCompoG->GetType()) // 벽 + 벽 or 보 + 보
				{
					SetFirstTrimOption(TO_ORIGIN);
					SetSecondTrimOption(TO_ORIGIN);
				}
				else
				{
					SetFirstTrimOption(TO_IGNORE);
					SetSecondTrimOption(TO_IGNORE);
				}
			}
			else if (miter_case)
			{
				if (FirstTrimOption == TO_DEFAULT) SetFirstTrimOption(TO_MITER);
				if (SecondTrimOption == TO_DEFAULT) SetSecondTrimOption(TO_MITER);
			}
			else // 평행하지 않음
			{
				if (mp_FirstCompoG->IsVerticalMember() == mp_SecondCompoG->IsVerticalMember()) // 벽 + 벽 or 보 + 보
				{
					if (SecondTrimOption == TO_DEFAULT &&
						m_eSecondeConPos == CON_POS::CON_MID && (m_eFirstConPos == CON_POS::CON_END || m_eFirstConPos == CON_POS::CON_START))
					{
					if (FirstTrimOption == TO_DEFAULT) SetFirstTrimOption(ms_DefaultLinkGOption);
						SetSecondTrimOption(ms_DefaultLinkGOption);
						SwitchFirstSecond();
					}
					else if (SecondTrimOption == TO_DEFAULT &&
						pSecondLinkG->HasOverLapWall() && pFirstLinkG->HasOverLapWall() == false)
					{
					if (FirstTrimOption == TO_DEFAULT) SetFirstTrimOption(ms_DefaultLinkGOption);
						SetSecondTrimOption(ms_DefaultLinkGOption);
						SwitchFirstSecond();
					}
					else
					{
						if (FirstTrimOption == TO_DEFAULT) SetFirstTrimOption(ms_DefaultLinkGOption);
						if (SecondTrimOption == TO_DEFAULT) SetSecondTrimOption(ms_DefaultLinkGOption);

						// 첫번째 부재의 트림옵션이 endCut으로 설정되지 않았고 첫번째 부재에 EndCut 옵션이 존재하는 경우 첫번째 부재의 트림옵션을 Orgin으로 처리 한다,
						if (FirstTrimOption != TRIM_OPTION::TO_ENDCUT && mp_FirstCompoG->HasTrimOption(TRIM_OPTION::TO_ENDCUT, m_eFirstConPos))
							SetFirstTrimOption(TO_ORIGIN);
						// 두번째 부재의 트림옵션이 endCut으로 설정되지 않았고 두번째 부재에 EndCut 옵션이 존재하는 경우 두번째 부재의 트림옵션을 Orgin으로 처리 한다,
						if (SecondTrimOption != TRIM_OPTION::TO_ENDCUT && mp_SecondCompoG->HasTrimOption(TRIM_OPTION::TO_ENDCUT, m_eSecondeConPos))
							SetSecondTrimOption(TO_ORIGIN);

						if ((mp_FirstCompoG->GetType() == MSElement::msHandRailG && mp_SecondCompoG->GetType() == MSElement::msHandRailG) &&
							(m_eFirstConPos != CON_MID && m_eSecondeConPos != CON_MID))
						{
							if (!m_SetByUser) {
								SetFirstTrimOption(TO_MITER);
								SetSecondTrimOption(TO_MITER);
							}
						}
					}

					// 양끝에서 만나고 두께(폭)가 두꺼우면 트림우선순위가 높다.
					if (m_eFirstConPos != CON_POS::CON_MID && m_eSecondeConPos != CON_POS::CON_MID)
					{
						if(mp_FirstCompoG->GetWidth() < mp_SecondCompoG->GetWidth())
							SwitchFirstSecond();


						// 내각이 15보다 작으면 컷 옵션 적용
						if (m_dInnerDeg < 15)
						{
							SetFirstTrimOption(TO_ORIGIN);
							SetSecondTrimOption(TO_ENDCUT);
						}
					}
				}
				else
				{
					if (mp_FirstCompoG->HasTrimOption(TO_MITER, m_eFirstConPos))
					{
						SetFirstTrimOption(TO_ORIGIN);
						SetSecondTrimOption(TO_ORIGIN);
					}
					else if (mp_SecondCompoG->HasTrimOption(TO_MITER, m_eSecondeConPos))
					{
						SetFirstTrimOption(TO_ORIGIN);
						SetSecondTrimOption(TO_ORIGIN);
					}
					else
					{
						if (mp_FirstCompoG->IsVerticalMember())
						{
							SetFirstTrimOption(TO_IGNORE);
							SetSecondTrimOption(TO_ORIGIN);
						}
						else if (mp_SecondCompoG->IsVerticalMember())
						{
							SetFirstTrimOption(TO_ORIGIN);
							SetSecondTrimOption(TO_IGNORE);
						}
						else
							ASSERT(0);
					}
				}
			}
		}
	}
}

bool MSConnection::IsCase_LineExtendToArc(MSLinkMemberG* compoG_Line, MSLinkMemberG* compoG_Arc)
{
	shared_ptr<GM2DCurve> curve1 = compoG_Line->GetCenCurve();
	shared_ptr<GM2DCurve> curve2 = compoG_Arc->GetCenCurve();
	if (curve1 == NULL || curve2 == NULL) return false;
	if (curve1->m_ObjType == GT_LINE && curve2->m_ObjType == GT_ARC)
	{
		// line 의 양 끝점이 일정 각도 이내에서 만나고 교점 각도가 45도 이내에서 수직으로 만나면 Extend
		GM2DLineVector* line = (GM2DLineVector*)&*curve1;
		GM2DArc* arc = (GM2DArc*)&*curve2;
		double dWidth = compoG_Arc->GetWidth();
		double dArcLeng = arc->Length();
		double ang1 = arc->m_Theta1;
		double ang2 = arc->m_Theta2;
		if (ang2 < ang1) ang2 += 2.0*PI;
		double ang_tol = (ang2 - ang1) * (compoG_Line->GetWidth()*0.5 / dArcLeng);
		for (int i = 0; i<2; ++i)
		{
			GM2DVector point = (i == 0 ? line->GetP0() : line->GetP1());
			double distFromCenter = arc->m_Center.Distance(point);
			double distFromArc = fabs(arc->GetRadius() - distFromCenter);
			if (distFromArc > dWidth / 2.0) continue;
			double dx = (point.m_X - arc->m_Center.m_X) / distFromCenter;
			double dy = (point.m_Y - arc->m_Center.m_Y) / distFromCenter;
			double theta = atan2(dy, dx);
			if (theta < ang1) theta += 2.0*PI;
			if (ang1 + ang_tol < theta && theta < ang2 - ang_tol)
			{
				GM2DVector line_dir = line->DirVector();
				double line_leng = line_dir.Distance(GM2DVector(0, 0));
				double inner = (line_dir.m_X / line_leng)*dx + (line_dir.m_Y / line_leng)*dy;
				if (fabs(inner) < cos(45.0 / 180.0 * PI)) continue;
				return true;
			}
		}
	}
	return false;
}

void MSConnection::SetTrimOption(TRIM_OPTION FirstTrimOption/*=DEFAULT*/, TRIM_OPTION SecondTrimOption/*=DEFAULT*/)
{
	if (m_SetByUser) return;

	if (SetSteelMemberTrimOption())
		return;

	SetFirstTrimOption(FirstTrimOption);
	SetSecondTrimOption(SecondTrimOption);
	m_IsParallel = false;
	CalculateContainedAngle();

	if (mp_FirstCompoG->IsPolyMember() || mp_SecondCompoG->IsPolyMember())	// 면부재 존재
	{
		SetFirstTrimOption(TO_ORIGIN);
		SetSecondTrimOption(TO_ORIGIN);

#pragma region 전이슬래브 + 일반 보 관계
		// 전이 슬래브와 보의 경우 보를 Cut 해줘야한다
		MSSlabG* pSlabG = nullptr;
		MSBeamG* pBeamG = nullptr;
		if (mp_FirstCompoG->GetType() == MSElement::msSlabG)
			pSlabG = dynamic_cast<MSSlabG*>(mp_FirstCompoG);
		if (mp_SecondCompoG->GetType() == MSElement::msSlabG)
			pSlabG = dynamic_cast<MSSlabG*>(mp_SecondCompoG);

		if (mp_FirstCompoG->GetType() == MSElement::msBeamG)
			pBeamG = dynamic_cast<MSBeamG*>(mp_FirstCompoG);
		if (mp_SecondCompoG->GetType() == MSElement::msBeamG)
			pBeamG = dynamic_cast<MSBeamG*>(mp_SecondCompoG);
		
		if (pSlabG != nullptr && pBeamG != nullptr)
		{
			if (pSlabG->IsTransfer() && pBeamG->IsGeneral())
			{
				if (pBeamG == mp_FirstCompoG)
				{
					if (m_eFirstConPos == CON_ALL || m_eFirstConPos == CON_MID)
					{
						SetFirstTrimOption(TO_ORIGIN);
						SetSecondTrimOption(TO_ORIGIN);
					}
					else
					{
						SetFirstTrimOption(TO_ENDCUT);
						SetSecondTrimOption(TO_ORIGIN);
					}
				}
				else if (pBeamG == mp_SecondCompoG)
				{
					if (m_eSecondeConPos == CON_ALL || m_eSecondeConPos == CON_MID)
					{
						SetFirstTrimOption(TO_ORIGIN);
						SetSecondTrimOption(TO_ORIGIN);
					}
					else
					{
						SetFirstTrimOption(TO_ORIGIN);
						SetSecondTrimOption(TO_ENDCUT);
					}
				}
			}
			else
			{
				SetFirstTrimOption(TO_IGNORE);
				SetSecondTrimOption(TO_IGNORE);
			}
		}
#pragma endregion
	}
	else if (mp_FirstCompoG->IsPointMember() && mp_SecondCompoG->IsPointMember())	// 기둥 + 기둥
	{
		SetFirstTrimOption(TO_ORIGIN);
		SetSecondTrimOption(TO_ORIGIN);
	}
	else if (mp_FirstCompoG->IsPointMember() && mp_SecondCompoG->IsLinkMember()) // 기둥 + 라인부재
	{
		SetFirstTrimOption(TO_ORIGIN);
	
		MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;
		if (pSecondLinkG->GetRigidMember() == true)
			SetSecondTrimOption(TO_ORIGIN);
		else
		{
			long eZLevelInfo = pSecondLinkG->GetZLevelInfo(mp_FirstCompoG, this);

			if (eZLevelInfo & ZLevelConnection)
				SetSecondTrimOption(TO_ORIGIN);
			else if (m_eSecondeConPos != CON_MID && (eZLevelInfo & LowUrMember))
			{
				SetSecondTrimOption(TO_ENDCUT);
				if (mp_FirstCompoG->GetType() == MSElement::msColumnG && mp_SecondCompoG->GetType() == MSElement::msHandRailG)
					SetSecondTrimOption(TO_ORIGIN);
			}
			else
				SetSecondTrimOption(ms_DefaultLinkGOption);

			if (mp_FirstCompoG->GetType() == MSElement::msIsoFootG && mp_SecondCompoG->GetType() == MSElement::msWallFootG)
				SetSecondTrimOption(TO_ORIGIN);

			if ((mp_FirstCompoG->GetType() == MSElement::msIsoFootG && mp_SecondCompoG->GetType() == MSElement::msWallG) ||
				(mp_FirstCompoG->GetType() == MSElement::msIsoFootG && mp_SecondCompoG->GetType() == MSElement::msBeamG))
			{
				SetFirstTrimOption(TO_IGNORE);
				SetSecondTrimOption(TO_IGNORE);
			}
		}
	}
	else if (mp_SecondCompoG->IsPointMember() && mp_FirstCompoG->IsLinkMember()) // 라인부재 + 기둥
	{
		SetSecondTrimOption(TO_ORIGIN);

		MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
		if (pFirstLinkG->GetRigidMember())
			SetFirstTrimOption(TO_ORIGIN);
		else
		{
			long eZLevelInfo = pFirstLinkG->GetZLevelInfo(mp_SecondCompoG, this);

			if (eZLevelInfo & ZLevelConnection)
				SetFirstTrimOption(TO_ORIGIN);
			else if (m_eFirstConPos != CON_MID && (eZLevelInfo & LowUrMember))
			{
				SetFirstTrimOption(TO_ENDCUT);
				if (mp_SecondCompoG->GetType() == MSElement::msColumnG && mp_FirstCompoG->GetType() == MSElement::msHandRailG)
					SetFirstTrimOption(TO_ORIGIN);
			}
			else
				SetFirstTrimOption(ms_DefaultLinkGOption);

			if (mp_SecondCompoG->GetType() == MSElement::msIsoFootG && mp_FirstCompoG->GetType() == MSElement::msWallFootG)
				SetFirstTrimOption(TO_ORIGIN);

			if ((mp_SecondCompoG->GetType() == MSElement::msIsoFootG && mp_FirstCompoG->GetType() == MSElement::msWallG) ||
				(mp_SecondCompoG->GetType() == MSElement::msIsoFootG && mp_FirstCompoG->GetType() == MSElement::msBeamG))
			{
				SetFirstTrimOption(TO_IGNORE);
				SetSecondTrimOption(TO_IGNORE);
			}
		}
	}
	else if(mp_FirstCompoG->IsLinkMember() && mp_SecondCompoG->IsLinkMember())// 라인 + 라인 부재
	{
		MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
		MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;

		long eZLevelInfo = pFirstLinkG->GetZLevelInfo(mp_SecondCompoG, this);
		if(eZLevelInfo & ZLevelConnection)
		{
			SetFirstTrimOption(TO_ORIGIN);
			SetSecondTrimOption(TO_ORIGIN);
		}
		else
			CalculateLinkMemberTrimOption();
	}
	

	/*if(mp_FirstCompoG->IsPointMember())
	{
	SetFirstTrimOption(TO_ORIGIN);
	if(mp_SecondCompoG->IsPointMember())
	SetSecondTrimOption(TO_ORIGIN);
	else // second가 Link인 경우
	{
	if(SecondTrimOption == TO_DEFAULT)
	SetSecondTrimOption(ms_DefaultLinkGOption);
	}
	}
	else // first가 Link인 경우
	{
	MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
	if(mp_SecondCompoG->IsPointMember())
	{
	SetSecondTrimOption(TO_ORIGIN);
	SetFirstTrimOption(ms_DefaultLinkGOption);
	}
	else // second가 Link인 경우
	{
	MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;

	//Calculate Contained Angle
	CalculateContainedAngle(pFirstLinkG, pSecondLinkG);

	if(pFirstLinkG->IsParallel(pSecondLinkG))
	{
	m_IsParallel = true;
	SetFirstTrimOption(TO_ORIGIN);
	SetSecondTrimOption(TO_ORIGIN);
	}
	else
	{
	if(mp_FirstCompoG->IsVerticalMember())
	{
	if(mp_SecondCompoG->IsVerticalMember())
	{
	if(FirstTrimOption==TO_DEFAULT) SetFirstTrimOption(ms_DefaultLinkGOption);
	if(SecondTrimOption==TO_DEFAULT) SetSecondTrimOption(ms_DefaultLinkGOption);
	}
	else // first 벽체, second 보인 경우
	{
	//m_FirstTrimOption = TO_ORIGIN | TO_NOHIDDEN;
	SetFirstTrimOption(TO_ORIGIN);
	if(SecondTrimOption==TO_DEFAULT) SetSecondTrimOption(ms_DefaultLinkGOption);
	}
	}
	else // 포스트가 보인 경우
	{
	if(mp_SecondCompoG->IsVerticalMember())
	{
	if(FirstTrimOption==TO_DEFAULT) SetFirstTrimOption(ms_DefaultLinkGOption);
	//m_SecondTrimOption = TO_ORIGIN | TO_NOHIDDEN;
	SetSecondTrimOption(TO_ORIGIN);
	}
	else // first 보, second 보인 경우
	{
	if(FirstTrimOption==TO_DEFAULT) SetFirstTrimOption(ms_DefaultLinkGOption);
	if(SecondTrimOption==TO_DEFAULT) SetSecondTrimOption(ms_DefaultLinkGOption);
	}
	}
	}
	}
	}*/
}

//bool MSConnection::MakeTrimmedBoundary4Colinear()
//{
//	GM2DPolyline MyBoundary(TRUE), UrBoundary(TRUE);
//	mp_FirstCompoG->GetBoundary(MyBoundary);
//	mp_SecondCompoG->GetBoundary(UrBoundary);
//
//	MyBoundary.BoolOperation('I', &UrBoundary, ma_Intersect, DTOL_GM); //MSFloor::ms_LineHitDistTol);
//	long nIntersect = ma_Intersect.size();
//	if (nIntersect < 1)
//		return false;
//
//	MergeIntersectToBoundary();
//	return true;
//}

void MSConnection::CheckIntersect()
{
	// ma_Intersect가 비정상적일경우 삭제 해준다. 
	// ex) 중복점 삭제후, 라인이 1개만 생성됨
	for (long i = ma_Intersect.size() - 1; i >= 0; i--)
	{
		GM2DPolyline* pIntersect = ma_Intersect[i];
		pIntersect->DeleteDuplicatedPoints(DTOL_MODELING);
		BOOL bRVal = pIntersect->CheckPolygon();
		if (bRVal == FALSE)
		{
			delete pIntersect;
			ma_Intersect.erase(ma_Intersect.begin() + i);
		}
	}
}

bool MSConnection::MakeIntersectBoundary()
{
	UpdateTrimOptionsForUserBoundary();
	
	/*
	if(pFirstLinkG->IsParallel(pSecondLinkG))
	{
	MakeTrimmedBoundary4Colinear();
	return true;
	}
	*/
	if (mp_FirstCompoG == nullptr || mp_SecondCompoG == nullptr)
		return false;

	if (GetFirstTrimOption() == TO_IGNORE && GetSecondTrimOption() == TO_IGNORE)
		return false;

	if (GetFirstTrimOption() == TO_MITER || GetSecondTrimOption() == TO_MITER)
		return false;

	GM2DPolyline ThePoly1, ThePoly2;
	GM2DPolyline* pMyBoundary = &ThePoly1;
	GM2DPolyline* pUrBoundary = &ThePoly2;
	MSCompoG::GetMyUrBoundary(mp_FirstCompoG, mp_SecondCompoG, pMyBoundary, pUrBoundary);

// 	GM2DPolyline* pMyBoundary = mp_FirstCompoG->GetTrimmedBoundary();
// 	GM2DPolyline* pUrBoundary = mp_SecondCompoG->GetTrimmedBoundary();



	AcisGeomHelper::DebugStr1.Format(L"ID1_%d_ID2_%d_MakeIntersectBoundary", this->m_nFirstCompoGID, this->m_nSecondCompoGID);

	// 묻힌 부재 리턴
	if (pMyBoundary->Contains(pUrBoundary))
		return true;
	if (pUrBoundary->Contains(pMyBoundary))
		return true;

	DeleteIntersects();

	// 옵션 체크 need to make ExtendCutBoundary
	GM2DPolyline MyExtBoundary(TRUE);
	if ((GetFirstTrimOption() == TO_EXTENDCUT || GetFirstTrimOption() == TO_ENDCUT) && mp_FirstCompoG->IsLinkMember() && m_IsParallel == false)
	{
		MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
		long nExtendType = m_dFirstLoc < 0.5 ? -1 : 1;
		pFirstLinkG->MakeExtendedBoundary(MyExtBoundary, nExtendType);
	}
	else if (mp_FirstCompoG->GetType() == MSElement::msColumnG)
	{
		MyExtBoundary.CopyFrom(pMyBoundary);
	}
	else
		mp_FirstCompoG->GetBoundary(MyExtBoundary);

	GM2DPolyline UrExtBoundary(TRUE);
	if ((GetSecondTrimOption() == TO_EXTENDCUT || GetSecondTrimOption() == TO_ENDCUT) && mp_SecondCompoG->IsLinkMember() && m_IsParallel == false)
	{
		MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;
		long nExtendType = m_dSecondeLoc < 0.5 ? -1 : 1;
		pSecondLinkG->MakeExtendedBoundary(UrExtBoundary, nExtendType);
	}
	else if (mp_SecondCompoG->GetType() == MSElement::msColumnG)
	{
		UrExtBoundary.CopyFrom(pUrBoundary);
	}
	else
		mp_SecondCompoG->GetBoundary(UrExtBoundary);

	// find intersection area
	MyExtBoundary.BoolOperation('I', &UrExtBoundary, ma_Intersect, DTOL_GM); //MSFloor::ms_LineHitDistTol);

	// ma_Intersect의 결과물이 비정상 적일 경우 체크 
	CheckIntersect();

	long nIntersect = ma_Intersect.size();
	if (nIntersect < 1)
		return false;

	for (long iIntersect = 0; iIntersect < nIntersect; iIntersect++)
	{
		GM2DPolyline* pIntersect = ma_Intersect[iIntersect];
		if (FALSE == pIntersect->IsClosed())
			pIntersect->MakeClosedPolyline(DTOL_GM);

// 		pIntersect->SegmentstoMe(&MyBoundary);
// 		pIntersect->SegmentstoMe(&UrBoundary);
	}

	AcisGeomHelper::DebugStr1 = L"";
	return true;
}
//
//bool MSConnection::MakeTrimmedBoundary()
//{
//	return false;
//	if (mp_FirstCompoG->IsLinkMember() && mp_SecondCompoG->IsLinkMember())
//	{
//		MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
//		MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;
//		/*
//				if(pFirstLinkG->IsParallel(pSecondLinkG))
//				{
//				MakeTrimmedBoundary4Colinear();
//				return true;
//				}
//				*/
//
//		GM2DPolyline* pMyBoundary = pFirstLinkG->GetTrimmedBoundary();
//		GM2DPolyline* pUrBoundary = pSecondLinkG->GetTrimmedBoundary();
//
//		// 묻힌 부재 리턴
//		if (pMyBoundary->Contains(pUrBoundary))
//			return true;
//		if (pUrBoundary->Contains(pMyBoundary))
//			return true;
//
//		// 옵션 체크 need to make ExtendCutBoundary
//		GM2DPolyline MyExtBoundary(TRUE);
//		if (GetFirstTrimOption() == TO_EXTENDCUT || GetFirstTrimOption() == TO_EXTENDORIGIN)
//			pFirstLinkG->MakeExtendedBoundary(MyExtBoundary);
//		else
//			pFirstLinkG->GetBoundary(MyExtBoundary);
//
//		GM2DPolyline UrExtBoundary(TRUE);
//		if (GetSecondTrimOption() == TO_EXTENDCUT || GetSecondTrimOption() == TO_EXTENDORIGIN)
//			pSecondLinkG->MakeExtendedBoundary(UrExtBoundary);
//		else
//			pSecondLinkG->GetBoundary(UrExtBoundary);
//
//		if (m_IsParallel)
//			return true;
//
//		// find intersection area
//		MyExtBoundary.BoolOperation('I', &UrExtBoundary, ma_Intersect, DTOL_GM); //MSFloor::ms_LineHitDistTol);
//		long nIntersect = ma_Intersect.size();
//		if (nIntersect < 1)
//			return false;
//
//		MergeIntersectToBoundary();
//		return true;
//	}
//}

/*
bool MSConnection::MakeTrimmedBoundary()
{
bool isFirstLinkMember = mp_FirstCompoG->IsLinkMember();
bool isSecondLinkMember = mp_SecondCompoG->IsLinkMember();
bool isParallelMember = false;
bool bCheckParallel = false;
if(isFirstLinkMember && isSecondLinkMember)
bCheckParallel = true;

if(bCheckParallel)
{
MSLinkMemberG* pFirstLinkG = (MSLinkMemberG*)mp_FirstCompoG;
MSLinkMemberG* pSecondLinkG = (MSLinkMemberG*)mp_SecondCompoG;
if(pFirstLinkG->IsParallel(pSecondLinkG))
isParallelMember = true;
}

GM2DPolyline* pMyBoundary = mp_FirstCompoG->GetTrimmedBoundary();
GM2DPolyline* pUrBoundary = mp_SecondCompoG->GetTrimmedBoundary();

if(pMyBoundary->Contains(pUrBoundary))
return true;
if(pUrBoundary->Contains(pMyBoundary))
return true;

GM2DPolyline MyExtBoundary(TRUE);
GM2DPolyline UrExtBoundary(TRUE);

if(!isParallelMember && isFirstLinkMember)
{
if(GetFirstTrimOption() == EXPAND)
((MSLinkMemberG*)mp_FirstCompoG)->MakeExtendedBoundary(MyExtBoundary);
else if(GetFirstTrimOption() == ORIGIN || GetFirstTrimOption() == DIRECT)
mp_FirstCompoG->GetBoundary(MyExtBoundary);
}
else // 링크 부재:평행 할 경우, 포인터부재 ==> 자신의 바운더리를 사용한다.
mp_FirstCompoG->GetBoundary(MyExtBoundary);

if(!isParallelMember && isSecondLinkMember)
{
if(GetSecondTrimOption() == EXPAND)
((MSLinkMemberG*)mp_SecondCompoG)->MakeExtendedBoundary(UrExtBoundary);
else if(GetSecondTrimOption() == ORIGIN || GetSecondTrimOption() == DIRECT)
mp_SecondCompoG->GetBoundary(UrExtBoundary);
}
else // 링크 부재:평행 할 경우, 포인터부재 ==> 자신의 바운더리를 사용한다.
mp_SecondCompoG->GetBoundary(UrExtBoundary);

// find intersection area
MyExtBoundary.BoolOperation('I', &UrExtBoundary, ma_Intersect, MSFloor::ms_LineHitDistTol);
long nIntersect = ma_Intersect.size();
if (nIntersect<1)
return false;

MergeIntersectToBoundary();
return true;

}
*/


MSCompoG* MSConnection::GetPartner(MSCompoG* pCompoG)
{
	if (pCompoG == nullptr)
		return nullptr;

	if (pCompoG == mp_FirstCompoG)
		return mp_SecondCompoG;
	else if (pCompoG == mp_SecondCompoG)
		return mp_FirstCompoG;

	if (pCompoG->m_ID == m_nFirstCompoGID)
		return mp_SecondCompoG;
	else if (pCompoG->m_ID == m_nSecondCompoGID)
		return mp_FirstCompoG;

	MSCompoG* pOwnerCompoG = (MSCompoG*)pCompoG->GetOwnerCompoG();
	if (pOwnerCompoG == mp_FirstCompoG)
		return mp_SecondCompoG;
	else if (pOwnerCompoG == mp_SecondCompoG)
		return mp_FirstCompoG;

	return NULL;
}

void MSConnection::RemoveInwardWedge_Old(MSCompoG* pCompoG)
{
	if (pCompoG->IsLinkMember() == FALSE)
		return;

	MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
	shared_ptr<GM2DCurve> pCurve = pLinkG->GetCenCurve();
	GM2DPolyline* pMyBoundary = pLinkG->GetTrimmedBoundary();

	vector<double> DistArr;
	long nVertex = pMyBoundary->GetSize();
	for (long iVertex = 0; iVertex < nVertex; iVertex++)
	{
		GM2DVector* pVertex = pMyBoundary->GetAt(iVertex);
		DistArr.push_back(pCurve->Distance(*pVertex, true));
	}

	double PrevDist, NextDist;
	double HalfWidth = pLinkG->GetWidth() / 2.0;
	for (long iVertex = 0; iVertex < nVertex - 1; iVertex++)
	{
		if (fabs(fabs(DistArr[iVertex]) - HalfWidth) > DTOL_GM)
		{
			if (iVertex == 0)
				PrevDist = DistArr[nVertex - 2];
			else
				PrevDist = DistArr[iVertex - 1];
			NextDist = DistArr[iVertex + 1];

			if (fabs(PrevDist - NextDist) < DTOL_GM)
			{
				pMyBoundary->RemovePointBulgeAt(iVertex);
				if (iVertex == 0)
				{
					DistArr.erase(DistArr.begin() + nVertex - 1);
					nVertex--;
				}
				DistArr.erase(DistArr.begin() + iVertex);
				nVertex--;
				iVertex--;

			}
		}
	}
}

void MSConnection::SetBoundaryVisibilities_Old(MSCompoG* pMyCompoG, MSCompoG* pUrCompoG)
{
// 	if ((pMyCompoG->IsLinkMember() == FALSE) || (pUrCompoG->IsLinkMember() == FALSE))
// 		return;
// 
// 	MSLinkMemberG* pMyLinkG = (MSLinkMemberG*)pMyCompoG;
// 	GM2DPolyline* pMyBoundary = pMyLinkG->GetTrimmedBoundary();
// 
// 	MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pUrCompoG;
// 	GM2DPolyline* pUrBoundary = pUrLinkG->GetTrimmedBoundary();
// 
// 	GM2DLineVector UrEdge0(pUrLinkG->mp_LLine->m_P0, pUrLinkG->mp_RLine->m_P0);
// 	GM2DLineVector UrEdge1(pUrLinkG->mp_LLine->m_P1, pUrLinkG->mp_RLine->m_P1);
// 
// 	vector<GM2DCurve*> MySegments;
// 	pMyBoundary->GetSegments(MySegments); // need to delete
// 	/*
// 
// 		long nSeg = MySegments.size();
// 		for (long iSeg =0;iSeg<nSeg;iSeg++)
// 		{
// 		GM2DCurve* pCurrent = MySegments[iSeg];
// 
// 		vector<GM2DVector> CrossVecArr;
// 		vector<double> MyLocArr, UrLocArr;
// 		vector<long> ResutlArr;
// 		///////////////////////////////////////////////////////////////////////////////
// 		// this벡터와  ALineVec벡터의 교차점 벡터 CrossVec와 절대 위치(0.~1.) MyLoc, YourLoc 계산
// 		// return :  0 : 무교차
// 		//          -1 : 외부 1점 교차
// 		//          -2 : 외부 2점 교차
// 		//         -21 : 평행 1점 투영연결
// 		//         -22 : 평행 오버랩 2점 투영연결
// 		//           1 : 1점 내부 교차
// 		//           2 : 2점 내부 교차
// 		//          11 : 1점 내부, 1점 외부 교차
// 		//          21 : 일직선 1점 연결
// 		//          22 : 일직선 오버랩 2점 교차
// 		///////////////////////////////////////////////////////////////////////////////
// 		long Result = pCurrent->IntersectEnd2(&UrEdge0, CrossVecArr, MyLocArr, UrLocArr, ResutlArr, true, DTOL_GM, 1.0, DTOL_GM);
// 		if(Result==1)
// 		pMyBoundary->InsertAtCopy(iSeg, CrossVecArr[0]);
// 		else if(Result==)
// 
// 		long test = pUrBoundary->Contains(&MidPt, DTOL_GM);
// 		if(-1 == test) // in polyline
// 		pMyBoundary->SetDraw(iSeg, 0);
// 		else if (0== test ) // on line
// 		{
// 		if(pUrLinkG->IsPointOnLRLines(MidPt))
// 		pMyBoundary->SetDraw(iSeg,0);
// 		}
// 		delete pCurrent;
// 		}
// 		*/
// 
// 	long nSeg = MySegments.size();
// 	for (long iSeg = 0; iSeg < nSeg; iSeg++)
// 	{
// 		if (false == pMyBoundary->GetVisibility(iSeg))
// 			continue;
// 
// 		GM2DCurve* pCurrent = MySegments[iSeg];
// 		GM2DVector MidPt = pCurrent->PointAtParam(0.5);
// 		long test = pUrBoundary->Contains(&MidPt, DTOL_GM);
// 		if (-1 == test) // in polyline
// 			pMyBoundary->SetVisibility(iSeg, false);
// 		else if (0 == test) // on line
// 		{
// 			if (pUrLinkG->IsPointOnLRLines(MidPt))
// 				pMyBoundary->SetVisibility(iSeg, false);
// 		}
// 		delete pCurrent;
// 	}
// 
// 	int ptc = pMyBoundary->ma_Point.GetCount();
// 	int tdraw = pMyBoundary->GetVisibilityCount(true);
// 	int fdraw = pMyBoundary->GetVisibilityCount(false);
}

void MSConnection::RemoveOutwardWedge_Old(MSCompoG* pCompoG)
{
	if (pCompoG->IsLinkMember() == FALSE)
		return;

	AcisGeomHelper::DebugStr1.Format(L"ID1_%d_RemoveOutwardWedge_Old", pCompoG->m_ID);

	MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
	GM2DPolyline* pMyBoundary = pLinkG->GetTrimmedBoundary();
	shared_ptr<GM2DCurve> pMyCenterCurve = pLinkG->GetCenCurve();

	//// 파트너 찾기
	//MSLinkMemberG* pPartnerLinkG = (MSLinkMemberG*)GetPartner(pCompoG);
	////long PartnerWhichSide = pLinkG->WhichSide(pPartnerLinkG->GetCenVec());

	// Others 찾기
	long nOverlap = pCompoG->ma_Connections.size();
	for (long iOverlap = 0; iOverlap < nOverlap; iOverlap++)
	{
		// Overlap Poly 로 트림
		MSConnection* pOverlap = pCompoG->ma_Connections[iOverlap];
		GM2DPolyline OverlapPoly(TRUE);
		pOverlap->GetOverlapPoly(OverlapPoly);

		//if (pOverlap->GetConAngle()==0)
		//	continue;

		// 예각인 경우만 트림
		//if(pOverlap->GetConAngle()!=ACUTE)
		//	continue;

		for (long jOverlap = 0; jOverlap < nOverlap; jOverlap++)
		{
			if (iOverlap == jOverlap)
				continue;

			MSConnection* pOtherOverlap = pCompoG->ma_Connections[jOverlap];
			MSCompoG* pOtherG = pOtherOverlap->GetPartner(pCompoG);
			if (pOtherG == nullptr || false == pOtherG->IsLinkMember())
				continue;
			MSLinkMemberG* pOtherLinkG = (MSLinkMemberG*)pOtherG;
			GM2DPolyline OppositeRect(TRUE);
			pOtherLinkG->GetOppositeRect(pMyCenterCurve->PointAtParam(0.5), OppositeRect);

			vector<GM2DPolyline*> WedgePoly;
			OppositeRect.BoolOperation('I', &OverlapPoly, WedgePoly, DTOL_GM);
			long nWedge = WedgePoly.size();
			if (nWedge < 1)
				continue;

			GM2DPolyline Wedge(TRUE);
			Wedge.CopyFrom(WedgePoly[0]);
			GMObject::DeleteGMObject(WedgePoly);

			// 삭제 판단
			//long WedgeWhichSide = pLinkG->WhichSide(Wedge.MassCenter());
			//if(PartnerWhichSide != WedgeWhichSide)
			{
				// 잘라내기
				GM2DPolyline* pSubtract = pMyBoundary->GetFirstBoolOperationResult('A', &OppositeRect, DTOL_GM);
				if (pSubtract != NULL)
				{
					pMyBoundary->CopyFrom(pSubtract);
				}
				delete pSubtract;
			}
		}
	}

	AcisGeomHelper::DebugStr1 = L"";
}

void MSConnection::SetConnPos(CON_POS& eConPos, double dLoc, double dLocTol, GM2DLineVector MySLine, GM2DLineVector MyELine, GM2DPolyline* pPoly)
{
	if(dLoc <= 0+ dLocTol )
		eConPos = CON_START;
	else if( dLoc >= 1 - dLocTol )
		eConPos = CON_END;
	else if( dLoc > 0 + dLocTol && dLoc < 1 - dLocTol)
	{
		eConPos = CON_MID;

		vector<GM2DVector> CrossVecs;
		vector<double> MyLocArr;
		vector<double> UrLocArr;
		vector<long> ResultArr;

		long nRVal = pPoly->OverlapEx(&MySLine);
		if(nRVal > 0)
		{
			eConPos = CON_START;
			return;
		}
		nRVal = pPoly->OverlapEx(&MyELine);
		if(nRVal > 0)
		{
			eConPos = CON_END;
			return;
		}
	}
}

void MSConnection::SetConnPos(CON_POS& eConPos, double dMyLoc, double dMyLocTol, double dUrLoc, MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink)
{
	double dCheckDist = MSFloor::ms_DefLineHitDistTol;

	shared_ptr<GM2DCurve> pMyCurve = pMyLink->GetCenCurve();
	shared_ptr<GM2DCurve> pUrCurve = pUrLink->GetCenCurve();

	double dMyLen = pMyCurve->Length();
	double dUrLen = pUrCurve->Length();
	double dMyWidth = pMyLink->GetWidth();
	double dUrWidth = pUrLink->GetWidth();

	// 1 : 진행방향 왼쪽, 0 : 선상, -1, 진행방향 오른쪽
	long nUrWichSide = pMyCurve->WhichSide(pUrCurve->PointAtParam(0.5));

	// 선상에서 교차 하였을 경우
	if (dMyLoc > 0 - dMyLocTol*2 && dMyLoc < 1 + dMyLocTol*2)
	{
		GM2DLineVector MySLine(pMyLink->mp_LLine->m_P0, pMyLink->mp_RLine->m_P0);
		GM2DLineVector MyELine(pMyLink->mp_LLine->m_P1, pMyLink->mp_RLine->m_P1);

		GM2DPolyline MyBoundry(TRUE);
		pMyLink->GetBoundary(MyBoundry);

		GM2DPolyline UrBoundry(TRUE);
		pUrLink->MakeExtendedBoundary(UrBoundry, dUrLoc < 0.5 ? -1 : 1);

		if (dMyWidth > dMyLen - dCheckDist * 2) // [BH-2473],[BH-3791] 짧은 부재인 경우 MID로 처리하여 물량이 음수가 나오는 경우 발생!
		{
			if (dMyLoc < 0.5)
				eConPos = CON_START;
			else
				eConPos = CON_END;
		}
		else
		{
			if (dMyLoc > 0 && dMyLoc < 1 && dMyLocTol * 2 >= 1) // My부재가 Ur부재에 안에 포함되는경우
				eConPos = CON_MID;
			else if (dMyLoc <= 0 + dMyLocTol)
				eConPos = CON_START;
			else if (dMyLoc >= 1 - dMyLocTol)
				eConPos = CON_END;
			else if (dMyLoc > 0 + dMyLocTol && dMyLoc < 1 - dMyLocTol)
			{
				eConPos = CON_MID;

				long nRVal = UrBoundry.OverlapEx(&MySLine);
				if (nRVal > 0)
					eConPos = CON_START;

				nRVal = UrBoundry.OverlapEx(&MyELine);
				if (nRVal > 0)
					eConPos = CON_END;

				if (eConPos == CON_MID)
				{
					MyBoundry.SegmentstoMe(&UrBoundry);
					long NumCurve = MyBoundry.GetNumLines();
					for (long iCurve = 0; iCurve < NumCurve; iCurve++)
					{
						GM2DCurve* pCurve = MyBoundry.GetCurve(iCurve);
						GM2DVector MidVec = pCurve->PointAtParam(0.5);
						double dLen = pCurve->Length();
						delete pCurve;

						if (nUrWichSide == 1 && pMyLink->IsPointOnLLine(MidVec) == false)
							continue;

						if (nUrWichSide == -1 && pMyLink->IsPointOnRLine(MidVec) == false)
							continue;

						double dCurLoc = pMyCurve->OrthoBaseLocValue(MidVec);
						if (dCurLoc < 0.5)
						{
							if (dCurLoc > dMyLoc)
								continue;
							else if (dLen < dCheckDist)
							{
								eConPos = CON_START;
								break;
							}
						}
						else
						{
							if (dCurLoc < dMyLoc)
								continue;
							else if (dLen < dCheckDist)
							{
								eConPos = CON_END;
								break;
							}
						}
					}
				}
			}
		}
	}
	else // 평행또는 비스듬히 만났을 경우 점과점의 거리로 계산
	{
		GM2DVector MySVec = pMyCurve->m_P0;
		GM2DVector MyEVec = pMyCurve->m_P1;

		GM2DVector UrSVec = pUrCurve->m_P0;
		GM2DVector UrEVec = pUrCurve->m_P1;

		double dSSDist = MySVec.Distance(UrSVec);
		double dSEDist = MySVec.Distance(UrEVec);
		double dESDist = MyEVec.Distance(UrSVec);
		double dEEDist = MyEVec.Distance(UrEVec);

		double dMinDist = min(dSSDist, min(dSEDist, min(dESDist, dEEDist)));
		
		if (fabs(dMinDist - dSSDist) < DTOL10  || fabs(dMinDist - dSEDist) < DTOL10)
			eConPos = CON_START;
		else if (fabs(dMinDist - dESDist) < DTOL10 || fabs(dMinDist - dEEDist) < DTOL10)
			eConPos = CON_END;
	}
	
}

void MSConnection::CalculateContainedAngle(MSLinkMemberG* pFirstLinkG, MSLinkMemberG* pSecondLinkG)
{
	m_IsParallel = false;
	shared_ptr<GM2DCurve> pFirstCurve = pFirstLinkG->GetCenCurve();
	shared_ptr<GM2DCurve> pSecondCurve = pSecondLinkG->GetCenCurve();

	// 각도 구하기
	GM2DVector Cross; // = CrossVecs[nIdx];
	long nRval = pFirstLinkG->CheckIntersectLinkMember(pSecondLinkG, Cross, m_dFirstLoc, m_dSecondeLoc, m_dFirstLocTol, m_dSecondeLocTol);
	if (nRval == 0)
	{
// 		m_eFirstConPos = CON_MID;
// 		m_eSecondeConPos = CON_MID;
		return;
	}
	if(nRval == 2) // 평행 2점 교차 (pFirstCurve, pSecondCurve 둘다 라인인 경우에만 평행함)
	{
		if(pFirstCurve->m_ObjType == GT_LINE && pSecondCurve->m_ObjType == GT_LINE)
			m_IsParallel = true;

		if (pFirstCurve->m_ObjType == GT_ARC && pSecondCurve->m_ObjType == GT_ARC)
		{
			GM2DArc* pFirstArc = (GM2DArc*)&*pFirstCurve;
			GM2DArc* pSecondArc = (GM2DArc*)&*pSecondCurve;
			if (fabs(pFirstArc->m_A - pSecondArc->m_A) < DTOL_GM)
				m_IsParallel = true;
		}
	}

	GM2DLineVector FirstSLine(pFirstLinkG->mp_LLine->m_P0, pFirstLinkG->mp_RLine->m_P0);

	GM2DLineVector SecondSLine(pSecondLinkG->mp_LLine->m_P0, pSecondLinkG->mp_RLine->m_P0);
	
	if(m_IsParallel)
	{
		if (m_dFirstLoc < 0.5)
			m_eFirstConPos = CON_START;
		else
			m_eFirstConPos = CON_END;

		if (m_dSecondeLoc < 0.5)
			m_eSecondeConPos = CON_START;
		else
			m_eSecondeConPos = CON_END;
	}
	else
	{
		MSConnection::SetConnPos(m_eFirstConPos, m_dFirstLoc, m_dFirstLocTol, m_dSecondeLoc, pFirstLinkG, pSecondLinkG);
		MSConnection::SetConnPos(m_eSecondeConPos, m_dSecondeLoc, m_dSecondeLocTol, m_dFirstLoc, pSecondLinkG, pFirstLinkG);
	}

// pFirstCurve, pSecondCurve의 길이가 같고 Croos 가 중점이면 길이 값이 0이 나온다.. 버그
// 	GM2DLineVector FirstLine(Cross, pFirstCurve->PointAtParam(0.5));
// 	GM2DLineVector SecondLine(Cross, pSecondCurve->PointAtParam(0.5));
	GM2DLineVector FirstLine;
	GM2DLineVector SecondLine;
	if (pFirstCurve->m_ObjType == GT_LINE)
	{
		FirstLine.m_P0 = pFirstCurve->m_P0;
		FirstLine.m_P1 = pFirstCurve->m_P1;
	}
	else
	{
		FirstLine.m_P0 = ((GM2DArc*)&*pFirstCurve)->m_Center;
		FirstLine.m_P1 = Cross;
	}

	if (pSecondCurve->m_ObjType == GT_LINE)
	{
		SecondLine.m_P0 = pSecondCurve->m_P0;
		SecondLine.m_P1 = pSecondCurve->m_P1;
	}
	else
	{
		SecondLine.m_P0 = ((GM2DArc*)&*pSecondCurve)->m_Center;
		SecondLine.m_P1 = Cross;
	}
	
	if (m_dFirstLoc > 0.5)
		FirstLine.ReverseDirection();
	if (m_dSecondeLoc > 0.5)
		SecondLine.ReverseDirection();

	double dFirstDeg = FirstLine.GetDegree();
	double dSecondDeg = SecondLine.GetDegree();
	if (pFirstCurve->m_ObjType == GT_ARC)
	{
		GM2DArc* pArc = (GM2DArc*)&*pFirstCurve;
		GM2DLineVector RadiusLine;
		RadiusLine.m_P0 = pArc->m_Center;
		RadiusLine.m_P1 = Cross;
		dFirstDeg = RadiusLine.GetDegree();
		dFirstDeg += 90;
	}
	if (pSecondCurve->m_ObjType == GT_ARC)
	{
		GM2DArc* pArc = (GM2DArc*)&*pSecondCurve;
		GM2DLineVector RadiusLine;
		RadiusLine.m_P0 = pArc->m_Center;
		RadiusLine.m_P1 = Cross;
		dSecondDeg = RadiusLine.GetDegree();
		dSecondDeg += 90;
	}

	while(dFirstDeg < 0)
		dFirstDeg += 360;
	while(dFirstDeg > 360)
		dFirstDeg -= 360;
	
	while(dSecondDeg < 0)
		dSecondDeg += 360;
	while(dSecondDeg > 360)
		dSecondDeg -= 360;
	

	double angle = fabs(max(dFirstDeg, dSecondDeg) - min(dFirstDeg, dSecondDeg));
	if (m_IsParallel)
	{
		angle = 180.0 - acos( fabs(FirstSLine.UnitVector()*SecondSLine.UnitVector()) ) * Rad2Deg;
	}
	if (angle > 180)
		angle = 360 - angle;

	// 중간에 교차되는 경우에는 작은값을 사용한다
	if (m_eFirstConPos == CON_MID && m_eSecondeConPos == CON_MID)
	{
		if (angle > 90)
			angle = 180 - angle;
	}

	m_dInnerDeg = angle;

	// 사잇각 enum 설정
	double ATol = DTOL_GM;
	if (angle < ATol)
		m_ConAngle = ZERO;
	else if (angle < 90 - ATol)
		m_ConAngle = ACUTE;
	else if (angle < 90 + ATol)
		m_ConAngle = RIGHT;
	else if (angle < 180 - ATol)
		m_ConAngle = OBTUSE;
	else
		m_ConAngle = STRAIGHT;
}

int MSConnection::GetConAngle()
{
	if (m_ConAngle < 0)
		m_ConAngle = 0;
	return m_ConAngle;
}

void MSConnection::GetOverlapPoly(GM2DPolyline& OverlapPoly)
{
	OverlapPoly.ResetPolyline();
	if (ma_Intersect.size() < 1)
		return;
	OverlapPoly.CopyFrom(ma_Intersect[0]);
}

MSConnection::TRIM_OPTION MSConnection::SetDefaultLinkGOption(TRIM_OPTION DefaultLinkGOption)
{
	TRIM_OPTION CurOption = ms_DefaultLinkGOption;
	ms_DefaultLinkGOption = DefaultLinkGOption;
	return CurOption;
}

void MSConnection::UpdateTrimOptionsForUserBoundary()
{
	MSCompoG* pFistCompoG = this->GetFirstCompoG();
	MSCompoG* pSecondCompoG = this->GetSecondCompoG();

	if (pFistCompoG != nullptr && pFistCompoG->IsUserBoundarySet())
	{
		SetFirstTrimOption(MSConnection::TO_ORIGIN);
	}

	if (pSecondCompoG != nullptr && pSecondCompoG->IsUserBoundarySet())
	{
		SetSecondTrimOption(MSConnection::TO_ORIGIN);
	}


}

void MSConnection::SwitchFirstSecond()
{
	// 부재 입력 수정 삭제에서만 처리한다
	// Undo,Redo 실행에서는 변경할 필요가 없다
	if (CmdProcessHelper::GetHistoryManager()->IsInUndoRedo())
		return;

	auto compoG = mp_FirstCompoG;
	mp_FirstCompoG = mp_SecondCompoG;
	mp_SecondCompoG = compoG;

	auto option = m_FirstTrimOption;
	m_FirstTrimOption = m_SecondTrimOption;
	m_SecondTrimOption = option;

	auto loc = m_dFirstLoc;
	m_dFirstLoc = m_dSecondeLoc;
	m_dSecondeLoc = loc;

	auto tol = m_dFirstLocTol;
	m_dFirstLocTol = m_dSecondeLocTol;
	m_dSecondeLocTol = tol;

	long tmp = m_nFirstCompoGID;
	m_nFirstCompoGID = m_nSecondCompoGID;
	m_nSecondCompoGID = tmp;

	auto opt = m_eFirstConPos;
	m_eFirstConPos = m_eSecondeConPos;
	m_eSecondeConPos = opt;

	mp_FirstCompoG->m_DoUpdateTrimmedProfile = true;
	mp_SecondCompoG->m_DoUpdateTrimmedProfile = true;

	MSFloor::UpdateConnectionCompoGMap[mp_FirstCompoG->m_ID] = mp_FirstCompoG;
	MSFloor::UpdateConnectionCompoGMap[mp_SecondCompoG->m_ID] = mp_SecondCompoG;
}

long MSConnection::GetFirstCompoGID()
{
	if (mp_FirstCompoG != NULL)
		m_nFirstCompoGID = mp_FirstCompoG->m_ID;
	return m_nFirstCompoGID;
}

MSCompoG* MSConnection::GetFirstCompoG()
{
	if(mp_FirstCompoG != nullptr && (mp_FirstCompoG->m_ID != m_nFirstCompoGID))
		m_nFirstCompoGID = mp_FirstCompoG->m_ID;
	return mp_FirstCompoG;
}

long MSConnection::GetSecondCompoGID()
{
	if (mp_SecondCompoG != NULL)
		m_nSecondCompoGID = mp_SecondCompoG->m_ID;
	return m_nSecondCompoGID;
}

MSCompoG* MSConnection::GetSecondCompoG()
{
	if (mp_SecondCompoG != nullptr && (mp_SecondCompoG->m_ID != m_nSecondCompoGID))
		m_nSecondCompoGID = mp_SecondCompoG->m_ID;
	return mp_SecondCompoG;
}

bool MSConnection::CONNECTION_SERIALIZE()
{
	return true;
}

bool* MSConnection::IGNORE_HISTORY()
{
	static bool bIgnoreHisory = false;
	return &bIgnoreHisory;
}

void MSConnection::DeleteIntersects()
{
	for (long iInter = 0; iInter < ma_Intersect.size(); iInter++)
	{
		GM2DPolyline* pPoly = ma_Intersect[iInter];
		delete pPoly;
	}
	ma_Intersect.clear();
}

void MSConnection::DeleteFiller()
{
	for (long iFiller = 0; iFiller < ma_Filler.size(); iFiller++)
	{
		GM2DPolyline* pPoly = ma_Filler[iFiller];
		delete pPoly;
	}
	ma_Filler.clear();
}

CString MSConnection::HistoryID(MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG)
{
	//id1과 id2가 다른 순서로 들어와도 같은 ID를 만든다.
	CString sGUID = pFirstCompoG->GetBuilding()->GetGUIDString();
	int id1 = pFirstCompoG->m_ID;
	int id2 = pSecondCompoG->m_ID;
	if (id1 > id2)
	{
		int tmp = id1; id1 = id2; id2 = tmp;
	}
	CString id;
	id.Format(L"CONN_%s_%d_%d", sGUID, id1, id2);
	return id;
}

void MSConnection::MergeIntersectToBoundary_After(MSCompoG* pCompoG)
{
	GM2DPolyline* pMyTrimmedBoundary = pCompoG->GetTrimmedBoundary();
	int MyTrimOption = this->GetTrimOption(pCompoG);

	for (long iIntersect = 0; iIntersect < ma_Intersect.size(); iIntersect++)
	{
		GM2DPolyline* pIntersect = this->ma_Intersect[iIntersect];
		if (FALSE == pIntersect->IsClosed())
			pIntersect->MakeClosedPolyline(DTOL_GM);

		if (pIntersect->OverlapEx(*pMyTrimmedBoundary, MSFloor::ms_DefLineHitDistTol) == FALSE)
			continue;
	}

	vector<GM2DPolyline*> UnionPoly;
	if (MyTrimOption == MSConnection::TO_ENDCUT)
	{
		MSCompoG* pPartnerG = GetPartner(pCompoG);
		if (pPartnerG == nullptr) return;
		GM2DPolyline* pPartnerPoly = pPartnerG->GetTrimmedBoundary();
		pMyTrimmedBoundary->BoolOperation('A', pPartnerPoly, UnionPoly, DTOL_GM);

		//Area크기 순으로 정렬
		if(UnionPoly.size() > 1)
		{
			sort(UnionPoly.begin(), UnionPoly.end(), 
				[](GM2DPolyline* a, GM2DPolyline* b)->bool {return a->Area() > b->Area();}
			);
		}

		if (UnionPoly.size()>0)
			pMyTrimmedBoundary->CopyFrom(UnionPoly[0]);
		GMObject::DeleteGMObject(UnionPoly);
	}
}

void MSConnection::MergeIntersectToBoundary(MSCompoG* pCompoG)
{
	AcisGeomHelper::DebugStr1.Format(L"ID1_%d_MergeIntersectToBoundary", pCompoG->m_ID);

	GM2DPolyline BoundaryNSubMemberPoly;
	GM2DPolyline* pMyTrimmedBoundary = pCompoG->GetTrimmedBoundary();

	// 난간벽 파라핏을 위한 작업 pSubPoly
	GM2DPolyline* pSubPoly = nullptr;
	if (pCompoG->GetType() == MSElement::msHandRailG)
	{
		pSubPoly = ((MSHandRailG*)pCompoG)->m_pParapetPoly;
	}

	MSCompoG* pParther = GetPartner(pCompoG);
	if (pParther == nullptr)
		return;

	bool isPartherColumnHeader = false;
	if (pParther->IsPointMember())
	{
		MSPointMemberG* pPointG = (MSPointMemberG*)pParther;
		if (pPointG->GetColumnHeaderMID() != 0)
			isPartherColumnHeader = true;
	}

	int MyTrimOption = this->GetTrimOption(pCompoG);

	// dMyLoc은 라인부재일 경우에만 사용한다.
	double dMyLoc = this->GetLinkMemberLoc(pCompoG);

	long nIntersect = this->ma_Intersect.size();
	for (long iIntersect = 0; iIntersect < nIntersect; iIntersect++)
	{
		GM2DPolyline* pIntersect = this->ma_Intersect[iIntersect];
		if (FALSE == pIntersect->IsClosed())
			pIntersect->MakeClosedPolyline(DTOL_GM);

		if (pIntersect->OverlapEx(*pMyTrimmedBoundary, MSFloor::ms_DefLineHitDistTol) == FALSE)
			continue;

		//TDL SegmentstoMe에 다녀오면 닫힌 포리라인이 열린다. 그러므로 이 함수에서 닫힌 포리라인은 닫힌 상태 유지하도록 변경 필요
		vector<GM2DPolyline*> UnionPoly;
		// Union Intersect Area to My Trimmed Boundary

		if (MyTrimOption == MSConnection::TO_ORIGIN || MyTrimOption == MSConnection::TO_EXTENDORIGIN)
		{
			// 인터섹트와 TrimmedBoundary의 교점은 추가해준다.
			//pMyTrimmedBoundary->SegmentstoMe(pIntersect);
			/*
			pMyTrimmedBoundary->BoolOperation('U', pIntersect, UnionPoly, DTOL_GM);
			// mylee - TEMP before jissi fix BoolOperation BUG
			//if ( (UnionPoly.size() > 0 && UnionPoly[0]->Area() >= pMyTrimmedBoundary->Area()) ||
			//	pMyTrimmedBoundary->HasBulge() || pIntersect->HasBulge() ||
			//	UnionPoly.size() > 0 && UnionPoly[0]->ma_Point.GetSize() >= pMyTrimmedBoundary->ma_Point.GetSize())
			{
			pMyTrimmedBoundary->CopyFrom(UnionPoly[0]);
			}
			GMObject::DeleteGMObject(UnionPoly);
			*/
		}
		else if (MyTrimOption == MSConnection::TO_EXTENDCUT || MyTrimOption == MSConnection::TO_ENDCUT)
		{
			vector<GM2DPolyline*> MySplitPoly;

			pMyTrimmedBoundary->BoolOperation('A', pIntersect, MySplitPoly, DTOL_GM);
			long nMySplitPoly = MySplitPoly.size();

			if (nMySplitPoly > 1)
			{
				//인터섹트로 본체를 Subtract해 본다. 이때, 결과가 2개 이상이고 작은 삼각형이 나오면 그것은 제거 대상이다.
				//Mybody가 other body를 살짝 뚫고 지나갔을 때 발생한다. 
				//Area크기 순으로 정렬
				sort(MySplitPoly.begin(), MySplitPoly.end(),
					[](GM2DPolyline* a, GM2DPolyline* b)->bool {return a->Area() > b->Area(); }
				);

				for (long iMySplitPoly = MySplitPoly.size() - 1; iMySplitPoly >= 0; iMySplitPoly--)
				{
					GM2DPolyline* pMySplitPoly = MySplitPoly[iMySplitPoly];
					pMySplitPoly->DeleteInLinePoint();
					if (MySplitPoly[iMySplitPoly]->GetNumLines() == 3)
					{
						pMyTrimmedBoundary->BoolOperation('A', MySplitPoly[iMySplitPoly], UnionPoly, DTOL_GM);
						if (UnionPoly.size() > 0)
							pMyTrimmedBoundary->CopyFrom(UnionPoly[0]);
						else
							ASSERT(0);
						GMObject::DeleteGMObject(UnionPoly);
						GMObject::DeleteGMObject(MySplitPoly);
						break;
					}
				}
			}

			//char oper = (MyTrimOption == MSConnection::TO_ENDCUT ? 'A' : 'U');
			char oper = 'U';
			pMyTrimmedBoundary->BoolOperation(oper, pIntersect, UnionPoly, DTOL_GM);
			bool doExtendTrim = false;
			//doExtendTrim 인터섹트 같은 유니온 결과가 나왔을 경우 트림바운더리를 사용하지 않고 익스텐드 바운더리를 사용한다. //추후 보완 필요 -kkw
			if (UnionPoly.size() > 1 || pCompoG->IsLinkMember())
			{
				for (long i = 0; i < UnionPoly.size(); i++)
				{
					if (UnionPoly[i]->SamePolyLine(pIntersect))
					{
						doExtendTrim = true;
						break;
					}
				}
			}

			double dExtendLen = MSFloor::ms_DefLineHitDistTol *1.3;
			// 			if(GetPartner(pCompoG)->IsLinkMember() == FALSE)
			// 				dExtendLen = DTOL_GM *2;
			if (doExtendTrim)
			{
				MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
				GM2DPolyline ExtendPoly(TRUE);
				long nExtendType = dMyLoc < 0.5 ? -1 : 1;
				pLinkG->MakeExtendedBoundary(ExtendPoly, nExtendType, dExtendLen);

				//char oper = (MyTrimOption == MSConnection::TO_ENDCUT ? 'A' : 'U');
				char oper = 'U';
				ExtendPoly.BoolOperation(oper, pMyTrimmedBoundary, UnionPoly, DTOL_GM);
				if (UnionPoly.size() > 0)
					ExtendPoly.CopyFrom(UnionPoly[0]);

				ExtendPoly.BoolOperation(oper, pIntersect, UnionPoly, DTOL_GM);
			}

			//Area크기 순으로 정렬
			if (UnionPoly.size() > 1)
			{
				sort(UnionPoly.begin(), UnionPoly.end(),
					[](GM2DPolyline* a, GM2DPolyline* b)->bool {return a->Area() > b->Area(); }
				);
			}

			if (UnionPoly.size() > 0)
			{
				pMyTrimmedBoundary->CopyFrom(UnionPoly[0]);
				if (pSubPoly != nullptr)
				{
					pSubPoly->BoolOperationNSortByArea('U', pIntersect, UnionPoly, DTOL_GM);
					if (UnionPoly.size() > 0)
					{
						pSubPoly->CopyFrom(UnionPoly[0]);
					}
				}
			}

			// 			if(doExtendTrim)
			// 				MergeIntersectToBoundary_DoExtendTrim(pCompoG, pMyTrimmedBoundary, pIntersect);
			// 			else
			// 			{
			// 				if (UnionPoly.size()>0)
			// 					pMyTrimmedBoundary->CopyFrom(UnionPoly[0]);
			// 				else
			// 					ASSERT(0);
			// 			}
			GMObject::DeleteGMObject(UnionPoly);
			GMObject::DeleteGMObject(MySplitPoly);

		}
		else if (MyTrimOption == MSConnection::TO_DIRECT)
		{

		}

		if (isPartherColumnHeader == true && pCompoG->IsLinkMember() && GetLinkMemberConPos(pCompoG) != CON_MID)
		{
			MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;

			vector<GM2DPolyline*> MySplitPoly;
			vector<GM2DPolyline*> MySplitPoly2;
			MSPointMemberG* pPointG = (MSPointMemberG*)pParther;
			
			//GM2DPolyline* pSecPoly = pPointG->GetTrimmedBoundary();
			GM2DPolyline SecPoly;
			pPointG->GetSecPolylineByInsertVec(SecPoly);

			double dLoc = GetLinkMemberLoc(pLinkG);
			shared_ptr<GM2DCurve> CenCurve = pLinkG->GetCenCurve();

			GM2DPolyline* delPoly = NULL;
			pIntersect->BoolOperation('A', &SecPoly, MySplitPoly, DTOL_GM);
			for (long iSplit = 0; iSplit < MySplitPoly.size(); iSplit++)
			{
				double dCurLoc = CenCurve->OrthoBaseLocValue(*MySplitPoly[iSplit]->ma_Point[0]);
				if (dLoc < 0.5)
				{
					if (dLoc > dCurLoc)
					{
						pMyTrimmedBoundary->BoolOperationNSortByArea('A', MySplitPoly[iSplit], MySplitPoly2, DTOL_GM);
						if (MySplitPoly2.size() > 0)
							pMyTrimmedBoundary->CopyFrom(MySplitPoly2[0]);
					}
				}
				else
				{
					if (dLoc < dCurLoc)
					{
						pMyTrimmedBoundary->BoolOperationNSortByArea('A', MySplitPoly[iSplit], MySplitPoly2, DTOL_GM);
						if (MySplitPoly2.size() > 0)
							pMyTrimmedBoundary->CopyFrom(MySplitPoly2[0]);
					}
				}
			}

			GMObject::DeleteGMObject(MySplitPoly);
			GMObject::DeleteGMObject(MySplitPoly2);
		}
		
	}

	pMyTrimmedBoundary->IsClosed(DTOL_MODELING);
	AcisGeomHelper::DebugStr1 = L"";
}

void MSConnection::MergeIntersectToBoundary_DoExtendTrim(MSCompoG* pMyCompoG, GM2DPolyline* pMyTrimmedBoundary, GM2DPolyline* pIntersect)
{
	MSLinkMemberG* pLinkG = (MSLinkMemberG*)pMyCompoG;
	if(pLinkG->isArc())
		return;

	double dMyLoc = this->GetLinkMemberLoc(pMyCompoG);
	double dExtendLen = MSFloor::ms_DefLineHitDistTol;
	vector<GM2DPolyline*> UnionPoly;
	MSCompoG* pPartnerCompoG = GetPartner(pMyCompoG);
	if (pPartnerCompoG == nullptr)
		return;

	dExtendLen = min(MSFloor::ms_DefLineHitDistTol *3, min(pLinkG->GetWidth(), pPartnerCompoG->GetWidth()));
	GM2DPolyline ExtendPoly(TRUE);
	long nExtendType = dMyLoc < 0.5 ? -1 : 1;
	pLinkG->MakeExtendedBoundary(ExtendPoly, nExtendType, dExtendLen);

	//char oper = (MyTrimOption == MSConnection::TO_ENDCUT ? 'A' : 'U');
	char oper = 'U';
	ExtendPoly.BoolOperation(oper, pIntersect, UnionPoly, DTOL_GM);
	if(UnionPoly.size() == 1)
		pMyTrimmedBoundary->CopyFrom(UnionPoly[0]);
	else
		ASSERT(0);

// // 추후 검토후 사용
// 	if(pMyCompoG->IsLinkMember() && pPartnerCompoG->IsLinkMember())
// 	{
// 		// 타입 1 라인부재를 임의 값으로 연장 후 트림
// 		dExtendLen = min(MSFloor::ms_DefLineHitDistTol *3, min(pLinkG->GetWidth(), pPartnerCompoG->GetWidth()));
// 		GM2DPolyline ExtendPoly(TRUE);
// 		long nExtendType = dMyLoc < 0.5 ? -1 : 1;
// 		pLinkG->MakeExtendedBoundary(ExtendPoly, nExtendType, dExtendLen);
// 
// 		//char oper = (MyTrimOption == MSConnection::TO_ENDCUT ? 'A' : 'U');
// 		char oper = 'U';
// 		ExtendPoly.BoolOperation(oper, pIntersect, UnionPoly, DTOL_GM);
// 		if(UnionPoly.size() == 1)
// 			pMyTrimmedBoundary->CopyFrom(UnionPoly[0]);
// 		else
// 			ASSERT(0);
// 	}
// 	else if(pMyCompoG->IsLinkMember() && pPartnerCompoG->IsPointMember())
// 	{
// 		GM2DCurve* pLCurve = pLinkG->mp_LLine->Copy();
// 		GM2DCurve* pRCurve = pLinkG->mp_RLine->Copy();
// 		dExtendLen = max(pPartnerCompoG->GetDepth(), pPartnerCompoG->GetWidth()) * 2;
// 
// 		long StartOrEnd = dMyLoc < 0.5 ? 0 : 1;
// 
// 		GM2DLineVector SLine(pLCurve->m_P0, pRCurve->m_P0);
// 		GM2DLineVector ELine(pLCurve->m_P1, pRCurve->m_P1);
// 
// 		bool bLRval = pIntersect->Trim2(pLCurve, StartOrEnd, true, false, dExtendLen);
// 		bool bRRval = pIntersect->Trim2(pRCurve, StartOrEnd, true, false, dExtendLen);
// 
// 		if(bLRval && bRRval)
// 		{
// 			SLine.m_P0 = pLCurve->m_P0;
// 			SLine.m_P1 = pRCurve->m_P0;
// 			ELine.m_P0 = pLCurve->m_P1;
// 			ELine.m_P1 = pRCurve->m_P1;
// 		}
// 		else if(bLRval || bRRval)
// 		{
// 			long StartorEnd2 = bLRval == true ? 0 : 1;
// 			if(StartOrEnd == 0)
// 				pIntersect->Trim2(&SLine, StartorEnd2, false, true);
// 			else if(StartOrEnd == 1)
// 				pIntersect->Trim2(&ELine, StartorEnd2, false, true);
// 		}
// 		vector<GM2DCurve*> SegArr;
// 		SegArr.push_back(pRCurve);
// 		SegArr.push_back(&ELine);
// 		SegArr.push_back(pLCurve);
// 		SegArr.push_back(&SLine);
// 		pMyTrimmedBoundary->AddSegments(SegArr, true);
// 
// 		pMyTrimmedBoundary->BoolOperation('U', pIntersect, UnionPoly, DTOL_GM);
// 		if(UnionPoly.size() == 1)
// 			pMyTrimmedBoundary->CopyFrom(UnionPoly[0]);
// 		else
// 			ASSERT(0);
// 
// 		delete pLCurve;
// 		delete pRCurve;
// 	}
// 	else // 추가 로직 개발
// 		ASSERT(0);

	GMObject::DeleteGMObject(UnionPoly);
}

void MSConnection::Initialize(MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG)
{
	mp_FirstCompoG = pFirstCompoG;
	m_nFirstCompoGID = 0;
	m_nSecondCompoGID = 0;
	if (pFirstCompoG != NULL)
		m_nFirstCompoGID = pFirstCompoG->m_ID;
	mp_SecondCompoG = pSecondCompoG;
	if (pSecondCompoG != NULL)
		m_nSecondCompoGID = pSecondCompoG->m_ID;
	m_ConAngle = 0;
	m_dFirstLoc = 0;
	m_dSecondeLoc = 0;
	m_dFirstLocTol = 0;
	m_dSecondeLocTol = 0;
	m_dInnerDeg = 0;
	m_dIgnoreDeg = 35;
	m_FirstTrimOption = m_SecondTrimOption = TO_DEFAULT;
	m_SetByUser = false;
	m_IsParallel = false;
	m_dPointMemberArea = 0;

	m_eFirstConPos = CON_UNKNOWN;
	m_eSecondeConPos = CON_UNKNOWN;

	if (pFirstCompoG != NULL && pSecondCompoG != NULL)
		SetTrimOption(m_FirstTrimOption, m_SecondTrimOption);
}

void MSConnection::SetFirstCompoG(MSCompoG* pCompoG)
{
	if(pCompoG == NULL)
	{
		mp_FirstCompoG = NULL;
		m_nFirstCompoGID = 0;
		return;
	}
	mp_FirstCompoG = pCompoG;
	m_nFirstCompoGID = pCompoG->m_ID;
}

void MSConnection::SetSecondCompoG(MSCompoG* pCompoG)
{
	if(pCompoG == NULL)
	{
		mp_SecondCompoG= NULL;
		m_nSecondCompoGID = 0;
		return;
	}
	mp_SecondCompoG= pCompoG;
	m_nSecondCompoGID = pCompoG->m_ID;
}

void MSConnection::SetHistoryID()
{
	if(mp_FirstCompoG == NULL || mp_SecondCompoG == NULL)
		return;

	CString id = MSConnection::HistoryID(mp_FirstCompoG, mp_SecondCompoG);
	_historyID = id;
}

bool MSConnection::SetSteelMemberTrimOption()
{
	if (mp_FirstCompoG->IsSteel() || mp_SecondCompoG->IsSteel())
	{
		SetFirstTrimOption(TO_IGNORE);
		SetSecondTrimOption(TO_IGNORE);

		return true;
	}
	return false;
}
double MSConnection::GetPointMemberArea()
{
	if (m_dPointMemberArea == 0)
		SetPointMemberArea();

	return m_dPointMemberArea;
}

void MSConnection::SetPointMemberArea()
{
	m_dPointMemberArea = 0;
	if (mp_FirstCompoG->IsPointMember())
		m_dPointMemberArea = max(mp_FirstCompoG->GetTrimmedBoundary()->Area(), m_dPointMemberArea);
	if (mp_SecondCompoG->IsPointMember())
		m_dPointMemberArea = max(mp_SecondCompoG->GetTrimmedBoundary()->Area(), m_dPointMemberArea);

	if (IsParallel())
	{
		MSLinkMemberG* pFirstLinkG = dynamic_cast<MSLinkMemberG*>(mp_FirstCompoG);
		MSLinkMemberG* pSecondLinkG = dynamic_cast<MSLinkMemberG*>(mp_SecondCompoG);
		if (pFirstLinkG != nullptr && pSecondLinkG != nullptr)
		{
			GM2DCurve* pFirstCenCurve = pFirstLinkG->GetLongLVec();
			GM2DCurve* pSecondCenCurve = pSecondLinkG->GetLongLVec();
			double sLoc = pFirstCenCurve->OrthoBaseLocValue(pSecondCenCurve->m_P0);
			double eLoc = pFirstCenCurve->OrthoBaseLocValue(pSecondCenCurve->m_P1);
			if (sLoc > eLoc)
				swap(sLoc, eLoc);

			sLoc = max(0, sLoc);
			eLoc = min(1, eLoc);
			double dLen = pFirstCenCurve->Length();
			// 중첩되는 길이, 음수인경우 중첩되지 않는다
			m_dPointMemberArea = (eLoc - sLoc) * dLen;

			delete pFirstCenCurve;
			delete pSecondCenCurve;
		}
	}

}

void MSConnection::SetIntersects(vector<GM2DPolyline*>& intersects)
{
	DeleteIntersects();
	for (int i = 0; i < intersects.size(); i++)
		ma_Intersect.push_back(intersects[i]);
}

bool MSConnection::IsSameFloor()
{
	if (mp_FirstCompoG == NULL || mp_SecondCompoG == NULL)
		return false;

	MSFloor* pFirstFloor = mp_FirstCompoG->GetFloor();
	MSFloor* pSecondFloor = mp_SecondCompoG->GetFloor();
	if (pFirstFloor == NULL || pSecondFloor == NULL)
		return false;
	if (pFirstFloor != pSecondFloor)
		return false;

	return true;
}

bool MSConnection::IsErrorPos()
{
	if ((m_dFirstLoc < 0 || m_dFirstLoc > 1) && m_dFirstLocTol * 2 >= 1 && m_eFirstConPos == CON_MID) // 0~1 교차 되지 않고 미드 교차면 에러
		return true;

	if ((m_dSecondeLoc < 0 || m_dSecondeLoc > 1) && m_dSecondeLocTol * 2 >= 1 && m_eSecondeConPos == CON_MID) // 0~1 교차 되지 않고 미드 교차면 에러
		return true;

	return false;
}

bool MSConnection::IsSameConnection(MSConnection* pUrConn)
{
	if ((GetFirstCompoGID() == pUrConn->GetFirstCompoGID() && GetSecondCompoGID() == pUrConn->GetSecondCompoGID()) ||
		(GetFirstCompoGID() == pUrConn->GetSecondCompoGID() && GetSecondCompoGID() == pUrConn->GetFirstCompoGID()))// &&
// 		fabs(m_dFirstLoc - pUrConn->m_dFirstLoc) < DTOL_GM &&
// 		fabs(m_dSecondeLoc == pUrConn->m_dSecondeLoc) < DTOL_GM)
		return true;

	return false;
}

CString MSConnection::GetLogingConnection()
{
	CString strLog;
	strLog.Format(_T("FirstCompoGID:%d, SecondCompoGID:%d, FirstConPos:%d, SecondeConPos:%d, dInnerDeg:%f, IgnoreDeg:%d"), m_nFirstCompoGID, m_nSecondCompoGID, (int)m_eFirstConPos, (int)m_eSecondeConPos, m_dInnerDeg, m_dIgnoreDeg);

	return strLog;
}

bool MSConnection::HasIntersect()
{
	if (ma_Intersect.size() == 0)
		return false;

	return true;
}

void MSConnection::UpdateMembers()
{
	map<int, MSCompoG*> UpdateCompoGMap;
	MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(mp_FirstCompoG->m_ID);
	MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(mp_SecondCompoG->m_ID);
	if (mp_FirstCompoG->GetType() == MSElement::msSlabG && mp_SecondCompoG->GetType() == MSElement::msSlabG)
	{
		// First,Secound 변환시 각 슬래브의 오프닝은 삭제해준다.
		MSSlabG* pCurSlab = (MSSlabG*)mp_FirstCompoG;
		MSSlabG* pUrSlab = (MSSlabG*)mp_SecondCompoG;
		MSOpening* pCurOpening = pCurSlab->mp_PolylineInfo->FindOpening_MadeByCompoG(pUrSlab->m_ID);
		if (pCurOpening != nullptr)
			pCurSlab->mp_PolylineInfo->RemoveOpening(pCurOpening);

		MSOpening* pUrOpening = pUrSlab->mp_PolylineInfo->FindOpening_MadeByCompoG(pCurSlab->m_ID);
		if (pUrOpening != nullptr)
			pUrSlab->mp_PolylineInfo->RemoveOpening(pUrOpening);

		vector<MSCompoG*> UpdateCompoG = mp_FirstCompoG->GetPartnerCompoGs();
		UpdateCompoG.push_back(mp_FirstCompoG);
		UpdateCompoG.push_back(mp_SecondCompoG);
		vector<MSCompoG*> UpdateCompoG2 = mp_SecondCompoG->GetPartnerCompoGs();
		UpdateCompoG.insert(UpdateCompoG.end(), UpdateCompoG2.begin(), UpdateCompoG2.end());

		sort(UpdateCompoG.begin(), UpdateCompoG.end(),
			[](MSCompoG* a, MSCompoG* b)->bool {return a->m_ID < b->m_ID; }
		);

		vector<MSCompoG*>::iterator new_end;
		new_end = unique(UpdateCompoG.begin(), UpdateCompoG.end());
		UpdateCompoG.erase(new_end, UpdateCompoG.end());

		for (int i = 0; i < UpdateCompoG.size(); i++)
		{
			// 원래의 입력 값을 셋팅하기 위한 함수
			//UpdateCompoG[i]->SetProfile(UpdateCompoG[i]->GetTrimmedBoundary_Acis());
			UpdateCompoG[i]->InitTrimmedBoundary(true);
			UpdateCompoG[i]->InitTrimmedBoundary(false);
		}
		for (int i = 0; i < UpdateCompoG.size(); i++)
		{
			if (UpdateCompoG[i]->mp_PolylineInfo != nullptr)
			{
				UpdateCompoG[i]->mp_PolylineInfo->TrimExistPolyLineObject(true);
				UpdateCompoG[i]->mp_PolylineInfo->TrimExistPolyLineObject(false);
			}
			UpdateCompoG[i]->UpdateTrimmedBoundary();
			UpdateCompoG[i]->UpdateBoundaryVisibilities();
		}
	}
	else
		mp_SecondCompoG->ModifyObject();
}

void MSConnection::SetDeleted(bool isDeleted)
{
	MSHistoryObject::SetDeleted(isDeleted);
	SetDefTrimOption();
}

void MSConnection::SubtractSecondTrimmedBoundary()
{
	MSCompoG* pFirstG = GetFirstCompoG();
	MSCompoG* pSecondG = GetSecondCompoG();
	if ((pFirstG == nullptr || pSecondG == nullptr) ||
		(m_FirstTrimOption == TO_IGNORE || m_SecondTrimOption == TO_IGNORE) ||
		(m_eSecondeConPos == CON_MID) ||
		(m_IsParallel == true) ||
		(pFirstG->IsLinkMember() == false || pSecondG->IsLinkMember() == false)
		)
		return;

	GM2DPolyline* pFirstTrimmedBoundary = pFirstG->GetTrimmedBoundary();
	GM2DPolyline* pSecondTrimmedBoundary = pSecondG->GetTrimmedBoundary();

	vector<GM2DPolyline*> SubtractBoundaryArr;
	pSecondTrimmedBoundary->BoolOperationNSortByArea('A', pFirstTrimmedBoundary, SubtractBoundaryArr, MSFloor::ms_DefLineHitDistTol);
	if (SubtractBoundaryArr.size() > 0)
		pSecondTrimmedBoundary->CopyFrom(SubtractBoundaryArr[0]);

	GMObject::DeleteGMObject(SubtractBoundaryArr);
}

bool MSConnection::IsLevelTouch()
{
	MSCompoG* pFirstG = GetFirstCompoG();
	MSCompoG* pSecondG = GetSecondCompoG();
	if (pFirstG == nullptr || pSecondG == nullptr)
		return false;

	return pFirstG->IsLevelTouch(pSecondG);
}

bool MSConnection::IsRightAngle()
{
	MSCompoG* pFirstG = GetFirstCompoG();
	MSCompoG* pSecondG = GetSecondCompoG();
	if (pFirstG == nullptr || pSecondG == nullptr)
		return false;
	if (pFirstG->IsLinkMember() == false || pSecondG->IsLinkMember() == false)
		return false;

	double dInnnerDeg = fabs(GetInnerDeg());
	if (fabs(dInnnerDeg - 90) < DTOL1
		|| fabs(dInnnerDeg - 270) < DTOL1)
		return true;

	return false;
}

void MSConnection::SetDefTrimOption()
{
	if (m_SetByUser == false)
	{
		m_FirstTrimOption = TO_DEFAULT;
		m_SecondTrimOption = TO_DEFAULT;
	}
}

bool MSConnection::HasTrimOption(TRIM_OPTION Opt)
{
	if (m_FirstTrimOption == Opt)
		return true;
	if (m_SecondTrimOption == Opt)
		return true;

	return false;
}


bool MSConnection::CompareTo(MSConnection* pTargetConn)
{
	bool myRightAngle = IsRightAngle();
	bool urRightAngle = pTargetConn->IsRightAngle();

	bool myMiter = (GetFirstTrimOption() == MSConnection::TO_MITER || GetSecondTrimOption() == MSConnection::TO_MITER);
	bool urMiter = (pTargetConn->GetFirstTrimOption() == MSConnection::TO_MITER || pTargetConn->GetSecondTrimOption() == MSConnection::TO_MITER);

	bool myParallel = IsParallel();
	bool urParallel = pTargetConn->IsParallel();

	double myMaxWidth = max(GetFirstCompoG()->GetWidth(), GetSecondCompoG()->GetWidth());
	double urMaxWidth = max(pTargetConn->GetFirstCompoG()->GetWidth(), pTargetConn->GetSecondCompoG()->GetWidth());

	if (myRightAngle == urRightAngle)
	{
		if (myMiter == urMiter)
		{
			if (myParallel == urParallel)
			{
				if (fabs(myMaxWidth - urMaxWidth) < DTOL_GM)
				{

				}
				else
				{
					if (myMaxWidth > urMaxWidth)
						return true;
				}
			}
			else
			{
				if (myParallel)
					return true;
			}
		}
		else
		{
			if (myMiter)
				return true;
		}
	}
	else
	{
		if (myRightAngle)
			return true;
	}

	return false;
}

MSConnection::TRIM_OPTION MSConnection::ms_DefaultLinkGOption = TO_EXTENDCUT;




