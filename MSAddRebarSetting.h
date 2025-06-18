#pragma once
#include "MSObject.h"
class StructureModel_CLASS MSAddRebarSetting :public MSObject
{
public:
	MSAddRebarSetting();
	~MSAddRebarSetting();
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSAddRebarSetting)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eDevType { Straight = 0, Hook = 1};
	enum eSpacingType { Dev = 0, Tension, Compression, Cover, UserSpacing};
	enum eSectionType { Section_S = 0, Section_HalfS, Section_Cover, Section_User};

	eDevType GetDevType() { return m_eDevType; };
	eSpacingType GetSpacingType() { return m_eSpacingType; };
	double GetUserSpacingLength() { return m_UserSpacingLength; };
	eSectionType GetSectionType() { return m_eSectionType; };
	double GetUserSectionLength() { return m_UserSectionLength; };

	void SetDevType(eDevType devType) { m_eDevType = devType; };
	void SetSpacingType(eSpacingType spacingType) { m_eSpacingType = spacingType; };
	void SetUserSpacingLength(double length) { m_UserSpacingLength = length; };
	void SetSectionType(eSectionType sectionType) { m_eSectionType = sectionType; };
	void SetUserSectionLength(double length) { m_UserSectionLength = length; };

protected:
	eDevType m_eDevType;
	eSpacingType m_eSpacingType;
	double m_UserSpacingLength;
	eSectionType m_eSectionType;
	double m_UserSectionLength;
};

