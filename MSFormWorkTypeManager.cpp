#include "stdafx.h"
#include "MSFormWorkTypeManager.h"
#include "..\RADFCore_CLI\AppOptionsWrapper.h"
#include "..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MSFormWorkTypeManager, MSObject, VERSIONABLE_SCHEMA | 1)

MSFormWorkTypeManager::MSFormWorkTypeManager()
{//옛날것을 받을때만 이걸로 초기화
	this->CreateDefaultData();
}

MSFormWorkTypeManager::~MSFormWorkTypeManager()
{
	DeleteFormWorkArr();
}

void MSFormWorkTypeManager::DeleteFormWorkArr()
{
	for (int index = 0; index < ma_FormWorkType.size(); index++)
	{
		delete ma_FormWorkType[index];
	}
	
	ma_FormWorkType.clear();
}


MSObject* MSFormWorkTypeManager::Copy(bool bNewID /*= true*/)
{
	MSFormWorkTypeManager* pNewObj;

	pNewObj = new MSFormWorkTypeManager();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSFormWorkTypeManager::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSFormWorkTypeManager* pSource = (MSFormWorkTypeManager*)pSource_in;

	DeleteFormWorkArr();

	for (int index = 0; index < pSource->ma_FormWorkType.size(); index++)
	{
		MSFormWorkType* pType = new MSFormWorkType();
		pType->CopyFromMe(pSource->ma_FormWorkType[index]);
		Add(pType);
	}

	MSObject::CopyFromMe(pSource, bNewID);
}
void MSFormWorkTypeManager::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	int count = ma_FormWorkType.size();
	buffer << count;
	for (int index = 0; index < ma_FormWorkType.size(); index++)
	{
		ma_FormWorkType[index]->Dump(buffer);
	}
}

void MSFormWorkTypeManager::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	int count = 0;
	buffer >> count;
	DeleteFormWorkArr();
	for (int index = 0; index < count; index++)
	{
		MSFormWorkType* pType = new MSFormWorkType();
		pType->Recovery(buffer);
		Add(pType);
	}
}

void MSFormWorkTypeManager::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		int count = 0;
		count = ma_FormWorkType.size();
		ar << count;
		for (int index = 0; index < ma_FormWorkType.size(); index++)
		{
			ma_FormWorkType[index]->Serialize(ar);
		}
	}
	else
	{
		DeleteFormWorkArr();

		int count = 0;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			MSFormWorkType* pType = new MSFormWorkType();
			pType->Serialize(ar);

			if (MSVersionInfo::GetCurrentVersion() < 20190828)
			{
				if (pType->GetName() == m_DeckTypeName)
				{
					CString language(AppOptionsWrapper::Instance()->GetLanguage());
					if (language.MakeLower() == "ko-kr")
						m_PCSlabTypeName = _T("PC슬래브");
					else
						m_PCSlabTypeName = _T("PCSlabForm");

					MSFormWorkType* pcSlabForm = new MSFormWorkType();
					pcSlabForm->SetName(m_PCSlabTypeName);
					pcSlabForm->SetColor(255, RGB(0, 0, 0));

					bool bReadOnly = FormWorkNameReadOnlyCheck(m_PCSlabTypeName);
					pcSlabForm->SetReadOnly(bReadOnly);

					Add(pcSlabForm);
				}
			}

			Add(pType);
		}
	}
}


#pragma region 초기화
void MSFormWorkTypeManager::CreateKorDefaultData()
{
	m_NoneTypeName = _T("None");
	m_RibLathTypeName = _T("Rib-Lath");
	m_DeckTypeName= _T("Deck");
	m_PCSlabTypeName = _T("PC슬래브");

	vector<MSFormWorkType*> defaultFormTypes = GetDefaultOldFormWorkTypeListAll();

	for (int i = 0; i < defaultFormTypes.size(); i++)
	{
		MSFormWorkType* form = defaultFormTypes[i];
		Add(form);
	}
}

