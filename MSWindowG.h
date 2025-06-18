#pragma once
#include "MSWallOpeningG.h"

class StructureModel_CLASS MSWindowG : public MSWallOpeningG
{
public:
	MSWindowG(void);
	~MSWindowG(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSWindowG)

	virtual Type_MSCompoG GetType(){return msWindowG;};
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msWindowM; };
};

