#include "StdAfx.h"
#include "MSIsoFootG.h"
#include "MSContainer.h"
#include "MSRebarBuilding.h"
#include "MSRcIsoFootM.h"
#include "MSFloor.h"
#include "MSColumnG.h"
#include "MSRcLineMemberM.h"
#include "GMLib/MSVersionInfo.h"
#include "MSSlabG.h"
#include "MSSubHaunchG.h"
#include "MSConnection.h"
#include "MSCompoGUtil.h"

#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSIsoFootG, MSPointMemberG, VERSIONABLE_SCHEMA | 1)

MSIsoFootG::MSIsoFootG()
{
	Init(true);
}

MSIsoFootG::~MSIsoFootG(void)
{
	DeleteHaunchs();
}

void MSIsoFootG::Dump(EFS::EFS_Buffer& buffer)
{
	MSPointMemberG::Dump(buffer);
	buffer << m_TopSpacerID << m_BotSpacerID;
	
	int count = ma_Haunches .size();
	buffer << count;
	for (int i = 0; i < count; ++i)
		ma_Haunches [i]->Dump(buffer);
}

void MSIsoFootG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSPointMemberG::Recovery(buffer);
	buffer >> m_TopSpacerID >> m_BotSpacerID;

	int count = 0;
	buffer >> count;
	for (int i = 0; i < ma_Haunches .size(); ++i)
		delete ma_Haunches [i];
	ma_Haunches .clear();
	for (int i = 0; i < count; ++i)
	{
		MSSubHaunchG* haunch = new MSSubHaunchG();
		haunch->mp_Owner = mp_Owner;
		haunch->Recovery(buffer);
		haunch->mp_OwnerCompoG = this;
		ma_Haunches .push_back(haunch);
	}
}

void MSIsoFootG::Serialize(CArchive &ar)
{
	MSPointMemberG::Serialize(ar);
	bool bEachCompoM = false;
	if(ar.IsStoring())
	{
		if(mp_CompoM)
		{
			if(mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if(bEachCompoM == true)
				Serialize_EachCompoM(ar);
		}

		ar << m_TopSpacerID;
		ar << m_BotSpacerID;

		int count = ma_Haunches .size();
		ar << count;
		for (int i = 0; i < count; ++i)
			ma_Haunches [i]->Serialize(ar);
	}
	else
	{
		ar >> bEachCompoM;
		if(bEachCompoM)
			Serialize_EachCompoM(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20141103)
		{
			ar >> m_TopSpacerID;
			ar >> m_BotSpacerID;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160627)
		{
			DeleteHaunchs();
			int count = 0;
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				MSSubHaunchG* haunch = new MSSubHaunchG();
				haunch->mp_Owner = mp_Owner;
				haunch->Serialize(ar);
				haunch->mp_OwnerCompoG = this;
				ma_Haunches .push_back(haunch);
			}
		}

		if (MSVersionInfo::GetCurrentVersion() < 20181127)
		{
			SubMemberMaterialValidCheck();
		}
	}
	long nJointID = 0;
	
}

void MSIsoFootG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
        long nType = mp_CompoM->m_nType;
		ar << nType;
		mp_CompoM->Serialize(ar);
	}
	else
	{
		if(MSVersionInfo::GetCurrentVersion() < 20220321)
		{
			MSBaseBuilding* pBldg = GetBuilding();
			long MType = 0;
			ar >> MType;

			MSRcIsoFootM* pFootM = (MSRcIsoFootM*)pBldg->CreateNewCompoM(MType);
			pFootM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pFootM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pFootM->m_Name;

			double dWidth = 0, dDepth = 0, dThick;
			ar >> dWidth;
			ar >> dDepth;
			ar >> dThick;

			pFootM->SetWidth(dWidth);
			pFootM->SetDepth(dDepth);
			pFootM->SetThick(dThick);

			pFootM->mp_Owner = pBldg;
			mp_CompoM = pFootM;
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

MSObject* MSIsoFootG::Copy( bool bNewID /*= true*/ )
{
	MSIsoFootG* pNewObj;

	pNewObj = new MSIsoFootG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSIsoFootG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msIsoFootG) { ASSERT(0); return; }

	MSIsoFootG* pSource = (MSIsoFootG*)pSource_in;
	m_TopSpacerID = pSource->m_TopSpacerID;
	m_BotSpacerID = pSource->m_BotSpacerID;
	
	for (int i = 0; i < ma_Haunches .size(); ++i)
		delete ma_Haunches [i];
	ma_Haunches .clear();
	for (int i = 0; i < pSource->ma_Haunches .size(); ++i)
	{
		MSSubHaunchG* haunch = new MSSubHaunchG();
		haunch->mp_Owner = mp_Owner;
		haunch->CopyFromMe(pSource->ma_Haunches [i], bNewID);
		haunch->mp_OwnerCompoG = this;
		ma_Haunches .push_back(haunch);
	}

	MSPointMemberG::CopyFromMe(pSource_in, bNewID);
}

void MSIsoFootG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSPointMemberG::Init(bInConstructor);

	m_TopSpacerID = 0;
	m_BotSpacerID = 0;
}

