#pragma once
#include "msobject.h"

class MRSection;
class MSCompoM;
class StructureModel_CLASS MRCompoM :
	public MSObject
{
public:
	MRCompoM(void);
	~MRCompoM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRCompoM)

	long m_ID;

	MSCompoM* mp_MSM;
	CTypedPtrArray<CObArray, MRSection*> ma_Section;
	void DeleteSections();
	void AddSection(MRSection* pSection);
};
