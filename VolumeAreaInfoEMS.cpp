#include "StdAfx.h"
#include "VolumeAreaInfoEMS.h"
#include "GMLib/MSVersionInfo.h"
#include "GMLib/AcisGeomHelper.h"
#include "MSObject.h"
#include "MSCompoG.h"
#include "MSSlabG.h"
#include "MSPitG.h"
#include "MSFloor.h"
#include "MSConstructionZone.h"
#include "MSSteelSection.h"
#include "MSBaseBuilding.h"
#include "MSHandRailG.h"
#include "MSRcSlabM.h"
#include "MSSrcColumnM.h"
#include "MSSteelRoundPipeKr.h"
#include "MSBuiltUpBeamG.h"
#include "StructureModel\MSQuantityOption.h"
#include "StructureModel\MSRcFaceMemberM.h"
#include "MSRebarBuilding.h"
#include "StructureModel/MSColumnHeaderM.h"

bool VolumeAreaInfoEMS::_bShowVariable = true;
VolumeAreaInfoEMS::VolumeAreaInfoEMS(MSCompoG* owner)
{
	mp_Owner = owner;
}

double VolumeAreaInfoEMS::GetFactorByCzone(int cz_id)
{

	auto result = find(ma_CZoneID.begin(), ma_CZoneID.end(), cz_id);

	if (result == ma_CZoneID.end())
		return -1;

	auto index = result - ma_CZoneID.begin();
	return ma_Factor[index];
}

void VolumeAreaInfoEMS::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		int count = ma_Param.size();
		ar << count;
		for (int i = 0; i < ma_Param.size(); ++i)
			ar << ma_Param[i];

		count = ma_CZoneID.size();
		ar << count;
		for (int i = 0; i < ma_CZoneID.size(); ++i)
			ar << ma_CZoneID[i];

		count = ma_Factor.size();
		ar << count;
		for (int i = 0; i < ma_Factor.size(); ++i)
			ar << ma_Factor[i];

		count = ma_LibLathArea.size();
		ar << count;
		for (int i = 0; i < ma_LibLathArea.size(); ++i)
			ar << ma_LibLathArea[i];

		count = ma_HaunchMatlID.size();
		ar << count;
		for (int i = 0; i < ma_HaunchMatlID.size(); ++i)
			ar << ma_HaunchMatlID[i];

		count = ma_HaunchVolume.size();
		ar << count;
		for (int i = 0; i < ma_HaunchVolume.size(); ++i)
			ar << ma_HaunchVolume[i];

		count = ma_HaunchNote.size();
		ar << count;
		for (int i = 0; i < ma_HaunchNote.size(); ++i)
			ar << ma_HaunchNote[i];

		count = ma_HaunchFormType.size();
		ar << count;
		for (int i = 0; i < ma_HaunchFormType.size(); ++i)
			ar << ma_HaunchFormType[i];

		count = ma_HaunchFormArea.size();
		ar << count;
		for (int i = 0; i < ma_HaunchFormArea.size(); ++i)
			ar << ma_HaunchFormArea[i];

		count = ma_HaunchFormNote.size();
		ar << count;
		for (int i = 0; i < ma_HaunchFormNote.size(); ++i)
			ar << ma_HaunchFormNote[i];

		count = ma_HaunchID.size();
		ar << count;
		for (int i = 0; i < ma_HaunchID.size(); ++i)
			ar << ma_HaunchID[i];

		count = ma_BuiltUpBemaMatlID.size();
		ar << count;
		for (int i = 0; i < ma_BuiltUpBemaMatlID.size(); ++i)
			ar << ma_BuiltUpBemaMatlID[i];

		count = ma_BuiltupBeamVolume.size();
		ar << count;
		for (int i = 0; i < ma_BuiltupBeamVolume.size(); ++i)
			ar << ma_BuiltupBeamVolume[i];

		count = ma_BuiltUpBeamNote.size();
		ar << count;
		for (int i = 0; i < ma_BuiltUpBeamNote.size(); ++i)
			ar << ma_BuiltUpBeamNote[i];

		count = ma_BuiltUpBeamID.size();
		ar << count;
		for (int i = 0; i < ma_BuiltUpBeamID.size(); ++i)
			ar << ma_BuiltUpBeamID[i];

		count = ma_Hanchczone.size();
		ar << count;
		for (int i = 0; i < ma_Hanchczone.size(); ++i)
			ar << ma_Hanchczone[i];

		count = ma_BuiltUpBeamLeftACV.size();
		ar << count;
		for (auto it = ma_BuiltUpBeamLeftACV.begin(); it != ma_BuiltUpBeamLeftACV.end(); it++)
		{
			count = it->second.size();
			ar << count;
			for (int i = 0; i < it->second.size(); i++)
				ar << it->second[i];
		}

		count = ma_BuiltUpBeamRightACV.size();
		ar << count;
		for (auto it = ma_BuiltUpBeamRightACV.begin(); it != ma_BuiltUpBeamRightACV.end(); it++)
		{
			count = it->second.size();
			ar << count;
			for (int i = 0; i < it->second.size(); i++)
				ar << it->second[i];
		}
	}
	else
	{
		ma_Param.clear();
		int count = 0;
		ar >> count;
		for (int i = 0; i < count; ++i)
		{
			double val = 0;
			ar >> val;
			ma_Param.push_back(val);
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150213)
		{
			ma_CZoneID.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				int val = 0; ar >> val; ma_CZoneID.push_back(val);
			}
			ma_Factor.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				double val = 0; ar >> val; ma_Factor.push_back(val);
			}
			ma_LibLathArea.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				double val = 0; ar >> val; ma_LibLathArea.push_back(val);
			}
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20150630)
		{
			ma_HaunchMatlID.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				int val = 0; ar >> val; ma_HaunchMatlID.push_back(val);
			}
			ma_HaunchVolume.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				double val = 0; ar >> val; ma_HaunchVolume.push_back(val);
			}
			ma_HaunchNote.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				CString val = _T(""); ar >> val; ma_HaunchNote.push_back(val);
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20161019)
		{
			ma_HaunchFormType.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				CString val = _T(""); ar >> val; ma_HaunchFormType.push_back(val);
			}
			ma_HaunchFormArea.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				double val = 0; ar >> val; ma_HaunchFormArea.push_back(val);
			}
			ma_HaunchFormNote.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				CString val = _T(""); ar >> val; ma_HaunchFormNote.push_back(val);
			}
			ma_HaunchID.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				int val = 0; ar >> val; ma_HaunchID.push_back(val);
			}
			ma_BuiltUpBemaMatlID.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				int val = 0; ar >> val; ma_BuiltUpBemaMatlID.push_back(val);
			}
			ma_BuiltupBeamVolume.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				double val = 0; ar >> val; ma_BuiltupBeamVolume.push_back(val);
			}
			ma_BuiltUpBeamNote.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				CString val = _T(""); ar >> val; ma_BuiltUpBeamNote.push_back(val);
			}
			ma_BuiltUpBeamID.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				int val = 0; ar >> val; ma_BuiltUpBeamID.push_back(val);
			}
		}

		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20210714)
		{
			ma_Hanchczone.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				int val = 0; ar >> val; ma_Hanchczone.push_back(val);
			}
		}
		else
		{
			count = ma_HaunchFormArea.size();
			for (int i = 0; i < count; ++i)
				ma_Hanchczone.push_back(0);
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20231121)
		{
			ma_BuiltUpBeamLeftACV.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				int size = 0;
				ar >> size;

				vector<double> area;
				for (int j = 0; j < size; j++)
				{
					double val = 0;
					ar >> val;
					area.push_back(val);
				}
				ma_BuiltUpBeamLeftACV.insert(make_pair(i, area));
			}

			ma_BuiltUpBeamRightACV.clear();
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				int size = 0;
				ar >> size;

				vector<double> area;
				for (int j = 0; j < size; j++)
				{
					double val = 0;
					ar >> val;
					area.push_back(val);
				}
				ma_BuiltUpBeamRightACV.insert(make_pair(i, area));
			}
		}
	}
}

void VolumeAreaInfoEMS::Dump(EFS::EFS_Buffer& buffer)
{
	int count = ma_Param.size();
	buffer << count;
	for (int i = 0; i < ma_Param.size(); ++i)
		buffer << ma_Param[i];

	count = ma_CZoneID.size();
	buffer << count;
	for (int i = 0; i < ma_CZoneID.size(); ++i)
		buffer << ma_CZoneID[i];

	count = ma_Factor.size();
	buffer << count;
	for (int i = 0; i < ma_Factor.size(); ++i)
		buffer << ma_Factor[i];

	count = ma_LibLathArea.size();
	buffer << count;
	for (int i = 0; i < ma_LibLathArea.size(); ++i)
		buffer << ma_LibLathArea[i];

	count = ma_HaunchMatlID.size();
	buffer << count;
	for (int i = 0; i < ma_HaunchMatlID.size(); ++i)
		buffer << ma_HaunchMatlID[i];

	count = ma_HaunchVolume.size();
	buffer << count;
	for (int i = 0; i < ma_HaunchVolume.size(); ++i)
		buffer << ma_HaunchVolume[i];

	count = ma_HaunchNote.size();
	buffer << count;
	for (int i = 0; i < ma_HaunchNote.size(); ++i)
		buffer << ma_HaunchNote[i];

	count = ma_HaunchFormType.size();
	buffer << count;
	for (int i = 0; i < ma_HaunchFormType.size(); ++i)
		buffer << ma_HaunchFormType[i];

	count = ma_HaunchFormArea.size();
	buffer << count;
	for (int i = 0; i < ma_HaunchFormArea.size(); ++i)
		buffer << ma_HaunchFormArea[i];

	count = ma_HaunchFormNote.size();
	buffer << count;
	for (int i = 0; i < ma_HaunchFormNote.size(); ++i)
		buffer << ma_HaunchFormNote[i];

	count = ma_Hanchczone.size();
	buffer << count;
	for (int i = 0; i < ma_Hanchczone.size(); ++i)
		buffer << ma_Hanchczone[i];

	count = ma_HaunchID.size();
	buffer << count;
	for (int i = 0; i < ma_HaunchID.size(); ++i)
		buffer << ma_HaunchID[i];

	count = ma_BuiltUpBemaMatlID.size();
	buffer << count;
	for (int i = 0; i < ma_BuiltUpBemaMatlID.size(); ++i)
		buffer << ma_BuiltUpBemaMatlID[i];

	count = ma_BuiltupBeamVolume.size();
	buffer << count;
	for (int i = 0; i < ma_BuiltupBeamVolume.size(); ++i)
		buffer << ma_BuiltupBeamVolume[i];

	count = ma_BuiltUpBeamNote.size();
	buffer << count;
	for (int i = 0; i < ma_BuiltUpBeamNote.size(); ++i)
		buffer << ma_BuiltUpBeamNote[i];

	count = ma_BuiltUpBeamID.size();
	buffer << count;
	for (int i = 0; i < ma_BuiltUpBeamID.size(); ++i)
		buffer << ma_BuiltUpBeamID[i];
}

void VolumeAreaInfoEMS::Recovery(EFS::EFS_Buffer& buffer)
{
	ma_Param.clear();
	int count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		double val = 0;
		buffer >> val;
		ma_Param.push_back(val);
	}

	ma_CZoneID.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		int val = 0; buffer >> val; ma_CZoneID.push_back(val);
	}

	ma_Factor.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		double val = 0; buffer >> val; ma_Factor.push_back(val);
	}

	ma_LibLathArea.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		double val = 0; buffer >> val; ma_LibLathArea.push_back(val);
	}

	ma_HaunchMatlID.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		int val = 0; buffer >> val; ma_HaunchMatlID.push_back(val);
	}

	ma_HaunchVolume.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		double val = 0; buffer >> val; ma_HaunchVolume.push_back(val);
	}

	ma_HaunchNote.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		CString val = _T(""); buffer >> val; ma_HaunchNote.push_back(val);
	}

	ma_HaunchFormType.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		CString val = _T(""); buffer >> val; ma_HaunchFormType.push_back(val);
	}

	ma_HaunchFormArea.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		double val = 0; buffer >> val; ma_HaunchFormArea.push_back(val);
	}

	ma_HaunchFormNote.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		CString val = _T(""); buffer >> val; ma_HaunchFormNote.push_back(val);
	}

	ma_HaunchID.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		int val = 0; buffer >> val; ma_HaunchID.push_back(val);
	}

	ma_Hanchczone.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		int val = 0; buffer >> val; ma_Hanchczone.push_back(val);
	}

	ma_BuiltUpBemaMatlID.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		int val = 0; buffer >> val; ma_BuiltUpBemaMatlID.push_back(val);
	}

	ma_BuiltupBeamVolume.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		double val = 0; buffer >> val; ma_BuiltupBeamVolume.push_back(val);
	}

	ma_BuiltUpBeamNote.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		CString val = _T(""); buffer >> val; ma_BuiltUpBeamNote.push_back(val);
	}

	ma_BuiltUpBeamID.clear();
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; ++i)
	{
		int val = 0; buffer >> val; ma_BuiltUpBeamID.push_back(val);
	}
}

#pragma region  Solid



void VolumeAreaInfoEMS::GetResult(double &rVolume, double& rArea, CString& noteVolume, CString& noteArea)
{
	// Obsolete
	rVolume = 0;
	rArea = 0;
	noteVolume = _T("");
	noteArea = _T("");
	if (mp_Owner == NULL) return;

	if (mp_Owner->GetType() == MSElement::msColumnG)
	{
		if (ma_Param.size() != 8) return;
		double W = ma_Param[0], D = ma_Param[1], H = ma_Param[2], TO = ma_Param[3], BO = ma_Param[4], TS = ma_Param[5];
		double LW = ma_Param[6], RW = ma_Param[7];
		double H2 = H + TO - BO - TS;

		rVolume = W * D*H2;
		rArea = (2 * W + 2 * D)*H2 - (LW + RW)*H2;
		noteVolume.Format(_T("%g x %g x %g"), W, D, H2);
		noteArea.Format(_T("(2(%g)+2(%g)) x %g - (%g + %g) x %g"), W, D, H2, LW, RW, H2);
	}
	else if (mp_Owner->GetType() == MSElement::msBeamG)
	{
		if (ma_Param.size() != 5) return;
		double W = ma_Param[0], D = ma_Param[1], CL = ma_Param[2], L = ma_Param[3], TS = ma_Param[4];

		rVolume = (D - TS)*W*L;
		rArea = (D - TS)*L * 2;
		noteVolume.Format(_T("(%g - %g) x %g x %g"), D, TS, W, L);
		noteArea.Format(_T("(%g - %g) x %g x 2"), D, TS, L);
	}
	else if (mp_Owner->GetType() == MSElement::msSlabG && !mp_Owner->IsMatSlab())
	{
		if (ma_Param.size() != 5) return;
		double A = ma_Param[0], OL = ma_Param[1], T = ma_Param[2], IA = ma_Param[3], IL = ma_Param[4];

		rVolume = A * T;
		rArea = (A - IA) + (OL*T + IL * T);
		noteVolume.Format(_T(""));
		noteArea.Format(_T(""));
	}
	else if (mp_Owner->GetType() == MSElement::msWallG)
	{
		if (ma_Param.size() != 9) return;
		double T = ma_Param[0], H = ma_Param[1], TO = ma_Param[2], BO = ma_Param[3], TSTB = ma_Param[4];
		double CL = ma_Param[5], L = ma_Param[6], IW = ma_Param[7], IH = ma_Param[8];
		double H2 = H + TO - BO - TSTB;

		rVolume = T * H2*L - T * IW*IH;
		rArea = H2 * L - IW * IH + (2 * IW + 2 * IH)*T;
		rArea += H2 * L - IW * IH;
		noteVolume.Format(_T("%g x %g x %g - %g x %g x %g"), T, H2, L, T, IW, IH);
		noteArea.Format(_T("%g x %g - %g x %g"), H2, L, IW, IH);
	}
	else if (mp_Owner->GetType() == MSElement::msIsoFootG)
	{
		if (ma_Param.size() != 5) return;
		double W = ma_Param[0], D = ma_Param[1], T = ma_Param[2], TB = ma_Param[3], ACV = ma_Param[4];

		rVolume = W * D*T;
		rArea = (2 * W + 2 * D)*T - ACV;
		noteVolume.Format(_T("%g x %g x %g"), W, D, T);
		noteArea.Format(_T("(2(%g)+2(%g)) x %g - (%g)"), W, D, T, ACV);
	}
	else if (mp_Owner->GetType() == MSElement::msSlabG && mp_Owner->IsMatSlab())
	{
		if (ma_Param.size() != 7) return;
		double A = ma_Param[0], OL = ma_Param[1], T = ma_Param[2], IA = ma_Param[3], IL = ma_Param[4];
		double AB = ma_Param[5], TB = ma_Param[6];

		rVolume = A * T;
		rArea = (OL*T) + (IL*T);
		noteVolume.Format(_T("%g x %g"), A, T);
		noteArea.Format(_T("(%g x %g) + (%g x %g"), OL, T, IL, T);
	}
	else if (mp_Owner->GetType() == MSElement::msWallFootG)
	{
		if (ma_Param.size() != 5) return;
		double W = ma_Param[0], H = ma_Param[1], L = ma_Param[2], TB = ma_Param[3], ACV = ma_Param[4];

		rVolume = W * H*L;
		rArea = 2 * H*L - ACV;
		noteVolume.Format(_T("%g x %g x %g"), W, H, L);
		noteArea.Format(_T("2 x %g x %g - (%g)"), H, L, ACV);
	}

	return;
}

#pragma endregion
void VolumeAreaInfoEMS::GetQuantityResult(double &rOriginVolume, double &rVolume, CString& noteVolume, FormResult& result, int cz_id, int calcType, double &srcColInnerVolume, CString& srcColInnerNoteVolume,
	vector<double>& addedConcVolumes, vector<CString>& addedConcNotes, FormResult& addedFormResult, vector<long>& addedMatIds, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid, vector<double>& arFWOriginArea, double rebarVolume)
{
	if (mp_Owner == NULL)
		return;

	// New EMS인 경우는 아래와 같이 VolumeAreaInfoEMS_NewEMS에서 가지고 온다.
	if (mp_Owner->GetBuilding()->GetQuantityOption()->m_nByOptionOrManual == 2)
	{
		VolumeAreaInfoEMS_NewEMS::QuantityData data;
		mp_Owner->m_VolumeAreaEMS_NewEMS.GetQuantityData(cz_id, calcType, data);

		rOriginVolume = data.Conc.Volume;
		rVolume = data.Conc.Volume;
		noteVolume = data.Conc.Note;
		for (auto form : data.Form)
		{
			result.formDir.push_back(form.Dir);
			result.formWorkName.push_back(form.Name);
			result.formArea.push_back(form.Area);
			result.formNote.push_back(form.Note);
		}
		return;
	}

	rVolume = 0.0;
	noteVolume = _T("");
	result.formDir.clear();
	result.formWorkName.clear();
	result.formArea.clear();
	result.formNote.clear();

	if (mp_Owner->GetType() == MSElement::msColumnG || mp_Owner->GetType() == MSElement::msWallColumnG)
		GetResultByCZoneColumn(cz_id, rVolume, noteVolume, result, calcType, srcColInnerVolume, srcColInnerNoteVolume);
	else if (mp_Owner->GetType() == MSElement::msBeamG)
		GetResultByCZoneBeam(cz_id, rVolume, noteVolume, result, calcType, subIds, subVolumes, subNotes);
	else if (mp_Owner->GetType() == MSElement::msHandRailG)
		GetResultByCZoneLowWall(cz_id, rVolume, noteVolume, result, calcType);
	else if (mp_Owner->GetType() == MSElement::msSlabG)
	{
		MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_Owner);
		if (slab != nullptr && slab->IsSlopeMember())
			GetResultByCZoneRamp(cz_id, rVolume, noteVolume, result);
		else
		{
			if (mp_Owner->IsMatSlab())
				GetResultByCZoneMat(cz_id, rVolume, noteVolume, result, subIds, subVolumes, subNotes, matid);
			else if (mp_Owner->IsGeneralDeckSlab())
				GetResultByCZoneGeneralDeck(cz_id, rVolume, noteVolume, result);
			else if (mp_Owner->IsIMTPCSlab())
				GetResultByCZoneIMTPCSlab(cz_id, rVolume, noteVolume, result, calcType);
			else if (mp_Owner->IsHCSPCSlab())
				GetResultByCZoneHCSPCSlab(cz_id, rVolume, noteVolume, result, calcType);
			else if (mp_Owner->IsRPSPCSlab())
				GetResultByCZoneRPSPCSlab(cz_id, rVolume, noteVolume, result, calcType);
			else
				GetResultByCZoneSlab(cz_id, rVolume, noteVolume, result, calcType, subIds, subVolumes, subNotes, matid);
		}
	}
	else if (mp_Owner->GetType() == MSElement::msWallG)
		GetResultByCZoneWall(cz_id, rVolume, noteVolume, result);
	else if (mp_Owner->GetType() == MSElement::msIsoFootG)
		GetResultByCZoneIsoFoot(cz_id, rVolume, noteVolume, result, subIds, subVolumes, subNotes, matid);
	else if (mp_Owner->GetType() == MSElement::msWallFootG)
		GetResultByCZoneWallFoot(cz_id, rVolume, noteVolume, result, subIds, subVolumes, subNotes, matid);
	else if (mp_Owner->GetType() == MSElement::msPitG)
		GetResultByCZonePit(cz_id, rVolume, noteVolume, result, subIds, subVolumes, subNotes, matid);
	else if (mp_Owner->GetType() == MSElement::msStairG)
		GetResultByCZoneStair(cz_id, rVolume, noteVolume, result);
	else if (mp_Owner->GetType() == MSElement::msHaunchG)
		GetResultByCZoneMainHaunch(cz_id, rVolume, noteVolume, result);

	// 거푸집 넓이가 매우 작은 거푸집은 삭제. 음수 거푸집은 따로 삭제하지 않는다.
	for (int i = result.formArea.size() - 1; i >= 0; i--)
	{
		if (abs(result.formArea[i]) < 0.001)
		{
			result.formDir.erase(result.formDir.begin() + i);
			result.formWorkName.erase(result.formWorkName.begin() + i);
			result.formArea.erase(result.formArea.begin() + i);
			result.formNote.erase(result.formNote.begin() + i);
		}
	}

	// rOriginVolume : 적산옵션에 의한 콘크리트 물량추가/제거가 반영되지 않은 순수한 부재의 콘크리트 물량값
	rOriginVolume = rVolume;
	// arFWOriginArea : 적산옵션에 의한 물량추가/제거가 반영되지 않은 순수한 부재의 거푸집 물량값
	arFWOriginArea.clear();
	for (int i = 0; i < result.formArea.size(); ++i)
		arFWOriginArea.push_back(result.formArea[i]);

	IncreaseFactor factor;
	mp_Owner->GetBuilding()->GetQuantityOption()->GetFactorForMemberType(mp_Owner, factor);
	ApplyIncreaseFactor(&factor, rVolume, result.formArea);
	ApplyIncreaseFactor(&factor, subVolumes);

	// 콘크리트 체적에서 철근체적 공제
	double rebarExVolume = 0;
	CString rebarExNote;
	//G[Q22-410] 산출서랑 체적 겹치지 않게 해당 함수 포함안시킴. 
	//GetResultRebarExcludeVolume(rebarVolume, rebarExVolume, rebarExNote);

	// 거푸집 별 추가 콘크리트, 거푸집 합산
	GetResultAdditionalConcAndForm(cz_id, result, arFWOriginArea, addedConcVolumes, addedConcNotes, addedFormResult, addedMatIds);
	ApplyIncreaseFactor(&factor, addedConcVolumes, addedFormResult.formArea);

	// 시설물 콘크리트 공제
	double dedConcVolume = 0;
	CString dedConcNote;
	GetResultConcDeduction(rVolume, dedConcVolume, dedConcNote);

	// 추가물량을 결과에 더함
	rVolume += rebarExVolume;

	for (int i = 0; i < addedFormResult.formArea.size(); i++)
	{
		result.formWorkName.push_back(addedFormResult.formWorkName[i]);
		result.formDir.push_back(addedFormResult.formDir[i]);
		result.formArea.push_back(addedFormResult.formArea[i]);
		result.formNote.push_back(addedFormResult.formNote[i]);
	}

	rVolume += dedConcVolume;

	//DQ1-652 음수 물량 0처리 start
	if (rVolume < 0)
	{
		rVolume = 0;
		noteVolume = _T("");
	}

	double areaSum = 0;
	for (int i = 0; i < result.formArea.size(); ++i)
		areaSum += result.formArea[i];

	if (areaSum < 0)
	{
		result.formDir.clear();
		result.formWorkName.clear();
		result.formArea.clear();
		result.formNote.clear();
	}
	//DQ1-652 음수 물량 0처리 end

	return;
}

void VolumeAreaInfoEMS::GetResultByCZone(int cz_id, double &rOriginVolume, double &rVolume, CString& noteVolume, FormResult& result, int calcType, double &srcColInnerVolume, CString& srcColInnerNoteVolume,
	vector<double>& addedConcVolumes, vector<CString>& addedConcNotes, FormResult& addedFormResult, vector<long>& addedMatIds, vector<long>& subIds,
	vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& maitd, vector<double>& arFWOriginArea, double rebarVolume)
{
	if (mp_Owner == NULL)
		return;

	// New EMS인 경우는 아래와 같이 VolumeAreaInfoEMS_NewEMS에서 가지고 온다.
	if (mp_Owner->GetBuilding()->GetQuantityOption()->m_nByOptionOrManual == 2)
	{
		VolumeAreaInfoEMS_NewEMS::QuantityData data;
		mp_Owner->m_VolumeAreaEMS_NewEMS.GetQuantityData(cz_id, calcType, data);

		rOriginVolume = data.Conc.Volume;
		rVolume = data.Conc.Volume;
		noteVolume = data.Conc.Note;
		for (auto form : data.Form)
		{
			result.formDir.push_back(form.Dir);
			result.formWorkName.push_back(form.Name);
			result.formArea.push_back(form.Area);
			result.formNote.push_back(form.Note);
		}
		return;
	}

	MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
	if (curFloor == NULL) return;

	if (!mp_Owner->IsLowWall())
	{
		if (NULL == curFloor->GetFloorCZone(cz_id))
			return;
	}
	else
	{
		MSHandRailG* lowWall = dynamic_cast<MSHandRailG*>(mp_Owner);
		if (!lowWall->GetTopQuantitySeparation())
		{
			if (NULL == curFloor->GetFloorCZone(cz_id))
				return;
		}
		else
		{
			if (calcType == 1)//top , upper
			{
				curFloor = curFloor->GetUpperFloor();
				if (NULL == curFloor)
					return;
				if (NULL == curFloor->GetFloorCZone(cz_id))
					return;
			}
			else //(calcType == 2 //under, 0 : all
			{
				if (NULL == curFloor->GetFloorCZone(cz_id))
					return;
			}
		}
	}

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
	}
	if (Factor == 0.0) return;

	double rArea = 0;
	CString noteArea;

	GetQuantityResult(rOriginVolume, rVolume, noteVolume, result, cz_id, calcType, srcColInnerVolume, srcColInnerNoteVolume, addedConcVolumes, addedConcNotes, addedFormResult, addedMatIds, subIds, subVolumes, subNotes, maitd, arFWOriginArea, rebarVolume);

	if (mp_Owner->GetType() == MSElement::msSlabG ||
		mp_Owner->GetType() == MSElement::msBeamG ||
		mp_Owner->GetType() == MSElement::msIsoFootG ||
		mp_Owner->GetType() == MSElement::msWallFootG ||
		mp_Owner->GetType() == MSElement::msPitG)
	{
		; // Slab, Beam, IsoFoot, WallFoot, Pit 는 GetQuantityResult 안에서 Factor 곱해서 나온다
	}
	else
	{
		// Apply CZone Factor
		if (Factor != 1.0)
		{
			MSConstructionZone* czone = curFloor->GetFloorCZone(cz_id);
			CString tmp;
			rOriginVolume *= Factor;
			rVolume *= Factor;
			tmp.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, noteVolume);
			noteVolume = tmp;

			for (int i = 0; i < result.formArea.size(); ++i)
			{
				result.formArea[i] *= Factor;
				tmp.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, result.formNote[i]);
				result.formNote[i] = tmp;
			}
		}
	}

	// Add LibLath Form
	if (dLibLathArea >= 0.01)
	{
		MSBaseBuilding* building = curFloor->GetBuilding();
		CString RibLathTypeName = building->GetFormManager()->GetRibLathFormWorkTypeName();

		MSConstructionZone* czone = curFloor->GetFloorCZone(cz_id);
		CString tmp;
		tmp.Format(_T("[%s] Area(%g)"), czone->GetName(), dLibLathArea);
		result.formDir.push_back(_T("Rib"));
		result.formWorkName.push_back(RibLathTypeName);
		result.formArea.push_back(dLibLathArea);
		result.formNote.push_back(tmp);
	}

	/*IncreaseFactor factor;
	mp_Owner->GetBuilding()->m_QuantityOption.GetFactorForMemberType(mp_Owner, factor);
	ApplyIncreaseFactor(&factor, rVolume, result.formArea);
	ApplyIncreaseFactor(&factor, subVolumes);
	ApplyIncreaseFactor(&factor, addedConcVolumes);*/

	return;
}

double VolumeAreaInfoEMS::GetLibLathAreaByCZoneID(int czone_id)
{
	for (int i = 0; i < ma_CZoneID.size(); ++i)
	{
		if (czone_id == ma_CZoneID[i])
		{
			return ma_LibLathArea[i];
		}
	}
	return -1;
}

void VolumeAreaInfoEMS::ApplyIncreaseFactor(IncreaseFactor* factor, double& rVolume, vector<double>& arFWArea)
{
	double f_vol = factor->concrete;
	double f_area = factor->formwork;

	if (fabs(f_vol - 1.0) > 1.e-5)
	{
		rVolume *= f_vol;
	}

	if (fabs(f_area - 1.0) > 1.e-5)
	{
		for (int i = 0; i < arFWArea.size(); ++i)
			arFWArea[i] = arFWArea[i] * f_area;
	}
}

void VolumeAreaInfoEMS::ApplyIncreaseFactor(IncreaseFactor* factor, vector<double>& arVolumes)
{
	double f_vol = factor->concrete;

	if (fabs(f_vol - 1.0) > 1.e-5)
	{
		for (int i = 0; i < arVolumes.size(); ++i)
			arVolumes[i] = arVolumes[i] * f_vol;
	}
}

void VolumeAreaInfoEMS::ApplyIncreaseFactor(IncreaseFactor* factor, vector<double>& arVolumes, vector<double>& arFWArea)
{
	double f_vol = factor->concrete;
	double f_area = factor->formwork;

	if (fabs(f_vol - 1.0) > 1.e-5)
	{
		for (int i = 0; i < arVolumes.size(); ++i)
			arVolumes[i] = arVolumes[i] * f_vol;
	}

	if (fabs(f_area - 1.0) > 1.e-5)
	{
		for (int i = 0; i < arFWArea.size(); ++i)
			arFWArea[i] = arFWArea[i] * f_area;
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneBurim(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
{
	if (mp_Owner == NULL) return;
	rVolume = 0.0;
	noteVolume = "";

	for (int i = 0; i < mp_Owner->ma_VolumeInfo.size(); ++i)
	{
		if (mp_Owner->ma_VolumeInfo[i].is_burim)
		{
			if ((mp_Owner->GetType() == MSElement::msIsoFootG || mp_Owner->GetType() == MSElement::msPitG) == false && 
				mp_Owner->ma_VolumeInfo[i].czone_id != cz_id) 
				continue;

			rVolume += mp_Owner->ma_VolumeInfo[i].value;
			if (mp_Owner->ma_VolumeInfo[i].note.GetLength() > 0)
			{
				if (noteVolume.GetLength() > 0)
					noteVolume += " + ";
				if (_bShowVariable)
					noteVolume += mp_Owner->ma_VolumeInfo[i].note;
				else
					noteVolume += ChangeVolumeInfoNoVariable(mp_Owner->ma_VolumeInfo[i].note);
			}
		}
	}

	if (MSCompoG::NOTE_VERIFY)
	{
		CString save(noteVolume);
		noteVolume = "= ";
		noteVolume += save;
	}

	double rOriginVolume = 0;
	IncreaseFactor factor;
	mp_Owner->GetBuilding()->GetQuantityOption()->GetFactorForMemberType(mp_Owner, factor);
	ApplyIncreaseFactor(&factor, rVolume, result.formArea);

	return;
}


//czone_id == -1 이면 czone_id를 무시하고 모두 모은다.
void VolumeAreaInfoEMS::GetBurimFormWorkAreaByCZone(int czone_id, vector<CString>& arFWAreaDir_burim, vector<CString>& arFWAreaType_burim, vector<double>& arFWArea_burim, vector<CString>& arFormWorkNote_burim)
{

	arFWAreaDir_burim.clear();
	arFWAreaType_burim.clear();
	arFWArea_burim.clear();
	arFormWorkNote_burim.clear();
	if (mp_Owner == NULL) return;

	for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
	{
		if (mp_Owner->ma_UserFormWork[i].is_burim)
		{
			if ((mp_Owner->GetType() == MSElement::msIsoFootG || mp_Owner->GetType() == MSElement::msPitG) == false &&
				mp_Owner->ma_UserFormWork[i].czone_id != czone_id)
				continue;

			if (mp_Owner->ma_UserFormWork[i].value < 0.001) continue;

			arFWAreaDir_burim.push_back(_T("Burim"));
			arFWAreaType_burim.push_back(mp_Owner->ma_UserFormWork[i].formWorkName);
			arFWArea_burim.push_back(mp_Owner->ma_UserFormWork[i].value);
			if (_bShowVariable)
				arFormWorkNote_burim.push_back(mp_Owner->ma_UserFormWork[i].note);
			else
				arFormWorkNote_burim.push_back(ChangeVolumeInfoNoVariable(mp_Owner->ma_UserFormWork[i].note));
		}
	}

	double rOriginVolume = 0;
	double rVolume = 0;
	IncreaseFactor factor;
	mp_Owner->GetBuilding()->GetQuantityOption()->GetFactorForMemberType(mp_Owner, factor);
	ApplyIncreaseFactor(&factor, rVolume, arFWArea_burim);

	return;
}

void VolumeAreaInfoEMS::GetResultByCZoneSubHaunch(int cz_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList)
{
	if (mp_Owner == NULL) return;

	matl_id_list.clear();
	volume_list.clear();
	noteList.clear();

	if (NULL == ((MSFloor*)mp_Owner->mp_Owner)->GetCZone(cz_id))
		return;

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
	}
	if (Factor == 0.0) return;

	////////////////////////////////////////////////////////////////////
	matl_id_list = ma_HaunchMatlID;
	volume_list = ma_HaunchVolume;
	noteList = ma_HaunchNote;
	////////////////////////////////////////////////////////////////////

	/*
	// Apply CZone Factor
	if (Factor != 1.0)
	{
		MSConstructionZone* czone = ((MSFloor*)mp_Owner->mp_Owner)->GetCZone(cz_id);
		CString tmp;

		for (int i = 0; i < volume_list.size(); ++i)
		{
			volume_list[i] *= Factor;
			tmp.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, noteList[i]);
			noteList[i] = tmp;
		}
	}
	*/

	IncreaseFactor factor;
	mp_Owner->GetBuilding()->GetQuantityOption()->GetFactorForMemberType(mp_Owner, factor);
	ApplyIncreaseFactor(&factor, volume_list);

	return;
}

void VolumeAreaInfoEMS::GetResultByCZoneSubHaunchAll(int cz_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList,
	vector<CString>& haunch_arFWAreaTypeList, vector<double>& haunch_arFWAreaList, vector<CString>& haunch_arFormWorkNoteList, vector<int>& haunchID, vector<int>& fwHaunchID, vector<double>& haunch_arFWOriginArea, vector<int>& hanch_matbool)
{
	if (mp_Owner == NULL) return;

	matl_id_list.clear();
	volume_list.clear();
	noteList.clear();
	haunch_arFWAreaTypeList.clear();
	haunch_arFWAreaList.clear();
	haunch_arFormWorkNoteList.clear();
	haunchID.clear();
	fwHaunchID.clear();
	haunch_arFWOriginArea.clear();

	if (NULL == ((MSFloor*)mp_Owner->mp_Owner)->GetCZone(cz_id))
		return;

	vector<MSElement*> subMembers = mp_Owner->GetSubMembers();
	for (int i = 0; i < subMembers.size(); ++i)
	{
		MSLineLevelDifG* diff = (MSLineLevelDifG*)subMembers[i];
		if (diff == nullptr)
			continue;
		for (int i = 0; i < ma_Hanchczone.size(); i++)
		{
			if (diff->GetID() == ma_HaunchID[i])
			{
				if (cz_id == ma_Hanchczone[i])
					haunch_arFWAreaTypeList.push_back(ma_HaunchFormType[i]);
				else
					haunch_arFWAreaTypeList.push_back(_T("None"));
				haunch_arFWAreaList.push_back(ma_HaunchFormArea[i]);
				haunch_arFWOriginArea.push_back(ma_HaunchFormArea[i]);
				haunch_arFormWorkNoteList.push_back(ma_HaunchFormNote[i]);
				fwHaunchID.push_back(ma_HaunchID[i]);
			}
		}
	}
	for (int i = 0; i < ma_Hanchczone.size(); i++)
	{
		hanch_matbool.push_back(ma_Hanchczone[i]);
	}

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
	}
	if (Factor == 0.0) return;

	////////////////////////////////////////////////////////////////////
	matl_id_list = ma_HaunchMatlID;
	volume_list = ma_HaunchVolume;
	if (_bShowVariable)
		noteList = ma_HaunchNote;
	else
	{
		for (int i = 0; i < ma_HaunchNote.size(); i++)
		{
			noteList.push_back(ChangeVolumeInfoNoVariable(ma_HaunchNote[i]));
		}
	}

	/*
	haunch_arFWAreaTypeList = ma_HaunchFormType;
	haunch_arFWAreaList = ma_HaunchFormArea;
	if (_bShowVariable)
		haunch_arFormWorkNoteList = ma_HaunchFormNote;
	else
	{
		for (int i = 0; i < ma_HaunchFormNote.size(); i++)
		{
			haunch_arFormWorkNoteList.push_back(ChangeVolumeInfoNoVariable(ma_HaunchFormNote[i]));
		}
	}
	*/

	haunchID = ma_HaunchID;
	////////////////////////////////////////////////////////////////////

	// Apply CZone Factor
	//if (Factor != 1.0)
	//{
	//	MSConstructionZone* czone = ((MSFloor*)mp_Owner->mp_Owner)->GetCZone(cz_id);
	//	CString tmp;

	//	for (int i = 0; i < haunchID.size(); ++i)
	//	{
	//		volume_list[i] *= Factor;
	//		haunch_arFWAreaList[i] *= Factor;
	//		tmp.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, noteList[i]);
	//		noteList[i] = tmp;

	//		tmp.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, haunch_arFormWorkNoteList[i]);
	//		haunch_arFormWorkNoteList[i] = tmp;
	//	}
	//}

	if (ma_HaunchNote.size() > ma_Hanchczone.size())
	{
		for (int i = ma_HaunchNote.size() - ma_Hanchczone.size(); i < ma_HaunchNote.size(); i++)
		{
			hanch_matbool.push_back(0);
		}
	}
	IncreaseFactor factor;
	mp_Owner->GetBuilding()->GetQuantityOption()->GetFactorForMemberType(mp_Owner, factor);
	ApplyIncreaseFactor(&factor, volume_list, haunch_arFWAreaList);

	return;
}

void VolumeAreaInfoEMS::GetResultByCZoneBuiltUpBeam(int cz_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList)
{
	if (mp_Owner == NULL) return;

	matl_id_list.clear();
	volume_list.clear();
	noteList.clear();

	if (NULL == ((MSFloor*)mp_Owner->mp_Owner)->GetCZone(cz_id))
		return;

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
	}
	if (Factor == 0.0) return;

	////////////////////////////////////////////////////////////////////
	matl_id_list = ma_BuiltUpBemaMatlID;
	volume_list = ma_BuiltupBeamVolume;
	noteList = ma_BuiltUpBeamNote;
	////////////////////////////////////////////////////////////////////

	// Apply CZone Factor
	if (Factor != 1.0)
	{
		MSConstructionZone* czone = ((MSFloor*)mp_Owner->mp_Owner)->GetCZone(cz_id);
		CString tmp;

		for (int i = 0; i < volume_list.size(); ++i)
		{
			volume_list[i] *= Factor;
			tmp.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, noteList[i]);
			noteList[i] = tmp;
		}
	}

	return;
}

