#pragma once
#include "msobject.h"

class MSConcrete;
class MSSteel;
class MSSrcMaterial;
class MSMaterial;
class MSReinforceBar;
class MSReinforceBarSetting;
class MSSteelGradeManager;
class MSCompoM;
class StructureModel_CLASS MSGlobalOption : public MSObject
{
public:
	static MSGlobalOption* GetInstance();
	~MSGlobalOption();
	void Serialize( CArchive& archive );
	void Serialize_FormWorkMaterial(CArchive& archive);
	void Serialize_FormWorkShape(CArchive& archive);
	DECLARE_SERIAL(MSGlobalOption)

	map<long, MSMaterial*> mm_Material;
	map<long, MSReinforceBar*> mm_ReinforceBar;
	map<long, MSReinforceBarSetting*> mm_ReinforceBarSetting;
	MSSteelGradeManager* m_pSGradeManager;
	vector<MSCompoM*> ma_FormWorkShape;
	
	bool WriteFileBinary(CString strFilePath);
	bool WriteFileBinary_FormWorkMaterial(CString strFilePath);
	bool WriteFileBinary_FormWorkShape(CString strFilePath);

	int ReadFileBinary(CString strFilePath);
	int ReadFileBinary_FormWorkMaterial(CString strFilePath);
	int ReadFileBinary_FormWorkShape(CString strFilePath);

	
	MSMaterial* CreateNewMaterial(long nType);

	void DeleteMaterials();
	void DeleteReinforceBars();
	void DeleteReinforceBarSettings();

	MSConcrete* CreateConcrete(CString Name, int Strength);
	MSSteel* CreateSteel(CString Name, int Strength);
	MSSrcMaterial* CreateSrc(MSConcrete* pConcrete, MSSteel* pSteel);

	bool AddMaterial(MSMaterial* pMaterial);
	bool AddReinforceBar(MSReinforceBar* pReinforceBar);
	bool AddReinforceBarSetting(MSReinforceBarSetting* pReinforceBarSetting);

	bool DeleteMaterial(MSMaterial* pMaterial);
	bool DeleteReinforceBar(MSReinforceBar* pReinforceBar);
	bool DeleteReinforceBarSetting(MSReinforceBarSetting* pReinforceBarSetting);

	MSReinforceBar* GetReinforceBar(long ID);
	MSReinforceBar* GetReinforceBar(CString Name);
	MSReinforceBar* GetFirstReinforceBar();
	bool IsValidReinforceBar();
	bool IsValidReinforceBar(MSReinforceBar* pFirstReinforceBar);
	bool IsValidReinforceBarSetting();
	bool IsValidReinforceBarSetting(MSReinforceBarSetting* pMSReinforceBarSetting);

	MSReinforceBarSetting* GetReinforceBarSetting(long ID);
	MSMaterial* GetMaterial( long MaterialID);

	void SetDefaultReinforceBarSettingID(long nID) { m_DefaultReinforceBarSettingID = nID;};
	long GetDefaultReinforceBarSettingID()				 { return m_DefaultReinforceBarSettingID;};
	void SetPlainConcreteReinforceBarSettingID(long nID) { m_PlainConcreteReinforceBarSettingID = nID;};
	long GetPlainConcreteReinforceBarSettingID()					{ return m_PlainConcreteReinforceBarSettingID;};
	MSSteelGradeManager* GetSGradeManager(){ return m_pSGradeManager;};

	void CreateMaterialData();
	void CreateFormWorkMaterialData();

	MSCompoM* CreateNewCompoM(long nType);
	void CreateFormWorkShapeDatas();
	void DeleteFormWorkShapeDatas();
	void AddFormWorkShape(MSCompoM* pCompoM);
	void DeleteFormWorkShape(MSCompoM* pCompoM);

	void SetReinforceBarManager();
	void PlainConcreteMaterialAdd();
	bool MaterialNameExistCheck(CString name);
	CString NewMaterialName(double dConcStrength);

protected:
	void CreateFWMainCompoMs();
	void CreateFWSubCompoMs();

	void MakeReinforceBarsByRebarCode();
	void MakeReinforceBarsByRebarCode_KR();
	void MakeReinforceBarsByRebarCode_ASTM();
	void MakeReinforceBarsByRebarCode_GB();
	void MakeReinforceBarsByRebarCode_BS();
	void MakeReinforceBarsByRebarCode_VIETNAM();
	void MakeReinforceBarsByRebarCode_INDIA();
	void MakeReinforceBarsByRebarCode_SINGAPORE();
	void MakeReinforceBarsByRebarCode_JIS();

	void MakeReinforceBarSettingAndMaterialDatasByRebarCode();
	void MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();
	void MakeReinforceBarSettingAndMaterialDatasByRebarCode_ASTM();
	void MakeReinforceBarSettingAndMaterialDatasByRebarCode_GB();
	void MakeReinforceBarSettingAndMaterialDatasByRebarCode_BS();
	void MakeReinforceBarSettingAndMaterialDatasByRebarCode_VIETNAM();
	void MakeReinforceBarSettingAndMaterialDatasByRebarCode_INDIA();
	void MakeReinforceBarSettingAndMaterialDatasByRebarCode_SINGAPORE();
	void MakeReinforceBarSettingAndMaterialDatasByRebarCode_JIS();

	void MakeDefaultReinforceBarSettings();

	void MakeRcMaterialsByRebarCode();
	void MakeDefaultSteelMaterials();
	void MakeDefaultSrcMaterials();

	void MakeRcMaterialsByRebarCode_Kr();
	void MakeRcMaterialsByRebarCode_GB();

	void UpdateReinforceBarGB_BH3374();

protected:
	static MSGlobalOption* m_instance;
	MSGlobalOption();
	void CreateNewReinforceBars();
	void Patch_Material_Version20150701();
	long m_PlainConcreteReinforceBarSettingID;
	long m_DefaultReinforceBarSettingID;
};
