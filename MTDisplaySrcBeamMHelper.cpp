#include "StdAfx.h"
#include "MTDisplaySrcBeamMHelper.h"


#include "MSCompoM.h"
#include "MSSrcBeamM.h"
#include "MDSrcBeamM.h"
#include "MSSteelSection.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

MTDisplaySrcBeamMHelper::MTDisplaySrcBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplaySrcBeamMHelper::~MTDisplaySrcBeamMHelper(void)
{
}

bool MTDisplaySrcBeamMHelper::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	int NumBox = pSection->mp_Table->ma_Box.GetSize();
	if(NumBox == 0)	return false;
	int NumCompoM = pSection->ma_CompoM.size();
	if(NumCompoM == 0)	return false;

	SRSectionTable* pTable = pSection->mp_Table;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	MSSrcBeamM* pCompoM = (MSSrcBeamM*)pSection->ma_CompoM[0];
	MDSrcBeamM* pRebarM = (MDSrcBeamM*)pCompoM->GetRebarM();

	CString sName, sSize;
	sName = GetName(pSection->ma_CompoM);

	if(!MakeRecognizeData(pSectionBox->mp_Name, sName, DataArr))
		MakeRecognizeData(pTable->mp_Name, sName, DataArr);
	sSize.Format(_T("%.0fX%.0f"), pCompoM->GetWidth(), pCompoM->GetDepth());
	MakeRecognizeData(pSectionBox->mp_Size, sSize, DataArr);

	auto figureIDs = pSectionBox->GetTextFigureIDs(MTReader::ctSectionName);
	MakeRecognizeData(figureIDs, pSectionBox->m_SteelSectionName, DataArr);

	MakeSectionDatas(pCompoM, pTable, DataArr);

	MakeSectionRect(pSectionBox);
	return true;
}

void MTDisplaySrcBeamMHelper::MakeSectionDatas(MSCompoM* pCompoM, SRSectionTable* pTable, vector<MTRecognizeData*>& DataArr)
{
	MDSrcBeamM* pRebarM = (MDSrcBeamM*)pCompoM->GetRebarM();
	int NumBox = pTable->ma_Box.GetSize();
	for(int iBox = 0; iBox < NumBox; iBox++)
	{
		SRSectionBox* pSectionBox = pTable->ma_Box[iBox];
		MSRcBeamSec* pBeamSec = GetBeamSection(pRebarM, NumBox, iBox);
		if(pBeamSec == NULL)	continue;

		MakeSectionDatas(pBeamSec, pSectionBox, DataArr);
	}
}

MSRcBeamSec* MTDisplaySrcBeamMHelper::GetBeamSection(MDSrcBeamM* pRebarM, int nSectionCount, int SectionIdx)
{
	if(nSectionCount == 1)		return pRebarM->mp_MidSec;
	else if(nSectionCount == 2)
	{
		if(SectionIdx == 0)	return pRebarM->mp_LargeSec;
		else                             return pRebarM->mp_MidSec;
	}
	else if(nSectionCount == 3)
	{
		if(mp_Reader->m_eBeamSectionType == MTReader::Small_Mid_Large)
		{
			if(SectionIdx==0)				return pRebarM->mp_SmallSec;
			else if(SectionIdx ==1)		return pRebarM->mp_MidSec;
			else											return pRebarM->mp_LargeSec;
		}
		else
		{
			if(SectionIdx==0)				return pRebarM->mp_LargeSec;
			else if(SectionIdx==1)		return pRebarM->mp_MidSec;
			else											return pRebarM->mp_SmallSec;
		}
	}

	return NULL;
}


void MTDisplaySrcBeamMHelper::MakeSectionDatas(MSRcBeamSec* pBeamSec, SRSectionBox* pSectionBox, vector<MTRecognizeData*>& DataArr)
{
	CString sTopBar = GetMainBarSetString(&pBeamSec->m_TBars);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctTBar), sTopBar, DataArr);
	CString sBotBar = GetMainBarSetString(&pBeamSec->m_BBars);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctBBar), sBotBar, DataArr);

	CString sStirrupBar = GetStirrupBarString(&pBeamSec->m_SBars);
	CString sSBar;	sSBar.Format(_T("%ld-%s"), pBeamSec->m_SBarNum, sStirrupBar);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctStirrup), sSBar, DataArr);

	CString sSideBar;
	if(pBeamSec->m_bIsNumPlaceSideBar)
		sSideBar = GetMainBarString(&pBeamSec->m_SideBars);
	else
		sSideBar = GetStirrupBarString(&pBeamSec->m_SideStirrupBar);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctSBar2), sSideBar, DataArr);

	CString sAddedBar;
	CString sDiaName = pBeamSec->m_SubMainBar.m_MainBar.GetBarDiameterName();
	sAddedBar.Format(_T("%ld-%s%s"), pBeamSec->m_SubMainBar.ma_Loc.size(),MSDeformedBar::m_sDefaultBarType, sDiaName);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctSubMainBar), sAddedBar, DataArr);

}

