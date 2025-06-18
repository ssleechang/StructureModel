#include "StdAfx.h"
#include "MSCompoG.h"
#include "MSFloor.h"
#include "MSRebarBuilding.h"
#include "GMLib/MSVersionInfo.h"

#include "MSConstructionZone.h"

#include "MSOpeningG.h"
#include "MSStoryMaterial.h"
#include "CmdProcess/CmdNewModifyVariable.h"
#include "CmdProcess/CmdProcessHelper.h"
#include "MSSrcMaterial.h"
#include "MSSteel.h"
#include "MSConcrete.h"
#include "MSModelChecker.h"
#include "MSPolylineObject.h"
#include "MSCompoGUtil.h"
#include "MSRcFWBeamM.h"

#include "GMLib/GM2DBound.h"
#include <algorithm>
#include "RebarTemplateManager.h"
#include "FormWorkTemplateManager.h"

#include "MSAddRebarBase.h"

#include "Radfcore_CLI/AppGlobalOptionWrapper.h"
#include "MSHandRailG.h"
#include "../RADFCore_CLI/RadfContextWrapper.h"

#include "MSSrcColumnM.h"
#include "MSConnection.h"
#include "MSRcSlabM.h"

int MSCompoG::NOTE_VERIFY = 0;
bool MSCompoG::_bUseEMS = true;

IMPLEMENT_SERIAL(MSCompoG, MSElement, VERSIONABLE_SCHEMA | 1)

MSCompoG::MSCompoG() : m_VolumeAreaEMS(this), m_VolumeAreaEMS_NewEMS(this)
{
	Init(true);
}

MSCompoG::~MSCompoG(void)
{
	/*CString msg;
	msg.Format(L"deleting compoG %d", m_ID);
	LoggerWrapper::Log(msg);*/
	
	try
	{

		delete mp_Boundary;

		DeleteTrimmedBoundary();
		if (mp_UserBoundary != NULL)
			delete mp_UserBoundary;
		DeleteTrimmedBoundary_Acis();

		DeleteConnections();
		DeleteVectorArr();
		DeleteVecInput();

		if (IsEachCompoM())
		{
			delete mp_CompoM;
			mp_CompoM = NULL;
		}

		DeleteAddRebars();

		delete m_pViewDisplaySetting;
	}
	catch (const std::exception& ex)
	{
		CString errorMsg;
		errorMsg.Format(L"Standard exception occurred in compoG destructor: %S", ex.what());
		StatusReporterWrapper::OutputError(errorMsg);
	}
	catch (const _com_error& comEx)
	{
		CString errorMsg;
		errorMsg.Format(L"COM exception occurred in compoG destructor: %s", comEx.ErrorMessage());
		StatusReporterWrapper::OutputError(errorMsg);
	}
#ifndef _DEBUG
	catch (...)
	{
		StatusReporterWrapper::OutputError(L"Unexpected exception occurred in compoG destructor!");
	}
#endif
}

void MSCompoG::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	bool isEachCompoM = IsEachCompoM();
	buffer << isEachCompoM;
	if (isEachCompoM)
	{
		int nMType = (int)mp_CompoM->GetType();
		buffer << nMType;
		mp_CompoM->Dump(buffer);
	}
	else
		buffer << &mp_CompoM;

	buffer << &mp_Source << m_MarkGID;
	
	long NumVec = ma_Vec.size();
	buffer << NumVec;

	for(long iVec = 0 ; iVec < NumVec ; iVec++)
	{
		GM2DVector* pVec = ma_Vec[iVec];
		pVec->Dump(buffer);
	}

	long NumVecInput = ma_Vec_Input.size();
	buffer << NumVecInput;

	for(long iVec = 0 ; iVec < NumVecInput ; iVec++)
	{
		GM2DVector* pVec = ma_Vec_Input[iVec];
		pVec->Dump(buffer);
	}

	int count = ma_VolumeInfo.size();
	buffer << count;
	for(int i=0 ; i<ma_VolumeInfo.size() ; ++i)
		ma_VolumeInfo[i].Dump(buffer);

	count = ma_AreaInfo.size();
	buffer << count;
	for(int i=0 ; i<ma_AreaInfo.size() ; ++i)
		ma_AreaInfo[i].Dump(buffer);

	count = ma_UserFormWork.size();
	buffer << count;
	for(int i=0 ; i<ma_UserFormWork.size() ; ++i)
		ma_UserFormWork[i].Dump(buffer);

	count = ma_UserRebar.size();
	buffer << count;
	for(int i=0 ; i<ma_UserRebar.size() ; ++i)
		ma_UserRebar[i].Dump(buffer);

	buffer << m_IsScheduleAssigned;
	buffer << m_startDate;
	buffer << m_finishDate;
	buffer << m_IsReverseBox;
	buffer << m_IsRigidMember;
	buffer << m_IsQuantityReplaced;
	m_LabelEcc.Dump(buffer);
	
	bool HasBoundary = false;
	if (mp_Boundary != NULL)
		HasBoundary = true;
	buffer << HasBoundary;
	if (HasBoundary)
	{
		mp_Boundary->Dump(buffer);
	}

	bool HasTrimmedBoundary = false;
	if(mp_TrimmedBoundary != NULL)
		HasTrimmedBoundary = true;
	buffer << HasTrimmedBoundary;
	if(HasTrimmedBoundary)
	{
		mp_TrimmedBoundary->Dump(buffer);
		
		long NumVisible = ma_TrimmedBoundaryVisible.size();
		buffer << NumVisible;
		for(long i = 0 ; i < NumVisible ; i++)
			buffer << ma_TrimmedBoundaryVisible[i];
	}

	m_VolumeAreaEMS.Dump(buffer);

	buffer << m_nRebarTemplateID;
	count = ma_ContainedMemberID.size();
	buffer << count;
	for(int i = 0 ; i < count ; i++)
		buffer << ma_ContainedMemberID[i];

	bool HasTrimmedBoundary_Acis = false;
	if (mp_TrimmedBoundary_Acis != NULL)
		HasTrimmedBoundary_Acis = true;
	buffer << HasTrimmedBoundary_Acis;
	if (HasTrimmedBoundary_Acis)
	{
		mp_TrimmedBoundary_Acis->Dump(buffer);
	}

	count = mm_AddRebar.size();
	buffer << count;
	for (auto it = mm_AddRebar.begin(); it != mm_AddRebar.end(); it++)
	{
		int type = (int)it->second->GetType();
		buffer << type;
		it->second->Dump(buffer);
	}

	buffer << m_nFormWorkTemplateID;

	buffer << m_nBuildingInfoID;

	m_pViewDisplaySetting->Dump(buffer);

	buffer << m_IsTempCompoG;

	int nConstructionPhase = (int)m_eConstructionPhase;
	buffer << nConstructionPhase;

	buffer << m_nMainCompoGID;

	buffer << m_IsUserFloor;
	buffer << m_nUserFloorID;
}

void MSCompoG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	MSBaseBuilding* pBuild = GetBuilding();

	if (IsEachCompoM())
	{
		delete mp_CompoM;
		mp_CompoM = nullptr;
	}

	bool isEachCompoM = false;
	buffer >> isEachCompoM;
	if (isEachCompoM)
	{
		int nMType = 0;
		buffer >> nMType;
		mp_CompoM = pBuild->CreateNewCompoM(nMType);
		mp_CompoM->Recovery(buffer);
	}
	else
		buffer >> &mp_CompoM;

	buffer >> &mp_Source >> m_MarkGID;

	DeleteVectorArr();
	DeleteVecInput();

	long NumVec = 0;
	buffer >> NumVec;

	for(long iVec = 0 ; iVec < NumVec ; iVec++)
	{
		GM2DVector* pVec = new GM2DVector();
		pVec->Recovery(buffer);
		ma_Vec.push_back(pVec);
	}

	long NumVecInput = 0;
	buffer >> NumVecInput;

	for(long iVec = 0 ; iVec < NumVecInput ; iVec++)
	{
		GM2DVector* pVec = new GM2DVector();
		pVec->Recovery(buffer);
		ma_Vec_Input.push_back(pVec);
	}

	int count = ma_VolumeInfo.size();
	buffer >> count;
	ma_VolumeInfo.resize(count);
	for(int i=0 ; i<ma_VolumeInfo.size() ; ++i)
		ma_VolumeInfo[i].Recovery(buffer);

	count = ma_AreaInfo.size();
	buffer >> count;
	ma_AreaInfo.resize(count);
	for(int i=0 ; i<ma_AreaInfo.size() ; ++i)
		ma_AreaInfo[i].Recovery(buffer);

	count = ma_UserFormWork.size();
	buffer >> count;
	ma_UserFormWork.resize(count);
	for(int i=0 ; i<ma_UserFormWork.size() ; ++i)
		ma_UserFormWork[i].Recovery(buffer);

	count = ma_UserRebar.size();
	buffer >> count;
	ma_UserRebar.resize(count);
	for(int i=0 ; i<ma_UserRebar.size() ; ++i)
		ma_UserRebar[i].Recovery(buffer);

	buffer >> m_IsScheduleAssigned;
	buffer >> m_startDate;
	buffer >> m_finishDate;
	buffer >> m_IsReverseBox;
	buffer >> m_IsRigidMember;
	buffer >> m_IsQuantityReplaced;
	m_LabelEcc.Recovery(buffer);

	bool HasBoundary = false;
	buffer >> HasBoundary;
	if (HasBoundary)
	{
		if (mp_Boundary == NULL)
			mp_Boundary = new GM2DPolyline(TRUE);
		mp_Boundary->Recovery(buffer);
	}

	bool HasTrimmedBoundary = false;
	buffer >> HasTrimmedBoundary;
	if(HasTrimmedBoundary)
	{
		if(mp_TrimmedBoundary == NULL)
			mp_TrimmedBoundary = new GM2DPolyline(TRUE);
		mp_TrimmedBoundary->Recovery(buffer);

		ma_TrimmedBoundaryVisible.clear();
		long NumVisible = 0;
		BOOL isVisible = TRUE;
		buffer >> NumVisible;
		for(long i = 0 ; i < NumVisible ; i++)
		{
			buffer >> isVisible;
			ma_TrimmedBoundaryVisible.push_back(isVisible);
		}
	}

	MSFloor* pFloor = GetFloor();

	m_VolumeAreaEMS.Recovery(buffer);
	
	buffer >> m_nRebarTemplateID;

	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		long nID = 0;
		buffer >> nID;
		ma_ContainedMemberID.push_back(nID);
	}

	bool HasTrimmedBoundary_Acis = false;
	buffer >> HasTrimmedBoundary_Acis;
	if (HasTrimmedBoundary_Acis == true)
	{
		if (mp_TrimmedBoundary_Acis == NULL)
			mp_TrimmedBoundary_Acis = new GM2DPolyline(TRUE);
		mp_TrimmedBoundary_Acis->Recovery(buffer);
	}

	DeleteAddRebars();
	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		int type = 0;
		buffer >> type;
		MSAddRebarBase* pAddRebar = MSAddRebarBase::CreateAddRebar((Type_MSCompoG)type);
		if (pAddRebar == nullptr)	continue;
		pAddRebar->SetOwnerCompoG(this);
		pAddRebar->Recovery(buffer);
		
		Add(pAddRebar);
	}

	buffer >> m_nFormWorkTemplateID;
	buffer >> m_nBuildingInfoID;
	m_pViewDisplaySetting->Recovery(buffer);

	buffer >> m_IsTempCompoG;
	
	int nConstructionPhase = 0;
	buffer >> nConstructionPhase;
	m_eConstructionPhase = (eConstructionPhase)nConstructionPhase;

	buffer >> m_nMainCompoGID;

	buffer >> m_IsUserFloor;
	buffer >> m_nUserFloorID;
}

void MSCompoG::Serialize( CArchive& ar)
{
	MSElement::Serialize(ar);

	long CompoMID = 0;
	long OwnerID = 0;
	long SourceID = 0;
	if(ar.IsStoring())
	{
		if(mp_CompoM)	CompoMID = mp_CompoM->m_ID;
		if(mp_Owner)			OwnerID = mp_Owner->m_ID;
		if(mp_Source)			SourceID = mp_Source->m_ID;
		ar << CompoMID;
		ar << OwnerID;
		ar << SourceID;
	
		int CloneCount = ma_Clone.size();
		ar << CloneCount;
		vector<MSCompoG*>::iterator itCompoG;
		for(itCompoG = ma_Clone.begin(); itCompoG != ma_Clone.end(); itCompoG++)
		{
			MSCompoG* pClone = *itCompoG;
			ar << pClone->GetOwner()->m_ID;
			ar << (*itCompoG)->m_ID;
		}

		int VecCount = ma_Vec.size();
		ar << VecCount;
		vector<GM2DVector*>::iterator itVec;
		for(itVec = ma_Vec.begin(); itVec != ma_Vec.end(); itVec++)
		{
			(*itVec)->Serialize(ar);
		}

		ar << m_MarkGID;

		int count = ma_VolumeInfo.size();
		ar << count;
		for(int i=0 ; i<ma_VolumeInfo.size() ; ++i)
			ma_VolumeInfo[i].Serialize(ar);

		count = ma_AreaInfo.size();
		ar << count;
		for(int i=0 ; i<ma_AreaInfo.size() ; ++i)
			ma_AreaInfo[i].Serialize(ar);

		count = ma_UserFormWork.size();
		ar << count;
		for(int i=0 ; i<ma_UserFormWork.size() ; ++i)
			ma_UserFormWork[i].Serialize(ar);

		count = ma_UserRebar.size();
		ar << count;
		for(int i=0 ; i<ma_UserRebar.size() ; ++i)
			ma_UserRebar[i].Serialize(ar);

		ar << m_IsScheduleAssigned;
		if(m_IsScheduleAssigned)
		{
			ar << m_startDate;
			ar << m_finishDate;
		}

		count = ma_BarSetID.size();
		ar << count;
		for (int i=0 ; i<ma_BarSetID.size() ; ++i)
			ar << ma_BarSetID[i];

		ar << m_IsReverseBox;
		ar << m_IsRigidMember;
		ar << m_IsQuantityReplaced;
		m_LabelEcc.Serialize(ar);

		// JOINT_SERIALIZE
// 		long ID = 0;
// 		count = ma_Joint.size();
// 		ar << count;
// 		for (int i=0 ; i<ma_Joint.size() ; ++i)
// 		{
// 			if (ma_Joint[i] == NULL) { ID = 0; ar << ID; }
// 			else
// 			{
// 				if (ma_Joint[i]->m_ID == 0)
// 				{
// 					ID = -1;
// 					ar << ID;
// 					ar << ma_Joint[i]->m_dXc << ma_Joint[i]->m_dYc;
// 				}
// 				else { ID = ma_Joint[i]->m_ID; ar << ID; }
// 			}
// 		}

		// Connection Serialize
		CheckValidConnections();
		count = ma_Connections.size();
		ar << count;
		for (int i=0 ; i<count ; ++i)
			ma_Connections[i]->Serialize(ar);

		GM2DPolyline::SerializeWithNullFlag(ar, mp_TrimmedBoundary);
		
		count = ma_TrimmedBoundaryVisible.size();
		ar << count;
		for(int i = 0 ; i < count ; i++)
			ar << ma_TrimmedBoundaryVisible[i];

		m_VolumeAreaEMS.Serialize(ar);

		int VecInputCount = ma_Vec_Input.size();
		ar << VecInputCount;
		for(itVec = ma_Vec_Input.begin(); itVec != ma_Vec_Input.end(); itVec++)
		{
			(*itVec)->Serialize(ar);
		}

		ar << m_nRebarTemplateID;

		count = ma_ContainedMemberID.size();
		ar << count;
		for (int i = 0; i < count; i++)
			ar << ma_ContainedMemberID[i];

		if (MSVersionInfo::GetCurrentVersion() > 20170518)
			RemoveSameConnection();

		mp_Boundary->Serialize(ar);

		count = mm_AddRebar.size();
		ar << count;
		for (auto it = mm_AddRebar.begin(); it != mm_AddRebar.end(); it++)
		{
			int type = (int)it->second->GetType();
			ar << type;
			it->second->Serialize(ar);
		}

		ar << m_nFormWorkTemplateID;

		ar << m_nBuildingInfoID;

		m_pViewDisplaySetting->Serialize(ar);

		int nConstructionPhase = (int)m_eConstructionPhase;
		ar << nConstructionPhase;

		ar << m_nMainCompoGID;

		ar << m_IsUserFloor;
		ar << m_nUserFloorID;
	}
	else
	{
		ar >> CompoMID;
		ar >> OwnerID;
		ar >> SourceID;

		int CloneCount = 0;
		ar >> CloneCount;
		for(int iClone = 0; iClone < CloneCount; iClone++)
		{
			int CloneOwnerID = 0;
			int CloneID = 0;
			ar >> CloneOwnerID;
			ar >> CloneID;

			tmep_CloneMap.insert(make_pair(CloneOwnerID, CloneID));
		}

		int VecCount = 0;
		ar >> VecCount;
		for(int iVec = 0; iVec < VecCount; iVec++)
		{
			GM2DVector* pVector = new GM2DVector();
			pVector->Serialize(ar);
			if(GetType()==MSCompoG::msSlabG || GetType() == MSCompoG::msStairG)
				ma_Vec.push_back(pVector);
			else
				ReplaceVec(iVec, pVector);
		}

		ar >> m_MarkGID;

		if(mp_Owner)
		{
			MSBaseBuilding* pBldg = GetBuilding();
			if(pBldg != nullptr)
			mp_CompoM = pBldg->GetCompoM(CompoMID);

			// CompoMID가 있으나 빌딩에 CompoM을 찾지 못하는 경우 EachCompoM으로 만들어준다(NoName)
			if (CompoMID != 0 && mp_CompoM == nullptr)
			{
				mp_CompoM = pBldg->CreateNewCompoM(GetDefCompoMType());
				CString strGroup = LocalDic::GetLocal(L"CompoM Error", L"StructureModel");
				CString strErrorMsg;
				strErrorMsg.Format(LocalDic::GetLocal(L"%s(%ld)의 CompoM 정보가 잘못 되어 NoName으로 변경합니다.", L"StructureModel"), GetGroupName(), m_ID);
				MSModelChecker::MakeErrorObject(strGroup, strErrorMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_COMPOM, this);
			}
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20140313)
		{
			int count = 0;
			ar >> count;
			ma_VolumeInfo.resize(count);
			for(int i=0 ; i<count ; ++i)
				ma_VolumeInfo[i].Serialize(ar);

			ar >> count;
			ma_AreaInfo.resize(count);
			for(int i=0 ; i<count ; ++i)
				ma_AreaInfo[i].Serialize(ar);

			if(MSVersionInfo::GetCurrentVersion() >= 20140522)
			{
				ar >> count;
				ma_UserFormWork.resize(count);
				for(int i=0 ; i<count ; ++i)
					ma_UserFormWork[i].Serialize(ar);
			}

			if (MSVersionInfo::GetCurrentVersion() >= 20160905 && MSVersionInfo::GetCurrentVersion() < 20170522)
			{
				for (int i = 0; i < ma_UserFormWork.size(); i++)
				{
					if (ma_UserFormWork[i].formWorkName == _T(""))
						ma_UserFormWork[i].formWorkName = _T("None");
				}
			}
			if(MSVersionInfo::GetCurrentVersion() >= 20150115)
			{
				ar >> count;
				ma_UserRebar.resize(count);
				for(int i=0 ; i<count ; ++i)
					ma_UserRebar[i].Serialize(ar);

				if (mp_CompoM != nullptr)
				{
					MSCompoM::Type_MSCompoM eMType = mp_CompoM->GetType();
					if (eMType == MSCompoM::msPcColumnM || eMType == MSCompoM::msSteelColumnM || eMType == MSCompoM::msSteelBeamM)
						ma_UserRebar.clear();
				}
			}

			if(MSVersionInfo::GetCurrentVersion() >= 20140314)
				ar >> m_IsScheduleAssigned;
			else
				m_IsScheduleAssigned = false;
			if(m_IsScheduleAssigned)
			{
				ar >> m_startDate;
				ar >> m_finishDate;
			}
			else
			{
				m_startDate = MSConstructionZone::InitialDate();
				m_finishDate = MSConstructionZone::InitialDate();
			}
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20140325)
		{
			int count = 0;
			ar >> count;
			ma_BarSetID.resize(count);
			for(int i=0 ; i<count ; ++i)
				ar >> ma_BarSetID[i];
		}
		if (((MSObject::IsBHQorBHCEMode() && MSVersionInfo::GetCurrentVersion() >= 20140407 && MSVersionInfo::GetCurrentVersion() < 20200909))
			|| (MSObject::IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() >= 20140407 && MSVersionInfo::GetCurrentVersion() < 20240307))
		{
			ar >> m_IsDummy;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20150612)
			ar >> m_IsReverseBox;
		if(MSVersionInfo::GetCurrentVersion() >= 20150108)
		{
			ar >> m_IsRigidMember;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20140910)
			ar >> m_IsQuantityReplaced;
		if(MSVersionInfo::GetCurrentVersion() >= 20140423)
			m_LabelEcc.Serialize(ar);

		if (MSVersionInfo::GetCurrentVersion() >= 20141003 && MSVersionInfo::GetCurrentVersion() < 20150107)
		{
			long ID = 0;
			int count = 0;
			ar >> count;
			for (int i=0 ; i<count ; ++i)
			{
				ar >> ID;
				if (ID == -1)
				{
					double x = 0, y = 0;
					ar >> x >> y;
				}
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20141217)
		{
			int count = 0;
			ar >> count;
			for (int i=0 ; i<count ; ++i)
			{
				MSConnection* con = MSConnection::GetEmptyConnection();
				con->Serialize(ar);
				if (MSConnection::CONNECTION_SERIALIZE())
					ma_Connections.push_back(con);
			}

			GM2DPolyline::SerializeWithNullFlag(ar, mp_TrimmedBoundary);
			if ( !MSConnection::CONNECTION_SERIALIZE())
				mp_TrimmedBoundary = NULL;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20141007 && MSVersionInfo::GetCurrentVersion() < 20150210)
		{
			InitTrimmedBoundaryVisibility();
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20150210)
		{
			ma_TrimmedBoundaryVisible.clear();
			int count = 0;
			ar >> count;
			for(int i = 0 ; i < count ; i++)
			{
				BOOL isVisibility;
				ar >> isVisibility;
				ma_TrimmedBoundaryVisible.push_back(isVisibility);
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150209)
		{
			m_VolumeAreaEMS.Serialize(ar);
		}
		if (MSVersionInfo::GetCurrentVersion() < 20170413)
		{
			//옛날 버전에 저장되어있던 면별 공제, 주두부거푸집을 변경 : 3-> 0 , 0->1 ,1->2, 2->3
			if (GetType() == Type_MSCompoG::msColumnG && m_VolumeAreaEMS.ma_Param.size() > 10)
			{
				if (m_VolumeAreaEMS.ma_Param.size() == 16)
				{
					double CHA0 = m_VolumeAreaEMS.ma_Param[15];
					double CHA1 = m_VolumeAreaEMS.ma_Param[12];
					double CHA2 = m_VolumeAreaEMS.ma_Param[13];
					double CHA3 = m_VolumeAreaEMS.ma_Param[14];
					m_VolumeAreaEMS.ma_Param[12] = CHA0;
					m_VolumeAreaEMS.ma_Param[13] = CHA1;
					m_VolumeAreaEMS.ma_Param[14] = CHA2;
					m_VolumeAreaEMS.ma_Param[15] = CHA3;
				}
				else if (m_VolumeAreaEMS.ma_Param.size() == 20)
				{
					double BCV0 = m_VolumeAreaEMS.ma_Param[13];
					double BCV1 = m_VolumeAreaEMS.ma_Param[10];
					double BCV2 = m_VolumeAreaEMS.ma_Param[11];
					double BCV3 = m_VolumeAreaEMS.ma_Param[12];
					m_VolumeAreaEMS.ma_Param[10] = BCV0;
					m_VolumeAreaEMS.ma_Param[11] = BCV1;
					m_VolumeAreaEMS.ma_Param[12] = BCV2;
					m_VolumeAreaEMS.ma_Param[13] = BCV3;

					double CHA0 = m_VolumeAreaEMS.ma_Param[19];
					double CHA1 = m_VolumeAreaEMS.ma_Param[16];
					double CHA2 = m_VolumeAreaEMS.ma_Param[17];
					double CHA3 = m_VolumeAreaEMS.ma_Param[18];
					m_VolumeAreaEMS.ma_Param[16] = CHA0;
					m_VolumeAreaEMS.ma_Param[17] = CHA1;
					m_VolumeAreaEMS.ma_Param[18] = CHA2;
					m_VolumeAreaEMS.ma_Param[19] = CHA3;
				}
			}
			else if (GetType() == Type_MSCompoG::msIsoFootG && m_VolumeAreaEMS.ma_Param.size() > 7)
			{
				int haunchCount = (m_VolumeAreaEMS.ma_Param.size() - 7)/3;
				for (int i = 0; i < haunchCount; i++)
				{
					double dirValue = m_VolumeAreaEMS.ma_Param[7 + i * 3 + 2];
					if (dirValue == 0)
						m_VolumeAreaEMS.ma_Param[7 + i * 3 + 2] = 1;
					else if(dirValue== 1)
						m_VolumeAreaEMS.ma_Param[7 + i * 3 + 2] = 2;
					else if (dirValue == 2)
						m_VolumeAreaEMS.ma_Param[7 + i * 3 + 2] = 3;
					else if (dirValue == 3)
						m_VolumeAreaEMS.ma_Param[7 + i * 3 + 2] = 0;
				}
			}
		}
		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20180308) //난간벽에 TH, BH 추가
		{
			if (GetType() == Type_MSCompoG::msHandRailG && m_VolumeAreaEMS.ma_Param.size() == 5)
			{
				m_VolumeAreaEMS.ma_Param.push_back(0);//TH
				m_VolumeAreaEMS.ma_Param.push_back(0);//BH
			}
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20180621) //난간벽에 TTS 추가
		{
			if (GetType() == Type_MSCompoG::msHandRailG && m_VolumeAreaEMS.ma_Param.size() == 7)
			{
				m_VolumeAreaEMS.ma_Param.push_back(0);//TTS
			}
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20190719)	// 벽과 보에 면적 추가
		{
			if ((GetType() == Type_MSCompoG::msBeamG && IsCustomBeam() == false) ||
				GetType() == Type_MSCompoG::msWallG)
			{
				if (m_VolumeAreaEMS.ma_Param.size() > 0)
				{
					m_VolumeAreaEMS.ma_Param.push_back(m_VolumeAreaEMS.ma_Param.size() - 1);
					for (int i = m_VolumeAreaEMS.ma_Param.size() - 1; i >= 3; i--)
						m_VolumeAreaEMS.ma_Param[i] = m_VolumeAreaEMS.ma_Param[i - 1];
					m_VolumeAreaEMS.ma_Param[2] = 0;
				}
			}
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20190909)	// CUS-995 PC 기둥 산출방식 업데이트
		{
			if (GetType() == Type_MSCompoG::msColumnG)
			{
				if (m_VolumeAreaEMS.ma_Param.size() == 20)
				{
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 14, 0);
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 14, 0);
				}
			}
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20190910)	// CUS-995 PC 보, 슬래브 산출방식 업데이트
		{
			if (GetType() == Type_MSCompoG::msBeamG)
			{
				if (m_VolumeAreaEMS.ma_Param.size() >= 11)
				{
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 8, m_VolumeAreaEMS.ma_Param[1]);
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 8, 0);
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 8, m_VolumeAreaEMS.ma_Param[0]);
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 8, 0);
				}
			}
			else if (GetType() == Type_MSCompoG::msSlabG && mp_CompoM != nullptr)
			{
				if (m_VolumeAreaEMS.ma_Param.size() == 17 && IsHalfPCSlab())
				{
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 3, m_VolumeAreaEMS.ma_Param[2]);
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 3, 0);
				}
			}
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20200113)	// BH-4367 Wall, WallFoot의 마구리면 거푸집 면적 공제로직 추가
		{
			if (GetType() == Type_MSCompoG::msWallG)
			{
				if (m_VolumeAreaEMS.ma_Param.size() == 19)
				{
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 14, 0);
					m_VolumeAreaEMS.ma_Param.insert(m_VolumeAreaEMS.ma_Param.begin() + 14, 0);
				}
			}
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20150430)
		{
			DeleteVecInput();
			int VecInputCount = 0;
			ar >> VecInputCount;
			for(int iVec = 0; iVec < VecInputCount; iVec++)
			{
				GM2DVector* pVector = new GM2DVector();
				pVector->Serialize(ar);
				ma_Vec_Input.push_back(pVector);
			}
		}
		else
		{
			DeleteVecInput();
			for (int i=0 ; i<ma_Vec.size() ; ++i)
				ma_Vec_Input.push_back((GM2DVector*)ma_Vec[i]->Copy());
		}
		
		if (MSVersionInfo::GetCurrentVersion() >= 20160122)
		{
			ar >> m_nRebarTemplateID;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170102)
		{
			ma_ContainedMemberID.clear();
			int count = 0;
			ar >> count;
			for (int i = 0; i < count; i++)
			{
				int nID;
				ar >> nID;
				ma_ContainedMemberID.push_back(nID);
			}
			SortContainMemberID();
		}
		
		//if (MSVersionInfo::GetCurrentVersion() <= 20170518)
			RemoveSameConnection();
		
		if(MSVersionInfo::GetCurrentVersion() >= 20180511)
			mp_Boundary->Serialize(ar);

		if (MSVersionInfo::GetCurrentVersion() >= 20171213)
		{
			int count = 0, type = 0;
			ar >> count;
			for (int i = 0; i < count; i++)
			{
				type = 0;
				ar >> type;
				MSAddRebarBase* pAddRebar = MSAddRebarBase::CreateAddRebar((Type_MSCompoG)type);
				if (pAddRebar == nullptr)	continue;
				pAddRebar->Serialize(ar);
				Add(pAddRebar);
			}
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180622)
		{
			ar >> m_nFormWorkTemplateID;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20190731)
		{
			ar >> m_nBuildingInfoID;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20140522 && MSVersionInfo::GetCurrentVersion() < 20180919)
		{
			if (ma_UserFormWork.size() > 0)
			{
				if (GetType() == MSElement::msStairG)
				{
					FormWorkDeleteByDir(_T("Y"));
				}
				else if (GetType() == MSElement::msHandRailG)
				{
					FormWorkDeleteByDir(_T("F"));
					FormWorkDeleteByDir(_T("E"));
				}
			}
		}

		if (MSVersionInfo::GetCurrentVersion() < 20181119)
		{
			if (GetType() == MSElement::msHandRailG)
			{
				for (int i = ma_AreaInfo.size() - 1; i >= 0; i--)
				{
					if (ma_AreaInfo[i].use_user_input && ma_AreaInfo[i].calcType == MSHandRailG::LowWallCalcType::All)
					{
						ma_AreaInfo.erase(ma_AreaInfo.begin() + i);
					}
				}
			}
		}

		if (MSVersionInfo::GetCurrentVersion() < 20181127)
		{
			SubMemberMaterialValidCheck();
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20200911)
		{
			m_pViewDisplaySetting->Serialize(ar);
		}

		int nConstructionPhase = 0;
		if (IsMVAddToBHQ())
			ar >> nConstructionPhase;
		m_eConstructionPhase = (eConstructionPhase)nConstructionPhase;

		if (IsMVAddToBHQ())
			ar >> m_nMainCompoGID;

		if (MSVersionInfo::GetCurrentVersion() >= 20240626)
		{
			ar >> m_IsUserFloor;
			ar >> m_nUserFloorID;
		}
	}
}

