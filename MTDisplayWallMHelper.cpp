#include "StdAfx.h"
#include "MTDisplayWallMHelper.h"

#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSCompoM.h"
#include "MSRcBWallM.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

MTDisplayWallMHelper::MTDisplayWallMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplayWallMHelper::~MTDisplayWallMHelper(void)
{
}

bool MTDisplayWallMHelper::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	int NumBox = pSection->mp_Table->ma_Box.GetSize();
	if(NumBox == 0)	return false;
	int NumCompoM = pSection->ma_CompoM.size();
	if(NumCompoM == 0)	return false;

	SRSectionTable* pTable = pSection->mp_Table;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	MSCompoM* pCompoM = pSection->ma_CompoM[0];
	CString sName = pCompoM->m_Name;

	if(!MakeRecognizeData(pSectionBox->mp_Name, sName, DataArr))
		MakeRecognizeData(pTable->mp_Name, sName, DataArr);

	map<SRWallInfo*, MSCompoM*>::iterator it;
	for(it = pSection->mm_WallInfoToCompoM.begin(); it != pSection->mm_WallInfoToCompoM.end(); it++)
	{
		SRWallInfo* pInfo = it->first;
		MSCompoM* pCompoM = it->second;
		MSRcBWallM* pRebarM = (MSRcBWallM*)pCompoM->GetRebarM();

		MSFloor* pFromFloor = mp_Bldg->GetFloor(pCompoM->m_nFromStory);
		MSFloor* pToFloor = mp_Bldg->GetFloor(pCompoM->m_nToStory);
		if(pFromFloor == NULL || pToFloor == NULL)	continue;

		auto MaterialIDs = pInfo->GetTextFigureIDs(MTReader::ctMaterial);
		CString MaterialName = USED_STORY_MATERIAL;
		if(!pCompoM->GetIsUsedStoryMaterial())
		{
			MSMaterial* pMaterial = mp_Bldg->GetMaterial(pInfo->GetMaterialID());
			if(pMaterial == NULL)	continue;
			MaterialName = pMaterial->m_Name;
		}
		MakeRecognizeData(MaterialIDs, MaterialName, DataArr);

		CString sStory;
		sStory.Format(_T("%s~%s"), pFromFloor->GetName(), pToFloor->GetName());
		auto storyIDs = pInfo->GetTextFigureIDs(MTReader::ctStory);
		MakeRecognizeData(storyIDs, sStory, DataArr);

		CString sThick;	sThick.Format(_T("%ld"), (long)pCompoM->GetThick());
		auto thickIDs = pInfo->GetTextFigureIDs(MTReader::ctThick);
		MakeRecognizeData(thickIDs, sThick, DataArr);

		CString sHBar, sVBar;
		sHBar = GetStirrupBarString(&pRebarM->m_HBars);
		auto hBarIDs = pInfo->GetTextFigureIDs(MTReader::ctHBar);
		MakeRecognizeData(hBarIDs, sHBar, DataArr);

		if(pRebarM->m_bIsNumPlaceVBar)
			sVBar = GetMainBarString(&pRebarM->m_MainVBar);
		else
			sVBar = GetStirrupBarString(&pRebarM->m_VBars);
		auto vBarIDs = pInfo->GetTextFigureIDs(MTReader::ctVBar);
		MakeRecognizeData(vBarIDs, sVBar, DataArr);
	}	
	return true;
}
