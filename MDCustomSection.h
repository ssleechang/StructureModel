#pragma once

#include "MDMember.h"
#include "MSSpecificTieBar.h"

class MDCustomPointUnitRebar;
class MDCustomLineUnitRebar;
class MSCustomPointBarUnit;
class MSCustomLineBarUnit;
class MSCustomSection;
class StructureModel_CLASS MDCustomSection : public MDMember
{
public:
	MDCustomSection();
	~MDCustomSection();
	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MDCustomSection)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	vector<MDCustomPointUnitRebar*> GetMDPointUnitRebars() { return ma_PointUnit; };
	vector<MDCustomLineUnitRebar*> GetMDLineUnitRebars() { return ma_LineUnit; };

	MDCustomPointUnitRebar* GetMDPointUnitRebar(int unitID);
	MDCustomLineUnitRebar* GetMDLineUnitRebar(int unitID);

	void DeleteCustomMDPointUnitRebars();
	void DeleteCustomMDLineUnitRebars();

	void MakeDefaultMDPointUnitRebar(MSCustomPointBarUnit* pMSPointUnit);
	void MakeDefaultMDLineUnitRebar(MSCustomLineBarUnit* pMSLineUnit);

	virtual void UpdateCustomRebarsID(map<long, long> oldUnitIDtoNewUnitID);
	virtual void UpdateCustomRebars(MSCustomSection* pMSSection);

	double GetMaxDeformedBarDiameterByLineUnitRebar();
	double GetMaxDeformedBarDiameterByPointUnitRebar();

	vector<MSSpecificTieBar*> ma_SpecificTieBar;
	void DeleteSpecificTieBars();

protected:
	void DeletePointRebarsByCustomSection(MSCustomSection* pMSSection);
	void DeleteLineRebarsByCustomSection(MSCustomSection* pMSSection);
	void AddedPointRebarsByCustomSection(MSCustomSection* pMSSection);
	void AddedLineRebarsByCustomSection(MSCustomSection* pMSSection);

protected:
	vector<MDCustomPointUnitRebar*> ma_PointUnit;
	vector<MDCustomLineUnitRebar*> ma_LineUnit;

};