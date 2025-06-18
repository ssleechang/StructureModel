#include "StdAfx.h"
#include "MTDisplayCompoMHelper.h"

#include "GMLib/GMText.h"

#include "MTReader.h"
#include "MSMainBar.h"
#include "MSStirrupBar.h"
#include "MSMainBarSet.h"
#include "SRSectionBox.h"
#include "MRRcSection.h"
#include "MREdge.h"

MTDisplayCompoMHelper::MTDisplayCompoMHelper(MSBaseBuilding* pBldg, MTReader* pReader)
{
	mp_Bldg = pBldg;
	mp_Reader = pReader;
}

MTDisplayCompoMHelper::~MTDisplayCompoMHelper(void)
{
}

bool MTDisplayCompoMHelper::MakeRecognizeData(GMText* pText, CString sData, vector<MTRecognizeData*>& DataArr)
{
	if(pText == NULL)	return false;
	int figueID = mp_Reader->FindTextFigureID(pText);
	if(figueID == 0)	return false;

	MTRecognizeData* pData = new MTRecognizeData;
	pData->m_sData = sData;
	pData->m_nFigureIDs.push_back(figueID);

	DataArr.push_back(pData);
	return true;
}

bool MTDisplayCompoMHelper::MakeRecognizeData(int figureId, CString sData, vector<MTRecognizeData*>& DataArr)
{
	vector<int> figureIds;
	figureIds.push_back(figureId);

	return MakeRecognizeData(figureIds, sData, DataArr);
}

bool MTDisplayCompoMHelper::MakeRecognizeData(vector<int> figureIDs, CString sData, vector<MTRecognizeData*>& DataArr)
{
	if(figureIDs.size() == 0)	return false;

	MTRecognizeData* pData = new MTRecognizeData;
	pData->m_sData = sData;
	pData->m_nFigureIDs = figureIDs;

	DataArr.push_back(pData);
	return true;
}

CString MTDisplayCompoMHelper::GetName(vector<MSCompoM*>& compoMs)
{
	CString sName;
	int NumCompoM = compoMs.size();
	for(int i = 0; i < NumCompoM; i++)
	{
		MSCompoM* pCompoM = compoMs[i];
		if(i + 1 == NumCompoM)
			sName += pCompoM->m_Name;
		else
			sName += pCompoM->m_Name + _T(", ");
	}
	return sName;
}

CString MTDisplayCompoMHelper::GetMainBarString(MSMainBar* pMainBar)
{
	int NumLayer = pMainBar->GetNumLayers();

	CString LayerData, BarData;
	for(int iLayer = 0; iLayer < NumLayer; iLayer++)
	{
		LayerData.Format(_T("%ld"), pMainBar->GetNumBarsOnALayer(iLayer));

		if(iLayer != NumLayer-1)
			BarData += LayerData + _T("/");
		else
			BarData += LayerData;
	}
	CString sDiaName = pMainBar->GetBarDiameterName();

	CString sMainBar;
	sMainBar.Format(_T("%s-%s%s"), BarData, MSDeformedBar::m_sDefaultBarType,sDiaName);
	return sMainBar;
}

CString MTDisplayCompoMHelper::GetMainBarSetString(MSMainBarSet* pMainBar)
{
	return pMainBar->GetBarString();
}

CString MTDisplayCompoMHelper::GetStirrupBarString(MSStirrupBar* pStirrupBar)
{
	return pStirrupBar->GetStirrupBarString();
}

CString MTDisplayCompoMHelper::GetShearBarString(MSStirrupBar* pHStirrupBar, MSStirrupBar* pVStirrupBar)
{
	if(pHStirrupBar == NULL || pVStirrupBar == NULL)	return _T("");

	CString sFirstDiaName = pHStirrupBar->GetFirstBarDiaName();
	long hSpacing = pHStirrupBar->GetSpacing();
	long vSpacing = pVStirrupBar->GetSpacing();

	CString sShearBar;
	sShearBar.Format(_T("%s%s@%ld@%ld"), MSDeformedBar::m_sDefaultBarType, sFirstDiaName, hSpacing, vSpacing);
	return sShearBar;
}

void MTDisplayCompoMHelper::MakeSectionRect(SRSectionBox* pSectionBox)
{
	if (pSectionBox->mp_Section != nullptr)
	{
		if (pSectionBox->mp_Section->ma_Edge.GetSize() == 4)
		{
			GM2DLineVector* bottomLine = dynamic_cast<GM2DLineVector*>(pSectionBox->mp_Section->ma_Edge[0]->mp_Curve);
			GM2DLineVector* rightLine = dynamic_cast<GM2DLineVector*>(pSectionBox->mp_Section->ma_Edge[1]->mp_Curve);
			if (bottomLine != nullptr && rightLine != nullptr)
			{
				GM2DVector originVec, cornerVec;
				if (bottomLine->m_P0.m_X < bottomLine->m_P1.m_X)
					originVec = bottomLine->m_P0;
				else
					originVec = bottomLine->m_P1;
				if (rightLine->m_P0.m_Y > rightLine->m_P1.m_Y)
					cornerVec = rightLine->m_P0;
				else
					cornerVec = rightLine->m_P1;
				GM2DRectangle* pSectionRect = new GM2DRectangle(originVec.m_X, originVec.m_Y, cornerVec.m_X, cornerVec.m_Y);
				mp_Reader->AddDisplaySectionRect(pSectionRect);
			}
		}
	}
}