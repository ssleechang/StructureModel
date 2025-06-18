#pragma once
#include "MSObject.h"
class MSBaseBuilding;

class StructureModel_CLASS NayukFormworkSetting : public MSObject
{
public:
	//FormMaterialCost, FormInstallCost 는 타입이 똑같으므로 FormCost로 통일해서 사용
	struct FormCost
	{
		FormCost(CString formName, CString code, CString name, CString standard, CString unit) :
			m_FormName(formName), m_Code(code), m_Name(name), m_Standard(standard), m_Unit(unit) {}

		FormCost() {}

		CString m_FormName; //거푸집
		CString m_Code; //코드
		CString m_Name; //명칭
		CString m_Standard; //규격
		CString m_Unit; //단위
	};

	NayukFormworkSetting(MSBaseBuilding* building);
	~NayukFormworkSetting();

	void Serialize(CArchive& archive);
	//DECLARE_SERIAL(NayukFormworkSetting) : 이건 정확히 모르겠어서 주석처리합니다. 같이 처리해주세요 팀장님.
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	//이 두 개의 함수도 현재는 같으므로 하나로만 사용
	void InitFormCostSet(MSBaseBuilding* building);
	void CreateFormWorkMaterialCost(int nMaterialCostNum);
	void CreateFormWorkInstallCost(int nInstallCostNum);

	vector<FormCost*> ma_FormMaterialCost;
	vector<FormCost*> ma_FormInstallCost;
};

