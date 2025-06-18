#pragma once
#include <string>
using std::string;

class MSCompoG;

class StructureModel_CLASS VolumeAreaInfoEMS_NewEMS
{
public:

	// Quantity Data by CZoneID, Type
	struct QuantityData
	{
		struct VariableData
		{
			CString Type;	// 분류
			int Index;		// 선택 단위
			CString Name;	// 변수명
			vector<double> Values;	// 변수값
		};
		///////////////////////////////////////
		struct Concrete
		{
			double Volume = 0;	// 콘크리트 물량
			CString Note;		// 콘크리트 산식
			vector<VariableData> Variables;	// 콘크리트 물량 변수
		};

		struct Formwork
		{
			// Side : 측면(Left 좌측, Right 우측), Bot: 바닥, Top: 상부 Ver:슬래브 피트?수직면 Close:마구리면   W~Z: 계단  User: 사용자 Burim: 버림 Haunch:헌치 Rib:리브라스 Slope :경사 Add: 거푸집 별 추가 거푸집
			CString Dir;
			CString Name;		// 거푸집 이름 (합판 3회 등)
			double Area = 0;	// 거푸집 물량
			CString Note;		// 거푸집 산식
			vector<VariableData> Variables;	// 거푸집 물량 변수
		};

		Concrete Conc;
		vector<Formwork> Form;
	};

private:

	// Key.first: CZoneID
	// Key.second: Type. (0: 전체 / 1: Bottom(난간벽), RC Part(PC부재) / 2: Top(난간벽), PC Part(PC부재))
	// MSAcisQuantityCalculator에서 물량계산시 가능한 모든 Type에 대해 데이터를 계산해 놓아야 한다.
	map<std::pair<int, int>, QuantityData> mp_Params;

public:

	static bool _bShowVariable;
	VolumeAreaInfoEMS_NewEMS(MSCompoG* owner);

	void Clear();
	vector<int> GetCZoneIDs();

	void AddQuantityData(int cZoneID, int calcType, QuantityData data);
	bool GetQuantityData(int cZoneID, int calcType, QuantityData& data);

	bool GetVariableData(int cZoneID, int calcType, vector<QuantityData::VariableData>& variables);
	vector<CString> GetVariableType(int cZoneID, int calcType);
	vector<int> GetVariableIndex(int cZoneID, int calcType, CString vType);
	vector<CString> GetVariableName(int cZoneID, int calcType, CString vType, int vIndex);
	vector<double> GetVariableValue(int cZoneID, int calcType, CString vType, int vIndex, CString vName);

	void AddVariableData(QuantityData::Concrete& conc, CString vType, CString vName, double value);
	void AddVariableData(QuantityData::Concrete& conc, CString vType, int vIndex, CString vName, double value);
	void AddVariableData(QuantityData::Formwork& form, CString vType, CString vName, double value);
	void AddVariableData(QuantityData::Formwork& form, CString vType, int vIndex, CString vName, double value);

	// 모든 물량, 변수를 CString으로 변환해서 리턴
	vector<CString> GetQuantityString();

private:
	MSCompoG* mp_Owner;

};