void MSCompoG::FormWorkDeleteByDir(CString dir)
{
	for (int index = 0; index < ma_UserFormWork.size(); index++)
	{
		if (ma_UserFormWork[index].dir != dir) continue;
		if (ma_UserFormWork[index].formWorkName != _T("")) continue;

		ma_UserFormWork.erase(ma_UserFormWork.begin() + index);
		break;
	}

	for (int index = 0; index < ma_AreaInfo.size(); index++)
	{
		if (ma_AreaInfo[index].dir != dir) continue;
		if (ma_AreaInfo[index].formWorkName != _T("")) continue;

		ma_AreaInfo.erase(ma_AreaInfo.begin() + index);
		break;
	}
}

void MSCompoG::Serialize_EachCompoM(CArchive& ar)
{
	int appMode = AppGlobalOptionWrapper::Instance()->GetAppMode();
	if (ar.IsStoring())
		ar << appMode;
	else
	{
		if (MSVersionInfo::GetCurrentVersion() >= 20240425
			|| MSVersionInfo::IsOldTotalVersion())
			ar >> appMode;
	}

	if (appMode == AppGlobalOptionWrapper::Rebar)
		Serialize_EachCompoM_Rebar(ar);
	else if (appMode == AppGlobalOptionWrapper::Formwork || appMode == AppGlobalOptionWrapper::Finish)
		Serialize_EachCompoM_Formwork(ar);
}

void MSCompoG::Serialize_EachCompoM_Rebar(CArchive& ar)
{

}

void MSCompoG::Serialize_EachCompoM_Formwork(CArchive& ar)
{
	if (ar.IsStoring())
	{
		long MType = mp_CompoM->GetType();
		ar << MType;
		mp_CompoM->Serialize(ar);
	}
	else
	{
		MSBaseBuilding* pBldg = GetBuilding();
		long mType = 0;
		ar >> mType;

		if ((IsBHFMode() && MSVersionInfo::GetCurrentVersion() < 20240425))
		{
			if (mType == MSCompoM::msBeamM)
				mType = MSCompoM::msFWBeamM;
			else if (mType == MSCompoM::msColumnM)
				mType = MSCompoM::msFWColumnM;
			else if (mType == MSCompoM::msBWallM)
				mType = MSCompoM::msFWBWallM;
			else if (mType == MSCompoM::msSlabM)
				mType = MSCompoM::msFWSlabM;
		}

		mp_CompoM = pBldg->CreateNewCompoM(mType);
		mp_CompoM->Serialize(ar);
	}
}

bool MSCompoG::CheckValidConnections(bool bRemove)
{
	//BH-1729
	CString strErr;
	bool bRval = true;
	int num = ma_Connections.size();
	for (int i = num - 1; i >= 0; --i)
	{
		MSConnection* pConn = ma_Connections[i];
		if (pConn->IsDeleted())
		{
			ma_Connections.erase(ma_Connections.begin() + i);
			bRval = false;
		}
		else if (pConn->GetFirstCompoG() == nullptr || pConn->GetSecondCompoG() == nullptr)
		{
			strErr.Format(_T("Delete InValidConnection: CompoGID:%d FirstCompoGID:%d SecondCompoGID:%d"), m_ID, pConn->GetFirstCompoGID(), pConn->GetSecondCompoGID());
			LoggerWrapper::Log(_T(""));
			if (bRemove == true)
			{
				pConn->SetDeleted(true);
				ma_Connections.erase(ma_Connections.begin() + i);
			}
			bRval = false;
		}
	}

	return bRval;
}

void MSCompoG::RecoverPointerAfterSerialize(MSFloor* floor)
{
	bool bAddToQuadTree = false;

	if (MSVersionInfo::GetCurrentVersion() >= 20141217)
	{
		if (MSConnection::CONNECTION_SERIALIZE())
		{
			for (int i = 0; i < ma_Connections.size(); ++i)
				ma_Connections[i]->RecoverPointerAfterSerialize(floor);

			// Error Check
			int num = ma_Connections.size();
			for (int i=num-1 ; i>=0 ; --i)
			{
				if (ma_Connections[i]->GetFirstCompoG() == NULL || ma_Connections[i]->GetSecondCompoG() == NULL)
				{
					ma_Connections.erase( ma_Connections.begin() + i);
					//ASSERT(0);
				}
			}

			if (GetType()==MSElement::msSlabG)
			{
				// Refactoring - Moved to MSSlabG::RecoverPointerAfterSerialize(MSFloor* floor)

				/*
				MSSlabG* slab = dynamic_cast<MSSlabG*>(this);
				vector<MSCompoG*> sub_list;
				for (int i=0 ; i<slab->ma_EdgeWall.size() ; ++i)
					sub_list.push_back((MSCompoG*)slab->ma_EdgeWall[i]);
				for (int i=0 ; i<sub_list.size() ; ++i)
				{
					for (int k=0 ; k<sub_list[i]->ma_Connections.size() ; ++k)
						sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
				}

				sub_list.clear();
				for (int i = 0; i<slab->ma_Haunch.size(); ++i)
					sub_list.push_back((MSCompoG*)slab->ma_Haunch[i]);
				for (int i = 0; i<sub_list.size(); ++i)
				{
					for (int k = 0; k<sub_list[i]->ma_Connections.size(); ++k)
						sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
				}
				*/
			}

			bAddToQuadTree = true;
		}
	}

	// 파일 오픈후 FloorQT에 입력한다.(예전에 Add로직을 사용하면서 입력하지 않았음)
	if (bAddToQuadTree)
		floor->mp_QT->AddToLeaves(this, true);

	if(GetType() == MSElement::msWallG)
		floor->InsertLowLevelDif2AWallMemberByLevelZone((MSWallMemberG*)this);

	AddCompoGZLevelInFloor();
}

void MSCompoG::CalculateConnectionsAngle()
{
	for (int i = 0; i < ma_Connections.size(); ++i)
		ma_Connections[i]->CalculateContainedAngle();
}

void MSCompoG::CalculateLinkMemberTrimOption()
{
	for (int i = 0; i < ma_Connections.size(); ++i)
		ma_Connections[i]->CalculateLinkMemberTrimOption();
}

void MSCompoG::FixCalculateConnectionsAngle()
{
	// [BH-1358] MSConnection::m_dInnerDeg 음수인 경우 CalculateContainedAngle(); 호출 하여 다시 계산해야함
	for (int i = 0; i < ma_Connections.size(); ++i)
	{
		if (ma_Connections[i]->GetInnerDeg() < 0)
			ma_Connections[i]->CalculateContainedAngle();
		else if(ma_Connections[i]->IsErrorPos())
			ma_Connections[i]->CalculateContainedAngle();
	}
}

void MSCompoG::RecoverConnectionAfterCopyNPaste(map<long, MSCompoG*>& OldCompoGMap)
{
	for (int i=0 ; i<ma_Connections.size() ; ++i)
		ma_Connections[i]->RecoverPointerAfterCopyNPaste(OldCompoGMap);

	// Error Check
	int num = ma_Connections.size();
	for (int i=num-1 ; i>=0 ; --i)
	{
		if (ma_Connections[i]->GetFirstCompoG() == NULL || ma_Connections[i]->GetSecondCompoG() == NULL)
		{
			ma_Connections.erase( ma_Connections.begin() + i);
			//ASSERT(0);
		}
	}
}

void MSCompoG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSElement::Init(bInConstructor);

	mp_CompoM = NULL;
	mp_Owner = NULL;
	mp_Source = NULL;
	mp_MsgObject = NULL;
	m_MarkGID = 0;
	mp_Boundary = new GM2DPolyline(TRUE);
	mp_TrimmedBoundary = NULL;
	mp_TrimmedBoundary_Acis = NULL;
	m_IsUserBoundarySet = false;
	mp_UserBoundary = NULL;
	m_DoUpdateTrimmedProfile = false;
	
	m_startDate = MSConstructionZone::InitialDate();
	m_finishDate = MSConstructionZone::InitialDate();
	m_IsScheduleAssigned = false;
	m_IsQuantityReplaced = false;

	m_IsDummy = false;
	m_IsReverseBox = false;
	m_IsRigidMember = false;
	m_LabelEcc.SetXY(0, 0);
	m_nCmdCnt = 0;
	ma_Clone.clear();
	DeleteVectorArr();
	DeleteVecInput();

	m_nRebarTemplateID = -1;
	m_nFormWorkTemplateID = -1;

	m_nEntityTag_Cur = -1;
	m_nEntityTag_Org = -1;
	m_nEntityTag_SrcSteel = -1;
	m_nEntityTag_PC = -1;

	m_nBuildingInfoID = -1;

	m_pViewDisplaySetting = new ViewDisplaySetting();

	m_IsTempCompoG = false;
	m_eConstructionPhase = eConstructionPhase::concurrentConstruction;
	m_nMainCompoGID = 0;

	m_IsUserFloor = false;
	m_nUserFloorID = 0;
}

void MSCompoG::DeleteVectorArr()
{
	vector<GM2DVector*>::iterator itVec;
	for(itVec = ma_Vec.begin(); itVec != ma_Vec.end(); itVec++)
	{
		delete (*itVec);
	}
	ma_Vec.clear();
}

void MSCompoG::DeleteTrimmedBoundary()
{
	if(mp_TrimmedBoundary != nullptr)
	delete mp_TrimmedBoundary;
	mp_TrimmedBoundary = nullptr;
}

void MSCompoG::DeleteTrimmedBoundary_Acis()
{
	if (mp_TrimmedBoundary_Acis != nullptr)
		delete mp_TrimmedBoundary_Acis;
	mp_TrimmedBoundary_Acis = nullptr;
}

MSObject* MSCompoG::Copy( bool bNewID /*= true*/ )
{
	MSCompoG* pNewObj;

	pNewObj = new MSCompoG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSCompoG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == nullptr) return;
	MSElement::CopyFromMe(pSource_in, bNewID);

	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() == msNULL) { ASSERT(0); return; }
	MSCompoG* pSource = (MSCompoG*)pSource_in;

	if(pSource)
	{
		if (IsEachCompoM() == false && pSource->IsEachCompoM() == false)
			mp_CompoM = pSource->mp_CompoM;
		else if (IsEachCompoM() == true && pSource->IsEachCompoM() == true)
			mp_CompoM->CopyFromMe(pSource->mp_CompoM);
		else if (IsEachCompoM() == true && pSource->IsEachCompoM() == false)
		{
			delete mp_CompoM;
			mp_CompoM = pSource->mp_CompoM;
		}
		else if (IsEachCompoM() == false && pSource->IsEachCompoM() == true)
		{
			mp_CompoM = pSource->mp_CompoM->mp_Owner->CreateNewCompoM(pSource->mp_CompoM->GetType());
			mp_CompoM->CopyFromMe(pSource->mp_CompoM);
		}
		
		mp_Owner = pSource->mp_Owner;
		if(pSource->mp_Source)
			mp_Source = pSource->mp_Source;
	}

	DeleteVectorArr();
	for (long iJoint = 0; iJoint < pSource->ma_Vec.size(); iJoint++)
		ma_Vec.push_back((GM2DVector*)pSource->ma_Vec[iJoint]->Copy());

	DeleteVecInput();
	for(long i = 0 ; i < pSource->ma_Vec_Input.size() ; i++)
		ma_Vec_Input.push_back((GM2DVector*)pSource->ma_Vec_Input[i]->Copy());

	m_MarkGID = pSource->m_MarkGID;

	ma_VolumeInfo.clear();
	ma_VolumeInfo = pSource->ma_VolumeInfo;
	ma_AreaInfo.clear();
	ma_AreaInfo = pSource->ma_AreaInfo;
	ma_UserFormWork.clear();
	ma_UserFormWork = pSource->ma_UserFormWork;
	ma_UserRebar.clear();
	ma_UserRebar = pSource->ma_UserRebar;

	m_IsScheduleAssigned = pSource->m_IsScheduleAssigned;
	m_startDate = pSource->m_startDate;
	m_finishDate = pSource->m_finishDate;
	m_IsReverseBox = pSource->GetReverseBox();
	m_IsRigidMember = pSource->GetRigidMember();
	m_IsQuantityReplaced = pSource->m_IsQuantityReplaced;
	m_LabelEcc = pSource->GetLabelEcc();

	if (pSource->mp_Boundary != NULL)
	{
		delete mp_Boundary;
		mp_Boundary = new GM2DPolyline(TRUE);
		mp_Boundary->CopyFrom(pSource->mp_Boundary);
	}

	DeleteTrimmedBoundary();

	if(pSource->mp_TrimmedBoundary != NULL)
	{
		mp_TrimmedBoundary = new GM2DPolyline(TRUE);
		mp_TrimmedBoundary->CopyFrom(pSource->mp_TrimmedBoundary);
	}
	ma_TrimmedBoundaryVisible = pSource->ma_TrimmedBoundaryVisible;

	DeleteTrimmedBoundary_Acis();

	if (pSource->mp_TrimmedBoundary_Acis != NULL)
	{
		mp_TrimmedBoundary_Acis = new GM2DPolyline(TRUE);
		mp_TrimmedBoundary_Acis->CopyFrom(pSource->mp_TrimmedBoundary_Acis);
	}

	m_nRebarTemplateID = pSource->m_nRebarTemplateID;
	m_nFormWorkTemplateID = pSource->m_nFormWorkTemplateID;

	DeleteAddRebars();
	for (auto it = pSource->mm_AddRebar.begin(); it != pSource->mm_AddRebar.end(); it++)
	{
		MSAddRebarBase* pAddRebar = MSAddRebarBase::CreateAddRebar(it->second->GetType());
		if (pAddRebar == nullptr)	continue;

		pAddRebar->CopyFromMe(it->second);
		Add(pAddRebar);
	}

	m_nBuildingInfoID = pSource->m_nBuildingInfoID;

	if (pSource->m_pViewDisplaySetting != NULL)
	{
		delete m_pViewDisplaySetting;
		m_pViewDisplaySetting = new ViewDisplaySetting();
		m_pViewDisplaySetting->CopyFromMe(pSource->m_pViewDisplaySetting);
	}

	
	m_IsTempCompoG = pSource->GetIsTempCompoG();
	m_eConstructionPhase = pSource->GetConstructionPhase();
	m_nMainCompoGID = pSource->m_nMainCompoGID;

	m_IsUserFloor = pSource->m_IsUserFloor;
	m_nUserFloorID = pSource->m_nUserFloorID;

	//ma_ContainedMemberID = pSource->ma_ContainedMemberID;
	/*
	if(pSource)
	{
		mp_CompoM = pSource->mp_CompoM;
		mp_Owner = pSource->mp_Owner;
		if(pSource->mp_Source)
			mp_Source = pSource->mp_Source;
// 		else
// 			mp_Source = pSource;

		InitJoint();
        long nJoint = pSource->ma_Joint.size();
        if(GetType()==MSElement::msSlabG || GetType()==MSElement::msStairG)
        {
            for(long iJoint=0; iJoint<nJoint; iJoint++)
                ma_Joint.push_back(pSource->ma_Joint[iJoint]);
            nJoint = pSource->ma_Vec.size();
            for(long iJoint=0; iJoint<nJoint; iJoint++)
                ma_Vec.push_back((GM2DVector*)pSource->ma_Vec[iJoint]->Copy());
        }
        else
        {
            for(long iJoint=0; iJoint<nJoint; iJoint++)
                ma_Joint[iJoint] = pSource->ma_Joint[iJoint];
            nJoint = pSource->ma_Vec.size();
            for(long iJoint=0; iJoint<nJoint; iJoint++)
                ma_Vec[iJoint]->SetXY(pSource->ma_Vec[iJoint]->m_X, pSource->ma_Vec[iJoint]->m_Y);
        }
		m_MarkID = pSource->m_MarkID;
	}
	*/
	
}

bool MSCompoG::SetCompoM( long MID, bool bUpdateVecInput)
{
	MSBaseBuilding* pBuilding = GetBuilding();
	if(!pBuilding)
		return false;
	mp_CompoM = pBuilding->GetCompoM(MID);
	if(mp_CompoM)
		return true;
	else
		return false;
}

bool MSCompoG::SetCompoM(MSCompoM* pCompoM, bool bUpdateVecInput)
{
	if(pCompoM != NULL)
	{
		mp_CompoM = pCompoM;
		return true;
	}

	return false;
}

MSBaseBuilding* MSCompoG::GetBuilding()
{
	MSBaseBuilding* pBldg = nullptr;
	if (mp_Owner == nullptr)
	{
		//pBldg = MSBaseBuilding::GetActiveBuilding();
	}
	else
	{
		if (mp_Owner->GetType() == MSElement::msBuilding)
			pBldg = (MSBaseBuilding*)mp_Owner;
		else
			pBldg = mp_Owner->GetBuilding();
	}
	return pBldg;
}

void MSCompoG::GetSecPolyline(GM2DPolyline& APGon )
{
	return mp_CompoM->GetSecPolyline(APGon);

}

void MSCompoG::GetHeaderPolyline(GM2DPolyline& APGon)
{
	return mp_CompoM->GetHeaderPolyline(APGon);
}

vector<GM2DPolyline> MSCompoG::GetSubHeaderPolyline()
{
	return mp_CompoM->GetSubHeaderPolyline();
}

/*
void MSCompoG::GetBoundary( GM2DPolygon& APGon )
{
	MakeBoundary();
	long numVec = ma_BoundaryPoints.size();
	for(long iVec = 0;iVec<numVec;iVec++)
		APGon.AddCopy(ma_BoundaryPoints[iVec]);
}
*/

/*
BOOL MSCompoG::VisibleInRect( GM2DRectangle* pRect)
{
	GM2DPolyline ThePoly(TRUE);
	GetBoundary(ThePoly);

	GM2DPolyline UrPoly(TRUE);
	pRect->GetBoundPolyline(UrPoly);

	bool bRVal = ThePoly.OverlapEx(UrPoly);

	return bRVal;
}
*/

void MSCompoG::GetBoundary( GM2DPolyline& APolyLine )
{
	bool Result = MakeBoundary();
	if(Result==false) 
		return;
	/*
	long numVec = ma_BoundaryPoints.size();
	for(long iVec = 0;iVec<numVec;iVec++)
	{	
		APolyLine.AddCopy(ma_BoundaryPoints[iVec]);
		APolyLine.ma_Bulge.Add(0.0);
	}
	APolyLine.AddCopy(ma_BoundaryPoints[0]);
	*/
	APolyLine.CopyFrom(mp_Boundary);
	//APolyLine.DeleteDuplicatedPoints();
	//APolyLine.CheckAndFixFoldedEdges(DTOL_GM);
}

void MSCompoG::GetBoundaryNSubMember(GM2DPolyline& APolyLine)
{
	GetBoundary(APolyLine);
	return;
}
double MSCompoG::GetFloorLevel()
{
	if (mp_Owner->GetType() != MSElement::msFloor)
		return 0.;

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	return pFloor->GetFloorLevel();
}

double MSCompoG::GetNextFloorLevel()
{
	if (mp_Owner->GetType() != MSElement::msFloor)
		return 0.;

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	MSFloor* pUpperFloor = pFloor->GetUpperFloor();
	if (pUpperFloor == NULL)
		return GetFloorLevel() + pFloor->GetHeight();

	return pUpperFloor->GetFloorLevel();
}

double MSCompoG::GetSlabLevel()
{
	if(mp_Owner == nullptr || mp_Owner->GetType() != MSElement::msFloor)
		return 0.;

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	return pFloor->GetSlabLevel();
}

double MSCompoG::GetNextSlabLevel()
{
	if(mp_Owner->GetType() != MSElement::msFloor)
		return 0.;

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	MSFloor* pUpperFloor = pFloor->GetUpperFloor();
	if (pUpperFloor == NULL)
		return GetSlabLevel() + pFloor->GetSlabHeight();
	
	return pUpperFloor->GetSlabLevel();
}

bool MSCompoG::ModifyClones()
{
	long nClone = ma_Clone.size();
	for(long iClone=0; iClone<nClone; iClone++)
	{
		MSCompoG* pCloneMem = ma_Clone[iClone];

		pCloneMem->CopyFromMe(this);
		if(pCloneMem->mp_Owner && pCloneMem->mp_Owner->IsAssembly())
		{
			SetCloneJoint((MSAssembly*)(pCloneMem->mp_Owner));
		}
	}
	return true;
}

void MSCompoG::SetOwner(MSContainer* val)
{
	if(IS_USE_NATIVE_CMD)
		CmdNewModifyVariable<MSContainer*>::Set(&mp_Owner, val);
	else
		mp_Owner = val;
}

bool MSCompoG::SetCloneJoint( MSAssembly* pCloneAssem )
{
    if(!pCloneAssem)
        return false;

    //for(long iJoint = 0;iJoint<nJoint;iJoint++)
    //    ma_Joint[iJoint]->TrimMembersEx(false);

    return true;
}

// Jissi rename for insert Vec
GM2DVector* MSCompoG::InsertVec( long Index, GM2DVector* pVec)
{
	if(Index >= ma_Vec.size())
		return NULL;
	GM2DVector* pOldVec = ma_Vec[Index];
	ma_Vec.insert(ma_Vec.begin()+Index, pVec);
	return pOldVec;
}

// Jissi rename for insert Vec
void MSCompoG::ReplaceVec( long Index, GM2DVector* pVec)
{
    if(Index >= ma_Vec.size())
        return;
    GM2DVector* pOldVec = ma_Vec[Index];
    ma_Vec[Index]= pVec;
    delete pOldVec;
}

GM2DVector* MSCompoG::GetVec( long Index )
{
	if(Index >= ma_Vec.size())
		return NULL;
	return ma_Vec[Index];
}


MDMember* MSCompoG::GetRebarM()
{
	if(mp_CompoM != NULL)
		return	mp_CompoM->GetRebarM();

	return NULL;
}

double MSCompoG::GetDepth()
{
	if(mp_CompoM)
		return mp_CompoM->GetDepth();
	return 0;
}

double MSCompoG::GetWidth()
{
	if(mp_CompoM)
		return mp_CompoM->GetWidth();
	return 0;
}

double MSCompoG::GetHeight()
{
	if (mp_CompoM)
		return mp_CompoM->GetHeight();
	return 0;
}

double MSCompoG::GetPcThick()
{
	if (mp_CompoM)
		return mp_CompoM->GetPcThick();
	return 0;
}

double MSCompoG::GetPcWidth()
{
	if (mp_CompoM)
		return mp_CompoM->GetPcWidth();
	return 0;
}

double MSCompoG::GetPcDepth()
{
	if (mp_CompoM)
		return mp_CompoM->GetPcDepth();
	return 0;
}

double MSCompoG::GetThick()
{
	if(mp_CompoM)
		return mp_CompoM->GetThick();
	return 0;

}

double MSCompoG::GetCover()
{
	if(mp_CompoM)
	{
		MDMember* pReBarM = mp_CompoM->GetRebarM();
		if(pReBarM)
			return pReBarM->GetCover();
	}
	return 0;
}

double MSCompoG::GetEndLevel()
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	return GetNextSlabLevel()+GetRelativeEndLevel();

}

double MSCompoG::GetStartLevel()
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	return GetNextSlabLevel()+GetRelativeStartLevel();

}

bool MSCompoG::CheckHeight(bool IsUseGroupName)
{
	return true;
}

