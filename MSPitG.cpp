#include "StdAfx.h"
#include "MSPitG.h"
#include "MSRebarBuilding.h"
#include "MSRcFaceMemberM.h"
#include "MSSubHaunchG.h"
#include "MSConnection.h"
#include "MSLevelZone.h"
#include "MSFloor.h"
#include "MSPolylineObject.h"
#include "MSModelChecker.h"
#include "MSCompoGUtil.h"

#include "GMLib/MSVersionInfo.h"

#include "GMLib/GM2DPolyline.h"
#include "GMLib\AcisGeomHelper.h"
#include "GMLib\GM2DBound.h"

#include "GMLib/MSVersionInfo.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

IMPLEMENT_SERIAL(MSPitG, MSPartCompoG, VERSIONABLE_SCHEMA | 1)
MSPitG::MSPitG()
{
	Init(true);
}

MSPitG::~MSPitG(void)
{
	DeletePolylineInfo();
	DeleteHaunchs();
}

void MSPitG::DeletePolylineInfo()
{
    if(mp_OutPoly)
        mp_OutPoly->ResetPolyline();
    
    if(mp_InnerPoly)
        mp_InnerPoly->ResetPolyline();

	long NumInPoly = ma_InPoly.GetSize();
    for(long iPoly=0; iPoly<NumInPoly; iPoly++)
		ma_InPoly[iPoly]->ResetPolyline();
	ma_InPoly.RemoveAll();

	NumInPoly = ma_WellPoly.GetSize();
	for (long iPoly = 0; iPoly<NumInPoly; iPoly++)
		ma_WellPoly[iPoly]->ResetPolyline();
	ma_WellPoly.RemoveAll();
}

MSObject* MSPitG::Copy( bool bNewID /*= true*/ )
{
	MSPitG* pNewObj;

	pNewObj = new MSPitG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSPitG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msPitG) { ASSERT(0); return; }
	MSPartCompoG::CopyFromMe(pSource_in, bNewID);
	
	MSPitG* pSource = (MSPitG*)pSource_in;

    this->m_dInnerDepth = pSource->m_dInnerDepth;
    this->m_dPitDepth = pSource->m_dPitDepth;
    this->m_dThick = pSource->m_dThick;
    this->m_dOffset = pSource->m_dOffset;
	this->m_dLxDeg = pSource->m_dLxDeg;
	this->m_bIsCutSlab = pSource->m_bIsCutSlab;

    this->m_dBarDir = pSource->m_dBarDir;
    this->m_IsHaunch = pSource->m_IsHaunch;
    this->m_numInPoly = pSource->m_numInPoly;

    DeletePolylineInfo();
    if(pSource->mp_OutPoly)
    {
		mp_OutPoly = new GM2DPolyline(TRUE);
        mp_OutPoly->CopyFrom(pSource->mp_OutPoly);
    }

    if(pSource->mp_InnerPoly)
    {
		mp_InnerPoly = new GM2DPolyline(TRUE);
        mp_InnerPoly->CopyFrom(pSource->mp_InnerPoly);
    }

    int count = pSource->ma_InPoly.GetSize();
    for (int i = 0; i < count; i++)
    {
		GM2DPolyline* temp = new GM2DPolyline(TRUE);
        temp->CopyFrom(pSource->ma_InPoly[i]);
		if (AddInPoly(temp) == false)
			delete temp;
    }

	count = pSource->ma_WellPoly.GetSize();
	for (int i = 0; i < count; i++)
	{
		GM2DPolyline* temp = new GM2DPolyline(TRUE);
		temp->CopyFrom(pSource->ma_WellPoly[i]);
		if (AddWellPoly(temp) == false)
			delete temp;
	}

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

void MSPitG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSPartCompoG::Init(bInConstructor);

	m_dBarDir = 0;
	m_IsHaunch = FALSE;
	m_numInPoly = 0;

    this->m_dInnerDepth = 300;
    this->m_dPitDepth = 2200;
    this->m_dThick = 800;
    this->m_dOffset = 0;
	this->m_dLxDeg = 0;
	this->m_bIsCutSlab = true;

    mp_OutPoly = NULL;
    mp_InnerPoly = NULL;

	m_TopSpacerID = 0;
	m_BotSpacerID = 0;
}

