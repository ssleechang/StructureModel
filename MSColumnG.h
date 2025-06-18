#pragma once
#include "MSVerticalLineMemberG.h"

#include "GMLib/GMLine.h"
#include "MSBoundaryCondition.h"

class MSPitG;
class MSIsoFootG;
class MSLinkMemberG;
class MSSlabG;
class StructureModel_CLASS MSColumnG : public MSVerticalLineMemberG
{
public:
	MSColumnG();
	~MSColumnG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSColumnG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual void Init(bool bInConstructor=false);

	virtual bool CheckData();
	virtual bool CheckHeight(bool IsUseGroupName = false);

	virtual Type_MSCompoG GetType(){return msColumnG;};
	virtual double GetDepth();

	virtual bool IsContainSlabinLowFloor();
	virtual bool IsButtress();
	virtual bool IsPC();


	bool IsConnectedBelowColumn(MSColumnG* pUpColumn, bool bCheckOverLap = false);

	MSColumnG* GetUpperColumn(bool bCheckButtress = true);
	MSColumnG* GetLowerColumn(bool bCheckButtress = true);
	MSIsoFootG* GetLowerIsoFooting();
	vector<MSLinkMemberG*> GetLowerSptLinkMember();
	MSPitG* GetLowerPit();
	MSSlabG* GetLowerSlab();
	bool hasBoundaryOffsetBeamMembers();

	MSBoundaryCondition::Type_Condition GetStartBoundaryCondition();
	MSBoundaryCondition::Type_Condition GetEndBoundaryCondition();
	void SetStartBoundaryCondition(MSBoundaryCondition::Type_Condition condition);
	void SetEndBoundaryCondition(MSBoundaryCondition::Type_Condition condition);

	GM2DVector GetSteelCenter()										{ return m_SteelCenter;};
	void				  SetSteelCenter(GM2DVector delta)	{ m_SteelCenter = delta;};

	bool IsCornerPlacing();
	bool HasColumnHeaderM();
	int GetColumnHeaderM_T1();
	int GetColumnHeaderM_T3();
	int GetColumnHeaderM_T5();
	MSCompoM* GetColumnHeaderM();
	void SetColumnHeaderM(CString columnHeadName);

	double GetColumnHeaderOffset();
	void SetColumnHeaderOffset(double value);

	double GetColumnHeaderSubOffset();
	void SetColumnHeaderSubOffset(double value);

	bool IsRectSecProfile();
	bool IsCircleSecProfile();

	enum LocationCalculationType	{ Auto = 0, Outer, Inner,	};
	long GetLocationCalculationType();
	void SetLocationCalculationType(long nType);
	long GetLocationCalculationResult();
	void SetLocationCalculationResult(long nType);
protected:
	MSBoundaryCondition m_StartCondition;
	MSBoundaryCondition m_EndCondition;
	GM2DVector m_SteelCenter;
	long m_nLocationCalculationType;//LocationCalculationType	{ Auto = 0, Outer, Inner,	};
	long m_nLocationCalculationResult;//0 none, 1 outer, 2 inner
	virtual void Serialize_EachCompoM_Rebar(CArchive& ar);

	double m_dColumnHeaderOffset;
	double m_dColumnHeaderSubOffset;

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msColumnM; };
};