void MSFormWorkTypeManager::CreateDefaultData()
{
	m_NoneTypeName = _T("None");
	m_RibLathTypeName = _T("Rib-Lath");
	m_DeckTypeName = _T("Deck");
	m_PCSlabTypeName = _T("PCSlabForm");

	vector<MSFormWorkType*> defaultFormTypes = GetDefaultFormWorkTypeListAll();

	for (int i = 0; i < defaultFormTypes.size(); i++)
	{
		MSFormWorkType* form = defaultFormTypes[i];
		Add(form);
	}
}

long MSFormWorkTypeManager::GetDefaultFormTypeNames(CStringArray& defaultFormTypeNames)
{
	defaultFormTypeNames.RemoveAll();

	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("합판3회")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("합판4회")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("원형")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("유로폼")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("갱폼")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("알폼")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("None")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("코팅")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("경사")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("슬래브")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("PC슬래브")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("Deck")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("합벽")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("필로티")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("지하PIT")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("노출1회")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("노출2회")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("문양")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("옥상옥탑")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("곡면")));
	defaultFormTypeNames.Add(LocalDic::GetLocal(_T("Rib-Lath")));

	//CString language(AppOptionsWrapper::Instance()->GetLanguage());

	//if (language.MakeLower() == "ko-kr")
	//	GetKorDefaultFormTypeNames(defaultFormTypeNames);
	//else
	//	GetEngDefaultFormTypeNames(defaultFormTypeNames);

	return defaultFormTypeNames.GetSize();
}

long MSFormWorkTypeManager::GetEngDefaultFormTypeNames(CStringArray& defaultFormTypeNames)
{
	defaultFormTypeNames.RemoveAll();

	defaultFormTypeNames.Add(_T("LumPanel3"));
	defaultFormTypeNames.Add(_T("LumPanel4"));
	defaultFormTypeNames.Add(_T("Round"));
	defaultFormTypeNames.Add(_T("EuroForm"));
	defaultFormTypeNames.Add(_T("GangForm"));
	defaultFormTypeNames.Add(_T("AlForm"));
	defaultFormTypeNames.Add(_T("None"));
	defaultFormTypeNames.Add(_T("Coating"));
	defaultFormTypeNames.Add(_T("Slope"));
	defaultFormTypeNames.Add(_T("SlabForm"));
	defaultFormTypeNames.Add(_T("PCSlabForm"));
	defaultFormTypeNames.Add(_T("Deck"));
	defaultFormTypeNames.Add(_T("CompositeWall"));
	defaultFormTypeNames.Add(_T("Pilotis"));
	defaultFormTypeNames.Add(_T("Pit"));
	defaultFormTypeNames.Add(_T("Exposed1"));
	defaultFormTypeNames.Add(_T("Exposed2"));
	defaultFormTypeNames.Add(_T("Pattern"));
	defaultFormTypeNames.Add(_T("RoofTop"));
	defaultFormTypeNames.Add(_T("Curved"));
	defaultFormTypeNames.Add(_T("Rib-Lath"));

	return defaultFormTypeNames.GetSize();
}

long MSFormWorkTypeManager::GetKorDefaultFormTypeNames(CStringArray& defaultFormTypeNames)
{
	defaultFormTypeNames.RemoveAll();

	defaultFormTypeNames.Add(_T("합판3회"));
	defaultFormTypeNames.Add(_T("합판4회"));
	defaultFormTypeNames.Add(_T("원형"));
	defaultFormTypeNames.Add(_T("유로폼"));
	defaultFormTypeNames.Add(_T("갱폼"));
	defaultFormTypeNames.Add(_T("알폼"));
	defaultFormTypeNames.Add(_T("None"));
	defaultFormTypeNames.Add(_T("코팅"));
	defaultFormTypeNames.Add(_T("경사"));
	defaultFormTypeNames.Add(_T("슬래브"));
	defaultFormTypeNames.Add(_T("PC슬래브"));
	defaultFormTypeNames.Add(_T("Deck"));
	defaultFormTypeNames.Add(_T("합벽"));
	defaultFormTypeNames.Add(_T("필로티"));
	defaultFormTypeNames.Add(_T("지하PIT"));
	defaultFormTypeNames.Add(_T("노출1회"));
	defaultFormTypeNames.Add(_T("노출2회"));
	defaultFormTypeNames.Add(_T("문양"));
	defaultFormTypeNames.Add(_T("옥상옥탑"));
	defaultFormTypeNames.Add(_T("곡면"));
	defaultFormTypeNames.Add(_T("Rib-Lath"));

	return defaultFormTypeNames.GetSize();
}

