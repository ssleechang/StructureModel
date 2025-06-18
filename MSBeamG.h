#pragma once
#include "mslinememberg.h"

#include "MSBoundaryCondition.h"

class MSBuiltUpBeamG;

class StructureModel_CLASS MSBeamG : public MSLineMemberG
{
public:
	MSBeamG();
	~MSBeamG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSBeamG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual bool IsPC();

	virtual Type_MSCompoG GetType(){return msBeamG;};

    virtual double GetThick();

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual void UpdateTrimmedBoundary(long nUpdateConnIdx = 0);
	virtual void UpdateTrimmedBoundary_AfterCalculate(CON_POS ePos = CON_ALL);
	virtual void UpdateBoundaryVisibilities(long nUpdateConnIdx = 0);
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true);
	virtual bool GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc = 0.5);
	bool GetZLevelsOnlyBeam(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true);

	virtual bool SetCompoM(long MID, bool bUpdateVecInput = true);
	virtual bool SetCompoM(MSCompoM* pCompoM, bool bUpdateVecInput = true);
	virtual void RecoverPointerAfterSerialize(MSFloor* floor);

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	virtual long ResetGetID();

	virtual void RemoveConnections(bool bNotRemoveSetByUser = false);

	virtual void DeleteConnections();
	virtual void RemoveConnection(MSConnection* pConnection);

	double GetDevLen(CON_POS ePos, bool bCheckColumn = false);
	//virtual void ModifyObject();

	bool m_bSubBeam;

	virtual bool HasOverLapWall();

	MSBoundaryCondition::Type_Condition GetStartBoundaryCondition();
	MSBoundaryCondition::Type_Condition GetEndBoundaryCondition();
	void SetStartBoundaryCondition(MSBoundaryCondition::Type_Condition condition);
	void SetEndBoundaryCondition(MSBoundaryCondition::Type_Condition condition);

	bool IsCornerPlacing();

	map<long, MSBuiltUpBeamG*> mm_BuiltUp;
	bool HasBuiltUpBeamG();
	void AddBuiltUpBeamG(MSBuiltUpBeamG* pBuiltG, bool bUpdate);
	void Remove(long ID);
	void DeleteBuiltGs();
	MSBuiltUpBeamG* GetBuiltG(long ID);

	void DeleteErrorBuiltUpBeam();

	void SyncBuiltdUpBeamLocationByLoc();
	void SyncBuiltdUpBeamStartEndLocationByLoc();
	virtual void SetLines();
	virtual bool HasSubMember()	override;
	virtual void DeleteSubMembers() override;

	virtual bool UsedMaterial(long materialID) override;
	void SubMemberMaterialValidCheck();

	virtual vector<MSElement*> GetSubMembers() override;
	virtual void SetDummy(bool IsDummy) override ;
	
	bool IsReversBeamM();
	bool IsCustomFWBeam();
protected:
	MSBoundaryCondition m_StartCondition;
	MSBoundaryCondition m_EndCondition;

private:
	virtual void Serialize_EachCompoM_Rebar(CArchive& ar);

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msBeamM; };
};