double MSCompoG::GetLevelZoneLevel( double dX,double dY )
{
	if (mp_Owner == NULL || mp_Owner->GetType() != MSElement::msFloor)
		return 0.;
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	return pFloor->GetLevel4LevelZone(dX, dY);
}

BOOL MSCompoG::AddErrorObject(long nErrorCode, long nLevel, CString strGroup, CString strMsg)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	if(pMessageContainer == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	if(mp_MsgObject == NULL)
	{
		XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(nErrorCode, nLevel, strMsg, XErrorObject::MSElementIDSelect);

		if(pMsgObject == NULL)
		{
			ASSERT(0);
			return FALSE;
		}
		pMessageContainer->AddErrorObject(pMsgObject);
		mp_MsgObject = pMsgObject;
	}
//	else
		//mp_MsgObject->m_Message.Add(_T("\r\n"));

	mp_MsgObject->SetGroupName(strGroup);
	mp_MsgObject->m_Message.Add(strMsg);
	mp_MsgObject->AddCompoGID(GetID());
	mp_MsgObject->SetShowSelectCompoG(TRUE);

	return TRUE;
}

void MSCompoG::GetTopBotLevelArr(MSCompoG* pUrCompoG, CheckModelOpt &CheckFlag, bool bCheckBigVal, bool bCheckLevelZone, vector<TopBotLevelStruct>& MyLevelArr, vector<TopBotLevelStruct>& UrLevelArr)
{
	double MyTopLevel = 0.0, MyBotLevel = 0.0;
	double UrTopLevel = 0.0, UrBotLevel = 0.0;

	bool IsMyLinkMember = false;
	if(IsLinkMember() == TRUE)	IsMyLinkMember = true;
	bool IsUrLinkMember = false;
	if(pUrCompoG->IsLinkMember()==TRUE)	IsUrLinkMember = true; 

	Type_MSCompoG MyType = GetType();
	Type_MSCompoG UrType = pUrCompoG->GetType();

	if(bCheckLevelZone == false ||
		(HasLevelDif() == false && pUrCompoG->HasLevelDif() == false)
		)
	{
		GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, false);
		pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, false);

		TopBotLevelStruct MyLevel(MyTopLevel, MyBotLevel);
		TopBotLevelStruct UrLevel(UrTopLevel, UrBotLevel);
		MyLevelArr.push_back(MyLevel);
		UrLevelArr.push_back(UrLevel);
	}
	else
	{
		//bool bCheckLevelZone = false;
		MSFloor* pMyFloor = GetFloor();
		MSFloor* pUrFloor = pUrCompoG->GetFloor();
		if(pMyFloor != pUrFloor)
			bCheckLevelZone = true;
		// 체크할 부재의 층이 다를 경우 bCheckLevelZone을 True로 해준다.

		if(IsMyLinkMember && IsUrLinkMember)
		{
			MSLinkMemberG* pMyLinkG = (MSLinkMemberG*)this;
			MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pUrCompoG;
			GM2DVector Cross;
			double dMyLoc, dUrLoc, dMyLocTol, dUrLocTol;
			long nRval = pMyLinkG->CheckIntersectLinkMember(pUrLinkG, Cross, dMyLoc, dUrLoc, dMyLocTol, dUrLocTol);
			if (nRval == 1)
			{
				pMyLinkG->GetZLevelsbyLoc(MyTopLevel, MyBotLevel, dMyLoc);
				pUrLinkG->GetZLevelsbyLoc(UrTopLevel, UrBotLevel, dUrLoc);
			}
			else
			{
				GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, bCheckLevelZone);
				pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, bCheckLevelZone);
			}
		}
		else
		{
			// 		GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, bCheckLevelZone);
			// 		pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, bCheckLevelZone);
			// 부재 연결관계 찾을시 LevelZone 값을 검토 안하도록 변경
			if(IsMyLinkMember && pUrCompoG->IsPointMember())
			{
				MSLinkMemberG* pMyLinkG = (MSLinkMemberG*)this;
				shared_ptr<GM2DCurve> pMyCurve = pMyLinkG->GetCenCurve();
				GM2DVector* pVec = pUrCompoG->GetSVec();

				double dMyLoc = pMyCurve->OrthoBaseLocValue(*pVec);
				pMyLinkG->GetZLevelsbyLoc(MyTopLevel, MyBotLevel, dMyLoc);
				pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal);
			}
			else if(IsUrLinkMember && IsPointMember())
			{
				MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pUrCompoG;
				shared_ptr<GM2DCurve> pUrCurve = pUrLinkG->GetCenCurve();
				GM2DVector* pVec = GetSVec();

				double dUrLoc = pUrCurve->OrthoBaseLocValue(*pVec);
				pUrLinkG->GetZLevelsbyLoc(UrTopLevel, UrBotLevel, dUrLoc);
				GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal);
			}
			else
			{
				if(MyType == MSElement::msSlabG)
				{
					MSSlabG* pMySlab = (MSSlabG*)this;
					if(pUrCompoG->IsPointMember())
					{
						GM2DVector* pVec = pUrCompoG->GetSVec();

						pMySlab->GetZLevels(*pVec, MyTopLevel, MyBotLevel, true);
						pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal);
					}
					else if(pUrCompoG->IsLinkMember())
					{
						MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pUrCompoG;
						double dCheckDist = DTOL5;

						vector<GM2DCurve*> NewCurveArr;
						if(pUrLinkG->GetOverLapPolyLineCenLoc(pMySlab->GetProfile(), NewCurveArr))
						{
							shared_ptr<GM2DCurve> pCenCurve = pUrLinkG->GetCenCurve();
							for(long i = 0 ; i < NewCurveArr.size() ; i++)
							{
								GM2DCurve* pCurve = NewCurveArr[i];
								double dLoc = pCenCurve->OrthoBaseLocValue(pCurve->m_P0);
								pUrLinkG->GetZLevelsbyLoc(UrTopLevel, UrBotLevel, dLoc - dCheckDist);
								pMySlab->GetZLevels(pCurve->m_P0, MyTopLevel, MyBotLevel, true);

								TopBotLevelStruct MyLevel0(MyTopLevel, MyBotLevel);
								TopBotLevelStruct UrLevel0(UrTopLevel, UrBotLevel);
								MyLevelArr.push_back(MyLevel0);
								UrLevelArr.push_back(UrLevel0);

								dLoc = pCenCurve->OrthoBaseLocValue(pCurve->m_P1);
								pUrLinkG->GetZLevelsbyLoc(UrTopLevel, UrBotLevel, dLoc + dCheckDist);
								pMySlab->GetZLevels(pCurve->m_P1, MyTopLevel, MyBotLevel, true);

								TopBotLevelStruct MyLevel1(MyTopLevel, MyBotLevel);
								TopBotLevelStruct UrLevel1(UrTopLevel, UrBotLevel);
								MyLevelArr.push_back(MyLevel1);
								UrLevelArr.push_back(UrLevel1);
							}
							GMObject::DeleteGMObject(NewCurveArr);
						}
						else
						{
							pUrLinkG->GetZLevels(UrTopLevel, UrBotLevel, true);
							pMySlab->GetZLevels(MyTopLevel, MyBotLevel, true);
						}
					}
					else if(UrType == MSElement::msSlabG)
					{
						MSSlabG* pUrSlab = (MSSlabG*)pUrCompoG;

						pMySlab->GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, bCheckLevelZone);
						pUrSlab->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, bCheckLevelZone);

						TopBotLevelStruct MyLevel(MyTopLevel, MyBotLevel);
						TopBotLevelStruct UrLevel(UrTopLevel, UrBotLevel);
						MyLevelArr.push_back(MyLevel);
						UrLevelArr.push_back(UrLevel);
						// 속도 저하 오래 걸림 더 좋은 방법을 생각...
// 						GM2DPolyline MyPoly(TRUE);
// 						GM2DPolyline UrPoly(TRUE);
// 						pMySlab->GetProfile_InSplitPointLevelZone(MyPoly);
// 						pUrSlab->GetProfile_InSplitPointLevelZone(UrPoly);
//
// 						vector<GM2DVector> OverLapVec;
// 						vector<GM2DCurve*> OverLapCurve;
// 						OverLapVec = MyPoly.IntersectEx(UrPoly, &OverLapCurve);
// 
// 						if(OverLapVec.size() > 0)
// 						{
// 							for(long i = 0 ; i < OverLapVec.size() ; i++)
// 							{
// 								pMySlab->GetZLevels(OverLapVec[i], MyTopLevel, MyBotLevel, bCheckBigVal);
// 								pUrSlab->GetZLevels(OverLapVec[i], UrTopLevel, UrBotLevel, bCheckBigVal);
// 								TopBotLevelStruct MyLevel(MyTopLevel, MyBotLevel);
// 								TopBotLevelStruct UrLevel(UrTopLevel, UrBotLevel);
// 								MyLevelArr.push_back(MyLevel);
// 								UrLevelArr.push_back(UrLevel);
// 							}
// 						}
// 						else
// 						{
// 							pMySlab->GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, bCheckLevelZone);
// 							pUrSlab->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, bCheckLevelZone);
// 
// 							TopBotLevelStruct MyLevel(MyTopLevel, MyBotLevel);
// 							TopBotLevelStruct UrLevel(UrTopLevel, UrBotLevel);
// 							MyLevelArr.push_back(MyLevel);
// 							UrLevelArr.push_back(UrLevel);
// 						}
// 						for(long i = 0 ; i < OverLapCurve.size() ; i++)
// 							delete OverLapCurve[i];
// 						OverLapCurve.clear();
					}
					else
					{
						GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, bCheckLevelZone);
						pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, bCheckLevelZone);
					}
				}
				else if(UrType == MSElement::msSlabG)	
				{
					MSSlabG* pUrSlab = (MSSlabG*)pUrCompoG;
					if(IsPointMember())
					{
						GM2DVector* pVec = GetSVec();
						GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal);
						pUrSlab->GetZLevels(*pVec, UrTopLevel, UrBotLevel, true);
					}
					else if(IsLinkMember())
					{
						MSLinkMemberG* pMyLinkG = (MSLinkMemberG*)this;
						double dCheckDist = DTOL5;

						vector<GM2DCurve*> NewCurveArr;
						if(pMyLinkG->GetOverLapPolyLineCenLoc(pUrSlab->GetProfile(), NewCurveArr))
						{
							shared_ptr<GM2DCurve> pCenCurve = pMyLinkG->GetCenCurve();
							for(long i = 0 ; i < NewCurveArr.size() ; i++)
							{
								GM2DCurve* pCurve = NewCurveArr[i];
								double dLoc = pCenCurve->OrthoBaseLocValue(pCurve->m_P0);
								pMyLinkG->GetZLevelsbyLoc(MyTopLevel, MyBotLevel, dLoc - dCheckDist);
								pUrSlab->GetZLevels(pCurve->m_P0, UrTopLevel, UrBotLevel, true);

								TopBotLevelStruct MyLevel0(MyTopLevel, MyBotLevel);
								TopBotLevelStruct UrLevel0(UrTopLevel, UrBotLevel);
								MyLevelArr.push_back(MyLevel0);
								UrLevelArr.push_back(UrLevel0);

								dLoc = pCenCurve->OrthoBaseLocValue(pCurve->m_P1);
								pMyLinkG->GetZLevelsbyLoc(MyTopLevel, MyBotLevel, dLoc + dCheckDist);
								pUrSlab->GetZLevels(pCurve->m_P1, UrTopLevel, UrBotLevel, true);

								TopBotLevelStruct MyLevel1(MyTopLevel, MyBotLevel);
								TopBotLevelStruct UrLevel1(UrTopLevel, UrBotLevel);
								MyLevelArr.push_back(MyLevel1);
								UrLevelArr.push_back(UrLevel1);
							}

							GMObject::DeleteGMObject(NewCurveArr);
						}
						else
						{
							pMyLinkG->GetZLevels(MyTopLevel, MyBotLevel, true);
							pUrSlab->GetZLevels(UrTopLevel, UrBotLevel, true);
						}

						
					}
					else if(MyType == MSElement::msSlabG)
					{
						MSSlabG* pMySlab = (MSSlabG*)this;
						GM2DPolyline* MyPoly = pMySlab->GetProfile();
						GM2DPolyline* pUrPoly = pUrSlab->GetProfile();

						vector<GM2DVector> OverLapVec;
						vector<GM2DCurve*> OverLapCurve;
						OverLapVec = MyPoly->IntersectEx(*pUrPoly, &OverLapCurve);
						if(OverLapVec.size() > 0)
						{
							for(long i = 0 ; i < OverLapVec.size() ; i++)
							{
								pMySlab->GetZLevels(OverLapVec[i], MyTopLevel, MyBotLevel, bCheckBigVal);
								pUrSlab->GetZLevels(OverLapVec[i], UrTopLevel, UrBotLevel, bCheckBigVal);
								TopBotLevelStruct MyLevel(MyTopLevel, MyBotLevel);
								TopBotLevelStruct UrLevel(UrTopLevel, UrBotLevel);
								MyLevelArr.push_back(MyLevel);
								UrLevelArr.push_back(UrLevel);
							}
						}
						else
						{
							pMySlab->GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, bCheckLevelZone);
							pUrSlab->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, bCheckLevelZone);

							TopBotLevelStruct MyLevel(MyTopLevel, MyBotLevel);
							TopBotLevelStruct UrLevel(UrTopLevel, UrBotLevel);
							MyLevelArr.push_back(MyLevel);
							UrLevelArr.push_back(UrLevel);
						}

						GMObject::DeleteGMObject(OverLapCurve);
					}
					else
					{
						GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, bCheckLevelZone);
						pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, bCheckLevelZone);
					}
				}
				else
				{
					//bCheckLevelZone == true
					GetZLevels(MyTopLevel, MyBotLevel, bCheckBigVal, bCheckLevelZone);
					pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, bCheckBigVal, bCheckLevelZone);
				}
			}
		}
		
		if(!(MyType == MSElement::msSlabG && UrType == MSElement::msSlabG))
		{
			TopBotLevelStruct MyLevel(MyTopLevel, MyBotLevel);
			TopBotLevelStruct UrLevel(UrTopLevel, UrBotLevel);
			MyLevelArr.push_back(MyLevel);
			UrLevelArr.push_back(UrLevel);
		}
	}
}

bool MSCompoG::IsLevelConnectedOverLap(MSCompoG* pUrCompoG, CheckModelOpt &CheckFlag, bool bCheckBigVal /*= false*/)
{
	// 오프닝 예외처리
	if(GetFloor() == pUrCompoG->GetFloor())
	{
		if(GetType() == MSElement::msOpeningG)
		{
			MSOpeningG* pMyOpen = (MSOpeningG*)this;
			if(pMyOpen->GetUseOffset() == false)
				return true;
		}

		if(pUrCompoG->GetType() == MSElement::msOpeningG)
		{
			MSOpeningG* pUrOpenG = (MSOpeningG*)pUrCompoG;
			if(pUrOpenG->GetUseOffset() == false)
				return true;
		}
	}

	vector<TopBotLevelStruct> MyLevelArr;
	vector<TopBotLevelStruct> UrLevelArr;

	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	bool bRval = false;
	bool bCheckLevelZone = true;
	GetTopBotLevelArr(pUrCompoG, CheckFlag, bCheckBigVal, bCheckLevelZone, MyLevelArr, UrLevelArr);

	if(MyLevelArr.size() == 1 && UrLevelArr.size() == 1)
	{
		MyTopLevel = MyLevelArr[0].m_dTopLevel;
		MyBotLevel = MyLevelArr[0].m_dBotLevel;
		UrTopLevel = UrLevelArr[0].m_dTopLevel;
		UrBotLevel = UrLevelArr[0].m_dBotLevel;

		if(fabs(MyBotLevel - UrTopLevel) < DTOL1 || (fabs(UrBotLevel - MyTopLevel) < DTOL1))
			bRval = true;

		//if(!(MyTopLevel <= UrBotLevel + DTOL1) || (UrTopLevel <= MyBotLevel + DTOL1))
		if( (MyTopLevel >= UrTopLevel) && (MyBotLevel <  UrTopLevel) ||
			(MyTopLevel >  UrBotLevel) && (MyBotLevel <= UrBotLevel) ||
			(UrTopLevel >= MyTopLevel) && (UrBotLevel <  MyTopLevel) ||
			(UrTopLevel >  MyBotLevel) && (UrBotLevel <= MyBotLevel) ||
			(MyTopLevel == UrTopLevel) && (MyBotLevel == UrBotLevel)
			)
		{
			bRval = true;
			if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP)) 
			{
				CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP, true);
			}
		}
	}
	else
	{
		for(long i = 0 ; i < MyLevelArr.size() ; i ++)
		{
			for(long j = 0 ; j < UrLevelArr.size() ; j++)
			{
				MyTopLevel = MyLevelArr[i].m_dTopLevel;
				MyBotLevel = MyLevelArr[i].m_dBotLevel;
				UrTopLevel = UrLevelArr[j].m_dTopLevel;
				UrBotLevel = UrLevelArr[j].m_dBotLevel;

				if(fabs(MyBotLevel - UrTopLevel) < DTOL1 || (fabs(UrBotLevel - MyTopLevel) < DTOL1))
					bRval = true;
				
				//if(!(MyTopLevel <= UrBotLevel + DTOL1) || (UrTopLevel <= MyBotLevel + DTOL1))
				if( (MyTopLevel >= UrTopLevel) && (MyBotLevel <  UrTopLevel) ||
					(MyTopLevel >  UrBotLevel) && (MyBotLevel <= UrBotLevel) ||
					(UrTopLevel >= MyTopLevel) && (UrBotLevel <  MyTopLevel) ||
					(UrTopLevel >  MyBotLevel) && (UrBotLevel <= MyBotLevel)||
					(MyTopLevel == UrTopLevel) && (MyBotLevel == UrBotLevel)
					)
				{
					bRval = true;
					if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP)) 
					{
						CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP, true);
					}
				}
			}
		}
	}

	return bRval;
}

bool MSCompoG::IsLevelOverlap( MSCompoG* pUrCompoG, bool bCheckLevelZone)
{
	Type_MSCompoG eMyType = GetType(), eUrType = pUrCompoG->GetType();
	// 오프닝 예외처리
	if(GetFloor() == pUrCompoG->GetFloor())
	{
		if (eMyType == MSElement::msOpeningG && eUrType == MSElement::msOpeningG)
		{
			MSOpeningG* pMyOpen = (MSOpeningG*)this;
			MSOpeningG* pUrOpenG = (MSOpeningG*)pUrCompoG;
			if (pMyOpen->GetUseOffset() == false || pUrOpenG->GetUseOffset() == false)
				return true;

			if (pMyOpen->GetSLevel() > pUrOpenG->GetSLevel() &&
				pMyOpen->GetSLevel() < pUrOpenG->GetELevel())
			{
				return true;
			}
			
			if (pUrOpenG->GetSLevel() > pMyOpen->GetSLevel() &&
				pUrOpenG->GetSLevel() < pMyOpen->GetELevel())
			{
				return true;
			}

			return false;
		}
		else
		{
			if (eMyType == MSElement::msOpeningG)
			{
				MSOpeningG* pMyOpen = (MSOpeningG*)this;
				if (pMyOpen->GetUseOffset() == false)
					return true;
			}

			if (eUrType == MSElement::msOpeningG)
			{
				MSOpeningG* pUrOpenG = (MSOpeningG*)pUrCompoG;
				if (pUrOpenG->GetUseOffset() == false)
					return true;
			}
		}
	}

	vector<TopBotLevelStruct> MyLevelArr;
	vector<TopBotLevelStruct> UrLevelArr;

	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	bool bRval = false;
	bool bCheckBigVal = true;
	CheckModelOpt CheckFlag;

	GetTopBotLevelArr(pUrCompoG, CheckFlag, bCheckBigVal, bCheckLevelZone, MyLevelArr, UrLevelArr);

	if(MyLevelArr.size() == 1 && UrLevelArr.size() == 1)
	{
		MyTopLevel = MyLevelArr[0].m_dTopLevel;
		MyBotLevel = MyLevelArr[0].m_dBotLevel;
		UrTopLevel = UrLevelArr[0].m_dTopLevel;
		UrBotLevel = UrLevelArr[0].m_dBotLevel;
		
		//if(!(MyTopLevel <= UrBotLevel + DTOL1) || (UrTopLevel <= MyBotLevel + DTOL1))
		if( (MyTopLevel >= UrTopLevel) && (MyBotLevel < UrTopLevel) ||
			(MyTopLevel > UrBotLevel) && (MyBotLevel <= UrBotLevel) ||
			(UrTopLevel >= MyTopLevel) && (UrBotLevel < MyTopLevel) ||
			(UrTopLevel >  MyBotLevel) && (UrBotLevel <= MyBotLevel) 
			)
		{
			bRval = true;
			if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP)) 
			{
				CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP, true);
			}
		}
	}
	else
	{
		for(long i = 0 ; i < MyLevelArr.size() ; i ++)
		{
			for(long j = 0 ; j < UrLevelArr.size() ; j++)
			{
				MyTopLevel = MyLevelArr[i].m_dTopLevel;
				MyBotLevel = MyLevelArr[i].m_dBotLevel;
				UrTopLevel = UrLevelArr[j].m_dTopLevel;
				UrBotLevel = UrLevelArr[j].m_dBotLevel;

				//if(!(MyTopLevel <= UrBotLevel + DTOL1) || (UrTopLevel <= MyBotLevel + DTOL1))
				if( (MyTopLevel >= UrTopLevel) && (MyBotLevel < UrTopLevel) ||
					(MyTopLevel > UrBotLevel) && (MyBotLevel <= UrBotLevel) ||
					(UrTopLevel >= MyTopLevel) && (UrBotLevel < MyTopLevel) ||
					(UrTopLevel >  MyBotLevel) && (UrBotLevel <= MyBotLevel) 
					)
				{
					bRval = true;
					if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP)) 
					{
						CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP, true);
					}
				}
			}
		}
	}

	return bRval;
}

bool MSCompoG::IsLevelContain(MSCompoG* pUrCompoG)
{
	// 오프닝 예외처리
	if (GetFloor() == pUrCompoG->GetFloor())
	{
		if (GetType() == MSElement::msOpeningG)
		{
			MSOpeningG* pMyOpen = (MSOpeningG*)this;
			if (pMyOpen->GetUseOffset() == false)
				return true;
		}

		if (pUrCompoG->GetType() == MSElement::msOpeningG)
		{
			MSOpeningG* pUrOpenG = (MSOpeningG*)pUrCompoG;
			if (pUrOpenG->GetUseOffset() == false)
				return true;
		}
	}

	vector<TopBotLevelStruct> MyLevelArr;
	vector<TopBotLevelStruct> UrLevelArr;

	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	bool bRval = false;
	bool bCheckBigVal = true;
	CheckModelOpt CheckFlag;
	bool bCheckLevelZone = false;
	GetTopBotLevelArr(pUrCompoG, CheckFlag, bCheckBigVal, bCheckLevelZone, MyLevelArr, UrLevelArr);

	if (MyLevelArr.size() == 1 && UrLevelArr.size() == 1)
	{
		MyTopLevel = MyLevelArr[0].m_dTopLevel;
		MyBotLevel = MyLevelArr[0].m_dBotLevel;
		UrTopLevel = UrLevelArr[0].m_dTopLevel;
		UrBotLevel = UrLevelArr[0].m_dBotLevel;

		if (((MyTopLevel + DTOL_GM >= UrTopLevel) && (MyBotLevel - DTOL_GM < UrTopLevel)) ||
			((MyTopLevel + DTOL_GM> UrBotLevel) && (MyBotLevel - DTOL_GM <= UrBotLevel))
			)
			bRval = true;
	}
	else
	{
		for (long i = 0; i < MyLevelArr.size(); i++)
		{
			for (long j = 0; j < UrLevelArr.size(); j++)
			{
				MyTopLevel = MyLevelArr[i].m_dTopLevel;
				MyBotLevel = MyLevelArr[i].m_dBotLevel;
				UrTopLevel = UrLevelArr[j].m_dTopLevel;
				UrBotLevel = UrLevelArr[j].m_dBotLevel;

				if ((MyTopLevel + DTOL_GM >= UrTopLevel) && (MyBotLevel - DTOL_GM < UrTopLevel) ||
					(MyTopLevel + DTOL_GM> UrBotLevel) && (MyBotLevel - DTOL_GM <= UrBotLevel)
					)
					bRval = true;
			}
		}
	}

	return bRval;
}

bool MSCompoG::IsLevelConnected(MSCompoG* pUrCompoG)
{
	// pUrCompoG 부재가 This 부재에 Connected 되었는지 판단함
	// This 부재가 Ur부재에 Connected 되었는지 판단하지 않음
	vector<TopBotLevelStruct> MyLevelArr;
	vector<TopBotLevelStruct> UrLevelArr;

	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	bool bRval = false;
	bool bCheckBigVal = true;
	CheckModelOpt CheckFlag;
	bool bCheckLevelZone = false;
	GetTopBotLevelArr(pUrCompoG, CheckFlag, bCheckBigVal, bCheckLevelZone, MyLevelArr, UrLevelArr);

	if (MyLevelArr.size() == 1 && UrLevelArr.size() == 1)
	{
		MyTopLevel = MyLevelArr[0].m_dTopLevel;
		MyBotLevel = MyLevelArr[0].m_dBotLevel;
		UrTopLevel = UrLevelArr[0].m_dTopLevel;
		UrBotLevel = UrLevelArr[0].m_dBotLevel;

		if ( (MyTopLevel <= UrTopLevel && MyTopLevel >= UrBotLevel) || 
			(MyBotLevel <= UrTopLevel && MyBotLevel >=UrBotLevel)
			)
			bRval = true;
	}
	else
	{
		for (long i = 0; i < MyLevelArr.size(); i++)
		{
			for (long j = 0; j < UrLevelArr.size(); j++)
			{
				MyTopLevel = MyLevelArr[i].m_dTopLevel;
				MyBotLevel = MyLevelArr[i].m_dBotLevel;
				UrTopLevel = UrLevelArr[j].m_dTopLevel;
				UrBotLevel = UrLevelArr[j].m_dBotLevel;

				if ((MyTopLevel <= UrTopLevel && MyTopLevel >= UrBotLevel) ||
					(MyBotLevel <= UrTopLevel && MyBotLevel >= UrBotLevel)
					)
					bRval = true;
			}
		}
	}

	return bRval;
}

bool MSCompoG::IsLevelTouch(MSCompoG* pUrCompoG)
{
	// pUrCompoG 부재가 This 부재에 Connected 되었는지 판단함
	// This 부재가 Ur부재에 Connected 되었는지 판단하지 않음
	vector<TopBotLevelStruct> MyLevelArr;
	vector<TopBotLevelStruct> UrLevelArr;

	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	bool bRval = false;
	bool bCheckBigVal = true;
	CheckModelOpt CheckFlag;
	bool bCheckLevelZone = false;
	GetTopBotLevelArr(pUrCompoG, CheckFlag, bCheckBigVal, bCheckLevelZone, MyLevelArr, UrLevelArr);

	if (MyLevelArr.size() == 1 && UrLevelArr.size() == 1)
	{
		MyTopLevel = MyLevelArr[0].m_dTopLevel;
		MyBotLevel = MyLevelArr[0].m_dBotLevel;
		UrTopLevel = UrLevelArr[0].m_dTopLevel;
		UrBotLevel = UrLevelArr[0].m_dBotLevel;

		if (fabs(MyTopLevel - UrBotLevel) < DTOL_GM
			|| fabs(MyBotLevel - UrTopLevel) < DTOL_GM)
			bRval = true;
	}
	else
	{
		for (long i = 0; i < MyLevelArr.size(); i++)
		{
			for (long j = 0; j < UrLevelArr.size(); j++)
			{
				MyTopLevel = MyLevelArr[i].m_dTopLevel;
				MyBotLevel = MyLevelArr[i].m_dBotLevel;
				UrTopLevel = UrLevelArr[j].m_dTopLevel;
				UrBotLevel = UrLevelArr[j].m_dBotLevel;

				if (fabs(MyTopLevel - UrBotLevel) < DTOL_GM
					|| fabs(MyBotLevel - UrTopLevel) < DTOL_GM)
					bRval = true;
			}
		}
	}

	return bRval;
}

