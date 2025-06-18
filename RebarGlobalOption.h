#pragma once

using namespace std;
class RebarGlobalOptionManager;
class OrderLengthSettingSet;
class CorrectiveLengthSet;
class CoverLengthSet;
class DevSpliceLengthSet;
class OrderLengthSet;
class CorrectiveLengthItem;

class StructureModel_CLASS RebarGlobalOption
{
public:
	RebarGlobalOption(){
		m_pManager = nullptr;
	};
	
	RebarGlobalOption(CString Name,RebarGlobalOptionManager* pManager);
	RebarGlobalOption(const RebarGlobalOption& option);
	~RebarGlobalOption(void);
	
	CString m_sName;
	void SetGlobalOptionName(CString m_sName);

	bool SetCorrectiveLengthOption(const CString& name);
	bool SetOrderLengthOption(const CString& name);
	bool SetCoverLengthOption( const CString& m_sName );
	bool SetDevelopmentSpliceLengthSettingOption(const CString& name);
		
	void SetCorrectiveLengthOption(shared_ptr<CorrectiveLengthSet> correctiveLengthOption);
	void SetOrderLengthOption(shared_ptr<OrderLengthSet> orderLengthOption);
	void SetCoverLengthOption(shared_ptr<CoverLengthSet> coverLengthOption);
	void SetDevelopmentSpliceLengthSettingOption(shared_ptr<DevSpliceLengthSet> developmentSpliceLengthOption);

	const shared_ptr<CorrectiveLengthSet> GetCorrectiveLengthOption();
	const shared_ptr<OrderLengthSet> GetOrderLengthOption();
	const shared_ptr<CoverLengthSet> GetCoverLengthOption();
	const shared_ptr<DevSpliceLengthSet> GetDevelopmentSpliceLengthSettingOption();

	const shared_ptr<OrderLengthSet> GetOrderLengthSet(const CString& OrderLengthSetName);
	const CorrectiveLengthItem* GetCorrectiveLengthItem(int eCorLenType);

	void Serialize( CArchive& ar );
	
	void Copy(shared_ptr<RebarGlobalOption> desRebarGlobalOption);

protected:
	RebarGlobalOptionManager* m_pManager;
	shared_ptr<OrderLengthSet> m_pOrderLengthOption;
	shared_ptr<CorrectiveLengthSet> m_pCorrectiveLengthOption;
	shared_ptr<CoverLengthSet> m_pCoverLengthOption;
	shared_ptr<DevSpliceLengthSet> m_pDevelopmentSpliceLengthOption;

};

