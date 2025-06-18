#pragma once
#include "MSRcObject.h"
#include "MSCustomSection.h"

class MSRebarBuilding;
class StructureModel_CLASS MSCustomBarUnit : public MSRcObject
{
public:
	MSCustomBarUnit();
	~MSCustomBarUnit();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomBarUnit)
	virtual void CopyFromMe(MSCustomBarUnit* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	MSCustomSection* GetCustomSection() { return mp_Section; };
	MSCustomSection::eSectionType GetCustomSectionType();
	void SetCustomSection(MSCustomSection* pSection) { mp_Section = pSection; };
	virtual MSBaseBuilding* GetBuilding() override;
protected:
	MSCustomSection* mp_Section;
};

