#pragma once
#include "mslinememberg.h"
#include "MDMember.h"
#include "MSStirrupBar.h"

class MSSubHaunchG;
class StructureModel_CLASS MSWallFootG :
	public MSLineMemberG
{
public:
	MSWallFootG();
	~MSWallFootG(void);
	void Serialize( CArchive& archive );
	void Serialize_EachCompoM_Rebar(CArchive& ar);
	void Serialize_EachCompoM_Formwork(CArchive& ar);
	DECLARE_SERIAL(MSWallFootG)
    virtual Type_MSCompoG GetType(){return msWallFootG;};
    virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual long ResetGetID();

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);

	virtual void RecoverPointerAfterSerialize(MSFloor* floor);
	virtual void RecoverPointerAfterSerialize_SubMembers();
	virtual void SetCenCurve(GM2DCurve* pCurve);
	virtual void UpdateBoundaryVisibilities(long nUpdateConnIdx = 0);

	bool HasSpacer();
// 우마 철근
public:
	long m_TopSpacerID;
	long m_BotSpacerID;
	virtual long GetTopSpacerID() { return m_TopSpacerID; };
	virtual long GetBotSpacerID() { return m_BotSpacerID; };

	vector<MSSubHaunchG*> ma_Haunches;
	
	virtual void AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	virtual void AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	virtual void DeleteHaunchs();
	virtual bool Remove(MSSubHaunchG* pSubHaunchG);
	virtual bool RemoveSubMember(MSElement* element) override;
	virtual bool HasHaunch();

	

	void TrimHaunches();
	void RegenHaunch();
	virtual bool HasSubMember() override;
	virtual void DeleteSubMembers() override;

	virtual bool UsedMaterial(long materialID) override;
	void SubMemberMaterialValidCheck();

	virtual vector<MSElement*> GetSubMembers() override;

private:
	vector<MSCompoG*> GetConnectionHaunchs();
	void AddHaunchGs(vector<MSSubHaunchG*> HaunchGArr);

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msWallFootM; };
};
