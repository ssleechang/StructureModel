#include "StdAfx.h"
#include "MTSteelColumnMHelper.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "StructureModel/MSRebarBuilding.h"
#include "StructureModel/MSSteelUtility.h"
#include "StructureModel/MSSteelColumnM.h"
#include "StructureModel/MSSteelSection.h"

MTSteelColumnMHelper::MTSteelColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTRcColumnMHelper(pBldg, pReader)
{
}


MTSteelColumnMHelper::~MTSteelColumnMHelper(void)
{
}

MSCompoM* MTSteelColumnMHelper::MakeData(SRSectionTable* pTable, SRSteelInfo* pSteelInfo)
{
	mp_ResultElement->SetArea(pTable->mp_Rect);

	if (pTable->ma_Box.GetSize() == 0)	return NULL;
	mp_SectionBox = pTable->ma_Box[0];
	CheckName(pSteelInfo->m_Name, pSteelInfo->GetTextFigureIDs(MTReader::ctName));
	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msSteelColumnM);
	MSSteelColumnM* pSteelM = (MSSteelColumnM*)mp_CompoM;
	pSteelM->m_Name = pSteelInfo->m_Name; 
	// 	MSSteelUtility* pUtil = mp_Bldg->GetSteelUtili();
// 	long nSectionType = pUtil->GetSectionType(pSteelInfo->m_SectionName);
// 	MSSteelSection* pSteelSection = pUtil->GetSection(nSectionType, pSteelInfo->m_SectionName);
// 	CString steelSectionName = pSteelInfo->m_SectionName;
// 	if (pSteelSection == NULL)
// 	{
// 		vector<double> params = Parsing_SteelName(pSteelInfo->m_SectionName);
// 		pSteelSection = pUtil->BuiltUpSteelSection(nSectionType, params);
// 		if (pSteelSection != NULL)
// 			steelSectionName = pSteelSection->GetName();
// 
// 		mp_Reader->m_IsMakeBuiltUp = true;
// 	}
// 	else
// 		steelSectionName = pSteelSection->GetName();
//	pSteelM->m_pSection = pSteelSection;
	AddResultItem(MTResultItem::SteelSection, pSteelInfo->m_SectionName);

	MSMaterial* pMaterial = mp_Bldg->GetMaterial(pSteelInfo->m_nMaterialID);
	if(pMaterial == NULL)	pMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msSteel);
	pSteelM->SetMaterial(pMaterial);
	pSteelM->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);
	pSteelM->m_nFromStory = pTable->m_nFromStory;
	pSteelM->m_nToStory = pTable->m_nToStory;

	CheckStory();

	return pSteelM;
}

