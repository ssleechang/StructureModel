#pragma once
#include "MSObject.h"
class StructureModel_CLASS MSBuildingUCS :public MSObject
{
public:
	MSBuildingUCS();
	~MSBuildingUCS();

	DECLARE_SERIAL(MSBuildingUCS)
	void Serialize(CArchive& archive);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	GM2DVector GetOriginPoint() { return m_Origin; };
	GM2DVector GetXDir() { return m_xDir; };
	GM2DVector GetYDir() { return m_yDir; };
	double GetUCSz() { return m_UCSz; };

	void SetUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir);
	void SetUCSz(double z);
private:
	GM2DVector m_Origin;
	GM2DVector m_xDir;
	GM2DVector m_yDir;
	double m_UCSz;
};

