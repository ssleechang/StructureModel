#pragma once
#include "GMLib/MSVersionInfo.h"
#include <string>
#include "MSFormWorkTypeManager.h"
using std::string;

class VolumeAreaInfo
{
public:

	//Common
	int czone_id;
	bool use_user_input;

	CString user_Category;
	int calcType;
	CString note;
	double value;

	//Concrete
	bool is_burim; // used for is_burim flag
	bool isUsedStoryMaterial;
	CString materialName;

	//FormWork
	CString formWorkName;
	CString dir; // (for area), T,B,L,R,F,E,U,O - Top, Bottom, Left, Right, Front, End, Up, Other
	bool is_user_formtype;

	// 삭제 예정
	double value_hand;
	CString note_hand;
	double value_user;
	double value_covered;
	bool is_joint;
	int joint_id;
	int user_Index;

	VolumeAreaInfo()
	{
		Init();
	};
	VolumeAreaInfo(const VolumeAreaInfo& src)
	{
		(*this) = src;
	};
	VolumeAreaInfo& operator=(const VolumeAreaInfo& src)
	{
		is_burim = src.is_burim;
		czone_id = src.czone_id;
		formWorkName = src.formWorkName;
		dir = src.dir;

		value = src.value;
		value_hand = src.value_hand;
		note = src.note;
		note_hand = src.note_hand;
		use_user_input = src.use_user_input;
		value_user = src.value_user;
		value_covered = src.value_covered;

		is_joint = src.is_joint;
		joint_id = src.joint_id;

		is_user_formtype = src.is_user_formtype;
		calcType = src.calcType;

		user_Category = src.user_Category;
		user_Index = src.user_Index;

		isUsedStoryMaterial = src.isUsedStoryMaterial;
		materialName = src.materialName;

		return (*this);
	};
	void Init()
	{
		is_burim = false;
		czone_id = 0;
		formWorkName = _T("None");
		dir = _T("T");
		value = value_hand = value_user = value_covered = 0.0;
		note = "";
		note_hand = "";
		use_user_input = false;
		is_joint = false;
		joint_id = 0;
		is_user_formtype = false;
		calcType = 0;
		user_Category = "";
		user_Index = -1;
		isUsedStoryMaterial = true;
		materialName = "";
	};

	void Serialize( CArchive& ar )
	{
		int type = 0;
		if(ar.IsStoring())
		{
			ar << is_burim;
			ar << czone_id;
			ar << type;
			ar << dir;

			ar << value;
			ar << note;

			ar << value_hand;
			ar << note_hand;

			ar << use_user_input;
			ar << value_user;

			ar << value_covered;

			ar << is_joint;
			ar << joint_id;

			ar << is_user_formtype;

			ar << formWorkName;

			ar << calcType;

			ar << user_Category;
			ar << user_Index;

			ar << isUsedStoryMaterial;
			ar << materialName;
		}
		else
		{
			ar >> is_burim;
			ar >> czone_id;
			ar >> type;
			if (MSVersionInfo::GetCurrentVersion() < 20170413)
			{
				char dirchar;
				ar >> dirchar;
				dir.Format(_T("%c"), dirchar);

				//사각기둥의 index 변환작업
				if (dir == _T("0")) dir = _T("1");
				else if (dir == _T("1")) dir = _T("2");
				else if (dir == _T("2")) dir = _T("3");
				else if (dir == _T("3")) dir = _T("0");
			}
			else
			{
				ar >> dir;
			}

			ar >> value;
			ar >> note;

			ar >> value_hand;
			ar >> note_hand;

			ar >> use_user_input;
			ar >> value_user;

			ar >> value_covered;

			ar >> is_joint;
			ar >> joint_id;

			if(MSVersionInfo::GetCurrentVersion() >= 20150106)
			{
				ar >> is_user_formtype;
			}

			if (MSVersionInfo::GetCurrentVersion() < 20160226)
			{
				if (type == 12) //lib-Lath Index 12 -> 20
					type = 20;
			}

			if (MSVersionInfo::GetCurrentVersion() >= 20160905)
			{
				ar >> formWorkName;
			}
			else
			{
				if (type > 0)
				{
					CStringArray defaultFormTypeNames;
					long numFormWork = MSFormWorkTypeManager::GetKorDefaultFormTypeNames(defaultFormTypeNames);
					formWorkName = defaultFormTypeNames[type - 1];
				}
			}

			if (MSVersionInfo::GetCurrentVersion() >= 20180308)
			{
				ar >> calcType;
			}
			else
			{
				calcType = 0;
			}

			if (MSVersionInfo::GetCurrentVersion() >= 20180921)
			{
				ar >> user_Category;
				ar >> user_Index;
			}

			if (MSVersionInfo::GetCurrentVersion() >= 20181119)
			{
				ar >> isUsedStoryMaterial;
				ar >> materialName;
			}
		}
	};

	void Dump(EFS::EFS_Buffer& ar)
	{
		ar << is_burim;
		ar << czone_id;
		ar << formWorkName;
		ar << dir;

		ar << value;
		ar << note;

		ar << value_hand;
		ar << note_hand;

		ar << use_user_input;
		ar << value_user;

		ar << value_covered;

		ar << is_joint;
		ar << joint_id;

		ar << is_user_formtype;

		ar << calcType;

		ar << user_Category;
		ar << user_Index;

		ar << isUsedStoryMaterial;
		ar << materialName;
	};

	void Recovery(EFS::EFS_Buffer& ar)
	{
		ar >> is_burim;
		ar >> czone_id;
		ar >> formWorkName;
		ar >> dir;

		ar >> value;
		ar >> note;

		ar >> value_hand;
		ar >> note_hand;

		ar >> use_user_input;
		ar >> value_user;

		ar >> value_covered;

		ar >> is_joint;
		ar >> joint_id;

		ar >> is_user_formtype;

		ar >> calcType;

		ar >> user_Category;
		ar >> user_Index;

		ar >> isUsedStoryMaterial;
		ar >> materialName;
	}
};

