#pragma once
#include "MSObject.h"

class MSShapeLengthParams;
class MSShapeAngleParams;
class StructureModel_CLASS MSShapeParams :public MSObject
{
public:
	MSShapeParams();
	~MSShapeParams();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSShapeParams)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	void Add(MSShapeLengthParams* lengthParam) { ma_LengthParam.push_back(lengthParam); };
	void Add(MSShapeAngleParams* angleParam) { ma_AngleParam.push_back(angleParam); };

	void DeleteParams();

	vector<MSShapeLengthParams*> GetLengthParams() { return ma_LengthParam; };
	vector<MSShapeAngleParams*> GetAngleParams() { return ma_AngleParam; };
	int GetGCode() { return m_GCode; };
	int GetSCode() { return m_SCode; };

	void SetGCode(int gCode) { m_GCode = gCode; };
	void SetSCode(int sCode) { m_SCode = sCode; };
protected:
	int m_GCode;
	int m_SCode;

	vector<MSShapeLengthParams*> ma_LengthParam;
	vector<MSShapeAngleParams*> ma_AngleParam;
};

