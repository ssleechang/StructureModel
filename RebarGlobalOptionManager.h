#pragma once
using namespace std;

class RebarGlobalOption;
class OrderLengthSettingSet;
class OrderLengthSet;
class CorrectiveLengthSet;
class CoverLengthSet;
class DevSpliceLengthSet;
class StructureModel_CLASS RebarGlobalOptionManager
{
public:
	RebarGlobalOptionManager(void);

	void Initialize();

	~RebarGlobalOptionManager(void);

	void SelectOptionSerialize(CArchive &ar, CString selectOptionName = _T(""));
	void Serialize(CArchive& archive, CString sCurrentOption = _T(""));

	shared_ptr<RebarGlobalOption> GetCurrentGlobalOption();
	shared_ptr<RebarGlobalOption> GetGlobalOption(CString name);

	void SetCurrentOption(CString& optionName);
	void SetDefaultOption();

	CString GetCurrentOptionName();
	void SetCurrentOptionName(CString newName);

	shared_ptr<OrderLengthSettingSet> GetOrderLengthSettingOption(const CString& OrderLengthSettingOptionName );
	shared_ptr<CorrectiveLengthSet> GetCorrectiveLengthOption(const CString& CorrectiveLengthOptionName);	
	shared_ptr<CoverLengthSet> GetCoverLengthOption(const CString& CoverLengthOptionName);
	shared_ptr<DevSpliceLengthSet> GetDevSpliceLengthOption(const CString& DevSpliceLengthOptionName);
	shared_ptr<OrderLengthSet> GetOrderLengthOption(const CString& OrderLengthOptionName);


	vector<shared_ptr<RebarGlobalOption>> m_aGlobalOptions;
	vector<shared_ptr<OrderLengthSettingSet>> m_aOrderLengthSettingSets;
	vector<shared_ptr<OrderLengthSet>> m_aOrderLengthSets;
	vector<shared_ptr<CorrectiveLengthSet>> m_aCorrectiveLengthSets;
	vector<shared_ptr<CoverLengthSet>> m_aCoverLengthSets;
	vector<shared_ptr<DevSpliceLengthSet>> m_aDevSpliceLengthSets;

	bool WriteFileBinary(CString filePath, CString selectOptionName);
	bool ReadFileBinary(CString filePath);
	bool isNumberCheck(CString str);
	//DECLARE_SERIAL(RebarGlobalOptionManager)

	void Copy(RebarGlobalOptionManager* desManager);

private:
	CString m_sCurrentOption;
};