bool MSCompoG::IsLevelEquals(MSCompoG* pUrCompoG)
{
	// pUrCompoG 부재가 This 부재에 Connected 되었는지 판단함
	// This 부재가 Ur부재에 Connected 되었는지 판단하지 않음
	vector<TopBotLevelStruct> MyLevelArr;
	vector<TopBotLevelStruct> UrLevelArr;

	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	bool bRval = false;
	bool bCheckBigVal = true;
	CheckModelOpt CheckFlag;
	bool bCheckLevelZone = false;
	GetTopBotLevelArr(pUrCompoG, CheckFlag, bCheckBigVal, bCheckLevelZone, MyLevelArr, UrLevelArr);

	if (MyLevelArr.size() == 1 && UrLevelArr.size() == 1)
	{
		MyTopLevel = MyLevelArr[0].m_dTopLevel;
		MyBotLevel = MyLevelArr[0].m_dBotLevel;
		UrTopLevel = UrLevelArr[0].m_dTopLevel;
		UrBotLevel = UrLevelArr[0].m_dBotLevel;

		if (fabs(MyTopLevel - UrTopLevel) < DTOL_GM
			|| fabs(MyBotLevel - UrBotLevel) < DTOL_GM)
			bRval = true;
	}
	else
	{
		for (long i = 0; i < MyLevelArr.size(); i++)
		{
			for (long j = 0; j < UrLevelArr.size(); j++)
			{
				MyTopLevel = MyLevelArr[i].m_dTopLevel;
				MyBotLevel = MyLevelArr[i].m_dBotLevel;
				UrTopLevel = UrLevelArr[j].m_dTopLevel;
				UrBotLevel = UrLevelArr[j].m_dBotLevel;

				if (fabs(MyTopLevel - UrTopLevel) < DTOL_GM
					|| fabs(MyBotLevel - UrBotLevel) < DTOL_GM)
					bRval = true;
			}
		}
	}

	return bRval;
}

void MSCompoG::ModifyObject(bool isDoit)
{
	MSFloor* pFloor = GetFloor();

	map<long, MSCompoG*>::iterator it = pFloor->mm_CompoG.find(m_ID);
	if (it == pFloor->mm_CompoG.end())	
		return;

	MSElement* pOwnerCompoG = GetOwnerCompoG();
	if (pOwnerCompoG != NULL && pOwnerCompoG->IsCompoG())
	{
		((MSCompoG*)pOwnerCompoG)->ModifyObject(isDoit);
		return;
	}
	SetOverlapFloor();
	
	pFloor->SetRunModifyObjectCompoG(true);
	pFloor->Modify(this);
	pFloor->SetRunModifyObjectCompoG(false);

	return;
}

MSFloor* MSCompoG::GetFloor()
{
	if(mp_Owner == NULL || mp_Owner->GetType() != MSElement::msFloor)
		return NULL;

	return (MSFloor*)mp_Owner;
}

void MSCompoG::RemoveContainedMemberID(long nCompoGID)
{
	for (int i = ma_ContainedMemberID.size() -1 ; i >= 0 ; i--)
	{
		if (ma_ContainedMemberID[i] == nCompoGID)
			ma_ContainedMemberID.erase(ma_ContainedMemberID.begin() + i);
	}
}


void MSCompoG::InitializeParameter()
{
	MSCompoG* pUrCompoG = (MSCompoG*)GetContainingElement();
	if(pUrCompoG != nullptr && pUrCompoG->IsCompoG())
		pUrCompoG->RemoveContainedMemberID(m_ID);

	MSElement::InitializeParameter();
}

bool MSCompoG::GetVolume(double& value, CString& note)
{
	value = 0.0;
	note = "";
	for(int i=0 ; i<ma_VolumeInfo.size() ; ++i)
	{
		value += ma_VolumeInfo[i].value;
		if (ma_VolumeInfo[i].note.GetLength() > 0)
		{
			if (note.GetLength() > 0)
				note += " + ";
			note += ma_VolumeInfo[i].note;
		}
	}
	return true;
}

bool MSCompoG::GetBurimVolume(double& value, CString& note)
{
	value = 0.0;
	note = "";
	
	return true;
}

bool MSCompoG::GetFormWorkArea(vector<CString>& arFWAreaType, vector<double>& arFWArea, vector<CString>& arFormWorkNote)
{
	arFWAreaType.clear();
	arFWArea.clear();
	arFormWorkNote.clear();

	for ( int i=0 ; i<ma_AreaInfo.size() ; ++i)
	{
		CString formWorkName = ma_AreaInfo[i].formWorkName;
		int idx = -1;
		for ( int k=0 ; k<arFWAreaType.size() ; ++k) // find if same type already inserted
		{
			if (arFWAreaType[k] == formWorkName) { idx = k; break; }
		}
		if ( idx == -1 ) // not inserted
		{
			arFWAreaType.push_back(formWorkName);
			arFWArea.push_back(0.0);
			arFormWorkNote.push_back(_T(""));
			idx = arFWAreaType.size() - 1;
		}

		if (idx != -1)
		{
			arFWArea[idx] += ma_AreaInfo[i].value;
			if (ma_AreaInfo[i].note.GetLength() > 0)
			{
				if (arFormWorkNote[idx].GetLength() > 0)
					arFormWorkNote[idx] += " + ";
				arFormWorkNote[idx] += ma_AreaInfo[i].note;
			}
		}
	}

	return true;
}



bool MSCompoG::GetVolumeByCZone(int czone_id, double &rOriginValue, double& value, CString& note, int calcType, vector<CString>& userCategory, vector<CString>& userMaterial, vector<CString>& userNote, vector<double>& userValue, double &srcColInnerVolume, CString& srcColInnerNoteVolume, 
															vector<double>& addedConcVolumes, vector<CString>& addedConcNotes, vector<long>& addedMatIds, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>&matid, double rebarVolume)
{
	//User 물량이 리스트로 들어오기 때문에 해당 부분에 대한 처리가 필요함. (리스트로 반환하도록 하자), userCategory, usermaterial userNote,  userValue
	rOriginValue = 0.0;
	value = 0.0;
	note = "";
	userCategory.clear();
	userMaterial.clear();
	userNote.clear();
	userValue.clear();

	VolumeAreaInfoEMS::FormResult form_result;
	VolumeAreaInfoEMS::FormResult addedForm_result;
	vector<double> arFWOriginArea;
	m_VolumeAreaEMS.GetResultByCZone(czone_id, rOriginValue, value, note, form_result, calcType, srcColInnerVolume, srcColInnerNoteVolume, addedConcVolumes, addedConcNotes, addedForm_result, addedMatIds, subIds, subVolumes, subNotes, matid, arFWOriginArea, rebarVolume);

	if (m_IsQuantityReplaced)
	{
		rOriginValue = 0.0;
		value = 0.0;
		note = _T("");
	}

	
	auto factor = m_VolumeAreaEMS.GetFactorByCzone(czone_id);
	auto cZone = GetCZoneByID(czone_id);

	if (IsLowWall())
	{
		MSFloor* curFloor = (MSFloor*)mp_Owner;
		MSHandRailG* lowWall = dynamic_cast<MSHandRailG*>(this);
		if (!lowWall->GetTopQuantitySeparation())
		{
			if (NULL == curFloor->GetFloorCZone(czone_id))
				return true;
		}
		else
		{
			if (calcType == 1)//top , upper
			{
				curFloor = curFloor->GetUpperFloor();
				if (NULL == curFloor)
					return true;
				if (NULL == curFloor->GetFloorCZone(czone_id))
					return true;
			}
			else //(calcType == 2 //under, 0 : all
			{
				if (NULL == curFloor->GetFloorCZone(czone_id))
					return true;
			}
		}
	}

	// User Input
	for (int i = 0; i < ma_VolumeInfo.size(); ++i)
	{
		if (!ma_VolumeInfo[i].use_user_input) continue;
		if (calcType == 0) //usercalcposition all
		{
			if (!(ma_VolumeInfo[i].calcType == 0 || ma_VolumeInfo[i].calcType == 1 || ma_VolumeInfo[i].calcType == 2)) continue;
		}
		else
		{
			if (ma_VolumeInfo[i].calcType != calcType) continue;
		}

		if (ma_VolumeInfo[i].is_burim == true) continue;
		//if (ma_VolumeInfo[i].czone_id != czone_id) continue;

		userCategory.push_back(ma_VolumeInfo[i].user_Category);
		if (ma_VolumeInfo[i].isUsedStoryMaterial)
		{
			MSMaterial* pMat = nullptr;

			MSHandRailG* lowWallG = dynamic_cast<MSHandRailG*>(this);
			if (lowWallG == nullptr)
				pMat = this->GetMaterial();
			else
			{
				if (calcType == (int)MSHandRailG::LowWallCalcType::Top && lowWallG->GetTopQuantitySeparation() == true)
				{
					// [Q24-1077] 층재지정으로 Root을 처리했을 경우 문제가 발생함 마테리얼 얻는 거기 때문에 Root층인경우는 Root층 마테리얼 사용하도록 수정
					MSFloor* pUpperFloor = this->GetFloor()->GetUpperFloor();
					if (pUpperFloor == nullptr)
						pUpperFloor = this->GetFloor();

					MSStoryMaterial* storyMat = this->GetBuilding()->GetStoryMaterialByFloorNum(pUpperFloor->GetFloorNum());
					pMat = this->GetBuilding()->GetMaterial(storyMat->GetRcHorizontalMaterialID());
				}
				else
					pMat = this->GetMaterial();
			}

			userMaterial.push_back(pMat->GetName());
		}
		else
		{
			userMaterial.push_back(ma_VolumeInfo[i].materialName);
		}
		CString note;
		if (factor != 1)
			note.Format(_T("[%s : %f] * %s"), cZone->GetName(), factor, ma_VolumeInfo[i].note);
		else
			note = ma_VolumeInfo[i].note;
		userNote.push_back(note);
		userValue.push_back(ma_VolumeInfo[i].value * factor);

	}
	return true;
}

bool MSCompoG::GetBurimVolumeByCZone(int czone_id, double& value, CString& note)
{
	value = 0.0;
	note = "";

	VolumeAreaInfoEMS::FormResult form_result;
	m_VolumeAreaEMS.GetResultByCZoneBurim(czone_id, value, note, form_result);

	//버림에도 이 옵션을 적용시킬 것인가?  - UserInput
	/*if (m_IsQuantityReplaced)
	{
		value = 0.0;
		note = _T("");
	}*/
	
	return true;
}

bool MSCompoG::GetBurimFormWorkAreaByCZone(int czone_id, vector<CString>& arFWAreaDir_burim, vector<CString>& arFWAreaType_burim, vector<double>& arFWArea_burim, vector<CString>& arFormWorkNote_burim)
{
	m_VolumeAreaEMS.GetBurimFormWorkAreaByCZone(czone_id, arFWAreaDir_burim, arFWAreaType_burim, arFWArea_burim, arFormWorkNote_burim);
	return true;
}


bool MSCompoG::GetHaunchVolumeByCZone(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList)
{
	m_VolumeAreaEMS.GetResultByCZoneSubHaunch(czone_id, matl_id_list, volume_list, noteList);
	return true;
}

bool MSCompoG::GetHaunchQuantityByCZone(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList,
	vector<CString>& haunch_arFWAreaType, vector<double>& haunch_arFWArea, vector<CString>& haunch_arFormWorkNote, vector<int>& haunchID, vector<int>& fwHaunchID, vector<double>& haunch_arFWOriginArea, vector<int>& haunch_matbool)
{
	haunch_arFWAreaType.clear();
	haunch_arFWArea.clear();
	haunch_arFormWorkNote.clear();
	haunchID.clear();
	haunch_arFWOriginArea.clear();
	haunch_matbool.clear();


	m_VolumeAreaEMS.GetResultByCZoneSubHaunchAll(czone_id, matl_id_list, volume_list, noteList, haunch_arFWAreaType, haunch_arFWArea, haunch_arFormWorkNote, haunchID, fwHaunchID, haunch_arFWOriginArea, haunch_matbool);

	//haunch_arFormWorkNote = m_VolumeAreaEMS.ma_HaunchFormNote;
	//haunch_arFWArea = m_VolumeAreaEMS.ma_HaunchFormArea;
	return true;
}

bool MSCompoG::GetBuiltUpBeamVolumeByCzone(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList)
{
	m_VolumeAreaEMS.GetResultByCZoneBuiltUpBeam(czone_id, matl_id_list, volume_list, noteList);
	return true;
}

void MSCompoG::SetUseEMS(bool bUseEMS)
{
	_bUseEMS = bUseEMS;
}

bool MSCompoG::GetUseEMS()
{
	return _bUseEMS;
}

bool MSCompoG::GetFormWorkAreaByCZone(int czone_id, vector<CString>& arFWAreaDir, vector<CString>& arFWAreaType, vector<double>& arFWArea, vector<CString>& arFormWorkNote, vector<CString>& arFormWorkCategory,
	vector<CString>& arFWAreaDir_burim, vector<CString>& arFWAreaType_burim, vector<double>& arFWArea_burim, vector<CString>& arFormWorkNote_burim, int calcType, 
	vector<CString>& arAddedFWAreaDir, vector<CString>& arAddedFWAreaType, vector<double>& arAddedFWArea, vector<CString>& arAddedFormWorkNote, vector<long>& arFWAreaID, vector<double>& arFWOriginArea)
{
	arFWAreaDir.clear();
	arFWAreaType.clear();
	arFWArea.clear();
	arFormWorkNote.clear();
	arFWAreaID.clear();

	arFWAreaType_burim.clear();
	arFWArea_burim.clear();
	arFormWorkNote_burim.clear();

	MSBaseBuilding* pBuilding = GetBuilding();
	CString noneTypeName = pBuilding->GetFormManager()->GetNoneFormWorkTypeName();

	VolumeAreaInfoEMS::FormResult form_result;
	VolumeAreaInfoEMS::FormResult addedForm_result;
	double con_Origin_value = 0, con_value = 0, srcColInnerVolume = 0;
	CString con_note, srcColInnerNoteVolume;
	vector<double> addedConcVolumes; vector<CString> addedConcNotes; vector<long> addedMatIds;
	vector<long> subIds; vector<double> subVolumes; vector<CString> subNotes; vector<int> matid;
	m_VolumeAreaEMS.GetResultByCZone(czone_id, con_Origin_value, con_value, con_note, form_result, calcType, srcColInnerVolume, srcColInnerNoteVolume, addedConcVolumes, addedConcNotes, addedForm_result, addedMatIds, subIds, subVolumes, subNotes, matid, arFWOriginArea);
	arFWAreaDir = form_result.formDir;
	arFWAreaType = form_result.formWorkName;
	arFWArea = form_result.formArea;
	arFormWorkNote = form_result.formNote;
	arFWAreaID = form_result.formId;

	arAddedFWAreaDir = addedForm_result.formDir;
	arAddedFWAreaType = addedForm_result.formWorkName;
	arAddedFWArea = addedForm_result.formArea;
	arAddedFormWorkNote = addedForm_result.formNote;

	if (m_IsQuantityReplaced)
	{
		arFWAreaDir.clear();
		arFWAreaType.clear();
		arFWArea.clear();
		arFormWorkNote.clear();
		arFormWorkCategory.clear();
	}

	for (int index = 0; index < arFWAreaType.size(); index++)
		arFormWorkCategory.push_back(_T(""));

	AddAreaUserInputs(czone_id, noneTypeName, arFWAreaDir, arFWAreaType, arFWArea, arFormWorkNote, arFormWorkCategory, calcType);


	//버림
	GetBurimFormWorkAreaByCZone(czone_id, arFWAreaDir_burim, arFWAreaType_burim, arFWArea_burim, arFormWorkNote_burim);

	return true;
}

//czone_id == -1 무시
void MSCompoG::AddAreaUserInputs(int czone_id, CString noneTypeName, vector<CString> &arFWAreaDir, vector<CString> &arFWAreaType, vector<double> &arFWArea, vector<CString> &arFormWorkNote, vector<CString> &arFWCategory, int calcType)
{
	if (IsLowWall())
	{
		MSFloor* curFloor = (MSFloor*)mp_Owner;
		MSHandRailG* lowWall = dynamic_cast<MSHandRailG*>(this);
		if (!lowWall->GetTopQuantitySeparation())
		{
			if (NULL == curFloor->GetFloorCZone(czone_id))
				return;
		}
		else
		{
			if (calcType == 1)//top , upper
			{
				curFloor = curFloor->GetUpperFloor();
				if (NULL == curFloor)
					return;
				if (NULL == curFloor->GetFloorCZone(czone_id))
					return;
			}
			else //(calcType == 2 //under, 0 : all
			{
				if (NULL == curFloor->GetFloorCZone(czone_id))
					return;
			}
		}
	}

	auto factor = m_VolumeAreaEMS.GetFactorByCzone(czone_id);
	auto cZone = GetCZoneByID(czone_id);
	// User Input
	for (int i = 0; i < ma_AreaInfo.size(); ++i)
	{
		if (!ma_AreaInfo[i].use_user_input) continue;
		if (calcType == 0) //usercalcposition all
		{
			if (!(ma_AreaInfo[i].calcType == 0 || ma_AreaInfo[i].calcType == 1 || ma_AreaInfo[i].calcType == 2)) continue;
		}
		else
		{
			if (ma_AreaInfo[i].calcType != calcType) continue;
		}

		if (ma_AreaInfo[i].is_burim == true) continue;
		//if (czone_id != -1 && ma_AreaInfo[i].czone_id != czone_id) continue;

		CString formWorkName = ma_AreaInfo[i].formWorkName;

		if (formWorkName == noneTypeName) continue; // none

		arFWAreaDir.push_back(ma_AreaInfo[i].dir); //"user"가 와야 정상
		arFWAreaType.push_back(formWorkName);
		arFWArea.push_back(ma_AreaInfo[i].value * factor);

		if (cZone == nullptr) //Q24-146 사용자 물량넣을때 시공존의 짝수가 달라서 생기는 오류 수정 
			cZone = GetCZoneByID(ma_AreaInfo[i].czone_id);

		CString note;
		if (factor == 1 || cZone == nullptr)
			note = ma_AreaInfo[i].note;
		else
			note.Format(_T("[%s : %f] * %s"), cZone->GetName(), factor, ma_AreaInfo[i].note);
		arFormWorkNote.push_back(note);
		arFWCategory.push_back(ma_AreaInfo[i].user_Category);
	}
}

bool MSCompoG::GetFormWorkAreaEMSWithoutCZone(vector<CString>& arFWAreaDir, vector<CString>& arFWAreaType, vector<double>& arFWArea, vector<CString>& arFormWorkNote, vector<CString>& arFormWorkCategory,
	vector<CString>& arFWAreaDir_burim, vector<CString>& arFWAreaType_burim, vector<double>& arFWArea_burim, vector<CString>& arFormWorkNote_burim, int calcType,
	vector<CString>& arAddedFWAreaDir, vector<CString>& arAddedFWAreaType, vector<double>& arAddedFWArea, vector<CString>& arAddedFormWorkNote)
{
	arFWAreaDir.clear();
	arFWAreaType.clear();
	arFWArea.clear();
	arFormWorkNote.clear();

	arFWAreaType_burim.clear();
	arFWArea_burim.clear();
	arFormWorkNote_burim.clear();


	MSBaseBuilding* pBuilding = GetBuilding();
	CString noneTypeName = pBuilding->GetFormManager()->GetNoneFormWorkTypeName();

	
	VolumeAreaInfoEMS::FormResult form_result;
	VolumeAreaInfoEMS::FormResult addedForm_result;
	double con_Origin_Value = 0, con_value = 0, srcColInnerVolume = 0;
	CString con_note, srcColInnerNoteVolume;
	vector<double> addedConcVolumes; vector<CString> addedConcNotes; vector<long> addedMatIds;
	vector<long> subIds; vector<double> subVolumes; vector<CString> subNotes; vector<int> matid;
	vector<double> arFWOriginArea;

	int cz_id = -1;//사용안함
	m_VolumeAreaEMS.GetQuantityResult(con_Origin_Value, con_value, con_note, form_result, cz_id, calcType, srcColInnerVolume, srcColInnerNoteVolume, addedConcVolumes, addedConcNotes, addedForm_result, addedMatIds, subIds, subVolumes, subNotes, matid, arFWOriginArea);

	arFWAreaDir = form_result.formDir;
	arFWAreaType = form_result.formWorkName;
	arFWArea = form_result.formArea;
	arFormWorkNote = form_result.formNote;

	if (m_IsQuantityReplaced)
	{
		arFWAreaDir.clear();
		arFWAreaType.clear();
		arFWArea.clear();
		arFormWorkNote.clear();
	}

	int czone_id = -1;// czone 무시

	AddAreaUserInputs(czone_id, noneTypeName, arFWAreaDir, arFWAreaType, arFWArea, arFormWorkNote, arFormWorkCategory, calcType);

	//버림
	GetBurimFormWorkAreaByCZone(czone_id, arFWAreaDir_burim, arFWAreaType_burim, arFWArea_burim, arFormWorkNote_burim);

	return true;
	
}


/*
bool MSCompoG::GetFormWorkArea(double& value, CString& note, double& value_out, CString& note_out)
{
	value = 0.0;
	note = "";
	value_out = 0.0;
	note_out = "";

	for(int i=0 ; i<ma_AreaInfo.size() ; ++i)
	{
		value += ma_AreaInfo[i].value;
		note += ma_AreaInfo[i].note;
		if (i != ma_AreaInfo.size()-1) note += " (+) ";
	}

	return true;
}
*/


vector<MSConstructionZone*> MSCompoG::GetCZones()
{
	if(mp_Owner->GetType() != MSElement::msFloor)
		return vector<MSConstructionZone*>();
	MSFloor* pFloor = (MSFloor*)mp_Owner;

	vector<MSConstructionZone*> czone_list;
	// ma_CZoneIDFromQuantityCalc 시리얼라이즈가 일단 안되어서 GetCZoneIDs() 사용
	//pFloor->GetCZone(ma_CZoneIDFromQuantityCalc, czone_list);
	pFloor->GetCZone( GetCZoneIDs() , czone_list);

	return czone_list;
}

MSConstructionZone* MSCompoG::GetCZoneByID(int cZoneID)
{
	if (mp_Owner->GetType() != MSElement::msFloor)
		return nullptr;

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	return pFloor->GetCZone(cZoneID);
}


CTime MSCompoG::GetStartDate()
{
	if(m_IsScheduleAssigned)
		return m_startDate;

	vector<MSConstructionZone*> cZones = GetCZones();

	if(cZones.size() ==0)
		return MSConstructionZone::InitialDate();


	CTime earliestDay = cZones[0]->GetActualStartDate();
	for(int i=1; i<cZones.size(); i++ )
	{
		MSConstructionZone* zone = cZones[i];
		if(earliestDay > zone->GetActualStartDate())
			earliestDay = zone->GetActualStartDate();		
	}

	return earliestDay;
}

void MSCompoG::SetStartDate( const CTime&  startDate )
{
	m_IsScheduleAssigned = true;
	
	m_startDate = startDate;	
	if(m_finishDate < m_startDate)
		m_finishDate = m_startDate;
}

CTime MSCompoG::GetFinishDate()
{
	if(m_IsScheduleAssigned)
		return m_finishDate;

	vector<MSConstructionZone*> cZones = GetCZones();
	CTime latestDay = MSConstructionZone::InitialDate();

	for(int i=0; i<cZones.size(); i++ )
	{
		MSConstructionZone* zone = cZones[i];
		if(latestDay < zone->GetActualFinishDate())
			latestDay = zone->GetActualFinishDate();		
	}
	return latestDay;

}

bool MSCompoG::SetFinishDate(const CTime& finishDate )
{
	if(m_finishDate < m_startDate)
		return false;

	m_finishDate = finishDate;
	return true;
}

void MSCompoG::SetCZoneIDToUserQuantity(int calcType)
{
	if (!IsLowWall())
		SetCZoneIDToUserQuantity(false, 0);
	else
	{
		MSHandRailG* lowWallG = dynamic_cast<MSHandRailG*>(this);
		if (lowWallG->GetTopQuantitySeparation())
		{
			if (calcType == (int)MSHandRailG::LowWallCalcType::Top)
			{
				SetCZoneIDToUserQuantity(true, (int)MSHandRailG::LowWallCalcType::Top);
			}
			else if (calcType == (int)MSHandRailG::LowWallCalcType::Bottom)
			{
				SetCZoneIDToUserQuantity(false, (int)MSHandRailG::LowWallCalcType::Bottom);
			}
			else
			{
				SetCZoneIDToUserQuantity(true, (int)MSHandRailG::LowWallCalcType::Top);
				SetCZoneIDToUserQuantity(false, (int)MSHandRailG::LowWallCalcType::Bottom);
			}
		}
		else
		{
			SetCZoneIDToUserQuantity(false, 0);
		}
	}
}

void MSCompoG::SetCZoneIDToUserQuantity(bool isUpperCzoneNeeded = false, int calcType = 0) //기본적으로 부재가 세팅된 czone의 가장 빠른 id를 기준으로 한다. 단 난간벽의 경우 옵션에 따라서 상층부 czone이 필요함.
{
	MSFloor* floor = nullptr;
	if (isUpperCzoneNeeded)
	{
		floor = GetFloor()->GetUpperFloor();
	}
	else
		floor = GetFloor();
	if (floor == nullptr) return;


	// 가장 빠른 ID 를 셋팅해준다.
	for (int ii = 0; ii < 2; ++ii)
	{
		int czoneID = GetFirstCzoneIDInCompoGOnFloor(floor);
		vector<VolumeAreaInfo>& target = ii == 0 ? ma_VolumeInfo : ma_AreaInfo;
		if (calcType == 0) //난간벽 이외의 부재 + 난간벽이 한덩어리로 출력될 경우
		{
			for (int i = 0; i < target.size(); ++i)
			{
				if (!target[i].use_user_input) continue;
				if (calcType == 0) //usercalcposition all
				{
					if (!(target[i].calcType == 0 || target[i].calcType == 1 || target[i].calcType == 2)) continue;
					target[i].czone_id = czoneID;
				}
			}
		}
		else
		{
			for (int i = 0; i < target.size(); ++i)
			{
				if (!target[i].use_user_input) continue;
				if (target[i].calcType != calcType) continue;
				target[i].czone_id = czoneID;
			}
		}
	}
}

