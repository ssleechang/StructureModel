#include "StdAfx.h"
#include "MSSubHaunchG.h"
#include "MSPitG.h"
#include "MSConnection.h"
#include "MSRebarBuilding.h"
#include "MSStoryMaterial.h"

IMPLEMENT_SERIAL(MSSubHaunchG, MSLinkMemberG, VERSIONABLE_SCHEMA | 1)

MSSubHaunchG::MSSubHaunchG()
{
	Init(true);
}

MSSubHaunchG::~MSSubHaunchG(void)
{
	long NumConnection = ma_Connections.size();
	for (long iConn = NumConnection - 1; iConn >= 0; iConn--)
	{
		MSConnection* pConnection = ma_Connections[iConn];
		pConnection->SetDeleted(true);
	}

	ma_Connections.clear();
}

void MSSubHaunchG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLinkMemberG::Dump(buffer);

	buffer << m_dHaunchAngle << m_dHaunchY << m_dHaunchY_Top;
	buffer << m_bUseHaunchMaterial << m_nHaunchMaterialID << m_bUseBackFillFormwork << m_IsUsedStoryMaterial;
}

void MSSubHaunchG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLinkMemberG::Recovery(buffer);

	buffer >> m_dHaunchAngle >> m_dHaunchY >> m_dHaunchY_Top;
	buffer >> m_bUseHaunchMaterial >> m_nHaunchMaterialID >> m_bUseBackFillFormwork >> m_IsUsedStoryMaterial;
}


void MSSubHaunchG::Serialize(CArchive &ar)
{
	MSLinkMemberG::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_dHaunchAngle << m_dHaunchY << m_dHaunchY_Top;
		ar << m_bUseHaunchMaterial << m_nHaunchMaterialID;
		ar << m_bUseBackFillFormwork;
		ar << m_IsUsedStoryMaterial;
	}
	else
	{
		ar >> m_dHaunchAngle >> m_dHaunchY >> m_dHaunchY_Top;
		ar >> m_bUseHaunchMaterial >> m_nHaunchMaterialID;

		if (MSVersionInfo::GetCurrentVersion() >= 20160309)
		{
			ar >> m_bUseBackFillFormwork;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160310)
		{
			ar >> m_IsUsedStoryMaterial;
		}
	}
}

MSObject* MSSubHaunchG::Copy(bool bNewID /*= true*/)
{
	MSSubHaunchG* pNewObj;

	pNewObj = new MSSubHaunchG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSSubHaunchG::CopyFromMe(MSSubHaunchG* pSource, bool bNewID/*=true*/)
{
	MSLinkMemberG::CopyFromMe(pSource, bNewID);

	if (pSource)
	{
		m_dHaunchAngle = pSource->m_dHaunchAngle;
		m_dHaunchY = pSource->m_dHaunchY;
		m_dHaunchY_Top = pSource->m_dHaunchY_Top;
		m_bUseHaunchMaterial = pSource->m_bUseHaunchMaterial;
		m_nHaunchMaterialID = pSource->m_nHaunchMaterialID;
		mp_OwnerCompoG = pSource->mp_OwnerCompoG;
		m_bUseBackFillFormwork = pSource->m_bUseBackFillFormwork;
		m_IsUsedStoryMaterial = pSource->m_IsUsedStoryMaterial;
	}
}

void MSSubHaunchG::Init(bool bInConstructor/*=false*/)
{
	if (!bInConstructor)
		MSElement::Init(bInConstructor);

	m_dHaunchAngle = 45;
	m_dHaunchY = 0;
	m_dHaunchY_Top = 0;
	m_bUseHaunchMaterial = false;
	m_bUseBackFillFormwork = true;
	m_nHaunchMaterialID = 0;
	mp_OwnerCompoG = NULL;
	m_IsUsedStoryMaterial = true;
}

MSMaterial* MSSubHaunchG::GetMaterial()
{
	MSMaterial* matl = NULL;
	if (m_bUseHaunchMaterial)
	{
		MSBaseBuilding* bldg = mp_OwnerCompoG->GetBuilding();
		long nHaunchMaterialID = GetHaunchMaterialID();
		matl = bldg->GetMaterial(nHaunchMaterialID);

		if (matl == NULL)
			matl = mp_OwnerCompoG->GetMaterial();
	}
	else
		matl = mp_OwnerCompoG->GetMaterial();

	return matl;
}

bool MSSubHaunchG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc)
{
	// Dummy for Trim Option
	TopLevel = 100;
	BottomLevel = 0;
	return true;
}

