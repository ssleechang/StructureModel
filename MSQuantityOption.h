#pragma once
#include "msobject.h"

class MSCompoG;

struct StructureModel_CLASS IncreaseFactor
{
	IncreaseFactor() { Init(); };
	double formwork, concrete;
	vector<double> factorByDia; // New
	void Init();
};

struct StructureModel_CLASS AdditionalConcAndForm
{
	AdditionalConcAndForm() { Init(); }
	CString form;
	double concAddLength;
	double formAddPercent;
	CString changedForm;
	void Init();
};

class StructureModel_CLASS MSQuantityOption : public MSObject
{
public:
	MSQuantityOption();
	void Serialize( CArchive& archive );
	void GetFactorForMemberType(MSCompoG* compoG, IncreaseFactor& factor);
	void WeightFactorDataInitCheck();
	DECLARE_SERIAL(MSQuantityOption)

	bool WriteFileBinary(CString strFilePath);
	bool ReadFileBinary(CString strFilePath);

	vector<CString> m_aMemberTypeName;
	vector<IncreaseFactor> m_aFactor;

	int m_nByOptionOrManual; // 0:ByOption, 1:ByManual
	bool m_bSumSOGAsFooting;
	bool m_bIgnoreAreaColumnBeam; // 기둥과 보 접합부 공제하지 않는 조건 적용
	double m_dIgnoreAreaColumnBeam; // 기둥과 보 접합부 공제하지 않는 면적
	bool m_bIgnoreAreaColumnWall;
	double m_dIgnoreAreaColumnWall;
	bool m_bIgnoreAreaGirderBeam;
	double m_dIgnoreAreaGirderBeam;
	
	bool m_bIgnoreOpening; // 오프닝 공제 면적 적용
	double m_dIgnoreOpeningOfWall;
	double m_dIgnoreOpeningOfSlab;

	int m_nBeamUnderCalcMethod; // 슬래브 보 거푸집 산출 0:보하단 산출 1:산출안함
	bool m_bIsSteelQuantityExclude; //SRC에서 false: Steel 물량 포함  true: Steel 물량 제외 

	int m_nApplyBurimConc; // 0:적용, 1:미적용
	CString m_strMaterial;//Material
	CString m_sBurimAreaType; //거푸집타입
	double m_dBurimHeight; // 높이 mm
	double m_dBurimOffset; // 오프셋 mm
	int m_nBackFillMaterial; // 0:By Member, 1:버림콘크리트
	bool m_bBackFillMatLevelZone;
	bool m_bBackFillMatOffset;
	bool m_bBackFillPIT;
	bool m_bIgnoreAreaVMemberUnderSlab; //슬래브하부 수직부재 공제 유무
	vector<int> m_aAppliedFloorNum; //[BH-5831]슬래브 하부 수직부재 공제를 적용 시킬 층
	bool m_bIsIgnoreAreaWallEndFormWork; //벽체 마구리 거푸집 접합부 공제
	bool m_bRebarVolumeExclude; //콘크리트 체적에서 철근체적 공제
	int m_nRebarVolumeExcludeType; // 0: 철근체적 자동계산 1: 철근중량 / 7.85
	double m_dRebarDensityInput; // 7.85 ? 
	bool m_bAdditionalConcreteNForm;
	vector<AdditionalConcAndForm> m_aAdditioanlConcAndForm;
	bool m_bConcDeduction; //시설물 콘크리트 공제
	double m_dConcDeduction;
	int m_nRebarEmbeddedDeckExtra; //데크 여유길이
	int m_nGeneralDeckExtra; //일반 데크 여유길이
	int m_nPCSlabExtra; // PC 슬래브  (IMT만해당) 여유 길이
	int m_nTouchSteelBeam;
	double m_dHaunchOutstretchedLength; //헌치내민길이
	void SetBuilding(MSBaseBuilding* bldg);
	void MakeAppliedFloorNum();
private:
	MSBaseBuilding* mp_owner;	
};