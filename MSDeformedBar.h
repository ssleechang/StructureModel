#pragma once


enum KS_DBar_Kind {KS_DBar_D6 , KS_DBar_D10, KS_DBar_D13, KS_DBar_D16, KS_DBar_D19,
KS_DBar_D22, KS_DBar_D25, KS_DBar_D29, KS_DBar_D32, KS_DBar_D35, KS_DBar_D38,
KS_DBar_D41, KS_DBar_D51, KS_DBar_Last};

enum ASTM_DBar_Kind {ASTM_DBar_D3 , ASTM_DBar_D4, ASTM_DBar_D5, ASTM_DBar_D6, ASTM_DBar_D7,
ASTM_DBar_D8, ASTM_DBar_D9, ASTM_DBar_D10, ASTM_DBar_D11, ASTM_DBar_D14, ASTM_DBar_D18, ASTM_DBar_Last};

enum GB_DBar_Kind {GB_DBar_D55, GB_DBar_D6, GB_DBar_D65, GB_DBar_D8, GB_DBar_D10, GB_DBar_D12, GB_DBar_D14,
GB_DBar_D16, GB_DBar_D18, GB_DBar_D20, GB_DBar_D22, GB_DBar_D25, GB_DBar_D28,
GB_DBar_D32, GB_DBar_D36, GB_DBar_D40, GB_DBar_D50, GB_DBar_Last
};

enum BS_DBar_Kind {BS_DBar_D6 , BS_DBar_D8, BS_DBar_D10, BS_DBar_D12, BS_DBar_D14, BS_DBar_D16, BS_DBar_D18,
BS_DBar_D20, BS_DBar_D25, BS_DBar_D32, BS_DBar_D40, BS_DBar_D50, BS_DBar_Last};

enum VIETNAM_DBar_Kind { VIETNAM_DBar_D6, VIETNAM_DBar_D8, VIETNAM_DBar_D10, VIETNAM_DBar_D12, VIETNAM_DBar_D13, 
VIETNAM_DBar_D14, VIETNAM_DBar_D16, VIETNAM_DBar_D18, VIETNAM_DBar_D19, VIETNAM_DBar_D20, VIETNAM_DBar_D22, 
VIETNAM_DBar_D25, VIETNAM_DBar_D28, VIETNAM_DBar_D29, VIETNAM_DBar_D32, VIETNAM_DBar_D35, VIETNAM_DBar_D36, 
VIETNAM_DBar_D38, VIETNAM_DBar_D40, VIETNAM_DBar_D41, VIETNAM_DBar_D43, VIETNAM_DBar_D51, VIETNAM_DBar_Last };

enum INDIA_DBar_Kind {INDIA_DBar_D8, INDIA_DBar_D10, INDIA_DBar_D12, INDIA_DBar_D16, INDIA_DBar_D20,
	INDIA_DBar_D25, INDIA_DBar_D28, INDIA_DBar_D32, INDIA_DBar_D36, INDIA_DBar_D40, INDIA_DBar_Last};

enum SINGAPORE_DBar_Kind {SINGAPORE_DBar_D8, SINGAPORE_DBar_D10, SINGAPORE_DBar_D13, SINGAPORE_DBar_D16,  
	SINGAPORE_DBar_D20, SINGAPORE_DBar_D22,	SINGAPORE_DBar_D25, SINGAPORE_DBar_D28, SINGAPORE_DBar_D32, SINGAPORE_DBar_D40, 
	SINGAPORE_DBar_D50, SINGAPORE_DBar_Last};

enum JIS_DBar_Kind {
	JIS_DBar_D10, JIS_DBar_D13, JIS_DBar_D16, JIS_DBar_D19,
	JIS_DBar_D22, JIS_DBar_D25, JIS_DBar_D29, JIS_DBar_D32, JIS_DBar_D35, JIS_DBar_D38,
	JIS_DBar_D41, JIS_DBar_D51, JIS_DBar_Last
};

enum NATION_CODE{NATION_CODE_KOREA=0, NATION_CODE_UAE};

class StructureModel_CLASS MSDeformedBar
{
public:
	static long sm_nRebarCode;
	static long sm_nNationCode;

