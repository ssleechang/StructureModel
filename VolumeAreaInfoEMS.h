#pragma once
#include <string>
#include "StructureModel\MSQuantityOption.h"
using std::string;

class MSCompoG;

class StructureModel_CLASS VolumeAreaInfoEMS
{
public:
	struct FormResult
	{
		vector<CString> formDir; //Side : 측면(Left 좌측, Right 우측), Bot: 바닥, Top: 상부 Ver:슬래브 피트?수직면 Close:마구리면   U,W~Z: 계단  User: 사용자 Burim: 버림 Haunch:헌치 Rib:리브라스 Slope :경사 Add: 거푸집 별 추가 거푸집
		vector<CString> formWorkName;
		vector<double> formArea;
		vector<CString> formNote;
		vector<long> formId;
	};

	static bool _bShowVariable;
	VolumeAreaInfoEMS(MSCompoG* owner);

	void Serialize( CArchive& archive );
	void Dump(EFS::EFS_Buffer& buffer);
	void Recovery(EFS::EFS_Buffer& buffer);
	
	void GetResult(double &rVolume, double& rArea, CString& noteVolume, CString& noteArea);
	void GetQuantityResult(double &rOriginVolume, double &rVolume, CString& noteVolume, FormResult& result, int cz_id, int calcType, double &srcColInnerVolume, CString& srcColInnerNoteVolume, vector<double>& addedConcVolumes, vector<CString>& addedConcNotes, FormResult& addedFormResult, vector<long>& addedMatIds, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid ,vector<double>& arFWOriginArea, double rebarVolume = 0);
	void GetResultByCZone(int cz_id, double &rOriginVolume, double &rVolume, CString& noteVolume, FormResult& result, int calcType, double &srcColInnerVolume, CString& srcColInnerNoteVolume, vector<double>& addedConcVolumes, vector<CString>& addedConcNotes, FormResult& addedFormResult, vector<long>& addedMatIds, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& maitd, vector<double>& arFWOriginArea, double rebarVolume = 0);
	void GetResultByCZoneBurim(int cz_id, double &rVolume, CString& noteVolume, FormResult& result);
	void GetBurimFormWorkAreaByCZone(int czone_id, vector<CString>& arFWAreaDir_burim, vector<CString>& arFWAreaType_burim, vector<double>& arFWArea_burim, vector<CString>& arFormWorkNote_burim);
	void GetResultByCZoneSubHaunch(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList);
	void GetResultByCZoneSubHaunchAll(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList, vector<CString>& haunch_arFWAreaType, vector<double>& haunch_arFWArea, vector<CString>& haunch_arFormWorkNote, vector<int>& haunchID, vector<int>& fwHaunchID, vector<double>& haunch_arFWOriginArea, vector<int>& hauch_bool);
	void GetResultByCZoneBuiltUpBeam(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList);

	void GetResultRebarExcludeVolume(double rebarVolume, double& rVolume, CString& noteVolume);
	void GetResultAdditionalConcAndForm(int czone_id, FormResult formResult, vector<double> arFWOriginArea, vector<double>& rVolumes, vector<CString>& noteVolumes, FormResult& additionalFormResult, vector<long>& addedMatIds);
	void GetResultConcDeduction(double concVolume, double& deductionConcVolume, CString& noteVolume);

	double GetLibLathAreaByCZoneID(int czone_id);

	double GetFactorByCzone(int cz_id);

	double Round( double value, int pos=3);
	void ConvertUnit(int unit[]);
	void ConvertUnitAndPushVector(vector<double> input, int unit);
	void ConvertUnitAndPushDouble(double input, int unit);