void VolumeAreaInfoEMS::GetResultByCZoneColumn(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, double &srcColInnerVolume, CString& srcColInnerNoteVolume)
{
	if (mp_Owner->IsCustomColumn() || mp_Owner->IsCustomWallColumn())
	{
		GetResultByCZoneCustomColumn(cz_id, rVolume, noteVolume, result);
	}
	else
		GetResultByCZoneNormalColumn(cz_id, rVolume, noteVolume, result, calcType, srcColInnerVolume, srcColInnerNoteVolume);
}

void VolumeAreaInfoEMS::GetResultByCZoneNormalColumn(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, double &srcColInnerVolume, CString& srcColInnerNoteVolume)
{
	if (mp_Owner->GetType() == MSElement::msColumnG)
	{
		if (ma_Param.size() != 27) return;

		MSColumnG* columnG = dynamic_cast<MSColumnG*>(mp_Owner);
		MSColumnHeaderM* headerM = dynamic_cast<MSColumnHeaderM*>(columnG->GetColumnHeaderM());
		int headerType = 0;
		if (headerM != NULL)
			headerType = headerM->GetHeadType();

		double W = ma_Param[0], D = ma_Param[1], H = ma_Param[2], TO = ma_Param[3], BO = ma_Param[4], TS = ma_Param[5], TB = ma_Param[6];
		double LW = ma_Param[7], RW = ma_Param[8], R = ma_Param[9], ACV0 = ma_Param[10], ACV1 = ma_Param[11], ACV2 = ma_Param[12], ACV3 = ma_Param[13];
		double BH1 = ma_Param[14], BH2 = ma_Param[15];	// PC기둥과 붙어있는 PC보의 높이
		double CHT = ma_Param[16], CHV = ma_Param[17], CHA0 = ma_Param[18], CHA1 = ma_Param[19], CHA2 = ma_Param[20], CHA3 = ma_Param[21];
		double CO = ma_Param[22];
		double SUBCHA0 = ma_Param[23], SUBCHA1 = ma_Param[24], SUBCHA2 = ma_Param[25], SUBCHA3 = ma_Param[26];
		double CHA = CHA0 + CHA1 + CHA2 + CHA3 + SUBCHA0 + SUBCHA1 + SUBCHA2 + SUBCHA3; //주두부 거푸집 각 면별
		double ACV = ACV0 + ACV1 + ACV2 + ACV3; //보 공제 거푸집 각 면별
		double rArea = 0;
		CString noteArea;

		double H2 = H; //H2 : 계산용 높이

		// Concrete 일반 기둥 주두부 포함
		if (calcType == 0 && !mp_Owner->IsPC())	// 전체 (기존 로직)
		{
			H2 = H + TO - BO - TS;

			if (headerType == 3)
			{
				double T1 = headerM->GetT1() * GM2ACIS;
				rVolume = W * D * (H2 - T1);

				if (_bShowVariable)
					noteVolume.Format(_T("%g(B) * %g(D) * %g(H')"), W, D, H2 - T1);
				else
					noteVolume.Format(_T("%g * %g * %g"), W, D, H2 - T1);
			}
			else
			{			
				rVolume = W * D * H2;

				if (_bShowVariable)
					noteVolume.Format(_T("%g(B) * %g(D) * %g(H')"), W, D, H2);
				else
					noteVolume.Format(_T("%g * %g * %g"), W, D, H2);
			}

			rVolume = Round(rVolume, 3);

			if (CHV > 0.0) //주두부가 있는 경우
			{
				CString tmp;
				rVolume += CHV;
				tmp.Format(_T(" + %g(CHV)"), CHV);
				noteVolume += tmp;
			}
			else if (CHV < 0.0)
			{
				CString tmp;
				rVolume += CHV;
				tmp.Format(_T("  %g(CHV)"), CHV);
				noteVolume += tmp;
			}
		}
		else if (calcType == 1 && mp_Owner->IsPC())	// RC부분(현장타설)
		{
			if (CO > 0)
				H2 = CO - TS;
			else if (H2 < 0)
				H2 = 0;
			else
				H2 = 0;

			rVolume = W * D * H2;
			rVolume = Round(rVolume, 3);
			if (_bShowVariable)
				noteVolume.Format(_T("%g(B) * %g(D) * %g(H')"), W, D, H2);
			else
				noteVolume.Format(_T("%g * %g * %g"), W, D, H2);
		}
		else if (calcType == 2 && mp_Owner->IsPC())	// PC부분(제작)
		{
			//H2 = H + TO - BO - BH1 - BH2 - CO;
			H2 = H + TO - BO - CO; //Q23-383 [DW - 80] PC기둥 제작물량 오류
			
			if (headerType == 3)
			{
				double T1 = headerM->GetT1() * GM2ACIS;
				rVolume = W * D * (H2 - T1);

				if (_bShowVariable)
					noteVolume.Format(_T("%g(B) * %g(D) * %g(H')"), W, D, H2 - T1);
				else
					noteVolume.Format(_T("%g * %g * %g"), W, D, H2 - T1);
			}
			else
			{
				rVolume = W * D * H2;
				if (_bShowVariable)
					noteVolume.Format(_T("%g(B) * %g(D) * %g(H')"), W, D, H2);
				else
					noteVolume.Format(_T("%g * %g * %g"), W, D, H2);
			}

			rVolume = Round(rVolume, 3);
			
			if (CHV > 0.0) //주두부가 있는 경우
			{
				CString tmp;
				rVolume += CHV;
				tmp.Format(_T(" + %g(CHV)"), CHV);
				noteVolume += tmp;
			}
			else if (CHV < 0.0)
			{
				CString tmp;
				rVolume += CHV;
				tmp.Format(_T("  %g(CHV)"), CHV);
				noteVolume += tmp;
			}

		}
		else
			return;

		if (R > 0.0)
		{
			rVolume = PI * R * R * H2;
			rVolume = Round(rVolume, 3);
			if (_bShowVariable)
				noteVolume.Format(_T("PI * %g(R)^2 * %g(H')"), R, H2);
			else
				noteVolume.Format(_T("PI * %g^2 * %g"), R, H2);
		}

		if (mp_Owner->IsSRC())
		{
			// [BH-5832] SRC 기둥 철골 내부 콘크리트 재질 추가 : SRC 재질과 철골 내부 재질이 다른 경우 공제
			MSSrcColumnM* srcColumnM = dynamic_cast<MSSrcColumnM*>(mp_Owner->GetCompoM());
			MSMaterial* material = mp_Owner->GetMaterial();
			MSMaterial* innerMaterial = mp_Owner->GetInnerConcMaterial();

			if (material != NULL && (innerMaterial != NULL && innerMaterial->GetName() != "") && (material->GetName() != innerMaterial->GetName()))
			{
				MSSteelSection* steelSection = srcColumnM->GetSteelSection();
				if (steelSection != NULL && steelSection->GetSectionType() == 1) // RECT
				{
					double steelH = steelSection->GetH() / 1000.0;
					double steelW = steelSection->GetB() / 1000.0;

					if (_bShowVariable)
						srcColInnerNoteVolume.Format(_T("%g(H) * %g(B) * %g(H')"), steelH, steelW, H2);
					else
						srcColInnerNoteVolume.Format(_T("%g * %g * %g"), steelH, steelW, H2);

					srcColInnerVolume = steelH * steelW * H2;
					rVolume -= srcColInnerVolume;
					rVolume = Round(rVolume, 3);
				}
				else if ((steelSection != NULL && steelSection->GetSectionType() == 2)) // ROUND
				{
					MSSteelRoundPipeKr* roundSection = dynamic_cast<MSSteelRoundPipeKr*>(steelSection);
					if (roundSection != NULL)
					{
						double steelR = Round(roundSection->GetD() / 1000.0 / 2, 3);

						if (_bShowVariable)
							srcColInnerNoteVolume.Format(_T("PI * %g(R)^2 * %g(H')"), steelR, H2);
						else
							srcColInnerNoteVolume.Format(_T("PI * %g^2 * %g"), steelR, H2);

						srcColInnerVolume = PI * steelR * steelR * H2;
						rVolume -= srcColInnerVolume;
						rVolume = Round(rVolume, 3);
					}
				}

				noteVolume.Format(_T("%s - (%s)"), noteVolume, srcColInnerNoteVolume);
			}

			bool isSteelExclude = mp_Owner->GetBuilding()->GetQuantityOption()->m_bIsSteelQuantityExclude;
			if (isSteelExclude)
			{
				double dSteelArea = mp_Owner->GetCompoM()->GetSteelSection()->GetArea() * GM2ACIS_A;
				rVolume -= dSteelArea * H2;
				rVolume = Round(rVolume, 3);
				CString steelStr;
				if (_bShowVariable)
					steelStr.Format(_T(" - (%g(STL) * %g(H'))"), dSteelArea, H2);
				else
					steelStr.Format(_T(" - (%g * %g)"), dSteelArea, H2);
				noteVolume += steelStr;
			}
		}

		// Form Area
		if (mp_Owner->IsPC() == false)
		{
			H2 = H + TO - BO;
			// BH-5119 PC기둥의 경우 거푸집 템플릿이 있어도 거푸집을 계산하지 않는다.
			bool all_same = true;
			for (int i = 0; i < (int)(mp_Owner->ma_UserFormWork.size()) - 1; ++i)
			{
				if (mp_Owner->ma_UserFormWork[i].formWorkName != mp_Owner->ma_UserFormWork[i + 1].formWorkName)
					all_same = false;
			}
			if (mp_Owner->ma_UserFormWork.size() > 0)
			{
				if (all_same)
				{
					rArea = (2 * W + 2 * D)*H2 - (LW + RW)*H2 - ACV;
					rArea = Round(rArea, 3);
					//noteArea.Format(_T("(2(%g)+2(%g)) x %g - (%g + %g) x %g"), W, D, H2, LW, RW, H2);
					
					if (headerType == 1)
					{
						double T3 = headerM->GetT3() * GM2ACIS;
						double T5 = headerM->GetT5() * GM2ACIS;
						rArea = 2 * W * (H2 - T5) + 2 * D * (H2 - T3) - (LW + RW) * H2 - ACV;
						rArea = Round(rArea, 3);

						if (LW == 0.0 && RW == 0.0)
						{
							if (_bShowVariable)
								noteArea.Format(_T("(2 * %g(B) * %g(H')) + (2 * %g(D) * %g(H'))"), W, H2 - T5, D, H2 - T3);
							else
								noteArea.Format(_T("(2 * %g * %g) + (2 * %g * %g)"), W, H2 - T5, D, H2 - T3);
						}
						else
						{
							if (_bShowVariable)
								noteArea.Format(_T("(2 * %g(B) * %g(H')) + (2 * %g(D) * %g(H')) - (%g(LW) + %g(RW)) * %g(H')"), W, H2 - T5, D, H2 - T3, LW, RW, H2);
							else
								noteArea.Format(_T("(2 * %g * %g) + (2 * %g * %g) - (%g + %g) * %g"), W, H2 - T5, D, H2 - T3, LW, RW, H2);
						}
					}
					else if (headerType == 3)
					{
						double T1 = headerM->GetT1() * GM2ACIS;
						double T5 = headerM->GetT5() * GM2ACIS;
						rArea = (2 * W + D) * (H2 - T1) + D * (H2 - T5) - (LW + RW) * H2 - ACV;
						rArea = Round(rArea, 3);

						if (LW == 0.0 && RW == 0.0)
						{
							if (_bShowVariable)
								noteArea.Format(_T("(2 * %g(B) + %g(D)) * %g(H') + %g(D) * %g(H')"), W, D, H2 - T1, D, H2 - T5);
							else
								noteArea.Format(_T("(2 * %g + %g) * %g + %g * %g"), W, D, H2 - T1, D, H2 - T5);
						}
						else
						{
							if (_bShowVariable)
								noteArea.Format(_T("(2 * %g(B) + %g(D)) * %g(H') + %g(D) * %g(H') - (%g(LW) + %g(RW)) * %g(H')"), W, D, H2 - T1, D, H2 - T5, LW, RW, H2);
							else
								noteArea.Format(_T("(2 * %g + %g) * %g + %g * %g - (%g + %g) * %g"), W, D, H2 - T1, D, H2 - T5, LW, RW, H2);
						}
					}
					else if (LW == 0.0 && RW == 0.0)
					{
						if (_bShowVariable)
							noteArea.Format(_T("(2 * %g(B) + 2 * %g(D)) * %g(H')"), W, D, H2);
						else
							noteArea.Format(_T("(2 * %g + 2 * %g) * %g"), W, D, H2);
					}
					else
					{
						if (_bShowVariable)
							noteArea.Format(_T("(2 * %g(B) + 2 * %g(D)) * %g(H') - (%g(LW) + %g(RW)) * %g(H')"), W, D, H2, LW, RW, H2);
						else
							noteArea.Format(_T("(2 * %g + 2 * %g) * %g - (%g + %g) * %g"), W, D, H2, LW, RW, H2);
					}

					if (ACV0 > 0.0)
					{
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" - %g(ACV)"), ACV0);
						else
							tmp.Format(_T(" - %g"), ACV0);
						noteArea += tmp;
					}
					if (ACV1 > 0.0)
					{
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" - %g(ACV)"), ACV1);
						else
							tmp.Format(_T(" - %g"), ACV1);
						noteArea += tmp;
					}
					if (ACV2 > 0.0)
					{
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" - %g(ACV)"), ACV2);
						else
							tmp.Format(_T(" - %g"), ACV2);
						noteArea += tmp;
					}
					if (ACV3 > 0.0)
					{
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" - %g(ACV)"), ACV3);
						else
							tmp.Format(_T(" - %g"), ACV3);
						noteArea += tmp;
					}
					if (CHA > 0.0)
					{
						rArea += CHA;
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" + %g(CHA)"), CHA);
						else
							tmp.Format(_T(" + %g"), CHA);
						noteArea += tmp;
					}
					if (R > 0.0)
					{
						rArea = (2.0*PI*R)*H2 - (LW + RW)*H2 - ACV;
						rArea = Round(rArea, 3);
						if (LW == 0.0 && RW == 0.0)
						{
							if (_bShowVariable)
								noteArea.Format(_T("(2 * PI * %g(R)) * %g(H')"), R, H2);
							else
								noteArea.Format(_T("(2 * PI * %g) * %g"), R, H2);
						}
						else
						{
							if (_bShowVariable)
								noteArea.Format(_T("(2 * PI * %g(R)) * %g(H') - (%g(LW) + %g(RW)) * %g(H')"), R, H2, LW, RW, H2);
							else
								noteArea.Format(_T("(2 * PI * %g) * %g - (%g + %g) * %g"), R, H2, LW, RW, H2);
						}
						if (ACV0 > 0.0)
						{
							CString tmp;
							if (_bShowVariable) tmp.Format(_T(" - %g(ACV)"), ACV0);
							else tmp.Format(_T(" - %g"), ACV0);
							noteArea += tmp;
						}
						if (ACV1 > 0.0)
						{
							CString tmp;
							if (_bShowVariable) tmp.Format(_T(" - %g(ACV)"), ACV1);
							else tmp.Format(_T(" - %g"), ACV1);
							noteArea += tmp;
						}
						if (ACV2 > 0.0)
						{
							CString tmp;
							if (_bShowVariable) tmp.Format(_T(" - %g(ACV)"), ACV2);
							else tmp.Format(_T(" - %g"), ACV2);
							noteArea += tmp;
						}
						if (ACV3 > 0.0)
						{
							CString tmp;
							if (_bShowVariable) tmp.Format(_T(" - %g(ACV)"), ACV3);
							else tmp.Format(_T(" - %g"), ACV3);
							noteArea += tmp;
						}
					}
					result.formDir.push_back(_T("Side"));
					result.formWorkName.push_back(mp_Owner->ma_UserFormWork[0].formWorkName);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
				else
				{
					map<CString, CString> dirToType;
					for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
						dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
					int total_num = 0;
					for (int i = 0; i < 4; ++i)
					{
						CString cur_dir[] = { _T("0"),_T("1"), _T("2"), _T("3") };
						double cur_value[] = { W, D, W, D };
						double cur_cheader[] = { CHA0, CHA1, CHA2, CHA3 };
						double cur_subcheader[] = { SUBCHA0, SUBCHA1, SUBCHA2, SUBCHA3 };
						double cur_ACV[] = { ACV0, ACV1, ACV2, ACV3 };
						if (dirToType.count(cur_dir[i]) > 0)
						{
							total_num++;
							rArea = (cur_value[i])*H2;
							rArea = Round(rArea, 3);
							//noteArea.Format(_T("(%g) x %g"), cur_value[i], H2);
							if (i % 2 == 0)
							{
								if (_bShowVariable)
									noteArea.Format(_T("%g(W) * %g"), cur_value[i], H2);
								else
									noteArea.Format(_T("%g * %g"), cur_value[i], H2);
							}
							else
							{
								if (_bShowVariable)
									noteArea.Format(_T("%g(D) * %g"), cur_value[i], H2);
								else
									noteArea.Format(_T("%g * %g"), cur_value[i], H2);
							}
							if (cur_cheader[i] > 0.0)
							{
								rArea += cur_cheader[i];

								//Q24-511 주두부 타입 추가								
								if (headerM != NULL)
								{
									if (headerType == 1) // 주두부 type-2 인 경우 0,2 번째 면에 서브주두부 추가
									{
										if (i == 0 || i == 2)
											rArea += cur_subcheader[i];
									}
									else if (headerType == 3) // 주두부 type-4 인 경우 마지막 면에 서브주두부 추가
									{
										if (i == 3)
											rArea += cur_subcheader[i];
									}
								}

								CString tmp;
								if (_bShowVariable) tmp.Format(_T(" + %g(CHA)"), cur_cheader[i]);
								else tmp.Format(_T(" + %g"), cur_cheader[i]);
								noteArea += tmp;
							}
							if (cur_ACV[i] > 0.0)
							{
								rArea -= cur_ACV[i];
								CString tmp;
								if (_bShowVariable) tmp.Format(_T(" - %g(ACV)"), cur_ACV[i]);
								else tmp.Format(_T(" - %g"), cur_ACV[i]);
								noteArea += tmp;
							}
							result.formDir.push_back(_T("Side"));
							result.formWorkName.push_back(dirToType[cur_dir[i]]);
							result.formArea.push_back(rArea);
							result.formNote.push_back(noteArea);
						}
					}

					// Put Together Same Type
					int count = result.formWorkName.size();

					// BH-4814 현재 Normal Column은 사각기둥으로 간주하고 거푸집 계산을 하는데
					// LW, RW (좌측, 우측 옹벽으로 가려진 길이) 계산시 좌/우 구분이 정확하게 되지 않아 LW, RW가 실제 좌/우 옹벽에 의한것인지 불분명하다. (다만 LW + RW 총합은 일정)
					// 거푸집 타입이 "None"이 있더라도 같은 거푸집 타입끼리 묶어서 (현재 거푸집 타입 개수 / 전체 거푸집 개수) 로 나누는데, 전체 거푸집 개수에 "None" 타입의 개수가 포함되는 문제가 있다.
					// 이러한 문제를 해결하기 위해 같은 거푸집을 묶을 때 전체 거푸집 개수에서 "None" 타입 거푸집을 제외하고 계산한다.
					// 만약 LW, RW가 "None"타입 거푸집에 속해있더라도 일반 거푸집으로 포함된다. (현재 LW, RW 계산이 불분명하고 거푸집 타입을 구분하지 않으므로)
					int noneTypeCount = 0;
					for (int i = 0; i < count; i++)
						if (result.formWorkName[i] == "None")
							noneTypeCount++;

					vector<int> count_list;
					for (int i = 0; i < count; ++i)
						count_list.push_back(1);
					for (int i = count - 1; i >= 0; --i)
					{
						for (int k = 0; k < i; ++k)
						{
							if (result.formWorkName[i] == result.formWorkName[k])
							{
								CString tmp; tmp.Format(_T("%s + %s"), result.formNote[k], result.formNote[i]);
								result.formNote[k] = tmp;
								result.formArea[k] += result.formArea[i];
								count_list[k] += 1;

								result.formDir.erase(result.formDir.begin() + i);
								result.formWorkName.erase(result.formWorkName.begin() + i);
								result.formArea.erase(result.formArea.begin() + i);
								result.formNote.erase(result.formNote.begin() + i);
								count_list.erase(count_list.begin() + i);

								break;
							}
						}
					}

					if (result.formWorkName.size() > 0 && total_num - noneTypeCount != 0)
					{
						if (LW == 0.0 && RW == 0.0 && ACV == 0.0) {}
						else
						{
							for (int i = 0; i < result.formWorkName.size(); ++i)
							{
								CString tmp;
								if (LW > 0.0 || RW > 0.0)
								{
									result.formArea[i] -= (LW + RW)*H2 * ((double)count_list[i] / ((double)total_num - (double)noneTypeCount));
									if (count_list[i] != (total_num - noneTypeCount))
									{
										if (_bShowVariable)
											tmp.Format(_T(" - ((%g(LW) + %g(RW)) * %g(H')) * (%d / %d)"), LW, RW, H2, count_list[i], total_num - noneTypeCount);
										else
											tmp.Format(_T(" - ((%g + %g) * %g ) * (%d / %d)"), LW, RW, H2, count_list[i], total_num - noneTypeCount);
									}
									else
									{
										if (_bShowVariable)
											tmp.Format(_T(" - ((%g(LW) + %g(RW)) * %g(H'))"), LW, RW, H2);
										else
											tmp.Format(_T(" - ((%g + %g) * %g )"), LW, RW, H2);
									}
									result.formNote[i] += tmp;
								}
							}
							/*
							rArea = -(LW+RW)*H2;
							//noteArea.Format(_T("- (%g + %g) x %g"), LW, RW, H2);
							noteArea.Format(_T("- (%g(LW) + %g(RW)) x %g(H')"), LW, RW, H2);
							result.formType.push_back( mp_Owner->ma_UserFormWork[0].type );
							result.formArea.push_back(rArea);
							result.formNote.push_back(noteArea);
							*/
						}
					}
				}
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneCustomColumn(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
{
	//columnG->m_VolumeAreaEMS.SetParamCustomColumn(profileLine,WCL, WCV, BCV, col_Area, H, TO, BO, TS, TB, );
	if (mp_Owner->GetType() == MSElement::msColumnG || mp_Owner->GetType() == MSElement::msWallColumnG)
	{
		GM2DPolyline* profile = mp_Owner->GetProfile();
		long NF = profile->GetNumLines();
		if (ma_Param.size() != 6 + 4 * NF) return;

		double rArea = 0;
		CString noteArea;
		vector<double> profileLine;
		vector<double> WCL;
		vector<double> WCV;
		vector<double> BCV;

		//ma_Param을 calculate 했던 자료로 변환
		double Area = ma_Param[4 * NF], H = ma_Param[4 * NF + 1], TO = ma_Param[4 * NF + 2], BO = ma_Param[4 * NF + 3];
		double TS = ma_Param[4 * NF + 4], TB = ma_Param[4 * NF + 5];

		for (int i = 0; i < NF * 4; i++)
		{
			if (i >= 0 && i < NF)
				profileLine.push_back(ma_Param[i]);
			else if (i >= NF && i < NF * 2)
				WCL.push_back(ma_Param[i]);
			else if (i >= NF * 2 && i < NF * 3)
				WCV.push_back(ma_Param[i]);
			else if (i >= NF * 3 && i < NF * 4)
				BCV.push_back(ma_Param[i]);
		}

		// 밖으로 내보낼 수식 정리
		double H2 = H + TO - BO - TS; //H2 : 계산용 높이
		if (TB > TS)
		{
			H2 = H2 + TS - TB;
		}

		// Concrete
		rVolume = Area * H2;
		rVolume = Round(rVolume, 3);
		if (_bShowVariable)
			noteVolume.Format(_T("%g(A) * %g(H')"), Area, H2);
		else
			noteVolume.Format(_T("%g * %g"), Area, H2);

		//src 추후		
		/*if (mp_Owner->IsSRC())
		{
			bool isSteelExclude = mp_Owner->GetBuilding()->GetQuantityOption()->m_bIsSteelQuantityExclude;
			if (isSteelExclude)
			{
				double dSteelArea = mp_Owner->GetCompoM()->GetSteelSection()->GetArea() * GM2ACIS_A;
				rVolume -= dSteelArea * H2;
				rVolume = Round(rVolume, 3);
				CString steelStr;
				if (_bShowVariable)
					steelStr.Format(_T(" - (%g(STL) x %g(H'))"), dSteelArea, H2);
				else
					steelStr.Format(_T(" - (%g x %g)"), dSteelArea, H2);
				noteVolume += steelStr;
			}
		}*/

		// Form Area
		bool all_same = true;
		for (int i = 0; i < (int)(mp_Owner->ma_UserFormWork.size()) - 1; ++i)
		{
			if (mp_Owner->ma_UserFormWork[i].formWorkName != mp_Owner->ma_UserFormWork[i + 1].formWorkName)
				all_same = false;
		}
		if (mp_Owner->ma_UserFormWork.size() > 0)
		{
			if (all_same)
			{
				double lineTotal = 0.0, WCLTotal = 0.0, WCVTotal = 0.0, BCVTotal = 0.0;
				for (int i = 0; i < NF; i++)
				{
					lineTotal += profileLine[i];
					WCLTotal += WCL[i];
					WCVTotal += WCV[i];
					BCVTotal += BCV[i];
				}
				rArea = lineTotal * H2 - WCVTotal - BCVTotal;
				rArea = Round(rArea, 3);

				if (_bShowVariable)
					noteArea.Format(_T("%g(OL) * %g(H')"), lineTotal, H2);
				else
					noteArea.Format(_T("%g * %g"), lineTotal, WCLTotal, H2);

				if (WCVTotal > 0.0)
				{
					CString tmp;
					if (_bShowVariable)
						tmp.Format(_T(" - %g(WCV)"), WCVTotal);
					else
						tmp.Format(_T(" - %g"), WCVTotal);
					noteArea += tmp;
				}

				if (BCVTotal > 0.0)
				{
					CString tmp;
					if (_bShowVariable)
						tmp.Format(_T(" - %g(BCV)"), BCVTotal);
					else
						tmp.Format(_T(" - %g"), BCVTotal);
					noteArea += tmp;
				}

				result.formDir.push_back(_T("Side"));
				result.formWorkName.push_back(mp_Owner->ma_UserFormWork[0].formWorkName);
				result.formArea.push_back(rArea);
				result.formNote.push_back(noteArea);
			}
			else
			{
				map<CString, CString> dirToType;
				for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
				{
					CString dirstr;  dirstr = mp_Owner->ma_UserFormWork[i].dir;
					dirToType[dirstr] = mp_Owner->ma_UserFormWork[i].formWorkName;
				}

				int total_num = 0;
				for (int i = 0; i < NF; ++i)
				{
					CString dirstr; dirstr.Format(_T("%d"), i);
					if (dirToType.count(dirstr) > 0)
					{
						total_num++;
						rArea = profileLine[i] * H2 - WCV[i] - BCV[i];
						rArea = Round(rArea, 3);

						if (_bShowVariable)
							noteArea.Format(_T("%g(L) * %g"), profileLine[i], H2);
						else
							noteArea.Format(_T("%g * %g"), profileLine[i], H2);

						if (WCV[i] > 0.0)
						{
							CString tmp;
							if (_bShowVariable) tmp.Format(_T(" - %g(WCV)"), WCV[i]);
							else tmp.Format(_T(" - %g"), WCV[i]);
							noteArea += tmp;
						}

						if (BCV[i] > 0.0)
						{
							CString tmp;
							if (_bShowVariable) tmp.Format(_T(" - %g(BCV)"), BCV[i]);
							else tmp.Format(_T(" - %g"), BCV[i]);
							noteArea += tmp;
						}

						// BH-4478 기둥의 한쪽 면이 모두 공제되었을 떄 공제면적은 ACIS로 계산한다. 그러나 ACIS는 Trim시 9e-4 m 만큼 늘려서 Subtract하므로 실제 ACIS 모델은 크기가 약간 작다.
						// 이러한 차이로 인해 매우 적은 면적이 산출되는 것을 막는다.
						if (abs(rArea / H) < 0.01)
							continue;

						result.formDir.push_back(_T("Side"));
						result.formWorkName.push_back(dirToType[dirstr]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);
					}
				}

				// Put Together Same Type
				int count = result.formWorkName.size();
				vector<int> count_list;
				for (int i = 0; i < count; ++i)
					count_list.push_back(1);
				for (int i = count - 1; i >= 0; --i)
				{
					for (int k = 0; k < i; ++k)
					{
						if (result.formWorkName[i] == result.formWorkName[k])
						{
							CString tmp; tmp.Format(_T("%s + %s"), result.formNote[k], result.formNote[i]);
							result.formNote[k] = tmp;
							result.formArea[k] += result.formArea[i];
							count_list[k] += 1;

							result.formDir.erase(result.formDir.begin() + i);
							result.formWorkName.erase(result.formWorkName.begin() + i);
							result.formArea.erase(result.formArea.begin() + i);
							result.formNote.erase(result.formNote.begin() + i);
							count_list.erase(count_list.begin() + i);

							break;
						}
					}
				}
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneBeam(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes)
{
	if (mp_Owner->IsCustomBeam())
	{
		GetResultByCZoneCustomBeam(cz_id, rVolume, noteVolume, result);
	}
	else
		GetResultByCZoneNormalBeam(cz_id, rVolume, noteVolume, result, calcType, subIds, subVolumes, subNotes);
}

void VolumeAreaInfoEMS::GetResultByCZoneCustomBeam(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
{
	//이형단면 V, left, right, bottom
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msBeamG)
	{
		MSBeamG* beamG = dynamic_cast<MSBeamG*>(mp_Owner);
		if (ma_Param.size() != 9) return;

		double A = ma_Param[0], TSA = ma_Param[1], CL = ma_Param[2], LL = ma_Param[3], RL = ma_Param[4], leftOL = ma_Param[5], rightOL = ma_Param[6], bottomOL = ma_Param[7], WAL = ma_Param[8];

		// BH-4478 콘크리트 물량 계산시 사용되는 길이값을 LL과 RL의 평균으로 계산하면 벽에 기둥이 맞닿아있는 경우 잘못된 물량이 계산된다.
		double LAvg = CL;
		//double LAvg = (LL + RL) / 2.0;
		// Concrete
		rVolume = (A - TSA) * (LAvg);
		rVolume = Round(rVolume, 3);
		if (TSA > 0.0)
		{
			if (_bShowVariable)
				noteVolume.Format(_T("(%g(A) - %g(TSA)) * %g(L')"), A, TSA, LAvg);
			else
				noteVolume.Format(_T("(%g - %g) * %g"), A, TSA, LAvg);
		}
		else
		{
			if (_bShowVariable)
				noteVolume.Format(_T("%g(A) * %g(L')"), A, LAvg);
			else
				noteVolume.Format(_T("%g * %g"), A, LAvg);
		}

		// Form Area (LR)
		rArea = LL * leftOL + RL * rightOL;
		rArea = Round(rArea, 3);
		if (_bShowVariable)
			noteArea.Format(_T("%g(LL) * %g(LOL) + %g(RL) * %g(ROL)"), LL, leftOL, RL, rightOL);
		else
			noteArea.Format(_T("%g * %g + %g * %g"), LL, leftOL, RL, rightOL);

		if (mp_Owner->ma_UserFormWork.size() > 0)
		{
			map<CString, CString> dirToType;
			for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
				dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
			bool bLRSame = (dirToType.count(_T("L")) > 0 && dirToType.count(_T("L")) > 0 && dirToType[_T("L")] == dirToType[_T("R")]);
			if (bLRSame)
			{
				rArea = LL * leftOL + RL * rightOL;
				rArea = Round(rArea, 3);
				if (rArea < 0) rArea = 0;
				if (_bShowVariable)
					noteArea.Format(_T("%g(LL) * %g(LOL) + %g(RL) * %g(ROL)"), LL, leftOL, RL, rightOL);
				else
					noteArea.Format(_T("%g * %g + %g * %g"), LL, leftOL, RL, rightOL);

				result.formDir.push_back(_T("Side"));
				result.formWorkName.push_back(dirToType[_T("L")]);
				result.formArea.push_back(rArea);
				result.formNote.push_back(noteArea);
			}
			else
			{
				for (int i = 0; i < 2; ++i)
				{
					CString cur_dir = (i == 0 ? _T("L") : _T("R"));
					double memberLength = (i == 0 ? LL : RL);
					double outLength = (i == 0 ? leftOL : rightOL);
					if (dirToType.count(cur_dir) > 0)
					{
						rArea = outLength * memberLength;
						rArea = Round(rArea, 3);
						if (rArea < 0) rArea = 0;
						if (_bShowVariable)
						{
							if (i == 0)
								noteArea.Format(_T("%g(LL) * %g(LOL)"), memberLength, outLength);
							else
								noteArea.Format(_T("%g(RL) * %g(ROL)"), memberLength, outLength);
						}
						else
						{
							if (i == 0)
								noteArea.Format(_T("%g * %g"), memberLength, outLength);
							else
								noteArea.Format(_T("%g * %g"), memberLength, outLength);
						}

						if (cur_dir == _T("L"))
							result.formDir.push_back(_T("Left"));
						else
							result.formDir.push_back(_T("Right"));
						result.formWorkName.push_back(dirToType[cur_dir]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);
					}
				}
			}

			//if (INC_BOT == 1.0 && dirToType.count(_T("B")) > 0)
			if (dirToType.count(_T("B")) > 0)
			{
				rArea = LAvg * bottomOL - WAL;
				rArea = Round(rArea, 3);

				if (WAL > 0)
				{
					if (_bShowVariable)
						noteArea.Format(_T("%g(L') * %g(BOL) - %g(WAL)"), LAvg, bottomOL, WAL);
					else
						noteArea.Format(_T("%g * %g - %g"), LAvg, bottomOL, WAL);
				}
				else
				{
					if (_bShowVariable)
						noteArea.Format(_T("%g(L') * %g(BOL)"), LAvg, bottomOL);
					else
						noteArea.Format(_T("%g * %g"), LAvg, bottomOL);
				}

				result.formDir.push_back(_T("Bot"));
				result.formWorkName.push_back(dirToType[_T("B")]);
				result.formArea.push_back(rArea);
				result.formNote.push_back(noteArea);
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneNormalBeam(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes)
{
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msBeamG)
	{
		MSBeamG* beamG = dynamic_cast<MSBeamG*>(mp_Owner);
		int builtupBeamCount = beamG->mm_BuiltUp.size();
		if (builtupBeamCount > 0)
		{
			if (ma_Param.size() != (20 + builtupBeamCount * 7)) return;
		}
		else			
		{
			if (ma_Param.size() != 20) return;
		}

		double W = ma_Param[0], D = ma_Param[1], A = ma_Param[2], CL = ma_Param[3], LL = ma_Param[4], RL = ma_Param[5], TS = ma_Param[6], ACV = ma_Param[7];
		double W1 = ma_Param[8], W2 = ma_Param[9], D1 = ma_Param[10], D2 = ma_Param[11];	// 기본적으로 W2와 W, D2와 D는 각각 동일하다.
		double INC_BOT = ma_Param[12], WAL = ma_Param[13], VH = ma_Param[14];
		double LACV = ma_Param[15], RACV = ma_Param[16], LLF = ma_Param[17], RLF = ma_Param[18], PCCL = ma_Param[19];

		map<int, vector<double>> builtupParam;
		for (int i = 0; i < builtupBeamCount; i++)
		{
			vector<double> tmpParam;
			for (int j = 1; j < 8; j++)
				tmpParam.push_back(ma_Param[19 + i * 7 + j]);

			builtupParam.insert(make_pair(i, tmpParam));
		}

		// BH-4478 콘크리트 물량 계산시 사용되는 길이값을 LL과 RL의 평균으로 계산하면 벽에 기둥이 맞닿아있는 경우 잘못된 물량이 계산된다.
		double LAvg = CL;
		//double LAvg = (LL+RL)/2.0;

		CString strW;
		CString strD_TS;
		double Wp = W;
		double D_TS = D;
		if (calcType == 0)	// 전체 (기존 로직)
		{
			Wp = W;
			if (_bShowVariable)
				strW.Format(_T("%g(B)"), W);
			else
				strW.Format(_T("%g"), W);

			if (TS > 0)
				D_TS = D - TS;
			else
				D_TS = D;

			if (_bShowVariable)
			{
				if (TS > 0)
					strD_TS.Format(_T("(%g(D) - %g(TS))"), D, TS);
				else
					strD_TS.Format(_T("%g(D)"), D);
			}
			else
			{
				if (TS > 0)
					strD_TS.Format(_T("(%g - %g)"), D, TS);
				else
					strD_TS.Format(_T("%g"), D);
			}
		}
		else if (calcType == 1 && mp_Owner->IsPC())	// RC부분
		{
			Wp = W2;
			if (_bShowVariable)
				strW.Format(_T("%g(B2)"), W2);
			else
				strW.Format(_T("%g"), W2);

			if (TS > 0)
			{
				if (D2 - TS > 0)
					D_TS = D2 - TS;
				else
					D_TS = 0;
			}
			else
				D_TS = D2;

			if (_bShowVariable)
			{
				if (TS > 0)
				{
					if (D2 - TS > 0)
						strD_TS.Format(_T("(%g(D2) - %g(TS))"), D2, TS);
					else
						strD_TS.Format(_T("(%g(D2) - %g(TS))"), D2, D2);	// 0이라서 D2 - D2로 계산한다.
				}
				else
					strD_TS.Format(_T("%g(D2)"), D2);
			}
			else
			{
				if (TS > 0)
				{
					if (D2 - TS > 0)
						strD_TS.Format(_T("(%g - %g)"), D2, TS);
					else
						strD_TS.Format(_T("(%g - %g)"), D2, D2);			// 0이라서 D2 - D2로 계산한다.
				}
				else
					strD_TS.Format(_T("%g"), D2);
			}
		}
		else if (calcType == 2 && mp_Owner->IsPC())	// PC부분
		{
			Wp = W1;
			if (_bShowVariable)
				strW.Format(_T("%g(B1)"), W1);
			else
				strW.Format(_T("%g"), W1);

			D_TS = D1;
			if (_bShowVariable)
				strD_TS.Format(_T("%g(D1)"), D1);
			else
				strD_TS.Format(_T("%g"), D1);
		}
		else
			return;

		// Concrete

		// BH-4478 길이값을 Center Line으로 하였으나, 기둥이 가운데에 들어가는 등 복잡하게 모델링 되는 경우 바닥의 면적x높이로 계산한다.
		bool isRegularShape = abs((A * D_TS) - (W * D_TS * LAvg)) < 0.1;
		if (A == 0) // A가 계산되어 있지 않으면 기존 로직으로 한다.
			isRegularShape = true;

		if (calcType == 2 && mp_Owner->IsPC())	// PC보의 PC 제작부위
		{
			LAvg = PCCL;
		}

		if (isRegularShape)
		{
			rVolume = D_TS * Wp  * LAvg;
			if (_bShowVariable)
				noteVolume.Format(_T("%s * %s * %g(L')"), strW, strD_TS, LAvg);
			else
				noteVolume.Format(_T("%s * %s * %g"), strW, strD_TS, LAvg);
		}
		else
		{
			if (calcType == 2 && mp_Owner->IsPC())	// PC보의 PC 제작부위
			{
				// [Q23-115] A 값이 PC보의 현장타설 면적이다.
				// W1와 W2의 비율을 찾아 PC보의 제작 면적을 계산하도록 수정
				double pcA = (W1 / W2) * A;
				rVolume = D_TS * pcA;
				if (_bShowVariable)
					noteVolume.Format(_T("%g(A) * %s"), pcA, strD_TS);
				else
					noteVolume.Format(_T("%g * %s"), pcA, strD_TS);
			}
			else
			{
				rVolume = D_TS * A;
				if (_bShowVariable)
					noteVolume.Format(_T("%g(A) * %s"), A, strD_TS);
				else
					noteVolume.Format(_T("%g * %s"), A, strD_TS);
			}
		}

		if (mp_Owner->IsSRC())
		{
			bool isSteelExclude = mp_Owner->GetBuilding()->GetQuantityOption()->m_bIsSteelQuantityExclude;
			if (isSteelExclude)
			{
				double dSteelArea = mp_Owner->GetCompoM()->GetSteelSection()->GetArea() * GM2ACIS_A;
				rVolume -= dSteelArea * LAvg;
				CString steelStr;
				if (_bShowVariable)
					steelStr.Format(_T(" - (%g(STL) * %g(L'))"), dSteelArea, LAvg);
				else
					steelStr.Format(_T(" - (%g * %g)"), dSteelArea, LAvg);
				noteVolume += steelStr;
			}
		}

		// Haunch (same matl)
		if (VH > 0)
		{
			rVolume += VH;
			CString str;
			if (_bShowVariable)
				str.Format(_T(" + %g(VH)"), VH);
			else
				str.Format(_T(" + %g"), VH);
			noteVolume += str;
		}

		double Factor = 0.0;
		double dLibLathArea = 0.0;

		if (cz_id != -1)
		{
			for (int i = 0; i < ma_CZoneID.size(); ++i)
			{
				if (cz_id == ma_CZoneID[i])
				{
					Factor = ma_Factor[i];
					dLibLathArea = ma_LibLathArea[i];
				}
			}
		}
		else
		{
			Factor = 1.0;
			dLibLathArea = 1.0;
		}
		if (Factor == 0.0) return;

		CString czoneName;
		CString czoneNote;
		if (Factor != 1.0)
		{
			MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
			MSConstructionZone* czone = curFloor->GetCZone(cz_id);
			czoneName = czone->GetName();
			rVolume *= Factor;
			czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteVolume);
			noteVolume = czoneNote;
		}

		if (builtupBeamCount > 0)
		{
			// builtupBeamParam :  0 ->builtupStartH  1-> builtupEndH  2->builtupCL 3-> builtupLL; 4-> builtupRL 5->isSameMatl
			map<int, vector<double>>::iterator it;
			for (it = builtupParam.begin(); it != builtupParam.end(); it++)
			{
				//bool isSameMatl = it->second[5];
				//if (isSameMatl == false) continue; //double diff 0,same 1로 구분됨 

				double builtUpTS = it->second[6];
				builtUpTS = Round(builtUpTS, 3);

				double builtupV = 1;
				builtupV = Wp * (it->second[0] - builtUpTS) * (it->second[3] + it->second[4]) / 2;
				builtupV = Round(builtupV, 3);

				int index = it->first;
				MSBuiltUpBeamG* builtUp = dynamic_cast<MSBuiltUpBeamG*>(beamG->GetSubMembers()[index]);
				vector<int> builtUpCZoneIDs = builtUp->m_VolumeAreaEMS.ma_CZoneID;

				double builtUpFactor = 0.0;
				for (int i = 0; i < builtUpCZoneIDs.size(); ++i)
				{
					if (cz_id == builtUpCZoneIDs[i])
						builtUpFactor = builtUp->m_VolumeAreaEMS.ma_Factor[i];
				}

				CString str;
				if (builtUpFactor == 1.0)
				{					
					if (_bShowVariable)
					{
						if (builtUpTS > 0)
							str.Format(_T("%s * (%g(H) - %g(TS)) * %g(L')"), strW, it->second[0], builtUpTS, (it->second[3] + it->second[4]) / 2);
						else
							str.Format(_T("%s * %g(H) * %g(L')"), strW, it->second[0], (it->second[3] + it->second[4]) / 2);
					}
					else
					{
						if (builtUpTS > 0)
							str.Format(_T("%s * (%g - %g) * %g"), strW, it->second[0], builtUpTS,(it->second[3] + it->second[4]) / 2);
						else
							str.Format(_T("%s * %g * %g"), strW, it->second[0], (it->second[3] + it->second[4]) / 2);
					}
				}
				else
				{
					if (builtUpFactor > 0.0)
					{
						MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
						if (curFloor != NULL)
						{
							MSConstructionZone* czone = curFloor->GetCZone(cz_id);
							if (_bShowVariable)
								str.Format(_T("[%s : %g] * %s * %g(H) * %g(L')"), czone->GetName(), builtUpFactor, strW, it->second[0], (it->second[3] + it->second[4]) / 2);
							else
								str.Format(_T("[%s : %g] * %s * %g * %g"), czone->GetName(), builtUpFactor, strW, it->second[0], (it->second[3] + it->second[4]) / 2);
						}
					}
				}

				//rVolume += builtupV * builtUpFactor;
				//noteVolume += str;
				subIds.push_back(builtUp->GetID());
				subVolumes.push_back(builtupV * builtUpFactor);
				subNotes.push_back(str);
			}
		}

		// Form Area (LR)
		if (calcType == 0 || calcType == 1)		// 전체 또는 PC기둥 RC부분일 때만 거푸집 계산
		{
			double builtFormVolume = 0;
			double builtUpFactorVolume = 0;
			vector<double> builtFormAreaVolume;
			vector<CString> builtStr;
			int builtup_id = 0;
			vector<long> builtId;
			CString str_built;
			//rArea = D_TS * LAvg * 2;
			//if (_bShowVariable)
			//	noteArea.Format(_T("%s * %g * 2"), strD_TS, LAvg);
			//else
			//	noteArea.Format(_T("%s * %g * 2"), strD_TS, LAvg);

			if (mp_Owner->ma_UserFormWork.size() > 0)
			{
				map<CString, CString> dirToType;
				for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
					dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
				bool bLRSame = (dirToType.count(_T("L")) > 0 && dirToType.count(_T("L")) > 0 && dirToType[_T("L")] == dirToType[_T("R")]);
				if (bLRSame)
				{
					rArea = D * (LLF + RLF) - LACV - RACV;
					if (rArea < 0) rArea = 0;
					if (_bShowVariable)
						noteArea.Format(_T("%g * (%g(LL) + %g(RL))"), D, LLF, RLF);
					else
						noteArea.Format(_T("%g * (%g + %g)"), D, LLF, RLF);

					CString tmp;
					if (_bShowVariable)
						tmp.Format(_T(" - %g(LACV)"), LACV);
					else
						tmp.Format(_T(" - %g"), LACV);
					if (LACV > 0) noteArea += tmp;

					if (_bShowVariable)
						tmp.Format(_T(" - %g(RACV)"), RACV);
					else
						tmp.Format(_T(" - %g"), RACV);
					if (RACV > 0) noteArea += tmp;

					if (builtupBeamCount > 0)
					{ // builtupBeamParam :  0 ->builtupStartH  1-> builtupEndH  2->builtupCL 3-> builtupLL; 4-> builtupRL
						map<int, vector<double>>::iterator it;
						for (it = builtupParam.begin(); it != builtupParam.end(); it++)
						{
							double builtupA = 1;
							double builtupH = it->second[1];
							builtupA = builtupH * (it->second[3] + it->second[4]);
							builtupA = Round(builtupA, 3);
							int index = it->first;
							MSBuiltUpBeamG* builtUp = dynamic_cast<MSBuiltUpBeamG*>(beamG->GetSubMembers()[index]);
							vector<int> builtUpCZoneIDs = builtUp->m_VolumeAreaEMS.ma_CZoneID;
							long built_inside_id = builtUp->GetID();
							double builtUpFactor = 0.0;
							for (int i = 0; i < builtUpCZoneIDs.size(); ++i)
							{
								if (cz_id == builtUpCZoneIDs[i])
									builtUpFactor = builtUp->m_VolumeAreaEMS.ma_Factor[i];
							}

							double leftACV = 0;
							map<int, vector<double>>::iterator itLeft;
							for (itLeft = ma_BuiltUpBeamLeftACV.begin(); itLeft != ma_BuiltUpBeamLeftACV.end(); itLeft++)
							{
								if (itLeft->first != it->first) continue;
								for (int i = 0; i < itLeft->second.size(); i++)
									leftACV += itLeft->second[i] * GM2ACIS_A;
							}
							builtupA -= leftACV;

							double rightACV = 0;
							map<int, vector<double>>::iterator itRight;
							for (itRight = ma_BuiltUpBeamRightACV.begin(); itRight != ma_BuiltUpBeamRightACV.end(); itRight++)
							{
								if (itRight->first != it->first) continue;
								for (int i = 0; i < itRight->second.size(); i++)
									rightACV += itRight->second[i] * GM2ACIS_A;
							}
							builtupA -= rightACV;

							if (builtupA < 0)
								builtupA = 0;

							CString str;
							if (builtUpFactor == 1.0)
							{
								if (_bShowVariable)
									str.Format(_T(" %g(H) * %g(L') * 2"), builtupH, (it->second[3] + it->second[4]) / 2);
								else
									str.Format(_T(" %g * %g * 2"), builtupH, (it->second[3] + it->second[4]));
							}
							else
							{
								if (builtUpFactor > 0.0)
								{
									MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
									if (curFloor != NULL)
									{
										MSConstructionZone* czone = curFloor->GetCZone(cz_id);
										if (_bShowVariable)
											str.Format(_T(" [%s : %g] * %g(H) * %g(L') * 2"), czone->GetName(), builtUpFactor, builtupH, (it->second[3] + it->second[4]) / 2);
										else
											str.Format(_T(" [%s : %g] * %g * %g * 2"), czone->GetName(), builtUpFactor, builtupH, (it->second[3] + it->second[4]));										
									}
								}
							}

							CString tmp;
							if (_bShowVariable)
								tmp.Format(_T(" - %g(LACV)"), leftACV);
							else
								tmp.Format(_T(" - %g"), leftACV);
							if (leftACV > 0) str += tmp;

							if (_bShowVariable)
								tmp.Format(_T(" - %g(RACV)"), rightACV);
							else
								tmp.Format(_T(" - %g"), rightACV);
							if (rightACV > 0) str += tmp;

							//noteArea += str;
							if (builtupParam.size() == 1)
							{
								builtFormVolume = builtupA;
								builtUpFactorVolume = builtUpFactor;
								str_built = str;
								builtup_id = builtUp->GetID();
							}
							else
							{
								builtFormAreaVolume.push_back((builtupA * builtUpFactor));
								builtStr.push_back(str);
								builtId.push_back(builtUp->GetID());
							}
						}
					}

					if (Factor != 1.0)
					{
						rArea *= Factor;
						czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
						noteArea = czoneNote;
					}

					result.formDir.push_back(_T("Side"));
					result.formWorkName.push_back(dirToType[_T("L")]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
					result.formId.push_back(beamG->GetID());

					if (builtFormAreaVolume.size() == 0)
					{
						result.formDir.push_back(_T("BuiltupBeam"));
						result.formWorkName.push_back(dirToType[_T("L")]);
						result.formArea.push_back(builtUpFactorVolume * builtFormVolume);
						result.formNote.push_back(str_built);
						result.formId.push_back(builtup_id);
					}
					else
					{
						for (int i = 0; i < builtFormAreaVolume.size(); i++)
						{
							result.formDir.push_back(_T("BuiltupBeam"));
							result.formWorkName.push_back(dirToType[_T("L")]);
							result.formArea.push_back(builtFormAreaVolume[i]);
							result.formNote.push_back(builtStr[i]);
							result.formId.push_back(builtId[i]);
						}
					}
				}
				else
				{
					for (int i = 0; i < 2; ++i)
					{
						CString cur_dir = (i == 0 ? _T("L") : _T("R"));
						double LCur = (i == 0 ? LLF : RLF);
						if (dirToType.count(cur_dir) > 0)
						{
							if (cur_dir == _T("L"))
							{
								rArea = D * LCur - LACV;
								if (rArea < 0) rArea = 0;
								if (_bShowVariable)	
									noteArea.Format(_T("%g * %g(LL)"), D, LCur);								
								else
									noteArea.Format(_T("%g * %g"), D, LCur);

								CString tmp;
								if (_bShowVariable)
									tmp.Format(_T(" - %g(LACV)"), LACV);
								else
									tmp.Format(_T(" - %g"), LACV);
								if (LACV > 0) noteArea += tmp;
							}
							else
							{
								rArea = D * LCur - RACV;
								if (rArea < 0) rArea = 0;
								if (_bShowVariable)
									noteArea.Format(_T("%g * %g(RL)"), D, LCur);
								else
									noteArea.Format(_T("%g * %g"), D, LCur);

								CString tmp;
								if (_bShowVariable)
									tmp.Format(_T(" - %g(RACV)"), RACV);
								else
									tmp.Format(_T(" - %g"), RACV);
								if (RACV > 0) noteArea += tmp;
							}							

							if (builtupBeamCount > 0)
							{ // builtupBeamParam :  0 ->builtupStartH  1-> builtupEndH  2->builtupCL 3-> builtupLL; 4-> builtupRL
								map<int, vector<double>>::iterator it;
								for (it = builtupParam.begin(); it != builtupParam.end(); it++)
								{
									double builtupA = 1;
									double builtupH = it->second[1];
									builtupA = builtupH * it->second[3 + i];
									builtupA = Round(builtupA, 3);
									//rArea += builtupA;

									int index = it->first;
									MSBuiltUpBeamG* builtUp = dynamic_cast<MSBuiltUpBeamG*>(beamG->GetSubMembers()[index]);
									vector<int> builtUpCZoneIDs = builtUp->m_VolumeAreaEMS.ma_CZoneID;
									long built_inside_id = builtUp->GetID();
									double builtUpFactor = 0.0;
									for (int bi = 0; bi < builtUpCZoneIDs.size(); ++bi)
									{
										if (cz_id == builtUpCZoneIDs[bi])
											builtUpFactor = builtUp->m_VolumeAreaEMS.ma_Factor[bi];
									}

									double ACV = 0;
									if (i == 0)
									{
										map<int, vector<double>>::iterator itLeft;
										for (itLeft = ma_BuiltUpBeamLeftACV.begin(); itLeft != ma_BuiltUpBeamLeftACV.end(); itLeft++)
										{
											if (itLeft->first != it->first) continue;
											for (int i = 0; i < itLeft->second.size(); i++)
												ACV += itLeft->second[i] * GM2ACIS_A;
										}
									}
									else
									{
										map<int, vector<double>>::iterator itRight;
										for (itRight = ma_BuiltUpBeamRightACV.begin(); itRight != ma_BuiltUpBeamRightACV.end(); itRight++)
										{
											if (itRight->first != it->first) continue;
											for (int i = 0; i < itRight->second.size(); i++)
												ACV += itRight->second[i] * GM2ACIS_A;
										}
									}
									builtupA -= ACV;

									if (builtupA < 0)
										builtupA = 0;

									CString str;
									if (fabs(builtUpFactor - 1) < DTOL_GM)
									{
										if (_bShowVariable)
										{
											if (i == 0)
												str.Format(_T(" %g(H) * %g(LL')"), builtupH, it->second[3 + i]);
											else
												str.Format(_T(" %g(H) * %g(RL')"), builtupH, it->second[3 + i]);
										}
										else
											str.Format(_T(" %g * %g"), builtupH, it->second[3 + i]);
									}
									else
									{
										if (builtUpFactor > 0.0)
										{
											MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
											if (curFloor != NULL)
											{
												MSConstructionZone* czone = curFloor->GetCZone(cz_id);
												if (_bShowVariable)
												{
													if (i == 0)
														str.Format(_T(" [%s : %g] * %g(H) * %g(LL')"), czone->GetName(), builtUpFactor, builtupH, it->second[3 + i]);
													else
														str.Format(_T(" [%s : %g] * %g(H) * %g(RL')"), czone->GetName(), builtUpFactor, builtupH, it->second[3 + i]);
												}
												else
													str.Format(_T(" [%s : %g] * %g * %g"), czone->GetName(), builtUpFactor, builtupH, it->second[3 + i]);
											}
										}
									}

									CString tmp;
									if (_bShowVariable)
									{
										if (i == 0)
											tmp.Format(_T(" - %g(LACV)"), ACV);
										else
											tmp.Format(_T(" - %g(RACV)"), ACV);
									}
									else
										tmp.Format(_T(" - %g"), ACV);
									if (ACV > 0) str += tmp;

									//noteArea += str;
									if (builtupParam.size() == 1)
									{
										builtFormVolume = builtupA;
										builtUpFactorVolume = builtUpFactor;
										str_built = str;
										builtup_id = built_inside_id;
									}
									else
									{
										builtFormAreaVolume.push_back((builtupA * builtUpFactor));
										builtStr.push_back(str);
										builtId.push_back(builtUp->GetID());
									}
								}
							}

							if (Factor != 1.0)
							{
								rArea *= Factor;
								czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
								noteArea = czoneNote;
							}

							if (cur_dir == _T("L"))
								result.formDir.push_back(_T("Left"));
							else
								result.formDir.push_back(_T("Right"));
							result.formWorkName.push_back(dirToType[cur_dir]);
							result.formArea.push_back(rArea);
							result.formNote.push_back(noteArea);
							result.formId.push_back(beamG->GetID());
							if (builtFormAreaVolume.size() == 0)
							{
								result.formDir.push_back(_T("BuiltupBeam"));
								result.formWorkName.push_back(dirToType[_T("L")]);
								result.formArea.push_back(builtUpFactorVolume * builtFormVolume);
								result.formNote.push_back(str_built);
								result.formId.push_back(builtup_id);
							}
							else
							{
								for (int i = 0; i < builtFormAreaVolume.size(); i++)
								{
									result.formDir.push_back(_T("BuiltupBeam"));
									result.formWorkName.push_back(dirToType[_T("L")]);
									result.formArea.push_back(builtFormAreaVolume[i]);
									result.formNote.push_back(builtStr[i]);
									result.formId.push_back(builtId[i]);
								}
							}
						}
					}
				}

				if (INC_BOT == 1.0 && dirToType.count(_T("B")) > 0 && mp_Owner->IsPC() == false)	// PC보는 하부 거푸집은 없다.
				{
					if (WAL > Wp * LAvg) WAL = Wp * LAvg;

					rArea = Wp * LAvg - WAL;
					//noteArea.Format(_T("(%g - %g) x %g"), D, TS, L);
					if (WAL > 0.0)
					{
						if (_bShowVariable)
							noteArea.Format(_T("%s * %g(L') - %g(WAL)"), strW, LAvg, WAL);
						else
							noteArea.Format(_T("%s * %g - %g"), strW, LAvg, WAL);
					}
					else
					{
						if (_bShowVariable)
							noteArea.Format(_T("%s * %g(L')"), strW, LAvg);
						else
							noteArea.Format(_T("%s * %g"), strW, LAvg);
					}

					if (Factor != 1.0)
					{
						rArea *= Factor;
						czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
						noteArea = czoneNote;
					}
					result.formDir.push_back(_T("Bot"));
					result.formWorkName.push_back(dirToType[_T("B")]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
					result.formId.push_back(beamG->GetID());
				}
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneLowWall(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType)
{
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msHandRailG)
	{
		if (ma_Param.size() != 12) return;
		double W = ma_Param[0], H = ma_Param[1], CL = ma_Param[2], L = ma_Param[3], TS = ma_Param[4], TH = ma_Param[5], BH = ma_Param[6], TTS = ma_Param[7];
		double TLACV = ma_Param[8], TRACV = ma_Param[9], BLACV = ma_Param[10], BRACV = ma_Param[11];
		double TotalTS = TS + TTS;
		double TotalACV = TLACV + TRACV + BLACV + BRACV;
		if (TH <= 0 && BH <= 0)
			calcType = 0;

		// Concrete
		if (calcType == (int)MSHandRailG::LowWallCalcType::All)
		{
			rVolume = (H - TotalTS)*W*L;
			if (TotalTS == 0)
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(B) * %g(H) * %g(L)"), W, H, L);
				else
					noteVolume.Format(_T("%g * %g * %g"), W, H, L);
			}
			else
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(B) * (%g(H) - %g(TS)) * %g(L)"), W, H, TotalTS, L);
				else
					noteVolume.Format(_T("%g * (%g - %g) * %g"), W, H, TotalTS, L);
			}
		}
		else if (calcType == (int)MSHandRailG::LowWallCalcType::Top)
		{
			rVolume = (TH - TTS)*W*L;
			if (TTS == 0)
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(B) * %g(H) * %g(L)"), W, TH, L);
				else
					noteVolume.Format(_T("%g * %g * %g"), W, TH, L);
			}
			else
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(B) * (%g(H) - %g(TS)) * %g(L)"), W, TH, TTS, L);
				else
					noteVolume.Format(_T("%g * (%g - %g) * %g"), W, TH, TTS, L);
			}
		}
		else if (calcType == (int)MSHandRailG::LowWallCalcType::Bottom)
		{
			if (BH - TS <= DTOL5)
			{
				rVolume = 0;
				noteVolume = _T("");
			}
			else
			{
				rVolume = (BH - TS)*W*L;
				if (TS == 0)
				{
					if (_bShowVariable)
						noteVolume.Format(_T("%g(B) * %g(H) * %g(L)"), W, BH, L);
					else
						noteVolume.Format(_T("%g * %g * %g"), W, BH, L);
				}
				else
				{
					if (_bShowVariable)
						noteVolume.Format(_T("%g(B) * (%g(H) - %g(TS)) * %g(L)"), W, BH, TS, L);
					else
						noteVolume.Format(_T("%g * (%g - %g) * %g"), W, BH, TS, L);
				}
			}
		}


		// Form Area (LR)
		if (calcType == (int)MSHandRailG::LowWallCalcType::All)
		{
			//rArea = (H - TotalTS)*L * 2;
			//if (TotalTS == 0)
			//	noteArea.Format(_T("%g * %g * 2"), H, L);
			//else
			//	noteArea.Format(_T("(%g - %g) * %g * 2"), H, TotalTS, L);
			
			if (mp_Owner->ma_UserFormWork.size() > 0)
			{
				map<CString, CString> dirToType;
				for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
					dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
				bool bLRSame = (dirToType.count(_T("L")) > 0 && dirToType.count(_T("L")) > 0 && dirToType[_T("L")] == dirToType[_T("R")]);
				if (bLRSame)
				{
					rArea = H * L * 2 - TotalACV;
					if (_bShowVariable)
						noteArea.Format(_T("%g(H) * %g(L) * 2"), H, L);
					else
						noteArea.Format(_T("%g * %g * 2"), H, L);

					CString tmp;
					if (TLACV > 0)
					{
						if (_bShowVariable)
							tmp.Format(_T(" - %g(TLACV)"), TLACV);
						else
							tmp.Format(_T(" - %g"), TLACV);

						noteArea += tmp;
					}

					if (TRACV > 0)
					{
						if (_bShowVariable)
							tmp.Format(_T(" - %g(TRACV)"), TRACV);
						else
							tmp.Format(_T(" - %g"), TRACV);

						noteArea += tmp;
					}

					if (BLACV > 0)
					{
						if (_bShowVariable)
							tmp.Format(_T(" - %g(BLACV)"), BLACV);
						else
							tmp.Format(_T(" - %g"), BLACV);

						noteArea += tmp;
					}

					if (BRACV > 0)
					{
						if (_bShowVariable)
							tmp.Format(_T(" - %g(BRACV)"), BRACV);
						else
							tmp.Format(_T(" - %g"), BRACV);

						noteArea += tmp;
					}

					//rArea = (H - TotalTS)*L * 2;
					//if (TotalTS == 0)
					//{
					//	if (_bShowVariable)
					//		noteArea.Format(_T("%g(H) * %g(L) * 2"), H, L);
					//	else
					//		noteArea.Format(_T("%g * %g * 2"), H, L);
					//}
					//else
					//{
					//	if (_bShowVariable)
					//		noteArea.Format(_T("(%g(H) - %g(TS)) * %g(L) * 2"), H, TotalTS, L);
					//	else
					//		noteArea.Format(_T("(%g - %g) * %g * 2"), H, TotalTS, L);
					//}
					result.formDir.push_back(_T("Side"));
					result.formWorkName.push_back(dirToType[_T("L")]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);

				}
				else
				{
					for (int i = 0; i < 2; ++i)
					{
						CString cur_dir = (i == 0 ? _T("L") : _T("R"));
						if (dirToType.count(cur_dir) > 0)
						{
							//rArea = (H - TotalTS) * L;
							//if (TotalTS == 0)
							//{
							//	if (_bShowVariable)
							//		noteArea.Format(_T("%g(H) * %g(L)"), H, L);
							//	else
							//		noteArea.Format(_T("%g * %g"), H, L);
							//}
							//else
							//{
							//	if (_bShowVariable)
							//		noteArea.Format(_T("(%g(H) - %g(TS)) * %g(L)"), H, TotalTS, L);
							//	else
							//		noteArea.Format(_T("(%g - %g) * %g"), H, TotalTS, L);
							//}

							rArea = H * L;
							if (_bShowVariable)
								noteArea.Format(_T("%g(H) * %g(L)"), H, L);
							else
								noteArea.Format(_T("%g * %g"), H, L);

							CString tmp;
							if (cur_dir == _T("L"))
							{
								rArea = H * L - TLACV - BLACV;

								if (TLACV > 0)
								{
									if (_bShowVariable)
										tmp.Format(_T(" - %g(TLACV)"), TLACV);
									else
										tmp.Format(_T(" - %g"), TLACV);

									noteArea += tmp;
								}

								if (BLACV > 0)
								{
									if (_bShowVariable)
										tmp.Format(_T(" - %g(BLACV)"), BLACV);
									else
										tmp.Format(_T(" - %g"), BLACV);

									noteArea += tmp;
								}

								result.formDir.push_back(_T("Left"));
							}
							else
							{
								rArea = H * L - TRACV - BRACV;

								if (TRACV > 0)
								{
									if (_bShowVariable)
										tmp.Format(_T(" - %g(TRACV)"), TRACV);
									else
										tmp.Format(_T(" - %g"), TRACV);

									noteArea += tmp;
								}

								if (BRACV > 0)
								{
									if (_bShowVariable)
										tmp.Format(_T(" - %g(BRACV)"), BRACV);
									else
										tmp.Format(_T(" - %g"), BRACV);

									noteArea += tmp;
								}

								result.formDir.push_back(_T("Right"));
							}

							//if (cur_dir == _T("L"))
							//	result.formDir.push_back(_T("Left"));
							//else
							//	result.formDir.push_back(_T("Right"));

							result.formWorkName.push_back(dirToType[cur_dir]);
							result.formArea.push_back(rArea);
							result.formNote.push_back(noteArea);
						}
					}
				}
			}

			if (BH - TS <= DTOL5)
			{
				// 하단부가 완전히 잘려서 없는 경우
			}
			else
			{
				//바닥거푸집
				rArea = W * L;
				noteArea.Format(_T("%g(B) * %g(L)"), W, L);


				if (mp_Owner->ma_UserFormWork.size() > 0)
				{
					map<CString, CString> dirToType;
					map<CString, CString>::iterator it;
					for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
						dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;

					it = dirToType.find(_T("T"));
					if (it != dirToType.end())//Top Exist
					{
						if (_bShowVariable)
							noteArea.Format(_T("%g(B) * %g(L)"), W, L);
						else
							noteArea.Format(_T("%g * %g"), W, L);

						result.formDir.push_back(_T("Top"));
						result.formWorkName.push_back(dirToType[_T("T")]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);
					}

					it = dirToType.find(_T("B"));
					if (it != dirToType.end()) //Bot Exist
					{
						rArea = W * L;
						if (_bShowVariable)
							noteArea.Format(_T("%g(B) * %g(L)"), W, L);
						else
							noteArea.Format(_T("%g * %g"), W, L);

						result.formDir.push_back(_T("Bot"));
						result.formWorkName.push_back(dirToType[_T("B")]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);

					}
				}
			}
		}
		else if (calcType == (int)MSHandRailG::LowWallCalcType::Top)
		{
			if (TH - TTS <= DTOL5)
			{
				// 상단부가 완전히 잘려서 없는 경우
			}
			else
			{
				//rArea = (TH - TTS)*L * 2;
				//if (TTS == 0)
				//	noteArea.Format(_T("%g * %g * 2"), TH, L);
				//else
				//	noteArea.Format(_T("(%g - %g) * %g * 2"), TH, TTS, L);

				if (mp_Owner->ma_UserFormWork.size() > 0)
				{
					map<CString, CString> dirToType;
					for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
						dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
					bool bLRSame = (dirToType.count(_T("L")) > 0 && dirToType.count(_T("L")) > 0 && dirToType[_T("L")] == dirToType[_T("R")]);
					if (bLRSame)
					{
						//rArea = (TH - TTS)*L * 2;
						//if (TTS == 0)
						//{
						//	if (_bShowVariable)
						//		noteArea.Format(_T("%g(H) * %g(L) * 2"), TH, L);
						//	else
						//		noteArea.Format(_T("%g * %g * 2"), TH, L);
						//}
						//else
						//{
						//	if (_bShowVariable)
						//		noteArea.Format(_T("(%g(H) - %g(TS)) * %g(L) * 2"), TH, TTS, L);
						//	else
						//		noteArea.Format(_T("(%g - %g) * %g * 2"), TH, TTS, L);
						//}

						rArea = TH * L * 2 - TLACV - TRACV;

						if (_bShowVariable)
							noteArea.Format(_T("%g(H) * %g(L) * 2"), TH, L);
						else
							noteArea.Format(_T("%g * %g * 2"), TH, L);

						CString tmp;
						if (TLACV > 0)
						{
							if (_bShowVariable)
								tmp.Format(_T(" - %g(TLACV)"), TLACV);
							else
								tmp.Format(_T(" - %g"), TLACV);

							noteArea += tmp;
						}

						if (TRACV > 0)
						{
							if (_bShowVariable)
								tmp.Format(_T(" - %g(TRACV)"), TRACV);
							else
								tmp.Format(_T(" - %g"), TRACV);

							noteArea += tmp;
						}

						result.formDir.push_back(_T("Side"));
						result.formWorkName.push_back(dirToType[_T("L")]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);

					}
					else
					{
						for (int i = 0; i < 2; ++i)
						{
							CString cur_dir = (i == 0 ? _T("L") : _T("R"));
							if (dirToType.count(cur_dir) > 0)
							{
								//rArea = (TH - TTS)*L;
								//if (TTS == 0)
								//{
								//	if (_bShowVariable)
								//		noteArea.Format(_T("%g(TH) * %g(L)"), TH, L);
								//	else
								//		noteArea.Format(_T("%g * %g"), TH, L);
								//}
								//else
								//{
								//	if (_bShowVariable)
								//		noteArea.Format(_T("(%g(TH) - %g(TS)) * %g(L)"), TH, TTS, L);
								//	else
								//		noteArea.Format(_T("(%g - %g) * %g"), TH, TTS, L);
								//}
								
								if (_bShowVariable)
									noteArea.Format(_T("%g(H) * %g(L)"), TH, L);
								else
									noteArea.Format(_T("%g * %g"), TH, L);

								CString tmp;
								if (cur_dir == _T("L"))
								{
									rArea = TH * L - TLACV;

									if (TLACV > 0)
									{
										if (_bShowVariable)
											tmp.Format(_T(" - %g(TLACV)"), TLACV);
										else
											tmp.Format(_T(" - %g"), TLACV);

										noteArea += tmp;
									}

									result.formDir.push_back(_T("Left"));
								}
								else
								{
									rArea = TH * L - TRACV;

									if (TRACV > 0)
									{
										if (_bShowVariable)
											tmp.Format(_T(" - %g(TRACV)"), TRACV);
										else
											tmp.Format(_T(" - %g"), TRACV);

										noteArea += tmp;
									}

									result.formDir.push_back(_T("Right"));
								}

								if (cur_dir == _T("L"))
									result.formDir.push_back(_T("Left"));
								else
									result.formDir.push_back(_T("Right"));
								result.formWorkName.push_back(dirToType[cur_dir]);
								result.formArea.push_back(rArea);
								result.formNote.push_back(noteArea);
							}
						}
					}
				}
				//Top,Bot 
				rArea = W * L;
				noteArea.Format(_T("%g(B) * %g(L)"), W, L);


				if (mp_Owner->ma_UserFormWork.size() > 0)
				{
					map<CString, CString> dirToType;
					map<CString, CString>::iterator it;
					for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
						dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;

					it = dirToType.find(_T("T"));
					if (it != dirToType.end())//Top Exist
					{
						if (_bShowVariable)
							noteArea.Format(_T("%g(B) * %g(L)"), W, L);
						else
							noteArea.Format(_T("%g * %g"), W, L);

						result.formDir.push_back(_T("Top"));
						result.formWorkName.push_back(dirToType[_T("T")]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);
					}
					/* [BH-6023] 난간벽 상부 거푸집 바닥면 중복산출 삭제
					if (BH != 0)
					{
						it = dirToType.find(_T("B"));
						if (it != dirToType.end())//Bot Exist
						{
							if (_bShowVariable)
								noteArea.Format(_T("%g(B) * %g(L)"), W, L);
							else
								noteArea.Format(_T("%g * %g"), W, L);

							result.formDir.push_back(_T("Bot"));
							result.formWorkName.push_back(dirToType[_T("B")]);
							result.formArea.push_back(rArea);
							result.formNote.push_back(noteArea);
						}
					}
					*/
				}
			}
		}
		else if (calcType == (int)MSHandRailG::LowWallCalcType::Bottom)
		{
			if (BH - TS <= DTOL5)
			{
				// 하단부가 완전히 잘려서 없는 경우
			}
			else
			{
				//rArea = (BH - TS)*L * 2;
				//if (TS == 0)
				//	noteArea.Format(_T("%g * %g * 2"), BH, L);
				//else
				//	noteArea.Format(_T("(%g - %g) * %g * 2"), BH, TS, L);

				if (mp_Owner->ma_UserFormWork.size() > 0)
				{
					map<CString, CString> dirToType;
					for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
						dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
					bool bLRSame = (dirToType.count(_T("L")) > 0 && dirToType.count(_T("L")) > 0 && dirToType[_T("L")] == dirToType[_T("R")]);
					if (bLRSame)
					{
						//rArea = (BH - TS)*L * 2;
						//if (TS == 0)
						//{
						//	if (_bShowVariable)
						//		noteArea.Format(_T("%g(H) * %g(L) * 2"), BH, L);
						//	else
						//		noteArea.Format(_T("%g * %g * 2"), BH, L);
						//}
						//else
						//{
						//	if (_bShowVariable)
						//		noteArea.Format(_T("(%g(H) - %g(TS)) * %g(L) * 2"), BH, TS, L);
						//	else
						//		noteArea.Format(_T("(%g - %g) * %g * 2"), BH, TS, L);
						//}

						rArea = BH * L * 2 - BLACV - BRACV;

						if (_bShowVariable)
							noteArea.Format(_T("%g(H) * %g(L) * 2"), BH, L);
						else
							noteArea.Format(_T("%g * %g * 2"), BH, L);

						CString tmp;
						if (BLACV > 0)
						{
							if (_bShowVariable)
								tmp.Format(_T(" - %g(BLACV)"), BLACV);
							else
								tmp.Format(_T(" - %g"), BLACV);

							noteArea += tmp;
						}

						if (BRACV > 0)
						{
							if (_bShowVariable)
								tmp.Format(_T(" - %g(BRACV)"), BRACV);
							else
								tmp.Format(_T(" - %g"), BRACV);

							noteArea += tmp;
						}

						result.formDir.push_back(_T("Side"));
						result.formWorkName.push_back(dirToType[_T("L")]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);

					}
					else
					{
						for (int i = 0; i < 2; ++i)
						{
							CString cur_dir = (i == 0 ? _T("L") : _T("R"));
							if (dirToType.count(cur_dir) > 0)
							{
								//rArea = (BH - TS)*L;
								//if (TS == 0)
								//{
								//	if (_bShowVariable)
								//		noteArea.Format(_T("%g(BH) * %g(L)"), BH, L);
								//	else
								//		noteArea.Format(_T("%g * %g"), BH, L);
								//}
								//else
								//{
								//	if (_bShowVariable)
								//		noteArea.Format(_T("(%g(BH) - %g(TS)) * %g(L)"), BH, TS, L);
								//	else
								//		noteArea.Format(_T("(%g - %g) * %g"), BH, TS, L);
								//}

								if (_bShowVariable)
									noteArea.Format(_T("%g(H) * %g(L)"), BH, L);
								else
									noteArea.Format(_T("%g * %g"), BH, L);

								CString tmp;
								if (cur_dir == _T("L"))
								{
									rArea = BH * L - BLACV;

									if (BLACV > 0)
									{
										if (_bShowVariable)
											tmp.Format(_T(" - %g(BLACV)"), BLACV);
										else
											tmp.Format(_T(" - %g"), BLACV);

										noteArea += tmp;
									}

									result.formDir.push_back(_T("Left"));
								}
								else
								{
									rArea = BH * L - BRACV;

									if (BLACV > 0)
									{
										if (_bShowVariable)
											tmp.Format(_T(" - %g(BRACV)"), BRACV);
										else
											tmp.Format(_T(" - %g"), BRACV);

										noteArea += tmp;
									}

									result.formDir.push_back(_T("Right"));
								}

								if (cur_dir == _T("L"))
									result.formDir.push_back(_T("Left"));
								else
									result.formDir.push_back(_T("Right"));
								result.formWorkName.push_back(dirToType[cur_dir]);
								result.formArea.push_back(rArea);
								result.formNote.push_back(noteArea);
							}
						}
					}
				}

				//바닥거푸집
				rArea = W * L;
				noteArea.Format(_T("%g(B) * %g(L)"), W, L);


				if (mp_Owner->ma_UserFormWork.size() > 0)
				{
					map<CString, CString> dirToType;
					map<CString, CString>::iterator it;
					for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
						dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;

					it = dirToType.find(_T("B"));
					if (it != dirToType.end()) //Bot Exist
					{
						rArea = W * L;
						if (_bShowVariable)
							noteArea.Format(_T("%g(B) * %g(L)"), W, L);
						else
							noteArea.Format(_T("%g * %g"), W, L);

						result.formDir.push_back(_T("Bot"));
						result.formWorkName.push_back(dirToType[_T("B")]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);
					}
					// [BH-6023] 난간벽 상부 거푸집 바닥면 중복산출 삭제 , 아예 Top이 존재하지 않는 경우에도 거푸집은 추가해주자.
					/*
					if (TH == 0)
					{
						it = dirToType.find(_T("T"));
						if (it != dirToType.end()) //Top Exist
						{
							rArea = W*L;
							if (_bShowVariable)
								noteArea.Format(_T("%g(B) * %g(L)"), W, L);
							else
								noteArea.Format(_T("%g * %g"), W, L);

							result.formDir.push_back(_T("Tnop"));
							result.formWorkName.push_back(dirToType[_T("T")]);
							result.formArea.push_back(rArea);
							result.formNote.push_back(noteArea);
						}
					}
					*/
				}
			}
		}

	}
}

void VolumeAreaInfoEMS::GetResultByCZoneSlab(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid)
{
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msSlabG && !mp_Owner->IsMatSlab())
	{
		MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_Owner);
		if (slab == NULL) return;

		bool isDeck = mp_Owner->IsDeckSlab();
		bool isHaunchOutstretched = false;
		MSRebarBuilding* bldg = dynamic_cast<MSRebarBuilding*>(mp_Owner->GetBuilding());
		if(bldg != nullptr)
			isHaunchOutstretched = bldg->GetQuantityOption()->m_dHaunchOutstretchedLength > 0;

		int haunchCount = slab->ma_Haunches.size();
		int haunchStretchedCnt = 0;
		if (isHaunchOutstretched)
			haunchStretchedCnt = haunchCount;

		int edgeWallCount = slab->ma_EdgeWall.size();
		int edgeWallHaunchStretchedCnt = 0;

		int edgeWallHaunchCount = 0;
		if (edgeWallCount > 0)
		{
			for (int i = 0; i < edgeWallCount; i++)
			{
				if (slab->ma_EdgeWall[i]->GetLevelDifHasHaunch())
				{
					edgeWallHaunchCount++;
					if (isHaunchOutstretched)
						edgeWallHaunchStretchedCnt++;
				}
			}
		}

		if (slab->IsRebarEmbeddedDeckSlab())
		{
			if (ma_Param.size() != 19 + haunchCount + haunchStretchedCnt + edgeWallCount + edgeWallHaunchCount + edgeWallHaunchStretchedCnt) return;
		}
		else if (slab->IsHalfPCSlab())
		{
			if (ma_Param.size() != 19 + haunchCount + haunchStretchedCnt + edgeWallCount + edgeWallHaunchCount + edgeWallHaunchStretchedCnt) return;
		}
		else
		{
			if (ma_Param.size() != 17 + haunchCount + haunchStretchedCnt + edgeWallCount + edgeWallHaunchCount + edgeWallHaunchStretchedCnt) return;
		}

		double A = ma_Param[0], OL = ma_Param[1], T = ma_Param[2], IA = ma_Param[3], IL = ma_Param[4];
		double ACV = ma_Param[5], V2 = ma_Param[6], A2 = ma_Param[7], A2_END = ma_Param[8], IAC = ma_Param[9], HUN = ma_Param[10];
		double VTR = ma_Param[11], VWL = ma_Param[12], HUNV = ma_Param[13];
		double ACV_OUT = 0, ACV_IN = 0, V_INTERSECT = 0;
		ACV_OUT = ma_Param[14], ACV_IN = ma_Param[15], V_INTERSECT = ma_Param[16];

		double SX = 0, SY = 0;
		if (slab->IsRebarEmbeddedDeckSlab())
		{
			SX = ma_Param[14];
			SY = ma_Param[15];
			ACV_OUT = ma_Param[16];
			ACV_IN = ma_Param[17];
			V_INTERSECT = ma_Param[18];
		}

		// 기본적으로 T2와 T는 동일하다.
		double T1 = 0, T2 = 0;
		if (slab->IsHalfPCSlab())
		{
			T1 = ma_Param[3], T2 = ma_Param[4], IA = ma_Param[5], IL = ma_Param[6];
			ACV = ma_Param[7], V2 = ma_Param[8], A2 = ma_Param[9], A2_END = ma_Param[10], IAC = ma_Param[11], HUN = ma_Param[12];
			VTR = ma_Param[13], VWL = ma_Param[14], HUNV = ma_Param[15];
			ACV_OUT = ma_Param[16], ACV_IN = ma_Param[17], V_INTERSECT = ma_Param[18];
		}

		// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
		vector<double> haunchParam;
		for (int i = 0; i < haunchCount; i++)
		{
			double volume = 0;
			if (slab->IsRebarEmbeddedDeckSlab() || slab->IsHalfPCSlab())
				volume = ma_Param[19 + i];
			else
				volume = ma_Param[17 + i];

			haunchParam.push_back(volume);
		}

		vector<double> haunchStretchedParam;
		for (int i = 0; i < haunchStretchedCnt; i++)
		{
			double volume = 0;
			if (slab->IsRebarEmbeddedDeckSlab() || slab->IsHalfPCSlab())
				volume = ma_Param[19 + haunchCount + edgeWallCount + edgeWallHaunchCount + i];
			else
				volume = ma_Param[17 + haunchCount + edgeWallCount + edgeWallHaunchCount + i];

			haunchStretchedParam.push_back(volume);
		}

		vector<double> edgeWallParam;
		for (int i = 0; i < edgeWallCount + edgeWallHaunchCount; i++)
		{
			double volume = 0;
			if (slab->IsRebarEmbeddedDeckSlab() || slab->IsHalfPCSlab())
				volume = ma_Param[19 + haunchCount + i];
			else
				volume = ma_Param[17 + haunchCount + i];

			edgeWallParam.push_back(volume);
		}

		vector<double> edgeWallHaunchStretchedParam;
		for (int i = 0; i < edgeWallHaunchStretchedCnt; i++)
		{
			double volume = 0;
			if (slab->IsRebarEmbeddedDeckSlab() || slab->IsHalfPCSlab())
				volume = ma_Param[19 + haunchCount + edgeWallCount + edgeWallHaunchCount + haunchStretchedCnt + + i];
			else
				volume = ma_Param[17 + haunchCount + edgeWallCount + edgeWallHaunchCount + haunchStretchedCnt + + i];

			edgeWallHaunchStretchedParam.push_back(volume);
		}

		//tolerance
		if (OL < 0.1) OL = 0;
		if (IL < 0.1) IL = 0;
		// Concrete
		//if (IAC == 0.0) IAC = IA;

		double Factor = 0.0;
		double dLibLathArea = 0.0;
		if (cz_id != -1)
		{
			for (int i = 0; i < ma_CZoneID.size(); ++i)
			{
				if (cz_id == ma_CZoneID[i])
				{
					Factor = ma_Factor[i];
					dLibLathArea = ma_LibLathArea[i];
				}
			}
		}
		else
		{
			Factor = 1.0;
			dLibLathArea = 1.0;
		}
		if (Factor == 0.0) return;

		CString czoneName;
		CString czoneNote;
		if (Factor != 1.0)
		{
			MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
			MSConstructionZone* czone = curFloor->GetCZone(cz_id);
			czoneName = czone->GetName();
		}

		if (calcType == 0)	// 전체 (기존로직)
		{
			rVolume = (A - IAC) * T - V_INTERSECT;

			if (IAC > 0)
			{
				if (_bShowVariable)
					noteVolume.Format(_T("(%g(A) - %g(IA)) * %g(T)"), A, IAC, T);
				else
					noteVolume.Format(_T("(%g - %g) * %g"), A, IAC, T);
			}
			else
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(A) * %g(T)"), A, T);
				else
					noteVolume.Format(_T("%g * %g"), A, T);
			}

			if (V_INTERSECT > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" - %g(V_INT)"), V_INTERSECT);
				else
					tmp.Format(_T(" - %g"), V_INTERSECT);
				noteVolume += tmp;
			}

			if (VTR > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" - %g(VTR)"), VTR);
				else
					tmp.Format(_T(" - %g"), VTR);
				noteVolume += tmp;
				rVolume -= VTR;
			}

			if (VWL > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" - %g(VWL)"), VWL);
				else
					tmp.Format(_T(" - %g"), VWL);
				noteVolume += tmp;
				rVolume -= VWL;
			}

			if (Factor != 1.0)
			{
				rVolume *= Factor;
				czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteVolume);
				noteVolume = czoneNote;
			}

			//if (HUNV > 0)
			//{
			//	CString tmp;
			//	if (_bShowVariable)
			//		tmp.Format(_T(" + %g(HAUNCH)"), HUNV);
			//	else
			//		tmp.Format(_T(" + %g"), HUNV);
			//	noteVolume += tmp;
			//	rVolume += HUNV;
			//}

			// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
			if (haunchParam.size() > 0)
			{
				for (int hi = 0; hi < haunchCount; hi++)
				{
					MSSubHaunchG* haunch = dynamic_cast<MSSubHaunchG*>(slab->ma_Haunches[hi]);
					vector<int> haunchCZoneIDs = haunch->m_VolumeAreaEMS.ma_CZoneID;
					int hannchmatid = haunch->GetHaunchMaterialID();
					if (hannchmatid == 0)
						hannchmatid = slab->GetMaterial()->m_ID;
					double haunchFactor = 0.0;
					for (int i = 0; i < haunchCZoneIDs.size(); ++i)
					{
						if (cz_id == haunchCZoneIDs[i])
							haunchFactor = haunch->m_VolumeAreaEMS.ma_Factor[i];
					}

					double haunchVolume = haunchParam[hi];

					CString str;
					if (fabs(haunchFactor - 1) < DTOL_GM || isDeck)
					{
						if (_bShowVariable)
							str.Format(_T("%g(HAUNCH: %d)"), haunchVolume, haunch->GetID());
						else
							str.Format(_T("%g"), haunchVolume);

						haunchFactor = 1.0;
					}
					else
					{
						if (haunchFactor > 0.0)
						{
							MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
							if (curFloor != NULL)
							{
								MSConstructionZone* czone = curFloor->GetCZone(cz_id);
								if (_bShowVariable)
									str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, haunchVolume, haunch->GetID());
								else
									str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, haunchVolume);
							}
						}
					}

					//noteVolume += str;
					//rVolume += haunchVolume * haunchFactor;

					if (haunchFactor == 0.0)
						str.Format(_T("NoSumVolume"));

					subIds.push_back(haunch->GetID());
					subVolumes.push_back(haunchVolume * haunchFactor);
					subNotes.push_back(str);
					matid.push_back(hannchmatid);

					if (isHaunchOutstretched)
					{
						double curVolume = haunchStretchedParam[hi];
						if (fabs(haunchFactor - 1) < DTOL_GM || isDeck)
						{
							if (_bShowVariable)
								str.Format(_T("%g(HAUNCH: %d)"), curVolume, haunch->GetID());
							else
								str.Format(_T("%g"), curVolume);

							haunchFactor = 1.0;
						}
						else
						{
							if (haunchFactor > 0.0)
							{
								MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
								if (curFloor != NULL)
								{
									MSConstructionZone* czone = curFloor->GetCZone(cz_id);
									if (_bShowVariable)
										str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, curVolume, haunch->GetID());
									else
										str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, curVolume);
								}
							}
						}

						subIds.push_back(haunch->GetID());
						subVolumes.push_back(curVolume * haunchFactor);
						subNotes.push_back(str);
						matid.push_back(hannchmatid);
					}
				}
			}

			//if (V2 > 0)
			//{
			//	CString tmp;
			//	if (_bShowVariable)
			//		tmp.Format(_T(" + %g(V2)"), V2);
			//	else
			//		tmp.Format(_T(" + %g"), V2);
			//	noteVolume += tmp;
			//}

			// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
			if (edgeWallParam.size() > 0)
			{
				int edgeWallParamIndex = 0;
				int edgeWallHaunchStretchedParamIndex = 0;
				for (int hi = 0; hi < edgeWallCount; hi++)
				{
					MSLineLevelDifG* edgeWall = dynamic_cast<MSLineLevelDifG*>(slab->ma_EdgeWall[hi]);
					vector<int> edgeWallCZoneIDs = edgeWall->m_VolumeAreaEMS.ma_CZoneID;

					double edgeWallFactor = 0.0;
					for (int i = 0; i < edgeWallCZoneIDs.size(); ++i)
					{
						if (cz_id == edgeWallCZoneIDs[i])
							edgeWallFactor = edgeWall->m_VolumeAreaEMS.ma_Factor[i];
					}

					if (edgeWall->GetLevelDifHasHaunch())
					{
						double edgeWallVolume = edgeWallParam[edgeWallParamIndex++];

						CString str;
						if (fabs(edgeWallFactor - 1) < DTOL_GM || isDeck)
						{
							if (_bShowVariable)
								str.Format(_T("%g(HAUNCH: %d)"), edgeWallVolume, edgeWall->GetID());
							else
								str.Format(_T("%g"), edgeWallVolume);

							edgeWallFactor = 1.0;
						}
						else
						{
							if (edgeWallFactor > 0.0)
							{
								MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
								if (curFloor != NULL)
								{
									MSConstructionZone* czone = curFloor->GetCZone(cz_id);
									if (_bShowVariable)
										str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), edgeWallFactor, edgeWallVolume, edgeWall->GetID());
									else
										str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, edgeWallVolume);
								}
							}
						}

						//noteVolume += str;
						//rVolume += edgeWallVolume * edgeWallFactor;
						if (edgeWallFactor == 0.0)
							str.Format(_T("NoSumVolume"));

						if (edgeWallVolume > 0.0)
						{
							subIds.push_back(edgeWall->GetID());
							subVolumes.push_back(edgeWallVolume * edgeWallFactor);
							subNotes.push_back(str);
							matid.push_back(edgeWall->GetLevelDifHaunchMaterialID());
						}

						if (isHaunchOutstretched)
						{
							double curVolume = edgeWallHaunchStretchedParam[edgeWallHaunchStretchedParamIndex++];
							if (fabs(edgeWallFactor - 1) < DTOL_GM || isDeck)
							{
								if (_bShowVariable)
									str.Format(_T("%g(HAUNCH: %d)"), curVolume, edgeWall->GetID());
								else
									str.Format(_T("%g"), curVolume);

								edgeWallFactor = 1.0;
							}
							else
							{
								if (edgeWallFactor > 0.0)
								{
									MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
									if (curFloor != NULL)
									{
										MSConstructionZone* czone = curFloor->GetCZone(cz_id);
										if (_bShowVariable)
											str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), edgeWallFactor, curVolume, edgeWall->GetID());
										else
											str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, curVolume);
									}
								}
							}

							subIds.push_back(edgeWall->GetID());
							subVolumes.push_back(curVolume * edgeWallFactor);
							subNotes.push_back(str);
							matid.push_back(edgeWall->GetLevelDifHaunchMaterialID());
						}
					}

					double edgeWallVolume = edgeWallParam[hi];

					CString str;
					if (fabs(edgeWallFactor - 1) < DTOL_GM || isDeck)
					{
						if (_bShowVariable)
							str.Format(_T("%g(V2: %d)"), edgeWallVolume, edgeWall->GetID());
						else
							str.Format(_T("%g"), edgeWallVolume);
						edgeWallFactor = 1.0;
					}
					else
					{
						if (edgeWallFactor > 0.0)
						{
							MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
							if (curFloor != NULL)
							{
								MSConstructionZone* czone = curFloor->GetCZone(cz_id);
								if (_bShowVariable)
									str.Format(_T("[%s : %g] * %g(V2: %d)"), czone->GetName(), edgeWallFactor, edgeWallVolume, edgeWall->GetID());
								else
									str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, edgeWallVolume);
							}
						}
					}

					//noteVolume += str;
					//rVolume += edgeWallVolume * edgeWallFactor;

					if (edgeWallFactor == 0.0)
						str.Format(_T("NoSumVolume"));

					if (edgeWallVolume > 0.0)
					{
						subIds.push_back(edgeWall->GetID());
						subVolumes.push_back(edgeWallVolume * edgeWallFactor);
						subNotes.push_back(str);
						matid.push_back(slab->GetMaterial()->m_ID);
					}
				}
			}			
		}
		else if (calcType == 1)	// Half PC Slab의 RC부분
		{
			rVolume = (A - IAC) * T2 - V_INTERSECT;

			if (IAC > 0)
			{
				if (_bShowVariable)
					noteVolume.Format(_T("(%g(A) - %g(IA)) * %g(T2)"), A, IAC, T2);
				else
					noteVolume.Format(_T("(%g - %g) * %g"), A, IAC, T2);
			}
			else
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(A) * %g(T2)"), A, T2);
				else
					noteVolume.Format(_T("%g * %g"), A, T2);
			}

			if (V_INTERSECT > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" - %g(V_INT)"), V_INTERSECT);
				else
					tmp.Format(_T(" - %g"), V_INTERSECT);
				noteVolume += tmp;
			}

			if (VTR > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" - %g(VTR)"), VTR);
				else
					tmp.Format(_T(" - %g"), VTR);
				noteVolume += tmp;
				rVolume -= VTR;
			}

			if (VWL > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" - %g(VWL)"), VWL);
				else
					tmp.Format(_T(" - %g"), VWL);
				noteVolume += tmp;
				rVolume -= VWL;
			}

			if (Factor != 1.0)
			{
				rVolume *= Factor;
				czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteVolume);
				noteVolume = czoneNote;
			}

			//if (HUNV > 0)
			//{
			//	CString tmp;
			//	if (_bShowVariable)
			//		tmp.Format(_T(" + %g(HAUNCH)"), HUNV);
			//	else
			//		tmp.Format(_T(" + %g"), HUNV);
			//	noteVolume += tmp;
			//	rVolume += HUNV;
			//}

			// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
			if (haunchParam.size() > 0)
			{
				for (int hi = 0; hi < haunchCount; hi++)
				{
					MSSubHaunchG* haunch = dynamic_cast<MSSubHaunchG*>(slab->ma_Haunches[hi]);
					vector<int> haunchCZoneIDs = haunch->m_VolumeAreaEMS.ma_CZoneID;
					int hannchmatid = haunch->GetHaunchMaterialID();
					if (hannchmatid == 0)
						hannchmatid = slab->GetMaterial()->m_ID;
					double haunchFactor = 0.0;
					for (int i = 0; i < haunchCZoneIDs.size(); ++i)
					{
						if (cz_id == haunchCZoneIDs[i])
							haunchFactor = haunch->m_VolumeAreaEMS.ma_Factor[i];
					}

					double haunchVolume = haunchParam[hi];

					CString str;
					str.Format(_T("%g(HAUNCH: %d)"), haunchVolume, haunch->GetID());
					//if (fabs(haunchFactor - 1) < DTOL_GM)
					//{
					//	if (_bShowVariable)
					//		str.Format(_T("%g(HAUNCH: %d)"), haunchVolume, haunch->GetID());
					//	else
					//		str.Format(_T("%g"), haunchVolume);

					//	haunchFactor = 1.0;
					//}
					//else
					//{
					//	if (haunchFactor > 0.0)
					//	{
					//		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
					//		if (curFloor != NULL)
					//		{
					//			MSConstructionZone* czone = curFloor->GetCZone(cz_id);
					//			if (_bShowVariable)
					//				str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, haunchVolume, haunch->GetID());
					//			else
					//				str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, haunchVolume);
					//		}
					//	}
					//}

					//noteVolume += str;
					//rVolume += haunchVolume * haunchFactor;

					if (haunchFactor == 0.0)
						str.Format(_T("NoSumVolume"));

					subIds.push_back(haunch->GetID());
					subVolumes.push_back(haunchVolume * haunchFactor);
					subNotes.push_back(str);
					matid.push_back(hannchmatid);

					if (isHaunchOutstretched)
					{
						double curVolume = haunchStretchedParam[hi];
						str.Format(_T("%g(HAUNCH: %d)"), curVolume, haunch->GetID());

						//if (fabs(haunchFactor - 1) < DTOL_GM)
						//{
						//	if (_bShowVariable)
						//		str.Format(_T("%g(HAUNCH: %d)"), curVolume, haunch->GetID());
						//	else
						//		str.Format(_T("%g"), curVolume);

						//	haunchFactor = 1.0;
						//}
						//else
						//{
						//	if (haunchFactor > 0.0)
						//	{
						//		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
						//		if (curFloor != NULL)
						//		{
						//			MSConstructionZone* czone = curFloor->GetCZone(cz_id);
						//			if (_bShowVariable)
						//				str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, curVolume, haunch->GetID());
						//			else
						//				str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, curVolume);
						//		}
						//	}
						//}

						subIds.push_back(haunch->GetID());
						subVolumes.push_back(curVolume * haunchFactor);
						subNotes.push_back(str);
						matid.push_back(hannchmatid);
					}
				}
			}

			//if (V2 > 0)
			//{
			//	CString tmp;
			//	if (_bShowVariable)
			//		tmp.Format(_T(" + %g(V2)"), V2);
			//	else
			//		tmp.Format(_T(" + %g"), V2);
			//	noteVolume += tmp;
			//}

			// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
			if (edgeWallParam.size() > 0)
			{
				int edgeWallParamIndex = 0;
				int edgeWallHaunchStretchedParamIndex = 0;
				for (int hi = 0; hi < edgeWallCount; hi++)
				{
					MSLineLevelDifG* edgeWall = dynamic_cast<MSLineLevelDifG*>(slab->ma_EdgeWall[hi]);
					vector<int> edgeWallCZoneIDs = edgeWall->m_VolumeAreaEMS.ma_CZoneID;

					double edgeWallFactor = 0.0;
					for (int i = 0; i < edgeWallCZoneIDs.size(); ++i)
					{
						if (cz_id == edgeWallCZoneIDs[i])
							edgeWallFactor = edgeWall->m_VolumeAreaEMS.ma_Factor[i];
					}

					if (edgeWall->GetLevelDifHasHaunch())
					{
						double edgeWallVolume = edgeWallParam[edgeWallParamIndex++];

						CString str;
						str.Format(_T("%g(HAUNCH: %d)"), edgeWallVolume, edgeWall->GetID());

						//if (fabs(edgeWallFactor - 1) < DTOL_GM)
						//{
						//	if (_bShowVariable)
						//		str.Format(_T("%g(HAUNCH: %d)"), edgeWallVolume, edgeWall->GetID());
						//	else
						//		str.Format(_T("%g"), edgeWallVolume);

						//	edgeWallFactor = 1.0;
						//}
						//else
						//{
						//	if (edgeWallFactor > 0.0)
						//	{
						//		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
						//		if (curFloor != NULL)
						//		{
						//			MSConstructionZone* czone = curFloor->GetCZone(cz_id);
						//			if (_bShowVariable)
						//				str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), edgeWallFactor, edgeWallVolume, edgeWall->GetID());
						//			else
						//				str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, edgeWallVolume);
						//		}
						//	}
						//}

						//noteVolume += str;
						//rVolume += edgeWallVolume * edgeWallFactor;
						if (edgeWallFactor == 0.0)
							str.Format(_T("NoSumVolume"));

						if (edgeWallVolume > 0.0)
						{
							subIds.push_back(edgeWall->GetID());
							subVolumes.push_back(edgeWallVolume * edgeWallFactor);
							subNotes.push_back(str);
							matid.push_back(edgeWall->GetLevelDifHaunchMaterialID());
						}

						if (isHaunchOutstretched)
						{
							double curVolume = edgeWallHaunchStretchedParam[edgeWallHaunchStretchedParamIndex++];
							str.Format(_T("%g(HAUNCH: %d)"), curVolume, edgeWall->GetID());

							//if (fabs(edgeWallFactor - 1) < DTOL_GM)
							//{
							//	if (_bShowVariable)
							//		str.Format(_T("%g(HAUNCH: %d)"), curVolume, edgeWall->GetID());
							//	else
							//		str.Format(_T("%g"), curVolume);

							//	edgeWallFactor = 1.0;
							//}
							//else
							//{
							//	if (edgeWallFactor > 0.0)
							//	{
							//		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
							//		if (curFloor != NULL)
							//		{
							//			MSConstructionZone* czone = curFloor->GetCZone(cz_id);
							//			if (_bShowVariable)
							//				str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), edgeWallFactor, curVolume, edgeWall->GetID());
							//			else
							//				str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, curVolume);
							//		}
							//	}
							//}

							subIds.push_back(edgeWall->GetID());
							subVolumes.push_back(curVolume * edgeWallFactor);
							subNotes.push_back(str);
							matid.push_back(edgeWall->GetLevelDifHaunchMaterialID());
						}
					}

					double edgeWallVolume = edgeWallParam[hi];

					CString str;
					str.Format(_T("%g(V2: %d)"), edgeWallVolume, edgeWall->GetID());

					//if (fabs(edgeWallFactor - 1) < DTOL_GM)
					//{						
					//	if (_bShowVariable)
					//		str.Format(_T("%g(V2: %d)"), edgeWallVolume, edgeWall->GetID());
					//	else
					//		str.Format(_T("%g"), edgeWallVolume);						
					//	edgeWallFactor = 1.0;
					//}
					//else
					//{
					//	if (edgeWallFactor > 0.0)
					//	{
					//		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
					//		if (curFloor != NULL)
					//		{
					//			MSConstructionZone* czone = curFloor->GetCZone(cz_id);								
					//			if (_bShowVariable)
					//				str.Format(_T("[%s : %g] * %g(V2: %d)"), czone->GetName(), edgeWallFactor, edgeWallVolume, edgeWall->GetID());
					//			else
					//				str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, edgeWallVolume);								
					//		}
					//	}
					//}

					//noteVolume += str;
					//rVolume += edgeWallVolume * edgeWallFactor;

					if (edgeWallFactor == 0.0)
						str.Format(_T("NoSumVolume"));

					if (edgeWallVolume > 0.0)
					{
						subIds.push_back(edgeWall->GetID());
						subVolumes.push_back(edgeWallVolume * edgeWallFactor);
						subNotes.push_back(str);
						matid.push_back(slab->GetMaterial()->m_ID);
					}
				}
			}			
		}
		else if (calcType == 2)	// Half PC Slab의 PC부분
		{
			rVolume = (A - IAC) * T1;

			if (IAC > 0)
			{
				if (_bShowVariable)
					noteVolume.Format(_T("(%g(A) - %g(IA)) * %g(T1)"), A, IAC, T1);
				else
					noteVolume.Format(_T("(%g - %g) * %g"), A, IAC, T1);
			}
			else
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(A) * %g(T1)"), A, T1);
				else
					noteVolume.Format(_T("%g * %g"), A, T1);
			}
		}
		else
			return;

		// Form Area
		if (calcType == 0 || calcType == 1)
		{
			if (calcType == 0)
			{
				rArea = (A - IA) - ACV + (OL*T - ACV_OUT + IL * T - ACV_IN) + A2 + A2_END;
				noteArea.Format(_T("(%g - %g) - (%g) + (%g * %g - %g + %g * %g - %g) + %g + %g"), A, IA, ACV, OL, T, ACV_OUT, IL, Round(T), Round(ACV_IN), Round(A2), Round(A2_END));
			}
			else if (calcType == 1)
			{
				rArea = (A - IA) - ACV + (OL*T2 - ACV_OUT + IL * T2 - ACV_IN) + A2 + A2_END;
				noteArea.Format(_T("(%g - %g) - (%g) + (%g * %g - %g + %g * %g - %g) + %g + %g"), A, IA, ACV, OL, T2, ACV_OUT, IL, Round(T2), Round(ACV_IN), Round(A2), Round(A2_END));
			}

			if (mp_Owner->ma_UserFormWork.size() > 0)
			{
				map<CString, CString> dirToType;
				for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
				{
					CString formworkName = mp_Owner->ma_UserFormWork[i].formWorkName;
					//if (formworkName == "STEEL_Deck")
					//	formworkName = "Deck";
					dirToType[mp_Owner->ma_UserFormWork[i].dir] = formworkName;
				}
				if (dirToType.count(_T("B")) > 0 && !slab->IsSOG() && slab->IsHalfPCSlab() == false)
				{
					if (mp_Owner->IsRebarEmbeddedDeckSlab() && !(SX == 0 && SY == 0))
					{
						// 지지보가 철골 보인지 판단
						bool isSteelBeam = false;
						vector<MSCompoG*> beamArr = mp_Owner->FindConnectMember(FindType_MSElement::msBeamG, 0, 1, true, true, false);
						for (MSCompoG* touchBeamG : beamArr)
						{
							if (touchBeamG->IsSteel())
							{
								isSteelBeam = true;
								break;
							}
						}

						if (isSteelBeam)
						{
							// 지지보 판단: 철골 보
							rArea = SX;
							if (_bShowVariable)
								noteArea.Format(_T("%g(A)"), SX);
							else
								noteArea.Format(_T("%g(A)"), SX);
						}
						else
						{
							// 지지보 판단: Not 철골 보
							rArea = SX;
							if (_bShowVariable)
								noteArea.Format(_T("%g(A)"), SX);
							else
								noteArea.Format(_T("%g(A)"), SX);
						}
					}
					else
					{
						rArea = A;
						//noteArea.Format(_T("(%g - %g)"), A, IA);
						if (_bShowVariable)
							noteArea.Format(_T("%g(A)"), A);
						else
							noteArea.Format(_T("%g"), A);
					}


					if (IA > 0)
					{
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" - %g(IA)"), IA);
						else
							tmp.Format(_T(" - %g"), IA);
						rArea -= IA;
						noteArea += tmp;
					}

					if (ACV > 0)
					{
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" - %g(ACV)"), ACV);
						else
							tmp.Format(_T(" - %g"), ACV);
						rArea -= ACV;
						noteArea += tmp;
					}

					if (Factor != 1.0)
					{
						rArea *= Factor;
						czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
						noteArea = czoneNote;
					}

					result.formDir.push_back(_T("Bot"));
					result.formWorkName.push_back(dirToType[_T("B")]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}

				if (dirToType.count(_T("V")) > 0)
				{
					rArea = (OL*T - ACV_OUT + IL * T - ACV_IN) + A2 + A2_END;
					if (calcType == 0)
					{
						rArea = (OL*T - ACV_OUT + IL * T - ACV_IN) + A2 + A2_END;
						if (_bShowVariable)
							noteArea.Format(_T("%g(OL) * %g(T)"), OL, T);
						else
							noteArea.Format(_T("%g * %g"), OL, T);

						if (ACV_OUT > 0)
						{
							if (_bShowVariable)
								noteArea.Format(_T("%g(OL) * %g(T) - %g(ACV_OUT)"), OL, T, ACV_OUT);
							else
								noteArea.Format(_T("%g * %g - %g"), OL, T, ACV_OUT);
						}

						if (IL > 0)
						{
							CString tmp;
							if (_bShowVariable)
								tmp.Format(_T(" + (%g(IL) * %g(T))"), IL, T);
							else
								tmp.Format(_T(" + (%g * %g)"), IL, T);

							if (ACV_IN > 0)
							{
								if (_bShowVariable)
									tmp.Format(_T(" + (%g(IL) * %g(T) - %g(ACV_IN))"), IL, T, ACV_IN);
								else
									tmp.Format(_T(" + (%g * %g - %g)"), IL, T, ACV_IN);
							}

							noteArea += tmp;
						}
					}
					else if (calcType == 1)
					{
						// T 대신 T2로 출력
						rArea = (OL*T2 - ACV_OUT + IL * T2 - ACV_IN) + A2 + A2_END;
						if (_bShowVariable)
							noteArea.Format(_T("%g(OL) * %g(T2)"), OL, T2);
						else
							noteArea.Format(_T("%g * %g"), OL, T2);

						if (ACV_OUT > 0)
						{
							if (_bShowVariable)
								noteArea.Format(_T("%g(OL) * %g(T2) - %g(ACV_OUT)"), OL, T2, ACV_OUT);
							else
								noteArea.Format(_T("%g * %g - %g"), OL, T2, ACV_OUT);
						}

						if (IL > 0)
						{
							CString tmp;
							if (_bShowVariable)
								tmp.Format(_T(" + (%g(IL) * %g(T2))"), IL, T2);
							else
								tmp.Format(_T(" + (%g * %g)"), IL, T2);

							if (ACV_IN > 0)
							{
								if (_bShowVariable)
									tmp.Format(_T(" + (%g(IL) * %g(T2) - %g(ACV_IN))"), IL, T2, ACV_IN);
								else
									tmp.Format(_T(" + (%g * %g - %g)"), IL, T2, ACV_IN);
							}

							noteArea += tmp;
						}
					}

					if (A2 > 0)
					{
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" + %g(A2)"), A2);
						else
							tmp.Format(_T(" + %g"), A2);
						noteArea += tmp;
					}
					if (A2_END > 0)
					{
						CString tmp;
						if (_bShowVariable)
							tmp.Format(_T(" + %g(A2_END)"), A2_END);
						else
							tmp.Format(_T(" + %g"), A2_END);
						noteArea += tmp;
					}
					if (HUN != 0)
					{
						rArea += HUN;
						if (rArea < 0) rArea = 0;
						CString str;
						if (_bShowVariable)
						{
							if (HUN > 0) str.Format(_T(" + %g(HAUNCH)"), fabs(HUN));
							else str.Format(_T(" - %g(HAUNCH)"), fabs(HUN));
						}
						else
						{
							if (HUN > 0) str.Format(_T(" + %g"), fabs(HUN));
							else str.Format(_T(" - %g"), fabs(HUN));
						}
						noteArea += str;
					}

					if (Factor != 1.0)
					{
						rArea *= Factor;
						czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
						noteArea = czoneNote;
					}

					if (rArea > 0.0)// [Q22-787] 슬래브 거푸집 옆면적이 0이 아닌경우만 출력
					{
						result.formDir.push_back(_T("Ver"));
						result.formWorkName.push_back(dirToType[_T("V")]);
						result.formArea.push_back(rArea);
						result.formNote.push_back(noteArea);
					}
				}
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneHCSPCSlab(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType)
{
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msSlabG && mp_Owner->IsHCSPCSlab())
	{
		MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_Owner);
		if (slab == nullptr)
			return;

		MSRcSlabM* rcSlab = dynamic_cast<MSRcSlabM*>(slab->GetRebarM());
		if (rcSlab == nullptr)
			return;

		MSRcFaceMemberM *pRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(slab->GetCompoM());
		if (pRcFaceMemberM == nullptr)
			return;

		if (ma_Param.size() != 20)
			return;

		double Lx = ma_Param[0], Ly = ma_Param[1], Sx = ma_Param[2], Sy = ma_Param[3];
		double L1 = ma_Param[4], L2 = ma_Param[5], L3 = ma_Param[6], H1 = ma_Param[7], H2 = ma_Param[8], HL = ma_Param[9];
		double FL = ma_Param[11], A_bot = ma_Param[12], A_open = ma_Param[13], T_s = ma_Param[14], B_b = ma_Param[15], L_b = ma_Param[16], W_col = ma_Param[17];
		double D_col = ma_Param[18], R_col = ma_Param[19];
		int NH = ma_Param[10];

		int NFH = (int)FL;

		if (calcType == 1)	// PC 현장타설
		{
			if (pRcFaceMemberM->GetHoleType() == 0)
			{
				rVolume = Lx * Ly * H2 + pow((L1 / 2), 2) * PI * (2 * HL) * NFH;
				if (_bShowVariable)
					noteVolume.Format(_T("%g(Lx) * %g(Ly) * %g(H2) + (%g(L1) / 2)^2 * PI * 2 *(%g(HL) * %d(NFH))"), Lx, Ly, H2, L1, HL, NFH);
				else
					noteVolume.Format(_T("%g * %g * %g + (%g / 2)^2 * PI * 2 *(%g * %d)"), Lx, Ly, H2, L1, HL, NFH);
			}
			else if (pRcFaceMemberM->GetHoleType() == 1)
			{
				rVolume = Lx * Ly * H2 + (pow((L1 / 2), 2) * PI + (L3 - L1) * L1) * HL * NFH;
				if (_bShowVariable)
					noteVolume.Format(_T("%g(Lx) * %g(Ly) * %g(H2) + ((%g(L1) / 2)^2 * PI + (%g(L3) - %g(L1)) * %g(L1)) * %g(HL) * %d(NFH)"), Lx, Ly, H2, L1, L3, L1, L1, HL, NFH);
				else
					noteVolume.Format(_T("%g * %g * %g + ((%g / 2)^2 * PI + (%g - %g) * %g) * %g * %d"), Lx, Ly, H2, L1, L3, L1, L1, HL, NFH);
			}
		}
		if (calcType == 2) // PC 제작
		{
			if (pRcFaceMemberM->GetHoleType() == 0)
			{
				rVolume = Sx * Sy * H1 - pow((L1 / 2), 2) * PI * Sx * NH;
				if (_bShowVariable)
					noteVolume.Format(_T("%g(Sx) * %g(Sy) * %g(H1) - (%g(L1) / 2)^2 * PI * %g(Sx) * %d(NH)"), Sx, Sy, H1, L1, Sx, NH);
				else
					noteVolume.Format(_T("%g * %g * %g - (%g / 2)^2 * PI * %g * %d"), Sx, Sy, H1, L1, Sx, NH);
			}
			else if (pRcFaceMemberM->GetHoleType() == 1)
			{
				rVolume = Sx * Sy * H1 - (pow((L1 / 2), 2) * PI + (L3 - L1) * L1) * Sx * NH;
				if (_bShowVariable)
					noteVolume.Format(_T("%g(Sx) * %g(Sy) * %g(H1) - ((%g(L1) / 2)^2 * PI + (%g(L3) - %g(L1)) * %g(L1)) * %g(Sx) * %d(NH)"), Sx, Sy, H1, L1, L3, L1, L1, Sx, NH);
				else
					noteVolume.Format(_T("%g * %g * %g - ((%g / 2)^2 * PI + (%g - %g) * %g) * %g * %d"), Sx, Sy, H1, L1, L3, L1, L1, Sx, NH);
			}
		}
	}
}