	enum // RebarKind
	{
		//CheckPre Error Code
		REBAR_MATERIAL_KS = 0,
		REBAR_MATERIAL_ASTM = 1,
		REBAR_MATERIAL_GB = 2,
		REBAR_MATERIAL_BS = 3,
		REBAR_MATERIAL_VIETNAM = 4,
		REBAR_MATERIAL_INDIA = 5,
		REBAR_MATERIAL_SINGAPORE = 6,
		REBAR_MATERIAL_JIS = 7,
	};

	double m_Weight;
	double m_Dia;
	double m_Area;
	double m_Peri;

	static CString m_sDefaultBarType;
public:
	static double KS_DBars[KS_DBar_Last][4];
	static double ASTM_DBars[ASTM_DBar_Last][4];
	static double GB_DBars[GB_DBar_Last][4];
	static double BS_DBars[BS_DBar_Last][4];
	static double VIETNAM_DBars[VIETNAM_DBar_Last][4];
	static double INDIA_DBars[INDIA_DBar_Last][4];
	static double SINGAPORE_DBars[SINGAPORE_DBar_Last][4];
	static double JIS_DBars[JIS_DBar_Last][4];

	MSDeformedBar();
	MSDeformedBar(CString DiaName);
	virtual ~MSDeformedBar();

	enum BAR_SPLICE {NON_SPLICE = 0, HALF_SPLICE, FULL_SPLICE};
	long GetNumBarsInOneLayer(double Width, long SpliceType, double MinPitch = 25);

	static double GetMinDia();
	static double GetMaxDia();

	static long GetRebarNameList(CStringArray& arNameList);
	static long GetRebarColorNameList(CStringArray& arColorList);
	static CString GetDefaultRebarNamePrefix(long BarType=1);
	static CString FindBarTypeFromID(long BarType);
	static long FindBarTypeFromString(CString AString);

	static bool IsCorrectRebarName(CString strName);
	static void SetRebarCode(long nRebarCode);
	static long GetRebarCode();
	static void SetNationCode(long nNationCode);
	static long GetRebarCodeKindList(CStringArray& rebarCodeKindList);

	static CString FindBarDiameterName(double dDeformedDia);
	static CString FindBarDiameterNameByIndex(int index);
	static double FindDeformedBarDiameterByIndex(int index);
	static int FindBarDiameterIndexByName(CString sDiaName);
	static int FindBarDiameterIndexByDia(double dDeformedDia);
	static double FindDeformedBarDiameterByName(CString sDiaName);

	static double GetDeformedBarDiameter(CString strBar);
	static CString GetBarDiameterName(CString strBar);

	static double GetBarSpacing(CString strBar);
	static long GetBundleNum(CString strBar);
	static int         GetNumBars(CString strBar);

	static double GetDeformedBarDiameter4StirrupBar(CString strBar);
	static double GetDeformedBarDiameter4MainBar(CString strBar);
	static bool IsIncludePrefix(CString strBar, vector<CString>& prefixes);
	static int GetStringIndex(CString strBar, vector<CString>& prefixes);

	static CString GetBarDiameterName4StirrupBar(CString strBar);
	static CString GetBarDiameterName4MainBar(CString strBar);

	static vector<CString> GetRebarPrefixNames();
	static vector<CString> GetRebarPrefixSpacings();
	static vector<CString> GetRebarPrefixBarNums();
	static CString GetDefaultRebarName();
	static bool ChinaDeformedBarCheck();
	static vector<CString> GetParserNames();

	static double GetWeightByName(CString sDiaName);
	static long GetBundleSpace(CString strBar);
	static CString GetBundleSpaceRemoveStr(CString strBar);

	static double GetDefaultDia();
	static CString GetMainBarStr(long numBar, double dDia = 0, CString sBarType = _T("D"));
	static CString GetStirrupBarStr(long nSpacing, double dDia = 0, CString sBarType = _T("D"));

	static bool IsUserSteelGradeS(CString str);
	static bool EnabledUserSteelGradeS(CString name);
private:
	static CString GetDiaStr(double dDia);
};
