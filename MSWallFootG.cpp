#include "StdAfx.h"
#include "MSWallFootG.h"
#include "MSRebarBuilding.h"
#include "MSRcLineMemberM.h"
#include "GMLib/MSVersionInfo.h"
#include "MSMemberLevelDif.h"
#include "MSFloor.h"
#include "MSSubHaunchG.h"
#include "MSConnection.h"
#include "MSCompoGUtil.h"

#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSWallFootG, MSLineMemberG, VERSIONABLE_SCHEMA | 1)
MSWallFootG::MSWallFootG()
{
	Init(true);
}

MSWallFootG::~MSWallFootG(void)
{
	DeleteHaunchs();
}

void MSWallFootG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Dump(buffer);
	buffer << m_TopSpacerID << m_BotSpacerID;
	
	int count = ma_Haunches.size();
	buffer << count;
	for (int i = 0; i < count; ++i)
		ma_Haunches[i]->Dump(buffer);
}

void MSWallFootG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Recovery(buffer);
	buffer >> m_TopSpacerID >> m_BotSpacerID;

	int count = 0;
	buffer >> count;
	for (int i = 0; i < ma_Haunches.size(); ++i)
		delete ma_Haunches[i];
	ma_Haunches.clear();
	for (int i = 0; i < count; ++i)
	{
		MSSubHaunchG* haunch = new MSSubHaunchG();
		haunch->mp_Owner = mp_Owner;
		haunch->Recovery(buffer);
		haunch->mp_OwnerCompoG = this;
		ma_Haunches.push_back(haunch);
	}
}

void MSWallFootG::Serialize(CArchive &ar)
{
	MSLineMemberG::Serialize(ar);
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

		int count = ma_Haunches.size();
		ar << count;
		for (int i = 0; i < count; ++i)
			ma_Haunches[i]->Serialize(ar);
	}
	else
	{
		ar >> bEachCompoM;
		if(bEachCompoM)
			Serialize_EachCompoM(ar);

		if (MSVersionInfo::GetCurrentVersion() <= 20140807)
		{
			m_EOffsetZ = m_SOffsetZ;
		}
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
				ma_Haunches.push_back(haunch);
			}
		}

		if (MSVersionInfo::GetCurrentVersion() < 20181127)
		{
			SubMemberMaterialValidCheck();
		}
	}
}

void MSWallFootG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
		MSRcLineMemberM* pFaceM = (MSRcLineMemberM*)mp_CompoM;
		long MType = pFaceM->m_nType;

		ar << MType;
		mp_CompoM->Serialize(ar);
		//ar << pFaceM->GetMaterial()->m_ID;
		//ar << pFaceM->m_Name;
		//ar << pFaceM->GetThick();
	}
	else
	{
		MSBaseBuilding* pBldg = GetBuilding();
		if (MSVersionInfo::GetCurrentVersion() < 20220321)
		{
			long MType = 0;
			ar >> MType;

			MSRcLineMemberM* pLineM = (MSRcLineMemberM*)pBldg->CreateNewCompoM(MType);
			pLineM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pLineM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pLineM->m_Name;

			double dWidth = 0, dDepth = 0;
			ar >> dWidth;
			ar >> dDepth;

			pLineM->SetWidth(dWidth);
			pLineM->SetDepth(dDepth);

			pLineM->mp_Owner = pBldg;
			mp_CompoM = pLineM;
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


void MSWallFootG::Serialize_EachCompoM_Formwork(CArchive& ar)
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
		if (MSVersionInfo::GetCurrentVersion() < 20240425)
		{
			long MType = 0;
			ar >> MType;

			MSRcLineMemberM* pLineM = (MSRcLineMemberM*)pBldg->CreateNewCompoM(MType);
			pLineM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pLineM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pLineM->m_Name;

			double dWidth = 0, dDepth = 0;
			ar >> dWidth;
			ar >> dDepth;

			pLineM->SetWidth(dWidth);
			pLineM->SetDepth(dDepth);

			pLineM->mp_Owner = pBldg;
			mp_CompoM = pLineM;
		}
		else
		{
			long MType = 0;
			ar >> MType;
			mp_CompoM = pBldg->CreateNewCompoM(MType);
			mp_CompoM->Serialize(ar);
		}
	}
}

