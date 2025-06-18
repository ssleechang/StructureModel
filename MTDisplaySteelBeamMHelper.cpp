#include "StdAfx.h"
#include "MTDisplaySteelBeamMHelper.h"

#include "MSCompoM.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "MSSteelBeamM.h"
#include "MSSteelSection.h"

MTDisplaySteelBeamMHelper::MTDisplaySteelBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplaySteelBeamMHelper::~MTDisplaySteelBeamMHelper(void)
{
}

bool MTDisplaySteelBeamMHelper::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	int NumBox = pSection->mp_Table->ma_Box.GetSize();
	if(NumBox == 0)	return false;
	int NumCompoM = pSection->ma_CompoM.size();
	if(NumCompoM == 0)	return false;

	SRSectionTable* pTable = pSection->mp_Table;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	MSCompoM* pCompoM = pSection->ma_CompoM[0];
	map<SRSteelInfo*, MSCompoM*>::iterator it;
	for(it = pSection->mm_SteelInfoToCompoM.begin(); it != pSection->mm_SteelInfoToCompoM.end(); it++)
	{
		SRSteelInfo* pInfo = it->first;
		MSSteelBeamM* pCompoM = (MSSteelBeamM*)it->second;
		CString sName = GetName(pSection->ma_CompoM);

		auto NameIDs = pInfo->GetTextFigureIDs(MTReader::ctName);
		auto SectionIDs = pInfo->GetTextFigureIDs(MTReader::ctSectionName);
		MakeRecognizeData(NameIDs, sName, DataArr);
		MakeRecognizeData(SectionIDs, pInfo->m_SectionName, DataArr);

		// 		int StudBoltID = pInfo->GetTextFigureID(MTReader::ctStudBolt);
		// 		if(StudBoltID != 0)
		// 			MakeRecognizeData(StudBoltID, pCompoM->m->m_StudBolt, DataArr);
		auto MaterialIDs = pInfo->GetTextFigureIDs(MTReader::ctMaterial);
		CString MaterialName = USED_STORY_MATERIAL;
		if(!pCompoM->GetIsUsedStoryMaterial())
		{
			MSMaterial* pMaterial = mp_Bldg->GetMaterial(pInfo->m_nMaterialID);
			if(pMaterial == NULL)	continue;
			MaterialName = pMaterial->m_Name;
		}
		MakeRecognizeData(MaterialIDs, MaterialName, DataArr);
	}
	return true;
}