#include "StdAfx.h"
#include "MTDisplayRWallMHelper.h"

#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSCompoM.h"
#include "MSRcRWallM.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

MTDisplayRWallMHelper::MTDisplayRWallMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplayRWallMHelper::~MTDisplayRWallMHelper(void)
{
}

bool MTDisplayRWallMHelper::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	int NumBox = pSection->mp_Table->ma_Box.GetSize();
	if(NumBox == 0)	return false;
	int NumCompoM = pSection->ma_CompoM.size();
	if(NumCompoM == 0)	return false;

	SRSectionTable* pTable = pSection->mp_Table;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	MSCompoM* pCompoM = pSection->ma_CompoM[0];
	CString sName = pCompoM->m_Name;
	MSRcRWallM* pRebarM = (MSRcRWallM*)pCompoM->GetRebarM();

	if(!MakeRecognizeData(pSectionBox->mp_Name, sName, DataArr))
		MakeRecognizeData(pTable->mp_Name, sName, DataArr);

	MSFloor* pFromFloor = mp_Bldg->GetFloor(pCompoM->m_nFromStory);
	MSFloor* pToFloor = mp_Bldg->GetFloor(pCompoM->m_nToStory);
	if(pFromFloor == NULL || pToFloor == NULL)	return false;
	CString sStory;

	sStory.Format(_T("%s~%s"), pFromFloor->GetName(), pToFloor->GetName());
	int storyID = mp_Reader->FindTextFigureID(pSectionBox->mp_Story);
	MakeRecognizeData(storyID, sStory, DataArr);

	CString sThick;	 sThick.Format(_T("%ld"), (long)pCompoM->GetThick());
	MakeRecognizeData(mp_Reader->FindTextFigureID(pSectionBox->mp_Size), sThick, DataArr);

	CString rebarText, addedBarText;
	// Vertical Exterior
	rebarText = GetStirrupBarString(&pRebarM->m_VOutMidBar);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctVExt_MainBar), rebarText, DataArr);
	rebarText.Format(_T("%s-%ld"), GetStirrupBarString(&pRebarM->m_VOutTopBar), pRebarM->m_nVTopBarLength);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctVExt_TopBar), rebarText, DataArr);
	rebarText.Format(_T("%s-%ld"), GetStirrupBarString(&pRebarM->m_VOutBotBar), pRebarM->m_nVBotBarLength);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctVExt_BotBar), rebarText, DataArr);

	// Vertical Interior
	rebarText = GetStirrupBarString(&pRebarM->m_VInMidBar);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctVInt_MainBar), rebarText, DataArr);
	rebarText.Format(_T("%s-%ld"), GetStirrupBarString(&pRebarM->m_VInTopBar), pRebarM->m_nVInTopBarLength);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctVInt_TopBar), rebarText, DataArr);
	rebarText.Format(_T("%s-%ld"), GetStirrupBarString(&pRebarM->m_VInBotBar), pRebarM->m_nVInBotBarLength);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctVInt_BotBar), rebarText, DataArr);

	// Horizontal Exterior
	rebarText = GetStirrupBarString(&pRebarM->m_HOutTopBar);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctHExt_TopBar), rebarText, DataArr);
	rebarText = GetStirrupBarString(&pRebarM->m_HOutMidBar);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctHExt_MidBar), rebarText, DataArr);
	rebarText = GetStirrupBarString(&pRebarM->m_HOutBotBar);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctHExt_BotBar), rebarText, DataArr);

	// Horizontal Interior
	rebarText = GetStirrupBarString(&pRebarM->m_HInTopBar);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctHInt_TopBar), rebarText, DataArr);
	rebarText = GetStirrupBarString(&pRebarM->m_HInMidBar);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctHInt_MidBar), rebarText, DataArr);
	rebarText = GetStirrupBarString(&pRebarM->m_HInBotBar);
	MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctHInt_BotBar), rebarText, DataArr);

	// Shear Top-Bot
	if(pRebarM->m_bUseTopStirrup)
	{
		rebarText.Format(_T("%s-%ld"), GetShearBarString(&pRebarM->m_HTopStirrup, &pRebarM->m_VTopStirrup), (long)pRebarM->m_dTopSRegion);
		MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctShear_TopBar), rebarText, DataArr);
	}
	if(pRebarM->m_bUseBotStirrup)
	{
		rebarText.Format(_T("%s-%ld"), GetShearBarString(&pRebarM->m_HBotStirrup, &pRebarM->m_VBotStirrup), (long)pRebarM->m_dBotSRegion);
		MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctShear_BotBar), rebarText, DataArr);
	}

	// Shear Start-End
	if(pRebarM->m_bStartShearAddBar)
	{
		rebarText.Format(_T("%s-%ld"), GetShearBarString(&pRebarM->m_StartShearHBars, &pRebarM->m_StartShearVBars), pRebarM->m_nStartShearAddBarLength);
		MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctShear_StartBar), rebarText, DataArr);
	}

	if(pRebarM->m_bEndShearAddBar)
	{
		rebarText.Format(_T("%s-%ld"), GetShearBarString(&pRebarM->m_EndShearHBars, &pRebarM->m_EndShearVBars), pRebarM->m_nEndShearAddBarLength);
		MakeRecognizeData(pTable->GetFirstTextToLayerName(MTReader::ctShear_EndBar), rebarText, DataArr);
	}
	return true;
}
