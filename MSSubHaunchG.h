#pragma once
#include "MSLinkMemberG.h"

class StructureModel_CLASS MSSubHaunchG :
	public MSLinkMemberG
{
public:
	MSSubHaunchG();
	~MSSubHaunchG(void);
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSSubHaunchG)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSSubHaunchG* pSource, bool bNewID = true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void Init(bool bInConstructor = false);
	virtual Type_MSCompoG GetType(){ return msSubHaunchG; };

	virtual MSMaterial* GetMaterial();

	virtual bool GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc = 0.5);
	virtual void SetOwnerCompoG(MSElement* pOwner) { mp_OwnerCompoG = (MSCompoG*)pOwner; };
	virtual MSElement* GetOwnerCompoG(){return mp_OwnerCompoG;};
	virtual double GetDepth();
	virtual double GetThick();
	virtual double GetWidth();

	virtual void SetLines();
	void SetCenCurve(GM2DCurve* pCurve);
	GM2DCurve* GetCurve();
	shared_ptr<GM2DCurve> GetCenCurve();
	virtual GM2DVector GetCenVec();

	virtual void Translate(GM2DVector& DeltaVec);
	virtual void RotateZ(double Theta, GM2DVector* pCenter);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);

	void AddConnectionWithMember(MSCompoG* pPartnerMember);
	
	double GetOwnerCompoGThick();

	long GetHaunchMaterialID();
	void SetHaunchMaterialID(long materialID)		{ m_nHaunchMaterialID = materialID; };
	bool GetIsUsedStoryMaterial()												{ return m_IsUsedStoryMaterial; };
	void SetIsUsedStoryMaterial(bool bUsedStoryMaterial)	{ m_IsUsedStoryMaterial = bUsedStoryMaterial; };
	void ResetCenterCurve();
	double GetLevelZoneZ();
	bool isSameHaunchInfo(MSSubHaunchG* pUrHaunch);
public:
	double m_dHaunchAngle;
	double m_dHaunchY;
	double m_dHaunchY_Top;
	bool m_bUseHaunchMaterial;
	bool m_bUseBackFillFormwork;
	int m_nHaunchMaterialID;
	bool m_IsUsedStoryMaterial;

	MSCompoG* mp_OwnerCompoG;
};
