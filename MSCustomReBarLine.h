#pragma once
#include "MSRcObject.h"
#include "CoverLengthItem.h"
using namespace RebarOption;
class MSCustomSection;
class StructureModel_CLASS MSCustomReBarLine : public MSRcObject
{
public:
	MSCustomReBarLine();
	MSCustomReBarLine(MSCustomSection* pCustomSection);
	~MSCustomReBarLine();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomReBarLine)
	virtual void CopyFromMe(MSCustomReBarLine* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eOffsetType { Cover, User};

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	int		GetEdgeIndex()	{ return m_EdgeIndex; };
	int		GetUserOffset() { return m_UserOffset; };
	eOffsetType		GetOffsetType() { return m_OffsetType; };

	void		SetEdgeIndex(int edgeIndex) { m_EdgeIndex = edgeIndex; };
	void		SetUserOffset(int offset) { m_UserOffset = offset; };
	void		SetOffsetType(int offsetType) { m_OffsetType = (eOffsetType)offsetType; };

	double GetDefCover();
	double GetOffSet();
	eCoverLengthFace GetCoverType() { return m_eCoverType; };
	void SetCoverType(eCoverLengthFace coverType) { m_eCoverType = coverType; };

	GM2DCurve* GetCurve();

	void SetCustomSection(MSCustomSection* pCustomSection);
	MSCustomSection* GetCustomSection() { return m_pCustomSection; };

	CString GetConstraint();
	CString GetName();
	virtual MSBaseBuilding* GetBuilding() override;

protected:
	int m_EdgeIndex;
	eOffsetType m_OffsetType;
	int m_UserOffset;
	eCoverLengthFace m_eCoverType;

	MSCustomSection* m_pCustomSection;
};