void MSPitG::Serialize(CArchive &ar)
{
	MSPartCompoG::Serialize(ar);
	long NumVec=0;
    bool bInnerPoly = false;
	bool bEachCompoM = false;
	if(ar.IsStoring())
	{
		if(mp_CompoM)
		{
			if(mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if(bEachCompoM == true)
			{
				long nType = mp_CompoM->m_nType;
				ar << nType;
				mp_CompoM->Serialize(ar);
			}
		}
		double dTempData=0.0;
		ar << dTempData;
		ar << m_dInnerDepth;
		ar << m_dBarDir;
		ar << m_IsHaunch;
		ar << m_numInPoly;//내부폴리곤 갯수
		ar << m_dOffset;
		ar << m_dPitDepth;
		ar << m_dThick;
		ar << m_dLxDeg;
		ar << m_bIsCutSlab;

		mp_OutPoly->Serialize(ar);

		long NumInPoly = ma_InPoly.GetSize();
		ar << NumInPoly;
		for(long iPoly=0; iPoly<NumInPoly; iPoly++)
		{
			GM2DPolyline* pPoly = ma_InPoly[iPoly];
			pPoly->Serialize(ar);
		}

		NumInPoly = ma_WellPoly.GetSize();
		ar << NumInPoly;
		for (long iPoly = 0; iPoly<NumInPoly; iPoly++)
		{
			GM2DPolyline* pPoly = ma_WellPoly[iPoly];
			pPoly->Serialize(ar);
		}

        if(mp_InnerPoly == NULL)
            ar << bInnerPoly;
        else
        {
			bInnerPoly = true;
			ar << bInnerPoly;
			mp_InnerPoly->Serialize(ar);
        }

		ar << m_TopSpacerID;
		ar << m_BotSpacerID;

		long NumSlabG = ma_SlabGID.size();
		ar << NumSlabG;
		for (long iSlabG = 0; iSlabG < NumSlabG; iSlabG++)
		{
			ar << ma_SlabGID[iSlabG];
		}

		int count = ma_Haunches.size();
		ar << count;
		for (int i = 0; i < count; ++i)
			ma_Haunches[i]->Serialize(ar);
	}
	else
	{
		ar >> bEachCompoM;
		if(bEachCompoM)
		{
			if ((IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() < 20240307)
			 || (IsBHQorBHCEMode() && (MSVersionInfo::GetCurrentVersion() < 20220321)))
			{
				MSBaseBuilding* pBldg = GetBuilding();
				long MType = 0;
				ar >> MType;

				MSRcFaceMemberM* pFaceM = (MSRcFaceMemberM*)pBldg->CreateNewCompoM(MType);
				pFaceM->m_nType = (MSCompoM::Type_MSCompoM)MType;

				long nMaterialID = 0;
				ar >> nMaterialID;
				pFaceM->SetMaterial(pBldg->GetMaterial(nMaterialID));
				ar >> pFaceM->m_Name;

				pFaceM->mp_Owner = pBldg;
				mp_CompoM = pFaceM;
			}
			else
			{
				long nType = 0;
				ar >> nType;
				mp_CompoM = GetBuilding()->CreateNewCompoM(nType);
				mp_CompoM->Serialize(ar);
			}
		}
		double dTempData=0.0;
		ar >> dTempData;
		ar >> m_dInnerDepth;
		ar >> m_dBarDir;
		ar >> m_IsHaunch;
		ar >> m_numInPoly;//내부폴리곤 갯수
		ar >> m_dOffset;
		ar >> m_dPitDepth;
		ar >> m_dThick;
		if(MSVersionInfo::GetCurrentVersion() >= 20141020)
			ar >> m_dLxDeg;
		if (MSVersionInfo::GetCurrentVersion() >= 20150806)
			ar >> m_bIsCutSlab;

		DeletePolylineInfo();
		mp_OutPoly = new GM2DPolyline(TRUE);
		mp_OutPoly->Serialize(ar);

		if (!mp_OutPoly->IsCCW())
			mp_OutPoly->ReverseDir();

		long NumInPoly = 0;
		ar >> NumInPoly;
		for(long iPoly=0; iPoly<NumInPoly; iPoly++)
		{
			GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
			pPoly->Serialize(ar);
			if (AddInPoly(pPoly) == false)
				delete pPoly;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150713)
		{
			NumInPoly = 0;
			ar >> NumInPoly;
			for (long iPoly = 0; iPoly < NumInPoly; iPoly++)
			{
				GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
				pPoly->Serialize(ar);
				if (AddWellPoly(pPoly) == false)
					delete pPoly;
			}
		}

        ar >> bInnerPoly;
        if(bInnerPoly)
		{
			mp_InnerPoly = new GM2DPolyline(TRUE);
			mp_InnerPoly->Serialize(ar);

			if (MSVersionInfo::GetCurrentVersion() < 20150713)
			{
				GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
				pPoly->CopyFrom(mp_InnerPoly);
				if (AddWellPoly(pPoly) == false)
					delete pPoly;
			}
        }

		if(MSVersionInfo::GetCurrentVersion() >= 20141103)
		{
			ar >> m_TopSpacerID;
			ar >> m_BotSpacerID;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150622)
		{
			long NumSlab = 0;
			ar >> NumSlab;
			for (long iSlab = 0; iSlab < NumSlab; iSlab++)
			{
				long nSlabID = 0;
				ar >> nSlabID;
				ma_SlabGID.push_back(nSlabID);
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150701)
		{
			int count = 0;
			ar >> count;
			ma_Haunches.clear();
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

void MSPitG::Dump(EFS::EFS_Buffer& buffer)
{
	MSPartCompoG::Dump(buffer);

	double dTempData=0.0;
	buffer << dTempData;
	buffer << m_dInnerDepth;
	buffer << m_dBarDir;
	buffer << m_IsHaunch;
	buffer << m_numInPoly;//내부폴리곤 갯수
	buffer << m_dOffset;

	buffer << m_dPitDepth;
	buffer << m_dThick;
	buffer << m_dLxDeg;
	buffer << m_bIsCutSlab;

	mp_OutPoly->Dump(buffer);

	long NumInPoly = ma_InPoly.GetSize();
	buffer << NumInPoly;
	for(long iPoly=0; iPoly<NumInPoly; iPoly++)
	{
		GM2DPolyline* pPoly = ma_InPoly[iPoly];
		pPoly->Dump(buffer);
	}

	NumInPoly = ma_WellPoly.GetSize();
	buffer << NumInPoly;
	for (long iPoly = 0; iPoly<NumInPoly; iPoly++)
	{
		GM2DPolyline* pPoly = ma_WellPoly[iPoly];
		pPoly->Dump(buffer);
	}

	bool bInnerPoly = false;
	if(mp_InnerPoly == NULL)
		buffer << bInnerPoly;
	else
	{
		bInnerPoly = true;
		buffer << bInnerPoly;
		mp_InnerPoly->Dump(buffer);
	}
	buffer << m_TopSpacerID << m_BotSpacerID;

	int count = ma_Haunches.size();
	buffer << count;
	for (int i = 0; i < count; ++i)
		ma_Haunches[i]->Dump(buffer);
}

void MSPitG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSPartCompoG::Recovery(buffer);

	double dTempData=0.0;
	buffer >> dTempData;
	buffer >> m_dInnerDepth;
	buffer >> m_dBarDir;
	buffer >> m_IsHaunch;
	buffer >> m_numInPoly;//내부폴리곤 갯수
	buffer >> m_dOffset;

	buffer >> m_dPitDepth;
	buffer >> m_dThick;
	buffer >> m_dLxDeg;
	buffer >> m_bIsCutSlab;

	DeletePolylineInfo();
	mp_OutPoly = new GM2DPolyline(TRUE);
	mp_OutPoly->Recovery(buffer);

	long NumInPoly = 0;
	buffer >> NumInPoly;
	for(long iPoly=0; iPoly<NumInPoly; iPoly++)
	{
		GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
		pPoly->Recovery(buffer);
		if (AddInPoly(pPoly) == false)
			delete pPoly;
	}

	NumInPoly = 0;
	buffer >> NumInPoly;
	for (long iPoly = 0; iPoly<NumInPoly; iPoly++)
	{
		GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
		pPoly->Recovery(buffer);
		if (AddWellPoly(pPoly) == false)
			delete pPoly;
	}

	bool bInnerPoly = false;
	buffer >> bInnerPoly;
	if(bInnerPoly)
	{
		mp_InnerPoly = new GM2DPolyline(TRUE);
		mp_InnerPoly->Recovery(buffer);
	}
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

void MSPitG::RecoverPointerAfterSerialize(MSFloor* floor)
{
	MSPartCompoG::RecoverPointerAfterSerialize(floor);

	MSPitG* slab = this;
	vector<MSCompoG*> sub_list;
	for (int i = 0; i<slab->ma_Haunches.size(); ++i)
		sub_list.push_back((MSCompoG*)slab->ma_Haunches[i]);
	for (int i = 0; i<sub_list.size(); ++i)
	{
		for (int k = 0; k<sub_list[i]->ma_Connections.size(); ++k)
			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	//BH-1729
	for (int i = 0; i < ma_Haunches.size(); i++)
		ma_Haunches[i]->CheckValidConnections();
}

void MSPitG::RecoverPointerAfterSerialize_SubMembers()
{
	MSPitG* slab = (MSPitG*)this;

	vector<MSCompoG*> sub_list;
	for (int i = 0; i<slab->ma_Haunches.size(); ++i)
		sub_list.push_back((MSCompoG*)slab->ma_Haunches[i]);
	for (int i = 0; i<sub_list.size(); ++i)
	{
		for (int k = 0; k<sub_list[i]->ma_Connections.size(); ++k)
			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	for (int i = 0; i < sub_list.size(); ++i)
		sub_list[i]->ResetGetID();

	//BH-1729
	for (int i = 0; i < ma_Haunches.size(); i++)
		ma_Haunches[i]->CheckValidConnections();
}

bool MSPitG::MakeBoundary()
{
	GM2DPolyline* pBoundary = mp_Boundary;
	pBoundary->CopyFrom(mp_OutPoly);
	pBoundary->MakeCCW();
	return true;
}

void MSPitG::Translate( GM2DVector &pDelta )
{
    MSPartCompoG::Translate(pDelta);
    if(mp_OutPoly)
        mp_OutPoly->Translate(pDelta);
    if(mp_InnerPoly)
        mp_InnerPoly->Translate(pDelta);
    int nInpoly = ma_InPoly.GetSize();
    for(int iPoly=0;iPoly<nInpoly;iPoly++)
    {
        GM2DPolyline* pPoly = ma_InPoly[iPoly];
        pPoly->Translate(pDelta);
    }
	nInpoly = ma_WellPoly.GetSize();
	for (int iPoly = 0; iPoly<nInpoly; iPoly++)
	{
		GM2DPolyline* pPoly = ma_WellPoly[iPoly];
		pPoly->Translate(pDelta);
	}
	for (int i = 0; i < ma_Haunches.size(); ++i)
	{
		MSSubHaunchG* cur = ma_Haunches[i];
		cur->Translate(pDelta);
	}
	MakeBoundary();
}

void MSPitG::MirrorByAxis( GM2DLineVector& pAxis )
{
    MSPartCompoG::MirrorByAxis(pAxis);
    if(mp_OutPoly)
        mp_OutPoly->MirrorByAxis(pAxis);
    if(mp_InnerPoly)
        mp_InnerPoly->MirrorByAxis(pAxis);
    int nInpoly = ma_InPoly.GetSize();
    for(int iPoly=0;iPoly<nInpoly;iPoly++)
    {
        GM2DPolyline* pPoly = ma_InPoly[iPoly];
        pPoly->MirrorByAxis(pAxis);
    }
	nInpoly = ma_WellPoly.GetSize();
	for (int iPoly = 0; iPoly<nInpoly; iPoly++)
	{
		GM2DPolyline* pPoly = ma_WellPoly[iPoly];
		pPoly->MirrorByAxis(pAxis);
	}
	for (int i = 0; i < ma_Haunches.size(); ++i)
	{
		MSSubHaunchG* cur = ma_Haunches[i];
		cur->MirrorByAxis(pAxis);
	}
	MakeBoundary();

	m_dLxDeg = m_dLxDeg + (pAxis.GetDegree() - m_dLxDeg) * 2;
}

void MSPitG::RotateZ( double theta, GM2DVector *pCenter )
{
    MSPartCompoG::RotateZ(theta, pCenter);
    if(mp_OutPoly)
        mp_OutPoly->RotateZ(theta, pCenter);
    if(mp_InnerPoly)
        mp_InnerPoly->RotateZ(theta, pCenter);
    int nInpoly = ma_InPoly.GetSize();
    for(int iPoly=0;iPoly<nInpoly;iPoly++)
    {
        GM2DPolyline* pPoly = ma_InPoly[iPoly];
        pPoly->RotateZ(theta, pCenter);
    }
	nInpoly = ma_WellPoly.GetSize();
	for (int iPoly = 0; iPoly<nInpoly; iPoly++)
	{
		GM2DPolyline* pPoly = ma_WellPoly[iPoly];
		pPoly->RotateZ(theta, pCenter);
	}
	for (int i = 0; i < ma_Haunches.size(); ++i)
	{
		MSSubHaunchG* cur = ma_Haunches[i];
		cur->RotateZ(theta, pCenter);
	}
	MakeBoundary();
	m_dLxDeg += theta * Rad2Deg;
}

double MSPitG::GetTopSlabThick()
{
	double dMaxThick = 0;

	MSFloor* pFloor = GetFloor();
	if (pFloor == NULL)
		return dMaxThick;

	for (long iSlab = 0; iSlab < ma_SlabGID.size(); iSlab++)
	{
		MSCompoG* pSlabG = pFloor->GetCompoG(ma_SlabGID[iSlab]);
		if (pSlabG == NULL)
			continue;
		
		double dSlabThick = pSlabG->GetThick();
		if (dMaxThick < dSlabThick)
			dMaxThick = dSlabThick;
	}

	return dMaxThick;
}

void MSPitG::RemoveSlabGID(long nSlabGID)
{
	for(long i = ma_SlabGID.size() -1 ; i >= 0 ; i--)
	{
		if(ma_SlabGID[i] == nSlabGID)
			ma_SlabGID.erase(ma_SlabGID.begin() + i);
	}
}

void MSPitG::MakeConnectedSlabInfoStep2(MSPolylineObject* pUrPolylineObject)
{
	MSSlabG* pSlab = (MSSlabG*)pUrPolylineObject->GetOwner();
	CheckModelOpt CheckOpt;
	if(IsLevelOverlap(pSlab) == false)
		return;

	BOOL bRval = mp_OutPoly->OverlapEx(*(pUrPolylineObject->mp_Profile));
	if(bRval == TRUE)
		ma_SlabGID.push_back(pUrPolylineObject->GetOwner()->GetID());

	//Pit 에 의한 오프닝 추가	
	// => 마감 로직으로 처리
	vector<GM2DPolyline*> Joined_Polys2;
	for (int idx = 0; idx < ma_InPoly.GetSize(); idx++)
	{
		GM2DPolyline* inPoly = ma_InPoly[idx];
		AcisGeomHelper::BooleanPolyline('I', *pSlab->GetProfile(), *inPoly, Joined_Polys2);
		long NumJoinedPolys2 = Joined_Polys2.size();

		for (long iJoind = 0; iJoind < NumJoinedPolys2; iJoind++)
			pUrPolylineObject->AddOpening(Joined_Polys2[iJoind], this);

		for (int iJoined = 0; iJoined < NumJoinedPolys2; iJoined++)
			delete Joined_Polys2[iJoined];
		Joined_Polys2.clear();
	}
}

void MSPitG::MakeConnectedSlabInfo()
{
	MSFloor* pFloor = GetFloor();
	if(pFloor == NULL)
		return;

	ma_SlabGID.clear();
	vector<MSPolylineObject*> PolylineObjectArr;
	pFloor->GetPolylineObjectArr(PolylineObjectArr, MSElement::msSlabG);

	long nPolylineObject = PolylineObjectArr.size();
	for(long iPolylineObject=0; iPolylineObject < nPolylineObject; iPolylineObject++)
	{
		MSPolylineObject* pCurPolylineObject = (MSPolylineObject*)(PolylineObjectArr[iPolylineObject]);
		MakeConnectedSlabInfoStep2(pCurPolylineObject);
	}
}

void MSPitG::InitializeParameter()
{
	RemoveOpeningInfoAll();
	MSCompoG::InitializeParameter();

	ma_SlabGID.clear();
}

void MSPitG::RemoveOpeningInfoAll()
{
	MSFloor *pFloor = GetFloor();
	long NumSlabID = ma_SlabGID.size();
	for(long i = 0 ; i < NumSlabID ; i++)
	{
		MSSlabG* pSlabG = (MSSlabG*)pFloor->GetCompoG(ma_SlabGID[i]);
		if (pSlabG != NULL)
			pSlabG->RemoveOpeningInfo(GetID());
	}
}

void MSPitG::UpdateBoundaryVisibilities(long nUpdateConnIdx)
{
	// 마감 프로젝트인 경우
	if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Finish)
	{
		vector<GM2DCurve*> SegArr;
		GM2DPolyline* pTrimmedPoly = GetTrimmedBoundary();
		double dTol = DTOL_MODELING;

		vector<MSCompoG*> partnerSlabArr;
		for (int iConn = 0; iConn < ma_Connections.size(); iConn++)
		{
			MSConnection* pConn = ma_Connections[iConn];
			MSSlabG* pPartnerG = (MSSlabG*)pConn->GetPartner(this);
			if (pPartnerG != nullptr && pPartnerG->GetType() == MSElement::msSlabG)
			{
				pTrimmedPoly->SegmentstoMe(pPartnerG->GetProfile(), dTol, FALSE);
				partnerSlabArr.push_back(pPartnerG);
			}
		}

		pTrimmedPoly->GetSegments(SegArr);
		ma_TrimmedBoundaryVisible.clear();
		bool bIncludeOnLine = true;

		for (long iSeg = 0; iSeg < SegArr.size(); iSeg++)
		{
			GM2DCurve* pCurve = SegArr[iSeg];
			int nVisible = 1;

			for (long iPit = 0; iPit < partnerSlabArr.size(); iPit++)
			{
				MSSlabG* pSlab = (MSSlabG*)partnerSlabArr[iPit];
				if (pSlab->Contains(pCurve, bIncludeOnLine) == true)
				{
					nVisible = 2;
					break;
				}
			}

			ma_TrimmedBoundaryVisible.push_back(nVisible);
			delete pCurve;
		}
	}

	MSCompoG::UpdateBoundaryVisibilities(nUpdateConnIdx);
}

bool MSPitG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	double dContaionLevelZoneLevel = 0;

	if (bCheckLevelZone)
		dContaionLevelZoneLevel = GetOffsetByContainingLevelZone();

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		TopLevel = GetSlabLevel() + ((MSFloor*)mp_Owner)->GetSlabHeight() + m_dOffset + dContaionLevelZoneLevel;
	}
	else
		TopLevel = GetSlabLevel() + m_dOffset + dContaionLevelZoneLevel;

	BottomLevel = TopLevel - GetThick();
	return true;
}

double MSPitG::GetOffsetByContainingLevelZone()
{
	double dLevel = 0;
	MSFloor* pCurFloor = (MSFloor*)mp_Owner;
	if (pCurFloor != NULL)
	{
		MSLevelZone* pLevelZone = pCurFloor->GetContainingLevelZone(GetProfile());
		if (pLevelZone != NULL)
			dLevel = pLevelZone->m_Level;
	}
	return  dLevel;
}

GM2DPolyline* MSPitG::GetProfile()
{
	return mp_OutPoly;
}

bool MSPitG::VisibleInRect(GM2DRectangle* pRect)
{
	if(mp_OutPoly == NULL)
		return false;

	if (m_RectForQuadTree.Right() < pRect->Left()) return false;
	else if (m_RectForQuadTree.Left() > pRect->Right()) return false;
	if (m_RectForQuadTree.Top() < pRect->Bottom()) return false;
	else if (m_RectForQuadTree.Bottom() > pRect->Top()) return false;

	if (pRect->Left() <= m_RectForQuadTree.Left() && m_RectForQuadTree.Right() <= pRect->Right())
	{
		if (pRect->Bottom() <= m_RectForQuadTree.Bottom() && m_RectForQuadTree.Top() <= pRect->Top())
			return true;
	}

	GM2DPolyline UrPoly(TRUE);
	pRect->GetBoundPolyline(UrPoly);
	
	if(mp_OutPoly->OverlapEx(UrPoly))	return true;

	return false;
}

void MSPitG::AddHaunchGs(vector<MSSubHaunchG*> HaunchGArr)
{
	for (int iHaunchG = 0; iHaunchG < HaunchGArr.size(); iHaunchG++)
	{
		MSSubHaunchG* pHaunchG = HaunchGArr[iHaunchG];
		pHaunchG->GetID();
		ma_Haunches.push_back(pHaunchG);
	}
}

void MSPitG::AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	AddHaunchGs(MSCompoGUtil::MakeHaunchG(this, x, y, x2, y2, haunchAngle, haunchY, haunchY_Top, bTopAuto));
	TrimHaunches();
}

void MSPitG::AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	DeleteHaunchs();
	AddHaunchGs(MSCompoGUtil::MakeHaunchGAllProfile(this, haunchAngle, haunchY, haunchY_Top, bTopAuto));
	TrimHaunches();
}

void MSPitG::TrimHaunches()
{
	MSFloor* pFloor = GetFloor();
	MSCompoGUtil::TrimHaunches(ma_Haunches, pFloor);
}

void MSPitG::CalcBoundRectBeforeAddQuadTree()
{
	GM2DBound MyBound;
	mp_OutPoly->Get2DBound(MyBound);
	GM2DVector* pOrigin = MyBound.GetBottomLeft();
	GM2DVector* pCorner = MyBound.GetTopRight();

	if (pOrigin == NULL || pCorner == NULL) return;
	m_RectForQuadTree.m_Origin.m_X = pOrigin->m_X; m_RectForQuadTree.m_Origin.m_Y = pOrigin->m_Y;
	m_RectForQuadTree.m_Corner.m_X = pCorner->m_X; m_RectForQuadTree.m_Corner.m_Y = pCorner->m_Y;
}

vector<MSCompoG*> MSPitG::GetPartnerCompoGs(long GetIdx)
{
	vector<MSCompoG*> SlabArr = FindConnectMember(FindType_MSElement::msSlabG, FIND_CURFLOOR);
	MSBaseBuilding* pBuilding = GetBuilding();
	if (pBuilding != nullptr)
	{
		for (int i = 0; i < ma_SlabGID.size(); i++)
		{

			for (int iSlab = 0; iSlab < SlabArr.size(); iSlab++)
			{
				if(SlabArr[iSlab]->m_ID == ma_SlabGID[i])
					continue;
			}

			MSCompoG* pCompoG = pBuilding->GetCompoGByFloors(ma_SlabGID[i]);
			if (pCompoG != nullptr)
				SlabArr.push_back(pCompoG);
		}
	}
	return SlabArr;
}

void MSPitG::RegenHaunch()
{
	for (int i = 0; i < ma_Haunches.size(); ++i)
		ma_Haunches[i]->SetLines();
	TrimHaunches();
}

bool MSPitG::HasSpacer()
{
	if (m_TopSpacerID != 0 || m_BotSpacerID != 0)
		return true;
	return false;
}


bool MSPitG::AddInPoly(GM2DPolyline* pInPoly)
{
	for (int i = 0 ; i < ma_InPoly.GetCount() ; i++)
	{
		if (ma_InPoly[i]->SamePolyLine(pInPoly))
			return false;
	}
	pInPoly->MakeCCW();
	ma_InPoly.Add(pInPoly);
	return true;
}

bool MSPitG::AddWellPoly(GM2DPolyline* pWellPoly)
{
	for (int i = 0; i < ma_WellPoly.GetCount(); i++)
	{
		if (ma_WellPoly[i]->SamePolyLine(pWellPoly))
			return false;
	}
	pWellPoly->MakeCCW();
	ma_WellPoly.Add(pWellPoly);
	return true;
}
void MSPitG::MergeColinearHaunch()
{
	vector<vector<MSSubHaunchG*>> HaunchGsArr;
	map<MSSubHaunchG*, bool> haunchmap;
	for (int i = 0; i < ma_Haunches.size(); i++)
		haunchmap.insert(make_pair(ma_Haunches[i], false));

	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		MSSubHaunchG* pHaunchGI = ma_Haunches[i];
		if (haunchmap[pHaunchGI] == true)
			continue;
		vector<MSSubHaunchG*> HaunchGs;
		HaunchGs.push_back(pHaunchGI);

		for (int j = i + 1; j < ma_Haunches.size(); j++)
		{
			MSSubHaunchG* pHaunchGJ = ma_Haunches[j];
			if(haunchmap[pHaunchGJ] == true)
				continue;

			long nRval = pHaunchGI->GetCurve()->IsColinear4DTol(&*pHaunchGJ->GetCurve());
			bool isSameInfo = pHaunchGI->isSameHaunchInfo(pHaunchGJ);
			if (nRval != 0 && isSameInfo == true)
			{
				haunchmap[pHaunchGJ] = true;
				HaunchGs.push_back(pHaunchGJ);
			}
		}

		sort(HaunchGs.begin(), HaunchGs.end(), [](MSSubHaunchG* a, MSSubHaunchG* b)->bool {
			GM2DVector baseVec(INTMAX_MIN, INTMAX_MIN);
			return a->GetCenVec().Distance(baseVec) < b->GetCenVec().Distance(baseVec); });

		HaunchGsArr.push_back(HaunchGs);
	}

	vector<MSSubHaunchG*> NewHaunchs;
	for (int i = 0; i < HaunchGsArr.size(); i++)
	{
		MSSubHaunchG* pNewHaunch = NULL;
		for (int j = 0; j < HaunchGsArr[i].size(); j++)
		{
			MSSubHaunchG* pHaunchG = HaunchGsArr[i][j];
			if (j == 0)
			{
				pNewHaunch = (MSSubHaunchG*)pHaunchG->Copy();
				NewHaunchs.push_back(pNewHaunch);
				continue;
			}

			GM2DCurve* pCurve = pNewHaunch->GetCurve();
			GM2DCurve* pUrCurve = pHaunchG->GetCurve();
			double dSLoc = pCurve->OrthoBaseLocValue(pUrCurve->m_P0);
			double dELoc = pCurve->OrthoBaseLocValue(pUrCurve->m_P1);
			if (dSLoc > dELoc)
				swap(dSLoc, dELoc);

			if (dSLoc < 0)
				pCurve->m_P0 = pCurve->PointAtParam(dSLoc);
			if(dELoc > 1)
				pCurve->m_P1 = pCurve->PointAtParam(dELoc);

			pNewHaunch->SetCenCurve(pCurve);
		}
		
	}
	DeleteHaunchs();
	ma_Haunches = NewHaunchs;
}

bool MSPitG::CheckData()
{
	if (MSElement::CheckData() == false)
		return false;

	return true;
}

void MSPitG::MakeCCW()
{
	MSCompoG::MakeCCW();
	mp_OutPoly->MakeCCW();
	for (int i = 0; i < ma_WellPoly.GetSize(); i++)
		ma_WellPoly[i]->MakeCCW();
	
	for (int i = 0; i < ma_InPoly.GetSize(); i++)
		ma_InPoly[i]->MakeCCW();
}

long MSPitG::ResetGetID()
{
	MSCompoG::ResetGetID();
	for (int i = 0; i < ma_Haunches.size(); i++)
		ma_Haunches[i]->ResetGetID();
	return m_ID;
}

void MSPitG::DeleteHaunchs()
{
	for (int i = 0; i < ma_Haunches.size(); i++)
		delete ma_Haunches[i];

	ma_Haunches.clear();
}

bool MSPitG::Remove(MSSubHaunchG* pSubHaunchG)
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

bool MSPitG::RemoveSubMember(MSElement* element)
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

bool MSPitG::HasHaunch()
{
	if (ma_Haunches.size() > 0)
		return true;
	return false;
}

bool MSPitG::HasSubMember()
{
	if (HasHaunch())	return true;
	return MSPartCompoG::HasSubMember();
}

vector<MSElement*> MSPitG::GetSubMembers()
{
	vector<MSElement*> subList;
	subList = MSPartCompoG::GetSubMembers();
	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		subList.push_back(ma_Haunches[i]);
	}
	return subList;
}

