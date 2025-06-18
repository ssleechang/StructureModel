#include "StdAfx.h"
#include "MSOpeningG.h"
#include "MSPolylineObject.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSCompoGUtil.h"

IMPLEMENT_SERIAL(MSOpeningG, MSCompoG, VERSIONABLE_SCHEMA | 1)
MSOpeningG::MSOpeningG(void)
{
	m_SLevel = 0.0;
	m_ELevel = 0.0;
	m_IsUseOffset = false;

	mp_PolylineInfo = new MSPolylineObject();
	mp_PolylineInfo->SetOwner(this);
}


MSOpeningG::~MSOpeningG(void)
{
}

MSObject* MSOpeningG::Copy( bool bNewID /*= true*/ )
{
	MSOpeningG* pNewObj = new MSOpeningG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSOpeningG::CopyFromMe(MSObject* pSource, bool bNewID/*=true*/)
{
	if(pSource==NULL)	return;
	MSCompoG::CopyFromMe(pSource, bNewID);

	MSOpeningG* pSourceG = (MSOpeningG*)pSource;

	m_SLevel = pSourceG->m_SLevel;
	m_ELevel = pSourceG->m_ELevel;
	m_IsUseOffset = pSourceG->m_IsUseOffset;

// 	m_Usage = pSourceM->m_Usage;
// 	m_CalcBy = pSourceM->m_CalcBy;
// 	m_Layer = pSourceM->m_Layer;
// 	m_IsReinforceBar1 = pSourceM->m_IsReinforceBar1;
// 	m_IsReinforceBar2 = pSourceM->m_IsReinforceBar2;
// 
// 	m_ReinforceBar1.CopyFromMe(&pSourceM->m_ReinforceBar1);
// 	m_ReinforceBar2.CopyFromMe(&pSourceM->m_ReinforceBar2);
}

void MSOpeningG::Dump( EFS::EFS_Buffer& buffer )
{
	MSCompoG::Dump(buffer);

 	buffer << m_SLevel << m_ELevel << m_IsUseOffset;
	mp_PolylineInfo->Dump(buffer);
}

void MSOpeningG::Recovery( EFS::EFS_Buffer& buffer )
{
	MSCompoG::Recovery(buffer);

 	buffer >> m_SLevel >> m_ELevel >> m_IsUseOffset;
	mp_PolylineInfo->Recovery(buffer);
}

void MSOpeningG::Serialize(CArchive &ar)
{
	MSCompoG::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_SLevel;
		ar << m_ELevel;
		ar << m_IsUseOffset;
		mp_PolylineInfo->Serialize(ar);
	}
	else
	{
		ar >> m_SLevel;
		ar >> m_ELevel;
		ar >> m_IsUseOffset;
		mp_PolylineInfo->Serialize(ar);
		mp_PolylineInfo->SetOwner(this);

		if(mp_CompoM == NULL)
		{
			auto building = dynamic_cast<MSRebarBuilding*>(GetBuilding());
			auto reinForceM = building->GetDefaultOpeningReinforceM();
			if (reinForceM == nullptr)
			{
				building->CreateDefaultOpeningReinforceM();
				reinForceM = building->GetDefaultOpeningReinforceM();
			}
			mp_CompoM = reinForceM;
		}
	}
}

double MSOpeningG::GetSLevel()
{
	return m_SLevel;
}
double MSOpeningG::GetELevel()
{
	return m_ELevel;
}
bool MSOpeningG::GetUseOffset()
{
	return m_IsUseOffset;
}

void MSOpeningG::SetSLevel(double level)
{
	m_SLevel = level;
}
void MSOpeningG::SetELevel(double level)
{
	m_ELevel = level;
}
void MSOpeningG::SetUseOffset(bool value)
{
	m_IsUseOffset = value;
}

bool MSOpeningG::VisibleInRect(GM2DRectangle* pRect)
{
	if(mp_PolylineInfo == NULL)
		return false;
	
	if(mp_PolylineInfo->VisibleInRect(pRect))	return true;

	return false;
}