void VolumeAreaInfoEMS::GetResultByCZoneRPSPCSlab(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType)
{
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msSlabG && mp_Owner->IsRPSPCSlab())
	{
		MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_Owner);
		if (slab == nullptr)
			return;

		MSRcSlabM* rcSlab = dynamic_cast<MSRcSlabM*>(slab->GetRebarM());
		if (rcSlab == nullptr)
			return;
		if (ma_Param.size() != 21)
			return;

		double Lx = ma_Param[0], Ly = ma_Param[1], Sx = ma_Param[2], Sy = ma_Param[3],
			L1 = ma_Param[4], L2 = ma_Param[5], L3 = ma_Param[6], H1 = ma_Param[7], H2 = ma_Param[8], H3 = ma_Param[9], HL = ma_Param[10],
			FL = ma_Param[11], A_bot = ma_Param[12], A_open = ma_Param[13], T_s = ma_Param[14], B_b = ma_Param[15], L_b = ma_Param[16], W_col = ma_Param[17],
			D_col = ma_Param[18], R_col = ma_Param[19];
		int NH = ma_Param[20];


		if (calcType == 1)	// PC 현장타설
		{
			if (R_col == 0)
			{
				if (_bShowVariable)
					rVolume = (Lx * Ly * H3) + (L2 * H2 * (2 * HL) * NH);

				noteVolume.Format(_T("(%g(Lx) * %g(Ly) * %g(H3)) + (%g(L2) * %g(H2) *( 2 * %g(HL)) * %d(NH))"), Lx, Ly, H3, L2, H2, HL, NH);
			}
			else
			{
				if (_bShowVariable)
					rVolume = (Lx * Ly * H3) + (((L2 * H2 *  (NH - 1)) + D_col * H2) * HL) * 2;

				noteVolume.Format(_T("(%g(Lx) * %g(Ly) * %g(H3)) + (((%g(L2) * %g(H2) * %d(NH-1)) + %g(L2@) * %g(H2)) * %g(HL)) * 2"), Lx, Ly, H3, L2, H2, NH - 1, D_col, H2, HL);
			}
		}
		if (calcType == 2) // PC 제작
		{
			if (R_col == 0)
			{
				if (_bShowVariable)
					rVolume = (Sx * Sy * (H1 + H2)) - (L2 * H2 * (2 * HL) * NH);

				noteVolume.Format(_T("(%g(Sx) * %g(Sy) * (%g(H1) + (%gH2)) - (%g(L2) * %g(H2) * ( 2 * %g(HL)) * %d(NH))"), Sx, Sy, H1, H2, L2, H2, HL, NH);
			}
			else
			{
				if (_bShowVariable)
					rVolume = (Sx * Sy * (H1 + H2)) - (((L2 * H2 *  (NH - 1)) + D_col * H2) * HL) * 2;

				noteVolume.Format(_T("(%g(Sx) * %g(Sy) * (%g(H1) + (%gH2)) - (((%g(L2) * %g(H2) * %d(NH-1)) + %g(L2@) * %g(H2)) * %g(HL)) * 2"), Sx, Sy, H1, H2, L2, H2, NH - 1, D_col, H2, HL);

			}
		}
	}
}


