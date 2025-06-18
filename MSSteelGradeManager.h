#pragma once
#include "msobject.h"
class StructureModel_CLASS MSSteelGradeManager : public MSObject
{
public:
	MSSteelGradeManager(void);
	~MSSteelGradeManager(void);

	enum eSteelGrade { SD300 = 0, SD350, SD400, SD400S, SD400W, SD500, SD500S, SD500W, SD600, SD600S, SD700,
	                                  Fe415 = 100, Fe415D, Fe500, Fe500D, Fe550, Fe550D, Fe600, CRS, /*for INDIA*/
									  B485B = 200, B500B, B600A, B600B,	/* for SINGAPORE*/
									  HPB235 = 300, HPB300, HRB335, HRBF335, HRB400, HRBF400, RRB400, HRB500, HRBF500, /*for GB*/
									  SD295 = 400, SD345, SD390, SD490, /* for JIS*/
									  NONE = 10000,
									};
	enum ePrefix			{ Prefix_D=0, Prefix_HD, Prefix_SHD, Prefix_UHD, Prefix_USER};

	void Serialize( CArchive& archive );
	void GlobalOptionSerialize(CArchive &ar);

	DECLARE_SERIAL(MSSteelGradeManager)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	int MakeDefaultDataByRebarCode(int rebarCode);
	map<eSteelGrade, CString> GetSGradePrefixes(){return mm_SteelGradePrefix;};
	void ClearSGradePrefixMap()	{ mm_SteelGradePrefix.clear();};
	void SetSGradePrefixes(map<eSteelGrade, CString>& newPrefixMap);

	CString GetPrintName(eSteelGrade SGrade, bool bChinaUsed = false);
	eSteelGrade GetSGrade(CString printName);
protected:
	int MakeDefaultDataByRebarCode_Kr();
	int MakeDefaultDataByRebarCode_ASTM();
	int MakeDefaultDataByRebarCode_GB();
	int MakeDefaultDataByRebarCode_BS();
	int MakeDefaultDataByRebarCode_VIETNAM();
	int MakeDefaultDataByRebarCode_INDIA();
	int MakeDefaultDataByRebarCode_SINGAPORE();
	int MakeDefaultDataByRebarCode_JIS();
protected:
	map<eSteelGrade, CString> mm_SteelGradePrefix;


};