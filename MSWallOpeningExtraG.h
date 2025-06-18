#pragma once
#include "mswallopeningg.h"
class StructureModel_CLASS MSWallOpeningExtraG :
	public MSWallOpeningG
{
public:
	MSWallOpeningExtraG(void);
	~MSWallOpeningExtraG(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSWallOpeningExtraG)

	virtual Type_MSCompoG GetType(){return msWallOpeningExtraG;};
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
};

