#pragma once
#include "MSObject.h"
class StructureModel_CLASS MSShapeLengthParams :public MSObject
{
public:
	MSShapeLengthParams();
	~MSShapeLengthParams();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSShapeLengthParams)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eParamType {	Hook = 0, UserHook, Constraint, UserConstaint };

	eParamType GetParamType() { return m_eParamType; };
	double GetUserLength() { return m_UserLength; };

	void SetParamType(eParamType type) { m_eParamType = type; };
	void SetUserLength(double length) { m_UserLength = length; };

protected:
	eParamType m_eParamType;
	double m_UserLength;
};

