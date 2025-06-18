#pragma once
#include "afx.h"

class SRSectionTable;
class SRDimension;
class GMText;
class StructureModel_CLASS MTReaderLayerHelper :
	public CObject
{
public:
	MTReaderLayerHelper();
	~MTReaderLayerHelper(void);

	void SetTable(SRSectionTable* pTable)		{ mp_Table = pTable;};
	void Add(int cellType, GMText* pText);
	void Add(int cellType, SRDimension* pDimension);

	vector<GMText*>			 FindTexts(int cellType);
	vector<SRDimension*> FindDimensions(int cellType);
private:
	SRSectionTable* mp_Table;
	map<int , vector<GMText*>>				mm_CellTypeToGMTexts;
	map<int, vector<SRDimension*>>	mm_CellTypeToDimensions;
};