vector<COLORREF> MSFormWorkTypeManager::CreateColorArr()
{
	vector<COLORREF> arr;
	arr.push_back(RGB(255, 255, 0)); //Yellow
	arr.push_back(RGB(0, 128, 0)); //Green
	arr.push_back(RGB(255, 182, 193)); //LightPink
	arr.push_back(RGB(255, 165, 0)); //Orange
	arr.push_back(RGB(0, 139, 139)); //DarkCyan
	arr.push_back(RGB(255, 0, 255)); //Magenta
	arr.push_back(RGB(119, 136, 153)); //LightSlateGray
	arr.push_back(RGB(0, 0, 255)); //Blue
	arr.push_back(RGB(128, 0, 128)); //Purple
	arr.push_back(RGB(154, 205, 50)); //YellowGreen
	arr.push_back(RGB(0, 0, 0)); //Black
	arr.push_back(RGB(191, 63, 255)); //Gold
	arr.push_back(RGB(0, 255, 255)); //AliceBlue
	arr.push_back(RGB(250, 235, 215)); //AntiqueWhite
	arr.push_back(RGB(255, 0, 0)); //Red
	arr.push_back(RGB(192, 192, 192)); //Silver
	arr.push_back(RGB(255, 250, 250)); //Snow
	arr.push_back(RGB(255, 99, 71)); //Tomato
	arr.push_back(RGB(238, 130, 238)); //Violet
	arr.push_back(RGB(46, 139, 87)); //SeaGreen
	arr.push_back(RGB(255, 69, 0)); //OrangeRed

	return arr;
}
#pragma endregion

#pragma region 매니저 정보 얻어오기
vector<MSFormWorkType*> MSFormWorkTypeManager::GetFormWorkTypeListAll()
{
	return ma_FormWorkType;
}

vector<CString> MSFormWorkTypeManager::GetFormWorkTypeNameListAll()
{
	vector<CString> formWorkTypeNameListAll;

	for (int index = 0; index < ma_FormWorkType.size(); index++)
	{
		CString formWorkNameNew = ma_FormWorkType[index]->GetName();

		formWorkTypeNameListAll.push_back(formWorkNameNew);
	}

	return formWorkTypeNameListAll;
}

vector<MSFormWorkType*> MSFormWorkTypeManager::GetDefaultFormWorkTypeListAll()
{
	CStringArray defaultFormTypeNames;
	GetDefaultFormTypeNames(defaultFormTypeNames);

	vector<COLORREF> colors;
	colors = CreateColorArr();

	vector<MSFormWorkType*> defaultFormTypes;

	for (int i = 0; i < defaultFormTypeNames.GetSize(); i++)
	{
		MSFormWorkType* form = new MSFormWorkType();
		form->SetName(defaultFormTypeNames[i]);
		form->SetColor(255, colors[i]);

		bool bReadOnly = FormWorkNameReadOnlyCheck(defaultFormTypeNames[i]);
		form->SetReadOnly(bReadOnly);

		defaultFormTypes.push_back(form);
	}

	return defaultFormTypes;
}

