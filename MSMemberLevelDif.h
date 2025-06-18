#pragma once
#include "msobject.h"

class MSLineLevelDifG;
class MSCompoG;


class StructureModel_CLASS MSMemberLevelDif :
	public MSObject
{
public:
	MSMemberLevelDif(void);
	~MSMemberLevelDif(void);
	void Serialize(CArchive& ar);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSObject* pSource, bool bNewID = true);
	MSMemberLevelDif& operator=(const MSMemberLevelDif& src);
	MSMemberLevelDif(const MSMemberLevelDif& src);

	MSLineLevelDifG* mp_LineLevelDifG;
	double m_Loc;
	double m_LLoc;
	double m_RLoc;
	double m_StartLevel;
	double m_EndLevel;
	double m_InfillThick;
	double m_dHaunchAngle;
	double m_dHaunchY;
	bool m_bUseHaunchMaterial;
	int m_nHaunchMaterialID;
	bool m_IsUsedStoryMaterial;

	MSCompoG* mp_OwnerCompoG;

	void ReverseData();
	double GetLevelDist();

	long GetHaunchMaterialID();
	void SetHaunchMaterialID(long materialID)		{ m_nHaunchMaterialID = materialID; };
	bool GetIsUsedStoryMaterial()												{ return m_IsUsedStoryMaterial; };
	void SetIsUsedStoryMaterial(bool bUsedStoryMaterial)	{ m_IsUsedStoryMaterial = bUsedStoryMaterial; };

	GM2DVector GetStartVec();
	GM2DVector GetEndVec();
	GM2DVector GetMidVec();
	double GetStartLoc();
	double GetEndLoc();
};

