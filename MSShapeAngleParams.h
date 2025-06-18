#pragma once
#include "MSObject.h"
class StructureModel_CLASS MSShapeAngleParams :public MSObject
{
public:
	MSShapeAngleParams();
	~MSShapeAngleParams();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSShapeAngleParams)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eAngleType { Angle90 = 0, Angle135, Angle180, User =10};

	eAngleType GetAngleType() { return m_eAngleType; };
	double GetUserAngle() { return m_UserAngle; };

	void SetAngleType(eAngleType angleType) { m_eAngleType = angleType; };
	void SetUserAngle(double degree) { m_UserAngle = degree; };
protected:
	eAngleType m_eAngleType;
	double m_UserAngle;
};

