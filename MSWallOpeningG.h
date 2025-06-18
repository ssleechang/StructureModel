#pragma once
#include "MSCompoG.h"

class MSWallMemberG;
class StructureModel_CLASS MSWallOpeningG : public MSCompoG
{
public:
    MSWallOpeningG(void);
    ~MSWallOpeningG(void);

    void Serialize( CArchive& archive );
    DECLARE_SERIAL(MSWallOpeningG)

    virtual void Dump(EFS::EFS_Buffer& buffer);
    virtual void Recovery(EFS::EFS_Buffer& buffer); 

	
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual MSElement* GetOwnerCompoG(){ return m_pOwnerCompoG; };
	virtual void SetOwnerCompoG(MSElement* pOwnerCompoG);

	virtual bool MakeBoundary();

	MSElement* m_pOwnerCompoG;

    GM2DVector m_BaseOffset; //X:Dist Y :Bottom Offset
    bool m_Reversed;

	bool m_IsUseFloorLevel;

	double GetHeightBySlabLevel(double dSlabLevel);
	double GetExtraSize();
	void GetZLevel(double& dTopZLevel, double& dBotZLevel, bool bUseExtraSize, bool bMaxLevel, bool IsNotOverWall = true);

	bool GetOpeningLoc(double& dSLoc, double& dELoc);

	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true);

	virtual CString GetKey(bool bUseID = true);
};

#pragma make_public(MSWallOpeningG)