int MSCompoG::GetFirstCzoneIDInCompoGOnFloor(MSFloor* floor)
{
	//해당층에 속한 Czone 과 compoG 가 가지고 있는 교집합 ID 중에서 가장 빠른 것을 찾아야한다. 없으면 Default 
	vector<int> czoneIDs = GetCZoneIDs();
	MSConstructionZone* pDefCZone = floor->GetCZoneDefault(IsVerticalMember());
	if (czoneIDs.size() == 0)
	{
		if (pDefCZone != nullptr)
			return pDefCZone->m_ID;
		return 0;
	}

	int minNum = 10000;
	int minID = 0;
	
	for (int i = 0; i < czoneIDs.size(); ++i)
	{
		MSConstructionZone* czone = floor->GetCZone(czoneIDs[i]);
		if (czone == NULL) continue;
		if (czone->GetNumber() < minNum)
		{
			minNum = czone->GetNumber();
			minID = czone->m_ID;
		}
	}

	if(minID == 0 && pDefCZone != nullptr)
		minID = pDefCZone->m_ID;
	
	return minID;
}

vector<int> MSCompoG::GetCZoneIDs()
{
	if (GetBuilding()->GetQuantityOption()->m_nByOptionOrManual == 0 || GetBuilding()->GetQuantityOption()->m_nByOptionOrManual == 1)
		return m_VolumeAreaEMS.ma_CZoneID;
	else if (GetBuilding()->GetQuantityOption()->m_nByOptionOrManual == 2)
		return m_VolumeAreaEMS_NewEMS.GetCZoneIDs();
	return vector<int>();
}

bool MSCompoG::Overlap(MSCompoG* pUrCompoG, double dTol , bool bIncludeOnLine)
{
	if(!IsLevelOverlap(pUrCompoG))
		return false;

	GM2DPolyline MyBoundary(TRUE);
	GetBoundary(MyBoundary);

	GM2DPolyline UrBoundary(TRUE);
	pUrCompoG->GetBoundary(UrBoundary);
	bool IsOverlap = false;
	if(MyBoundary.OverlapEx(UrBoundary, dTol, bIncludeOnLine)==TRUE)
		IsOverlap = true;
	return IsOverlap;
}
/*
OverLap(Contain 정책)

1. 입력 되는 부재가 기존 부재에 포함되면 입력 안함
2. 입력 되는 부재가 기존 부재를 포함 하면 입력 안함
3. 입력 되는 부재가 조인트에 포함되면 입력 안함
4. 입력 되는 부재가 조인트를 포함하면 입력함
5. 입력 되는 부재가 기존 부재에 3점 포함되면 입력 안함(반대의 경우도 입력 안함)
6. 입력 되는 부재가 기존 부재보다 길경우(시작점, 끝점 모두)
6-1. 입력 되는 부재가 기존 부재에 수선의 발을 내려 LLine(RLine)의 시작점 끝점이 기존 부재에 포함될 경우 입력안함
6-2. 6-1의 경우 입력되는 부재의 LLine과 RLine이 기존 부재의 LLine RLine과 일치하지 않는 선이 1개 나오면 처리 하지 않는다.
*/


bool MSCompoG::Contains(MSCompoG* pUrCompoG, double dTol /*= DTOL_GM*/, bool bIncludeOnLine /*= TRUE */, bool isCheckZLevel)
{
	if(isCheckZLevel && !IsLevelOverlap(pUrCompoG))
		return false;

	GM2DPolyline MyBoundary(TRUE);
	GetBoundary(MyBoundary);
	
	GM2DPolyline UrBoundary(TRUE);
	pUrCompoG->GetBoundary(UrBoundary);

	GM2DBound MyBound;
	GM2DBound UrBound;
	
	MyBoundary.Get2DBound(MyBound);
	UrBoundary.Get2DBound(UrBound);

	bool bRavl = MyBound.OverLapTo(&UrBound, dTol);
	if(!bRavl)
		return FALSE;

	double dLineHitDistLoc = 0;

	long nConVal = 1;
	if(bIncludeOnLine)
		nConVal = 1;
	else
		nConVal = 0;

	// 같은 타입의 부재 체크
	long NumLinkMember = 0;
	if(IsLinkMember())
		NumLinkMember++;
	if(pUrCompoG->IsLinkMember())
		NumLinkMember++;

	bool bMyUrSameLinkCurveType = false;
	if(NumLinkMember == 2)
	{
		MSLinkMemberG* pUrLinkG = (MSLinkMemberG*) pUrCompoG;
		MSLinkMemberG* pMyLinkG = (MSLinkMemberG*)this;
		if(pMyLinkG->isArc() == pUrLinkG->isArc())
			bMyUrSameLinkCurveType = true;
	}

	// 벽체 오프닝은 포인트 부재와 같은 로직으로 처리한다.
	bool hasPointMember = false;
	if((IsPointMember() || IsWallOpeningG()) || (pUrCompoG->IsPointMember() || pUrCompoG->IsWallOpeningG()))
		hasPointMember = true;
	
	long NumPointMember = 0;
	if(IsPointMember() || IsWallOpeningG())
		NumPointMember++;
	if(pUrCompoG->IsPointMember() || pUrCompoG->IsWallOpeningG())
		NumPointMember++;

	long nColinearLineCnt = 0;

	if(pUrCompoG->IsLinkMember())
	{
		MSLinkMemberG* pUrLinkG = (MSLinkMemberG*) pUrCompoG;
		GM2DLineVector UrCenLine = pUrLinkG->GetCenLVec();

		if(bMyUrSameLinkCurveType)
		{
			MSLinkMemberG* pMyLinkG = (MSLinkMemberG*)this;
			if(pUrLinkG->mp_LLine->IsColinear4DTol(pMyLinkG->mp_LLine) != 0)
				nColinearLineCnt++;
			if(pUrLinkG->mp_LLine->IsColinear4DTol(pMyLinkG->mp_RLine) != 0)
				nColinearLineCnt++;
			if(pUrLinkG->mp_RLine->IsColinear4DTol(pMyLinkG->mp_LLine) != 0)
				nColinearLineCnt++;
			if(pUrLinkG->mp_RLine->IsColinear4DTol(pMyLinkG->mp_RLine) != 0)
				nColinearLineCnt++;
		}

		if(nColinearLineCnt != 1)
		{
			if(bMyUrSameLinkCurveType == true ||  NumPointMember >= 1)
			{
				long nCRval0 = MyBoundary.Contains(pUrLinkG->GetSVec());
				long nCRval1 = MyBoundary.Contains(pUrLinkG->GetEVec());

				if(nCRval0 < nConVal && nCRval1 < nConVal)
					return true;

				long nLRval0 = MyBoundary.Contains(&pUrLinkG->mp_LLine->m_P0);
				long nLRval1 = MyBoundary.Contains(&pUrLinkG->mp_LLine->m_P1);

				if(nLRval0 < nConVal && nLRval1 < nConVal)
					return true;

				long nRRval0 = MyBoundary.Contains(&pUrLinkG->mp_RLine->m_P0);
				long nRRval1 = MyBoundary.Contains(&pUrLinkG->mp_RLine->m_P1);

				if(nRRval0 < nConVal && nRRval1 < nConVal)
					return true;
			}

			if(bMyUrSameLinkCurveType)
			{
				MSLinkMemberG* pMyLinkG = (MSLinkMemberG*)this;

				GM2DLineVector MyCenLine = pMyLinkG->GetCenLVec();
				double dSLoc = MyCenLine.OrthoBaseLocValue(UrCenLine.m_P0);
				double dELoc = MyCenLine.OrthoBaseLocValue(UrCenLine.m_P1);
				if(dSLoc > dELoc)
				{
					double dTmp = dSLoc;
					dSLoc = dELoc;
					dELoc = dTmp;
				}

				long nLen = MyCenLine.Length();

				if(nLen > MSFloor::ms_DefLineHitDistTol)
					dLineHitDistLoc = MSFloor::ms_DefLineHitDistTol / nLen;
				else
					dLineHitDistLoc = 0;

				if(dSLoc <= 0 + dLineHitDistLoc && dELoc >= 1 - dLineHitDistLoc)
				{
					long nLRval0 = MyBoundary.Contains(&pUrLinkG->mp_LLine->OrthoBaseVector(MyCenLine.m_P0));
					long nLRval1 = MyBoundary.Contains(&pUrLinkG->mp_LLine->OrthoBaseVector(MyCenLine.m_P1));

					if(nLRval0 < nConVal && nLRval1 < nConVal)
						return true;

					long nRRval0 = MyBoundary.Contains(&pUrLinkG->mp_RLine->OrthoBaseVector(MyCenLine.m_P0));
					long nRRval1 = MyBoundary.Contains(&pUrLinkG->mp_RLine->OrthoBaseVector(MyCenLine.m_P1));

					if(nRRval0 < nConVal && nRRval1 < nConVal)
						return true;
				}
			}
		}
	}

	// 포인트 부재끼리 체크
	if(NumPointMember == 2)
	{
		bool bRavl = false;
		if(MyBoundary.OverlapEx(UrBoundary, DTOL_GM, FALSE))
			bRavl = true;
		return bRavl;
	}


	long nContainCnt = 0;
	bool isUrAllContain = true;
	long NumUrVec = UrBoundary.GetSize() -1;
	for(long iVec = 0 ; iVec < NumUrVec ; iVec++)
	{
		long nRval = MyBoundary.Contains(UrBoundary.ma_Point[iVec], DTOL_GM);
		if(nRval < nConVal)
			nContainCnt++;
		else
			isUrAllContain = false;

	}
	//if(NumUrVec > 1 && isUrAllContain == true)
	if(NumLinkMember == 2)
	{
		if(NumUrVec > 1 && (nContainCnt == NumUrVec -1 || isUrAllContain == true) )
			return true;
	}
	else if(NumPointMember >= 0)
	{
		if(NumUrVec > 1 && nContainCnt > 0)
			return true;
	}

	/*bool isMyAllContain = true;
	long NumMyVec = MyBoundary.GetSize() - 1;
	for(long iVec = 0 ; iVec < NumMyVec ; iVec++)
	{
		long nRval = UrBoundary.Contains(MyBoundary.ma_Point[iVec], dTol);
		if(nRval >= nConVal)
			isMyAllContain = false;
	}
	if(NumMyVec > 1 && isMyAllContain == true)
		return true;*/
	
	return false;
}

GM2DVector* MSCompoG::GetSVec()
{
	if(ma_Vec.size() == 0)
		return NULL;
	return ma_Vec.front();
}

GM2DVector* MSCompoG::GetEVec()
{
	if(ma_Vec.size() == 0)
		return NULL;
	return ma_Vec.back();
}

GM2DVector* MSCompoG::GetSVecInput()
{
	if(IsInvalidVecInput())
		UpdateVecInput();
	return ma_Vec_Input.front();
}

GM2DVector* MSCompoG::GetEVecInput()
{
	if(IsInvalidVecInput())
		UpdateVecInput();
	return ma_Vec_Input.back();
}

void MSCompoG::Translate( GM2DVector &pDelta )
{
	MSElement::Translate(pDelta);
	long nVec = ma_Vec.size();
	for(long iVec=0; iVec<nVec; iVec++)
		ma_Vec[iVec]->Translate(pDelta);

	nVec = ma_Vec_Input.size();
	for (long iVec = 0; iVec < nVec; iVec++)
		ma_Vec_Input[iVec]->Translate(pDelta);

	TranslateAddRebars(pDelta);
}

void MSCompoG::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSElement::MirrorByAxis(pAxis);
	long nVec = ma_Vec.size();
	for(long iVec=0; iVec<nVec; iVec++)
		ma_Vec[iVec]->MirrorByAxis(pAxis);

	nVec = ma_Vec_Input.size();
	for (long iVec = 0; iVec < nVec; iVec++)
		ma_Vec_Input[iVec]->MirrorByAxis(pAxis);
	MirrorByAxisAddRebars(pAxis);
}

void MSCompoG::RotateZ( double theta, GM2DVector *pCenter )
{
	MSElement::RotateZ(theta, pCenter);
	long nVec = ma_Vec.size();
	for(long iVec=0; iVec<nVec; iVec++)
		ma_Vec[iVec]->RotateZ(theta, pCenter);

	nVec = ma_Vec_Input.size();
	for (long iVec = 0; iVec < nVec; iVec++)
		ma_Vec_Input[iVec]->RotateZ(theta, pCenter);

	RotateZAddRebars(theta, pCenter);
}

void MSCompoG::TranslateAddRebars(GM2DVector &pDelta)
{
	for (auto it = mm_AddRebar.begin(); it != mm_AddRebar.end(); it++)
		it->second->Translate(pDelta);
}
void MSCompoG::MirrorByAxisAddRebars(GM2DLineVector& pAxis)
{
	for (auto it = mm_AddRebar.begin(); it != mm_AddRebar.end(); it++)
		it->second->MirrorByAxis(pAxis);
}
void MSCompoG::RotateZAddRebars(double theta, GM2DVector *pCenter)
{
	for (auto it = mm_AddRebar.begin(); it != mm_AddRebar.end(); it++)
		it->second->RotateZ(theta, pCenter);
}

bool MSCompoG::IsContainSlabinLowFloor()
{
	return false;
}



bool MSCompoG::CheckCompoM()
{
	MSCompoM* pCompoM = GetCompoM();
	if(pCompoM == NULL)
		return false;

	MSCompoM::Type_MSCompoM eMType = pCompoM->GetType();
	Type_MSCompoG eGType = GetType();

// 	enum Type_MSCompoG {msNULL=0, msBuilding=1, msFloor, msBeamG, msBraceG, msWallG, msColumnG, msIsoFootG, msRampG,  msSlabG, msStairG, msPitG, 
// 		msWallFootG, msProject, msSector, msBeamLine, msSlabLineG, msLineLevelDifG, msPointLevelDifG, msHandRailG, msWallJointG, msLevelZone, msOpening, 
// 		msWindowG, msDoorG, /*msWallOpeningG,*/ msStairPart =26, msSlabRebar, msWell, msTrench};
	
//	enum Type_MSCompoM {msBeamM=100, msBraceM, msBWallM, msColumnM, msIsoFootM, msRampM, msRWallM, msSlabM=107, /*msStairM*/ 
//		msWallFootM = 109, msPitM, msLevelDifM, msHandRailM, msWindowM, msDoorM, msWallOpeningM, 
//		msStairLandingM, msStairStepM, msButtressM, msPcBeamM};

	if (eGType == MSElement::msBeamG && (eMType == MSCompoM::msBeamM || eMType == MSCompoM::msPcBeamM || eMType == MSCompoM::msCustomBeamM))
		return true;
	else if (eGType == MSElement::msBraceG && eMType == MSCompoM::msBraceM)
		return true;
	else if (eGType == MSElement::msColumnG && (eMType == MSCompoM::msColumnM || eMType == MSCompoM::msButtressM || eMType == MSCompoM::msPcColumnM || eMType == MSCompoM::msCustomColumnM))
		return true;
	else if (eGType == MSElement::msWallG && (eMType == MSCompoM::msBWallM || eMType == MSCompoM::msRWallM))
		return true;
	else if (eGType == MSElement::msWallColumnG && eMType == MSCompoM::msCustomWallColumnM)
		return true;
	else if(eGType == MSElement::msIsoFootG && eMType == MSCompoM::msIsoFootM)
		return true;
	else if(eGType == MSElement::msWallFootG && eMType == MSCompoM::msWallFootM)
		return true;
	else if(eGType == MSElement::msRampG && eMType == MSCompoM::msRampM)
		return true;
	else if(eGType == MSElement::msSlabG && eMType == MSCompoM::msSlabM)
		return true;
	else if(eGType == MSElement::msPitG && eMType == MSCompoM::msPitM)
		return true;
	else if(eGType == MSElement::msStairG && (eMType == MSCompoM::msStairLandingM || eMType == MSCompoM::msStairStepM) )
		return true;
	else if(eGType == MSElement::msHandRailG && eMType == MSCompoM::msHandRailM)
		return true;
	
	

	return false;
}

void MSCompoG::AddConnectionWithMember_NotExistOnly(MSCompoG* pMember)
{
	// 폴리맴버는 Connection 생성 안함
	//if(IsPolyMember() || pMember->IsPolyMember())
		//return;

	for (int i=0 ; i<ma_Connections.size() ; ++i)
	{
		MSConnection* pConnection = ma_Connections[i];
		if(pConnection->GetFirstCompoG() == pMember || pConnection->GetSecondCompoG() == pMember)
			return;
	}

	CheckModelOpt CheckFlag;
	if (this->IsLevelConnectedOverLap(pMember, CheckFlag, true) == false)
		return;

	MSConnection* pConnection = NULL;
	if(pConnection == NULL)
	{
		pConnection = MSConnection::GetConnection(pMember, this);
		
		pConnection->GetFirstCompoG()->AddConnection(pConnection);
		pConnection->GetSecondCompoG()->AddConnection(pConnection);
		pConnection->CalculateContainedAngle();
		bool isFirstFloorMember = false;
		if(pConnection->GetFirstCompoG()->IsFloorMember())	isFirstFloorMember = true;
		bool isSecondFloorMember = false;
		if(pConnection->GetSecondCompoG()->IsFloorMember())	isSecondFloorMember = true;
		
		if (pConnection->m_SetByUser == false)
		{
			if (isFirstFloorMember == isSecondFloorMember)
			{
				pConnection->SetFirstTrimOption(MSConnection::TO_DEFAULT);
				pConnection->SetSecondTrimOption(MSConnection::TO_DEFAULT);
			}
			else
			{
				if (isFirstFloorMember == true || isSecondFloorMember == true)
				{
					if (pConnection->GetFirstCompoG()->IsLinkMember() && pConnection->GetSecondCompoG()->IsLinkMember())
					{
						pConnection->SetFirstTrimOption(MSConnection::TO_IGNORE);
						pConnection->SetSecondTrimOption(MSConnection::TO_IGNORE);
					}
					else
					{
						pConnection->SetFirstTrimOption(MSConnection::TO_DEFAULT);
						pConnection->SetSecondTrimOption(MSConnection::TO_DEFAULT);
					}
				}
			}
		}
	}
}

void MSCompoG::AddConnectionWithMember(MSCompoG* pPartnerMember )
{
	// 폴리맴버는 Connection 생성 안함
// 	if (pPartnerMember->IsPolyMember() && IsPolyMember())
// 		return;

	if (AppGlobalOptionWrapper::Instance()->GetTrim3DMode() == 1)
	{
		// 경사 폴리 맴버는 Connection 생성 안함
		if (pPartnerMember->IsPolyMember() && IsPolyMember() &&
			(IsSlopeMember() || pPartnerMember->IsSlopeMember()))
			return;
	}

	if ((pPartnerMember->IsPolyMember() && IsPolyMember() == false) || 
		pPartnerMember->IsPolyMember() == false && IsPolyMember())
	{
		MSCompoG* pPolyMemberG = this;
		MSCompoG* pNotPolyMemberG = pPartnerMember;

		if (IsPolyMember() == false)
			std::swap(pPolyMemberG, pNotPolyMemberG);

		// 폴리부재와 다른부재가 터치 되는 경우는 MConnection을 생성하지 않는다
		if (pPolyMemberG->IsLevelTouch(pNotPolyMemberG))
			return;

		if (pNotPolyMemberG->GetContainingElementId() != pPolyMemberG->m_ID)
		{
			if(find(pPolyMemberG->ma_ContainedMemberID.begin(), pPolyMemberG->ma_ContainedMemberID.end(), pNotPolyMemberG->m_ID) == pPolyMemberG->ma_ContainedMemberID.end())
				pPolyMemberG->ma_ContainedMemberID.push_back(pNotPolyMemberG->m_ID);

			pNotPolyMemberG->SetContainingElementId(pPolyMemberG->m_ID);
		}

		//if(AppGlobalOptionWrapper::Instance()->GetTrim3DMode() == 1)
			//return;
	}

	MSConnection* pConnection = NULL;
	long NumOverMem = ma_Connections.size();
	for(long iOverMem = 0 ; iOverMem < NumOverMem ; iOverMem++)
	{
		MSConnection* pConnection_Cur = ma_Connections[iOverMem];
		if(pConnection_Cur->GetFirstCompoG() == pPartnerMember || pConnection_Cur->GetSecondCompoG() == pPartnerMember)
		{
			pPartnerMember->AddConnection(pConnection_Cur);
			pConnection_Cur->CalculateContainedAngle();
			pConnection_Cur->CalculateLinkMemberTrimOption();
			return;
		}
	}

	if(pConnection == NULL)
	{	
		pConnection = MSConnection::GetConnection(pPartnerMember, this);		
		pConnection->GetFirstCompoG()->AddConnection(pConnection);
		pConnection->GetSecondCompoG()->AddConnection(pConnection);
		pConnection->CalculateContainedAngle();
		pConnection->CalculateLinkMemberTrimOption();
	}
}

void MSCompoG::ClearContainMemberID()
{
	if (IsSubMember())
	{
		ma_ContainedMemberID.clear();
		return;
	}

	vector<MSFloor*> FloorList = GetOverlapFloor();
	for (int i = 0; i < ma_ContainedMemberID.size(); i++)
	{
		long nContainID = ma_ContainedMemberID[i];
		for (int iFloor = 0; iFloor < FloorList.size(); iFloor++)
		{
			MSFloor* pFloor = FloorList[iFloor];
			MSCompoG* pFindCompoG = pFloor->GetCompoG(nContainID);
			if (pFindCompoG != NULL && pFindCompoG->GetContainingElementId() == m_ID)
			{
				pFindCompoG->RemoveOpeningInfo(m_ID);
				pFindCompoG->SetContainingElementId(0);
				break;
			}
		}
	}
	ma_ContainedMemberID.clear();
}

void MSCompoG::SortContainMemberID()
{
	sort(ma_ContainedMemberID.begin(), ma_ContainedMemberID.end(), 
		[](int a, int b)->bool {return a < b; }
	);
	
	vector<int>::iterator new_end;
	new_end = unique(ma_ContainedMemberID.begin(), ma_ContainedMemberID.end());
	ma_ContainedMemberID.erase(new_end, ma_ContainedMemberID.end());
}

void MSCompoG::RemoveConnections(bool bNotRemoveSetByUser)
{
	ClearContainMemberID();
	
	long NumConnection = ma_Connections.size();
	for (long iConn = NumConnection -1 ; iConn >= 0; iConn--)
	{
		MSConnection* pConnection = ma_Connections[iConn];
		if (pConnection->IsDeleted() == false)
		{
			if (pConnection->GetFirstCompoG() == pConnection->GetSecondCompoG())
			{
				pConnection->SetDeleted(true);
				ma_Connections.erase(ma_Connections.begin() + iConn);
				continue;
			}
			MSCompoG* pPartnerG = pConnection->GetPartner(this);
			if (bNotRemoveSetByUser == true && pConnection->m_SetByUser == true)
				continue;

			if (pPartnerG != nullptr)
				pPartnerG->RemoveConnection(pConnection);

			pConnection->SetDeleted(true);
		}
		ma_Connections.erase(ma_Connections.begin() + iConn);
	}
	//ma_Connections.clear();
	//if (IsSubMember() == false)
		InitTrimmedBoundary();
}

void MSCompoG::RemoveConnection(MSConnection* pConnection)
{
	long NumOverMem = ma_Connections.size();
	for(long iOverMem = 0 ; iOverMem < NumOverMem ; iOverMem++)
	{
		MSConnection* pCur = ma_Connections[iOverMem];
		if(pCur == pConnection)
		{
			ma_Connections.erase(ma_Connections.begin()+iOverMem);
			break;
		}
	}
	m_DoUpdateTrimmedProfile = true;
}


void MSCompoG::AddConnection(MSConnection* pConnection)
{
	//우선 순위 
	// 포인트 부재가 가장 마지막
	// 포인트 부재끼리 우선순위는 교차영역이 가장 작은게 높다
	// 나머지는 입력한 순서대로
	MSCompoG* pParther = pConnection->GetPartner(this);
	int nInsertIdx = 0;

	long NumConn = ma_Connections.size();
	for (long iConn = 0; iConn < NumConn; iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		if (pConn == pConnection)
			return;

		if (pParther != nullptr && pParther->IsPointMember())
		{
			MSCompoG* pCurPartner = pConn->GetPartner(this);
			if (pCurPartner != nullptr && pCurPartner->IsPointMember())
			{
				if (pConn->GetPointMemberArea() < pConnection->GetPointMemberArea())
					nInsertIdx = iConn + 1;
				else
					nInsertIdx = iConn;
			}
			else
				nInsertIdx = iConn + 1;
		}
		else
		{
			MSCompoG* pCurPartner = pConn->GetPartner(this);
			if (pCurPartner != nullptr && pCurPartner->IsPointMember() == false)
				nInsertIdx = iConn + 1;
			else
				nInsertIdx = iConn;
		}
	}
	ma_Connections.insert(ma_Connections.begin() + nInsertIdx, pConnection);
}

CString MSCompoG::GetKeybyIDtoFloorNum(int ID, int FloorNum)
{
	CString strID;
	strID.Format(_T("%d_%d"), ID, FloorNum);
	return strID;
}

void MSCompoG::CopyConnection(MSCompoG* pSourcepG, MSCompoG* pTargetG, map<CString, MSCompoG*>& NewCompoGMap)
{
	pTargetG->GetID();

	for (int i = 0; i < pSourcepG->ma_Connections.size(); i++)
	{
		MSConnection* pCon = pSourcepG->ma_Connections[i];
		MSConnection* pNewCon = MSConnection::GetEmptyConnection();
		pNewCon->CopyFromMe(pCon);
		pTargetG->ma_Connections.push_back(pNewCon);
	}

	NewCompoGMap.insert(make_pair(GetKeybyIDtoFloorNum(pSourcepG->m_ID, pTargetG->GetFloorNum()), pTargetG));
}

void MSCompoG::RecoveryCopyConnectionCompoGID(MSCompoG* pCompoG, map<CString, MSCompoG*>& NewCompoGMap, map<CString, MSConnection*>& RecoveryConnectionMap)
{
	if (NewCompoGMap.size() == 0)
		return;

	int nFloorNum = pCompoG->GetFloorNum();

	bool bOld = CmdProcessHelper::GetHistoryManager()->IsInUndoRedo();
	CmdProcessHelper::GetHistoryManager()->SetIsInUndoRedo(true);

	for (int i = pCompoG->ma_Connections.size() - 1; i >= 0; i--)
	{
		MSConnection* pCon = pCompoG->ma_Connections[i];

		if (NewCompoGMap.find(GetKeybyIDtoFloorNum(pCon->GetFirstCompoGID(), nFloorNum)) != NewCompoGMap.end() &&
			NewCompoGMap.find(GetKeybyIDtoFloorNum(pCon->GetSecondCompoGID(), nFloorNum)) != NewCompoGMap.end())
		{
			pCon->SetFirstCompoG(NewCompoGMap[GetKeybyIDtoFloorNum(pCon->GetFirstCompoGID(), nFloorNum)]);
			pCon->SetSecondCompoG(NewCompoGMap[GetKeybyIDtoFloorNum(pCon->GetSecondCompoGID(), nFloorNum)]);
			pCon->SetHistoryID();

			CString HistoryID = pCon->GetHistoryID();

			MSConnection* pConnection = dynamic_cast<MSConnection*>(CmdProcessHelper::GetHistoryManager()->GetHistoryObject(HistoryID));
			if (pConnection == nullptr)
				CmdProcessHelper::GetHistoryManager()->AddHistoryObject(pCon);
			else
				continue;
			
			MSCompoG* pPartner = pCon->GetPartner(pCompoG);
			if (pPartner == nullptr) continue;
			for (int iCon = pPartner->ma_Connections.size() - 1; iCon >= 0; iCon--)
			{
				MSConnection* pPartnerCon = pPartner->ma_Connections[iCon];
				if (pCon == pPartnerCon)
					continue;
				CString FirstCompoGID = GetKeybyIDtoFloorNum(pPartnerCon->GetFirstCompoGID(), nFloorNum);
				CString SecondCompoGID = GetKeybyIDtoFloorNum(pPartnerCon->GetSecondCompoGID(), nFloorNum);
				if (NewCompoGMap.find(FirstCompoGID) != NewCompoGMap.end() &&
					NewCompoGMap.find(SecondCompoGID) != NewCompoGMap.end())
				{
					long NewFirstCompoGID = NewCompoGMap[FirstCompoGID]->m_ID;
					long NewSecoundCompoGID = NewCompoGMap[SecondCompoGID]->m_ID;
					if ((pCon->GetFirstCompoGID() == NewFirstCompoGID && pCon->GetSecondCompoGID() == NewSecoundCompoGID) ||
					(pCon->GetSecondCompoGID() == NewFirstCompoGID && pCon->GetFirstCompoGID() == NewSecoundCompoGID) )
					{
						delete pPartnerCon;
						pPartner->ma_Connections[iCon] = pCon;
						break;
					}
				}
			}
			RecoveryConnectionMap.insert(make_pair(HistoryID, pCon));
		}
		else
		{
			if (RecoveryConnectionMap.find(pCon->GetHistoryID()) == RecoveryConnectionMap.end())
			{
				pCompoG->ma_Connections.erase(pCompoG->ma_Connections.begin() + i);
				delete pCon;
			}
		}
	}
	
	CmdProcessHelper::GetHistoryManager()->SetIsInUndoRedo(bOld);
	pCompoG->CheckValidConnections();
}

