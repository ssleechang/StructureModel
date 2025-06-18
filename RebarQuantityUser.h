#pragma once

#include "GMLib/MSVersionInfo.h"
#include <string>
using std::string;

class StructureModel_CLASS RebarQuantityUser
{
public:
	CString strength;
	CString dia;
	double value;
	CString category;
	CString note;
	int calcPosition_LowWall;
	double length;

	RebarQuantityUser()
	{
		Init();
	};
	RebarQuantityUser(const RebarQuantityUser& src)
	{
		(*this) = src;
	};
	RebarQuantityUser& operator=(const RebarQuantityUser& src)
	{
		strength = src.strength;
		dia = src.dia;
		value = src.value;
		category = src.category;
		note = src.note;
		calcPosition_LowWall = src.calcPosition_LowWall;
		length = src.length;

		return (*this);
	};
	void Init()
	{
		strength = _T("");
		dia = _T("");
		value = 0;
		category = _T("");
		note = _T("");
		calcPosition_LowWall = 0;
		length = 0;
	};

	void Serialize( CArchive& ar )
	{
		if(ar.IsStoring())
		{
			ar << strength;
			ar << dia;
			ar << value;
			ar << category;
			ar << note;
			ar << calcPosition_LowWall;
			ar << length;
		}
		else
		{
			ar >> strength;
			ar >> dia;
			ar >> value;

			if (MSVersionInfo::GetCurrentVersion() >= 20181010)
			{
				ar >> category;
				ar >> note;
			}

			if (MSVersionInfo::GetCurrentVersion() >= 20181119)
			{
				ar >> calcPosition_LowWall;
			}

			if (MSVersionInfo::GetCurrentVersion() >= 20190201)
			{
				ar >> length;
			}
			else
			{
				length = _tstof(note);
			}
		}
	};

	void Dump(EFS::EFS_Buffer& ar)
	{
		ar << strength;
		ar << dia;
		ar << value;
		ar << category;
		ar << note;
		ar << calcPosition_LowWall;
	};

	void Recovery(EFS::EFS_Buffer& ar)
	{
		ar >> strength;
		ar >> dia;
		ar >> value;
		ar >> category;
		ar >> note;
		ar >> calcPosition_LowWall;
	}
};