void MSPitG::DeleteSubMembers()
{
	DeleteHaunchs();
	MSPartCompoG::DeleteSubMembers();
}

vector<GM2DCurve*> MSPitG::GetVisibileCurves(eVisibileCurveType nVisibile)
{
	vector<GM2DCurve*> VisibileCurveArr;
	GM2DPolyline* pPoly = GetTrimmedBoundary();
	vector<GM2DCurve*> SegArr;
	pPoly->GetSegments(SegArr);

	if (SegArr.size() != ma_TrimmedBoundaryVisible.size())
	{
		UpdateBoundaryVisibilities();
		for (long iSeg = 0; iSeg < SegArr.size(); iSeg++)
			delete SegArr[iSeg];
		SegArr.clear();
		pPoly = GetTrimmedBoundary();
		pPoly->GetSegments(SegArr);
	}

	for (long iSeg = 0; iSeg < SegArr.size(); iSeg++)
	{
		if (ma_TrimmedBoundaryVisible[iSeg] == nVisibile)
			VisibileCurveArr.push_back(SegArr[iSeg]);
	}

	return VisibileCurveArr;
}

bool MSPitG::IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance)
{
	MSPitG* urPit = dynamic_cast<MSPitG*>(urCompoG);
	if (urPit == nullptr) return false;
	
	//inner 개수 체크
	long NumInPoly = ma_InPoly.GetSize();
	long urNumInPoly = urPit->ma_InPoly.GetSize();
	if (NumInPoly != urNumInPoly) return false;

	//well 개수 체크
	long NumWellPoly = ma_WellPoly.GetSize();
	long urNumWellPoly = urPit->ma_WellPoly.GetSize();
	if (NumWellPoly != urNumWellPoly) return false;

	// line 체크 : 무조건 same만 체크
	//outline
	if (!mp_OutPoly->SamePolyLine((urPit->mp_OutPoly))) return false;
	
	//inner
	for (long iPoly = 0; iPoly < NumInPoly; iPoly++)
	{
		if (!ma_InPoly[iPoly]->SamePolyLine(urPit->ma_InPoly[iPoly])) return false;
	}

	for (long iPoly = 0; iPoly < NumWellPoly; iPoly++)
	{
		if (!ma_WellPoly[iPoly]->SamePolyLine(urPit->ma_WellPoly[iPoly])) return false;
	}
	return true;
}

bool MSPitG::UsedMaterial(long materialID)
{
	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		long nHaunchMaterialID = ma_Haunches[i]->GetHaunchMaterialID();
		if (nHaunchMaterialID == materialID)
			return true;
	}

	return MSCompoG::UsedMaterial(materialID);
}

void MSPitG::SubMemberMaterialValidCheck()
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

void MSPitG::FixProfile()
{
	mp_OutPoly->DeleteInLinePoint(DTOL_MODELING);
	mp_OutPoly->MakeClosedPolyline(DTOL_MODELING);
}