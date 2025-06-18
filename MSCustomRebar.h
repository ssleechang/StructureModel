#pragma once
#include "MSRcObject.h"
#include "GMLib/GM2DVector.h"
class StructureModel_CLASS MSCustomRebar : public MSRcObject
{
public:
	MSCustomRebar();
	~MSCustomRebar();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomRebar)
	virtual void CopyFromMe(MSCustomRebar* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	CString GetDiaName() { return m_DiaName; };
	GM2DVector GetLocation() { return m_Location; };

	void SetDiaName(CString diaName) { m_DiaName = diaName; };
	void SetLocation(GM2DVector location) { m_Location = location; };

protected:
	CString m_DiaName;
	GM2DVector m_Location;
};

