#include "stdafx.h"
#include "VolumeAreaInfoEMS_NewEMS.h"

bool VolumeAreaInfoEMS_NewEMS::_bShowVariable = true;
VolumeAreaInfoEMS_NewEMS::VolumeAreaInfoEMS_NewEMS(MSCompoG* owner)
{
	mp_Owner = owner;
}

void VolumeAreaInfoEMS_NewEMS::Clear()
{
	for (auto iter = mp_Params.begin(); iter != mp_Params.end(); iter++)
	{
		iter->second.Conc.Volume = 0;
		iter->second.Conc.Note.Empty();
		iter->second.Conc.Variables.clear();
		iter->second.Form.clear();
	}
	mp_Params.clear();
}

vector<int> VolumeAreaInfoEMS_NewEMS::GetCZoneIDs()
{
	vector<int> cZoneIDs;
	for (auto iter = mp_Params.begin(); iter != mp_Params.end(); iter++)
		cZoneIDs.push_back(iter->first.first);
	
	// vector 중복제거
	sort(cZoneIDs.begin(), cZoneIDs.end());
	auto end = unique(cZoneIDs.begin(), cZoneIDs.end());
	cZoneIDs.erase(end, cZoneIDs.end());

	return cZoneIDs;
}

void VolumeAreaInfoEMS_NewEMS::AddQuantityData(int cZoneID, int calcType, QuantityData data)
{
	std::pair<int, int> keyPair = std::make_pair(cZoneID, calcType);
	mp_Params[keyPair] = data;
}

bool VolumeAreaInfoEMS_NewEMS::GetQuantityData(int cZoneID, int calcType, QuantityData& data)
{
	std::pair<int, int> keyPair = std::make_pair(cZoneID, calcType);
	if (mp_Params.find(keyPair) == mp_Params.end())
		return false;

	data = mp_Params[keyPair];
	return true;
}

bool VolumeAreaInfoEMS_NewEMS::GetVariableData(int cZoneID, int calcType, vector<QuantityData::VariableData>& variables)
{
	variables.clear();

	QuantityData data;
	if (GetQuantityData(cZoneID, calcType, data) == false)
		return false;

	variables.insert(variables.end(), data.Conc.Variables.begin(), data.Conc.Variables.end());
	for (QuantityData::Formwork f : data.Form)
		variables.insert(variables.end(), f.Variables.begin(), f.Variables.end());

	// 변수값이 하나라도 있으면 true 반환
	for (QuantityData::VariableData var : variables)
		if (var.Values.size() != 0)
			return true;

	variables.clear();
	return false;
}

vector<CString> VolumeAreaInfoEMS_NewEMS::GetVariableType(int cZoneID, int calcType)
{
	vector<CString> typeList;
	vector<QuantityData::VariableData> variables;
	if (GetVariableData(cZoneID, calcType, variables) == false)
		return typeList;

	for (QuantityData::VariableData var : variables)
	{
		// 넣으면서 중복체크를 해준다.
		bool isUnique = true;
		for (CString type : typeList)
			if (type == var.Type)
			{
				isUnique = false;
				break;
			}

		if (isUnique)
			typeList.push_back(var.Type);
	}

	return typeList;
}

vector<int> VolumeAreaInfoEMS_NewEMS::GetVariableIndex(int cZoneID, int calcType, CString vType)
{
	vector<int> indexList;
	vector<QuantityData::VariableData> variables;
	if (GetVariableData(cZoneID, calcType, variables) == false)
		return indexList;

	for (QuantityData::VariableData var : variables)
	{
		// 넣으면서 중복체크를 해준다.
		bool isUnique = true;
		for (int index : indexList)
			if (index == var.Index)
			{
				isUnique = false;
				break;
			}

		if (isUnique)
			indexList.push_back(var.Index);
	}

	return indexList;
}

vector<CString> VolumeAreaInfoEMS_NewEMS::GetVariableName(int cZoneID, int calcType, CString vType, int vIndex)
{
	vector<CString> nameList;
	vector<QuantityData::VariableData> variables;
	if (GetVariableData(cZoneID, calcType, variables) == false)
		return nameList;

	for (QuantityData::VariableData var : variables)
	{
		if (var.Type != vType || var.Index != vIndex)
			continue;

		// 넣으면서 중복체크를 해준다.
		bool isUnique = true;
		for (CString name : nameList)
			if (name == var.Name)
			{
				isUnique = false;
				break;
			}

		if (isUnique)
			nameList.push_back(var.Name);
	}

	return nameList;
}

