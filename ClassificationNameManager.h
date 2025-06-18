#pragma once
#include "ClassificationNameData.h"

class StructureModel_CLASS ClassificationNameManager
{
public:
	static ClassificationNameManager* GetInstance();
	~ClassificationNameManager(void);

	bool WriteFileBinary(CString strFilePath);
	int ReadFileBinary(CString strFilePath);
	bool WriteClassificationNameDataFileBinary(MSBaseBuilding* building, CString strFilePath);
	void ReadClassificationNameDataFileBinary(CString strFilePath);

	void Serialize(CArchive& archive);

	map<long, ClassificationNameData*> mm_ClassificationNameData;

	ClassificationNameData* GetClassificationNameDataByID(long nID);
	ClassificationNameData* GetClassificationNameDataByName(CString sName);

	void Add(ClassificationNameData* classificationNameData);
	long Remove(long nID);
	void DeleteClassificationNameDatas();

	long m_nID;
	long GetNewID();
	static long ms_LastID;
	static void SetLastID(long NewID);

private:
	static ClassificationNameManager* m_instance;
	ClassificationNameManager();	
};