MSObject* MSWallFootG::Copy( bool bNewID /*= true*/ )
{
	MSWallFootG* pNewObj;

	pNewObj = new MSWallFootG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSWallFootG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msWallFootG) { ASSERT(0); return; }
	MSLineMemberG::CopyFromMe(pSource_in, bNewID);

	MSWallFootG* pSource = (MSWallFootG*)pSource_in;
	m_TopSpacerID = pSource->m_TopSpacerID;
	m_BotSpacerID = pSource->m_BotSpacerID;

	for (int i = 0; i < ma_Haunches.size(); ++i)
		delete ma_Haunches[i];
	ma_Haunches.clear();
	for (int i = 0; i < pSource->ma_Haunches.size(); ++i)
	{
		MSSubHaunchG* haunch = new MSSubHaunchG();
		haunch->mp_Owner = mp_Owner;
		haunch->CopyFromMe(pSource->ma_Haunches[i], bNewID);
		haunch->mp_OwnerCompoG = this;
		ma_Haunches.push_back(haunch);
	}
}

void MSWallFootG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSLineMemberG::Init(bInConstructor);
	m_TopSpacerID = 0;
	m_BotSpacerID = 0;
}

bool MSWallFootG::HasSpacer()
{
	if (m_TopSpacerID != 0 || m_BotSpacerID != 0)
		return true;
	return false;
}

void MSWallFootG::DeleteHaunchs()
{
	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
		delete ma_Haunches[iHaunch];

	ma_Haunches.clear();
}

bool MSWallFootG::Remove(MSSubHaunchG* pSubHaunchG)
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

bool MSWallFootG::RemoveSubMember(MSElement* element)
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

void MSWallFootG::AddHaunchGs(vector<MSSubHaunchG*> HaunchGArr)
{
	for (int iHaunchG = 0; iHaunchG < HaunchGArr.size(); iHaunchG++)
	{
		MSSubHaunchG* pHaunchG = HaunchGArr[iHaunchG];
		pHaunchG->GetID();
		ma_Haunches.push_back(pHaunchG);
	}
}

void MSWallFootG::AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	AddHaunchGs(MSCompoGUtil::MakeHaunchG(this, x, y, x2, y2, haunchAngle, haunchY, haunchY_Top, bTopAuto));
	TrimHaunches();
}

void MSWallFootG::AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	DeleteHaunchs();
	AddHaunchGs(MSCompoGUtil::MakeHaunchGAllProfile(this, haunchAngle, haunchY, haunchY_Top, bTopAuto));
	TrimHaunches();
}

vector<MSCompoG*> MSWallFootG::GetConnectionHaunchs()
{
	CheckValidConnections();

	vector<MSCompoG*> HaunchArr;
	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
		HaunchArr.push_back(ma_Haunches[iHaunch]);

	GM2DPolyline* pPoly = GetTrimmedBoundary();
	vector<MSConnection*> SConn = GetConnectionByPos(CON_ALL);
	for (int iConn = 0; iConn < SConn.size(); iConn++)
	{
		MSConnection* pConn = SConn[iConn];
		MSCompoG* pPartner = pConn->GetPartner(this);
		if(pPartner == nullptr)
			continue;
		if (pPartner->GetType() == MSElement::msWallFootG)
		{
			MSWallFootG* pUrWallFootG = (MSWallFootG*)pPartner;
			for (int iUrHaunch = 0; iUrHaunch < pUrWallFootG->ma_Haunches.size(); iUrHaunch++)
			{
				MSSubHaunchG* pUrHaunch = pUrWallFootG->ma_Haunches[iUrHaunch];
				GM2DPolyline* pUrPoly = pUrHaunch->GetTrimmedBoundary();
				if (pPoly->OverlapEx(*pUrPoly))
					HaunchArr.push_back(pUrHaunch);
			}
		}
	}
	return HaunchArr;
}

void MSWallFootG::TrimHaunches()
{
	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
		ma_Haunches[iHaunch]->RemoveConnections();

	vector<MSCompoG*> HaunchArr = GetConnectionHaunchs();
	vector<MSSubHaunchG*> CurHaunchArr;
	for (int i = 0; i < HaunchArr.size(); i++)
		CurHaunchArr.push_back((MSSubHaunchG*)HaunchArr[i]);
	MSFloor* pFloor = GetFloor();
	MSCompoGUtil::TrimHaunches(CurHaunchArr, pFloor);
}

void MSWallFootG::RegenHaunch()
{
	for (int i = 0; i < ma_Haunches.size(); ++i)
		ma_Haunches[i]->SetLines();
	TrimHaunches();
}

