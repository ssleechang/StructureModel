#pragma once
#include "mscompog.h"

class MSSlabG;
class StructureModel_CLASS MSOpeningG : public MSCompoG
{
public:
	MSOpeningG(void);
	~MSOpeningG(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSOpeningG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual Type_MSCompoG GetType(){return msOpeningG;};

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual bool VisibleInRect( GM2DRectangle* pRect);
	virtual bool GetZLevels( double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true  );

	virtual inline void CalcBoundRectBeforeAddQuadTree();
	
	virtual void InitializeParameter();

	virtual vector<MSCompoG*> GetPartnerCompoGs(long GetIdx = 0);
	virtual GM2DPolyline* GetTrimmedBoundary(bool IsTop = true);
	
	virtual MDMember* GetRebarM();
	
	virtual bool MakeBoundary();
	vector<MSCompoG*> GetSlabGArr();
	map<long, long> mm_SlabGID;

	double GetSLevel();
	double GetELevel();
	bool GetUseOffset();

	void SetSLevel(double level);
	void SetELevel(double level);
	void SetUseOffset(bool value);

	virtual void FixProfile() override;
protected:
	double m_SLevel;
	double m_ELevel;
	bool m_IsUseOffset;
};

