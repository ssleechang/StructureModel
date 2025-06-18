#pragma once
#include "mslinememberg.h"

class StructureModel_CLASS MSBraceG :	public MSLineMemberG
{
public:
	MSBraceG();
	~MSBraceG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSBraceG)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSBraceG* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msBraceG;};

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	bool m_bTensionOnly;
	long m_Direction;     // 0 : X Brace, 1 : 1->2,  2 : 2->1

};
