#pragma once
#include "MSCustomSection.h"
class MSCustomLineBarUnit;
class MSCustomPointBarUnit;
class MSbaseBuilding;
class StructureModel_CLASS T_PointUnitIDtoRebarIndex : public CObject
{
public:
	T_PointUnitIDtoRebarIndex()
	{
		_PointUnitID = 0;
		_RebarIndex = -1;
	}
	T_PointUnitIDtoRebarIndex(const T_PointUnitIDtoRebarIndex& src)
	{
		(*this) = src;
	}
	T_PointUnitIDtoRebarIndex& operator=(const T_PointUnitIDtoRebarIndex& src)
	{
		_PointUnitID = src._PointUnitID;
		_RebarIndex = src._RebarIndex;

		return (*this);
	}
	int _PointUnitID;
	int _RebarIndex;
};

class StructureModel_CLASS MSCustomLineSection : public MSCustomSection
{
public:
	MSCustomLineSection(MSBaseBuilding* pBuilding = nullptr);
	~MSCustomLineSection();

	DECLARE_SERIAL(MSCustomLineSection)
	void Serialize(CArchive& archive);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSCustomSection* pSource, bool bNewID = true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void CopyFromMe_Info(MSCustomSection* pSource);
	void CopyFromMe_MSCustomLineBarUnit(MSCustomSection* pSource);
	void CopyFromMe_MSCustomPointBarUnit(MSCustomSection* pSource);

	virtual bool DeleteMSReBarLine(long RebarLineID) override;
	virtual void DeleteMSReBarLines();
	virtual void GetRebarTypeToIDs(map<MSCustomSection::eRebarType, vector<long>>& RebarTypeToIDs);
	virtual void SetRebarTypeToIDs(map<MSCustomSection::eRebarType, vector<long>> RebarTypeToIDs);

	virtual vector<MSCustomPointBarUnit*> GetMSPointUnits() override { return ma_PointUnit; };
	virtual vector<MSCustomLineBarUnit*> GetMSLineUnits() override { return ma_LineUnit; };

	virtual MSCustomPointBarUnit* GetMSPointUnit(int nID) override;
	virtual MSCustomLineBarUnit* GetMSLineUnit(int nID) override;

	virtual void UpdateCustomSectionInfoID(MSCustomSection* pOldCustomSection, map<long, long>& oldUnitIDtoNewUnitID, bool NewID) override;

	void AddMSLineUnit(MSCustomLineBarUnit* lineUnit);
	void AddMSPointUnit(MSCustomPointBarUnit* pointUnit);

	void DeleteMSLineUnits();
	void DeleteMSPointUnits();

	int GetInputNumberByMSPointUnitID(int nID) override;
	int GetInputNumberByMSLineUnitID(int nID) override;


	virtual bool DeleteMSPointUnit(long nID);
	virtual bool DeleteMSLineBarUnit(long nID);

protected:
	vector<MSCustomLineBarUnit*> ma_LineUnit;
	vector<MSCustomPointBarUnit*> ma_PointUnit;

	bool DeleteLineUnit_HasRebarLineID(long RebarLineID);
	bool DeletePointUnit_HasRebarLineID(long RebarLineID);
};