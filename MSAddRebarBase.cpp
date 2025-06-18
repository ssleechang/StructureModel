#include "stdafx.h"
#include "MSAddRebarBase.h"

#include "MSRebarBuilding.h"
#include "MSCompoG.h"
#include "ReinforceBarSetting.h"
#include "MSAddRebarAreaLine.h"
#include "MSAddRebarAreaBent.h"
#include "MSAddRebarAreaShape.h"
IMPLEMENT_SERIAL(MSAddRebarBase, MSElement, VERSIONABLE_SCHEMA | 1)

MSAddRebarBase::MSAddRebarBase() : MSElement()
{
	m_pBuilding = nullptr;
	m_pOwnerCompoG = nullptr;

	m_pProfile = new GM2DPolyline();

	m_eAddBarType = eAddBarType::AddBar;
	m_eTopOffsetType = eOffsetType::Cover;
	m_TopUserOffset = 40;
	m_eBotOffsetType = eOffsetType::Cover;
	m_BotUserOffset = 40;

	m_IsUsedOwnerMaterial = true;
	m_MaterialID = 0;
	m_IsConstraint = false;
	m_PlacingRebarDegree = 0.0;

	m_IsMainAddBarType = false;
}


MSAddRebarBase::~MSAddRebarBase()
{
	delete m_pProfile;
}

MSAddRebarBase* MSAddRebarBase::CreateAddRebar(MSElement::Type_MSCompoG type)
{
	switch (type)
	{
	case MSElement::msAddRebarAreaLine:	return new MSAddRebarAreaLine();
	case MSElement::msAddRebarAreaBent:	return new MSAddRebarAreaBent();
	case MSElement::msAddRebarAreaShape: return new MSAddRebarAreaShape();
	}
	return nullptr;
}
void MSAddRebarBase::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSElement::CopyFromMe(pSource_in, bNewID);

	MSAddRebarBase* pSourceRebar = dynamic_cast<MSAddRebarBase*>(pSource_in);
	if (pSourceRebar == nullptr)	return;
	m_pBuilding = pSourceRebar->m_pBuilding;
	m_pOwnerCompoG = pSourceRebar->m_pOwnerCompoG;

	m_pProfile->CopyFrom(pSourceRebar->GetProfile());
	m_eAddBarType = pSourceRebar->GetAddBarType();
	m_eTopOffsetType = pSourceRebar->GetTopOffsetType();
	m_TopUserOffset = pSourceRebar->GetTopUserOffset();
	m_eBotOffsetType = pSourceRebar->GetBotOffsetType();
	m_BotUserOffset = pSourceRebar->GetBotUserOffset();

	m_IsUsedOwnerMaterial = pSourceRebar->GetIsUsedOwnerMaterial();
	m_MaterialID = pSourceRebar->m_MaterialID;
	m_IsConstraint = pSourceRebar->GetIsConstraint();
	m_PlacingRebarDegree = pSourceRebar->GetPlacingRebarDegree();

	m_IsMainAddBarType = pSourceRebar->GetIsMainAddBarType();
}

void MSAddRebarBase::Dump(EFS::EFS_Buffer& ar)
{
	MSElement::Dump(ar);
	ar << m_pOwnerCompoG;

	int addBarType = (int)GetAddBarType();
	int topOffsetType = (int)GetTopOffsetType();
	int botOffsetType = (int)GetBotOffsetType();
	ar << addBarType;
	ar << topOffsetType << m_TopUserOffset;
	ar << botOffsetType << m_BotUserOffset;

	ar << m_IsUsedOwnerMaterial;
	ar << m_MaterialID;
	ar << m_IsConstraint;
	ar << m_PlacingRebarDegree;

	ar << m_IsMainAddBarType;

	m_pProfile->Dump(ar);
}

void MSAddRebarBase::Recovery(EFS::EFS_Buffer& ar)
{
	MSElement::Recovery(ar);
	ar >> m_pOwnerCompoG;

	int addBarType = 0, topOffsetType = 0, botOffsetType = 0;
	ar >> addBarType;
	ar >> topOffsetType >> m_TopUserOffset;
	ar >> botOffsetType >> m_BotUserOffset;

	SetAddBarType((eAddBarType)addBarType);
	SetTopOffsetType((eOffsetType)topOffsetType);
	SetBotOffsetType((eOffsetType)botOffsetType);

	ar >> m_IsUsedOwnerMaterial;
	ar >> m_MaterialID;
	ar >> m_IsConstraint;
	ar >> m_PlacingRebarDegree;

	ar >> m_IsMainAddBarType;

	m_pProfile->Recovery(ar);
}

