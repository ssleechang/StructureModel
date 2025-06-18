#pragma once
#include "MSObject.h"
class StructureModel_CLASS MSFormWorkDisplay : public MSObject
{
public:
	MSFormWorkDisplay();
	~MSFormWorkDisplay();
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFormWorkDisplay)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	COLORREF m_Color;
	long m_Alpha;
};



	
	