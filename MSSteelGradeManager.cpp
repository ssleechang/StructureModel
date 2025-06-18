#include "StdAfx.h"
#include "MSSteelGradeManager.h"
#include "MSDeformedBar.h"

#define STR_PRE_FIX_D			_T("D")
#define STR_PRE_FIX_HD		_T("HD")
#define STR_PRE_FIX_SHD		_T("SHD")
#define STR_PRE_FIX_UHD		_T("UHD")

IMPLEMENT_SERIAL(MSSteelGradeManager, MSObject, VERSIONABLE_SCHEMA | 1)
MSSteelGradeManager::MSSteelGradeManager(void)
{
}


MSSteelGradeManager::~MSSteelGradeManager(void)
{
}

void MSSteelGradeManager::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	int numData = mm_SteelGradePrefix.size();
	buffer << numData;
	map<eSteelGrade, CString>::iterator it;
	for(it = mm_SteelGradePrefix.begin(); it != mm_SteelGradePrefix.end(); it++)
	{
		int SGrade = (int)it->first;
		buffer << SGrade << it->second;
	}
}

void MSSteelGradeManager::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	mm_SteelGradePrefix.clear();
	int numData = 0;
	buffer >> numData;

	int SGrade = 0;
	CString Prefix = _T("");
	for(int i = 0; i < numData; i++)
	{
		buffer >> SGrade >> Prefix;
		mm_SteelGradePrefix.insert(make_pair((eSteelGrade)SGrade, Prefix));
	}
}

void MSSteelGradeManager::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	if(ar.IsStoring())
	{
		int numData = mm_SteelGradePrefix.size();
		ar << numData;
		map<eSteelGrade, CString>::iterator it;
		for(it = mm_SteelGradePrefix.begin(); it != mm_SteelGradePrefix.end(); it++)
		{
			int SGrade = (int)it->first;
			ar << SGrade;
			ar << it->second;
		}
	}
	else
	{
		int numData = 0;
		ar >> numData;

		int SGrade = 0;
		CString Prefix = _T("");
		for(int i = 0; i < numData; i++)
		{
			ar >> SGrade;
			ar >> Prefix;
			mm_SteelGradePrefix.insert(make_pair((eSteelGrade)SGrade, Prefix));
		}
	}
}

void MSSteelGradeManager::GlobalOptionSerialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	if(ar.IsStoring())
	{
		int numData = mm_SteelGradePrefix.size();
		ar << numData;
		map<eSteelGrade, CString>::iterator it;
		for(it = mm_SteelGradePrefix.begin(); it != mm_SteelGradePrefix.end(); it++)
		{
			int SGrade = (int)it->first;
			ar << SGrade;
			ar << it->second;
		}
	}
	else
	{
		int numData = 0;
		ar >> numData;

		int SGrade = 0;
		CString Prefix = _T("");
		for(int i = 0; i < numData; i++)
		{
			ar >> SGrade;
			ar >> Prefix;
			mm_SteelGradePrefix.insert(make_pair((eSteelGrade)SGrade, Prefix));
		}
	}
}
int MSSteelGradeManager::MakeDefaultDataByRebarCode(int rebarCode)
{
	mm_SteelGradePrefix.clear();
	switch (rebarCode)
	{
	case MSDeformedBar::REBAR_MATERIAL_KS:					return MakeDefaultDataByRebarCode_Kr();	break;
	case MSDeformedBar::REBAR_MATERIAL_ASTM:				return MakeDefaultDataByRebarCode_ASTM();	break;
	case MSDeformedBar::REBAR_MATERIAL_GB:					return MakeDefaultDataByRebarCode_GB();	break;
	case MSDeformedBar::REBAR_MATERIAL_BS:						return MakeDefaultDataByRebarCode_BS();	break;
	case MSDeformedBar::REBAR_MATERIAL_VIETNAM:			return MakeDefaultDataByRebarCode_VIETNAM();	break;
	case MSDeformedBar::REBAR_MATERIAL_INDIA:				return MakeDefaultDataByRebarCode_INDIA();	break;
	case MSDeformedBar::REBAR_MATERIAL_SINGAPORE:		return MakeDefaultDataByRebarCode_SINGAPORE();	break;
	case MSDeformedBar::REBAR_MATERIAL_JIS:		return MakeDefaultDataByRebarCode_JIS();	break;
	default:
		return MakeDefaultDataByRebarCode_Kr();
	}
}