void MSOpeningG::CalcBoundRectBeforeAddQuadTree()
{
	if(mp_PolylineInfo == NULL)
		return;

	mp_PolylineInfo->CalcBoundRectBeforeAddQuadTree();
}

bool MSOpeningG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	MSFloor* pFloor = GetFloor();
	if(pFloor == NULL)
		return false;

	BottomLevel = pFloor->GetSlabLevel();
	TopLevel = BottomLevel + pFloor->GetSlabHeight();

	if(m_IsUseOffset)
	{
		BottomLevel = TopLevel + GetSLevel();
		TopLevel += GetELevel();
	}

	return true;
}

vector<MSCompoG*> MSOpeningG::GetSlabGArr()
{
	mm_SlabGID.clear();
	vector<MSCompoG*> SlabGArr;

	MSFloor* pFloor = GetFloor();
	GM2DPolyline* pMyBoundary = GetProfile();
	double dTol = DTOL_GM;
	double openingTop = 0;
	double openingBot = 0;
	GetZLevels(openingTop, openingBot);

	vector<MSCompoG*> slabGs = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, FindType_MSElement::msSlabG, false, true, dTol);

	for(long iCompoG = 0 ; iCompoG < slabGs.size() ; iCompoG++)
	{
		MSCompoG* pCompoG = slabGs[iCompoG];
		if(pCompoG == this) 
			continue;
		if(m_IsUseOffset)
		{
			double slabTop = 0;
			double slabBot = 0;
			pCompoG->GetZLevels(slabTop, slabBot, false);

			if (openingBot >= slabTop)
				continue;
			if (openingTop <= slabBot)
				continue;
		}
		
		SlabGArr.push_back(pCompoG);
		mm_SlabGID.insert(make_pair(pCompoG->m_ID, pCompoG->m_ID));
	}

	return SlabGArr;
}

vector<MSCompoG*> MSOpeningG::GetPartnerCompoGs(long GetIdx)
{
	vector<MSCompoG*> CompoGArr = GetSlabGArr();

	MSFloor* pFloor = GetFloor();
	map<long, long>::iterator it;
	for(it = mm_SlabGID.begin() ; it != mm_SlabGID.end() ; it++)
	{
		MSCompoG* pCompoG = pFloor->GetCompoG(it->second);
		if (pCompoG != NULL)
			CompoGArr.push_back(pCompoG);
	}

	sort(CompoGArr.begin(), CompoGArr.end(), 
		[](const MSCompoG* a, const MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
	);

	vector<MSCompoG*>::iterator new_end;
	new_end = unique(CompoGArr.begin(), CompoGArr.end());
	CompoGArr.erase(new_end, CompoGArr.end());

	return CompoGArr;
}

GM2DPolyline* MSOpeningG::GetTrimmedBoundary(bool IsTop)
{
	return GetProfile();
}

MDMember* MSOpeningG::GetRebarM()
{
	vector<MSCompoG*> SlabArr = GetSlabGArr();

	if (SlabArr.size() > 0)
		return SlabArr[0]->GetRebarM();

	return NULL;
}

void MSOpeningG::InitializeParameter()
{
	MSFloor* pFloor = GetFloor();
	map<long, long>::iterator it;

	for(it = mm_SlabGID.begin() ; it != mm_SlabGID.end() ; it++)
	{
		MSCompoG* pCompoG = pFloor->GetCompoG(it->second);
		if (pCompoG != NULL)
			pCompoG->UpdateBoundaryVisibilities();
	}
	mm_SlabGID.clear();
}

bool MSOpeningG::MakeBoundary()
{
	mp_Boundary->ResetPolyline();
	GM2DPolyline* pProFile = GetProfile();
	if (pProFile == NULL)
		return false;

	mp_Boundary->CopyFrom(pProFile);
	if (mp_Boundary->ma_Point.GetSize() > 0)
		mp_Boundary->MakeCCW();
	return true;
}

void MSOpeningG::FixProfile()
{
	GM2DPolyline* pProFile = GetProfile();
	pProFile->DeleteInLinePoint(DTOL_MODELING);
	pProFile->MakeClosedPolyline(DTOL_MODELING);

	MakeBoundary();
}