#pragma once
#include "MSWallMemberG.h"
class StructureModel_CLASS MSSlantWallMemberG :
	public MSWallMemberG
{
public:
	MSSlantWallMemberG();
	~MSSlantWallMemberG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSlantWallMemberG)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSSlantWallMemberG* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);

	GM2DVector m_SVecUp, m_EVecUp;
};