int MSSteelGradeManager::MakeDefaultDataByRebarCode_Kr()
{
	CString Prefix = _T("");
	for (int grade = MSSteelGradeManager::SD300; grade <= MSSteelGradeManager::SD700; grade++)
	{
		eSteelGrade SGrade = (eSteelGrade)grade;
		if (SGrade < SD400)	Prefix = STR_PRE_FIX_D;
		else if (SD400 <= grade && grade <= SD400W)	Prefix = STR_PRE_FIX_HD;
		else if (SD500 <= grade && grade <= SD500W)	Prefix = STR_PRE_FIX_SHD;
		else if (SD600 <= grade && grade <= SD700)		Prefix = STR_PRE_FIX_UHD;

		mm_SteelGradePrefix.insert(make_pair(SGrade, Prefix));
	}
	return (int)mm_SteelGradePrefix.size();
}
int MSSteelGradeManager::MakeDefaultDataByRebarCode_ASTM()
{
	return MakeDefaultDataByRebarCode_Kr();
}
int MSSteelGradeManager::MakeDefaultDataByRebarCode_GB()
{
	CString Prefix = _T("");
	for (int grade = MSSteelGradeManager::HPB235; grade <= MSSteelGradeManager::HRBF500; grade++)
	{
		eSteelGrade SGrade = (eSteelGrade)grade;
		Prefix = STR_PRE_FIX_HD;
		mm_SteelGradePrefix.insert(make_pair(SGrade, Prefix));
	}
	return (int)mm_SteelGradePrefix.size();
}
int MSSteelGradeManager::MakeDefaultDataByRebarCode_BS()
{
	return MakeDefaultDataByRebarCode_Kr();
}
int MSSteelGradeManager::MakeDefaultDataByRebarCode_VIETNAM()
{
	return MakeDefaultDataByRebarCode_Kr();
}
int MSSteelGradeManager::MakeDefaultDataByRebarCode_INDIA()
{
	CString Prefix = _T("");
	for (int grade = MSSteelGradeManager::Fe415; grade <= MSSteelGradeManager::CRS; grade++)
	{
		eSteelGrade SGrade = (eSteelGrade)grade;
		Prefix = STR_PRE_FIX_HD;
		mm_SteelGradePrefix.insert(make_pair(SGrade, Prefix));
	}
	return (int)mm_SteelGradePrefix.size();
}
int MSSteelGradeManager::MakeDefaultDataByRebarCode_SINGAPORE()
{
	CString Prefix = _T("");
	for (int grade = MSSteelGradeManager::B485B; grade <= MSSteelGradeManager::B600B; grade++)
	{
		eSteelGrade SGrade = (eSteelGrade)grade;
		Prefix = STR_PRE_FIX_HD;
		mm_SteelGradePrefix.insert(make_pair(SGrade, Prefix));
	}
	return (int)mm_SteelGradePrefix.size();
}

int MSSteelGradeManager::MakeDefaultDataByRebarCode_JIS()
{
	CString Prefix = STR_PRE_FIX_D;
	for (int grade = MSSteelGradeManager::SD295; grade <= MSSteelGradeManager::SD490; grade++)
	{
		eSteelGrade SGrade = (eSteelGrade)grade;
		mm_SteelGradePrefix.insert(make_pair(SGrade, Prefix));
	}
	return (int)mm_SteelGradePrefix.size();
}

void MSSteelGradeManager::SetSGradePrefixes(map<eSteelGrade, CString>& newPrefixMap)
{
	mm_SteelGradePrefix.clear();

	map<eSteelGrade, CString>::iterator it;
	for(it = newPrefixMap.begin(); it != newPrefixMap.end(); it++)
	{
		mm_SteelGradePrefix.insert(make_pair(it->first, it->second));
	}
}