void MSSubHaunchG::SetLines()
{
	if (ma_Vec.size() != 2) return;
	if (mp_Curve == NULL) return;

	double WIDTH = GetWidth();
	GM2DVector* p1 = ma_Vec[0];
	GM2DVector* p2 = ma_Vec[1];

	if (mp_RLine != nullptr) { delete mp_RLine; mp_RLine = nullptr; }
	if (mp_LLine != nullptr) { delete mp_LLine; mp_LLine = nullptr; }

	if (mp_LLineTR != nullptr) { delete mp_LLineTR; mp_LLineTR = nullptr; }
	if (mp_RLineTR != nullptr) { delete mp_RLineTR; mp_RLineTR = nullptr; }

	mp_LLine = mp_Curve->Copy();
	mp_RLine = mp_Curve->Copy();
	mp_RLine->NormalTranslateMe(-WIDTH);
	if (mp_Curve->m_ObjType == GT_ARC)
	{
		GM2DVector LCenVec = mp_RLine->PointAtParam(0.5);
		// Owner부재가 슬래브 또는 라인부재가 된다.
		MSElement* pOwnerCompoG = (MSElement*)GetOwnerCompoG(); 
		GM2DPolyline* pPoly = pOwnerCompoG->GetProfile();
		if (pPoly->Contains(&LCenVec) != 1)
			mp_RLine->NormalTranslateMe(WIDTH * 2);

		// 아크인 경우 RLine이 LLine보다 반지름이 커야한다. SetLine() 함수와 동일하게 셋팅함
		if (((GM2DArc*)mp_LLine)->GetRadius() > ((GM2DArc*)mp_RLine)->GetRadius())
			std::swap(mp_LLine, mp_RLine);
	}

	if (mp_LLineTR != nullptr)
	{
		delete mp_LLineTR;
		mp_LLineTR = nullptr;
	}
	if (mp_RLineTR != nullptr)
	{
		delete mp_RLineTR;
		mp_RLineTR = nullptr;
	}
	m_DoUpdateTrimmedProfile = true;

	// 	mp_RLine->NormalTranslateMe(-WIDTH);
	// 	if(mp_LeftSlab->Contains(&(mp_LLine->PointAtParam(0.5))) > 0)
	// 		mp_LLine = mp_Curve->Copy();
	// 	if(mp_LeftSlab->Contains(&(mp_RLine->PointAtParam(0.5))) > 0)
	// 		mp_RLine = mp_Curve->Copy();

	// 	if (mp_Curve->m_ObjType == GT_ARC)
	// 	{
	// 		GM2DArc* pArc = (GM2DArc*)mp_Curve;
	// 		bool reverse = (pArc->GetStartPoint().Distance(*p1) > 1.e-3);
	// 		double delta_r = reverse ? WIDTH : -WIDTH;
	// 		GM2DArc* pArcLeft = new GM2DArc(pArc->GetRadius()+delta_r, pArc->m_Center.m_X, pArc->m_Center.m_Y, pArc->m_Theta1, pArc->m_Theta2);
	// 		//mp_Curve = pArc->Copy();
	// 		
	// 		mp_RLine = pArc->Copy();
	// 		mp_LLine = pArcLeft;
	// 	}
	// 	else
	// 	{
	// 		double leng = p1->Distance(*p2);
	// 		double uvec[2] = { (p2->m_X - p1->m_X)/leng, (p2->m_Y - p1->m_Y)/leng };
	// 		double uvec_left[2] = { -uvec[1], uvec[0] };
	// 		//mp_Curve = new GM2DLineVector(p1->m_X, p1->m_Y, p2->m_X, p2->m_Y);
	// 		GM2DVector p1Left(p1->m_X+uvec_left[0]*WIDTH, p1->m_Y+uvec_left[1]*WIDTH);
	// 		GM2DVector p2Left(p2->m_X+uvec_left[0]*WIDTH, p2->m_Y+uvec_left[1]*WIDTH);
	// 
	// 		mp_RLine = new GM2DLineVector(p1->m_X, p1->m_Y, p2->m_X, p2->m_Y);
	// 		mp_LLine = new GM2DLineVector(p1Left.m_X, p1Left.m_Y, p2Left.m_X, p2Left.m_Y);
	// 	}
}

