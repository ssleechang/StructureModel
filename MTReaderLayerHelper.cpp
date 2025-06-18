#include "StdAfx.h"
#include "MTReaderLayerHelper.h"


MTReaderLayerHelper::MTReaderLayerHelper()
{
	mp_Table = NULL;
}

MTReaderLayerHelper::~MTReaderLayerHelper(void)
{
}

void MTReaderLayerHelper::Add(int cellType, GMText* pText)
{
	map<int, vector<GMText*>>::iterator it = mm_CellTypeToGMTexts.find(cellType);
	if(it != mm_CellTypeToGMTexts.end())
		it->second.push_back(pText);
	else
	{
		vector<GMText*> textArr;	
		textArr.push_back(pText);
		mm_CellTypeToGMTexts.insert(make_pair(cellType, textArr));
	}
}

void MTReaderLayerHelper::Add(int cellType, SRDimension* pDimension)
{
	map<int, vector<SRDimension*>>::iterator it = mm_CellTypeToDimensions.find(cellType);
	if(it != mm_CellTypeToDimensions.end())
		it->second.push_back(pDimension);
	else
	{
		vector<SRDimension*> dimensionArr;	dimensionArr.push_back(pDimension);
		mm_CellTypeToDimensions.insert(make_pair(cellType, dimensionArr));
	}
}

vector<GMText*> MTReaderLayerHelper::FindTexts(int cellType)
{
	vector<GMText*> textArr;
	map<int, vector<GMText*>>::iterator it = mm_CellTypeToGMTexts.find(cellType);
	if(it != mm_CellTypeToGMTexts.end())
		return it->second;
	return textArr;
}

vector<SRDimension*> MTReaderLayerHelper::FindDimensions(int cellType)
{
	vector<SRDimension*> dimensionArr;
	map<int, vector<SRDimension*>>::iterator it = mm_CellTypeToDimensions.find(cellType);
	if(it != mm_CellTypeToDimensions.end())
		return it->second;
	return dimensionArr;
}