bool MSIsoFootG::CheckData()
{
	if (MSElement::CheckData() == false)
		return false;

	// 부재 중첩 체크 (각도 돌린 경우는 여기서 체크해야만 함)
	MSFloor* floor = GetFloor();
	if (floor != NULL && floor->CheckCompoGOverlap_WithUpDownFloor(this, this->m_ID))
		return true;
		//return false;

	// Check add...

	return true;
}

bool MSIsoFootG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	if(bCheckLevelZone)
	{
		if(GetBigVal)
		{
			BottomLevel = GetStartLevel();
			TopLevel = GetEndLevel();
		}
		else
		{
			BottomLevel = GetStartLevel(true);
			TopLevel = GetEndLevel(false);
		}
	}
	else
	{
		double dThick = 0;
		if(mp_CompoM != NULL)
			dThick = mp_CompoM->GetThick();
		BottomLevel = GetNextSlabLevel() - dThick + m_SOffsetZ;
		
		TopLevel = GetNextSlabLevel() + m_SOffsetZ;
		
	}
	
	return true;
}

double MSIsoFootG::GetEndLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	return GetNextSlabLevel()+GetRelativeEndLevel(isMax);

}
double MSIsoFootG::GetRelativeEndLevel(bool isMax)
{

	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	return pFloor->GetLevel4LevelZone(ma_Vec[0]->m_X, ma_Vec[0]->m_Y, isMax)+ m_SOffsetZ;
}
double MSIsoFootG::GetStartLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	double dThick = 0;
	if(mp_CompoM != NULL)
		dThick = mp_CompoM->GetThick();
	return GetNextSlabLevel()+GetRelativeStartLevel(isMax) - dThick;
}
double MSIsoFootG::GetRelativeStartLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	return pFloor->GetLevel4LevelZone(ma_Vec[0]->m_X, ma_Vec[0]->m_Y, isMax)+m_SOffsetZ;
}

MSColumnG* MSIsoFootG::GetConnectedColumnG()
{
	MSFloor* pFloor = GetFloor();
	if(pFloor == NULL) 
		return NULL;
	MSFloor* pUpperFloor = pFloor->GetUpperFloor();
	if(pUpperFloor == NULL)
		return NULL;
	
	int Flag_VM = -1;
	vector<MSCompoG*> ColumnArr = FindConnectMember(FindType_MSElement::msColumnG, 1);
	for(long i = 0 ; i < ColumnArr.size() ; i++)
		return (MSColumnG*)ColumnArr[i];

	return NULL;
}

bool MSIsoFootG::IsDropPanel()
{
	MSRcIsoFootM* compoM = dynamic_cast<MSRcIsoFootM*>(mp_CompoM);
	if (compoM != NULL && compoM->IsDropPanel())
		return true;
	else
		return false;
}

void MSIsoFootG::Translate(GM2DVector &pDelta)
{
	MSPointMemberG::Translate(pDelta);
	for (int iHaunch = 0; iHaunch < ma_Haunches .size(); iHaunch++)
		ma_Haunches [iHaunch]->Translate(pDelta);
}

void MSIsoFootG::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSPointMemberG::MirrorByAxis(pAxis);
	for (int iHaunch = 0; iHaunch < ma_Haunches .size(); iHaunch++)
		ma_Haunches [iHaunch]->MirrorByAxis(pAxis);
}

void MSIsoFootG::RotateZ(double theta, GM2DVector *pCenter)
{
	MSPointMemberG::RotateZ(theta, pCenter);
	for (int iHaunch = 0; iHaunch < ma_Haunches .size(); iHaunch++)
		ma_Haunches [iHaunch]->RotateZ(theta, pCenter);
}

void MSIsoFootG::SetRotation(double dDeg)
{
	double dRad = dDeg*Deg2Rad - m_dAxisRotation*Deg2Rad;
	// RotateZ 사용하면 m_dAxisRotation 값이 지저분해진다.
	MSPointMemberG::SetRotation(dDeg);

	GM2DVector BaseVec = *GetSVec();
	for (int iHaunch = 0; iHaunch < ma_Haunches .size(); iHaunch++)
		ma_Haunches [iHaunch]->RotateZ(dRad, &BaseVec);
}

void MSIsoFootG::ModifyObject(bool isDoit)
{
	MSCompoG::ModifyObject(isDoit);
	TrimHaunches();
}

bool MSIsoFootG::HasSpacer()
{
	if (m_TopSpacerID != 0 || m_BotSpacerID != 0)
		return true;
	return false;
}

void MSIsoFootG::DeleteHaunchs()
{
	for (int iHaunch = 0; iHaunch < ma_Haunches .size(); iHaunch++)
		delete ma_Haunches [iHaunch];

	ma_Haunches .clear();
}

bool MSIsoFootG::Remove(MSSubHaunchG* pSubHaunchG)
{
	int findIdx = -1;
	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
	{
		if (ma_Haunches[iHaunch] == pSubHaunchG)
			findIdx = iHaunch;
	}

	if (findIdx != -1)
	{
		ma_Haunches[findIdx]->RemoveConnections();
		ma_Haunches.erase(ma_Haunches.begin() + findIdx);
		return true;
	}

	return false;
}