void VolumeAreaInfoEMS::GetResultByCZoneIMTPCSlab(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, int calcType)
{
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msSlabG && mp_Owner->IsIMTPCSlab())
	{
		MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_Owner);
		if (slab == nullptr)
			return;

		MSRcSlabM* rcSlab = dynamic_cast<MSRcSlabM*>(slab->GetRebarM());
		if (rcSlab == nullptr)
			return;


		if (ma_Param.size() != 17)
			return;


		// Sx, Sy: Inner Slab 길이 / L1, L2, L3, H1, H2, H3: IMT Type PC Slab Profile 변수 / ELx, ELy: 여유길이 / OL: 둘레
		double Lx = ma_Param[0], Ly = ma_Param[1], Sx = ma_Param[2], Sy = ma_Param[3], L1 = ma_Param[4], L2 = ma_Param[5], L3 = ma_Param[6];
		double H1 = ma_Param[7], H2 = ma_Param[8], H3 = ma_Param[9], ELx = ma_Param[10], ELy = ma_Param[11], OL = ma_Param[12], DF = ma_Param[16];
		// NR: Number of Rib / NELx: 단변 여유적용 갯수 / NELy: 장변 여유적용 갯수
		int NR = ma_Param[13], NELx = ma_Param[14], NELy = ma_Param[15];

		// Concrete
		CString SxEL;
		CString SyEL;
		if (calcType == 0 || calcType == 1)	// 전체, RC부분
		{
			CString tmp;
			SxEL.Format(_T("(%g(Sx)"), Sx);
			if (NELx > 0 && ELx > 0)
			{
				tmp.Format(_T(" + (%d)(NELx) * %g(ELx)"), NELx, ELx);
				SxEL += tmp;
			}
			SxEL += _T(")");

			SyEL.Format(_T("(%g(Sy)"), Sy);
			if (NELy > 0 && ELy > 0)
			{
				tmp.Format(_T(" + (%d)(NELy) * %g(ELy)"), NELy, ELy);
				SyEL += tmp;
			}
			SyEL += _T(")");

			//https://changsoft.atlassian.net/browse/Q24-102 IMT슬래브 현장타설 산식변경
			//rVolume = Lx * Ly * H3 + (2 * L1 + (NR - 1) * L3) * (Sx + NELx * ELx) * H2;
			rVolume = Lx * Ly * (H3 +H2) - (NR * L2) * (Sx + NELx * ELx) * H2;
			noteVolume.Format(_T("(%g(Lx) * %g(Ly) * (%g(H3)) + %g(H2))"), Lx, Ly, H3, H2);
			if (slab->GetIsHasRib() && NR > 0)
			{
				//https://changsoft.atlassian.net/browse/Q24-102 IMT슬래브 현장타설 산식변경
				//tmp.Format(_T(" + ((2) * %g(L1) + ((%d)(NR) - (1)) * %g(L3)) * "), L1, NR, L3);
				tmp.Format(_T(" - (%d(NR) * %g(L2)) * "), NR, L2);
				noteVolume += tmp;

				noteVolume += SxEL;

				tmp.Format(_T(" * %g(H2)"), H2);
				noteVolume += tmp;
			}
			else
			{
				rVolume = Lx * Ly * H3 + (Sx + NELx * ELx) * (Sy + NELy * ELy) * H2;

				noteVolume += _T(" + ");
				noteVolume += SxEL;
				noteVolume += _T(" * ");
				noteVolume += SyEL;

				tmp.Format(_T(" * %g(H2)"), H2);
				noteVolume += tmp;
			}
		}
		else if (calcType == 2)	//  PC 부분
		{
			if (slab->GetIsHasRib() && NR > 0)
			{
				rVolume = Sx * Sy * H1 + (NR * Sx * L2 * H2);
				noteVolume.Format(_T("%g(Sx) * %g(Sy) * %g(H1) + ((%d)(NR) * %g(Sx) * %g(L2) * %g(H2))"), Sx, Sy, H1, NR, Sx, L2, H2);
			}
			else
			{
				rVolume = Sx * Sy * H1;
				noteVolume.Format(_T("%g(Sx) * %g(Sy) * %g(H1)"), Sx, Sy, H1);
			}
		}

		// Form Area
		if (calcType == 0 || calcType == 1)	// 전체 또는  RC부분
		{
			if (mp_Owner->ma_UserFormWork.size() > 0)
			{
				map<CString, CString> dirToType;
				for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
					dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;

				// 밑면
				if (dirToType.count(_T("B")) > 0)
				{
					rArea = DF;
					noteArea.Format(_T("%g(A)"), DF);

					result.formDir.push_back(_T("Bot"));
					result.formWorkName.push_back(dirToType[_T("B")]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}

				// 옆면
				if (dirToType.count(_T("V")) > 0)
				{
					rArea = (H2 + H3) * OL;
					if (_bShowVariable)
						noteArea.Format(_T("(%g(H2) + %g(H3)) * %g(OL)"), H2, H3, OL);
					else
						noteArea.Format(_T("(%g + %g) * %g"), H2, H3, OL);

					result.formDir.push_back(_T("Ver"));
					result.formWorkName.push_back(dirToType[_T("V")]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneGeneralDeck(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
{
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msSlabG && mp_Owner->IsGeneralDeckSlab())
	{
		if (ma_Param.size() != 11) return;
		double SX = ma_Param[0], SY = ma_Param[1], LX = ma_Param[2], LY = ma_Param[3];
		double OA = ma_Param[4], T = ma_Param[5], FT = ma_Param[6], FBW = ma_Param[7], FTW = ma_Param[8], FS = ma_Param[9], DF = ma_Param[10];

		//Concrete
		rVolume = (LX * LY * T) + ((0.5 * (FBW + FTW) * FT *SX) * SY / FS) + ((LX - SX) * SY + (LY - SY) * SX) * FT;
		rVolume = Round(rVolume, 3);
		//noteVolume
		CString topslabV;
		CString formV;
		CString endGapV;

		if (_bShowVariable)
		{
			topslabV.Format(_T("(%g(LX) * %g(LY) * %g(T)) + "), LX, LY, T);
			formV.Format(_T("(0.5 * (%g(FBW) + %g(FTW)) * %g(FT) * %g(SX) * %g(SY) / %g(FS)) + "), FBW, FTW, FT, SX, SY, FS);
			endGapV.Format(_T("((%g(LX) - %g(SX)) * %g(SY) + (%g(LY) - %g(SY)) * %g(SX)) * %g(FT))"), LX, SX, SY, LY, SY, SX, FT);
			noteVolume = topslabV + formV + endGapV;
		}
		else
		{
			topslabV.Format(_T("(%g * %g * %g) + "), LX, LY, T);
			formV.Format(_T("(0.5 * (%g + %g) * %g * %g * %g / %g) + "), FBW, FTW, FT, SX, SY, FS);
			endGapV.Format(_T("((%g - %g) * %g + (%g - %g) * %g) * %g)"), LX, SX, SY, LY, SY, SX, FT);
			noteVolume = topslabV + formV + endGapV;
		}

		//Form - side 없음
		if (mp_Owner->ma_UserFormWork.size() > 0)
		{
			map<CString, CString> dirToType;
			for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
				dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;

			// 지지보가 철골 보인지 판단
			bool isSteelBeam = false;
			vector<MSCompoG*> beamArr = mp_Owner->FindConnectMember(FindType_MSElement::msBeamG, 0, 1, true, true, false);
			for (MSCompoG* touchBeamG : beamArr)
			{
				if (touchBeamG->IsSteel())
				{
					isSteelBeam = true;
					break;
				}
			}

			if (isSteelBeam)
			{
				// 지지보 판단: 철골 보
				rArea = DF;
				if (_bShowVariable)
					noteArea.Format(_T("%g(A)"), DF);
				else
					noteArea.Format(_T("%g(A)"), DF);
			}
			else
			{
				// 지지보 판단: Not 철골 보
				rArea = SX * SY;
				if (_bShowVariable)
					noteArea.Format(_T("%g(A)"), SX*SY);
				else
					noteArea.Format(_T("%g(A)"), SX*SY);
			}

			if (OA > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" - %g(OA)"), OA);
				else
					tmp.Format(_T(" - %g"), OA);
				noteArea += tmp;
				rArea -= OA;
			}

			double Factor = 0.0;
			if (cz_id != -1)
			{
				for (int i = 0; i < ma_CZoneID.size(); ++i)
				{
					if (cz_id == ma_CZoneID[i])
						Factor = ma_Factor[i];
				}
			}
			else
				Factor = 1.0;

			if (Factor == 0.0) return;

			CString czoneName;
			CString czoneNote;
			if (Factor != 1.0)
			{
				MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
				MSConstructionZone* czone = curFloor->GetCZone(cz_id);
				czoneName = czone->GetName();
			}

			if (Factor != 1.0)
			{
				rVolume *= Factor;
				czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteVolume);
				noteVolume = czoneNote;
			}

			if (Factor != 1.0)
			{
				rArea *= Factor;
				czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
				noteArea = czoneNote;
			}



			result.formDir.push_back(_T("Bot"));
			result.formWorkName.push_back(dirToType[_T("B")]);
			result.formArea.push_back(rArea);
			result.formNote.push_back(noteArea);
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneWall(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
{
	double rArea = 0;
	CString noteArea;

	if (mp_Owner->GetType() == MSElement::msWallG)
	{
		if (ma_Param.size() != 22) return;
		double T = ma_Param[0], H = ma_Param[1], A = ma_Param[2], TO = ma_Param[3], BO = ma_Param[4], TSTB = ma_Param[5];
		double CL = ma_Param[6], LL = ma_Param[7], RL = ma_Param[8], /*IW = ma_Param[9], IH = ma_Param[10],*/ LACV = ma_Param[11], RACV = ma_Param[12];	// BH-4745 IW와 IH는 물량계산에는 사용하면 안된다. 물량산출 옵션이 반영되어있지 않기때문. 대신 IWC, IHC (콘크리트) / IWF, IWF (거푸집)을 사용해야 한다.
		double IE1 = ma_Param[13], IE2 = ma_Param[14], ACVE1 = ma_Param[15], ACVE2 = ma_Param[16], IWC = ma_Param[17], IHC = ma_Param[18], IWF = ma_Param[19], IHF = ma_Param[20];
		double NUM_O = ma_Param[21];
		double H2 = H + TO - BO - TSTB;	// 콘크리트 물량, 거푸집 옆면 계산시 사용되는 값. 높이값이 공제되어 있다.
		double HF = H + TO - BO;	// 마구리면 거푸집 계산시 사용되는 높이값. 적산옵션 -> 벽체 마구리 거푸집 접합부 공제 체크 해제 시 사용
		// BH-4478 콘크리트 물량 계산시 사용되는 길이값을 LL과 RL의 평균으로 계산하면 벽에 기둥이 맞닿아있는 경우 잘못된 물량이 계산된다.
		double LAvg = CL;
		//double LAvg = (LL+RL)/2.0;

		// Concrete
		// BH-4478 길이값을 Center Line으로 하였으나, 기둥이 가운데에 들어가는 등 복잡하게 모델링 되는 경우 바닥의 면적x높이로 계산한다.
		bool isRegularShape = abs((A * H2) - (T * H2 * LAvg)) < 0.1;
		if (A == 0)
			isRegularShape = true;
		if (isRegularShape)
		{
			rVolume = T * H2 * LAvg - T * IWC * IHC;
			//noteVolume.Format(_T("%g x %g x %g - %g x %g x %g"), T, H2, L, T, IW, IH);
			if (IWC > 0.0)
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(T) * %g(H') * %g(L') - %g(T) * %g(IW) * %g(IH)"), T, H2, LAvg, T, IWC, IHC);
				else
					noteVolume.Format(_T("%g * %g * %g - %g * %g * %g"), T, H2, LAvg, T, IWC, IHC);
			}
			else
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(T) * %g(H') * %g(L')"), T, H2, LAvg);
				else
					noteVolume.Format(_T("%g * %g * %g"), T, H2, LAvg);
			}
		}
		else
		{
			rVolume = A * H2 - T * IWC * IHC;
			if (IWC > 0.0)
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(A) * %g(H') - %g(T) * %g(IW) * %g(IH)"), A, H2, T, IWC, IHC);
				else
					noteVolume.Format(_T("%g * %g - %g * %g * %g"), A, H2, T, IWC, IHC);
			}
			else
			{
				if (_bShowVariable)
					noteVolume.Format(_T("%g(A) * %g(H')"), A, H2);
				else
					noteVolume.Format(_T("%g * %g"), A, H2);
			}
		}

		// Form Area
		if (mp_Owner->ma_UserFormWork.size() > 0)
		{
			map<CString, CString> dirToType;
			for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
				dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
			if (dirToType.count(_T("L")) > 0 && dirToType.count(_T("R")) > 0 && dirToType[_T("L")] == dirToType[_T("R")])
			{
				rArea = HF * (LL + RL) - 2 * IWF * IHF - LACV - RACV;
				if (IWF > 0.0)
				{
					if (_bShowVariable)
						noteArea.Format(_T("%g(H') * (%g(LL) + %g(RL)) - 2 * (%g(IW) * %g(IH))"), HF, LL, RL, IWF, IHF);
					else
						noteArea.Format(_T("%g * (%g + %g) - 2 * (%g * %g)"), HF, LL, RL, IWF, IHF);
				}
				else
				{
					if (_bShowVariable)
						noteArea.Format(_T("%g(H') * (%g(LL) + %g(RL))"), HF, LL, RL);
					else
						noteArea.Format(_T("%g * (%g + %g)"), HF, LL, RL);
				}
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" - %g(LACV)"), LACV);
				else
					tmp.Format(_T(" - %g"), LACV);
				if (LACV > 0) noteArea += tmp;

				if (_bShowVariable)
					tmp.Format(_T(" - %g(RACV)"), RACV);
				else
					tmp.Format(_T(" - %g"), RACV);
				if (RACV > 0) noteArea += tmp;

				result.formDir.push_back(_T("Side"));
				result.formWorkName.push_back(dirToType[_T("L")]);
				result.formArea.push_back(rArea);
				result.formNote.push_back(noteArea);
			}
			else
			{
				if (dirToType.count(_T("L")) > 0)
				{
					rArea = HF * LL - IWF * IHF - LACV;
					if (IWF > 0.0)
					{
						if (_bShowVariable)
							noteArea.Format(_T("(%g(H') * %g(LL)) - (%g(IW) * %g(IH))"), HF, LL, IWF, IHF);
						else
							noteArea.Format(_T("(%g * %g) - (%g * %g)"), HF, LL, IWF, IHF);
					}
					else
					{
						if (_bShowVariable)
							noteArea.Format(_T("%g(H') * %g(LL)"), HF, LL);
						else
							noteArea.Format(_T("%g * %g"), HF, LL);
					}
					CString tmp;
					if (_bShowVariable)
						tmp.Format(_T(" - %g(LACV)"), LACV);
					else
						tmp.Format(_T(" - %g"), LACV);
					if (LACV > 0) noteArea += tmp;

					result.formDir.push_back(_T("Left"));
					result.formWorkName.push_back(dirToType[_T("L")]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
				if (dirToType.count(_T("R")) > 0)
				{
					rArea = HF * RL - IWF * IHF - RACV;
					if (IWF > 0.0)
					{
						if (_bShowVariable)
							noteArea.Format(_T("(%g(H') * %g(RL)) - (%g(IW) * %g(IH))"), HF, RL, IWF, IHF);
						else
							noteArea.Format(_T("(%g * %g) - (%g * %g)"), HF, RL, IWF, IHF);
					}
					else
					{
						if (_bShowVariable)
							noteArea.Format(_T("%g(H') * %g(RL)"), HF, RL);
						else
							noteArea.Format(_T("%g * %g"), HF, RL);
					}
					CString tmp;
					if (_bShowVariable)
						tmp.Format(_T(" - %g(RACV)"), RACV);
					else
						tmp.Format(_T(" - %g"), RACV);
					if (RACV > 0) noteArea += tmp;

					result.formDir.push_back(_T("Right"));
					result.formWorkName.push_back(dirToType[_T("R")]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
			}

			if (dirToType.count(_T("L")) > 0 || dirToType.count(_T("R")) > 0)
			{
				if (IWF > 0.0)
				{
					int num_side = 2;
					if (NUM_O > 1.0)
						num_side *= NUM_O;

					rArea = (IWF + num_side * IHF) * T;
					if (_bShowVariable)
						noteArea.Format(_T("(%g(IW) + %d * %g(IH)) * %g(T)"), IWF, num_side, IHF, T);
					else
						noteArea.Format(_T("(%g + %d * %g) * %g"), IWF, num_side, IHF, T);
					CString formWorkName = (dirToType.count(_T("L")) > 0 ? dirToType[_T("L")] : dirToType[_T("R")]);
					CString formDir = (dirToType.count(_T("L")) > 0 ? _T("Left") : _T("Right"));

					result.formDir.push_back(formDir);
					result.formWorkName.push_back(formWorkName);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}

				if (IE1 > 0.0 || IE2 > 0.0)
				{
					double ACVE_H = 0.1;	// 마구리면의 공제면적을 공제높이로 변경했을 떄 이 높이보다 크면 공제면적을 공제높이로 표현한다. (단위: m)
					CString type1 = 0, type2 = 0;
					double area1 = 0, area2 = 0;
					CString end_note1, end_note2;

					bool isIgnoreAreaWallEndFormWork = mp_Owner->GetBuilding()->GetQuantityOption()->m_bIsIgnoreAreaWallEndFormWork; // 벽체 마구리 거푸집 접합부 공제 여부
					if (IE1 > 0.0)
					{
						if (isIgnoreAreaWallEndFormWork == false)
						{
							// 벽체의 마구리면 공제를 하지 않는 경우 
							rArea = HF * T;
							if (_bShowVariable)
								noteArea.Format(_T("%g(H) * %g(T)"), HF, T);
							else
								noteArea.Format(_T("%g * %g"), HF, T);
						}
						else
						{
							rArea = (H2)*T - ACVE1;
							if (T > 0 && ACVE1 / T > ACVE_H)
							{
								// 공제면적이 크면 공제높이로 변경해서 출력한다.
								if (_bShowVariable)
									noteArea.Format(_T("(%g(H') - %g(DH)) * %g(T)"), H2, ACVE1 / T, T);
								else
									noteArea.Format(_T("(%g - %g) * %g"), H2, ACVE1 / T, T);
							}
							else
							{
								if (_bShowVariable)
									noteArea.Format(_T("%g(H') * %g(T)"), H2, T);
								else
									noteArea.Format(_T("%g * %g"), H2, T);

								if (ACVE1 > 0)
								{
									CString tmp1;
									if (_bShowVariable)
										tmp1.Format(_T(" - %g(ACVE)"), ACVE1);
									else
										tmp1.Format(_T(" - %g"), ACVE1);
									noteArea += tmp1;
								}
							}
						}

						if (dirToType.count(_T("F")) > 0)
						{
							type1 = dirToType[_T("E")];
							area1 = rArea;
							end_note1 = noteArea;
						}
					}
					if (IE2 > 0.0)
					{
						if (isIgnoreAreaWallEndFormWork == false)
						{
							// 벽체의 마구리면 공제를 하지 않는 경우 
							rArea = HF * T;
							if (_bShowVariable)
								noteArea.Format(_T("%g(H) * %g(T)"), HF, T);
							else
								noteArea.Format(_T("%g * %g"), HF, T);
						}
						else
						{
							rArea = (H2)*T - ACVE2;
							if (T > 0 && ACVE2 / T > ACVE_H)
							{
								// 공제면적이 크면 공제높이로 변경해서 출력한다.
								if (_bShowVariable)
									noteArea.Format(_T("(%g(H') - %g(DH)) * %g(T)"), H2, ACVE2 / T, T);
								else
									noteArea.Format(_T("(%g - %g) * %g"), H2, ACVE2 / T, T);
							}
							else
							{
								if (_bShowVariable)
									noteArea.Format(_T("%g(H') * %g(T)"), H2, T);
								else
									noteArea.Format(_T("%g * %g"), H2, T);

								if (ACVE2 > 0)
								{
									CString tmp2;
									if (_bShowVariable)
										tmp2.Format(_T(" - %g(ACVE)"), ACVE2);
									else
										tmp2.Format(_T(" - %g"), ACVE2);
									noteArea += tmp2;
								}
							}
						}

						if (dirToType.count(_T("E")) > 0)
						{
							type2 = dirToType[_T("F")];
							area2 = rArea;
							end_note2 = noteArea;
						}
					}
					double Factor = 0.0; //벽체 경우 Czone을 계산할때 따로 해주는데 여기서 거푸집 마구리 물량을 판단할때 Factor 값이 없어서 Czone 계산을 산식을 만들때 X2로 만들어주기 때문에 Factor를 가져와서 계산 해 준뒤 Facotr가 1이면 X2 아니면 "" 공백란을 넣어줌
					double dLibLathArea = 0.0;
					for (int i = 0; i < ma_CZoneID.size(); ++i)
					{
						if (cz_id == ma_CZoneID[i])
						{
							Factor = ma_Factor[i];
							dLibLathArea = ma_LibLathArea[i];
						}
					}
					if (area1 > 0 && area2 > 0 && abs(area1 - area2) < DTOL5 && type1 == type2)
					{
						result.formDir.push_back(_T("Close"));
						result.formWorkName.push_back(type1);
						result.formArea.push_back(area1 + area2);
						if (Factor == 1)
							result.formNote.push_back(end_note1 + _T(" x 2"));
						else
							result.formNote.push_back(end_note1 + _T(""));

					}
					else
					{
						if (area1 > 0) { result.formDir.push_back(_T("Close")); result.formWorkName.push_back(type1); result.formArea.push_back(area1); result.formNote.push_back(end_note1); }
						if (area2 > 0) { result.formDir.push_back(_T("Close")); result.formWorkName.push_back(type2); result.formArea.push_back(area2); result.formNote.push_back(end_note2); }
					}

				}
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneIsoFoot(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid)
{
	double rArea = 0;
	CString noteArea;

	MSIsoFootG* isofoot = dynamic_cast<MSIsoFootG*>(mp_Owner);
	if (isofoot == NULL) return;

	bool isHaunchOutstretched = mp_Owner->GetBuilding()->GetQuantityOption()->m_dHaunchOutstretchedLength > 0;

	int haunchcount = 0;
	for (int i = 0; i < isofoot->ma_Haunches.size(); i++)
	{
		MSSubHaunchG* haunch = isofoot->ma_Haunches[i];
		//if (haunch->m_bUseHaunchMaterial== false)
		haunchcount++; //구조재로 쓰이는 경우에만 여기에서 처리
	}

	int haunchStretchedCnt = 0;
	if (isHaunchOutstretched)
		haunchStretchedCnt = haunchcount;

	if (ma_Param.size() != 7 + 3 * haunchcount + haunchStretchedCnt) return;

	double W = ma_Param[0], D = ma_Param[1], T = ma_Param[2], TB = ma_Param[3], ACV = ma_Param[4], ACV_BOT = ma_Param[5], VCUT = ma_Param[6];
	vector<double> haunch_param;
	for (int i = 0; i < haunchcount; i++)
	{
		haunch_param.push_back(ma_Param[7 + i * 3]);
		haunch_param.push_back(ma_Param[7 + i * 3 + 1]);
		haunch_param.push_back(ma_Param[7 + i * 3 + 2]);
	}

	vector<double> haunchVolumes;
	double HV = 0;
	double H_FORM = 0;
	for (int i = 0; i < haunchcount; i++)
	{
		H_FORM += haunch_param[i * 3];
		HV += haunch_param[i * 3 + 1];
		haunchVolumes.push_back(haunch_param[i * 3 + 1]);
	}

	vector<double> haunchStretchedParam;
	for (int i = 0; i < haunchStretchedCnt; i++)
	{
		double volume = ma_Param[7 + 3 * haunchcount + i];
		haunchStretchedParam.push_back(volume);
	}

	// Concrete
	rVolume = W * D*T;
	if (_bShowVariable)
		noteVolume.Format(_T("%g(B) * %g(D) * %g(T)"), W, D, T);
	else
		noteVolume.Format(_T("%g * %g * %g"), W, D, T);

	if (VCUT > 0)
	{
		CString str;
		if (_bShowVariable)
			str.Format(_T("- %g(VCUT)"), VCUT);
		else
			str.Format(_T("- %g"), VCUT);
		noteVolume += str;
		rVolume -= VCUT;
	}

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
	}
	if (Factor == 0.0) return;

	if (Factor != 1.0)
	{
		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
		MSConstructionZone* czone = curFloor->GetCZone(cz_id);
		rVolume *= Factor;
		CString tmp;
		tmp.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, noteVolume);
		noteVolume = tmp;
	}

	//if (HV > 0) // haunch가 구조재로 사용될 때 
	//{
	//	rVolume += HV;
	//	CString str;
	//	if (_bShowVariable)
	//		str.Format(_T(" + %g(HAUNCH)"), HV);
	//	else
	//		str.Format(_T(" + %g"), HV);

	//	noteVolume += str;
	//}

	// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
	if (haunchVolumes.size() > 0)
	{
		for (int hi = 0; hi < haunchcount; hi++)
		{
			MSSubHaunchG* haunch = dynamic_cast<MSSubHaunchG*>(isofoot->ma_Haunches[hi]);
			vector<int> haunchCZoneIDs = haunch->m_VolumeAreaEMS.ma_CZoneID;
			int hannchmatid = haunch->GetHaunchMaterialID();
			if (hannchmatid == 0)
				hannchmatid = isofoot->GetMaterial()->m_ID;


			double haunchFactor = 0.0;
			for (int i = 0; i < haunchCZoneIDs.size(); ++i)
			{
				if (cz_id == haunchCZoneIDs[i])
					haunchFactor = haunch->m_VolumeAreaEMS.ma_Factor[i];
			}

			double haunchVolume = haunchVolumes[hi];

			CString str;
			if (fabs(haunchFactor - 1) < DTOL_GM)
			{
				if (_bShowVariable)
					str.Format(_T("%g(HAUNCH: %d)"), haunchVolume, haunch->GetID());
				else
					str.Format(_T("%g"), haunchVolume);

				haunchFactor = 1.0;
			}
			else
			{
				if (haunchFactor > 0.0)
				{
					MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
					if (curFloor != NULL)
					{
						MSConstructionZone* czone = curFloor->GetCZone(cz_id);
						if (_bShowVariable)
							str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, haunchVolume, haunch->GetID());
						else
							str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, haunchVolume);
					}
				}
			}

			//noteVolume += str;
			//rVolume += haunchVolume * haunchFactor;

			subIds.push_back(haunch->GetID());
			subVolumes.push_back(haunchVolume * haunchFactor);
			subNotes.push_back(str);
			matid.push_back(hannchmatid);

			if (isHaunchOutstretched)
			{
				double curVolume = haunchStretchedParam[hi];
				if (fabs(haunchFactor - 1) < DTOL_GM)
				{
					if (_bShowVariable)
						str.Format(_T("%g(HAUNCH: %d)"), curVolume, haunch->GetID());
					else
						str.Format(_T("%g"), curVolume);

					haunchFactor = 1.0;
				}
				else
				{
					if (haunchFactor > 0.0)
					{
						MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
						if (curFloor != NULL)
						{
							MSConstructionZone* czone = curFloor->GetCZone(cz_id);
							if (_bShowVariable)
								str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, curVolume, haunch->GetID());
							else
								str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, curVolume);
						}
					}
				}

				subIds.push_back(haunch->GetID());
				subVolumes.push_back(curVolume * haunchFactor);
				subNotes.push_back(str);
				matid.push_back(hannchmatid);
			}
		}
	}

	// Form Area

	rArea = (2 * W + 2 * D)*T - ACV + H_FORM;
	if (rArea < 0)
		ACV = ACV + rArea;

	noteArea.Format(_T("(2 * (%g) + 2 * (%g)) * %g - %g + %g"), W, D, T, ACV, H_FORM);

	bool all_same = true;
	for (int i = 0; i < (int)(mp_Owner->ma_UserFormWork.size()); ++i)
	{
		if (mp_Owner->ma_UserFormWork[i].dir == _T("B") || mp_Owner->ma_UserFormWork[i].is_burim == true)
			continue;

		for (int j = i + 1; j < (int)(mp_Owner->ma_UserFormWork.size()); ++j)
		{
			if (mp_Owner->ma_UserFormWork[j].dir == _T("B") || mp_Owner->ma_UserFormWork[j].is_burim == true)
				continue;

			if ((mp_Owner->ma_UserFormWork[i].formWorkName != mp_Owner->ma_UserFormWork[j].formWorkName))
				all_same = false;
		}
	}
	if (mp_Owner->ma_UserFormWork.size() > 0)
	{
		if (all_same)
		{
			rArea = (2 * W + 2 * D)*T - ACV + H_FORM;
			if (ACV > 0)
			{
				if (_bShowVariable)
					noteArea.Format(_T("(2 * %g(B) + 2 * %g(D)) * %g(T) - %g(ACV)"), W, D, T, ACV);
				else
					noteArea.Format(_T("(2 * %g + 2 * %g) * %g - %g"), W, D, T, ACV);
			}
			else
			{
				if (_bShowVariable)
					noteArea.Format(_T("(2 * %g(B) + 2 * %g(D)) * %g(T)"), W, D, T);
				else
					noteArea.Format(_T("(2 * %g + 2 * %g) * %g"), W, D, T);
			}

			if (H_FORM != 0)
			{
				CString str;
				if (_bShowVariable)
				{
					if (H_FORM > 0) str.Format(_T(" + %g(HAUNCH)"), fabs(H_FORM));
					else str.Format(_T(" - %g(HAUNCH)"), fabs(H_FORM));
				}
				else
				{
					if (H_FORM > 0) str.Format(_T(" + %g"), fabs(H_FORM));
					else str.Format(_T(" - %g"), fabs(H_FORM));
				}
				noteArea += str;
			}

			//버림거푸집 제외. 예전에는 버림거푸집이 없으므로 index로 찾으면 다운될 수 있음
			CString formWorkName = _T("");
			for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); i++)
			{
				if (mp_Owner->ma_UserFormWork[i].is_burim == true)
					continue;
				else
				{
					formWorkName = mp_Owner->ma_UserFormWork[i].formWorkName;
					break;
				}
			}

			if (formWorkName != _T(""))
			{
				result.formDir.push_back(_T("Side"));
				result.formWorkName.push_back(formWorkName);
				result.formArea.push_back(rArea);
				result.formNote.push_back(noteArea);
			}
		}
		else
		{
			map<CString, CString> dirToType;
			for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
			{
				if (mp_Owner->ma_UserFormWork[i].is_burim == true) continue;
				dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
			}

			for (int i = 0; i < 4; ++i)
			{
				CString cur_dir[] = { _T("0"), _T("1"), _T("2"), _T("3") };
				double cur_value[] = { W, D, W, D };
				if (dirToType.count(cur_dir[i]) > 0)
				{
					rArea = (cur_value[i])*T;
					if (i % 2 == 0)
					{
						if (_bShowVariable)
							noteArea.Format(_T("%g(B) * %g(T)"), cur_value[i], T);
						else
							noteArea.Format(_T("%g * %g"), cur_value[i], T);
					}
					else
					{
						if (_bShowVariable)
							noteArea.Format(_T("%g(D) * %g(T)"), cur_value[i], T);
						else
							noteArea.Format(_T("%g * %g"), cur_value[i], T);
					}
					//haunch 
					for (int j = 0; j < haunchcount; j++)
					{
						if (_bShowVariable)
						{
							if (haunch_param[j * 3 + 2] == i)
							{
								CString str;
								str.Format(_T("%g(HAUNCH)"), haunch_param[j * 3]);
								noteArea += _T(" + ");
								noteArea += str;
								rArea += haunch_param[j * 3];
							}
						}
						else
						{
							if (haunch_param[j * 3 + 2] == i)
							{
								CString str;
								str.Format(_T("%g"), haunch_param[j * 3]);
								noteArea += _T(" + ");
								noteArea += str;
								rArea += haunch_param[j * 3];
							}
						}
					}

					result.formDir.push_back(_T("Side"));
					result.formWorkName.push_back(dirToType[cur_dir[i]]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
			}
			if (result.formWorkName.size() > 0)
			{
				if (ACV > 0)
				{
					rArea = -ACV;
					if (_bShowVariable)
						noteArea.Format(_T("- %g(ACV)"), ACV);
					else
						noteArea.Format(_T("- %g"), ACV);

					result.formDir.push_back(_T("Side"));
					result.formWorkName.push_back(mp_Owner->ma_UserFormWork[0].formWorkName);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
			}
		}
	}

	MSIsoFootG* iso = dynamic_cast<MSIsoFootG*>(mp_Owner);
	if (iso != NULL && iso->IsDropPanel())
	{
		map<CString, CString> dirToType;
		for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
			dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;

		if (dirToType.count(_T("B")) > 0)
		{
			rArea = W * D - ACV_BOT;
			if (_bShowVariable)
				noteArea.Format(_T("%g(B) * %g(D)"), W, D);
			else
				noteArea.Format(_T("%g * %g"), W, D);
			if (ACV_BOT > 0)
			{
				CString str;
				if (_bShowVariable)
					str.Format(_T(" - %g(COL)"), ACV_BOT);
				else
					str.Format(_T(" - %g"), ACV_BOT);
				noteArea += str;
			}

			result.formDir.push_back(_T("Bot"));
			result.formWorkName.push_back(dirToType[_T("B")]);
			result.formArea.push_back(rArea);
			result.formNote.push_back(noteArea);
		}
	}

}

void VolumeAreaInfoEMS::GetResultByCZoneMat(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid)
{
	double rArea = 0;
	CString noteArea;

	MSSlabG* slab = dynamic_cast<MSSlabG*>(mp_Owner);
	if (slab == NULL)
		return;

	bool isHaunchOutstretched = false;
	MSRebarBuilding* bldg = dynamic_cast<MSRebarBuilding*>(mp_Owner->GetBuilding());
	if (bldg != nullptr)
		isHaunchOutstretched = bldg->GetQuantityOption()->m_dHaunchOutstretchedLength > 0;

	int haunchCount = slab->ma_Haunches.size();
	int haunchStretchedCnt = 0;
	if (isHaunchOutstretched)
		haunchStretchedCnt = haunchCount;

	int edgeWallCount = slab->ma_EdgeWall.size();
	int edgeWallHaunchStretchedCnt = 0;

	int edgeWallHaunchCount = 0;
	if (edgeWallCount > 0)
	{
		for (int i = 0; i < edgeWallCount; i++)
		{
			if (slab->ma_EdgeWall[i]->GetLevelDifHasHaunch())
			{
				edgeWallHaunchCount++;
				if (isHaunchOutstretched)
					edgeWallHaunchStretchedCnt++;
			}
		}
	}

	if (ma_Param.size() != 19 + haunchCount + haunchStretchedCnt + edgeWallCount + edgeWallHaunchCount + edgeWallHaunchStretchedCnt)
		return;

	double A = ma_Param[0], OL = ma_Param[1], T = ma_Param[2], IA = ma_Param[3], IL = ma_Param[4];
	double ACV = ma_Param[5], V2 = ma_Param[6], A2 = ma_Param[7], A2_END = ma_Param[8], IAC = ma_Param[9];
	double AB = ma_Param[10], TB = ma_Param[11], HUN = ma_Param[12];
	double VTR = ma_Param[13], VWL = ma_Param[14], HUNV = ma_Param[15];
	double ACV_OUT = ma_Param[16], ACV_IN = ma_Param[17], V_INTERSECT = ma_Param[18];

	// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
	vector<double> haunchParam;
	for (int i = 0; i < haunchCount; i++)
	{
		double volume = ma_Param[19 + i];
		haunchParam.push_back(volume);
	}

	vector<double> haunchStretchedParam;
	for (int i = 0; i < haunchStretchedCnt; i++)
	{
		double volume = ma_Param[19 + haunchCount + edgeWallCount + edgeWallHaunchCount + i];
		haunchStretchedParam.push_back(volume);
	}

	vector<double> edgeWallParam;
	for (int i = 0; i < edgeWallCount + edgeWallHaunchCount; i++)
	{
		double volume = 0;
		volume = ma_Param[19 + haunchCount + i];

		edgeWallParam.push_back(volume);
	}

	vector<double> edgeWallHaunchStretchedParam;
	for (int i = 0; i < edgeWallHaunchStretchedCnt; i++)
	{
		double volume = ma_Param[19 + haunchCount + edgeWallCount + edgeWallHaunchCount + haunchStretchedCnt + i];
		edgeWallHaunchStretchedParam.push_back(volume);
	}

	//tolerance
	if (OL < 0.1) OL = 0;
	if (IL < 0.1) IL = 0;
	// Concrete
	//if (IAC = 0.0) IAC = IA;

	rVolume = (A - IAC) * T - V_INTERSECT;
	if (IAC > 0)
	{
		if (_bShowVariable)
			noteVolume.Format(_T("(%g(A) - %g(IA)) * %g(T)"), A, IAC, T);
		else
			noteVolume.Format(_T("(%g - %g) * %g"), A, IAC, T);
	}
	else
	{
		if (_bShowVariable)
			noteVolume.Format(_T("%g(A) * %g(T)"), A, T);
		else
			noteVolume.Format(_T("%g * %g"), A, T);
	}

	if (V_INTERSECT > 0)
	{
		CString tmp;
		if (_bShowVariable)
			tmp.Format(_T(" - %g(V_INT)"), V_INTERSECT);
		else
			tmp.Format(_T(" - %g"), V_INTERSECT);
		noteVolume += tmp;
	}

	if (V_INTERSECT > 0)
	{
		CString tmp;
		if (_bShowVariable)
			tmp.Format(_T(" - %g(V_INT)"), V_INTERSECT);
		else
			tmp.Format(_T(" - %g"), V_INTERSECT);
		noteVolume += tmp;
	}

	if (VTR > 0)
	{
		CString tmp;
		if (_bShowVariable)
			tmp.Format(_T(" - %g(VTR)"), VTR);
		else
			tmp.Format(_T(" - %g"), VTR);
		noteVolume += tmp;
		rVolume -= VTR;
	}

	if (VWL > 0)
	{
		CString tmp;
		if (_bShowVariable)
			tmp.Format(_T(" - %g(VWL)"), VWL);
		else
			tmp.Format(_T(" - %g"), VWL);
		noteVolume += tmp;
		rVolume -= VWL;
	}

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
	}
	if (Factor == 0.0) return;

	CString czoneName;
	CString czoneNote;
	if (Factor != 1.0)
	{
		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
		MSConstructionZone* czone = curFloor->GetCZone(cz_id);
		czoneName = czone->GetName();
		rVolume *= Factor;
		czoneNote.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, noteVolume);
		noteVolume = czoneNote;
	}

	//if (HUNV > 0)
	//{
	//	CString tmp;
	//	if (_bShowVariable)
	//		tmp.Format(_T(" + %g(HAUNCH)"), HUNV);
	//	else
	//		tmp.Format(_T(" + %g"), HUNV);
	//	noteVolume += tmp;
	//	rVolume += HUNV;
	//}

	// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
	if (haunchParam.size() > 0)
	{
		for (int hi = 0; hi < haunchCount; hi++)
		{
			MSSubHaunchG* haunch = dynamic_cast<MSSubHaunchG*>(slab->ma_Haunches[hi]);
			vector<int> haunchCZoneIDs = haunch->m_VolumeAreaEMS.ma_CZoneID;
			int hannchmatid = haunch->GetHaunchMaterialID();
			if (hannchmatid == 0)
				hannchmatid = slab->GetMaterial()->m_ID;

			double haunchFactor = 0.0;
			for (int i = 0; i < haunchCZoneIDs.size(); ++i)
			{
				if (cz_id == haunchCZoneIDs[i])
					haunchFactor = haunch->m_VolumeAreaEMS.ma_Factor[i];
			}

			double haunchVolume = haunchParam[hi];

			CString str;
			if (fabs(haunchFactor - 1) < DTOL_GM)
			{
				if (_bShowVariable)
					str.Format(_T("%g(HAUNCH: %d)"), haunchVolume, haunch->GetID());
				else
					str.Format(_T("%g"), haunchVolume);

				haunchFactor = 1.0;
			}
			else
			{
				if (haunchFactor > 0.0)
				{
					MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
					if (curFloor != NULL)
					{
						MSConstructionZone* czone = curFloor->GetCZone(cz_id);
						if (_bShowVariable)
							str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, haunchVolume, haunch->GetID());
						else
							str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, haunchVolume);
					}
				}
			}

			//noteVolume += str;
			//rVolume += haunchVolume * haunchFactor;

			if (haunchFactor == 0.0)
				str.Format(_T("NoSumVolume"));

			subIds.push_back(haunch->GetID());
			subVolumes.push_back(haunchVolume * haunchFactor);
			subNotes.push_back(str);
			matid.push_back(hannchmatid);

			if (isHaunchOutstretched)
			{
				double curVolume = haunchStretchedParam[hi];
				if (fabs(haunchFactor - 1) < DTOL_GM)
				{
					if (_bShowVariable)
						str.Format(_T("%g(HAUNCH: %d)"), curVolume, haunch->GetID());
					else
						str.Format(_T("%g"), curVolume);

					haunchFactor = 1.0;
				}
				else
				{
					if (haunchFactor > 0.0)
					{
						MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
						if (curFloor != NULL)
						{
							MSConstructionZone* czone = curFloor->GetCZone(cz_id);
							if (_bShowVariable)
								str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, curVolume, haunch->GetID());
							else
								str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, curVolume);
						}
					}
				}

				subIds.push_back(haunch->GetID());
				subVolumes.push_back(curVolume * haunchFactor);
				subNotes.push_back(str);
				matid.push_back(hannchmatid);
			}
		}
	}

	//if (V2 > 0)
	//{
	//	CString tmp;
	//	if (_bShowVariable)
	//		tmp.Format(_T(" + %g(V2)"), V2);
	//	else
	//		tmp.Format(_T(" + %g"), V2);
	//	noteVolume += tmp;
	//}

	// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
	if (edgeWallParam.size() > 0)
	{
		int edgeWallParamIndex = 0;
		int edgeWallHaunchStretchedParamIndex = 0;
		for (int ei = 0; ei < edgeWallCount; ei++)
		{
			MSLineLevelDifG* edgeWall = dynamic_cast<MSLineLevelDifG*>(slab->ma_EdgeWall[ei]);
			vector<int> edgeWallCZoneIDs = edgeWall->m_VolumeAreaEMS.ma_CZoneID;

			double edgeWallFactor = 0.0;
			for (int i = 0; i < edgeWallCZoneIDs.size(); ++i)
			{
				if (cz_id == edgeWallCZoneIDs[i])
					edgeWallFactor = edgeWall->m_VolumeAreaEMS.ma_Factor[i];
			}

			if (edgeWall->GetLevelDifHasHaunch())
			{
				double edgeWallVolume = edgeWallParam[edgeWallParamIndex++];

				CString str;
				if (fabs(edgeWallFactor - 1) < DTOL_GM)
				{
					if (_bShowVariable)
						str.Format(_T("%g(HAUNCH: %d)"), edgeWallVolume, edgeWall->GetID());
					else
						str.Format(_T("%g"), edgeWallVolume);

					edgeWallFactor = 1.0;
				}
				else
				{
					if (edgeWallFactor > 0.0)
					{
						MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
						if (curFloor != NULL)
						{
							MSConstructionZone* czone = curFloor->GetCZone(cz_id);
							if (_bShowVariable)
								str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), edgeWallFactor, edgeWallVolume, edgeWall->GetID());
							else
								str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, edgeWallVolume);
						}
					}
				}

				//noteVolume += str;
				//rVolume += edgeWallVolume * edgeWallFactor;
				if (edgeWallFactor == 0.0)
					str.Format(_T("NoSumVolume"));

				if (edgeWallVolume > 0.0)
				{
					subIds.push_back(edgeWall->GetID());
					subVolumes.push_back(edgeWallVolume * edgeWallFactor);
					subNotes.push_back(str);
					matid.push_back(edgeWall->GetLevelDifHaunchMaterialID());
				}

				if (isHaunchOutstretched)
				{
					double curVolume = edgeWallHaunchStretchedParam[edgeWallHaunchStretchedParamIndex++];
					if (fabs(edgeWallFactor - 1) < DTOL_GM)
					{
						if (_bShowVariable)
							str.Format(_T("%g(HAUNCH: %d)"), curVolume, edgeWall->GetID());
						else
							str.Format(_T("%g"), curVolume);

						edgeWallFactor = 1.0;
					}
					else
					{
						if (edgeWallFactor > 0.0)
						{
							MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
							if (curFloor != NULL)
							{
								MSConstructionZone* czone = curFloor->GetCZone(cz_id);
								if (_bShowVariable)
									str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), edgeWallFactor, curVolume, edgeWall->GetID());
								else
									str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, curVolume);
							}
						}
					}

					subIds.push_back(edgeWall->GetID());
					subVolumes.push_back(curVolume * edgeWallFactor);
					subNotes.push_back(str);
					matid.push_back(edgeWall->GetLevelDifHaunchMaterialID());
				}
			}

			double edgeWallVolume = edgeWallParam[edgeWallParamIndex++];

			CString str;
			if (fabs(edgeWallFactor - 1) < DTOL_GM)
			{
				if (_bShowVariable)
					str.Format(_T("%g(V2: %d)"), edgeWallVolume, edgeWall->GetID());
				else
					str.Format(_T("%g"), edgeWallVolume);

				edgeWallFactor = 1.0;
			}
			else
			{
				if (edgeWallFactor > 0.0)
				{
					MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
					if (curFloor != NULL)
					{
						MSConstructionZone* czone = curFloor->GetCZone(cz_id);
						if (_bShowVariable)
							str.Format(_T("[%s : %g] * %g(V2: %d)"), czone->GetName(), edgeWallFactor, edgeWallVolume, edgeWall->GetID());
						else
							str.Format(_T("[%s : %g] * %g"), czone->GetName(), edgeWallFactor, edgeWallVolume);
					}
				}
			}

			//noteVolume += str;
			//rVolume += edgeWallVolume * edgeWallFactor;
			if (edgeWallFactor == 0.0)
				str.Format(_T("NoSumVolume"));

			if (edgeWallVolume > 0.0)
			{
				subIds.push_back(edgeWall->GetID());
				subVolumes.push_back(edgeWallVolume * edgeWallFactor);
				subNotes.push_back(str);
				matid.push_back(slab->GetMaterial()->m_ID);
			}

			//edgeWallParamIndex++;
		}
	}

	// Form Area

	//rArea = (OL*T - ACV_OUT) + (IL*T - ACV_IN) + A2 + A2_END;
	//noteArea.Format(_T("(%g * %g - %g) + (%g * %g - %g) + %g + %g"), OL, T,ACV_OUT, IL, T, ACV_IN, A2, A2_END);

	if (mp_Owner->ma_UserFormWork.size() > 0)
	{
		map<CString, CString> dirToType;
		for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
			dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;

		if (dirToType.count(_T("V")) > 0)
		{
			rArea = (OL*T - ACV_OUT + IL * T - ACV_IN) + A2 + A2_END;
			//noteArea.Format(_T("(%g x %g) + (%g x %g)"), OL, T, IL, T);

			if (ACV_OUT > 0)
			{
				if (_bShowVariable)
					noteArea.Format(_T("%g(OL) * %g(T) - %g(ACV_OUT)"), OL, T, ACV_OUT);
				else
					noteArea.Format(_T("%g * %g - %g"), OL, T, ACV_OUT);
			}
			else
			{
				if (_bShowVariable)
					noteArea.Format(_T("%g(OL) * %g(T)"), OL, T);
				else
					noteArea.Format(_T("%g * %g"), OL, T);
			}

			if (IL > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" + (%g(IL) * %g(T))"), IL, T);
				else
					tmp.Format(_T(" + (%g * %g)"), IL, T);

				if (ACV_IN > 0)
				{
					if (_bShowVariable)
						tmp.Format(_T(" + (%g(IL) * %g(T) - %g(ACV_IN))"), IL, T, ACV_IN);
					else
						tmp.Format(_T(" + (%g * %g - %g)"), IL, T, ACV_IN);
				}

				noteArea += tmp;
			}

			if (A2 > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" + %g(A2)"), A2);
				else
					tmp.Format(_T(" + %g"), A2);
				noteArea += tmp;
			}
			if (A2_END > 0)
			{
				CString tmp;
				if (_bShowVariable)
					tmp.Format(_T(" + %g(A2_END)"), A2_END);
				else
					tmp.Format(_T(" + %g"), A2_END);
				noteArea += tmp;
			}
			if (HUN != 0)
			{
				rArea += HUN;
				CString str;
				if (_bShowVariable)
				{
					if (HUN > 0) str.Format(_T(" + %g(HAUNCH)"), fabs(HUN));
					else str.Format(_T(" - %g(HAUNCH)"), fabs(HUN));
				}
				else
				{
					if (HUN > 0) str.Format(_T(" + %g"), fabs(HUN));
					else str.Format(_T(" - %g"), fabs(HUN));
				}
				noteArea += str;
			}

			if (Factor != 1.0)
			{
				rArea *= Factor;
				czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
				noteArea = czoneNote;
			}

			if (rArea > 0)
			{
				result.formDir.push_back(_T("Ver"));
				result.formWorkName.push_back(dirToType[_T("V")]);
				result.formArea.push_back(rArea);
				result.formNote.push_back(noteArea);
			}
		}
	}

}
void VolumeAreaInfoEMS::GetResultByCZoneWallFoot(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid)
{
	double rArea = 0;
	CString noteArea;

	MSWallFootG* wallfoot = dynamic_cast<MSWallFootG*>(mp_Owner);
	if (wallfoot == NULL) return;

	bool isHaunchOutstretched = mp_Owner->GetBuilding()->GetQuantityOption()->m_dHaunchOutstretchedLength > 0;

	int haunchcount = 0;
	for (int i = 0; i < wallfoot->ma_Haunches.size(); i++)
	{
		MSSubHaunchG* haunch = wallfoot->ma_Haunches[i];
		//if (haunch->m_bUseHaunchMaterial == false)
		haunchcount++; //구조재로 쓰이는 경우에만 여기에서 처리
	}

	int haunchStretchedCnt = 0;
	if (isHaunchOutstretched)
		haunchStretchedCnt = haunchcount;

	if (ma_Param.size() != 9 + 3 * haunchcount + haunchStretchedCnt) return;

	double W = ma_Param[0], H = ma_Param[1], CL = ma_Param[2], LL = ma_Param[3], RL = ma_Param[4];
	double TS = ma_Param[5], ACV = ma_Param[6];
	double IE1 = ma_Param[7], IE2 = ma_Param[8];

	vector<double> haunch_param;
	for (int i = 0; i < haunchcount; i++)
	{
		haunch_param.push_back(ma_Param[9 + i * 3]);
		haunch_param.push_back(ma_Param[9 + i * 3 + 1]);
		haunch_param.push_back(ma_Param[9 + i * 3 + 2]);
	}

	vector<double> haunchVolumes;
	double HV = 0;
	double H_FORM = 0;
	for (int i = 0; i < haunchcount; i++)
	{
		H_FORM += haunch_param[i * 3];
		HV += haunch_param[i * 3 + 1];
		haunchVolumes.push_back(haunch_param[i * 3 + 1]);
	}

	vector<double> haunchStretchedParam;
	for (int i = 0; i < haunchStretchedCnt; i++)
	{
		double volume = ma_Param[9 + 3 * haunchcount + i];
		haunchStretchedParam.push_back(volume);
	}


	//실제 내용
	// BH-4478 콘크리트 물량 계산시 사용되는 길이값을 LL과 RL의 평균으로 계산하면 벽에 기둥이 맞닿아있는 경우 잘못된 물량이 계산된다.
	double LAvg = CL;
	//double LAvg = (LL + RL) / 2.0;
	LAvg = Round(LAvg);

	CString strH_TS;
	if (_bShowVariable)
	{
		if (TS > 0)
			strH_TS.Format(_T("%g(H) - %g(TS)"), H, TS);
		else
			strH_TS.Format(_T("%g(H)"), H);
	}
	else
	{
		if (TS > 0)
			strH_TS.Format(_T("%g - %g"), H, TS);
		else
			strH_TS.Format(_T("%g"), H);
	}
	// Concrete
	rVolume = (H - TS)*W*LAvg;
	rVolume = Round(rVolume);
	if (_bShowVariable)
		noteVolume.Format(_T("%g(B) * %s * %g(L')"), W, strH_TS, LAvg);
	else
		noteVolume.Format(_T("%g * %s * %g"), W, strH_TS, LAvg);

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	double listFacotr = 0.0;
	int de_cz_id;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
			de_cz_id = ma_CZoneID[ma_CZoneID.size() - 1];
			listFacotr = 1.0 - Factor;
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
		de_cz_id = 0;
		listFacotr = 0.0;
	}
	if (Factor == 0.0) return;

	CString czoneName;
	CString czoneNote;
	CString d_czone_Name;
	CString d_czone_Note;
	if (Factor != 1.0)
	{
		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
		MSConstructionZone* czone = curFloor->GetCZone(cz_id);
		MSConstructionZone* dczone = curFloor->GetCZone(de_cz_id);
		czoneName = czone->GetName();
		d_czone_Name = dczone->GetName();
		rVolume *= Factor;
		czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteVolume);
		noteVolume = czoneNote;
	}

	// Haunch (same matl)
	//if (HV > 0)
	//{
	//	rVolume += HV;
	//	CString str;
	//	if (_bShowVariable)
	//		str.Format(_T(" + %g(HV)"), HV);
	//	else
	//		str.Format(_T(" + %g"), HV);
	//	noteVolume += str;
	//}

	// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
	if (haunchVolumes.size() > 0)
	{
		vector<int> list_haunch_czid;
		vector<double> haunchFactor;
		if (haunchVolumes.size() > 0)
		{
			for (int hi = 0; hi < haunchcount; hi++)
			{
				MSSubHaunchG* haunch = dynamic_cast<MSSubHaunchG*>(wallfoot->ma_Haunches[hi]);
				vector<int> haunchCZoneIDs = haunch->m_VolumeAreaEMS.ma_CZoneID;
				haunchFactor = haunch->m_VolumeAreaEMS.ma_Factor;
				double haunchVolume = haunchVolumes[hi];
				int hannchmatid = haunch->GetHaunchMaterialID();
				if (hannchmatid == 0)
					hannchmatid = wallfoot->GetMaterial()->m_ID;
				int cur_cz_id = -1;
				CString str;
				CString str_1;
				for (int i = 0; i < haunchCZoneIDs.size(); ++i)
				{
					bool czone_one = false;
					if (cz_id == haunchCZoneIDs[i])
						cz_id = haunchCZoneIDs[i];
					else
						czone_one = true;

					cur_cz_id = haunchCZoneIDs[i];
					haunchFactor[i] = haunch->m_VolumeAreaEMS.ma_Factor[i];

					if (fabs(haunchFactor[i] - 1) < DTOL_GM)
					{
						if (_bShowVariable && czone_one == false)
							str.Format(_T("%g(HAUNCH: %d)"), haunchVolume, haunch->GetID());
						else
						{
							MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
							MSConstructionZone* czone = curFloor->GetCZone(cur_cz_id);
							str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor[i], haunchVolume, haunch->GetID());
						}
						haunchFactor[i] = 1.0;
					}
					else if (czone_one == true && haunchFactor[i] == 1)
					{

					}
					else
					{
						if (haunchFactor[i] > 0.0)
						{
							MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
							if (curFloor != NULL)
							{
								MSConstructionZone* czone = curFloor->GetCZone(cz_id);
								if (czone_one == true)
								{
									czone = curFloor->GetCZone(cur_cz_id);
								}
								str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor[i], haunchVolume, haunch->GetID());
							}
						}
					}
					subIds.push_back(haunch->GetID());
					subVolumes.push_back(haunchVolume * haunchFactor[i]);
					subNotes.push_back(str);
					matid.push_back(hannchmatid);

					if (isHaunchOutstretched)
					{
						double curVolume = haunchStretchedParam[hi];
						if (fabs(haunchFactor[i] - 1) < DTOL_GM)
						{
							if (_bShowVariable)
								str.Format(_T("%g(HAUNCH: %d)"), curVolume, haunch->GetID());
							else
								str.Format(_T("%g"), curVolume);

							haunchFactor[i] = 1.0;
						}
						else
						{
							if (haunchFactor[i] > 0.0)
							{
								MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
								if (curFloor != NULL)
								{
									MSConstructionZone* czone = curFloor->GetCZone(cz_id);
									if (_bShowVariable)
										str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, curVolume, haunch->GetID());
									else
										str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, curVolume);
								}
							}
						}

						subIds.push_back(haunch->GetID());
						subVolumes.push_back(curVolume * haunchFactor[i]);
						subNotes.push_back(str);
						matid.push_back(hannchmatid);
					}
				}
			}
		}
	}

	// Form Area (LR)
	rArea = (H - TS)*LAvg * 2;
	rArea = Round(rArea);
	if (_bShowVariable)
		noteArea.Format(_T("%s * %g(L') * 2"), strH_TS, LAvg);
	else
		noteArea.Format(_T("%s * %g * 2"), strH_TS, LAvg);

	if (mp_Owner->ma_UserFormWork.size() > 0)
	{
		map<CString, CString> dirToType;
		for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
		{
			if (mp_Owner->ma_UserFormWork[i].is_burim == true) continue;
			dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
		}

		bool bLRSame = (dirToType.count(_T("L")) > 0 && dirToType.count(_T("L")) > 0 && dirToType[_T("L")] == dirToType[_T("R")]);

		if (bLRSame)
		{
			rArea = (H - TS)*(LL + RL) - ACV + H_FORM;
			rArea = Round(rArea);
			if (rArea < 0) rArea = 0;
			if (_bShowVariable)
				noteArea.Format(_T("%s * (%g(LL) + %g(RL))"), strH_TS, LL, RL);
			else
				noteArea.Format(_T("%s * (%g + %g)"), strH_TS, LL, RL);

			CString tmpACV;
			if (_bShowVariable)
				tmpACV.Format(_T(" - %g(ACV)"), ACV);
			else
				tmpACV.Format(_T(" - %g"), ACV);

			CString tmpH_FORM;
			if (_bShowVariable)
			{
				if (H_FORM > 0) tmpH_FORM.Format(_T(" + %g(HAUNCH)"), fabs(H_FORM));
				else tmpH_FORM.Format(_T(" - %g(HAUNCH)"), fabs(H_FORM));
			}
			else
			{
				if (H_FORM > 0) tmpH_FORM.Format(_T(" + %g"), fabs(H_FORM));
				else tmpH_FORM.Format(_T(" - %g"), fabs(H_FORM));
			}


			if (ACV > 0) noteArea += tmpACV;
			if (H_FORM != 0) noteArea += tmpH_FORM;


			if (Factor != 1.0)
			{
				rArea *= Factor;
				czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
				noteArea = czoneNote;
			}

			result.formDir.push_back(_T("Side"));
			result.formWorkName.push_back(dirToType[_T("L")]);
			result.formArea.push_back(rArea);
			result.formNote.push_back(noteArea);

		}
		else
		{
			for (int i = 0; i < 2; ++i)
			{
				CString cur_dir = (i == 0 ? _T("L") : _T("R"));
				double LCur = (i == 0 ? LL : RL);
				if (dirToType.count(cur_dir) > 0)
				{
					rArea = (H - TS)*LCur - ACV / 2.0;
					rArea = Round(rArea);
					if (rArea < 0) rArea = 0;
					if (_bShowVariable)
					{
						if (i == 0)
							noteArea.Format(_T("%s * %g(LL)"), strH_TS, LCur);
						else
							noteArea.Format(_T("%s * %g(RL)"), strH_TS, LCur);
					}
					else
						noteArea.Format(_T("%s * %g"), strH_TS, LCur);
					CString tmp;
					if (_bShowVariable)
						tmp.Format(_T(" - %g(ACV) / 2"), ACV);
					else
						tmp.Format(_T(" - %g / 2"), ACV);
					if (ACV > 0) noteArea += tmp;


					//haunch
					for (int j = 0; j < haunchcount; j++)
					{
						if (_bShowVariable)
						{
							if (haunch_param[j * 3 + 2] == i)
							{
								CString str;
								str.Format(_T("%g(HAUNCH)"), haunch_param[j * 3]);
								noteArea += _T(" + ");
								noteArea += str;
								rArea += haunch_param[j * 3];
							}
						}
						else
						{
							if (haunch_param[j * 3 + 2] == i)
							{
								CString str;
								str.Format(_T("%g"), haunch_param[j * 3]);
								noteArea += _T(" + ");
								noteArea += str;
								rArea += haunch_param[j * 3];
							}
						}
					}

					if (cur_dir == _T("L"))
						result.formDir.push_back(_T("Left"));
					else
						result.formDir.push_back(_T("Right"));

					if (Factor != 1.0)
					{
						rArea *= Factor;
						czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
						noteArea = czoneNote;

					}
					result.formWorkName.push_back(dirToType[cur_dir]);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
			}
		}

		if (IE1 > 0.0 || IE2 > 0.0 && haunchcount == 0)// 줄기초가 시공존일때는 서브헌치 물량이 출력 되서는 안되기 떄문에 해당 조건문으로 변경
		{
			CString type1 = 0, type2 = 0;
			double area1 = 0, area2 = 0;
			CString end_note1, end_note2;
			if (IE1 > 0.0)
			{
				rArea = (H - TS)*W;
				rArea = Round(rArea);
				if (_bShowVariable)
					noteArea.Format(_T("%s * %g(B)"), strH_TS, W);
				else
					noteArea.Format(_T("%s * %g"), strH_TS, W);
				if (dirToType.count(_T("F")) > 0) { type1 = dirToType[_T("F")]; area1 = rArea; end_note1 = noteArea; }
			}
			if (IE2 > 0.0)
			{
				rArea = (H - TS)*W;
				rArea = Round(rArea);
				if (_bShowVariable)
					noteArea.Format(_T("%s * %g(B)"), strH_TS, W);
				else
					noteArea.Format(_T("%s * %g"), strH_TS, W);
				if (dirToType.count(_T("E")) > 0) { type2 = dirToType[_T("E")]; area2 = rArea; end_note2 = noteArea; }
			}

			if (area1 > 0 && area2 > 0 && type1 == type2 && haunchcount == 0) // 줄기초가 시공존일때는 서브헌치 물량이 출력 되서는 안되기 떄문에 해당 조건문으로 변경 , [DQ1-1682] 줄기초 서브헌치 마구리면 X2 다시 출력 으로 변경
			{
				end_note1.Format(_T("%s * 2"), end_note1);
				noteArea = end_note1;

				if (Factor != 1.0)
				{
					rArea = (area1 + area2) * Factor;					
					czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, end_note1);
					noteArea = czoneNote;
				}

				result.formDir.push_back(_T("Close"));
				result.formWorkName.push_back(type1);
				result.formArea.push_back(rArea);
				result.formNote.push_back(noteArea);
			}
			else
			{
				if (area1 > 0)
				{
					if (Factor != 1.0)
					{
						double drArea;
						rArea = area1 * Factor;
						czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, end_note1);
						noteArea = czoneNote;

					}

					result.formDir.push_back(_T("Close"));
					result.formWorkName.push_back(type1);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
				if (area2 > 0)
				{
					if (Factor != 1.0)
					{
						double drArea;
						rArea = area2 * Factor;
						czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, end_note2);
						noteArea = czoneNote;
					}

					result.formDir.push_back(_T("Close"));
					result.formWorkName.push_back(type2);
					result.formArea.push_back(rArea);
					result.formNote.push_back(noteArea);
				}
			}

		}

	}
}