void MSWallFootG::Translate(GM2DVector &pDelta)
{
	MSLinkMemberG::Translate(pDelta);
	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
	{
		MSSubHaunchG* pHaunch = ma_Haunches[iHaunch];
		pHaunch->Translate(pDelta);
	}
}

void MSWallFootG::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSLinkMemberG::MirrorByAxis(pAxis);
	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
	{
		MSSubHaunchG* pHaunch = ma_Haunches[iHaunch];
		pHaunch->MirrorByAxis(pAxis);
	}
}

void MSWallFootG::RotateZ(double theta, GM2DVector *pCenter)
{
	MSLinkMemberG::RotateZ(theta, pCenter);
	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
	{
		MSSubHaunchG* pHaunch = ma_Haunches[iHaunch];
		pHaunch->RotateZ(theta, pCenter);
	}
}

void MSWallFootG::RecoverPointerAfterSerialize(MSFloor* floor)
{
	MSCompoG::RecoverPointerAfterSerialize(floor);
	MSWallFootG* pIsoFootG = this;
	vector<MSCompoG*> sub_list = GetConnectionHaunchs();
	for (int i = 0; i < sub_list.size(); ++i)
	{
		for (int k = 0; k < sub_list[i]->ma_Connections.size(); ++k)
			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	//BH-1729
	for (int i = 0; i < ma_Haunches.size(); i++)
		ma_Haunches[i]->CheckValidConnections();
}

void MSWallFootG::RecoverPointerAfterSerialize_SubMembers()
{
	MSWallFootG* pIsoFootG = (MSWallFootG*)this;
	vector<MSCompoG*> sub_list = GetConnectionHaunchs();
	for (int i = 0; i < sub_list.size(); ++i)
	{
		for (int k = 0; k < sub_list[i]->ma_Connections.size(); ++k)
			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	for (int i = 0; i < sub_list.size(); ++i)
		sub_list[i]->ResetGetID();
	
	//BH-1729
	for (int i = 0; i < ma_Haunches.size(); i++)
		ma_Haunches[i]->CheckValidConnections();
}

void MSWallFootG::SetCenCurve(GM2DCurve* pCurve)
{
	bool isSameCurve = false;
	shared_ptr<GM2DCurve> OldCenCurve = GetCenCurve();
	if (pCurve != nullptr)
	{
		GM2DVector MidVec = pCurve->PointAtParam(0.5);
		if (OldCenCurve->m_P0.SameVector4DTOL(pCurve->m_P0) &&
			OldCenCurve->m_P1.SameVector4DTOL(pCurve->m_P1) &&
			OldCenCurve->PointAtParam(0.5).SameVector4DTOL(pCurve->PointAtParam(0.5)))
			isSameCurve = true;
	}
	MSLinkMemberG::SetCenCurve(pCurve);
	if(isSameCurve == false)
		DeleteHaunchs();
}

void MSWallFootG::UpdateBoundaryVisibilities(long nUpdateConnIdx)
{
	MSLinkMemberG::UpdateBoundaryVisibilities(nUpdateConnIdx);
	RegenHaunch();
}

long MSWallFootG::ResetGetID()
{
	MSCompoG::ResetGetID();
	for (int i = 0; i < ma_Haunches.size(); i++)
		ma_Haunches[i]->ResetGetID();
	return m_ID;
}

bool MSWallFootG::HasHaunch()
{
	if (ma_Haunches.size() > 0)
		return true;
	return false;
}

bool MSWallFootG::HasSubMember()
{
	if (HasHaunch())	return true;
	return MSLineMemberG::HasSubMember();

}

vector<MSElement*> MSWallFootG::GetSubMembers()
{
	vector<MSElement*> subList;
	subList = MSLineMemberG::GetSubMembers();
	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		subList.push_back(ma_Haunches[i]);
	}
	return subList;
}

void MSWallFootG::DeleteSubMembers()
{
	DeleteHaunchs();
	MSLineMemberG::DeleteSubMembers();
}

bool MSWallFootG::UsedMaterial(long materialID)
{
	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		long nHaunchMaterialID = ma_Haunches[i]->GetHaunchMaterialID();
		if (nHaunchMaterialID == materialID)
			return true;
	}

	return MSLinkMemberG::UsedMaterial(materialID);
}

void MSWallFootG::SubMemberMaterialValidCheck()
{
	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return;

	MSLinkMemberG::SubMemberMaterialValidCheck();

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