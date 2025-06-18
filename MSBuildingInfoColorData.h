#pragma once
#include "MSObject.h"
#include "StructureModel/MSElement.h"

class StructureModel_CLASS MSBuildingInfoColorData : public MSObject
{
public:
	MSBuildingInfoColorData(void);
	MSBuildingInfoColorData(int alpha, int r, int g, int b, bool isShow);
	~MSBuildingInfoColorData(void);

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSBuildingInfoColorData)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	void GetColor(int& alpha, int& red, int& green, int& blue);
	void SetColor(int alpha, int red, int green, int blue);

	bool GetShow();
	void SetShow(bool bShow);

private:
	int m_Alpha;
	int m_R;
	int m_G;
	int m_B;
	bool m_bShow;
};