	void SetParamColumn(double W, double D, double H, double TO, double BO, double TS, double TB, double LW, double RW, double R, double BCV0, double BCV1, double BCV2, double BCV3, double BH1, double BH2, double CHT, double CHV, double CHA0, double CHA1, double CHA2, double CHA3, double CO, double SUBCHA0, double SUBCHA1, double SUBCHA2, double SUBCHA3);
	void SetParamCustomColumn(vector<double> profileLine, double col_Area, double H, double TO, double BO, double TS, double TB, vector<double> WCL, vector<double> WCV, vector<double> BCV);
	void SetParamBeam(double W, double D, double A, double CL, double LL, double RL, double TS, double ACV, double W1, double W2, double D1, double D2, double INC_BOT, double WAL, double VH, double LACV, double RACV, double LLF, double RLF, double PCCL, vector<double> BUILTUP, map<int, vector<double>> BUILTUP_LEFTAREA, map<int, vector<double>> BUILTUP_RIGHTAREA);
	void SetParamCustomBeam(double A, double TSA, double CL, double LL, double RL, double leftOL, double rightOL, double bottomOL, double WAL);
	void SetParamLowWall(double W, double H, double CL, double L, double TS, double TH, double BH, double TTS, double TLACV, double TRACV, double BLACV, double BRACV);
	void SetParamSlab(double A, double OL, double T, double IA, double IL, double ACV, double V2, double A2, double A2_END, double IAC, double H_FORM,
		double VTR, double VWL, double HUNV, double ACV_OUT, double ACV_IN, double V_INTERSECT, vector<double> HunchV, vector<double> EdgeWallV, vector<double> haunchStretchedVolumes, vector<double> edgeWallHaunchStretchedVolumes);
	void SetParamHalfPCSlab(double A, double OL, double T, double T1, double T2, double IA, double IL, double ACV, double V2, double A2, double A2_END, double IAC, double H_FORM,
		double VTR, double VWL, double HUNV, double ACV_OUT, double ACV_IN, double V_INTERSECT, vector<double> HunchV, vector<double> EdgeWallV, vector<double> haunchStretchedVolumes, vector<double> edgeWallHaunchStretchedVolumes);
	void SetParamIMTPCSlab(double Lx, double Ly, double Sx, double Sy, double L1, double L2, double L3, double H1, double H2, double H3, double ELx, double ELy, double OL, int NR, int NELx, int NELy, double DF);
	void SetParamHCSPCSlab(double Lx, double Ly, double Sx, double Sy, double L1, double L2, double L3, double H1, double H2, double HL, int NH, double FL, double A_bot, double A_open, double T_s, double B_b, double L_b,
		double W_col, double D_col, double R_col);
	void SetParamRPSPCSlab(double Lx, double Ly, double Sx, double Sy, double L1, double L2, double L3, double H1, double H2, double H3, double HL, double FL, double A_bot, double A_open, double T_s, double B_b, double L_b,
		double W_col, double D_col, double R_col,int NH);
	void SetParamRebarEmbeddedSlab(double A, double OL, double T, double IA, double IL, double ACV, double V2, double A2, double A2_END, double IAC, double H_FORM,
		double VTR, double VWL, double HUNV, double SX, double SY, double ACV_OUT, double ACV_IN, double V_INTERSECT, vector<double> HunchV, vector<double> EdgeWallV, vector<double> haunchStretchedVolumes, vector<double> edgeWallHaunchStretchedVolumes);
	void SetParamWall(double T, double H, double A, double TO, double BO, double TS, double CL, double LL, double RL, double IW, double IH, double LACV, double RACV, double IE1, double IE2, double ACVE1, double ACVE2, double IWC, double IHC, double IWF, double IHF, double NUM_O);
	void SetParamIsoFoot(double W, double D, double T, double TB, double ACV, double ACV_BOT, double VCUT, vector<double> H_FORM, vector<double> HV, vector<CString> HDir, vector<double> haunchStretchedVolumes);
	void SetParamMat(double A, double OL, double T, double IA, double IL, double ACV, double V2, double A2, double A2_END, double IAC, double AB, double TB, double H_FORM,
		double VTR, double VWL, double HUNV, double ACV_OUT, double ACV_IN, double V_INTERSECT, vector<double> HunchV, vector<double> EdgeWallV, vector<double> haunchStretchedVolumes, vector<double> edgeWallHaunchStretchedVolumes);
	void SetParamGeneralDeck(double SX, double SY, double LX, double LY, double OA, double T, double FT, double FBW, double FTW, double FS, double DF);
	void SetParamWallFoot(double W, double H, double CL, double LL, double RL, double TS, double ACV, double IE1, double IE2, vector<double> H_FORM, vector<double> HV, vector<CString> HDir, vector<double> haunchStretchedVolumes);
	void SetParamPit(double A, double OL, double H1, double PA, double POL, double H2, double WA, double WOL, double H3, double ACV, double HUN, double HUN_V, vector<double> HunchV, vector<double> haunchStretchedVolumes);
	void SetParamStair(double CA1, double T1, double CA2, double T2, double L1, double W1, double H1, double N1, double ST1,
		double L2, double W2, double H2, double N2, double ST2, double ACV);
	void SetParamStair(vector<double> LANDING, vector<double>STEP_PARAM);
	void SetParamRamp(double V, double BA, double VA);
	void SetParamMainHaunch(double W, double H, double SBL, double STL, double HSL, double VCUT, double ACV_SIDE, double ACV_SLOPE, double IE1, double IE2);
	void AddSubHaunchQuantityAll(int matlID, double volume, CString note, CString haunchFormWorkName, double area, CString areanote, int haunchID, int czone_id);
	void AddEdgeWallSubHaunchQuantityAll(int matlID, double volume, CString note, CString haunchFormWorkName, double area, CString areanote, int haunchID, vector<int> czone_ids);
	void SetSubHaunchCZoneIds(vector<int> czone_ids);
	CString ChangeVolumeInfoNoVariable(CString note);
	CString ReplaceStringInfo(CString note, CString replaceitem);
	void AddBuiltUpBeamQuantity(int matlID, double volume, CString note, int builtupBeamID);
	//void AddSubEgewallQuantityAll(int matID, double volume, CString note, int haunchID);

#pragma region Clear
	void ClearAll();
	void ClearCZoneInfo();
	void ClearSubHaunchQuantityAll();
	void ClearBuiltUpBeamQuantity();
#pragma endregion

