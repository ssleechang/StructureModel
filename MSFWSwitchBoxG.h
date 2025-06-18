#pragma once
#include "mswallopeningg.h"
class StructureModel_CLASS MSFWSwitchBoxG : public MSWallOpeningG
{
public:
	MSFWSwitchBoxG();
	~MSFWSwitchBoxG();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWSwitchBoxG)

	virtual Type_MSCompoG GetType() { return msFWSwitchBoxG; };
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);
	virtual bool MakeBoundary();

	void SetIsLeftOwnerCurve(bool value);
	bool GetIsLeftOwnerCurve();

private:
	bool m_IsLeftOwnerCurve;
};

