#pragma once
#include "MSObject.h"
#include "StructureModel/MSElement.h"
class MSBaseBuilding;

class StructureModel_CLASS NayukRebarSetting : public MSObject
{
public:

	struct RebarMaterialCost
	{
		RebarMaterialCost(CString dia, CString material, CString code, CString name, CString standard, CString unit) :
			m_Dia(dia), m_Material(material), m_Code(code), m_Name(name), m_Standard(standard), m_Unit(unit) {}

		RebarMaterialCost() {}

		CString m_Dia;//직경
		CString m_Material; //강종
		CString m_Code; //코드
		CString m_Name; //명칭
		CString m_Standard; //규격
		CString m_Unit; //단위
	};

	struct RebarInstallCost
	{
		RebarInstallCost(int bigCategory, CString smallCategory, CString code, CString name, CString standard, CString unit) :
			m_BigCategory(bigCategory), m_SmallCategory(smallCategory), m_Code(code), m_Name(name), m_Standard(standard), m_Unit(unit) {}

		RebarInstallCost() {}

		int m_BigCategory;   // 1: 가공/조립 2: 운반 3:하차/선별 4.시공도제작 5: 기타
		CString m_SmallCategory; // 1 : 복잡 2: 보통 || 1 : 생산공장->가공 2: 가공공장>현장 || 1: 생산공장->현장 || 1. 하차비, 2. 하차/선별 || 1. 시공도제작 || 기타
		CString m_Code; 
		CString m_Name; 
		CString m_Standard; 
		CString m_Unit; 
	};

	NayukRebarSetting(MSBaseBuilding* building);
	~NayukRebarSetting();

	void Serialize(CArchive& archive);
	//DECLARE_SERIAL(NayukRebarSetting)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	void InitData(MSBaseBuilding* building);
	void InitRebarMaterialCostSet(MSBaseBuilding* building);
	void InitRebarInstallCostSet();
	void CreateRebarInstallCost(int nFactoryInstallCostNum, int nSiteInstallCostNum);
	void CreateRebarMaterialCost(int nMaterialCostNum);

	int m_nFabricateType; //0: 공장가공, 1: 현장가공false;
	vector<RebarMaterialCost*> ma_RebarMaterialCost;
	map<int, vector<RebarInstallCost*>> mapWorkTypeToRebarInstallCost;
};

