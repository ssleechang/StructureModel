#pragma once
#include "MDMember.h"
class MSbaseBuilding;
class MSCustomPointBarUnit;
class MSCustomReBarLine;
class MSCustomLineBarUnit;
class StructureModel_CLASS  MSCustomSection : public MDMember
{
public:
	MSCustomSection(MSBaseBuilding* pBuilding= nullptr);
	~MSCustomSection();

	DECLARE_SERIAL(MSCustomSection)
	void Serialize(CArchive& archive);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSCustomSection* pSource, bool bNewID = true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void CopyFromMe_Info(MSCustomSection* pSource);
	void CopyFromMe_CustomBarLine(MSCustomSection* pSource);

	enum eSectionType { CUSTOM_NONE = -1, CUSTOM_COLUMN = 0, CUSTOM_WALLCOLUMN, CUSTOM_BEAM, CUSTOM_BUTTRESS };
	enum eRebarType { REBAR, POINT, LINE };

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	virtual void GetSecPolyline(GM2DPolyline& APGon);

	eSectionType GetSectionType() { return m_eSectionType; };
	void SetSectionType(int nSectionType) { m_eSectionType = (eSectionType)nSectionType; }
	void SetName(CString name) {m_Name = name.MakeUpper();};
	CString GetName() { return m_Name; };
	void SetProfile(GM2DPolyline* profile);
	GM2DPolyline*  GetProfile();

	void Add(MSCustomReBarLine* RebarLine);
	MSCustomReBarLine* GetMSRebarLine(long nID);
	vector<MSCustomReBarLine*> GetMSRebarLines() { return ma_RebarLine; };
	virtual bool DeleteMSReBarLine(long RebarLineID);
	virtual void DeleteMSReBarLines();
	virtual void GetRebarTypeToIDs(map<MSCustomSection::eRebarType, vector<long>>& RebarTypeToIDs) {};
	virtual void SetRebarTypeToIDs(map<MSCustomSection::eRebarType, vector<long>> RebarTypeToIDs) {};

	virtual bool DeleteMSPointUnit(long nID) { return false; };
	virtual bool DeleteMSLineBarUnit(long nID) { return false; };

	void ResetProfile();

	GM2DCurve* GetRebarLineGeometry(long RebarLineID);
	
	bool MakeMSRebarLine(GM2DCurve* pCurve, int offsetType, double offset, int coverType);
	int FindEdgeIndex(GM2DCurve* pCurve);

	void MakeRebarLineIDs();

	virtual void ReMakeMSPointUnit() { return; };
	virtual int CheckData();

	GM2DVector* GetCrossPointByRebarLineIDs(long rebarLineID1, long rebarLineID2);

	virtual int GetInputNumberByMSPointUnitID(int nID) {return 0;};
	virtual int GetInputNumberByMSLineUnitID(int nID) { return 0; };
	virtual vector<GM2DVector> CalculatorPointBarLocation(int pointUnitID);

	virtual vector<MSCustomPointBarUnit*> GetMSPointUnits();
	virtual vector<MSCustomLineBarUnit*> GetMSLineUnits();
	virtual MSCustomPointBarUnit* GetMSPointUnit(int nID) { return nullptr; };
	virtual MSCustomLineBarUnit* GetMSLineUnit(int nID) { return nullptr; };

	virtual MSBaseBuilding* GetBuilding() override { return mp_Builing; };
	virtual double GetFloorLevelOffSet() { return 0; };

	virtual void UpdateCustomSectionInfoID(MSCustomSection* pOldCustomSection, map<long, long>& oldUnitIDtoNewUnitID, bool NewID) {};

	static MSCustomReBarLine* CreateMSCustomBarLine();
	static MSCustomPointBarUnit* CreateMSPointBarUnit();
	static MSCustomLineBarUnit* CreateMSLineBarUnit();

	//해당하는 edge Index의 리바라인을 가져온다. 옵셋별로 정렬해서 가져옴.
	virtual vector<MSCustomReBarLine*> GetRebarLineByEdgeIdx(int EdgeIdx);

	bool IsContainRebarLine(MSCustomReBarLine* pRebarLine);
protected:
	bool IsExistRebarLine(int edgeIndex, double offset);

protected:
	eSectionType m_eSectionType;
	CString m_Name;
	GM2DPolyline* mp_Profile;
	vector<MSCustomReBarLine*> ma_RebarLine;
	MSBaseBuilding* mp_Builing;
};

