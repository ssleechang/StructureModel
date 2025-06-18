#pragma once
#include "MSObject.h"
#include "StructureModel/MSElement.h"
#include "StructureModel/NayukConcreteSetting.h"
#include "StructureModel/NayukFormworkSetting.h"
#include "StructureModel/NayukRebarSetting.h"
#include "StructureModel/MSBaseBuilding.h"

class StructureModel_CLASS NayukTemplate : public MSObject
{
public:
	NayukTemplate(MSBaseBuilding* building);
	~NayukTemplate();

	void Serialize(CArchive& archive);
	//DECLARE_SERIAL(NayukTemplate) : 이건 정확히 모르겠어서 주석처리합니다. 같이 처리해주세요 팀장님.
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	long m_nID;
	CString m_sName;

	NayukConcreteSetting* m_ConcSetting;
	NayukFormworkSetting* m_FormSetting;
	NayukRebarSetting* m_RebarSetting;

	void InitData(MSBaseBuilding* building);
};