CString MSSteelGradeManager::GetPrintName(eSteelGrade eSGrade, bool bChinaUsed)
{
	CString sPrefix = STR_PRE_FIX_HD;
	map<eSteelGrade, CString>::iterator it = mm_SteelGradePrefix.find(eSGrade);
	if (it != mm_SteelGradePrefix.end())
		sPrefix = it->second;

	if (bChinaUsed == true && MSDeformedBar::sm_nRebarCode == MSDeformedBar::REBAR_MATERIAL_GB)
	{
		CString sChinaPrefix = _T("");
		if (sPrefix == STR_PRE_FIX_D)
			sChinaPrefix = _T("%%130");
		else if (sPrefix == STR_PRE_FIX_HD)
			sChinaPrefix = _T("%%131");
		else if (sPrefix == STR_PRE_FIX_SHD)
			sChinaPrefix = _T("%%132");
		else //if (sPrefix == STR_PRE_FIX_UHD)
			sChinaPrefix = _T("%%133");

		return sChinaPrefix;
	}
	else
	{
		return sPrefix;
	}
}

MSSteelGradeManager::eSteelGrade MSSteelGradeManager::GetSGrade(CString printName)
{
	if(printName == STR_SD300)				return MSSteelGradeManager::SD300;
	else if(printName == STR_SD350)		return MSSteelGradeManager::SD350;
	else if(printName == STR_SD400)		return MSSteelGradeManager::SD400;
	else if(printName == STR_SD400S)	return MSSteelGradeManager::SD400S;
	else if(printName == STR_SD400W)	return MSSteelGradeManager::SD400W;
	else if(printName == STR_SD500)		return MSSteelGradeManager::SD500;
	else if(printName == STR_SD500S)	return MSSteelGradeManager::SD500S;
	else if(printName == STR_SD500W)	return MSSteelGradeManager::SD500W;
	else if(printName == STR_SD600)		return MSSteelGradeManager::SD600;
	else if(printName == STR_SD600S)		return MSSteelGradeManager::SD600S;
	else if(printName == STR_SD700)		return MSSteelGradeManager::SD700;

	else if (printName == STR_Fe415)		return MSSteelGradeManager::Fe415;
	else if (printName == STR_Fe415D)		return MSSteelGradeManager::Fe415D;
	else if (printName == STR_Fe500)		return MSSteelGradeManager::Fe500;
	else if (printName == STR_Fe550)		return MSSteelGradeManager::Fe550;
	else if (printName == STR_Fe550D)		return MSSteelGradeManager::Fe550D;
	else if (printName == STR_Fe600)		return MSSteelGradeManager::Fe600;
	else if (printName == STR_CRS)		return MSSteelGradeManager::CRS;

	else if (printName == STR_B485B)		return MSSteelGradeManager::B485B;
	else if (printName == STR_B500B)		return MSSteelGradeManager::B500B;
	else if (printName == STR_B600A)		return MSSteelGradeManager::B600A;
	else if (printName == STR_B600B)		return MSSteelGradeManager::B600B;
	
	else if (printName == STR_HPB235)		return MSSteelGradeManager::HPB235;
	else if (printName == STR_HPB300)		return MSSteelGradeManager::HPB300;
	else if (printName == STR_HRB335)		return MSSteelGradeManager::HRB335;
	else if (printName == STR_HRBF335)		return MSSteelGradeManager::HRBF335;
	else if (printName == STR_HRB400)		return MSSteelGradeManager::HRB400;
	else if (printName == STR_HRBF400)		return MSSteelGradeManager::HRBF400;
	else if (printName == STR_RRB400)		return MSSteelGradeManager::RRB400;
	else if (printName == STR_HRB500)		return MSSteelGradeManager::HRB500;
	else if (printName == STR_HRBF500)		return MSSteelGradeManager::HRBF500;

	else if (printName == STR_SD295)		return MSSteelGradeManager::SD295;
	else if (printName == STR_SD345)		return MSSteelGradeManager::SD345;
	else if (printName == STR_SD390)		return MSSteelGradeManager::SD390;
	else if (printName == STR_SD490)		return MSSteelGradeManager::SD490;

	return MSSteelGradeManager::SD300;
}