vector<MSFormWorkType*> MSFormWorkTypeManager::GetDefaultOldFormWorkTypeListAll()
{
	CStringArray defaultFormTypeNames;
	GetKorDefaultFormTypeNames(defaultFormTypeNames);

	vector<COLORREF> colors;
	colors = CreateColorArr();

	vector<MSFormWorkType*> defaultOldFormTypes;

	for (int i = 0; i < defaultFormTypeNames.GetSize(); i++)
	{
		MSFormWorkType* form = new MSFormWorkType();
		form->SetName(defaultFormTypeNames[i]);
		form->SetColor(255, colors[i]);

		bool bReadOnly = FormWorkNameReadOnlyCheck(defaultFormTypeNames[i]);
		form->SetReadOnly(bReadOnly);

		defaultOldFormTypes.push_back(form);
	}

	return defaultOldFormTypes;
}

MSFormWorkType* MSFormWorkTypeManager::GetFormWorkTypeByName(CString formWorkName)
{
	for (int index = 0; index < ma_FormWorkType.size(); index++)
	{
		CString formWorkNameNew = ma_FormWorkType[index]->GetName();

		if (formWorkNameNew == formWorkName)
			return ma_FormWorkType[index];
	}

	return NULL;
}

MSFormWorkType* MSFormWorkTypeManager::FormWorkTypeAdd(CString formWorkName)
{
	MSFormWorkType* pFormType = new MSFormWorkType();
	pFormType->SetName(formWorkName);

	bool bReadOnly = FormWorkNameReadOnlyCheck(formWorkName);
	pFormType->SetReadOnly(bReadOnly);

	Add(pFormType);

	return pFormType;
}

CString MSFormWorkTypeManager::GetNoneFormWorkTypeName()
{
	return m_NoneTypeName;
}

CString MSFormWorkTypeManager::GetRibLathFormWorkTypeName()
{
	return m_RibLathTypeName;
}

CString MSFormWorkTypeManager::GetDeckFormWorkTypeName()
{
	return m_DeckTypeName;
}

CString MSFormWorkTypeManager::GetPCSlabFormWorkTypeName()
{
	return m_PCSlabTypeName;
}

bool MSFormWorkTypeManager::FormWorkNameExistCheck(CString formWorkName)
{
	MSFormWorkType* pFormWorkType = GetFormWorkTypeByName(formWorkName);

	if (pFormWorkType == NULL) return false;

	return true;
}

bool MSFormWorkTypeManager::FormWorkNameReadOnlyCheck(CString formWorkName)
{
	if (formWorkName == m_NoneTypeName || formWorkName == m_RibLathTypeName || formWorkName == m_DeckTypeName || formWorkName == m_PCSlabTypeName)
		return true;

	return false;
}

#pragma endregion

#pragma region 매니저 정보 수정
void MSFormWorkTypeManager::Add(MSFormWorkType* pFormType)
{
	for (int i = 0; i < ma_FormWorkType.size(); i++)
	{
		if (ma_FormWorkType[i]->GetName() == pFormType->GetName())
			return;
	}

	ma_FormWorkType.push_back(pFormType);
}

long MSFormWorkTypeManager::Remove(MSFormWorkType* pFormType)
{
	CString delFormWorkName = pFormType->GetName();

	vector<MSFormWorkType*>::iterator it;


    //ChangedName 물고있는 애가 지워질때는 해당 FormworkType의 이름으로 바꿔준다
    for (it = ma_FormWorkType.begin(); it != ma_FormWorkType.end(); it++)
    {
        CString changedName = (*it)->GetChangedName();

        if (changedName == delFormWorkName)
            (*it)->SetChangedName((*it)->GetName());
    }

	for (it = ma_FormWorkType.begin(); it != ma_FormWorkType.end(); it++)
	{
		CString formWorkName = (*it)->GetName();

		if (formWorkName == delFormWorkName)
		{
			ma_FormWorkType.erase(it);
			return 0;
		}
	}

	return -1;
}
#pragma endregion