vector<double> VolumeAreaInfoEMS_NewEMS::GetVariableValue(int cZoneID, int calcType, CString vType, int vIndex, CString vName)
{
	vector<double> valueList;
	vector<QuantityData::VariableData> variables;
	if (GetVariableData(cZoneID, calcType, variables) == false)
		return valueList;

	for (QuantityData::VariableData var : variables)
	{
		if (var.Type != vType || var.Index != vIndex || var.Name != vName)
			continue;

		for (double value : var.Values)
			valueList.push_back(value);
	}

	return valueList;
}

void VolumeAreaInfoEMS_NewEMS::AddVariableData(QuantityData::Concrete& conc, CString vType, CString vName, double value)
{
	AddVariableData(conc, vType, 0, vName, value);
}

void VolumeAreaInfoEMS_NewEMS::AddVariableData(QuantityData::Concrete& conc, CString vType, int vIndex, CString vName, double value)
{
	if (value < -DTOL_GM)
		return;

	bool bNewData = true;
	for (int i = 0; i < conc.Variables.size(); i++)
	{
		// VariableData는 스택에 쌓이기 때문에 아래와 같이 vData를 정의 후 vData.Values.push_back(value)를 하더라도 conc.Variables[i]에 추가되지 않기 때문에 직접 추가한다.
		//QuantityData::VariableData vData = conc.Variables[i];
		if (conc.Variables[i].Type == vType && conc.Variables[i].Index == vIndex && conc.Variables[i].Name == vName)
		{
			conc.Variables[i].Values.push_back(value);
			bNewData = false;
			break;
		}
	}

	if (bNewData)
	{
		QuantityData::VariableData vData;
		vData.Type = vType;
		vData.Index = vIndex;
		vData.Name = vName;
		vData.Values.push_back(value);
		conc.Variables.push_back(vData);
	}
}

void VolumeAreaInfoEMS_NewEMS::AddVariableData(QuantityData::Formwork& form, CString vType, CString vName, double value)
{
	AddVariableData(form, vType, 0, vName, value);
}

void VolumeAreaInfoEMS_NewEMS::AddVariableData(QuantityData::Formwork& form, CString vType, int vIndex, CString vName, double value)
{
	if (value < -DTOL_GM)
		return;

	bool bNewData = true;
	for (int i = 0; i < form.Variables.size(); i++)
	{
		// VariableData는 스택에 쌓이기 때문에 아래와 같이 vData를 정의 후 vData.Values.push_back(value)를 하더라도 form.Variables[i]에 추가되지 않기 때문에 직접 추가한다.
		//QuantityData::VariableData vData = form.Variables[i];
		if (form.Variables[i].Type == vType && form.Variables[i].Index == vIndex && form.Variables[i].Name == vName)
		{
			form.Variables[i].Values.push_back(value);
			bNewData = false;
			break;
		}
	}

	if (bNewData)
	{
		QuantityData::VariableData vData;
		vData.Type = vType;
		vData.Index = vIndex;
		vData.Name = vName;
		vData.Values.push_back(value);
		form.Variables.push_back(vData);
	}
}

// 모든 물량, 변수를 CString으로 변환해서 리턴
vector<CString> VolumeAreaInfoEMS_NewEMS::GetQuantityString()
{
	vector<CString> strList;

	for (auto itr = mp_Params.begin(); itr != mp_Params.end(); ++itr)
	{
		CString str;
		CString tmp;

		int cZoneID = itr->first.first;
		int calcType = itr->first.second;
		QuantityData data = itr->second;

		tmp.Format(_T("CZoneID: %d, CalcType: %d\n"), cZoneID, calcType);
		str += tmp;
		str += _T("Type, Index, Name, Value\n");

		// Concrete
		tmp.Format(_T("Conc / %g / %s\n"), data.Conc.Volume, data.Conc.Note);
		str += tmp;
		for (auto concVar : data.Conc.Variables)
		{
			tmp.Format(_T("  %s, %d, %s"), concVar.Type, concVar.Index, concVar.Name);
			str += tmp;
			for (double value : concVar.Values)
			{
				tmp.Format(_T(", %g"), value);
				str += tmp;
			}
			str += _T("\n");
		}

		// Formwork
		for (auto f : data.Form)
		{
			tmp.Format(_T("Form / %s / %s / %g / %s\n"), f.Dir, f.Name, f.Area, f.Note);
			str += tmp;
			for (auto formVar : f.Variables)
			{
				tmp.Format(_T("  %s, %d, %s"), formVar.Type, formVar.Index, formVar.Name);
				str += tmp;
				for (double value : formVar.Values)
				{
					tmp.Format(_T(", %g"), value);
					str += tmp;
				}
				str += _T("\n");
			}
		}

		strList.push_back(str);
	}

	return strList;
}