	// Quantity Data
	vector<double> ma_Param;

	// CZone Info
	vector<int> ma_CZoneID;
	vector<double> ma_Factor;
	vector<double> ma_LibLathArea;

	// Haunch Quantity - V
	vector<int> ma_HaunchID;
	vector<int> ma_HaunchMatlID;
	vector<double> ma_HaunchVolume;
	vector<CString> ma_HaunchNote;

	//HaunchFormwork - A
	vector<CString> ma_HaunchFormType;
	vector<double> ma_HaunchFormArea;
	vector<CString> ma_HaunchFormNote;
	vector<int> ma_Hanchczone;

	//BuiltupBeamQuantity - V
	vector<int> ma_BuiltUpBeamID;
	vector<int> ma_BuiltUpBemaMatlID;
	vector<double> ma_BuiltupBeamVolume;
	vector<CString> ma_BuiltUpBeamNote;
	//BuiltupBeamQuantity - A
	map<int, vector<double>> ma_BuiltUpBeamLeftACV;
	map<int, vector<double>> ma_BuiltUpBeamRightACV;

private:
	MSCompoG* mp_Owner;



	void GetResultByCZoneColumn(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, double &srcColInnerVolume, CString& srcColInnerNoteVolume);
	void GetResultByCZoneNormalColumn(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, double &srcColInnerVolume, CString& srcColInnerNoteVolume);
	void GetResultByCZoneCustomColumn(int cz_id, double &rVolume, CString& noteVolume, FormResult& result);
	void GetResultByCZoneBeam(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes);
	void GetResultByCZoneNormalBeam(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes);
	void GetResultByCZoneCustomBeam(int cz_id, double &rVolume, CString& noteVolume, FormResult& result);
	void GetResultByCZoneLowWall(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType);
	void GetResultByCZoneSlab(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid);
	void GetResultByCZoneWall(int cz_id, double &rVolume, CString& noteVolume, FormResult& result);
	void GetResultByCZoneIsoFoot(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid);
	void GetResultByCZoneMat(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid);
	void GetResultByCZoneRPSPCSlab(int cz_id, double & rVolume, CString & noteVolume, FormResult & result, int calcType);
	void GetResultByCZoneIMTPCSlab(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType);
	void GetResultByCZoneHCSPCSlab(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType);
	void GetResultByCZoneGeneralDeck(int cz_id, double &rVolume, CString& noteVolume, FormResult& result);
	void GetResultByCZoneWallFoot(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid);
	void GetResultByCZonePit(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid);
	void GetResultByCZoneStair(int cz_id, double &rVolume, CString& noteVolume, FormResult& result);
	void GetResultByCZoneRamp(int cz_id, double &rVolume, CString& noteVolume, FormResult& result);
	void GetResultByCZoneMainHaunch(int cz_id, double &rVolume, CString& noteVolume, FormResult& result);
	void ApplyIncreaseFactor(IncreaseFactor* factor, double& rVolume, vector<double>& arFWArea);
	void ApplyIncreaseFactor(IncreaseFactor* factor, vector<double>& arVolumes);
	void ApplyIncreaseFactor(IncreaseFactor* factor, vector<double>& arVolumes, vector<double>& arFWArea);
};