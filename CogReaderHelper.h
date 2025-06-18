#pragma once
#include "afx.h"

class MSBaseBuilding;
class CogReader;
class MSElement;
class StructureModel_CLASS CogReaderHelper : public CObject
{
public:
	CogReaderHelper(MSBaseBuilding* pBldg, CogReader* pReader);
	~CogReaderHelper(void);

	void AddErrorObject(MSElement* pElement, long nErrorCode, long nLevel, CString strGroup, CString strMsg);

protected:
	MSBaseBuilding* mp_Bldg;
	CogReader* mp_Reader;
};