void MSCompoG::DeleteConnections()
{
	long NumOverMem = ma_Connections.size();
	for(long iOverMem = 0 ; iOverMem < NumOverMem ; iOverMem++)
	{
		MSConnection* pConnection = ma_Connections[iOverMem];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if(pPartnerG == NULL)
			continue;
		
		pPartnerG->RemoveConnection(pConnection);
		pConnection->SetDeleted(true);
	}
	ma_Connections.clear();
}

void MSCompoG::CheckZLevelConnection()
{
	long NumOverMem = ma_Connections.size();
	for(long iOverMem = NumOverMem-1 ; iOverMem >= 0 ; iOverMem--)
	{
		MSConnection* pConnection = ma_Connections[iOverMem];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if(pPartnerG == NULL)
			continue;
		if(IsLevelOverlap(pPartnerG) == false)
		{
			pPartnerG->RemoveConnection(pConnection);
			pConnection->SetDeleted(true);
			ma_Connections.erase(ma_Connections.begin() + iOverMem);
		}
	}
}

vector<MSCompoG*> MSCompoG::GetPartnerCompoGs(long GetIdx)
{
	vector<MSCompoG*> PartnerCompoGArr;
	long NumOverMem = ma_Connections.size();
	for(long iOverMem = GetIdx ; iOverMem < NumOverMem ; iOverMem++)
	{
		MSConnection* pConnection = ma_Connections[iOverMem];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if(pPartnerG == NULL)
			continue;
		PartnerCompoGArr.push_back(pPartnerG);
	}
	
	//정렬 필요 EndCut 옵션이 있는 CompoG는 가장 마지막 순서에 와야한다.
	sort(PartnerCompoGArr.begin(), PartnerCompoGArr.end(), 
		[](MSCompoG* a, MSCompoG* b)->bool {return a->GetTrimOrder() < b->GetTrimOrder() ;}
	);

	return PartnerCompoGArr;
}

int MSCompoG::GetTrimOrder()
{
	int nOrder = 0;
	bool bHasCut = HasTrimOption(MSConnection::TO_ENDCUT);
	if(bHasCut)
		nOrder = 10;

	return nOrder;
}

void MSCompoG::InitTrimmedBoundaryVisibility()
{
	if(mp_TrimmedBoundary == NULL)
	{
		ma_TrimmedBoundaryVisible.clear();
		return;
	}

	ma_TrimmedBoundaryVisible.clear();
	for(long i = 0 ; i < mp_TrimmedBoundary->GetSize() -1 ; i++)
		ma_TrimmedBoundaryVisible.push_back(true);
}

int MSCompoG::GetTrimmedBoundaryVisibility(int idx)
{
	if(ma_TrimmedBoundaryVisible.size() <= idx)
	{
		ASSERT(0);
		InitTrimmedBoundaryVisibility();

		if (ma_TrimmedBoundaryVisible.size() <= idx)
			return -1;
	}
	return ma_TrimmedBoundaryVisible[idx];
}

void MSCompoG::SetTrimmedBoundaryVisibility(int idx, int bVal)
{
	int NumVisble = ma_TrimmedBoundaryVisible.size();
	if(NumVisble < idx)
	{
		ASSERT(0);
		InitTrimmedBoundaryVisibility();
		return;
	}
	else if(NumVisble == idx)
		ma_TrimmedBoundaryVisible.push_back(bVal);
	else
		ma_TrimmedBoundaryVisible[idx] = bVal;
}

vector<int> MSCompoG::GetTrimmedBoundaryVisibilityVec()
{
	return ma_TrimmedBoundaryVisible;
}

void MSCompoG::SetTrimmedBoundaryVisibilityVec(vector<int>& vec)
{
	ma_TrimmedBoundaryVisible.clear();

	auto it = vec.begin();
	for (; it != vec.end(); it++)
		ma_TrimmedBoundaryVisible.push_back(*it);
}

void MSCompoG::InitTrimmedBoundary(bool IsTopLevel)
{
	if(mp_TrimmedBoundary == NULL)
		mp_TrimmedBoundary = new GM2DPolyline(TRUE);
	GetBoundary(*mp_TrimmedBoundary);
}

GM2DPolyline* MSCompoG::GetTrimmedBoundary(bool IsTop)
{
	if(mp_TrimmedBoundary==NULL || mp_TrimmedBoundary->ma_Point.GetCount() < 2)
	{
		InitTrimmedBoundary();
		InitTrimmedBoundaryVisibility();
	}

	return mp_TrimmedBoundary;
}

void MSCompoG::SetTrimmedBoundary(GM2DPolyline* pPoly)
{
	if (pPoly == mp_TrimmedBoundary)
		return;

	if (mp_TrimmedBoundary == NULL)
		mp_TrimmedBoundary = new GM2DPolyline(TRUE);

	mp_TrimmedBoundary->CopyFrom(pPoly);
}

void MSCompoG::GetTrimmedBoundaryZLevel(GM2DPolyline& APoly, double dTopLevel, double dBotLevel)
{
	if (mp_TrimmedBoundary == NULL)
	{
		InitTrimmedBoundary();
		InitTrimmedBoundaryVisibility();
	}

	APoly.CopyFrom(mp_TrimmedBoundary);
}

void MSCompoG::BuildBoundaryByLevel( vector<GM2DPolyline>& BoundaryByLevelArr )
{

}

bool MSCompoG::HasTrimOption(MSConnection::TRIM_OPTION FindOpt, CON_POS CheckPos)
{
	for (int i = 0; i < ma_Connections.size(); i++)
	{
		MSConnection* pCon = ma_Connections[i];
		if (CheckPos == CON_ALL)
		{
			if (pCon->GetTrimOption(this) == FindOpt)
				return true;
		}
		else
		{
			if (pCon->GetTrimOption(this) == FindOpt &&
				pCon->GetLinkMemberConPos(this) == CheckPos)
				return true;
		}
	}

	return false;
}

void MSCompoG::CheckTrimOpt()
{
	bool HasMiterOpt = HasTrimOption(MSConnection::TO_MITER);
	if (HasMiterOpt == false)
		return;

	for (int i = 0; i < ma_Connections.size(); i++)
	{
		MSConnection* pCon = ma_Connections[i];
		if (pCon->GetTrimOption(this) == MSConnection::TO_ENDCUT ||
			pCon->GetTrimOption(this) == MSConnection::TO_EXTENDORIGIN)
		{
			if (pCon->GetFirstCompoG() == this)
				pCon->SetFirstTrimOption(MSConnection::TO_ORIGIN);
			else if(pCon->GetSecondCompoG() == this)
				pCon->SetSecondTrimOption(MSConnection::TO_ORIGIN);
		}
	}
}

void MSCompoG::UpdateTrimmedBoundary(long nUpdateConnIdx)
{
	CheckTrimOpt();
	//mp_PolylineInfo 가 존재하는 부재는 TrimPoly를 다른곳에서 계산한다.
	if (mp_PolylineInfo == nullptr)
	{
		if(m_DoUpdateTrimmedProfile)
		{
			TrimInitialize(nUpdateConnIdx);
			TrimIntersect(nUpdateConnIdx);
			TrimMerge(nUpdateConnIdx);
			m_DoUpdateTrimmedProfile = false;
		}
	}
	UpdateTrimmedBoundary_AfterCalculate();
	GM2DPolyline* pPoly = GetTrimmedBoundary();
	if(pPoly != nullptr)
		pPoly->MakeClosedPolyline(DTOL_MODELING);

	CalculateTrimmedBoundary_Acis();
}

void MSCompoG::ApplyMiterOptionToBoundary(MSConnection* pConnection)
{
	int opt1 = pConnection->GetFirstTrimOption();
	int opt2 = pConnection->GetSecondTrimOption();
	if ( opt1 == MSConnection::TO_MITER || opt2 == MSConnection::TO_MITER)
	{
		MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(this);
		MSCompoG* other = (this==pConnection->GetFirstCompoG() ? pConnection->GetSecondCompoG() : pConnection->GetFirstCompoG());
		MSLinkMemberG* link_other = dynamic_cast<MSLinkMemberG*>(other);
		if ((link != nullptr) && (link_other != nullptr))
		{
			GM2DLineVector rLine;
			link->FindMiterLineWithOtherLinkG(link_other, rLine);
			rLine.ExtendLineVector(TRUE, GetWidth() * 2, FALSE);
			rLine.ExtendLineVector(FALSE, GetWidth() * 2, TRUE);
			GM2DPolyline newBoundary(TRUE);
			link->MakeExtendedBoundary_ToLine(newBoundary, rLine);

			GM2DPolyline* pMyTrimmedBoundary = this->GetTrimmedBoundary();
			pMyTrimmedBoundary->CopyFrom(&newBoundary);

			link->SetTrimmedLines();			
		}
		UpdateTrimmedBoundary_AfterCalculate(pConnection->GetLinkMemberConPos(this));
	}
	
}

void MSCompoG::ApplySquareOffOptionToBoundary(MSConnection* pConnection)
{
	int opt1 = pConnection->GetFirstTrimOption();
	int opt2 = pConnection->GetSecondTrimOption();
	if (opt1 == MSConnection::TO_SQUAREOFF || opt2 == MSConnection::TO_SQUAREOFF)
	{
		MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(this);
		MSCompoG* other = (this == pConnection->GetFirstCompoG() ? pConnection->GetSecondCompoG() : pConnection->GetFirstCompoG());
		MSLinkMemberG* link_other = dynamic_cast<MSLinkMemberG*>(other);
		
		//Square-Off joint = Square-End (First) + Cut-End (Second)
		if (link != NULL && link_other != NULL)
		{
			if (link == pConnection->GetFirstCompoG())
				link->MakeSquareEnd(link_other);
			else
				link->MakeCutEnd(link_other);

			link->SetTrimmedLines(); 
			
		}
	}
}

void MSCompoG::MergeIntersectToBoundary(MSConnection* pConnection)
{
	// make mp_Trimmed union
	
	pConnection->MergeIntersectToBoundary(this);

	this->RemoveInwardWedge();

	pConnection->MergeIntersectToBoundary_After(this);
}

void MSCompoG::SetUserBoundary(GM2DPolyline* pUserBoundary)
{
	if (pUserBoundary != NULL)
	{
		m_IsUserBoundarySet = true;
		mp_UserBoundary = pUserBoundary;
	}
}

void MSCompoG::UnSetUserBoundary()
{
	m_IsUserBoundarySet = false;
	mp_UserBoundary = NULL;
}

MSConnection* MSCompoG::GetConnection(MSCompoG* pOverlapMember)
{
	
	long NumOverMem = ma_Connections.size();
	for (long iOverMem = 0; iOverMem < NumOverMem; iOverMem++)
	{
		MSConnection* pConnection = ma_Connections[iOverMem];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if (pPartnerG == NULL)
			continue;
		if (pPartnerG == pOverlapMember)
			return pConnection;
	}
	return NULL;
}

vector<MSConnection*> MSCompoG::GetConnectionByPos(char cLoc)
{
	CON_POS eConPos = CON_UNKNOWN;
	if(cLoc == 'S')
		eConPos = CON_START;
	else if(cLoc == 'E')
		eConPos = CON_END;
	else if(cLoc == 'M')
		eConPos = CON_MID;
	else if(cLoc == 'A')
		eConPos = CON_ALL;

	return GetConnectionByPos(eConPos);
}
vector<MSConnection*> MSCompoG::GetConnectionByPos(CON_POS eConPos, bool checkSameFloor)
{
	if(eConPos == CON_ALL)
		return ma_Connections;

	if(IsLinkMember())
	{
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)this;
		vector<MSConnection*> ConnArr;
		long NumConn = ma_Connections.size();
		for(long iConn = 0 ; iConn < NumConn ; iConn++)
		{
			MSConnection* pConn = ma_Connections[iConn];
			if (pConn->IsSameFloor() == false && checkSameFloor)
				continue;

			CON_POS eCurConPos = pConn->GetLinkMemberConPos(this);
			if(eConPos == eCurConPos)
				ConnArr.push_back(pConn);
		}

		return ConnArr;
	}
	else 
	{
		return ma_Connections;
	}
}

bool MSCompoG::IsSetbyUserConnection(CON_POS eConPos)
{
	vector<MSConnection*> ConnArr = GetConnectionByPos(eConPos);
	for (long iConn = 0; iConn < ConnArr.size(); iConn++)
	{
		MSConnection* pConn = ConnArr[iConn];
		if (pConn->m_SetByUser)
			return true;
	}
	return false;
}

void MSCompoG::ClearOverlapFloor()
{
	ma_OverlapFloor.clear();
}

void MSCompoG::SetOverlapFloor()
{
	ma_OverlapFloor.clear();
	double dTopLevel = 0;
	double dBotLevel = 0;
	if (GetFloor()->GetFloorNum() == 0)
	{
		ma_OverlapFloor.push_back(GetFloor());
		return;
	}

	GetZLevels(dTopLevel, dBotLevel);

// 	MSFloor* pFloor = GetFloor();
// 	ma_OverlapFloor.push_back(pFloor);
// 	MSFloor* pUpperFloor = pFloor->GetUpperFloor();
// 	if(pUpperFloor != NULL && dTopLevel > pUpperFloor->GetBotCompoGZLevel())
// 		ma_OverlapFloor.push_back(pUpperFloor);
// 
// 	MSFloor* pLowerFloor = pFloor->GetLowerFloor();
// 	if(pLowerFloor != NULL && dBotLevel < pLowerFloor->GetTopCompoGZLevel())
// 		ma_OverlapFloor.push_back(pLowerFloor);
// 
// 	if(pUpperFloor != NULL && dTopLevel > pUpperFloor->GetTopCompoGZLevel() || 
// 		pLowerFloor != NULL && dBotLevel < pLowerFloor->GetBotCompoGZLevel())
		
	ma_OverlapFloor = GetFloor()->GetFloorsByLevel(dBotLevel, dTopLevel);
}

vector<MSFloor*> MSCompoG::GetOverlapFloor()
{
	if (ma_OverlapFloor.size() == 0)
		SetOverlapFloor();

	return ma_OverlapFloor;
}

vector<MSCompoG*> MSCompoG::FindConnectMember(FindType_MSElement memberType, long nFloorType, double dTol, bool bUseTrimedBoundry, bool bFindPolyMemberOnlyOnLine, bool bIncludeOnLine)
{
	return MSCompoGUtil::FindConnectMember(this, memberType, nFloorType, dTol, bUseTrimedBoundry, bFindPolyMemberOnlyOnLine, bIncludeOnLine);
}

bool MSCompoG::HasTrimOption(int nOpt, Type_MSCompoG ePartnerType)
{
	long NumConn = ma_Connections.size();
	for(long iConn = 0 ; iConn < NumConn; iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		MSCompoG* pPartner = pConn->GetPartner(this);
		if(pPartner == nullptr)
			continue;
		Type_MSCompoG eType = pPartner->GetType();

		int nCurOpt = pConn->GetTrimOption(this);
		if(ePartnerType != MSCompoG::msNULL)
		{
			if(eType == ePartnerType && nCurOpt == nOpt)
				return true;
		}
		else
		{
			if(nCurOpt == nOpt)
				return true;
		}
	}

	return false;
}

bool MSCompoG::isXDirection()
{
	double dDeg = GetMemberDegree();
	if (dDeg >= 0 && dDeg <= 45 || dDeg >= 135 && dDeg <= 225 || dDeg >= 315 && dDeg <= 360)
		return true;
	
	return false;
}

bool MSCompoG::IsReverseDir(bool bApplyReverseBox)
{
	//270~90 사이이면 false 91~269 사이이면 true
	bool bRval = false;

	double dDeg = GetMemberDegree();
	if(dDeg > 90 + DTOL_GM && dDeg <= 270 + DTOL_GM)
		bRval = true;
	else 
		bRval = false;

	if(bApplyReverseBox)
	{
		if(m_IsReverseBox)
			bRval = !bRval;
	}

	return bRval;
}

void MSCompoG::TrimInitialize(long nUpdateConnIdx)
{
	if(nUpdateConnIdx == 0)
	{
		DeleteTrimmedBoundary();
		ma_TrimmedBoundaryVisible.clear();
		DeleteTrimmedBoundary_Acis();

		if (m_IsUserBoundarySet)//TODO 지금 상태로는 connection계산시 my가 아닌 다른 compog의 userboundary는 누락될 수 있다.
		{
			mp_UserBoundary->CopyTo(mp_TrimmedBoundary);
			InitTrimmedBoundaryVisibility();
		}
	}
}

void MSCompoG::TrimIntersect(long nUpdateConnIdx)
{
	for (int iOverlapMember = nUpdateConnIdx; iOverlapMember < ma_Connections.size(); iOverlapMember++)
	{
		MSConnection* pConnection = ma_Connections[iOverlapMember];
		if (pConnection->IsLevelTouch()) continue;
		ApplyMiterOptionToBoundary(pConnection);
		ApplySquareOffOptionToBoundary(pConnection);
	}

	for (int iOverlapMember = nUpdateConnIdx; iOverlapMember < ma_Connections.size(); iOverlapMember++)
	{
		MSConnection* pConnection = ma_Connections[iOverlapMember];
		if (pConnection->IsLevelTouch()) continue;
		pConnection->MakeIntersectBoundary();
	}
}

void MSCompoG::TrimMerge(long nUpdateConnIdx)
{
	if(ma_Connections.size() == 0)
		return;

	for (int iOverlapMember = nUpdateConnIdx; iOverlapMember < ma_Connections.size(); iOverlapMember++)
	{
		MSConnection* pConnection = ma_Connections[iOverlapMember];
		if (pConnection->IsLevelTouch()) continue;
		MergeIntersectToBoundary(pConnection);
	}

	RemoveOutwardWedge();
	GM2DPolyline* pMyBoundary = this->GetTrimmedBoundary();
	pMyBoundary->DeleteInLinePoint();
	double dTol = 10;
	if (IsLinkMember())
		pMyBoundary->DeleteParallelLine(dTol);
}

void MSCompoG::AddCompoGZLevelInFloor()
{
	if (IsSubMember())
		return;

	MSFloor* pFloor = GetFloor();
	double dTopZLevel = 0;
	double dBotZLevel = 0;
	if(GetZLevels(dTopZLevel, dBotZLevel))
	{
		if(pFloor != NULL)
		{
			if (GetType() == MSElement::msSlabG && ((MSSlabG*)this)->IsSlopeMember())
				((MSSlabG*)this)->GetZLevels_Slope(dTopZLevel, dBotZLevel);

			pFloor->AddCompoGZLevel(dBotZLevel);
			pFloor->AddCompoGZLevel(dTopZLevel);
		}
	}
}

void MSCompoG::DelCompoGZLevelInFloor()
{
	if (IsSubMember())
		return;

	MSFloor* pFloor = GetFloor();
	double dTopZLevel = 0;
	double dBotZLevel = 0;
	if(GetZLevels(dTopZLevel, dBotZLevel))
	{
		if(pFloor != NULL)
		{
			if (GetType() == MSElement::msSlabG && ((MSSlabG*)this)->IsSlopeMember())
				((MSSlabG*)this)->GetZLevels_Slope(dTopZLevel, dBotZLevel);

			pFloor->DelCompoGZLevel(dBotZLevel);
			pFloor->DelCompoGZLevel(dTopZLevel);
		}
	}
}

map<long, MSFloor*> MSCompoG::GetConnectionSlabGID()
{
	return mm_ConnectionSlabID;
}

void MSCompoG::SetConnectionSlabGID()
{
	mm_ConnectionSlabID.clear();
	MSElement::Type_MSCompoG eType = GetType();
	//if(eType == MSElement::msSlabG || eType == MSElement::msHandRailG || IsSteel())
	if (eType == MSElement::msHandRailG)
		return;

	double dTopLevel = 0;
	double dBotLevel = 0;
	GetZLevels(dTopLevel, dBotLevel);

	vector<MSCompoG*> SlabGArr = FindConnectMember(FindType_MSElement::msSlabG, FIND_ALLFLOOR);
	for(long iSlab = 0 ; iSlab < SlabGArr.size() ; iSlab++)
	{
		MSSlabG* pSlabG = (MSSlabG*)SlabGArr[iSlab];

		double dSlabTopLevel = 0;
		double dSlabBotLevel = 0;
		pSlabG->GetZLevels(dSlabTopLevel, dSlabBotLevel);
		//void MSSlabG::UpdateInnerProfile() 와 찾는 로직이 일치 해야함
		if (dSlabBotLevel > dTopLevel || dSlabTopLevel < dBotLevel)
			continue;

		MSFloor* pFloor = pSlabG->GetFloor();
		if (pFloor == NULL) 
			continue;
		mm_ConnectionSlabID.insert(make_pair(pSlabG->m_ID, pFloor));
	}
}

void MSCompoG::UpdateConnectionSlabInnerProFile()
{
	MSElement::Type_MSCompoG eType = GetType();
	if(eType == MSElement::msSlabG)
		return;

	MSFloor* pFloor = NULL;
	map<long, MSFloor*>::iterator it;
	for (it = mm_ConnectionSlabID.begin(); it != mm_ConnectionSlabID.end(); it++)
	{
		pFloor = it->second;
		MSSlabG* pSlabG = (MSSlabG*)pFloor->GetCompoG(it->first);
		if (pSlabG == nullptr)
		{
			ASSERT(0);
			continue;
		}
		pSlabG->UpdateInnerProfile();
		pFloor->CalculateUpdatedCompoGArray(pSlabG, pFloor->CompoGIDList_NeedRegen_2DView_CurTR());
	}
}

void MSCompoG::DeleteVecInput()
{
	long NumVec = ma_Vec_Input.size();
	for(long iVec = 0 ; iVec < NumVec ; iVec++)
	{
		GM2DVector* pVec = ma_Vec_Input[iVec];
		delete pVec;
	}

	ma_Vec_Input.clear();
}

void MSCompoG::RecoverConnectionInCompoGCopyNPaste(map<long, MSCompoG*> OldCompoGMap)
{
	map<long, MSCompoG*>::iterator it;
	for(long i = 0 ; i < ma_Connections.size() ; i++)
	{
		MSConnection* pConn = ma_Connections[i];
		if(pConn == NULL)
			continue;
		 it = OldCompoGMap.find(pConn->GetFirstCompoGID()); 
		 if(it != OldCompoGMap.end())
			 pConn->SetFirstCompoG(it->second);

		 it = OldCompoGMap.find((pConn->GetSecondCompoGID()));
		 if(it != OldCompoGMap.end())
			 pConn->SetSecondCompoG(it->second);

		 pConn->SetHistoryID();
	}

	// Haunches
	RecoverPointerAfterSerialize_SubMembers();
	RemoveSameConnection();
}

MSMaterial* MSCompoG::GetMaterial()
{
	MSMaterial* pMaterial = nullptr;
	MSCompoM* pCompoM = GetCompoM();
	if (pCompoM != nullptr)
	{
		if (pCompoM->GetIsUsedStoryMaterial())
		{
			MSBaseBuilding* pBldg = pCompoM->mp_Owner;
			MSMaterial::Type_MSMaterial matType = pCompoM->GetMaterialType();
			BOOL isVerticalMember = IsVerticalMember();
			int floorNum = GetFloorNum();

			MSStoryMaterial* pStoryMaterial = pBldg->GetStoryMaterialByFloorNum(floorNum);
			if (pStoryMaterial != nullptr)
			{
				bool IsVert = false;
				if (isVerticalMember)	IsVert = true;
				pMaterial = pStoryMaterial->GetMaterial(matType, IsVert);
			}
			else
				pMaterial = pCompoM->GetMaterial();
		}
		else
			pMaterial = pCompoM->GetMaterial();
	}

	if(pMaterial == nullptr)
		pMaterial = GetBuilding()->GetDefMaterial();
	return pMaterial;
}

vector<long> MSCompoG::GetMaterialIDs()
{
	vector<long> MaterialIDs;
	MSMaterial* pMaterial = GetMaterial();
	if (pMaterial == NULL)	return MaterialIDs;

	MaterialIDs.push_back(pMaterial->m_ID);
	if (pMaterial->GetType() == MSMaterial::msSRC)
	{
		MSSrcMaterial* pSrcMaterial = (MSSrcMaterial*)pMaterial;
		MaterialIDs.push_back(pSrcMaterial->mp_RcMaterial->m_ID);
		MaterialIDs.push_back(pSrcMaterial->mp_SteelMaterial->m_ID);
	}
	return MaterialIDs;
}

MSMaterial* MSCompoG::GetInnerConcMaterial()
{
	if ((GetType() == MSElement::msColumnG && IsSRC()) == false)
		return GetMaterial();

	MSCompoM* pCompoM = GetCompoM();
	MSSrcColumnM* srcColumnM = dynamic_cast<MSSrcColumnM*>(pCompoM);
	if (srcColumnM->GetIsUsedStoryInnerConcMaterial())
	{
		MSBaseBuilding* pBldg = srcColumnM->mp_Owner;
		MSMaterial::Type_MSMaterial matType = srcColumnM->GetMaterialType();
		BOOL isVerticalMember = IsVerticalMember();
		int floorNum = GetFloorNum();
		MSStoryMaterial* pStoryMaterial = pBldg->GetStoryMaterialByFloorNum(floorNum);
		if (pStoryMaterial == NULL)
		{
			ASSERT(0);
			return pBldg->GetDefMaterial(matType);
		}
		bool IsVert = false;
		if (isVerticalMember)	IsVert = true;
		return pStoryMaterial->GetMaterial(matType, IsVert);
	}
	else
		return srcColumnM->GetInnerConcMaterial();

	MSBaseBuilding* pbuilding = GetBuilding();
	if (pbuilding == nullptr)
		return nullptr;

	return pbuilding->GetDefMaterial();
}

bool MSCompoG::GetIsUsedStoryMaterial()
{
	MSCompoM* pCompoM = GetCompoM();
	if (pCompoM == NULL)
		return true;
	return pCompoM->GetIsUsedStoryMaterial();
}