void MSSubHaunchG::ResetCenterCurve()
{
	MSPitG* pit = dynamic_cast<MSPitG*>(mp_OwnerCompoG);
	MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_OwnerCompoG);
	double OwnerDegree = 0.0;
	GM2DPolyline* pOwnerProfile = NULL;
	if (pit != NULL)
		pOwnerProfile = pit->GetProfile();
	else if (slab != NULL)
	{
		pOwnerProfile = slab->GetProfile();
	}
	else
		return;
	
	GM2DCurve* pCurve;
	GM2DLineVector centerLine;
	if (isArc())
	{	
		GM2DArc* arcCen = dynamic_cast<GM2DArc*>(mp_Curve);
		pCurve = pOwnerProfile->GetOnLineCurve(arcCen, 1.0);
		if (pCurve == NULL) return;
	}
	else
	{
		centerLine.m_P0.SetXY(ma_Vec[0]->m_X, ma_Vec[0]->m_Y);
		centerLine.m_P1.SetXY(ma_Vec[1]->m_X, ma_Vec[1]->m_Y);
		pCurve = pOwnerProfile->GetOnLineCurve(&centerLine, 1.0);
		if (pCurve == NULL)	return;
	}

	long ParalleType = pCurve->IsParallel4DTol(&centerLine);
	// 0 : Not Parallel, -1 : Parallel but opposite dir, 1 : Parallel and same dir
	if(ParalleType == -1)
	{
		GM2DVector* tempVec = ma_Vec[0];
		ma_Vec[0] = ma_Vec[1];
		ma_Vec[1] = tempVec;
	}
	if (mp_Curve)
	{
		mp_Curve->m_P0.SetXY(ma_Vec[0]->m_X, ma_Vec[0]->m_Y);
		mp_Curve->m_P1.SetXY(ma_Vec[1]->m_X, ma_Vec[1]->m_Y);
	}
	else
	{
		if (isArc())
		{
			return;
		}
		else
		{
			double x1 = ma_Vec[0]->m_X, y1 = ma_Vec[0]->m_Y, x2 = ma_Vec[1]->m_X, y2 = ma_Vec[1]->m_Y;
			mp_Curve = new GM2DLineVector(x1,y1,x2,y2);
			if (mp_Curve->DirVector()*GM2DVector(x2 - x1, y2 - y1) < 0.0)
				((GM2DLineVector*)mp_Curve)->ReverseDirection();
		}
	}

	SetLines();
}

GM2DCurve* MSSubHaunchG::GetCurve()
{
	return mp_Curve;
}

GM2DVector MSSubHaunchG::GetCenVec()
{
	if (mp_Curve != nullptr)
		return mp_Curve->PointAtParam(0.5);

	GM2DLineVector TheLine;
	TheLine.m_P0 = *GetSVec();
	TheLine.m_P1 = *GetEVec();
	return TheLine.PointAtParam(0.5);
}

shared_ptr<GM2DCurve> MSSubHaunchG::GetCenCurve()
{
	if (mp_LLine == NULL || mp_RLine == NULL)
		SetLines();

	shared_ptr<GM2DCurve> pCenCurve;
	if (isArc() == false)
	{
		GM2DLineVector SLine(mp_LLine->m_P0, mp_RLine->m_P0);
		GM2DLineVector ELine(mp_LLine->m_P1, mp_RLine->m_P1);
		pCenCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(SLine.PointAtParam(0.5), ELine.PointAtParam(0.5)));
	}
	else
	{
		GM2DVector SVec = mp_LLine->m_P0 + mp_RLine->m_P0;
		GM2DVector EVec = mp_LLine->m_P1 + mp_RLine->m_P1;
		GM2DVector CVec = mp_LLine->PointAtParam(0.5) + mp_RLine->PointAtParam(0.5);
		SVec.m_X = SVec.m_X / 2;
		SVec.m_Y = SVec.m_Y / 2;
		EVec.m_X = EVec.m_X / 2;
		EVec.m_Y = EVec.m_Y / 2;
		CVec.m_X = CVec.m_X / 2;
		CVec.m_Y = CVec.m_Y / 2;

		GM2DArc* pArc = new GM2DArc();
		if (pArc->Set3P(SVec, CVec, EVec))
			pCenCurve = shared_ptr<GM2DCurve>(pArc);
		else
		{
			delete pArc;
			pCenCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(mp_Curve->m_P0, mp_Curve->m_P1));
			SetCenCurve(pCenCurve->Copy());
			SetLines();
		}
	}
	return pCenCurve;
}