//void VolumeAreaInfoEMS::GetResultByCZoneWallFoot(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
//{
//	double rArea = 0;
//	CString noteArea;
//
//	MSWallFootG* wallfoot = dynamic_cast<MSWallFootG*>(mp_Owner);
//	if (wallfoot == NULL) return;
//	int haunchcount = 0;
//	for (int i = 0; i < wallfoot->ma_Haunch.size(); i++)
//	{
//		MSHaunchG* haunch = wallfoot->ma_Haunch[i];
//		if (haunch->m_bUseHaunchMaterial == false)
//			haunchcount++; //구조재로 쓰이는 경우에만 여기에서 처리
//	}
//	if (ma_Param.size() != 7 + 3 * haunchcount) return;
//
//	double W = ma_Param[0], H = ma_Param[1], L = ma_Param[2], TB = ma_Param[3], ACV = ma_Param[4];
//	double IE1 = ma_Param[5], IE2 = ma_Param[6];
//
//	vector<double> haunch_param;
//	for (int i = 0; i < haunchcount; i++)
//	{
//		haunch_param.push_back(ma_Param[7 + i * 3]);
//		haunch_param.push_back(ma_Param[7 + i * 3 + 1]);
//		haunch_param.push_back(ma_Param[7 + i * 3 + 2]);
//	}
//
//	double HV = 0;
//	double H_FORM = 0;
//	for (int i = 0; i < haunchcount; i++)
//	{
//		H_FORM += haunch_param[i * 3];
//		HV += haunch_param[i * 3 + 1];
//	}
//
//	// Concrete
//	rVolume = W*H*L;
//	if (_bShowVariable)
//		noteVolume.Format(_T("%g(B) x %g(H) x %g(L)"), W, H, L);
//	else
//		noteVolume.Format(_T("%g x %g x %g"), W, H, L);
//
//	if (HV > 0)
//	{
//		rVolume = W*H*L + HV;
//		if (_bShowVariable)
//			noteVolume.Format(_T("%g(B) x %g(H) x %g(L) + %g(HAUNCH)"), W, H, L, HV);
//		else
//			noteVolume.Format(_T("%g x %g x %g + %g"), W, H, L, HV);
//	}
//
//
//	// Form Area
//	rArea = 2 * H*L - ACV + H_FORM;
//	noteArea.Format(_T("2 x %g x %g - %g + %g"), H, L, ACV, H_FORM);
//
//	if (mp_Owner->ma_UserFormWork.size() > 0)
//	{
//		map<CString, CString> dirToType;
//		for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
//		{
//			if (mp_Owner->ma_UserFormWork[i].is_burim == true) continue;
//			dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
//		}
//
//		bool bLRSame = (dirToType.count(_T("L")) > 0 && dirToType.count(_T("L")) > 0 && dirToType[_T("L")] == dirToType[_T("R")]);
//		if (bLRSame)
//		{
//			rArea = 2 * H*L - ACV + H_FORM;
//			if (ACV > 0.0)
//			{
//				if (_bShowVariable)
//					noteArea.Format(_T("2 x %g(H) x %g(L) - %g(ACV)"), H, L, ACV);
//				else
//					noteArea.Format(_T("2 x %g x %g - %g"), H, L, ACV);
//			}
//			else
//			{
//				if (_bShowVariable)
//					noteArea.Format(_T("2 x %g(H) x %g(L)"), H, L);
//				else
//					noteArea.Format(_T("2 x %g x %g"), H, L);
//			}
//
//			if (H_FORM != 0)
//			{
//				CString str;
//				if (_bShowVariable)
//				{
//					if (H_FORM > 0) str.Format(_T(" + %g(HAUNCH)"), fabs(H_FORM));
//					else str.Format(_T(" - %g(HAUNCH)"), fabs(H_FORM));
//				}
//				else
//				{
//					if (H_FORM > 0) str.Format(_T(" + %g"), fabs(H_FORM));
//					else str.Format(_T(" - %g"), fabs(H_FORM));
//				}
//				noteArea += str;
//			}
//			result.formDir.push_back(_T("Side"));
//			result.formWorkName.push_back(dirToType[_T("L")]);
//			result.formArea.push_back(rArea);
//			result.formNote.push_back(noteArea);
//		}
//		else
//		{
//			for (int i = 0; i<2; ++i)
//			{
//				CString cur_dir = (i == 0 ? _T("L") : _T("R"));
//				if (dirToType.count(cur_dir) > 0)
//				{
//					rArea = (H)*L - 0.5*ACV;
//					if (ACV > 0.0)
//					{
//						if (_bShowVariable)
//							noteArea.Format(_T("%g(H) x %g(L) - (%g(ACV))/2"), H, L, ACV);
//						else
//							noteArea.Format(_T("%g x %g - (%g)/2"), H, L, ACV);
//					}
//					else
//					{
//						if (_bShowVariable)
//							noteArea.Format(_T("%g(H) x %g(L)"), H, L);
//						else
//							noteArea.Format(_T("%g x %g"), H, L);
//					}
//					//haunch
//					for (int j = 0; j < haunchcount; j++)
//					{
//						if (_bShowVariable)
//						{
//							if (haunch_param[j * 3 + 2] == i)
//							{
//								CString str;
//								str.Format(_T("%g(HAUNCH)"), haunch_param[j * 3]);
//								noteArea += _T(" + ");
//								noteArea += str;
//								rArea += haunch_param[j * 3];
//							}
//						}
//						else
//						{
//							if (haunch_param[j * 3 + 2] == i)
//							{
//								CString str;
//								str.Format(_T("%g"), haunch_param[j * 3]);
//								noteArea += _T(" + ");
//								noteArea += str;
//								rArea += haunch_param[j * 3];
//							}
//						}
//					}
//					if (cur_dir == _T("L"))
//						result.formDir.push_back(_T("Left"));
//					else
//						result.formDir.push_back(_T("Right"));
//					result.formWorkName.push_back(dirToType[cur_dir]);
//					result.formArea.push_back(rArea);
//					result.formNote.push_back(noteArea);
//				}
//			}
//		}
//
//		if (IE1 > 0.0 || IE2 > 0.0)
//		{
//			CString type1 = 0, type2 = 0;
//			double area1 = 0, area2 = 0;
//			CString end_note1, end_note2;
//			if (IE1 > 0.0)
//			{
//				rArea = (H)*W;
//				if (_bShowVariable)
//					noteArea.Format(_T("%g(H) x %g(B)"), H, W);
//				else
//					noteArea.Format(_T("(%g) x %g"), H, W);
//				if (dirToType.count(_T("F")) > 0) { type1 = dirToType[_T("F")]; area1 = rArea; end_note1 = noteArea; }
//			}
//			if (IE2 > 0.0)
//			{
//				rArea = (H)*W;
//				if (_bShowVariable)
//					noteArea.Format(_T("%g(H) x %g(B)"), H, W);
//				else
//					noteArea.Format(_T("(%g) x %g"), H, W);
//				if (dirToType.count(_T("E")) > 0) { type2 = dirToType[_T("E")]; area2 = rArea; end_note2 = noteArea; }
//			}
//
//			if (area1 > 0 && area2 > 0 && type1 == type2)
//			{
//				result.formDir.push_back(_T("Close")); result.formWorkName.push_back(type1); result.formArea.push_back(area1 + area2); result.formNote.push_back(end_note1 + _T(" x 2"));
//			}
//			else
//			{
//				if (area1 > 0) { result.formDir.push_back(_T("Close")); result.formWorkName.push_back(type1); result.formArea.push_back(area1); result.formNote.push_back(end_note1); }
//				if (area2 > 0) { result.formDir.push_back(_T("Close")); result.formWorkName.push_back(type2); result.formArea.push_back(area2); result.formNote.push_back(end_note2); }
//			}
//
//		}
//	}
//
//}