void MSAddRebarBase::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	int addBarType = 0, topOffsetType = 0, botOffsetType = 0;
	if (ar.IsStoring())
	{
		addBarType = (int)GetAddBarType();
		topOffsetType = (int)GetTopOffsetType();
		botOffsetType = (int)GetBotOffsetType();
		ar << addBarType;
		ar << topOffsetType;
		ar << m_TopUserOffset;
		ar << botOffsetType;
		ar << m_BotUserOffset;

		ar << m_IsUsedOwnerMaterial;
		ar << m_MaterialID;
		ar << m_IsConstraint;
		ar << m_PlacingRebarDegree;

		m_pProfile->Serialize(ar);

		ar << m_IsMainAddBarType;
	}
	else
	{
		ar >> addBarType;
		ar >> topOffsetType;
		ar >> m_TopUserOffset;
		ar >> botOffsetType;
		ar >> m_BotUserOffset;
		SetAddBarType((eAddBarType)addBarType);
		SetTopOffsetType((eOffsetType)topOffsetType);
		SetBotOffsetType((eOffsetType)botOffsetType);

		ar >> m_IsUsedOwnerMaterial;
		ar >> m_MaterialID;
		ar >> m_IsConstraint;
		ar >> m_PlacingRebarDegree;

		m_pProfile->Serialize(ar);

		if (MSVersionInfo::GetCurrentVersion() >= 20190425)
		{
			ar >> m_IsMainAddBarType;
		}
		double dTol = DTOL_MODELING;
		GM2DPolyline* pSlabPoly = GetProfile();
		pSlabPoly->DeleteDuplicatedPoints(dTol);
		pSlabPoly->MakeClosedPolyline(dTol);
		pSlabPoly->CheckAndFixFoldedEdges(dTol);
		pSlabPoly->CorrectiveBulges();
	}
}

void MSAddRebarBase::Translate(GM2DVector &pDelta)
{
	m_pProfile->Translate(pDelta);
}
void MSAddRebarBase::MirrorByAxis(GM2DLineVector& pAxis)
{
	m_pProfile->MirrorByAxis(pAxis);
	m_PlacingRebarDegree = m_PlacingRebarDegree + (pAxis.GetDegree() - m_PlacingRebarDegree) * 2;
}
void MSAddRebarBase::RotateZ(double theta, GM2DVector *pCenter)
{
	m_pProfile->RotateZ(theta, pCenter);
	m_PlacingRebarDegree += theta*Rad2Deg;
}

void MSAddRebarBase::SetProfile(GM2DPolyline* profile)
{
	m_pProfile->CopyFrom(profile);
}

int MSAddRebarBase::GetMaterialID()
{
	if (GetIsUsedOwnerMaterial())
		return m_pOwnerCompoG->GetMaterial()->m_ID;
	else
		return m_MaterialID;
}

MSMaterial* MSAddRebarBase::GetMaterial()
{
	if (GetIsUsedOwnerMaterial())
	{
		if (m_pOwnerCompoG == nullptr)
			return nullptr;
		return m_pOwnerCompoG->GetMaterial();
	}
	else
	{
		if (m_pBuilding == nullptr)	return nullptr;
		MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBuilding);
		if (pRebarBuilding == nullptr)	return nullptr;
		return pRebarBuilding->GetMaterial(m_MaterialID);
	}
}
bool MSAddRebarBase::isPlainConcrete()
{
	MSMaterial* pMat = GetMaterial();
	MSReinforceBarSetting* pReinforceBarSetting = pMat->GetReinforceBarSetting();
	if(pReinforceBarSetting)
	{
		if(pReinforceBarSetting->mm_IndexToReinforceBar.size() == 0)
			return true;
	}
	return false;
}
MSElement* MSAddRebarBase::GetOwnerCompoG()
{ return m_pOwnerCompoG; }

void MSAddRebarBase::SetOwnerCompoG(MSElement* pCompoG)
{
	m_pOwnerCompoG = dynamic_cast<MSCompoG*>(pCompoG);
	if(m_pOwnerCompoG != nullptr)
		SetBuilding(m_pOwnerCompoG->GetBuilding());
}