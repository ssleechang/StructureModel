#pragma once
#include "MSColumnG.h"

#include "GMLib/GMLine.h"
#include "MSBoundaryCondition.h"

class MSPitG;
class MSIsoFootG;
class MSLinkMemberG;
class MSSlabG;
class StructureModel_CLASS MSWallColumnG : public MSColumnG
{
public:
	MSWallColumnG();
	~MSWallColumnG(void);
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSWallColumnG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void Init(bool bInConstructor = false);

	virtual bool CheckData();

	virtual Type_MSCompoG GetType() { return msWallColumnG; };

protected:
	virtual void Serialize_EachCompoM_Rebar(CArchive& ar);
};
