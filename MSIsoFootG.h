#pragma once
#include "MSPointMemberG.h"
#include "MSStirrupBar.h"
#include "MDMember.h"

class MSColumnG;
class MSSubHaunchG;

class StructureModel_CLASS MSIsoFootG :
	public MSPointMemberG
{
public:
	MSIsoFootG();
	~MSIsoFootG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSIsoFootG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msIsoFootG;};

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual long ResetGetID();

	virtual bool CheckData();
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true );
	virtual double GetEndLevel(bool isMax = true);
	virtual double GetRelativeEndLevel(bool isMax = true);
	virtual double GetStartLevel(bool isMax = false);
	virtual double GetRelativeStartLevel(bool isMax = false);
	virtual bool IsDropPanel();

	virtual void Translate(GM2DVector &pDelta);
	virtual void ModifyObject(bool isDoit = true);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	virtual void SetRotation(double dDeg);
	virtual void AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	virtual void AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	virtual void DeleteHaunchs();
	virtual bool Remove(MSSubHaunchG* pSubHaunchG);
	virtual bool RemoveSubMember(MSElement* element) override;
	virtual bool HasHaunch();

	virtual void RecoverPointerAfterSerialize(MSFloor* floor);
	virtual void RecoverPointerAfterSerialize_SubMembers();

	void TrimHaunches();
	void RegenHaunch();
	MSColumnG* GetConnectedColumnG();
	
	bool HasSpacer();

	// 우마 철근
	long m_TopSpacerID;
	long m_BotSpacerID;
	virtual long GetTopSpacerID() { return m_TopSpacerID; };
	virtual long GetBotSpacerID() { return m_BotSpacerID; };

	virtual bool HasSubMember() override;
	virtual void DeleteSubMembers() override;

	vector<MSSubHaunchG*> ma_Haunches ;

	virtual bool UsedMaterial(long materialID) override;
	void SubMemberMaterialValidCheck();

	virtual vector<MSElement*> GetSubMembers() override;

private:
	void Serialize_EachCompoM_Rebar(CArchive& ar);
	void AddHaunchGs(vector<MSSubHaunchG*> HaunchGArr);

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msIsoFootM; };
};
