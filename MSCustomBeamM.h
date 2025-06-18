#pragma once
#include "MSCustomM.h"

class MDCustomPointUnitRebar;
class MDCustomLineUnitRebar;
class MDCustomSection;
class MDCustomBeamM;
class StructureModel_CLASS MSCustomBeamM : public MSCustomM
{
public:
	MSCustomBeamM();
	~MSCustomBeamM();

	DECLARE_SERIAL(MSCustomBeamM)
	void Serialize(CArchive& ar);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void MakeDefaultRebars() override;

	virtual void UpdateCustomRebarsID(map<long, long> oldUnitIDtoNewUnitID) override;
	virtual void UpdateCustomCompoM() override;

	MDCustomBeamM* GetMDCustomBeamM();

	vector<CString> GetLinkedExteriors();
	vector<CString> GetLinkedInteriors();
	void SetLinkedExteriors(vector<CString>& linkeds);
	void SetLinkedInteriors(vector<CString>& linkeds);

	double GetSectionFloorLevelOffSet();
	vector<GM2DPolyline*> GetSplitSecPolylinebyFloorLevel(double dFloorLevel, bool IsUp);

private:
	void MakeDefaultRebars(MDCustomSection* pMDCustomSection);

	vector<CString> ma_LinkedExterior;
	vector<CString> ma_LinkedInterior;
};