#include "StdAfx.h"
#include "MTSteelBeamMHelper.h"

#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "StructureModel/MSSteelUtility.h"
#include "StructureModel/MSSteelBeamM.h"
#include "StructureModel/MSSteelSection.h"

MTSteelBeamMHelper::MTSteelBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTRcBeamMHelper(pBldg, pReader)
{
}


MTSteelBeamMHelper::~MTSteelBeamMHelper(void)
{
}

MSCompoM* MTSteelBeamMHelper::MakeData(SRSectionTable* pTable, SRSteelInfo* pSteelInfo)
{
	mp_ResultElement->SetArea(pTable->mp_Rect);

	CheckName(pSteelInfo->m_Name, pSteelInfo->GetTextFigureIDs(MTReader::ctName));
	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msSteelBeamM);
	MSSteelBeamM* pSteelM = (MSSteelBeamM*)mp_CompoM;
	pSteelM->m_Name = pSteelInfo->m_Name;

// 	MSSteelUtility* pUtil = mp_Bldg->GetSteelUtili();
// 	long nSectionType = pUtil->GetSectionType(pSteelInfo->m_SectionName);
// 	MSSteelSection* pSteelSection = pUtil->GetSection(nSectionType, pSteelInfo->m_SectionName);
// 	CString steelSectionName = pSteelInfo->m_SectionName;
// 	if(pSteelSection == NULL)
// 	{
// 		vector<double> params = Parsing_SteelName(pSteelInfo->m_SectionName);
// 		pSteelSection = pUtil->BuiltUpSteelSection(nSectionType, params);
// 		if(pSteelSection != NULL)	
// 			steelSectionName = pSteelSection->GetName();
// 
// 		mp_Reader->m_IsMakeBuiltUp = true;
// 	}
// 	else
// 		steelSectionName = pSteelSection->GetName();
// 
// 	pSteelM->m_pSection = pSteelSection;

	AddResultItem(MTResultItem::SteelSection, pSteelInfo->m_SectionName);

	MSMaterial* pMaterial = mp_Bldg->GetMaterial(pSteelInfo->m_nMaterialID);
	if(pMaterial == NULL)	pMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msSteel);
	pSteelM->SetMaterial(pMaterial);
	pSteelM->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);
	pSteelM->m_nFromStory = GetFromStory();
	pSteelM->m_nToStory = GetToStory();

	return mp_CompoM;
}