bool MSIsoFootG::RemoveSubMember(MSElement* element)
{
	bool rVal = false;
	switch (element->GetType())
	{
	case MSElement::msSubHaunchG:
		rVal = Remove((MSSubHaunchG*)element);
		if (rVal == true)
			RegenHaunch();
		return rVal;
	}
	return MSCompoG::RemoveSubMember(element);
}

void MSIsoFootG::AddHaunchGs(vector<MSSubHaunchG*> HaunchGArr)
{
	for (int iHaunchG = 0; iHaunchG < HaunchGArr.size(); iHaunchG++)
	{
		MSSubHaunchG* pHaunchG = HaunchGArr[iHaunchG];
		pHaunchG->GetID();
		ma_Haunches.push_back(pHaunchG);
	}
}

void MSIsoFootG::AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	AddHaunchGs(MSCompoGUtil::MakeHaunchG(this, x, y, x2, y2, haunchAngle, haunchY, haunchY_Top, bTopAuto));
	TrimHaunches();
}

void MSIsoFootG::AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	DeleteHaunchs();
	AddHaunchGs(MSCompoGUtil::MakeHaunchGAllProfile(this, haunchAngle, haunchY, haunchY_Top, bTopAuto));
	TrimHaunches();
}

void MSIsoFootG::TrimHaunches()
{
	MSFloor* pFloor = GetFloor();
	MSCompoGUtil::TrimHaunches(ma_Haunches, pFloor);
}

void MSIsoFootG::RegenHaunch()
{
	for (int i = 0; i < ma_Haunches .size(); ++i)
		ma_Haunches [i]->SetLines();
	TrimHaunches();
}

void MSIsoFootG::RecoverPointerAfterSerialize(MSFloor* floor)
{
	MSCompoG::RecoverPointerAfterSerialize(floor);
	MSIsoFootG* pIsoFootG = this;
	vector<MSCompoG*> sub_list;
	for (int i = 0; i < pIsoFootG->ma_Haunches .size(); ++i)
		sub_list.push_back((MSCompoG*)pIsoFootG->ma_Haunches [i]);
	for (int i = 0; i < sub_list.size(); ++i)
	{
		for (int k = 0; k < sub_list[i]->ma_Connections.size(); ++k)
			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	//BH-1729
	for (int i = 0; i < ma_Haunches .size(); i++)
		ma_Haunches [i]->CheckValidConnections();
}

void MSIsoFootG::RecoverPointerAfterSerialize_SubMembers()
{
	MSIsoFootG* pIsoFootG = (MSIsoFootG*)this;

	vector<MSCompoG*> sub_list;
	for (int i = 0; i < pIsoFootG->ma_Haunches .size(); ++i)
		sub_list.push_back((MSCompoG*)pIsoFootG->ma_Haunches [i]);
	for (int i = 0; i < sub_list.size(); ++i)
	{
		for (int k = 0; k < sub_list[i]->ma_Connections.size(); ++k)
			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	for (int i = 0; i < sub_list.size(); ++i)
		sub_list[i]->ResetGetID();

	//BH-1729
	for (int i = 0; i < ma_Haunches .size(); i++)
		ma_Haunches [i]->CheckValidConnections();
}

long MSIsoFootG::ResetGetID()
{
	MSCompoG::ResetGetID();
	for (int i = 0; i < ma_Haunches .size(); i++)
		ma_Haunches [i]->ResetGetID();
	return m_ID;
}

bool MSIsoFootG::HasHaunch()
{
	if (ma_Haunches .size() > 0)
		return true;
	return false;
}

bool MSIsoFootG::HasSubMember()
{
	if (ma_Haunches .size() > 0)	return true;
	return MSPointMemberG::HasSubMember();
}

vector<MSElement*> MSIsoFootG::GetSubMembers()
{
	vector<MSElement*> subList;
	subList = MSPointMemberG::GetSubMembers();
	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		subList.push_back(ma_Haunches[i]);
	}
	return subList;
}

void MSIsoFootG::DeleteSubMembers()
{
	DeleteHaunchs();
	MSPointMemberG::DeleteSubMembers();
}

bool MSIsoFootG::UsedMaterial(long materialID)
{
	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		long nHaunchMaterialID = ma_Haunches[i]->GetHaunchMaterialID();
		if (nHaunchMaterialID == materialID)
			return true;
	}

	return MSCompoG::UsedMaterial(materialID);
}

void MSIsoFootG::SubMemberMaterialValidCheck()
{
	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return;

	long defMaterialID = -1;

	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		if (ma_Haunches[i]->GetIsUsedStoryMaterial() == true) continue;

		long nHaunchMaterialID = ma_Haunches[i]->GetHaunchMaterialID();
		if (pBldg->GetMaterial(nHaunchMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		ma_Haunches[i]->SetHaunchMaterialID(defMaterialID);
	}
}