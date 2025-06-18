#pragma once
#include "MSCompoM.h"
#include "StructureModelEnum.h"

class MDCustomPointUnitRebar;
class MDCustomLineUnitRebar;
class MSCustomSection;
class MDCustomSection;
class StructureModel_CLASS MSCustomM : public MSCompoM
{
public:
	MSCustomM();
	~MSCustomM();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	bool IsGeometryDifferent(MSCompoM* compoM) override;
	virtual void GetSecPolyline(GM2DPolyline& APGon);
	virtual bool IsCustomM() { return true; };

	void SetMSCustomSection(MSCustomSection* customSection);
	MSCustomSection* GetMSCustomSection();
	int GetSectionID() { return m_nSectionID; };
	virtual void MakeDefaultRebars() {};

	MDCustomSection* GetMDCustomSection(eCustomM_SectionType eType);
	vector<GM2DVector> CalculatorPointBarLocation(int msPointUnitID, eCustomM_SectionType eType);
	vector<GM2DPolyline*> CalculatorLineBarLocation(int msLineUnitID, eCustomM_SectionType eType);

	MDCustomPointUnitRebar* GetMDPointUnitRebar(int unitID, eCustomM_SectionType eType);
	MDCustomLineUnitRebar* GetMDLineUnitRebar(int unitID, eCustomM_SectionType eType);

	virtual void UpdateCustomRebarsID(map<long, long> oldUnitIDtoNewUnitID) {};
	virtual void UpdateCustomCompoM() {};
	virtual double GetWidth();
	virtual double GetDepth();

	bool IsCustomColumnSection();
	bool IsCustomWallColumnSection();
	bool IsCustomBeamSection();
	double GetFloorLevelOffSet();
	long GetCustomSectionID();
protected:

	MSCustomSection* mp_Section;
	int m_nSectionID;
};