void MSSubHaunchG::SetCenCurve(GM2DCurve* pCurve)
{
	if (pCurve == NULL)
		return;

	if (mp_Curve != NULL && mp_Curve != pCurve)
		delete mp_Curve;
	if (pCurve->m_ObjType == GT_ARC)
	{
		GM2DVector SVec = pCurve->m_P0;
		GM2DVector EVec = pCurve->m_P1;
		GM2DVector CVec = pCurve->PointAtParam(0.5);
		GM2DArc TmpArc;
		if (TmpArc.Set3P(SVec, CVec, EVec) == false)
		{
			delete pCurve;
			pCurve = new GM2DLineVector(SVec, EVec);
		}
	}

	mp_Curve = pCurve;

	ma_Vec[0]->SetXY(pCurve->m_P0.m_X, pCurve->m_P0.m_Y);
	ma_Vec[1]->SetXY(pCurve->m_P1.m_X, pCurve->m_P1.m_Y);
}

void MSSubHaunchG::Translate(GM2DVector &pDelta)
{
	MSLinkMemberG::Translate(pDelta);

	if (mp_Curve != NULL) mp_Curve->Translate(pDelta);
	if (mp_LLine != NULL) mp_LLine->Translate(pDelta);
	if (mp_RLine != NULL) mp_RLine->Translate(pDelta);
	if (mp_LLineTR != NULL) mp_LLineTR->Translate(pDelta);
	if (mp_RLineTR != NULL) mp_RLineTR->Translate(pDelta);

	GetTrimmedBoundary()->Translate(pDelta);
}

void MSSubHaunchG::RotateZ(double Theta, GM2DVector* pCenter)
{
	MSLinkMemberG::RotateZ(Theta, pCenter);

	if (mp_Curve != NULL) mp_Curve->RotateZ(Theta, pCenter);
	if (mp_LLine != NULL) mp_LLine->RotateZ(Theta, pCenter);
	if (mp_RLine != NULL) mp_RLine->RotateZ(Theta, pCenter);
	if (mp_LLineTR != NULL) mp_LLineTR->RotateZ(Theta, pCenter);
	if (mp_RLineTR != NULL) mp_RLineTR->RotateZ(Theta, pCenter);

	GetTrimmedBoundary()->RotateZ(Theta, pCenter);
}

void MSSubHaunchG::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSLinkMemberG::MirrorByAxis(pAxis);

	GM2DVector* tmp = ma_Vec[0];
	ma_Vec[0] = ma_Vec[1];
	ma_Vec[1] = tmp;

	if (mp_Curve != NULL) { mp_Curve->MirrorByAxis(pAxis); mp_Curve->ReverseDirection(); }
	if (mp_LLine != NULL) { mp_LLine->MirrorByAxis(pAxis); mp_LLine->ReverseDirection(); }
	if (mp_RLine != NULL) { mp_RLine->MirrorByAxis(pAxis); mp_RLine->ReverseDirection(); }
	if (mp_LLineTR != NULL) { mp_LLineTR->MirrorByAxis(pAxis); mp_LLineTR->ReverseDirection(); }
	if (mp_RLineTR != NULL) { mp_RLineTR->MirrorByAxis(pAxis); mp_RLineTR->ReverseDirection(); }

	GetTrimmedBoundary()->MirrorByAxis(pAxis);
}

void MSSubHaunchG::AddConnectionWithMember(MSCompoG* pPartnerMember)
{
	// 폴리맴버는 Connection 생성 안함
	if (IsPolyMember() || pPartnerMember->IsPolyMember())
		return;

	MSConnection* pConnection = NULL;
	long NumOverMem = ma_Connections.size();
	for (long iOverMem = 0; iOverMem < NumOverMem; iOverMem++)
	{
		MSConnection* pConnection_Cur = ma_Connections[iOverMem];
		if (pConnection_Cur->GetFirstCompoG() == pPartnerMember || pConnection_Cur->GetSecondCompoG() == pPartnerMember)
		{
			pPartnerMember->AddConnection(pConnection_Cur);
			pConnection_Cur->CalculateContainedAngle();
			pConnection_Cur->CalculateLinkMemberTrimOption();
			return;
		}
	}

	if (pConnection == NULL)
	{
		pConnection = MSConnection::GetConnection(pPartnerMember, this);
		pConnection->GetFirstCompoG()->AddConnection(pConnection);
		pConnection->GetSecondCompoG()->AddConnection(pConnection);
		pConnection->CalculateContainedAngle();
		pConnection->CalculateLinkMemberTrimOption();
	}
}