bool MSCompoG::IsRC()
{
	MSCompoM* pCompomM = GetCompoM();
	if (pCompomM != NULL)
		return pCompomM->IsRC();

	return false;
}

bool MSCompoG::IsSteel()
{
	MSCompoM* pCompomM = GetCompoM();
	if (pCompomM != NULL)
		return pCompomM->IsSteel();

	return false;
}

bool MSCompoG::IsSRC()
{
	MSCompoM* pCompomM = GetCompoM();
	if (pCompomM != NULL)
		return pCompomM->IsSRC();

	return false;
}

bool MSCompoG::IsReverseBeam()
{
	MSCompoM* pCompomM = GetCompoM();
	auto msRcFWBeamM = dynamic_cast<MSRcFWBeamM*>(pCompomM);
	if (msRcFWBeamM != nullptr)
		return msRcFWBeamM->IsReversBeamM();

	return false;
}

RT_MemberType MSCompoG::GetCompoGTypeToMemberTemplateType()
{
	RT_MemberType nMemberTemplateType = (RT_MemberType)-1;

	Type_MSCompoG type = GetType();

	if (type == MSElement::msColumnG)
	{
		nMemberTemplateType = IsTransfer() ? RT_MemberType::RT_TransferColumn : RT_MemberType::RT_Column;
	}
	else if (type == MSElement::msBeamG)
	{
		if(IsTransfer())
			nMemberTemplateType = RT_MemberType::RT_TransferBeam;
		else if(IsDeck())
			nMemberTemplateType = RT_MemberType::RT_DeckBeam;
		else
			nMemberTemplateType = RT_MemberType::RT_Beam;
		
	}
	else if (type == MSElement::msWallColumnG)
		nMemberTemplateType = RT_MemberType::RT_Column;
	else if (type == MSElement::msWallG)
	{
		MSCompoM::Type_MSCompoM compoMType = GetCompoM()->GetType();
		if (compoMType == MSCompoM::Type_MSCompoM::msBWallM)
			nMemberTemplateType = RT_MemberType::RT_BWall;
		else if (compoMType == MSCompoM::Type_MSCompoM::msRWallM)
			nMemberTemplateType = RT_MemberType::RT_RWall;
	}
	else if (type == MSElement::msPitG)
		nMemberTemplateType = RT_MemberType::RT_Pit;
	else if (type == MSElement::msIsoFootG)
		nMemberTemplateType = RT_MemberType::RT_IsoFoot;
	else if (type == MSElement::msWallFootG)
		nMemberTemplateType = RT_MemberType::RT_WallFoot;
	else if (type == MSElement::msSlabG)
	{
		nMemberTemplateType = IsTransfer() ? RT_MemberType::RT_TransferSlab : RT_MemberType::RT_Slab;
	}
	else if(type == MSElement::msStairG)
		nMemberTemplateType = RT_MemberType::RT_Slab;
	else
	{
		if (IsSubMember())
		{
			MSCompoG* pCompoG = (MSCompoG*)GetOwnerCompoG();

			if (pCompoG != NULL)
			{
				type = GetOwnerCompoG()->GetType();

				if (type == MSElement::msColumnG)
					nMemberTemplateType = RT_MemberType::RT_Column;
				else if (type == MSElement::msBeamG)
					nMemberTemplateType = RT_MemberType::RT_Beam;
				else if (type == MSElement::msWallG)
				{
					MSCompoM::Type_MSCompoM compoMType = GetCompoM()->GetType();
					if (compoMType == MSCompoM::Type_MSCompoM::msBWallM)
						nMemberTemplateType = RT_MemberType::RT_BWall;
					else if (compoMType == MSCompoM::Type_MSCompoM::msRWallM)
						nMemberTemplateType = RT_MemberType::RT_RWall;
				}
				else if (type == MSElement::msPitG)
					nMemberTemplateType = RT_MemberType::RT_Pit;
				else if (type == MSElement::msSlabG)
					nMemberTemplateType = RT_MemberType::RT_Slab;
			}
		}
		else if (type == MSElement::msHandRailG)
			nMemberTemplateType = RT_MemberType::RT_LowWall;
	}

	return nMemberTemplateType;
}

BaseRebarTemplate* MSCompoG::GetRebarTemplate()
{
	BaseRebarTemplate* rebarTemplate = nullptr;

	RT_MemberType type = GetCompoGTypeToMemberTemplateType();
	
	int nRebarTemplateID = GetRebarTemplateID();

	if (nRebarTemplateID != -1)
		rebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplate(type, nRebarTemplateID);
	else
		rebarTemplate = GetFloorForUser()->GetRebarTemplate(type);

	return rebarTemplate;
}

CString MSCompoG::GetRebarTemplateName()
{
	CString sRebarTemplaeName = RebarTemplateManager::GetInstance()->GetNoneRebarTemplateName();

	int nRebarTemplateID = GetRebarTemplateID();

	if (nRebarTemplateID != -1)
		sRebarTemplaeName = RebarTemplateManager::GetInstance()->GetRebarTemplateByID(nRebarTemplateID)->m_sName;

	return sRebarTemplaeName;
}

void MSCompoG::SetRebarTemplateName(CString sName)
{
	if (sName == RebarTemplateManager::GetInstance()->GetNoneRebarTemplateName())
		m_nRebarTemplateID = -1;
	else
	{
		RebarTemplate* pRebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplateByName(sName);
		if(pRebarTemplate != nullptr)
			m_nRebarTemplateID = pRebarTemplate->m_nID;
	}
}

int MSCompoG::GetRebarTemplateID()
{
	RebarTemplate* rebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplateByID(m_nRebarTemplateID);
	if (rebarTemplate == nullptr)
		m_nRebarTemplateID = -1;

	return m_nRebarTemplateID;
}

FormWorkTemplate* MSCompoG::GetFormWorkTemplate()
{
	FormWorkTemplate* formworkTemplate = nullptr;

	int nFormWorkTemplateID = GetFormWorkTemplateID();

	if (nFormWorkTemplateID != -1)
		formworkTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(nFormWorkTemplateID);
	else
		formworkTemplate = GetFloorForUser()->GetFormWorkTemplate();

	return formworkTemplate;
}

CString MSCompoG::GetFormWorkTemplateName()
{
	CString sFormWorkTemplaeName = FormWorkTemplateManager::GetInstance()->GetNoneFormWorkTemplateName();

	int nFormWorkTemplateID = GetFormWorkTemplateID();

	if (nFormWorkTemplateID != -1)
		sFormWorkTemplaeName = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(nFormWorkTemplateID)->m_sName;

	return sFormWorkTemplaeName;
}

void MSCompoG::SetFormWorkTemplateName(CString sName)
{
	if (sName == FormWorkTemplateManager::GetInstance()->GetNoneFormWorkTemplateName())
		m_nFormWorkTemplateID = -1;
	else
	{
		FormWorkTemplate* pFormWorkTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByName(sName);
		m_nFormWorkTemplateID = pFormWorkTemplate->m_nID;
	}
}

int MSCompoG::GetFormWorkTemplateID()
{
	FormWorkTemplate* formWorkTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(m_nFormWorkTemplateID);
	if (formWorkTemplate == nullptr)
		m_nFormWorkTemplateID = -1;

	return m_nFormWorkTemplateID;
}

void MSCompoG::GetMyUrBoundary(MSCompoG* pMyCompoG, MSCompoG* pUrCompoG, GM2DPolyline* &pMyBoundary, GM2DPolyline* &pUrBoundary)
{

	if (pMyCompoG->GetType() == MSElement::msColumnG && pUrCompoG->IsLinkMember())
	{
		if (pUrCompoG->IsFloorMember())
		{
			pUrBoundary = pUrCompoG->GetTrimmedBoundary();
			double dTopLevel = 0, dBotLevel = 0;
			pUrCompoG->GetZLevels(dTopLevel, dBotLevel);
			pMyCompoG->GetTrimmedBoundaryZLevel(*pMyBoundary, dTopLevel, dBotLevel);
		}
		else
		{
			MSColumnG* pMyColumnG = (MSColumnG*)pMyCompoG;
			pMyColumnG->GetSecPolylineByInsertVec(*pMyBoundary);
			pUrBoundary = pUrCompoG->GetTrimmedBoundary();
		}
	}
	else if (pUrCompoG->GetType() == MSElement::msColumnG && pMyCompoG->IsLinkMember())
	{
		if (pMyCompoG->IsFloorMember())
		{
			pMyBoundary = pMyCompoG->GetTrimmedBoundary();
			double dTopLevel = 0, dBotLevel = 0;
			pMyCompoG->GetZLevels(dTopLevel, dBotLevel);
			pUrCompoG->GetTrimmedBoundaryZLevel(*pUrBoundary, dTopLevel, dBotLevel);
		}
		else
		{
			MSColumnG* pUrColumnG = (MSColumnG*)pUrCompoG;
			pUrColumnG->GetSecPolylineByInsertVec(*pUrBoundary);
			pMyBoundary = pMyCompoG->GetTrimmedBoundary();
		}
	}
	else
	{
		pMyBoundary = pMyCompoG->GetTrimmedBoundary();
		pUrBoundary = pUrCompoG->GetTrimmedBoundary();
	}
}

void MSCompoG::MakeCCW()
{
	GM2DPolyline* pPoly = GetProfile();
	if (pPoly != NULL)
		pPoly->MakeCCW();
}

bool MSCompoG::IsEachCompoM()
{
	if (mp_CompoM != NULL && mp_CompoM->m_ID == 0)
		return true;

	return false;
}

bool MSCompoG::IsCustomM()
{
	if (mp_CompoM == nullptr)
		return false;

	if (mp_CompoM->IsCustomM())
		return true;

	return false;
}

bool MSCompoG::IsPolygonSection()
{
	if (mp_CompoM == nullptr)
		return false;

	if (mp_CompoM->IsPolygonSection())
		return true;

	return false;
}

bool MSCompoG::IsCustomColumn()
{
	if (mp_CompoM == nullptr)
		return false;

	if (mp_CompoM->IsCustomM())
	{
		MSCustomM* pCustomM = (MSCustomM*)mp_CompoM;
		if(pCustomM->IsCustomColumnSection())
			return true;
	}

	return false;
}

bool MSCompoG::IsCustomWallColumn()
{
	if (mp_CompoM == nullptr)
		return false;

	if (mp_CompoM->IsCustomM())
	{
		MSCustomM* pCustomM = (MSCustomM*)mp_CompoM;
		if (pCustomM->IsCustomWallColumnSection())
			return true;
	}

	return false;
}

bool MSCompoG::IsCustomBeam()
{
	if (mp_CompoM == nullptr)
		return false;

	if (mp_CompoM->IsCustomM())
	{
		MSCustomM* pCustomM = (MSCustomM*)mp_CompoM;
		if (pCustomM->IsCustomBeamSection())
			return true;
	}

	return false;
}

bool MSCompoG::IsCustomFWBeam()
{
	MSBeamG* beamG = dynamic_cast<MSBeamG*>(this);
	if (beamG == nullptr)
		return false;
	return beamG->IsCustomFWBeam();
}

vector<double> MSCompoG::GetUserRebarWeights()
{
	vector<double> RebarWeights;
	for (int i = 0; i < ma_UserRebar.size(); i++)
		RebarWeights.push_back(ma_UserRebar[i].value);
	return RebarWeights;
}
vector<CString> MSCompoG::GetUserRebarCalcultingFormulas()
{
	vector<CString> RebarCalcultingFormulas;
	for (int i = 0; i < ma_UserRebar.size(); i++)
		RebarCalcultingFormulas.push_back(_T("사용자입력"));
	return RebarCalcultingFormulas;
}
vector<CString> MSCompoG::GetUserRebarDiaPrintNames()
{
	vector<CString> RebarDiaPrintNames;
	MSRebarBuilding* pBuilding = dynamic_cast<MSRebarBuilding*>(GetBuilding());
	if (pBuilding == NULL)
		return RebarDiaPrintNames;

	MSSteelGradeManager* pManager = pBuilding->m_pSGradeManager;
	
	MSMaterial* pMatrtail = GetMaterial();
	MSReinforceBarSetting* pSet = pMatrtail->GetReinforceBarSetting();
	
	for (int i = 0; i < ma_UserRebar.size(); i++)
		RebarDiaPrintNames.push_back(pManager->GetPrintName(pManager->GetSGrade(ma_UserRebar[i].strength)));
	return RebarDiaPrintNames;
}
vector<CString> MSCompoG::GetUserRebarDiaIndexs()
{
	vector<CString> RebarDiaIndexs;
	for (int i = 0; i < ma_UserRebar.size(); i++)
		RebarDiaIndexs.push_back(ma_UserRebar[i].dia);
	return RebarDiaIndexs;
}

GM2DPolyline* MSCompoG::GetProfile()
{
	GM2DPolyline* pProfile = MSElement::GetProfile();
	if (pProfile != nullptr)
		return pProfile;

	if (mp_Boundary == NULL || mp_Boundary->GetSize() == 0)
		MakeBoundary();
	return mp_Boundary;
}

void MSCompoG::CalculateTrimmedBoundary_Acis()
{
	if (GetType() == MSElement::msSlabG)
	{
		// MSSlabG::SetProfile()에서 처리를 원칙으로함
		// 데이터 업데이트시 처리하기위해 추가함
// 		if(mp_TrimmedBoundary_Acis == nullptr || mp_TrimmedBoundary_Acis->SamePolyLine(mp_Boundary) == false)
// 			SetTrimmedBoundary_Acis(mp_Boundary);
		if (mp_TrimmedBoundary_Acis == nullptr || mp_TrimmedBoundary_Acis->SamePolyLine(mp_PolylineInfo->mp_Profile) == false)
			SetTrimmedBoundary_Acis(mp_PolylineInfo->mp_Profile);
	}
	else
	{
		DeleteTrimmedBoundary_Acis();
		mp_TrimmedBoundary_Acis = new GM2DPolyline();
		mp_TrimmedBoundary_Acis->CopyFrom(GetTrimmedBoundary());

		if (AppGlobalOptionWrapper::Instance()->GetTrim3DMode() == 1)
			mp_TrimmedBoundary_Acis->DeleteInLinePoint(DTOL_MODELING);
		else if (AppGlobalOptionWrapper::Instance()->GetTrim3DMode() == 0)
		{
			bool isTrimmed = false;
			for (int idx = 0; idx < ma_Connections.size(); idx++)
			{
				MSConnection* pConn = ma_Connections[idx];
				MSConnection::TRIM_OPTION conOpt = pConn->GetTrimOption(this);
				MSCompoG* pPartnerG = pConn->GetPartner(this);
				Type_MSCompoG partnerGType = pPartnerG->GetType();

				CON_POS curPos = pConn->GetLinkMemberConPos(this);

				if (pPartnerG->IsPointMember())
				{
					if (curPos == CON_POS::CON_START || curPos == CON_POS::CON_END)
					{
						vector<GM2DPolyline*> polyArr = pConn->GetIntersects();
						vector<GM2DPolyline*> Joined_Polys;
						for (int jdx = 0; jdx < polyArr.size(); jdx++)
						{
							mp_TrimmedBoundary_Acis->BoolOperationNSortByArea('A', polyArr[jdx], Joined_Polys, DTOL_MODELING);
							if (Joined_Polys.size() > 0)
							{
								mp_TrimmedBoundary_Acis->CopyFrom(Joined_Polys[0]);
								isTrimmed = true;
							}
						}
					}
				}
			}

			if (isTrimmed == true)
				mp_TrimmedBoundary_Acis->FixPolyLine();
			else
				mp_TrimmedBoundary_Acis->DeleteInLinePoint(DTOL_MODELING);
		}
	}
}

GM2DPolyline* MSCompoG::GetTrimmedBoundary_Acis()
{
	if (mp_TrimmedBoundary_Acis == nullptr)
		CalculateTrimmedBoundary_Acis();
	return mp_TrimmedBoundary_Acis;
}

void MSCompoG::SetTrimmedBoundary_Acis(GM2DPolyline* pPoly)
{
	if (pPoly == mp_TrimmedBoundary_Acis)
		return;

	if (mp_TrimmedBoundary_Acis == NULL)
		mp_TrimmedBoundary_Acis = new GM2DPolyline(TRUE);
	mp_TrimmedBoundary_Acis->CopyFrom(pPoly);
}

void MSCompoG::UpdateBoundaryVisibilities(long nUpdateConnIdx)
{
	UpdateBoundaryVisibilities_After();
}

void MSCompoG::UpdateBoundaryVisibilities_After()
{
	CalculateTrimmedBoundary_Acis();
	SetTrimmedLines();
}

void MSCompoG::SetTrimmedLines()
{

}

void MSCompoG::RemoveSameConnection()
{
	// 파일 오픈시 호출되는 경우에는 문제가 없으나
	// 오픈 이후 모델링 중에 중복된 pConnection을 삭제하는 경우 HistoryManager에서 문제가 발생하여 삭제하지 않도록 수정
	// ma_Connections에서만 제거하도록 수정함

	for (int i = ma_Connections.size() - 1; i >= 0 ; i--)
	{
		MSConnection* pConnection = ma_Connections[i];
		for (int j = i - 1; j >= 0 ; j--)
		{
			if (pConnection->IsSameConnection(ma_Connections[j]))
			{
				ASSERT(0);
				CString strMsg;
				strMsg.Format(_T("Has Same Connection(IDx: %2d, %2d) in compoG(ID:%d FirstCompoGID:%d SecondCompoGID:%d)"), i, j, m_ID, pConnection->GetFirstCompoGID(), pConnection->GetSecondCompoGID());
				StatusReporterWrapper::OutputWarning(strMsg);
				MSCompoG* pPartnerG = pConnection->GetPartner(this);
				if (pPartnerG != nullptr)
					pPartnerG->RemoveConnection(pConnection);
//				if(pConnection != ma_Connections[j])
//					delete pConnection;
				ma_Connections.erase(ma_Connections.begin() + i);
				break;
			}
		}
	}
}

void MSCompoG::LoggingConnections()
{
	for (int i = 0; i < ma_Connections.size(); i++)
	{
		CString strConn = ma_Connections[i]->GetLogingConnection();
		CString strLog;
		strLog.Format(_T("CompoGID:%7d , ConIdx:%2d, Log:%s"), m_ID, i, strConn);
		StatusReporterWrapper::OutputMessage(strLog);
		LoggerWrapper::Log(strLog);
	}
}

void MSCompoG::SortByPriorityNum(vector<MSCompoG*>& CompoGArr)
{
	sort(CompoGArr.begin(), CompoGArr.end(),
		[](MSCompoG* a, MSCompoG* b)->bool {return a->GetPriorityNum() < b->GetPriorityNum(); }
	);

	vector<MSCompoG*>::iterator new_end;
	new_end = unique(CompoGArr.begin(), CompoGArr.end());
	CompoGArr.erase(new_end, CompoGArr.end());
}

bool MSCompoG::HasSubMember()
{
	// 단차벽 새기획들어오면 이부분 수정해야 함
	return false;
}

void MSCompoG::DeleteSubMembers()
{
	// 단차벽 새기획들어오면 이부분 수정해야 함
}

void MSCompoG::ClearAllQuantityData()
{
	ma_VolumeInfo.clear();
	ma_AreaInfo.clear();
	ma_UserFormWork.clear();
	ma_UserRebar.clear();
	m_VolumeAreaEMS.ClearAll();
	m_RebarQuantityInfo.ClearAllQuantity();
	m_IsQuantityReplaced = false;
}

bool MSCompoG::Add(MSAddRebarBase* pRebar)
{
	pair<map<long, MSAddRebarBase*>::iterator, bool> bPair;
	pRebar->SetOwnerCompoG(this);
	pRebar->SetBuilding(GetBuilding());
	bPair = mm_AddRebar.insert(make_pair(pRebar->GetID(), pRebar));
	return bPair.second;
}

long MSCompoG::Remove(MSAddRebarBase* pRebar)
{
	long nID = pRebar->m_ID;
	map<long, MSAddRebarBase*>::iterator it = mm_AddRebar.find(nID);
	if (it == mm_AddRebar.end())	return 0;
	mm_AddRebar.erase(it);
	return nID;
}

long MSCompoG::Delete(MSAddRebarBase* pRebar)
{
	long nID = Remove(pRebar);
	delete pRebar;
	return nID;
}

void MSCompoG::DeleteAddRebars()
{
	map<long, MSAddRebarBase*>::iterator it;
	for (it = mm_AddRebar.begin(); it != mm_AddRebar.end(); it++)
		delete it->second;
	mm_AddRebar.clear();
}

bool MSCompoG::GetIsTempCompoG()
{
	return m_IsTempCompoG;
}

void MSCompoG::SetIsTempCompoG(bool value)
{
	m_IsTempCompoG = value;
}

eConstructionPhase MSCompoG::GetConstructionPhase() 
{
	return m_eConstructionPhase;
}

void MSCompoG::SetConstructionPhase(eConstructionPhase value)
{
	m_eConstructionPhase = value;
}

bool MSCompoG::RemoveSubMember(MSElement* element)
{
	switch (element->GetType())
	{
	case MSElement::msAddRebarAreaBent:
	case MSElement::msAddRebarAreaLine:
	case MSElement::msAddRebarAreaShape:
	{
		Remove((MSAddRebarBase*)element);
		return true;
	}
	}
	return MSElement::RemoveSubMember(element);
}

void MSCompoG::AddUserConcreteQuantity(vector<VolumeAreaInfo>& userQuanityArr)
{
	for (int i = 0; i < userQuanityArr.size(); i++)
	{
		if (userQuanityArr[i].use_user_input == false) continue;

		ma_VolumeInfo.push_back(userQuanityArr[i]);
	}
}

void MSCompoG::AddUserAreaQuantity(vector<VolumeAreaInfo>& userQuanityArr)
{
	for (int i = 0; i < userQuanityArr.size(); i++)
	{
		if (userQuanityArr[i].use_user_input == false) continue;

		ma_AreaInfo.push_back(userQuanityArr[i]);
	}
}

void MSCompoG::AddUserRebarQuantity(vector<RebarQuantityUser>& userQuanityArr)
{
	for (int i = 0; i < userQuanityArr.size(); i++)
		ma_UserRebar.push_back(userQuanityArr[i]);
}

void MSCompoG::ClearUserQuantityData()
{
	for (int i = ma_VolumeInfo.size() - 1; i >= 0; i--)
	{
		if (ma_VolumeInfo[i].use_user_input)
		{
			ma_VolumeInfo.erase(ma_VolumeInfo.begin() + i);
		}
	}
	
	for (int i = ma_AreaInfo.size() - 1; i >= 0; i--)
	{
		if (ma_AreaInfo[i].use_user_input)
		{
			ma_AreaInfo.erase(ma_AreaInfo.begin() + i);
		}
	}

	ma_UserRebar.clear();
}

void MSCompoG::ClearQuantityData()
{
	m_VolumeAreaEMS.ClearAll();
	m_RebarQuantityInfo.ClearAllQuantity();
}

bool MSCompoG::HasUserQuantityData()
{
	for (int i = 0; i < ma_VolumeInfo.size(); ++i)
	{
		if (ma_VolumeInfo[i].use_user_input == false) continue;

		return true;
	}
		
	for (int i = 0; i < ma_AreaInfo.size(); ++i)
	{
		if (ma_AreaInfo[i].use_user_input == false) continue;

		return true;
	}

	if (ma_UserRebar.size() > 0) return true;

	return false;
}

bool MSCompoG::UsedMaterial(long materialID)
{
	if(GetMaterial()->m_ID == materialID)
		return true;

	map<long, MSAddRebarBase*>::iterator it;
	for (it = mm_AddRebar.begin(); it != mm_AddRebar.end(); it++)
	{
		long nMaterialID = it->second->GetMaterialID();
		if (nMaterialID == materialID)
			return true;
	}

	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return true;
	CString materialName = _T("");

	for (int i = 0; i < ma_VolumeInfo.size(); ++i)
	{
		if (ma_VolumeInfo[i].isUsedStoryMaterial == true) continue;

		if (materialName == _T(""))
		{
			MSMaterial* material = pBldg->GetMaterial(materialID);
			if(material == nullptr) continue;
			materialName = material->GetName();
		}

		if (ma_VolumeInfo[i].materialName == materialName)
			return true;
	}

	return false;
}

