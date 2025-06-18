#include "StdAfx.h"
#include "MTDisplaySlabMHelper.h"

#include "MSCompoM.h"
#include "MSRcSlabM.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

MTDisplaySlabMHelper::MTDisplaySlabMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplaySlabMHelper::~MTDisplaySlabMHelper(void)
{
}

bool MTDisplaySlabMHelper::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	int NumBox = pSection->mp_Table->ma_Box.GetSize();
	if(NumBox == 0)	return false;
	int NumCompoM = pSection->ma_CompoM.size();
	if(NumCompoM == 0)	return false;

	SRSectionTable* pTable = pSection->mp_Table;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	MSCompoM* pCompoM = pSection->ma_CompoM[0];
	CString sName = GetName(pSection->ma_CompoM);

	if(!MakeRecognizeData(pSectionBox->mp_Name, sName, DataArr))
		MakeRecognizeData(pTable->mp_Name, sName, DataArr);

	map<SRSlabInfo*, MSCompoM*>::iterator it;
	for(it = pSection->mm_SlabInfoToCompoM.begin(); it != pSection->mm_SlabInfoToCompoM.end(); it++)
	{
		SRSlabInfo* pInfo = it->first;
		MSCompoM* pCompoM = it->second;
		MSRcSlabM* pRebarM = (MSRcSlabM*)pCompoM->GetRebarM();

		CString sThick;		sThick.Format(_T("%ld"), (long)pCompoM->GetThick());
		auto thickIDs = pInfo->GetTextFigureIDs(MTReader::ctThick);
		MakeRecognizeData(thickIDs, sThick, DataArr);

		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx1), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar1), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx2), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar2), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx3), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar3), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx4), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar4), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx5), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar5), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx6), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar6), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx7), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar7), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx8), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar8), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx9), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar9), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLx10), GetStirrupBarString(&pRebarM->m_ShortStrip.m_Bar10), DataArr);

		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy1), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar1), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy2), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar2), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy3), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar3), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy4), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar4), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy5), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar5), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy6), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar6), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy7), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar7), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy8), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar8), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy9), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar9), DataArr);
		MakeRecognizeData(pInfo->GetTextFigureIDs(MTReader::ctSlabLy10), GetStirrupBarString(&pRebarM->m_LongStrip.m_Bar10), DataArr);
	}

	return true;
}
