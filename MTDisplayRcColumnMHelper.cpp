#include "StdAfx.h"
#include "MTDisplayRcColumnMHelper.h"

#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSCompoM.h"
#include "MSRcLineMemberM.h"
#include "MSRcColumnM.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

MTDisplayRcColumnMHelper::MTDisplayRcColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplayRcColumnMHelper::~MTDisplayRcColumnMHelper(void)
{
}

bool MTDisplayRcColumnMHelper::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	int NumBox = pSection->mp_Table->ma_Box.GetSize();
	if(NumBox == 0)	return false;
	int NumCompoM = pSection->ma_CompoM.size();
	if(NumCompoM == 0)	return false;

	SRSectionTable* pTable = pSection->mp_Table;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	MSRcLineMemberM* pCompoM = (MSRcLineMemberM*)pSection->ma_CompoM[0];
	MSRcColumnM* pRebarM = (MSRcColumnM*)pCompoM->GetRebarM();

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
	if(pCompoM->GetShape() == MSRcLineMemberM::CIRCLE)
		TotalBars = NumTBars;
	CString sMainBar;
	sMainBar.Format(_T("%ld-%s%s"), TotalBars,MSDeformedBar::m_sDefaultBarType, sMainDiaName);

	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctMainBar), sMainBar, DataArr);
	CString sCenStrpBar = GetStirrupBarString(&pRebarM->m_CenHoopBar);
	CString sEndStrpBar = GetStirrupBarString(&pRebarM->m_EndHoopBar);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctSBar1), sCenStrpBar, DataArr);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctSBar2), sEndStrpBar, DataArr);

	CString sAddedBar;
	CString sAddBarDiaName = pRebarM->m_SubMainBar.m_MainBar.GetBarDiameterName();
	sAddedBar.Format(_T("%ld-%s%s"), pRebarM->m_SubMainBar.ma_Loc.size(), MSDeformedBar::m_sDefaultBarType,sAddBarDiaName);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctSubMainBar), sAddedBar, DataArr);

	MakeSectionRect(pSectionBox);
	return true;
}