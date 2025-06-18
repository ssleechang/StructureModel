#pragma once
#include "MSWallOpeningG.h"

class StructureModel_CLASS MSDoorG : public MSWallOpeningG
{
public:
	MSDoorG(void);
	~MSDoorG(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSDoorG)

	virtual Type_MSCompoG GetType(){return msDoorG;};
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msDoorM; };
};

