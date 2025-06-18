#pragma once
#include "MSObject.h"
#include "MSFormWorkType.h"

class StructureModel_CLASS MSFormWorkTypeManager : 	public MSObject
{
public:
	MSFormWorkTypeManager();
	MSFormWorkTypeManager(long lang);
	~MSFormWorkTypeManager();
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFormWorkTypeManager)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	enum LangCode { ko_Kr =0, en_US };

	void DeleteFormWorkArr();

	//초기화
	void CreateKorDefaultData();
	void CreateDefaultData();
	vector<COLORREF> CreateColorArr();

	void Add(MSFormWorkType* pFormType);
	long Remove(MSFormWorkType* pFormType);

	//매니저 정보 얻어오기
	vector<MSFormWorkType*> GetFormWorkTypeListAll();
	vector<CString> GetFormWorkTypeNameListAll();
	vector<MSFormWorkType*> GetDefaultFormWorkTypeListAll();
	vector<MSFormWorkType*> GetDefaultOldFormWorkTypeListAll();
	MSFormWorkType* GetFormWorkTypeByName(CString formWorkName);

	MSFormWorkType* FormWorkTypeAdd(CString formWorkName);
	CString GetNoneFormWorkTypeName();
	CString GetRibLathFormWorkTypeName();
	CString GetDeckFormWorkTypeName();
	CString GetPCSlabFormWorkTypeName();
	bool FormWorkNameExistCheck(CString formWorkName);
	bool FormWorkNameReadOnlyCheck(CString formWorkName);

	static long GetEngDefaultFormTypeNames(CStringArray& defaultFormTypeNames);
	static long GetDefaultFormTypeNames(CStringArray& defaultFormTypeNames);
	static long GetKorDefaultFormTypeNames(CStringArray& defaultFormTypeNames);

private:
	vector<MSFormWorkType*> ma_FormWorkType;
	LangCode m_Language;
	CString m_NoneTypeName;
	CString m_RibLathTypeName;
	CString m_DeckTypeName;
	CString m_PCSlabTypeName;
};

