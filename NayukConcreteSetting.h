#pragma once
#include "MSObject.h"
#include "StructureModel/MSElement.h"
#include "MSBaseBuilding.h"

class StructureModel_CLASS NayukConcreteSetting : public MSObject
{
public:
	struct ConcMaterialCost
	{
		ConcMaterialCost(int aggregate, int strength, int slump, bool isReinforce, CString code, CString name, CString standard, CString unit) :
			m_Aggregate(aggregate), m_Strength(strength), m_Slump(slump), m_IsPlainConc(isReinforce),  m_Code(code), m_Name(name), m_Standard(standard), m_Unit(unit) {}

		ConcMaterialCost() {}

		int m_Aggregate; //콘크리트 골재
		int m_Strength; //강도
		int m_Slump;   //슬럼프
		bool m_IsPlainConc; //철근 or 무근
		CString m_Code; //코드
		CString m_Name; //명칭
		CString m_Standard; //규격
		CString m_Unit; //단위
	};

	struct ConcInstallCost
	{
		ConcInstallCost(bool isPlain, int placementType, CString code, CString name, CString standard, CString unit) :
			m_IsPlainConc(isPlain), m_nPlacementType(placementType), m_Code(code), m_Name(name), m_Standard(standard), m_Unit(unit) {}

		ConcInstallCost() {}

		bool m_IsPlainConc;		//철근 or 무근
		int m_nPlacementType;	//0:붐대타설 1:배관타설1구간 2:배관타설2구간
		CString m_Code; 
		CString m_Name; 
		CString m_Standard; 
		CString m_Unit;
	};

	NayukConcreteSetting();
	~NayukConcreteSetting();

	void Serialize(CArchive& archive);
	//DECLARE_SERIAL(NayukConcreteSetting)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	void InitData();
	void InitConcMaterialCostSet();
	void InitConcInstallCostSet();

	void CreateConcMaterialCost(int materialCostNum);
	void CreateConcInstallCost(int installCostNum);

	bool m_UsingPump;
	int m_nPumpSectionType;		//0:단일구간 1:다구간
	int m_nOneSectionFloorNum;	//1구간 ㅁ번 층 이상
	int m_nTwoSectionFloorNum;	//2구간 ㅁ번 층 이상
	vector<ConcMaterialCost*> ma_ConcMaterialCost;
	vector<ConcInstallCost*> ma_ConcInstallCost;
};

