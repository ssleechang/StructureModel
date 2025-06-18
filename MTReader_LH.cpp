#include "StdAfx.h"
#include "MTReader.h"

#include "GMLib\GMLib.h"
#include "GMLib\GMText.h"

#include "SRSectionTable.h"
#include "MTBWallMHelper_LH.h"

void MTReader::AddTable_LH(GM2DVector cenVec, double width, double height, int figureID)
{
	AddTable(cenVec.m_X, cenVec.m_Y, width, height, figureID);
}
void MTReader::AddText_LH(GM2DVector insertVec, CString text, GM2DVector originVec, GM2DVector cornerVec, int figureID)
{
	GMText* pText = new GMText();
	pText->m_Vec = insertVec;
	pText->m_Origin = originVec;
	pText->m_Corner = cornerVec;

	SRSectionTable* pTable = FindSectionTable(&insertVec, NULL);
	if (pTable)
	{
		pTable->ma_Text.Add(pText);
		mm_TextToFigureID.insert(make_pair(pText, figureID));
	}
	else
		delete pText;
}

GM2DRectangle* MTBWallMHelper_LH::GetCopyRect()
{
	auto pRect = GetResultArea();
	if (pRect != nullptr)
		return (GM2DRectangle*)pRect->Copy();
	return nullptr;
}

void MTReader::SortSRSectionTable()
{
	std::vector<SRSectionTable*> tempVec;
	for (int i = 0; i < ma_Table.GetSize(); ++i)
	{
		tempVec.push_back(ma_Table[i]);
	}

	std::sort(tempVec.begin(), tempVec.end(), [](SRSectionTable* a, SRSectionTable* b) {
		auto aRect = a->mp_Rect;
		auto bRect = b->mp_Rect;

		double aX = min(aRect->m_Origin.m_X, aRect->m_Corner.m_X);
		double aY = max(aRect->m_Origin.m_Y, aRect->m_Corner.m_Y); // 상단

		double bX = min(bRect->m_Origin.m_X, bRect->m_Corner.m_X);
		double bY = max(bRect->m_Origin.m_Y, bRect->m_Corner.m_Y);

		if (aX != bX)
			return aX < bX;
		return aY > bY;
		});

	for (int i = 0; i < tempVec.size(); ++i)
	{
		ma_Table[i] = tempVec[i];
	}
}
void MTReader::MakeWallM_LH()
{
	vector<MTBWallMHelper_LH*> helperList;
	SortSRSectionTable();
	for (int i = 0; i < ma_Table.GetSize(); i++)
	{
		SRSectionTable* pTable = ma_Table[i];

		MTBWallMHelper_LH* theHelper = new MTBWallMHelper_LH(mp_Bldg, this);
		theHelper->MakeData(pTable);
		helperList.push_back(theHelper);
	}

	map<CString, vector<GM2DRectangle*>> errorNameMap = CheckWallData_LH(helperList);

	MakeBWallMData_LH(helperList, errorNameMap);

	for (int i = 0; i < helperList.size(); i++)
		delete helperList[i];
	helperList.clear();
}

map<CString, vector<GM2DRectangle*>> MTReader::CheckWallData_LH(vector<MTBWallMHelper_LH*>& helperList)
{
	map<CString, vector<GM2DRectangle*>> errorNameMap;
	for (int i = 0; i < helperList.size(); i++)
	{
		MTBWallMHelper_LH* iHelper = helperList[i];
		//if (!iHelper->GetIsMaked())	continue;
		if (iHelper->GetIsDummy())	continue;
		auto it = errorNameMap.find(iHelper->GetName());
		if (it != errorNameMap.end())	continue;

		for (int j = i + 1; j < helperList.size(); j++)
		{
			MTBWallMHelper_LH* jHelper = helperList[j];
			//if (!jHelper->GetIsMaked())		continue;
			if (jHelper->GetIsDummy())	continue;
			if (iHelper->GetName() == jHelper->GetName())
			{
				if (iHelper->IsSameData(jHelper))
				{
//					iHelpter->SetIsMaked(false);
					jHelper->SetIsDummy(true);
				}
			}
		}
	}
	return errorNameMap;
}

void MTReader::MakeBWallMData_LH(vector<MTBWallMHelper_LH*>& helperList, map<CString, vector<GM2DRectangle*>>& errorNameMap)
{
	map<CString, GM2DRectangle*> makeNameMap;
	for (int iHelper = 0; iHelper < helperList.size(); iHelper++)
	{
		MTBWallMHelper_LH* pHelper = helperList[iHelper];
		auto itError = errorNameMap.find(pHelper->GetName());
		if (itError != errorNameMap.end())	continue;

		//auto itMaked = makeNameMap.find(pHelper->GetName());
		//if (itMaked != makeNameMap.end())	continue;

		//makeNameMap.insert(make_pair(pHelper->GetName(), pHelper->GetCopyRect()));
		vector<MSCompoM*> compoMs = pHelper->MakeBWallMs();
		//if (!pHelper->GetIsMaked())	continue;	// MakeBWallMs에서 생성실패

        for (int iCompoM = 0; iCompoM < compoMs.size(); iCompoM++)
            ma_NewM.Add(compoMs[iCompoM]);

		auto results = pHelper->GetResults();
		ma_ResultM.insert(ma_ResultM.end(), results.begin(), results.end());
		map<int, CString> figureIDMap = pHelper->GetFigureIDsMap();

		if (results.size() > 0)
		{
			MTResultElement* firstResult = results[0];
			GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pHelper->GetCopyRect());
			if (pRect != nullptr)
			{
				MTRecognizeSection* pRecSection = MakeRecognizeSection(pRect, pHelper->GetTable(), compoMs);
				pRecSection->AddLHWallInfoToCompoMMap(figureIDMap);
			}
		}
	}

	CString errorMessage;
	for (auto it = errorNameMap.begin(); it != errorNameMap.end(); it++)
	{
		errorMessage.Format(_T("%s"), it->first);
		for (int i = 0; i < it->second.size(); i++)
		{
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_SAMENAME_DIFF_DATA, errorMessage, it->second[i]);
		}
	}
}
