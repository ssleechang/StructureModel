#pragma once
#include "MRObject.h"
class MRSection;

class StructureModel_CLASS MRSectionZone :
	public MRObject
{
public:
	MRSectionZone(void);
	~MRSectionZone(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRSectionZone)

	int m_SectionIdx;
	MRSection* mp_Section;
 	double m_SLoc;
	double m_ELoc;
};
