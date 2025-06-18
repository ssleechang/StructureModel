#pragma once
#include "msopeningm.h"
class StructureModel_CLASS MSDoorM :public MSOpeningM
{
public:
	MSDoorM(void);
	~MSDoorM(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSDoorM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual bool IsGeometryDifferent(MSCompoM* compoM);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual double GetHeightBySlabLevel(double dSlabLevel);

	bool GetIsOpenToWallBottom()													{ return m_IsOpenToWallBottom;};
	void SetIsOpenToWallBottom(bool isOpenToSlabLevel)		{ m_IsOpenToWallBottom = isOpenToSlabLevel;};
protected:
	bool m_IsOpenToWallBottom;
};

