#include "StdAfx.h"
#include "MTDisplayButtressMHelper.h"

#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSCompoM.h"
#include "MDRcButtressM.h"
#include "MDRcButtressSec.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"
#include "MSMainBarSet.h"

MTDisplayButtressMHelper::MTDisplayButtressMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplayButtressMHelper::~MTDisplayButtressMHelper(void)
{
}

bool MTDisplayButtressMHelper::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	int NumBox = pSection->mp_Table->ma_Box.GetSize();
	if(NumBox == 0)	return false;
	int NumCompoM = pSection->ma_CompoM.size();
	if(NumCompoM == 0)	return false;

	SRSectionTable* pTable = pSection->mp_Table;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	MSCompoM* pCompoM = pSection->ma_CompoM[0];
	MDRcButtressM* pRebarM = (MDRcButtressM*)pCompoM->GetRebarM();

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

	MakeSectionDatas(pCompoM, pTable, DataArr);

	return true;
}

void MTDisplayButtressMHelper::MakeSectionDatas(MSCompoM* pCompoM, SRSectionTable* pTable, vector<MTRecognizeData*>& DataArr)
{
	MDRcButtressM* pRebarM = (MDRcButtressM*)pCompoM->GetRebarM();
	int NumBox = pTable->ma_Box.GetSize();
	for(int iBox = 0; iBox < NumBox; iBox++)
	{
		SRSectionBox* pSectionBox = pTable->ma_Box[iBox];
		MDRcButtressSec* pButtressSec = GetButtressSection(pRebarM, NumBox, iBox);
		if(pButtressSec == NULL)	continue;

		MakeSectionDatas(pButtressSec, pSectionBox, DataArr);
	}
}

MDRcButtressSec* MTDisplayButtressMHelper::GetButtressSection(MDRcButtressM* pRebarM, int nSectionCount, int SectionIdx)
{
	if(nSectionCount == 1)		return pRebarM->mp_CenSec;
	else if(nSectionCount == 2)
	{
		if(SectionIdx == 0)	return pRebarM->mp_TopSec;
		else                             return pRebarM->mp_CenSec;
	}
	else if(nSectionCount == 3)
	{
		if(mp_Reader->m_eBeamSectionType == MTReader::Small_Mid_Large)
		{
			if(SectionIdx==0)				return pRebarM->mp_TopSec;
			else if(SectionIdx ==1)		return pRebarM->mp_CenSec;
			else											return pRebarM->mp_BotSec;
		}
		else
		{
			if(SectionIdx==0)				return pRebarM->mp_BotSec;
			else if(SectionIdx==1)		return pRebarM->mp_CenSec;
			else											return pRebarM->mp_TopSec;
		}
	}

	return NULL;
}

void MTDisplayButtressMHelper::MakeSectionDatas(MDRcButtressSec* pButtressSec, SRSectionBox* pSectionBox, vector<MTRecognizeData*>& DataArr)
{
	CString sTopBar = GetMainBarSetString(&pButtressSec->m_TBars);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctTBar), sTopBar, DataArr);
	CString sBotBar = GetMainBarSetString(&pButtressSec->m_BBars);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctBBar), sBotBar, DataArr);

	CString sStirrupBar = GetStirrupBarString(&pButtressSec->m_SBars);
	CString sSBar;	sSBar.Format(_T("%ld-%s"), pButtressSec->m_SBarNum, sStirrupBar);
	MakeRecognizeData(pSectionBox->GetGMText(MTReader::ctStirrup), sSBar, DataArr);
}