void MSCompoG::SubMemberMaterialValidCheck()
{
	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return;

	long defMaterialID = -1;

	map<long, MSAddRebarBase*>::iterator it;
	for (it = mm_AddRebar.begin(); it != mm_AddRebar.end(); it++)
	{
		if (it->second->GetIsUsedOwnerMaterial() == true) continue;;

		long nMaterialID = it->second->GetMaterialID();
		if (pBldg->GetMaterial(nMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		it->second->SetMaterialID(defMaterialID);
	}

	for (int i = 0; i < m_VolumeAreaEMS.ma_HaunchMatlID.size(); ++i)
	{
		long nHaunchMaterialID = m_VolumeAreaEMS.ma_HaunchMatlID[i];
		if (pBldg->GetMaterial(nHaunchMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		m_VolumeAreaEMS.ma_HaunchMatlID[i] = defMaterialID;
	}
		
	for (int i = 0; i < m_VolumeAreaEMS.ma_BuiltUpBemaMatlID.size(); ++i)
	{
		long nBuiltUpBeamMaterialID = m_VolumeAreaEMS.ma_BuiltUpBemaMatlID[i];
		if (pBldg->GetMaterial(nBuiltUpBeamMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		m_VolumeAreaEMS.ma_BuiltUpBemaMatlID[i] = defMaterialID;
	}
}

bool MSCompoG::IsLowWallTopQuantitySeperate()
{
	MSHandRailG* lowWall = dynamic_cast<MSHandRailG*>(this);
	if (lowWall == nullptr) return false;
	return lowWall->GetTopQuantitySeparation();
}

int MSCompoG::GetBuildingInfoID()
{
	if (IsSubMember())
	{
		MSCompoG* pOwnerCompoG = dynamic_cast<MSCompoG*>(GetOwnerCompoG());
		if (pOwnerCompoG != nullptr)
		{
			m_nBuildingInfoID = pOwnerCompoG->GetBuildingInfoID();
			return m_nBuildingInfoID;
		}
	}

	if (m_nBuildingInfoID == -1)
	{
		MSBaseBuilding* pBldg = GetBuilding();
		if (pBldg == nullptr) return m_nBuildingInfoID;

		m_nBuildingInfoID = pBldg->GetBuildingInfoID();
	}

	return m_nBuildingInfoID;
}

void MSCompoG::SetBuildingInfoID(int buildingInfoID)
{
	// 서브 부재는 GetOwnerCompoG()의 빌딩정보를 사용한다.
	if (IsSubMember())
		return;

	m_nBuildingInfoID = buildingInfoID;
}

MSBuildingInfo* MSCompoG::GetBuildingInfo()
{
	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return nullptr;

	int buildingInfoID = GetBuildingInfoID();
	MSBuildingInfo* buildingInfo = pBldg->GetBuildingInfoManager()->GetBuildingInfoByID(buildingInfoID);
	if (buildingInfo == nullptr)
	{
		buildingInfo = pBldg->GetBuildingInfoManager()->GetDefaultBuildingInfo();
		m_nBuildingInfoID = buildingInfo->m_ID;
	}

	return buildingInfo;
}

CString MSCompoG::GetBuildingInfoClassification()
{
	MSBuildingInfo* buildingInfo = GetBuildingInfo();
	if (buildingInfo == nullptr) return _T("");

	return buildingInfo->GetBuildingInfoClassification();
}

CString MSCompoG::GetBuildingInfoType()
{
	MSBuildingInfo* buildingInfo = GetBuildingInfo();
	if (buildingInfo == nullptr) return _T("");

	return buildingInfo->GetBuildingInfoType();
}

CString MSCompoG::GetBuildingInfoName()
{
	MSBuildingInfo* buildingInfo = GetBuildingInfo();
	if (buildingInfo == nullptr) return _T("");

	return buildingInfo->GetName();		
}

double MSCompoG::GetBuildingInfoOverGrossFloorArea()
{
	MSBuildingInfo* buildingInfo = GetBuildingInfo();
	if (buildingInfo == nullptr) return 0;

	return buildingInfo->GetOverGrossFloor();
}

double MSCompoG::GetBuildingInfoUnderGrossFloorArea()
{
	MSBuildingInfo* buildingInfo = GetBuildingInfo();
	if (buildingInfo == nullptr) return 0;

	return buildingInfo->GetUnderGrossFloor();
}


CString MSCompoG::ToString()
{
	auto str = MSElement::ToString();
	MSFloor* pFloor = GetFloor();
	MSCompoM* pCompoM = GetCompoM();
	if (pFloor != nullptr)
		str.AppendFormat(L"FloorNum: %d, ", GetFloor()->GetFloorNum());
	if (pCompoM != nullptr)
		str.AppendFormat(L"CompoMID: %d, ", GetCompoM()->m_ID);
	return str;
}

long MSCompoG::GetMainCompoGID()
{
	return m_nMainCompoGID;
}

void MSCompoG::SetMainCompoGID(long nMainCompoGID)
{
	m_nMainCompoGID = nMainCompoGID;
}

MSCompoG* MSCompoG::GetMainCompoG()
{
	MSCompoG* pMainCompoG = nullptr;
	MSFloor* pFloor = GetFloor();
	if (pFloor != nullptr)
		pMainCompoG = pFloor->GetCompoG(m_nMainCompoGID);

	return pMainCompoG;
}

vector<long> MSCompoG::GetLinkedCompoGIDArr()
{
	vector<long> LinkedCompoGIDArr;
	MSFloor* pFloor = GetFloor();
	if (pFloor != nullptr)
		return pFloor->GetLinkedCompoGIDArr(m_ID);

	return LinkedCompoGIDArr;
}

vector<MSCompoG*> MSCompoG::GetLinkedCompoGArr()
{
	vector<MSCompoG*> LinkedCompoGs;
	MSFloor* pFloor = GetFloor();
	if (pFloor != nullptr)
	{
		vector<long> LinkedGIDArr = pFloor->GetLinkedCompoGIDArr(m_ID);
		for (int idx = 0; idx < LinkedGIDArr.size(); idx++)
		{
			MSCompoG* pCompoG = pFloor->GetCompoG(LinkedGIDArr[idx]);
			if (pCompoG != nullptr)
				LinkedCompoGs.push_back(pCompoG);
		}
	}

	return LinkedCompoGs;
}

CString MSCompoG::GetClassificationName()
{
	if (mp_CompoM == nullptr)
		return _T("");
	return mp_CompoM->GetClassificationName();
}

MSSteelGradeManager::eSteelGrade MSCompoG::GetSteelGrade(double dDia)
{
	MSMaterial *pMaterial = GetMaterial();
	if(pMaterial)
	{
		MSReinforceBarSetting* pReinforceBarSetting = pMaterial->GetReinforceBarSetting();
		if(pReinforceBarSetting)
		{
			CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
			long DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
			auto Iter = pReinforceBarSetting->mm_IndexToReinforceBar.find(DiaIndex);
			if(Iter != pReinforceBarSetting->mm_IndexToReinforceBar.end())
				return Iter->second->GetSGrade();
		}
	}
	return MSSteelGradeManager::eSteelGrade::NONE;
}

bool MSCompoG::HasRightAngleConnection()
{
	for (int i = 0; i < ma_Connections.size(); ++i)
	{
		MSConnection* pConn = ma_Connections[i];
		if (pConn == nullptr) continue;
		if (pConn->IsRightAngle())
			return true;
	}

	return false;
}

void MSCompoG::SortNCalcTrimOption()
{
	if (IsLinkMember() == false)
		return;

	vector<MSConnection*> startFindConnectionArr;
	vector<MSConnection*> endFindConnectionArr;

	bool hasStartConnPointG = false;
	bool hasEndConnPointG = false;

// 	sort(ma_Connections.begin(), ma_Connections.end(),
// 		[](MSConnection* a, MSConnection* b)->bool {return b->IsRightAngleORMiter(); }
	sort(ma_Connections.begin(), ma_Connections.end(),
		[](MSConnection* a, MSConnection* b)->bool {return b->CompareTo(a); }

	);

	bool hasRightAngleORMiter = false;

	for (int i = 0; i < ma_Connections.size(); ++i)
	{
		MSConnection* pConn = ma_Connections[i];
		if (pConn == nullptr) continue;
		MSCompoG* pPartner = pConn->GetPartner(this);

		CON_POS conPos = pConn->GetLinkMemberConPos(this);
		CON_POS partnerConPos = pConn->GetLinkMemberConPos(pPartner);

		if ((pConn->GetFirstTrimOption() == MSConnection::TO_MITER || pConn->GetSecondTrimOption() == MSConnection::TO_MITER)
			|| pConn->IsRightAngle())
			hasRightAngleORMiter = true;

		vector<MSConnection*> partnerConnArr = pPartner->GetConnectionByPos(partnerConPos);

		for (int j = 0; j < partnerConnArr.size() ; j++)
		{
			MSConnection* pPartnerConn = partnerConnArr[j];
			if(pPartnerConn->GetPartner(pPartner) == this)
				continue;

			if ((pPartnerConn->GetFirstTrimOption() == MSConnection::TO_MITER || pPartnerConn->GetSecondTrimOption() == MSConnection::TO_MITER)
				|| pPartnerConn->IsRightAngle())
			{
				if (conPos == CON_POS::CON_START)
					startFindConnectionArr.push_back(pConn);
				else if (conPos == CON_POS::CON_END)
					endFindConnectionArr.push_back(pConn);
			}
		}

		if (pPartner->IsPointMember())
		{
			if (conPos == CON_POS::CON_START)
				hasStartConnPointG = true;
			else if (conPos == CON_POS::CON_END)
				hasEndConnPointG = true;
		}
	}

	for (int i = 0; i < ma_Connections.size(); ++i)
	{
		MSConnection* pConn = ma_Connections[i];
		if (pConn->m_SetByUser || pConn->IsParallel() || pConn->IsRightAngle() || pConn->HasTrimOption(MSConnection::TO_IGNORE)) continue;
		MSCompoG* pPartner = pConn->GetPartner(this);

		if(GetType() != pPartner->GetType())
			continue;
		if(IsLevelEquals(pPartner) == false) 
			continue;

		CON_POS conPos = pConn->GetLinkMemberConPos(this);
		CON_POS partnerConPos = pConn->GetLinkMemberConPos(pPartner);

		bool isTowEndPos = false;
		if ((conPos == CON_POS::CON_START || conPos == CON_POS::CON_END)
			&& (partnerConPos == CON_POS::CON_START || partnerConPos == CON_POS::CON_END))
			isTowEndPos = true;

		if (isTowEndPos && conPos == CON_POS::CON_START)
		{
			if (startFindConnectionArr.size() == 0)
			{
				if (hasRightAngleORMiter == false)
				{
					if ((pConn->GetFirstTrimOption() == MSConnection::TO_ORIGIN && pConn->GetSecondTrimOption() == MSConnection::TO_ENDCUT)
						|| (pConn->GetFirstTrimOption() == MSConnection::TO_ENDCUT && pConn->GetSecondTrimOption() == MSConnection::TO_ORIGIN))
					{
						pConn->SetDefTrimOption();
						pConn->CalculateLinkMemberTrimOption();
					}
				}
			}
			else
			{
				if (hasStartConnPointG == false)
				{
					if (pConn->GetFirstCompoG() != pPartner)
						pConn->SwitchFirstSecond();

					pConn->SetFirstTrimOption(MSConnection::TO_ORIGIN);
					pConn->SetSecondTrimOption(MSConnection::TO_ENDCUT);
				}
			}
		}
		else if (isTowEndPos && conPos == CON_POS::CON_END)
		{
			if (endFindConnectionArr.size() == 0)
			{
				if (hasRightAngleORMiter == false)
				{
					if ((pConn->GetFirstTrimOption() == MSConnection::TO_ORIGIN && pConn->GetSecondTrimOption() == MSConnection::TO_ENDCUT)
						|| (pConn->GetFirstTrimOption() == MSConnection::TO_ENDCUT && pConn->GetSecondTrimOption() == MSConnection::TO_ORIGIN))
					{
						pConn->SetDefTrimOption();
						pConn->CalculateLinkMemberTrimOption();
					}
				}
			}
			else
			{
				if (hasEndConnPointG == false)
				{
					if (pConn->GetFirstCompoG() != pPartner)
						pConn->SwitchFirstSecond();

					pConn->SetFirstTrimOption(MSConnection::TO_ORIGIN);
					pConn->SetSecondTrimOption(MSConnection::TO_ENDCUT);
				}
			}
		}
		else if (conPos == CON_POS::CON_MID)
		{
			if (partnerConPos == CON_POS::CON_START || partnerConPos == CON_POS::CON_END)
			{
				if (pConn->GetFirstCompoG() != this)
					pConn->SwitchFirstSecond();

				pConn->SetFirstTrimOption(MSConnection::TO_ORIGIN);
				pConn->SetSecondTrimOption(MSConnection::TO_ENDCUT);
			}
		}
		else if (partnerConPos == CON_POS::CON_MID)
		{
			if (conPos == CON_POS::CON_START || conPos == CON_POS::CON_END)
			{
				if (pConn->GetFirstCompoG() != pPartner)
					pConn->SwitchFirstSecond();

				pConn->SetFirstTrimOption(MSConnection::TO_ORIGIN);
				pConn->SetSecondTrimOption(MSConnection::TO_ENDCUT);
			}
		}
	}
}

void MSCompoG::SetByUserInConnections(bool value)
{
	for (int i = 0; i < ma_Connections.size(); ++i)
	{
		MSConnection* pConn = ma_Connections[i];
		if (pConn == nullptr) continue;;
		pConn->m_SetByUser = value;
	}
}
CString MSCompoG::GetSteelGradeName(double dDia, bool bUserSteelGradeS)
{
	MSMaterial *pMaterial = GetMaterial();
	if(pMaterial)
	{
		MSReinforceBarSetting* pReinforceBarSetting = pMaterial->GetReinforceBarSetting();
		if(pReinforceBarSetting)
		{
			CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
			long DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
			auto Iter = pReinforceBarSetting->mm_IndexToReinforceBar.find(DiaIndex);
			if(Iter != pReinforceBarSetting->mm_IndexToReinforceBar.end())
			{
				CString sName = Iter->second->m_Name;
				if(bUserSteelGradeS && MSDeformedBar::EnabledUserSteelGradeS(sName))
					sName += "S";
				return sName;
			}
		}
	}
	return _T("");
}

bool MSCompoG::FixGeometry()
{
	//라인부재 + 포인트 부재 높이 체크
	//끝에 포인트 부재 있으면 빼기
	//보 + 벽 평행하게 만들기
	//벽이 아닌 라인부재를 평행하게 만들기
	//라인부재 + 라인부재 중간 + 끝에 만나는 경우  중간에 만나는 객체를 수정
	bool isFix = false;

	double hitDist = DefHitDist / 2;

	if (IsLinkMember())
	{
		MSLinkMemberG* pLinkG = dynamic_cast<MSLinkMemberG*>(this);
		Type_MSCompoG curGType = GetType();
		shared_ptr<GM2DCurve> cenCurve = pLinkG->GetCenCurve();
		GM2DCurve* pLCurve = pLinkG->mp_LLine;
		GM2DCurve* pRCurve = pLinkG->mp_RLine;
		double dLenTol = 10 / cenCurve->Length();

		vector<MSConnection*> connections = ma_Connections;
		for (int idx = 0; idx < connections.size(); idx++)
			connections[idx]->SetPointMemberArea();

		sort(connections.begin(), connections.end(),
			[](MSConnection* a, MSConnection* b)->bool {return a->GetPointMemberArea() < b->GetPointMemberArea(); }
		);

		for (int idx = 0; idx < connections.size(); idx++)
		{
			//if (isFix) break;

			MSConnection* pConn = connections[idx];
			MSConnection::TRIM_OPTION conOpt = pConn->GetTrimOption(this);
			MSCompoG* pPartnerG = pConn->GetPartner(this);
			Type_MSCompoG partnerGType = pPartnerG->GetType();

			CON_POS curPos = pConn->GetLinkMemberConPos(this);
			CON_POS partnerPos = pConn->GetLinkMemberConPos(pPartnerG);

			if (pPartnerG->IsPointMember())
			{
				if (curPos == CON_POS::CON_START || curPos == CON_POS::CON_END)
				{
					GM2DVector checkVec;
					if (curPos == CON_POS::CON_START)
						checkVec = cenCurve->m_P0;
					else if (curPos == CON_POS::CON_END)
						checkVec = cenCurve->m_P1;

					vector<GM2DPolyline*> polyArr = pConn->GetIntersects();
					if (polyArr.size() > 0)
					{
						GM2DPolyline* poly = polyArr[0];
						if (poly->Contains(&checkVec, DTOL_MODELING) != 1)
						{
							vector<GM2DCurve*> OverLapCurve;
							vector<GM2DVector> crossVecArr = poly->IntersectEx(&*cenCurve, &OverLapCurve);
							int crossVecSize = crossVecArr.size();
							if (crossVecSize > 0)
							{
								if (curPos == CON_POS::CON_START)
								{
									GM2DVector crossVec;
									double crossLoc = 1;
									for (int idx = 0; idx < crossVecSize; idx++)
									{
										GM2DVector curVec = crossVecArr[idx];
										double curLoc = cenCurve->OrthoBaseLocValue(curVec);
										if (curLoc < crossLoc)
										{
											crossLoc = curLoc;
											crossVec = curVec;
										}
									}
									
									if (crossVec.SameVector4DTOL(cenCurve->m_P0, DTOL_GM) == false)
									{
										cenCurve->m_P0 = crossVec;
										pLinkG->SetCenCurve(cenCurve->Copy());
										isFix = true;
									}
								}
								else if (curPos == CON_POS::CON_END)
								{
									GM2DVector crossVec;
									double crossLoc = 0;
									for (int idx = 0; idx < crossVecSize; idx++)
									{
										GM2DVector curVec = crossVecArr[idx];
										double curLoc = cenCurve->OrthoBaseLocValue(curVec);
										if (curLoc > crossLoc)
										{
											crossLoc = curLoc;
											crossVec = curVec;
										}
									}

									if (crossVec.SameVector4DTOL(cenCurve->m_P1, DTOL_GM) == false)
									{
										cenCurve->m_P1 = crossVec;
										pLinkG->SetCenCurve(cenCurve->Copy());
										isFix = true;
									}
								}
							}
						}
					}
				}
			}
			else if (pPartnerG->IsLinkMember())
			{
				MSLinkMemberG* pPartnerLinkG = dynamic_cast<MSLinkMemberG*>(pPartnerG);
				if (pConn->IsParallel())
				{
					if (curGType != Type_MSCompoG::msWallG && partnerGType == Type_MSCompoG::msWallG)
					{
						// 라인부재의 각도 동일하게 맞추기
						// 라인부재의 이동이 필요한 경우 맞추기

						shared_ptr<GM2DCurve> pPartnerCenCurve = pPartnerLinkG->GetCenCurve();
						double curDeg = cenCurve->GetDegree();
						double partnerDeg = pPartnerCenCurve->GetDegree();
						double partnerWidth = pPartnerLinkG->GetWidth();
						double curWidth = GetWidth();

						GM2DCurve* pCheckPartnerCurve = nullptr;
						GM2DCurve* pCheckCurCurve = nullptr;
						double dTransDist = 0;
						if (fabs(curWidth - partnerWidth) < DTOL_GM && cenCurve->IsColinear4DTol(&*pPartnerCenCurve, DTOL_MODELING, DTOL_MODELING))
						{
							pCheckPartnerCurve = &*pPartnerCenCurve;
							pCheckCurCurve = cenCurve->Copy();
							dTransDist = 0;
						}
						else
						{
							GM2DCurve* pCheckPartnerLCurve = nullptr;
							GM2DCurve* pCheckPartnerRCurve = nullptr;
							GM2DVector partnerLCenVec = pPartnerLinkG->mp_LLine->PointAtParam(0.5);
							GM2DVector partnerRCenVec = pPartnerLinkG->mp_RLine->PointAtParam(0.5);

							bool isLColinearPoint = pLCurve->IsColinearPoint(partnerLCenVec, DTOL_MODELING);
							if (isLColinearPoint == true)
								pCheckPartnerLCurve = pPartnerLinkG->mp_LLine;

							if (isLColinearPoint == false)
							{
								isLColinearPoint = pLCurve->IsColinearPoint(partnerRCenVec, DTOL_MODELING);
								if (isLColinearPoint == true)
									pCheckPartnerLCurve = pPartnerLinkG->mp_RLine;
							}

							bool isRColinearPoint = pRCurve->IsColinearPoint(partnerLCenVec, DTOL_MODELING);
							if (isRColinearPoint == true)
								pCheckPartnerRCurve = pPartnerLinkG->mp_LLine;

							if (isRColinearPoint == false)
							{
								isRColinearPoint = pRCurve->IsColinearPoint(partnerRCenVec, DTOL_MODELING);
								if (isRColinearPoint == true)
									pCheckPartnerRCurve = pPartnerLinkG->mp_RLine;
							}

							if (isLColinearPoint)
							{
								pCheckPartnerCurve = pCheckPartnerLCurve;
								pCheckCurCurve = pLCurve->Copy();
								if(cenCurve->WhichSide(pCheckCurCurve->m_P0) == 1)
									dTransDist = -curWidth / 2;
								else
									dTransDist = curWidth / 2;
							}
							else if (isRColinearPoint)
							{
								pCheckPartnerCurve = pCheckPartnerRCurve;
								pCheckCurCurve = pRCurve->Copy();
								if (cenCurve->WhichSide(pCheckCurCurve->m_P0) == 1)
									dTransDist = -curWidth / 2;
								else
									dTransDist = curWidth / 2;
							}
						}

						if (pCheckPartnerCurve != nullptr && pCheckCurCurve != nullptr)
						{
							//GM2DVector cenVec = pCheckCurCurve->PointAtParam(0.5);
							//GM2DVector partnerCenVec = pCheckPartnerCurve->PointAtParam(0.5);
							//if (pCheckPartnerCurve->HasOnLine(cenVec, DTOL_MODELING) || pCheckCurCurve->HasOnLine(partnerCenVec, DTOL_MODELING))
							{
								if (cenCurve->m_ObjType == pPartnerCenCurve->m_ObjType == GT_LINE)
								{
									GM2DVector sVec = pCheckPartnerCurve->OrthoBaseVector(pCheckCurCurve->m_P0);
									GM2DVector eVec = pCheckPartnerCurve->OrthoBaseVector(pCheckCurCurve->m_P1);
									if (sVec.SameVector4DTOL(pCheckPartnerCurve->m_P0, DTOL_MODELING))
										sVec = pCheckPartnerCurve->m_P0;
									if (sVec.SameVector4DTOL(pCheckPartnerCurve->m_P1, DTOL_MODELING))
										sVec = pCheckPartnerCurve->m_P1;
									if (eVec.SameVector4DTOL(pCheckPartnerCurve->m_P0, DTOL_MODELING))
										eVec = pCheckPartnerCurve->m_P0;
									if (eVec.SameVector4DTOL(pCheckPartnerCurve->m_P1, DTOL_MODELING))
										eVec = pCheckPartnerCurve->m_P1;

									pCheckCurCurve->m_P0 = sVec;
									pCheckCurCurve->m_P1 = eVec;

									if (dTransDist != 0)
										pCheckCurCurve->NormalTranslateMe(dTransDist);

									pLinkG->SetCenCurve(pCheckCurCurve);
									isFix = true;
								}
							}
						}
						else // 라인부재가 평행하면서 센터라인 L/R라인 이 평행하지 않는경우 
							 // 살짝 떨어지는 경우가 발생하여 라인부재의 끝을 마춰준다
						{
							double dPartnerCenLen = pPartnerCenCurve->Length();
							double dPartnerTol = hitDist / dPartnerCenLen;
							if (cenCurve->m_ObjType == pPartnerCenCurve->m_ObjType == GT_LINE)
							{
								double sLoc = pPartnerCenCurve->OrthoBaseLocValue(cenCurve->m_P0);
								double eLoc = pPartnerCenCurve->OrthoBaseLocValue(cenCurve->m_P1);

								GM2DVector sVec = cenCurve->m_P0;
								GM2DVector eVec = cenCurve->m_P1;

								if (sLoc < 0.5 && eLoc < 0.5) // 파트너의 앞에 위치
								{
									if (sLoc < eLoc)
									{
										if (eLoc > 0 - dPartnerTol)
											eVec = cenCurve->OrthoBaseVector(pPartnerCenCurve->m_P0);
									}
									else
									{
										if (sLoc > 0 - dPartnerTol)
											sVec = cenCurve->OrthoBaseVector(pPartnerCenCurve->m_P0);
									}
								}
								else if (sLoc > 0.5 && eLoc > 0.5) // 파트너의 뒤에 위치
								{
									if (sLoc < eLoc)
									{
										if (sLoc < 1 + dPartnerTol)
											sVec = cenCurve->OrthoBaseVector(pPartnerCenCurve->m_P1);
									}
									else
									{
										if (eLoc < 1 + dPartnerTol)
											eVec = cenCurve->OrthoBaseVector(pPartnerCenCurve->m_P1);
									}
								}

								GM2DCurve* pCheckCurCurve = cenCurve->Copy();
								pCheckCurCurve->m_P0 = sVec;
								pCheckCurCurve->m_P1 = eVec;
								pLinkG->SetCenCurve(pCheckCurCurve);
								isFix = true;
							}
						}
					}
				}
				else // 평행하지 않은 경우
				{
					if (curPos == CON_POS::CON_START || curPos == CON_POS::CON_END)
					{
						GM2DVector checkVec;
						if (curPos == CON_POS::CON_START)
							checkVec = cenCurve->m_P0;
						else if (curPos == CON_POS::CON_END)
							checkVec = cenCurve->m_P1;

						vector<GM2DPolyline*> polyArr = pConn->GetIntersects();
						if (polyArr.size() > 0)
						{
							GM2DPolyline* poly = polyArr[0];
							if (poly->Contains(&checkVec, DTOL_MODELING) != 1)
							{
								vector<GM2DCurve*> OverLapCurve;
								vector<GM2DVector> crossVecArr = poly->IntersectEx(&*cenCurve, &OverLapCurve);
								int crossVecSize = crossVecArr.size();
								if (crossVecSize > 0)
								{
									if (curPos == CON_POS::CON_START)
									{
										GM2DVector crossVec;
										double crossLoc = 1;
										for (int idx = 0; idx < crossVecSize; idx++)
										{
											GM2DVector curVec = crossVecArr[idx];
											double curLoc = cenCurve->OrthoBaseLocValue(curVec);
											if (curLoc < crossLoc)
											{
												crossLoc = curLoc;
												crossVec = curVec;
											}
										}
										if (crossVec.SameVector4DTOL(cenCurve->m_P0, DTOL_GM) == false)
										{
											cenCurve->m_P0 = crossVec;
											pLinkG->SetCenCurve(cenCurve->Copy());
											isFix = true;
										}
									}
									else if (curPos == CON_POS::CON_END)
									{
										GM2DVector crossVec;
										double crossLoc = 0;
										for (int idx = 0; idx < crossVecSize; idx++)
										{
											GM2DVector curVec = crossVecArr[idx];

											double curLoc = cenCurve->OrthoBaseLocValue(curVec);
											if (curLoc > crossLoc)
											{
												crossLoc = curLoc;
												crossVec = curVec;
											}
										}
										if (crossVec.SameVector4DTOL(cenCurve->m_P1, DTOL_GM) == false)
										{
											cenCurve->m_P1 = crossVec;
											pLinkG->SetCenCurve(cenCurve->Copy());
											isFix = true;
										}
									}
								}
							}
						}
					}
					
				}
			}
		}
	}
	return isFix;
}

CString MSCompoG::GetTypeAttribute()
{
	if (IsPC())
		return _T("PC");
	
	Type_MSCompoG curType = GetType();
	if (curType == Type_MSCompoG::msBeamG)
	{
		if(IsSRC())
			return _T("SRCBeam");
		else if(IsSteel())
			return _T("Steel_Beam");
		return _T("Beam");
	}
	else if (curType == Type_MSCompoG::msColumnG)
	{
		if (IsSRC())
			return _T("SRCColumn");
		else if (IsSteel())
			return _T("Steel_Column");
		return _T("Column");
	}
	else if (curType == Type_MSCompoG::msWallG)
	{
		if (IsRetainingWall())
			return _T("RWall");
		return _T("Wall");
	}
	else if (curType == Type_MSCompoG::msHandRailG)
	{
		return _T("LowWall");
	}
	else if (curType == Type_MSCompoG::msSlabG)
	{
		return _T("Slab");
	}
	else if (curType == Type_MSCompoG::msPitG)
	{
		return _T("Pit");
	}
	else if (curType == Type_MSCompoG::msStairG)
	{
		return _T("Stair");
	}
	else if (curType == Type_MSCompoG::msWallFootG)
	{
		return _T("WallFoot");
	}
	else if (curType == Type_MSCompoG::msWallFootG)
	{
		return _T("WallFoot");
	}
	else if (curType == Type_MSCompoG::msIsoFootG)
	{
		return _T("IsoFoot");
	}

	return _T("Other");
}

bool MSCompoG::GetIsUserFloor()
{
	return m_IsUserFloor;
}

void MSCompoG::SetIsUserFloor(bool value)
{
	m_IsUserFloor = value;
}

int MSCompoG::GetUserFloorID()
{
	if (m_IsUserFloor == false)
	{
		MSFloor* pFloor = GetDefUserFloor();
		if(pFloor != nullptr)
			return pFloor->m_ID;
		return 0;
	}

	return m_nUserFloorID;
}

void MSCompoG::SetUserFloorID(int nFloorID)
{
	m_nUserFloorID = nFloorID;
}

MSFloor* MSCompoG::GetUserFloor()
{
	if (m_IsUserFloor == false)
		return GetDefUserFloor();

	MSBaseBuilding* pBldg = GetBuilding();
	MSFloor* pUserFloor = pBldg->GetFloorByID(m_nUserFloorID);
	if (pUserFloor == nullptr)
	{
		m_nUserFloorID = 0;
		return GetDefUserFloor();
	}
	return pUserFloor;
}

void MSCompoG::SetUserFloor(MSFloor* pUserFloor)
{
	if (pUserFloor == nullptr)
		m_nUserFloorID = 0;
	else
		m_nUserFloorID = pUserFloor->m_ID;
}

MSFloor* MSCompoG::GetDefUserFloor()
{
	return GetFloor();
}

bool MSCompoG::IsTransfer()
{
	MDMember* pMDMember = GetRebarM();
	if (pMDMember != nullptr)
		return pMDMember->IsTransfer();

	return false;
}

bool MSCompoG::IsDeck()
{
	MDMember* pMDMember = GetRebarM();
	if (pMDMember != nullptr)
		return pMDMember->IsDeck();

	return false;
}