#pragma once
#include "MSVerticalLineMemberG.h"
#include "GMLib/GM2DVector.h"
#include "StructureModelEnum.h"

class StructureModel_CLASS MSFWSurfaceFoldingG : public MSVerticalLineMemberG
{
public:
	MSFWSurfaceFoldingG();
	~MSFWSurfaceFoldingG();
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWSurfaceFoldingG)
	virtual Type_MSCompoG GetType() { return msFWSurfaceFoldingG; };
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);
	virtual bool MakeBoundary();

	int m_FirstCompoGID;
	int m_SecondCompoGID;
	GM2DVector m_BasePnt;

	void MirrorByAxis(GM2DLineVector& pAxis);

	eSurfaceFoldingMType GetSurfaceFoldingMType();
	void SetSurfaceFoldingMType(eSurfaceFoldingMType type);
};