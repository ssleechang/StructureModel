#include "StdAfx.h"
#include "MTDisplaySrcColumnMHelper.h"

#include "MSFloor.h"
#include "MSSrcColumnM.h"
#include "MDSrcColumnM.h"
#include "MSSteelSection.h"

#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

MTDisplaySrcColumnMHelper::MTDisplaySrcColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplaySrcColumnMHelper::~MTDisplaySrcColumnMHelper(void)
{
}

bool MTDisplaySrcColumnMHelper::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	int NumBox = pSection->mp_Table->ma_Box.GetSize();
	if(NumBox == 0)	return false;
	int NumCompoM = pSection->ma_CompoM.size();
	if(NumCompoM == 0)	return false;

	SRSectionTable* pTable = pSection->mp_Table;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	MSSrcColumnM* pCompoM = (MSSrcColumnM*)pSection->ma_CompoM[0];
	MDSrcColumnM* pRebarM = (MDSrcColumnM*)pCompoM->GetRebarM();

	MSFloor* pFromFloor = mp_Bldg->GetFloor(pCompoM->m_nFromStory);
	MSFloor* pToFloor = mp_Bldg->GetFloor(pCompoM->m_nToStory);
	if(pFromFloor == NULL || pToFloor == NULL)	return false;

	CString sName, sStory, sSize;
	sName = pCompoM->m_Name;
	sStory.Format(_T("%s~%s"), pFromFloor->GetName(), pToFloor->GetName());
	sSize.Format(_T("%.0fX%.0f"), pCompoM->GetWidth(), pCompoM->GetDepth());
	if(!MakeRecognizeData(pSectionBox->mp_Name, sName, DataArr))
		MakeRecognizeData(pTable->mp_Name, sName, DataArr);
	MakeRecognizeData(pSectionBox->mp_Story, sStory, DataArr);
	MakeRecognizeData(pSectionBox->mp_Size, sSize, DataArr);

	CString sMainDiaName = pRebarM->m_TBars.GetMaxDiaNameOnSet(0);
	int NumTBars = pRebarM->m_TBars.GetTotalBars();
	int NumLBars = pRebarM->m_LBars.GetTotalBars();

	int TotalBars = NumTBars*2 + NumLBars *2;
	CString sMainBar;
	sMainBar.Format(_T("%ld-%s%s"), TotalBars, MSDeformedBar::m_sDefaultBarType, sMainDiaName);

	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctMainBar), sMainBar, DataArr);
	CString sCenStrpBar = GetStirrupBarString(&pRebarM->m_CenHoopBar);
	CString sEndStrpBar = GetStirrupBarString(&pRebarM->m_EndHoopBar);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctSBar1), sCenStrpBar, DataArr);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctSBar2), sEndStrpBar, DataArr);

	CString sAddedBar;
	CString sAddedBarDiaName = pRebarM->m_SubMainBar.m_MainBar.GetBarDiameterName();
	sAddedBar.Format(_T("%ld-%s%s"), pRebarM->m_SubMainBar.ma_Loc.size(),MSDeformedBar::m_sDefaultBarType, sAddedBarDiaName);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctSubMainBar), sAddedBar, DataArr);

	auto figureIDs = pSectionBox->GetTextFigureIDs(MTReader::ctSectionName);
	MakeRecognizeData(figureIDs, pSectionBox->m_SteelSectionName, DataArr);

	MakeSectionRect(pSectionBox);
	return true;
}