double MSSubHaunchG::GetOwnerCompoGThick()
{
	if (mp_OwnerCompoG == NULL)
		return 0;

	MSPitG* pit = dynamic_cast<MSPitG*>(mp_OwnerCompoG);
	MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_OwnerCompoG);
	double dThick = 0;
	if (pit != NULL)
	{
		dThick = pit->GetThick();
	}
	else if (slab != NULL)
	{
		dThick = slab->GetThick();
	}
	else
		dThick = mp_OwnerCompoG->GetThick();

	return dThick;
}

double MSSubHaunchG::GetDepth()
{
	return GetOwnerCompoGThick() - m_dHaunchY + m_dHaunchY_Top;
}

double MSSubHaunchG::GetThick()
{
	return GetOwnerCompoGThick() - m_dHaunchY + m_dHaunchY_Top;
}

double MSSubHaunchG::GetWidth()
{
	MSPitG* pit = dynamic_cast<MSPitG*>(mp_OwnerCompoG);
	MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_OwnerCompoG);
	double h = GetThick();
	double dWidth = h * (1 / tan(m_dHaunchAngle / 180.0*PI));
	return dWidth;
}

long MSSubHaunchG::GetHaunchMaterialID()
{
	if(!m_bUseHaunchMaterial)
	{
		if(mp_OwnerCompoG == nullptr)
		{
			ASSERT(0);
			return m_nHaunchMaterialID;
		}
		MSMaterial* pMaterial = mp_OwnerCompoG->GetMaterial();
		if(pMaterial)
			return pMaterial->m_ID;
		return m_nHaunchMaterialID;
	}
	if (m_IsUsedStoryMaterial)
	{
		MSFloor* pFloor = GetFloor();

		if (pFloor == NULL)
		{
			ASSERT(0);
			return m_nHaunchMaterialID;
		}
		else
		{
			MSBaseBuilding* pBldg = pFloor->GetBuilding();
			MSStoryMaterial* pStoryMaterial = pBldg->GetStoryMaterialByFloorNum(pFloor->GetFloorNum());
			if (pStoryMaterial == NULL)
			{
				ASSERT(0);
				return m_nHaunchMaterialID;
			}

			MSMaterial* pMaterial = pStoryMaterial->GetMaterial(MSMaterial::msConcrete, false);
			if (pMaterial)	return pMaterial->m_ID;
		}
	}
	return m_nHaunchMaterialID;
}

double MSSubHaunchG::GetLevelZoneZ()
{
	MSFloor* pFloor = GetFloor();
	if (GetOwnerCompoG()->GetType() == MSElement::msPitG || pFloor == NULL)
		return 0;
	
	double dLevelZoneZ = 0;
	if (GetOwnerCompoG()->GetType() == MSElement::msIsoFootG)
	{
		MSIsoFootG* pIsoFootG = (MSIsoFootG*)GetOwnerCompoG();
		dLevelZoneZ = pFloor->GetLevel4LevelZone(pIsoFootG);
	}
	else
		dLevelZoneZ = pFloor->GetLevel4LevelZone(this);
	
	return dLevelZoneZ;
}

bool MSSubHaunchG::isSameHaunchInfo(MSSubHaunchG* pUrHaunch)
{
	if (m_dHaunchAngle == pUrHaunch->m_dHaunchAngle &&
		m_dHaunchY == pUrHaunch->m_dHaunchY &&
		m_dHaunchY_Top == pUrHaunch->m_dHaunchY_Top &&
		m_bUseHaunchMaterial == pUrHaunch->m_bUseHaunchMaterial &&
		m_bUseBackFillFormwork == pUrHaunch->m_bUseBackFillFormwork &&
		m_nHaunchMaterialID == pUrHaunch->m_nHaunchMaterialID &&
		m_IsUsedStoryMaterial == pUrHaunch->m_IsUsedStoryMaterial)
		return true;
	
	return false;
}