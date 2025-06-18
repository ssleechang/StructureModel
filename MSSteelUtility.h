#pragma once
#include "afx.h"

#ifndef _MSSteelUtility_HeaderFile
#define _MSSteelUtility_HeaderFile

class MSSteelSection;
class MSSteelFileInfo;
#include "StructureModelEnum.h"

class StructureModel_CLASS MSSteelUtility : public CObject
{
public:

	MSSteelUtility();
	~MSSteelUtility(void);

	DECLARE_SERIAL(MSSteelUtility)
public:
	static long m_nSteelCode;
	static void SetSteelCode(long nSteelCode){m_nSteelCode=nSteelCode;};
	static long GetSteelCode(){return m_nSteelCode;};
// 	static MSSteelUtility* m_pSteelUtil;
// 	static MSSteelUtility* GetSteelUtili();
//		static void DeleteSteelUtil();
	MSSteelSection* GetSection(long nSectionType, long nSectionID);
	MSSteelSection* GetSection(long nSectionType, CString name);
	MSSteelSection* GetSection_Kr(long nSectionType, long nSectionID);
	MSSteelSection* GetSection_Kr(long nSectionType, CString name);
	MSSteelSection* FindOrBuiltUpSection(long nSectionType, vector<double>& valueArr);

	MSSteelSection* GetDefaultHSection();
	void CreateDefaultHSection();

	MSSteelSection* BuiltUpSteelSection(long type, vector<double>& valueArr);
	long MakeSections(msSteelSecType type, vector<MSSteelFileInfo*>& datas);
	bool CheckSteelSection(long type, MSSteelSection* section);
	MSSteelSection* IsSectionExist(long type, MSSteelSection* section, bool findAll = false);
	bool IsMakeBuiltUpSection(CString steelName);

	vector<MSSteelSection*> GetSectionList(msSteelSecType type);
	long GetSectionType(CString name);

	map<long, vector<MSSteelSection*>> mm_AddSection;
	void ClearAddSections();
	vector<double> Parsing_SteelName(CString steelName);
	int ParsingTextData(CString text, vector<CString>& dataArr, CString parser);

	static XErrorObject* CreateErrorObject(long nErrorCode, long nLevel, CString strMsg, XErrorObject::SelectType type);
protected:

	MSSteelSection* GetSection_Kr(long nSectionID, vector<MSSteelSection*>& SectionArr);
	MSSteelSection* GetSection_Kr(CString name, vector<MSSteelSection*>& SectionArr);

	// 엑셀파일 읽어서 만든 MSSteelFileInfo로 각 SteelSection생성 함수
	long MakeSections_HSectionKr		(vector<MSSteelFileInfo*>& datas);
	long MakeSections_RectPipeKr		(vector<MSSteelFileInfo*>& datas);
	long MakeSections_RoundPipeKr	(vector<MSSteelFileInfo*>& datas);
	long MakeSections_TSectionKr		(vector<MSSteelFileInfo*>& datas);
	long MakeSections_CSectionKr		(vector<MSSteelFileInfo*>& datas);
	long MakeSections_DAngleKr		(vector<MSSteelFileInfo*>& datas);
	long MakeSections_AngleKr			(vector<MSSteelFileInfo*>& datas);
	long MakeSections_DeckPlateKr	(vector<MSSteelFileInfo*>& datas);

	// 메모리 해제
	void DeleteSections();
	void DeleteSections(vector<MSSteelSection*>& SectionArr);

	// BuiltUp
	MSSteelSection* BuiltUpSection_HSectionKr		(vector<double>& valueArr);
	MSSteelSection* BuiltUpSection_RectPipeKr			(vector<double>& valueArr);
	MSSteelSection* BuiltUpSection_RoundPipeKr		(vector<double>& valueArr);
	MSSteelSection* BuiltUpSection_TSectionKr			(vector<double>& valueArr);
	MSSteelSection* BuiltUpSection_CSectionKr		(vector<double>& valueArr);
//		MSSteelSection* BuiltUpSection_DAngleKr			(vector<double>& valueArr);
	MSSteelSection* BuiltUpSection_AngleKr				(vector<double>& valueArr);
//		MSSteelSection* BuiltUpSection_DeckPlateKr		(vector<double>& valueArr);

	// Check
	bool CheckSection_HSectionKr			(MSSteelSection* pSection);
	bool CheckSection_RectPipeKr			(MSSteelSection* pSection);
	bool CheckSection_RoundPipeKr		(MSSteelSection* pSection);
	bool CheckSection_TSectionKr		(MSSteelSection* pSection);
	bool CheckSection_CSectionKr			(MSSteelSection* pSection);
	//		bool CheckSection_DAngleKr			(MSSteelSection* pSection);
	bool CheckSection_AngleSectionKr		(MSSteelSection* pSection);
	//		bool CheckSection_DeckPlateKr		(MSSteelSection* pSection);

	// Exist
	MSSteelSection* ExistSection_HSectionKr				(MSSteelSection* pSection, bool findAll);
	MSSteelSection* ExistSection_RectPipeKr				(MSSteelSection* pSection, bool findAll);
	MSSteelSection* ExistSection_RoundPipeKr			(MSSteelSection* pSection, bool findAll);
	MSSteelSection* ExistSection_TSectionKr			(MSSteelSection* pSection, bool findAll);
	MSSteelSection* ExistSection_CSectionKr				(MSSteelSection* pSection, bool findAll);
	//		bool ExistSection_DAngleKr			(MSSteelSection* pSection, bool findAll);
	MSSteelSection* ExistSection_AngleSectionKr		(MSSteelSection* pSection, bool findAll);
	//		bool ExistSection_DeckPlateKr		(MSSteelSection* pSection, bool findAll);

	int m_nErrorCode;
	void AddSection(long type, MSSteelSection* pSection);
	bool IsSameValue(double myVal, double urVal, double tolerance = DTOL_GM);
protected:
	vector<MSSteelSection*> ma_HSectionKr;
	vector<MSSteelSection*> ma_RectPipeKr;
	vector<MSSteelSection*> ma_RoundPipeKr;
	vector<MSSteelSection*> ma_TSectionKr;
	vector<MSSteelSection*> ma_CSectionKr;
	vector<MSSteelSection*> ma_DAngleKr;
	vector<MSSteelSection*> ma_AngleKr;
	vector<MSSteelSection*> ma_DeckPlateKr;

};

#endif