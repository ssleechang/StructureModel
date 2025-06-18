#pragma once
#include "msopeningm.h"
class StructureModel_CLASS MSWallOpeningExtraM : public MSOpeningM
{
public:
	MSWallOpeningExtraM(void);
	~MSWallOpeningExtraM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSWallOpeningExtraM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

};