void VolumeAreaInfoEMS::GetResultByCZonePit(int cz_id, double &rVolume, CString& noteVolume, FormResult& result, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>& matid)
{
	double rArea = 0;
	CString noteArea;

	MSPitG* pitG = dynamic_cast<MSPitG*>(mp_Owner);
	if (pitG == NULL) return;

	bool isHaunchOutstretched = mp_Owner->GetBuilding()->GetQuantityOption()->m_dHaunchOutstretchedLength > 0;
	int haunchCount = pitG->ma_Haunches.size();

	int haunchStretchedCnt = 0;
	if (isHaunchOutstretched)
		haunchStretchedCnt = haunchCount;

	if (ma_Param.size() != 12 + haunchCount + haunchStretchedCnt)
		return;

	double A = ma_Param[0], OL = ma_Param[1], H1 = ma_Param[2], PA = ma_Param[3], POL = ma_Param[4];
	double H2 = ma_Param[5], WA = ma_Param[6], WOL = ma_Param[7], H3 = ma_Param[8], ACV = ma_Param[9], HUN = ma_Param[10], HUNV = ma_Param[11];

	// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
	map<int, double> haunchParam;
	for (int i = 0; i < haunchCount; i++)
	{
		double volume = ma_Param[12 + i];
		haunchParam.insert(make_pair(i, volume));
	}

	vector<double> haunchStretchedParam;
	for (int i = 0; i < haunchStretchedCnt; i++)
	{
		double volume = ma_Param[12 + haunchCount + i];
		haunchStretchedParam.push_back(volume);
	}

	// Concrete
	rVolume = (A*H1) - (PA*H2) - (WA*H3);
	if (_bShowVariable)
	{
		if (WA > 0)
			noteVolume.Format(_T("(%g(A) * %g(H1)) - (%g(PA) * %g(H2)) - (%g(WA) * %g(H3))"), A, H1, PA, H2, WA, H3);
		else
			noteVolume.Format(_T("(%g(A) * %g(H1)) - (%g(PA) * %g(H2))"), A, H1, PA, H2);
	}
	else
	{
		if (WA > 0)
			noteVolume.Format(_T("(%g * %g) - (%g * %g) - (%g * %g)"), A, H1, PA, H2, WA, H3);
		else
			noteVolume.Format(_T("(%g * %g) - (%g * %g)"), A, H1, PA, H2);
	}

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
	}
	if (Factor == 0.0) return;

	if (Factor != 1.0)
	{
		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
		MSConstructionZone* czone = curFloor->GetCZone(cz_id);
		CString tmp;
		rVolume *= Factor;
		tmp.Format(_T("[%s : %g] * %s"), czone->GetName(), Factor, noteVolume);
		noteVolume = tmp;
	}

	//if (HUNV > 0) // haunch가 구조재로 사용될 때 
	//{
	//	rVolume += HUNV;
	//	CString str;
	//	if (_bShowVariable)
	//		str.Format(_T(" + %g(HAUNCH)"), HUNV);
	//	else
	//		str.Format(_T(" + %g"), HUNV);
	//	
	//	noteVolume += str;
	//}

	// DQ1-126 시공구역 물량 개선 - 헌치가 각각 시공존을 갖도록 수정
	if (haunchParam.size() > 0)
	{
		for (int hi = 0; hi < haunchCount; hi++)
		{
			MSSubHaunchG* haunch = dynamic_cast<MSSubHaunchG*>(pitG->ma_Haunches[hi]);
			vector<int> haunchCZoneIDs = haunch->m_VolumeAreaEMS.ma_CZoneID;
			int hannchmatid = haunch->GetHaunchMaterialID();
			if (hannchmatid == 0)
				hannchmatid = pitG->GetMaterial()->m_ID;

			double haunchFactor = 0.0;
			for (int i = 0; i < haunchCZoneIDs.size(); ++i)
			{
				if (cz_id == haunchCZoneIDs[i])
					haunchFactor = haunch->m_VolumeAreaEMS.ma_Factor[i];
			}

			double haunchVolume = haunchParam[hi];

			CString str;
			if (fabs(haunchFactor - 1) < DTOL_GM)
			{
				if (_bShowVariable)
					str.Format(_T("%g(HAUNCH: %d)"), haunchVolume, haunch->GetID());
				else
					str.Format(_T("%g"), haunchVolume);

				haunchFactor = 1.0;
			}
			else
			{
				if (haunchFactor > 0.0)
				{
					MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
					if (curFloor != NULL)
					{
						MSConstructionZone* czone = curFloor->GetCZone(cz_id);
						if (_bShowVariable)
							str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, haunchVolume, haunch->GetID());
						else
							str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, haunchVolume);
					}
				}
			}

			//noteVolume += str;
			//rVolume += haunchVolume * haunchFactor;
			subIds.push_back(haunch->GetID());
			subVolumes.push_back(haunchVolume * haunchFactor);
			subNotes.push_back(str);
			matid.push_back(hannchmatid);

			if (isHaunchOutstretched)
			{
				double curVolume = haunchStretchedParam[hi];
				if (fabs(haunchFactor - 1) < DTOL_GM)
				{
					if (_bShowVariable)
						str.Format(_T("%g(HAUNCH: %d)"), curVolume, haunch->GetID());
					else
						str.Format(_T("%g"), curVolume);

					haunchFactor = 1.0;
				}
				else
				{
					if (haunchFactor > 0.0)
					{
						MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
						if (curFloor != NULL)
						{
							MSConstructionZone* czone = curFloor->GetCZone(cz_id);
							if (_bShowVariable)
								str.Format(_T("[%s : %g] * %g(HAUNCH: %d)"), czone->GetName(), haunchFactor, curVolume, haunch->GetID());
							else
								str.Format(_T("[%s : %g] * %g"), czone->GetName(), haunchFactor, curVolume);
						}
					}
				}

				subIds.push_back(haunch->GetID());
				subVolumes.push_back(curVolume * haunchFactor);
				subNotes.push_back(str);
				matid.push_back(hannchmatid);
			}
		}
	}

	// Form Area
	rArea = (OL*H1) + (POL*H2) + (WOL*H3) - ACV + HUN;
	noteArea.Format(_T("(%g * %g) + (%g * %g) + (%g * %g) - %g + %g"), OL, H1, POL, H2, WOL, H3, ACV, HUN);

	if (mp_Owner->ma_UserFormWork.size() > 0)
	{
		map<CString, CString> dirToType;
		for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
			dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
		if (dirToType.count(_T("V")) > 0) //Outer
		{
			rArea = (OL*H1) - ACV;
			if (_bShowVariable)
				noteArea.Format(_T("%g(OL) * %g(H1) - %g(ACV)"), OL, H1, ACV);
			else
				noteArea.Format(_T("%g * %g - %g"), OL, H1, ACV);

			if (HUN != 0)
			{
				rArea += HUN;
				CString str;
				if (_bShowVariable)
				{
					if (HUN > 0) str.Format(_T(" + %g(HAUNCH)"), fabs(HUN));
					else str.Format(_T(" - %g(HAUNCH)"), fabs(HUN));
				}
				else
				{
					if (HUN > 0) str.Format(_T(" + %g"), fabs(HUN));
					else str.Format(_T(" - %g"), fabs(HUN));
				}
				noteArea += str;
			}
			result.formDir.push_back(_T("Ver"));
			result.formWorkName.push_back(dirToType[_T("V")]); result.formArea.push_back(rArea); result.formNote.push_back(noteArea);
		}

		if (dirToType.count(_T("I")) > 0) //Inner
		{
			rArea = (POL*H2) + (WOL*H3);
			if (_bShowVariable)
			{
				if (WOL > 0)
					noteArea.Format(_T("(%g(POL) * %g(H2)) + (%g(WOL) * %g(H3))"), POL, H2, WOL, H3);
				else
					noteArea.Format(_T("%g(POL) * %g(H2)"), POL, H2);
			}
			else
			{
				if (WOL > 0)
					noteArea.Format(_T("(%g * %g) + (%g * %g)"), POL, H2, WOL, H3);
				else
					noteArea.Format(_T("%g * %g"), POL, H2);
			}
			result.formDir.push_back(_T("Ver"));
			result.formWorkName.push_back(dirToType[_T("I")]); result.formArea.push_back(rArea); result.formNote.push_back(noteArea);
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneStair(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
{
	MSGenStairG* stair = dynamic_cast<MSGenStairG*>(mp_Owner);
	if (stair == NULL) return;
	int landingCount = stair->ma_Landing.GetSize();
	int stepCount = stair->ma_Step.GetSize();
	if (ma_Param.size() != landingCount * 2 + stepCount * 5) return;

	map<int, vector<double>> landingParam;
	map<int, vector<double>> stepParam;

	for (int i = 0; i < landingCount; i++)
	{//CA , T 
		vector<double> tmpParam;
		for (int j = 0; j < 2; j++)
			tmpParam.push_back(ma_Param[i * 2 + j]);

		landingParam.insert(make_pair(i, tmpParam));
	}

	for (int i = 0; i < stepCount; i++)
	{//stepN,  L, W, H, T
		vector<double> tmpParam;
		for (int j = 0; j < 5; j++)
			tmpParam.push_back(ma_Param[landingCount * 2 + i * 5 + j]);

		stepParam.insert(make_pair(i, tmpParam));
	}

	rVolume = 0;
	noteVolume = "";

	struct StairQuantity
	{
		CString category; //L:참 S: 스텝 SL:경사 V:부피 A:거푸집 
		double value;
		CString note;
		int num;
	};
	vector<StairQuantity> totalV;

	//Concrete 물량 : 참 + 계단 (경사 포함)
	map<int, vector<double>>::iterator landing_it;
	for (landing_it = landingParam.begin(); landing_it != landingParam.end(); landing_it++)
	{
		double tmplanding = 0;
		CString tmpnote;
		StairQuantity tmpquant;

		double LA = Round(landing_it->second[0]);
		double LT = Round(landing_it->second[1]);

		tmplanding = Round(LA * LT);
		if (_bShowVariable)
			tmpnote.Format(_T("%s : ((%g(LA) * %g(LT) = %g)"), LocalDic::GetLocal(_T("계단참"), _T("Quantity")), LA, LT, tmplanding);
		else
			tmpnote.Format(_T("%s : ((%g * %g = %g)"), LocalDic::GetLocal(_T("계단참"), _T("Quantity")), LA, LT, tmplanding);

		tmpquant.category = _T("LV");
		tmpquant.value = tmplanding;
		tmpquant.note = tmpnote;
		tmpquant.num = 1;
		totalV.push_back(tmpquant);
	}

	map<int, vector<double>>::iterator step_it;
	for (step_it = stepParam.begin(); step_it != stepParam.end(); step_it++)
	{
		double tmpslope = 0;
		CString tmpnote;
		StairQuantity tmpquant;

		double SN = Round(step_it->second[0], 0);
		double SL = Round(step_it->second[1]);
		double SW = Round(step_it->second[2]);
		double SH = Round(step_it->second[3]);
		double ST = Round(step_it->second[4]);

		if (SN > 0)
		{
			double slope_leng = 0;
			double  avg_step_thick = 0;
			tmpnote = _T("");

			slope_leng = Round(sqrt(SL*SL + SH * SH));
			avg_step_thick = Round(ST + 0.5* (SH / SN)*(SL / SN) / (sqrt((SH / SN)*(SH / SN) + (SL / SN)*(SL / SN)))); //계단두께 + 평균step 두께/2
			tmpslope = Round(slope_leng * avg_step_thick * SW);

			if (_bShowVariable)
				tmpnote.Format(_T("%s : ((%g(%s) * %g(%s) * %g(%s) = %g)"), LocalDic::GetLocal(_T("계단"), _T("Quantity")), slope_leng, LocalDic::GetLocal(_T("경사길이"), _T("Quantity")), avg_step_thick, LocalDic::GetLocal(_T("계단평균두께"), _T("Quantity")), SW, LocalDic::GetLocal(_T("계단폭"), _T("Quantity")), tmpslope);
			else
				tmpnote.Format(_T("%s : ((%g * %g * %g = %g)"), LocalDic::GetLocal(_T("계단"), _T("Quantity")), slope_leng, avg_step_thick, SW, tmpslope);

			tmpquant.category = _T("SV");
			tmpquant.value = tmpslope;
			tmpquant.note = tmpnote;
			tmpquant.num = 1;
			totalV.push_back(tmpquant);
		}
	}

	//동일한 요소들 정리 : 콘크리트
	for (int i = 0; i < totalV.size(); i++)
	{
		if (totalV[i].num == 0) continue;
		int count = 0;
		for (int j = i + 1; j < totalV.size(); j++)
		{
			if (totalV[i].note == totalV[j].note)
			{
				totalV[j].num = 0;
				count++;
			}
		}
		totalV[i].num += count;
		totalV[i].value *= totalV[i].num;
		CString tmp;
		tmp.Format(_T(" * %d = %g)"), totalV[i].num, totalV[i].value);
		totalV[i].note = totalV[i].note + tmp;
	}

	for (int i = (int)totalV.size() - 1; i >= 0; i--)
	{
		if (totalV[i].num == 0)
			totalV.erase(totalV.begin() + i);
	}

	/*for (int i = 0; i < totalV.size(); i++)
	{
		CString str;
		str.Format(_T("category: %s , value:%g, note:%s, num:%d"), totalV[i].category, totalV[i].value, totalV[i].note, totalV[i].num);
		StatusReporterWrapper::OutputMessage(str);
	}*/

	for (int i = 0; i < totalV.size(); i++)
	{
		rVolume += Round(totalV[i].value, 3);
		noteVolume += totalV[i].note;
		if (i < totalV.size() - 1)
		{
			noteVolume += char(10);
			noteVolume += _T(" + ");
		}
	}

	//FormWork
	vector<StairQuantity> totalA;
	double rArea = 0;
	CString noteArea;
	if (mp_Owner->ma_UserFormWork.size() > 0)
	{
		map<CString, CString> dirToType;
		for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
			dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
		if (dirToType.count(_T("W")) + dirToType.count(_T("X")) + dirToType.count(_T("Z")) > 0)
		{ //landing 바닥 : z

			for (landing_it = landingParam.begin(); landing_it != landingParam.end(); landing_it++)
			{
				StairQuantity tmpquant;
				// 거푸집 (계단참(랜딩 바닥)) : Z
				double LA = Round(landing_it->second[0]);
				rArea = LA;

				if (_bShowVariable)
					noteArea.Format(_T("%s : %g(LA)"), LocalDic::GetLocal(_T("계단참 밑판"), _T("Quantity")), LA);
				else
					noteArea.Format(_T("%s : %g"), LocalDic::GetLocal(_T("계단참 밑판"), _T("Quantity")), LA);

				tmpquant.category = _T("LAZ");
				tmpquant.value = Round(rArea, 3);
				tmpquant.note = noteArea;
				tmpquant.num = 1;
				totalA.push_back(tmpquant);

				//거푸집 (참 옆면) RH-1840 : Y
				/*rArea = (CL1*T1 + CL2*T2);
				if (_bShowVariable)
				noteArea.Format(_T("참 옆면: %g(CL1) x %g(T1) + %g(CL2) x %g(T2)"), CL1, T1, CL2, T2);
				else
				noteArea.Format(_T("참 옆면: %g x %g + %g x %g"), CL1, T1, CL2, T2);*/
			}

			for (step_it = stepParam.begin(); step_it != stepParam.end(); step_it++)
			{
				double SN = Round(step_it->second[0], 0);
				double SL = Round(step_it->second[1]);
				double SW = Round(step_it->second[2]);
				double SH = Round(step_it->second[3]);
				double ST = Round(step_it->second[4]);
				double slope_leng = Round(sqrt(SL*SL + SH * SH));
				double SD = Round(SL / (SN - 1)); // 디딤판 춤

				if (SN > 0)
				{
					StairQuantity tmpquant;
					{
						// 거푸집 (계단부 - 챌판1) 챌판 : X
						rArea = Round(SH / SN * SW * SN);

						if (_bShowVariable)
							noteArea.Format(_T("%s : %g(%s) * %g(%s) * %g(%s)"), LocalDic::GetLocal(_T("계단 챌판"), _T("Quantity")), Round(SH / SN), LocalDic::GetLocal(_T("챌판높이"), _T("Quantity")), SW, LocalDic::GetLocal(_T("계단폭"), _T("Quantity")), SN, LocalDic::GetLocal(_T("계단수"), _T("MSDBManager")));
						else
							noteArea.Format(_T("%s : %g * %g * %g"), LocalDic::GetLocal(_T("계단 챌판"), _T("Quantity")), Round(SH / SN), SW, SN);

						tmpquant.category = _T("SAX");
						tmpquant.value = rArea;
						tmpquant.note = noteArea;
						tmpquant.num = 1;
						totalA.push_back(tmpquant);
					}
					{
						// 거푸집 (계단부 - 경사면1) 밑면 : Y
						rArea = Round(slope_leng * SW);

						if (_bShowVariable)
							noteArea.Format(_T("%s : %g(%s) * %g(%s)"), LocalDic::GetLocal(_T("계단 밑판"), _T("Quantity")), slope_leng, LocalDic::GetLocal(_T("경사길이"), _T("Quantity")), SW, LocalDic::GetLocal(_T("계단폭"), _T("Quantity")));
						else
							noteArea.Format(_T("%s : %g * %g"), LocalDic::GetLocal(_T("계단 밑판"), _T("Quantity")), slope_leng, SW);

						tmpquant.category = _T("SLAX");
						tmpquant.value = rArea;
						tmpquant.note = noteArea;
						tmpquant.num = 1;
						totalA.push_back(tmpquant);
					}
					{
						// 거푸집(계단부 - 경사 옆면) 측면 : W
						double  avg_step_thick = 0;
						avg_step_thick = Round(ST + 0.5* (SH / SN)*(SL / SN) / (sqrt(fabs((SH / SN)*(SH / SN) + (SL / SN)*(SL / SN))))); //계단두께 + 평균step 두께/2
						rArea = Round(avg_step_thick * slope_leng);

						if (_bShowVariable)
							noteArea.Format(_T("%s : %g(%s) * %g(%s)"), LocalDic::GetLocal(_T("계단 측면"), _T("Quantity")), avg_step_thick, LocalDic::GetLocal(_T("계단평균두께"), _T("Quantity")), slope_leng, LocalDic::GetLocal(_T("경사길이"), _T("Quantity")));
						else
							noteArea.Format(_T("%s : %g * %g"), LocalDic::GetLocal(_T("계단 측면"), _T("Quantity")), avg_step_thick, slope_leng);

						tmpquant.category = _T("SLAW");
						tmpquant.value = rArea;
						tmpquant.note = noteArea;
						tmpquant.num = 1;
						totalA.push_back(tmpquant);
					}
					{
						// 거푸집 (계단부 - 디딤판) 
						rArea = Round(SW * SD * SN);

						if (_bShowVariable)
							noteArea.Format(_T("%s : %g(%s) * %g(%s) * %g(%s)"), LocalDic::GetLocal(_T("계단 디딤판"), _T("Quantity")), SW, LocalDic::GetLocal(_T("계단폭"), _T("Quantity")), SD, LocalDic::GetLocal(_T("디딤판 너비"), _T("Quantity")), SN, LocalDic::GetLocal(_T("계단수"), _T("MSDBManager")));
						else
							noteArea.Format(_T("%s : %g * %g * %g"), LocalDic::GetLocal(_T("계단 디딤판"), _T("Quantity")), SW, SD, SN);

						tmpquant.category = _T("SLAU");
						tmpquant.value = rArea;
						tmpquant.note = noteArea;
						tmpquant.num = 1;
						totalA.push_back(tmpquant);
					}
				}
			}
			// 거푸집 (겹친면) - RH-1840을 진행하면서 같이 처리
			/*	if (ACV > 0.0)
			{
			rArea = -ACV;
			if(_bShowVariable)
			noteArea.Format(_T("겹친면: -%g(ACV)"), ACV);
			else
			noteArea.Format(_T("겹친면: -%g"), ACV);
			result.formType.push_back( dirToType['W'] ); result.formArea.push_back(rArea); result.formNote.push_back(noteArea);
			}*/
		}

		//겹치는 것 정리 및 저장 :거푸집
		for (int i = 0; i < totalA.size(); i++)
		{
			if (totalA[i].num == 0) continue;
			int count = 0;
			for (int j = i + 1; j < totalA.size(); j++)
			{

				if (totalA[i].note == totalA[j].note)
				{
					totalA[j].num = 0;
					count++;
				}
			}
			totalA[i].num += count;
			totalA[i].value *= totalA[i].num;
			CString tmp;
			tmp.Format(_T(" * %d = %g"), totalA[i].num, totalA[i].value);
			totalA[i].note = totalA[i].note + tmp;
		}

		for (int i = (int)totalA.size() - 1; i >= 0; i--)
		{
			if (totalA[i].num == 0)
				totalA.erase(totalA.begin() + i);
		}

		/*	StatusReporterWrapper::OutputMessage(_T("지움"));
			for (int i = 0; i < totalA.size(); i++)
			{
				CString str;
				str.Format(_T("category: %s , value:%g, note:%s, num:%d"), totalA[i].category, totalA[i].value, totalA[i].note, totalA[i].num);
				StatusReporterWrapper::OutputMessage(str);
			}*/

			// 계단참
		for (int i = 0; i < totalA.size(); i++)
		{
			if (totalA[i].category == "LAZ")
			{
				result.formDir.push_back(_T("Z"));
				result.formWorkName.push_back(dirToType[_T("Z")]); result.formArea.push_back(totalA[i].value); result.formNote.push_back(totalA[i].note);
			}
		}
		// 계단 챌판
		for (int i = 0; i < totalA.size(); i++)
		{
			if (totalA[i].category == "SAX")
			{
				result.formDir.push_back(_T("X"));
				result.formWorkName.push_back(dirToType[_T("X")]); result.formArea.push_back(totalA[i].value); result.formNote.push_back(totalA[i].note);
			}
		}
		// 계단 밑면
		for (int i = 0; i < totalA.size(); i++)
		{
			if (totalA[i].category == "SLAX")
			{
				result.formDir.push_back(_T("Y"));
				result.formWorkName.push_back(dirToType[_T("Y")]); result.formArea.push_back(totalA[i].value); result.formNote.push_back(totalA[i].note);
			}
		}
		// 계단 측면
		for (int i = 0; i < totalA.size(); i++)
		{
			if (totalA[i].category == "SLAW")
			{
				result.formDir.push_back(_T("W"));
				result.formWorkName.push_back(dirToType[_T("W")]); result.formArea.push_back(totalA[i].value); result.formNote.push_back(totalA[i].note);
			}
		}
		// 계단 디딤판
		for (int i = 0; i < totalA.size(); i++)
		{
			if (totalA[i].category == "SLAU")
			{
				result.formDir.push_back(_T("U"));
				result.formWorkName.push_back(dirToType[_T("U")]); result.formArea.push_back(totalA[i].value); result.formNote.push_back(totalA[i].note);
			}
		}
	}
}



void VolumeAreaInfoEMS::GetResultByCZoneRamp(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
{
	double rArea = 0;
	CString noteArea;

	if (ma_Param.size() != 3) return;
	double V = ma_Param[0], BA = ma_Param[1], VA = ma_Param[2];

	// Concrete
	rVolume = V;
	if (_bShowVariable)
		noteVolume.Format(_T("%g(V)"), V);
	else
		noteVolume.Format(_T("%g"), V);

	double Factor = 0.0;
	double dLibLathArea = 0.0;
	if (cz_id != -1)
	{
		for (int i = 0; i < ma_CZoneID.size(); ++i)
		{
			if (cz_id == ma_CZoneID[i])
			{
				Factor = ma_Factor[i];
				dLibLathArea = ma_LibLathArea[i];
			}
		}
	}
	else
	{
		Factor = 1.0;
		dLibLathArea = 1.0;
	}
	if (Factor == 0.0) return;

	CString czoneName;
	CString czoneNote;
	if (Factor != 1.0)
	{
		MSFloor* curFloor = (MSFloor*)mp_Owner->mp_Owner;
		MSConstructionZone* czone = curFloor->GetCZone(cz_id);
		czoneName = czone->GetName();
	}

	if (Factor != 1.0)
	{
		rVolume *= Factor;
		czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteVolume);
		noteVolume = czoneNote;
	}

	// 거푸집
	rArea = BA + VA;
	if (_bShowVariable)
		noteArea.Format(_T("%g(BA) + %g(VA)"), BA, VA);
	else
		noteArea.Format(_T("%g + %g"), BA, VA);

	if (mp_Owner->ma_UserFormWork.size() > 0)
	{
		map<CString, CString> dirToType;
		for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
			dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
		if (dirToType.count(_T("B")) > 0 && dirToType.count(_T("V")) > 0 && dirToType[_T("B")] == dirToType[_T("V")])
		{
			rArea = BA + VA;
			if (_bShowVariable)
				noteArea.Format(_T("%g(BA) + %g(VA)"), BA, VA);
			else
				noteArea.Format(_T("%g + %g"), BA, VA);

			if (Factor != 1.0)
			{
				rArea *= Factor;
				czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
				noteArea = czoneNote;
			}

			result.formDir.push_back(_T("Bot"));
			result.formWorkName.push_back(dirToType[_T("B")]); result.formArea.push_back(rArea); result.formNote.push_back(noteArea);
		}
		else
		{
			if (dirToType.count(_T("B")) > 0)
			{
				rArea = BA;
				if (_bShowVariable)
					noteArea.Format(_T("%g(BA)"), BA);
				else
					noteArea.Format(_T("%g"), BA);

				if (Factor != 1.0)
				{
					rArea *= Factor;
					czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
					noteArea = czoneNote;
				}

				result.formDir.push_back(_T("Bot"));
				result.formWorkName.push_back(dirToType[_T("B")]); result.formArea.push_back(rArea); result.formNote.push_back(noteArea);
			}
			if (dirToType.count(_T("V")) > 0)
			{
				rArea = VA;
				if (_bShowVariable)
					noteArea.Format(_T("%g(VA)"), VA);
				else
					noteArea.Format(_T("%g"), VA);

				if (Factor != 1.0)
				{
					rArea *= Factor;
					czoneNote.Format(_T("[%s : %g] * %s"), czoneName, Factor, noteArea);
					noteArea = czoneNote;
				}

				result.formDir.push_back(_T("Ver"));
				result.formWorkName.push_back(dirToType[_T("V")]); result.formArea.push_back(rArea); result.formNote.push_back(noteArea);
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultByCZoneMainHaunch(int cz_id, double &rVolume, CString& noteVolume, FormResult& result)
{
	//V(옆면), S(경사), F,E(마구리)
	double rArea = 0;
	CString noteArea;

	if (ma_Param.size() != 10) return;
	double W = ma_Param[0], H = ma_Param[1], SBL = ma_Param[2], STL = ma_Param[3], HSL = ma_Param[4];
	double VCUT = ma_Param[5], ACV_SIDE = ma_Param[6], ACV_SLOPE = ma_Param[7];
	double IE1 = ma_Param[8], IE2 = ma_Param[9];

	// Concrete
	rVolume = W * H / 2 * (SBL + abs(STL - SBL) / 3);
	if (_bShowVariable)
		noteVolume.Format(_T("%g(W) * %g(H) / 2 * (%g(SBL) + %g(STL-SBL) / 3)"), W, H, SBL, abs(STL - SBL));
	else
		noteVolume.Format(_T("%g * %g / 2 * (%g + %g / 3)"), W, H, SBL, abs(STL - SBL));

	// 거푸집
	rArea = SBL * H + W * H + (SBL + STL) / 2 * HSL;//옆면, 마구리면, 경사면
	if (_bShowVariable)
		noteArea.Format(_T("%g(SBL) * %g(H) + %g(W) * %g(H) + (%g(SBL) + %g(STL) / 2) * %g(HSL)"), SBL, H, W, H, SBL, STL, HSL);
	else
		noteArea.Format(_T("%g * %g + %g * %g + (%g + %g / 2) * %g"), SBL, H, W, H, SBL, STL, HSL);

	if (mp_Owner->ma_UserFormWork.size() > 0)
	{
		map<CString, CString> dirToType;
		for (int i = 0; i < mp_Owner->ma_UserFormWork.size(); ++i)
			dirToType[mp_Owner->ma_UserFormWork[i].dir] = mp_Owner->ma_UserFormWork[i].formWorkName;
		if (dirToType.count(_T("V")) > 0)
		{
			rArea = SBL * H - ACV_SIDE;
			if (_bShowVariable)
				noteArea.Format(_T("%g(SBL) * %g(H)"), SBL, H);
			else
				noteArea.Format(_T("%g * %g"), SBL, H);

			CString tmp;
			if (_bShowVariable)
				tmp.Format(_T(" - %g(ACV)"), ACV_SIDE);
			else
				tmp.Format(_T(" - %g"), ACV_SIDE);
			if (ACV_SIDE > 0) noteArea += tmp;
			result.formDir.push_back(_T("Side"));
			result.formWorkName.push_back(dirToType[_T("V")]);
			result.formArea.push_back(rArea);
			result.formNote.push_back(noteArea);
		}
		if (dirToType.count(_T("S")) > 0)
		{
			rArea = (SBL + STL) / 2 * HSL - ACV_SLOPE;
			if (_bShowVariable)
				noteArea.Format(_T("(%g(SBL) + %g(STL) / 2) * %g(HSL)"), SBL, STL, HSL);
			else
				noteArea.Format(_T("(%g + %g / 2) * %g"), SBL, STL, HSL);
			CString tmp;
			if (_bShowVariable)
				tmp.Format(_T(" - %g(ACV)"), ACV_SLOPE);
			else
				tmp.Format(_T(" - %g"), ACV_SLOPE);
			if (ACV_SLOPE > 0) noteArea += tmp;
			result.formDir.push_back(_T("Slope"));
			result.formWorkName.push_back(dirToType[_T("S")]);
			result.formArea.push_back(rArea);
			result.formNote.push_back(noteArea);
		}

		//마구리면
		if (IE1 > 0.0 || IE2 > 0.0)
		{
			CString type1 = 0, type2 = 0;
			double area1 = 0, area2 = 0;
			CString end_note1, end_note2;
			if (IE1 > 0.0)
			{
				rArea = W * H / 2;
				if (_bShowVariable)
					noteArea.Format(_T("%g(W) * %g(H) / 2"), W, H);
				else
					noteArea.Format(_T("%g * %g / 2"), W, H);
				if (dirToType.count(_T("F")) > 0) { type1 = dirToType[_T("F")]; area1 = rArea; end_note1 = noteArea; }
			}
			if (IE2 > 0.0)
			{
				rArea = W * H / 2;
				if (_bShowVariable)
					noteArea.Format(_T("%g(W) * %g(H) / 2"), W, H);
				else
					noteArea.Format(_T("%g * %g / 2"), W, H);
				if (dirToType.count(_T("E")) > 0) { type2 = dirToType[_T("E")]; area2 = rArea; end_note2 = noteArea; }
			}

			if (area1 > 0 && area2 > 0 && type1 == type2)
			{
				result.formDir.push_back(_T("Close"));
				result.formWorkName.push_back(type1); result.formArea.push_back(area1 + area2); result.formNote.push_back(end_note1 + _T(" * 2"));
			}
			else
			{
				if (area1 > 0) { result.formDir.push_back(_T("Close")); result.formWorkName.push_back(type1); result.formArea.push_back(area1); result.formNote.push_back(end_note1); }
				if (area2 > 0) { result.formDir.push_back(_T("Close")); result.formWorkName.push_back(type2); result.formArea.push_back(area2); result.formNote.push_back(end_note2); }
			}
		}
	}
}

void VolumeAreaInfoEMS::GetResultRebarExcludeVolume(double rebarVolume, double& rVolume, CString& noteVolume)
{
	// 콘크리트 체적에서 철근 체적 공제
	rVolume = 0;
	noteVolume = "0";
	bool isRebarVolumeExclude = mp_Owner->GetBuilding()->GetQuantityOption()->m_bRebarVolumeExclude;
	if (isRebarVolumeExclude)
	{
		// 철근부피공제를 위한 철근데이터를 가지고 옴
		rVolume = (-1) * rebarVolume;
		noteVolume.Format(_T("%g"), (-1) * rebarVolume);
	}
}

void VolumeAreaInfoEMS::GetResultAdditionalConcAndForm(int cz_id, FormResult formResult, vector<double> arFWOriginArea, vector<double>& rVolumes, vector<CString>& noteVolumes, FormResult& additionalFormResult, vector<long>& addedMatIds)
{
	// 거푸집 별 추가 콘크리트, 거푸집 합산
	bool isRebarAdditionalConcAndForm = mp_Owner->GetBuilding()->GetQuantityOption()->m_bAdditionalConcreteNForm;
	if (isRebarAdditionalConcAndForm)
	{
		int num = formResult.formArea.size();
		if (formResult.formDir.size() != num || formResult.formNote.size() != num || formResult.formWorkName.size() != num)
			return;

		for (int i = 0; i < num; i++)
		{
			if (formResult.formArea[i] < 0)
				continue;

			CString formTypeString = formResult.formWorkName[i];
			if (formTypeString == "None" || formTypeString == "Rib-Lath")
				continue;

			if (mp_Owner->GetBuilding()->GetFormManager()->FormWorkNameExistCheck(formTypeString) == false)
				continue;

			MSFormWorkType* formworkType = mp_Owner->GetBuilding()->GetFormManager()->GetFormWorkTypeByName(formTypeString);
			double concThick = formworkType->GetConcreteThick();
			double formRatio = formworkType->GetFormworkRatio();

			// Concrete
			if (concThick > 0 && formTypeString == formworkType->GetName())
			{
				CString findMatName;

				// [Q22-176] Suffix 이름이 설정되지 않은경우 mp_Owner->GetMaterial()->GetName()를 사용한다.
				// 즉 기존에 입력된 마테리얼 값 사용
				CString strSuffix = formworkType->GetSuffix();
				if (strSuffix != _T(""))
					findMatName.Format(_T("%s-%s"), mp_Owner->GetMaterial()->GetName(), strSuffix);
				else
					findMatName = mp_Owner->GetMaterial()->GetName();

				MSBaseBuilding* pBuilding = mp_Owner->GetBuilding();

				MSMaterial* pFindMaterial = pBuilding->GetMaterial(findMatName);
				if (pFindMaterial == NULL)
				{
					MSMaterial* newMaterial = dynamic_cast<MSMaterial*>(mp_Owner->GetMaterial()->Copy());
					newMaterial->SetName(findMatName);
					newMaterial->m_IsAddedMat = true;
					newMaterial->GetID();
					mp_Owner->GetBuilding()->Add(newMaterial);

					pFindMaterial = newMaterial;
				}

				double rVolume = 0;
				CString noteVolume;

				rVolume = arFWOriginArea[i] * concThick * 0.001;
				noteVolume.Format(_T("%g * %g"), arFWOriginArea[i], concThick * 0.001);
				//if (rVolume != 0)
				{
					rVolumes.push_back(rVolume);
					noteVolumes.push_back(noteVolume);
					addedMatIds.push_back(pFindMaterial->GetID());
				}
			}

			// Form
			if (formRatio > 0)
			{
				double rArea = arFWOriginArea[i] * formRatio * 0.01;
				CString noteArea;
				noteArea.Format(_T("%g * %g"), arFWOriginArea[i], formRatio * 0.01);

				additionalFormResult.formDir.push_back(_T("Add"));
				additionalFormResult.formWorkName.push_back(formResult.formWorkName[i]);
				additionalFormResult.formArea.push_back(rArea);
				additionalFormResult.formNote.push_back(noteArea);
			}
		}

		vector<int> haunch_matl;
		vector<double> haunch_volume;
		vector<CString> haunch_note;
		vector<int> haunch_id;

		vector<CString> haunch_arFWAreaType;
		vector<double> haunch_arFWArea;
		vector<CString> haunch_arFormWorkNote;
		vector<int> fwHaunch_id;
		vector<double> haunch_arFWOriginArea;
		vector<int> haunch_matbool;

		mp_Owner->GetHaunchQuantityByCZone(cz_id, haunch_matl, haunch_volume, haunch_note, haunch_arFWAreaType, haunch_arFWArea, haunch_arFormWorkNote, haunch_id, fwHaunch_id, haunch_arFWOriginArea, haunch_matbool);

		int subNum = haunch_arFWArea.size();
		if (haunch_arFWAreaType.size() != subNum || haunch_arFormWorkNote.size() != subNum)
			return;

		for (int i = 0; i < subNum; i++)
		{
			if (haunch_arFWArea[i] < 0)
				continue;

			CString formTypeString = haunch_arFWAreaType[i];
			if (formTypeString == "None" || formTypeString == "Rib-Lath")
				continue;

			if (mp_Owner->GetBuilding()->GetFormManager()->FormWorkNameExistCheck(formTypeString) == false)
				continue;

			MSFormWorkType* formworkType = mp_Owner->GetBuilding()->GetFormManager()->GetFormWorkTypeByName(formTypeString);
			double concThick = formworkType->GetConcreteThick();
			double formRatio = formworkType->GetFormworkRatio();

			CString findMatName;

			// [Q22-176] Suffix 이름이 설정되지 않은경우 mp_Owner->GetMaterial()->GetName()를 사용한다.
			// 즉 기존에 입력된 마테리얼 값 사용
			CString strSuffix = formworkType->GetSuffix();
			if (strSuffix != _T(""))
				findMatName.Format(_T("%s-%s"), mp_Owner->GetMaterial()->GetName(), strSuffix);
			else
				findMatName = mp_Owner->GetMaterial()->GetName();

			MSBaseBuilding* pBuilding = mp_Owner->GetBuilding();
			MSMaterial* pFindMaterial = pBuilding->GetMaterial(findMatName);
			if (pFindMaterial == NULL)
			{
				MSMaterial* newMaterial = dynamic_cast<MSMaterial*>(mp_Owner->GetMaterial()->Copy());
				newMaterial->SetName(findMatName);
				newMaterial->m_IsAddedMat = true;
				newMaterial->GetID();
				mp_Owner->GetBuilding()->Add(newMaterial);

				pFindMaterial = newMaterial;
			}

			addedMatIds.push_back(pFindMaterial->GetID());

			double rVolume = 0;
			CString noteVolume;

			// Concrete
			if (concThick > 0 && formTypeString == formworkType->GetName())
			{
				rVolume = haunch_arFWOriginArea[i] * concThick * 0.001;
				noteVolume.Format(_T("%g * %g"), haunch_arFWOriginArea[i], concThick * 0.001);
				//if (rVolume != 0)
				{
					rVolumes.push_back(rVolume);
					noteVolumes.push_back(noteVolume);
				}
			}

			// Form
			if (formRatio > 0)
			{
				double rArea = haunch_arFWOriginArea[i] * formRatio * 0.01;
				CString noteArea;
				noteArea.Format(_T("%g * %g"), haunch_arFWOriginArea[i], formRatio * 0.01);

				additionalFormResult.formDir.push_back(_T("Add"));
				additionalFormResult.formWorkName.push_back(haunch_arFWAreaType[i]);
				additionalFormResult.formArea.push_back(rArea);
				additionalFormResult.formNote.push_back(noteArea);
			}

		}
	}
}

void VolumeAreaInfoEMS::GetResultConcDeduction(double concVolume, double& deductionConcVolume, CString& noteVolume)
{
	// 시설물 콘크리트 공제
	deductionConcVolume = 0;
	noteVolume = "0";
	bool isConcDeduction = mp_Owner->GetBuilding()->GetQuantityOption()->m_bConcDeduction;
	if (isConcDeduction)
	{
		// 독립기초, 줄기초, 피트는 제외
		if (mp_Owner->GetType() == MSElement::msIsoFootG || mp_Owner->GetType() == MSElement::msWallFootG || mp_Owner->GetType() == MSElement::msPitG)
			return;

		double value = mp_Owner->GetBuilding()->GetQuantityOption()->m_dConcDeduction;
		if (value > 0)
		{
			deductionConcVolume = (-1) * concVolume * value * 0.01;
			noteVolume.Format(_T("%g * %g"), (-1) * concVolume, value * 0.01);
		}
	}
}

double VolumeAreaInfoEMS::Round(double value, int pos)
{
	double factor = 1000.0;
	if (pos != 3) factor = pow(10.0, pos);

	double temp = 0;
	temp = value * factor;  // 원하는 소수점 자리수만큼 10의 누승을 함
	temp = floor(temp + 0.5);          // 0.5를 더한후 버림하면 반올림이 됨
	temp /= factor;           // 다시 원래 소수점 자리수로
	return temp;
}


void VolumeAreaInfoEMS::ConvertUnit(int unit[])
{
	//https://changsoft.atlassian.net/browse/Q24-363
	//acis로 계산한 값이 xx4.9999 xxx5.0000 의 값으로 다르게 나올 때가 있어서
	//mm값을 미리 반올림 처리 
	for (int i = 0; i < ma_Param.size(); ++i)
		ma_Param[i] = Round(ma_Param[i], 3);

	for (int i = 0; i < ma_Param.size(); ++i)
		if (fabs(ma_Param[i]) < DTOL_GM) ma_Param[i] = 0.0;

	for (int i = 0; i < ma_Param.size(); ++i)
	{
		double factor = 1.0;
		if (unit[i] == 0) factor = GM2ACIS;
		else if (unit[i] == 1) factor = GM2ACIS_A;
		else if (unit[i] == 2) factor = GM2ACIS_V;
		else continue;
		ma_Param[i] *= factor;
	}

	for (int i = 0; i < ma_Param.size(); ++i)
		ma_Param[i] = Round(ma_Param[i], 3);
}

void VolumeAreaInfoEMS::ConvertUnitAndPushVector(vector<double> input, int unit)
{
	for (int i = 0; i < input.size(); i++)
	{
		double outputvalue;
		double factor = 1.0;
		if (unit == 1) factor = GM2ACIS;
		else if (unit == 2) factor = GM2ACIS_A;
		else if (unit == 3) factor = GM2ACIS_V;
		else continue;

		outputvalue = input[i] * factor;
		outputvalue = Round(outputvalue, 3);
		ma_Param.push_back(outputvalue);
	}
}

void VolumeAreaInfoEMS::ConvertUnitAndPushDouble(double input, int unit)
{
	double outputvalue;
	double factor = 1.0;
	if (unit == 1) factor = GM2ACIS;
	else if (unit == 2) factor = GM2ACIS_A;
	else if (unit == 3) factor = GM2ACIS_V;

	outputvalue = input * factor;
	outputvalue = Round(outputvalue, 3);
	ma_Param.push_back(outputvalue);
}

void VolumeAreaInfoEMS::SetParamColumn(double W, double D, double H, double TO, double BO, double TS, double TB, double LW, double RW, double R, double BCV0, double BCV1, double BCV2, double BCV3, double BH1, double BH2, double CHT, double CHV, double CHA0, double CHA1, double CHA2, double CHA3, double CO, double SUBCHA0, double SUBCHA1, double SUBCHA2, double SUBCHA3)
{
	double input[] = { W, D, H, TO, BO, TS, TB, LW, RW, R, BCV0, BCV1, BCV2, BCV3, BH1, BH2, CHT, CHV, CHA0, CHA1, CHA2, CHA3, CO, SUBCHA0, SUBCHA1, SUBCHA2, SUBCHA3 };
	int unit[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,1,1,1, 0, 0, 0, 2, 1, 1, 1, 1, 0, 1, 1, 1, 1 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::SetParamCustomColumn(vector<double> profileLine, double col_Area, double H, double TO, double BO, double TS, double TB, vector<double> WCL, vector<double> WCV, vector<double> BCV)
{
	ma_Param.clear();
	ConvertUnitAndPushVector(profileLine, 1);
	ConvertUnitAndPushVector(WCL, 1);
	ConvertUnitAndPushVector(WCV, 2);
	ConvertUnitAndPushVector(BCV, 2);
	ConvertUnitAndPushDouble(col_Area, 2);
	ConvertUnitAndPushDouble(H, 1);
	ConvertUnitAndPushDouble(TO, 1);
	ConvertUnitAndPushDouble(BO, 1);
	ConvertUnitAndPushDouble(TS, 1);
	ConvertUnitAndPushDouble(TB, 1);
}

void VolumeAreaInfoEMS::SetParamBeam(double W, double D, double A, double CL, double LL, double RL, double TS, double ACV, double W1, double W2, double D1, double D2, double INC_BOT, double WAL, double VH, double LACV, double RACV, double LLF, double RLF, double PCCL, vector<double> BUILTUP, map<int, vector<double>> BUILTUP_LEFTAREA, map<int, vector<double>> BUILTUP_RIGHTAREA)
{
	double input[] = { W, D, A, CL, LL, RL, TS, ACV, W1, W2, D1, D2, INC_BOT, WAL, VH, LACV, RACV, LLF, RLF, PCCL };
	int unit[] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 1, 2, 1, 1, 0, 0, 0 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

	//Builtup 추가
	if (BUILTUP.size() > 0)
	{
		for (int i = 0; i < BUILTUP.size(); i++)
			ma_Param.push_back(BUILTUP[i] * GM2ACIS);
	}
	ma_BuiltUpBeamLeftACV = BUILTUP_LEFTAREA;
	ma_BuiltUpBeamRightACV = BUILTUP_RIGHTAREA;
}


void VolumeAreaInfoEMS::SetParamCustomBeam(double A, double TSA, double CL, double LL, double RL, double leftOL, double rightOL, double bottomOL, double WAL)
{
	double input[] = { A, TSA, CL, LL, RL, leftOL, rightOL, bottomOL, WAL };
	int unit[] = { 1, 1,0,0,0,0,0,0,1 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::SetParamLowWall(double W, double H, double CL, double L, double TS, double TH, double BH, double TTS, double TLACV, double TRACV, double BLACV, double BRACV)
{
	double input[] = { W, H, CL, L, TS, TH, BH, TTS, TLACV, TRACV, BLACV, BRACV };
	int unit[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::SetParamSlab(double A, double OL, double T, double IA, double IL, double ACV, double V2, double A2, double A2_END, double IAC, double H_FORM,
	double VTR, double VWL, double HUNV, double ACV_OUT, double ACV_IN, double V_INTERSECT, vector<double> HaunchV, vector<double> EdgeWallV, vector<double> haunchStretchedVolumes, vector<double> edgeWallHaunchStretchedVolumes)
{
	double input[] = { A, OL, T, IA, IL, ACV, V2, A2, A2_END, IAC, H_FORM, VTR, VWL, HUNV, ACV_OUT, ACV_IN , V_INTERSECT };
	int unit[] = { 1, 0, 0, 1, 0, 1, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1 ,2 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

	for (int i = 0; i < HaunchV.size(); ++i)
		ma_Param.push_back(HaunchV[i]);

	for (int i = 0; i < EdgeWallV.size(); ++i)
		ma_Param.push_back(EdgeWallV[i]);

	for (int i = 0; i < haunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(haunchStretchedVolumes[i]);

	for (int i = 0; i < edgeWallHaunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(edgeWallHaunchStretchedVolumes[i]);
}

void VolumeAreaInfoEMS::SetParamHalfPCSlab(double A, double OL, double T, double T1, double T2, double IA, double IL, double ACV, double V2, double A2, double A2_END, double IAC, double H_FORM,
	double VTR, double VWL, double HUNV, double ACV_OUT, double ACV_IN, double V_INTERSECT, vector<double> HaunchV, vector<double> EdgeWallV, vector<double> haunchStretchedVolumes, vector<double> edgeWallHaunchStretchedVolumes)
{
	double input[] = { A, OL, T, T1, T2, IA, IL, ACV, V2, A2, A2_END, IAC, H_FORM, VTR, VWL, HUNV, ACV_OUT, ACV_IN , V_INTERSECT };
	int unit[] = { 1, 0, 0, 0, 0, 1, 0, 1, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1 ,2 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

	for (int i = 0; i < HaunchV.size(); ++i)
		ma_Param.push_back(HaunchV[i]);

	for (int i = 0; i < EdgeWallV.size(); ++i)
		ma_Param.push_back(EdgeWallV[i]);

	for (int i = 0; i < haunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(haunchStretchedVolumes[i]);

	for (int i = 0; i < edgeWallHaunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(edgeWallHaunchStretchedVolumes[i]);
}

void VolumeAreaInfoEMS::SetParamIMTPCSlab(double Lx, double Ly, double Sx, double Sy, double L1, double L2, double L3, double H1, double H2, double H3, double ELx, double ELy, double OL, int NR, int NELx, int NELy, double DF)
{
	double input[] = { Lx, Ly, Sx, Sy, L1, L2, L3, H1, H2, H3, ELx, ELy, OL, NR, NELx, NELy ,DF };
	int unit[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1 ,0 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::SetParamHCSPCSlab(double Lx, double Ly, double Sx, double Sy, double L1, double L2, double L3, double H1, double H2, double HL, int NH, double FL, double A_bot, double A_open, double T_s, double B_b, double L_b,
	double W_col, double D_col, double R_col)
{
	double input[] = { Lx, Ly, Sx, Sy, L1, L2, L3, H1, H2, HL, NH, FL, A_bot, A_open, T_s, B_b, L_b, W_col, D_col, R_col };
	int unit[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 ,0, 0, 0, 0, 0 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

}
void VolumeAreaInfoEMS::SetParamRPSPCSlab(double Lx, double Ly, double Sx, double Sy, double L1, double L2, double L3, double H1, double H2, double H3, double HL, double FL, double A_bot, double A_open, double T_s, double B_b, double L_b,
	double W_col, double D_col, double R_col, int NH)
{
	double input[] = { Lx, Ly, Sx, Sy, L1, L2, L3, H1, H2, H3, HL, FL, A_bot, A_open, T_s, B_b, L_b, W_col, D_col, R_col,NH };
	int unit[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, -1, -1 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

}

void VolumeAreaInfoEMS::SetParamRebarEmbeddedSlab(double A, double OL, double T, double IA, double IL, double ACV, double V2, double A2, double A2_END, double IAC, double H_FORM,
	double VTR, double VWL, double HUNV, double SX, double SY, double ACV_OUT, double ACV_IN, double V_INTERSECT, vector<double> HaunchV, vector<double> EdgeWallV, vector<double> haunchStretchedVolumes, vector<double> edgeWallHaunchStretchedVolumes)
{
	double input[] = { A, OL, T, IA, IL, ACV, V2, A2, A2_END, IAC, H_FORM, VTR, VWL, HUNV, SX, SY, ACV_OUT, ACV_IN, V_INTERSECT };
	int unit[] = { 1, 0, 0, 1, 0, 1, 2, 1, 1, 1, 1, 2, 2, 2, 0, 0, 1, 1, 2 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

	for (int i = 0; i < HaunchV.size(); ++i)
		ma_Param.push_back(HaunchV[i]);

	for (int i = 0; i < EdgeWallV.size(); ++i)
		ma_Param.push_back(EdgeWallV[i]);

	for (int i = 0; i < haunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(haunchStretchedVolumes[i]);

	for (int i = 0; i < edgeWallHaunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(edgeWallHaunchStretchedVolumes[i]);
}

void VolumeAreaInfoEMS::SetParamWall(double T, double H, double A, double TO, double BO, double TS, double CL, double LL, double RL, double IW, double IH, double LACV, double RACV, double IE1, double IE2, double ACVE1, double ACVE2, double IWC, double IHC, double IWF, double IHF, double NUM_O)
{
	double input[] = { T, H, A, TO, BO, TS, CL, LL, RL, IW, IH, LACV, RACV, IE1, IE2, ACVE1, ACVE2, IWC, IHC, IWF, IHF, NUM_O };
	int unit[] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, 0, 0, 0, 0, -1 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::SetParamIsoFoot(double W, double D, double T, double TB, double ACV, double ACV_BOT, double VCUT, vector<double> H_FORM, vector<double> HV, vector<CString> HDir, vector<double> haunchStretchedVolumes)
{
	double input[] = { W, D, T, TB, ACV, ACV_BOT, VCUT };
	int unit[] = { 0, 0, 0, 0, 1, 1, 2 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

	for (int i = 0; i < HV.size(); i++)
	{
		ma_Param.push_back(Round(H_FORM[i] * GM2ACIS_A));
		//ma_Param.push_back(Round(HV[i] * GM2ACIS_V));
		ma_Param.push_back(HV[i]);
		if (HDir[i] == _T("0"))
			ma_Param.push_back(0);
		else if (HDir[i] == _T("1"))
			ma_Param.push_back(1);
		else if (HDir[i] == _T("2"))
			ma_Param.push_back(2);
		else if (HDir[i] == _T("3"))
			ma_Param.push_back(3);
		else if (HDir[i] == _T("Empty"))
			ma_Param.push_back(-1);
	}

	for (int i = 0; i < haunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(haunchStretchedVolumes[i]);
}

void VolumeAreaInfoEMS::SetParamMat(double A, double OL, double T, double IA, double IL, double ACV, double V2, double A2, double A2_END, double IAC, double AB, double TB, double H_FORM,
	double VTR, double VWL, double HUNV, double ACV_OUT, double ACV_IN, double V_INTERSECT, vector<double> HaunchV, vector<double> EdgeWallV, vector<double> haunchStretchedVolumes, vector<double> edgeWallHaunchStretchedVolumes)
{
	double input[] = { A, OL, T, IA, IL, ACV, V2, A2, A2_END, IAC, AB, TB, H_FORM, VTR, VWL, HUNV, ACV_OUT, ACV_IN , V_INTERSECT };
	int unit[] = { 1, 0, 0, 1, 0, 1, 2, 1, 1, 1, 1, 0, 1, 2, 2, 2, 1, 1 , 2 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

	for (int i = 0; i < HaunchV.size(); ++i)
		ma_Param.push_back(HaunchV[i]);

	for (int i = 0; i < EdgeWallV.size(); ++i)
		ma_Param.push_back(EdgeWallV[i]);

	for (int i = 0; i < haunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(haunchStretchedVolumes[i]);

	for (int i = 0; i < edgeWallHaunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(edgeWallHaunchStretchedVolumes[i]);
}

void VolumeAreaInfoEMS::SetParamGeneralDeck(double SX, double SY, double LX, double LY, double OA, double T, double FT, double FBW, double FTW, double FS, double DF)
{
	double input[] = { SX, SY, LX, LY, OA, T , FT, FBW, FTW, FS, DF };
	int unit[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,0 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::SetParamWallFoot(double W, double H, double CL, double LL, double RL, double TS, double ACV, double IE1, double IE2, vector<double> H_FORM, vector<double> HV, vector<CString> HDir, vector<double> haunchStretchedVolumes)
{
	double input[] = { W, H, CL , LL , RL, TS, ACV, IE1, IE2 };
	int unit[] = { 0, 0, 0, 0,0,0 ,1, -1, -1 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

	for (int i = 0; i < HV.size(); i++)
	{
		ma_Param.push_back(Round(H_FORM[i] * GM2ACIS_A));
		//ma_Param.push_back(Round(HV[i] * GM2ACIS_V));
		ma_Param.push_back(HV[i]);
		if (HDir[i] == _T("L"))
			ma_Param.push_back(0);
		else if (HDir[i] == _T("R"))
			ma_Param.push_back(1);
		else if (HDir[i] == _T("Empty"))
			ma_Param.push_back(-1);
	}

	for (int i = 0; i < haunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(haunchStretchedVolumes[i]);
}


void VolumeAreaInfoEMS::SetParamPit(double A, double OL, double H1, double PA, double POL, double H2, double WA, double WOL, double H3, double ACV, double HUN_F, double HUN_V, vector<double> HaunchV, vector<double> haunchStretchedVolumes)
{
	double input[] = { A, OL, H1, PA, POL, H2, WA, WOL, H3, ACV, HUN_F, HUN_V };
	int unit[] = { 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 2 }; // 0:L, 1:A, 2:V
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);

	for (int i = 0; i < HaunchV.size(); ++i)
		ma_Param.push_back(HaunchV[i]);

	for (int i = 0; i < haunchStretchedVolumes.size(); ++i)
		ma_Param.push_back(haunchStretchedVolumes[i]);
}

void VolumeAreaInfoEMS::SetParamStair(double CA1, double T1, double CA2, double T2, double L1, double W1, double H1, double N1, double ST1,
	double L2, double W2, double H2, double N2, double ST2, double ACV)
{
	double input[] = { CA1,  T1,  CA2,  T2,  L1,  W1,  H1,  N1,  ST1, L2,  W2,  H2,  N2,  ST2, ACV };
	int unit[] = { 1, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 1 }; // 0:L, 1:A, 2:V
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::SetParamStair(vector<double> LANDING, vector<double>STEP_PARAM)
{//landing 1,0 step 0000 num -1
	ma_Param.clear();
	for (int i = 0; i < LANDING.size(); i++)
	{
		if (i % 2 == 0)
			ma_Param.push_back(LANDING[i] * GM2ACIS_A);
		else
			ma_Param.push_back(LANDING[i] * GM2ACIS);
	}

	for (int i = 0; i < STEP_PARAM.size(); i++)
	{
		if (i % 5 == 0)
			ma_Param.push_back(STEP_PARAM[i]); //stepN
		else
			ma_Param.push_back(STEP_PARAM[i] * GM2ACIS);
	}
}

void VolumeAreaInfoEMS::SetParamRamp(double V, double BA, double VA)
{
	double input[] = { V, BA, VA };
	int unit[] = { 2, 1, 1 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::SetParamMainHaunch(double W, double H, double SBL, double STL, double HSL, double VCUT, double ACV_SIDE, double ACV_SLOPE, double IE1, double IE2)
{
	double input[] = { W, H, SBL, STL, HSL, VCUT, ACV_SIDE, ACV_SLOPE, IE1, IE2 };
	int unit[] = { 0, 0, 0, 0, 0, 2, 1, 1, -1, -1 };
	ma_Param.clear();
	for (int i = 0; i < sizeof(input) / sizeof(double); ++i)
		ma_Param.push_back(input[i]);

	ConvertUnit(unit);
}

void VolumeAreaInfoEMS::AddSubHaunchQuantityAll(int matlID, double volume, CString note, CString haunchFormWorkName, double area, CString areanote, int haunchID, int czone_id)
{
	volume = Round(volume, 3);
	area = Round(area, 3);

	ma_HaunchMatlID.push_back(matlID);
	ma_HaunchVolume.push_back(volume);
	ma_HaunchNote.push_back(note);
	ma_HaunchFormType.push_back(haunchFormWorkName);
	ma_HaunchFormArea.push_back(area);
	ma_HaunchFormNote.push_back(areanote);
	ma_HaunchID.push_back(haunchID);
	ma_Hanchczone.push_back(czone_id);	
}

void VolumeAreaInfoEMS::AddEdgeWallSubHaunchQuantityAll(int matlID, double volume, CString note, CString haunchFormWorkName, double area, CString areanote, int haunchID, vector<int> czone_ids)
{
	volume = Round(volume, 3);
	area = Round(area, 3);

	for (int i = 0; i < czone_ids.size(); i++)
	{
		ma_HaunchMatlID.push_back(matlID);
		ma_HaunchVolume.push_back(volume);
		ma_HaunchNote.push_back(note);
		ma_HaunchFormType.push_back(haunchFormWorkName);
		ma_HaunchFormArea.push_back(area);
		ma_HaunchFormNote.push_back(areanote);
		ma_HaunchID.push_back(haunchID);
		ma_Hanchczone.push_back(czone_ids[i]);
	}	
}

void VolumeAreaInfoEMS::SetSubHaunchCZoneIds(vector<int> czone_ids)
{
	//ma_Hanchczone = czone_ids;
}

void VolumeAreaInfoEMS::ClearSubHaunchQuantityAll()
{
	ma_HaunchMatlID.clear();
	ma_HaunchVolume.clear();
	ma_HaunchNote.clear();
	ma_HaunchFormType.clear();
	ma_HaunchFormArea.clear();
	ma_HaunchFormNote.clear();
	ma_HaunchID.clear();
	ma_Hanchczone.clear();
}

/*
void VolumeAreaInfoEMS::AddSubEgewallQuantityAll(int matlID, double volume, CString note, int haunchID)
{
	volume = Round(volume, 3);

	ma_edgeWAallMatlID.push_back(matlID);
	ma_edgeWallVolume.push_back(volume);
	ma_edgeWallNote.push_back(note);
	ma_edgeWallID.push_back(haunchID);
}
*/

CString VolumeAreaInfoEMS::ChangeVolumeInfoNoVariable(CString note)
{
	CString noVariableNote = _T("");
	noVariableNote = note;

	//일단 헌치와 버림 물량만 해당 코드를 탄다. - 기회가 된다면 기존 코드를 이걸로 다 바꾸는 것이 낫지 않나? 
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(H)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(B)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(W)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(B1)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(B2)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(D)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(D1)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(D2)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(A)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(OL)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(IL)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(OPENING_A)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(면적)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(둘레)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(L)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(L')"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(L2)"));
	noVariableNote = ReplaceStringInfo(noVariableNote, _T("(HSL)"));


	return noVariableNote;
}

CString VolumeAreaInfoEMS::ReplaceStringInfo(CString note, CString replaceitem)
{
	CString tmpnote = _T("");
	tmpnote = note;
	int index = 0;

	while (tmpnote.Find(replaceitem, index) >= 0)
	{
		tmpnote.Replace(replaceitem, _T(""));
	}

	return tmpnote;
}

void VolumeAreaInfoEMS::AddBuiltUpBeamQuantity(int matlID, double volume, CString note, int builtupBeamID)
{
	volume = Round(volume, 3);

	ma_BuiltUpBeamID.push_back(builtupBeamID);
	ma_BuiltUpBemaMatlID.push_back(matlID);
	ma_BuiltupBeamVolume.push_back(volume);
	ma_BuiltUpBeamNote.push_back(note);
}

void VolumeAreaInfoEMS::ClearBuiltUpBeamQuantity()
{
	ma_BuiltUpBeamID.clear();
	ma_BuiltUpBemaMatlID.clear();
	ma_BuiltupBeamVolume.clear();
	ma_BuiltUpBeamNote.clear();
}

void VolumeAreaInfoEMS::ClearCZoneInfo()
{
	ma_CZoneID.clear();
	ma_Factor.clear();
	ma_LibLathArea.clear();
}

void VolumeAreaInfoEMS::ClearAll()
{
	ma_Param.clear();
	ClearCZoneInfo();
	ClearSubHaunchQuantityAll();
	ClearBuiltUpBeamQuantity();
}