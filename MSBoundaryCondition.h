#pragma once
#include "msobject.h"
class StructureModel_CLASS MSBoundaryCondition : public MSObject
{
public:
	MSBoundaryCondition(void);
	~MSBoundaryCondition(void);

	enum Type_Condition {msShear=0, msMoment, msAngle, msBeamStifferner};

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSBoundaryCondition)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	Type_Condition GetCondition()								{ return m_Condition;}				
	void SetCondition(Type_Condition condition)	{ m_Condition = condition;};
protected:
	Type_Condition m_Condition;
};

