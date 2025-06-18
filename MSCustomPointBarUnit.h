#pragma once
#include "MSCustomBarUnit.h"
#include "MSCustomLocation.h"
#include "MSCustomSection.h"

class MSCustomReBarLine;
class MSCustomRebar;

class StructureModel_CLASS MSCustomPointBarUnit : public MSCustomBarUnit
{
public:
	MSCustomPointBarUnit();
	~MSCustomPointBarUnit();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomPointBarUnit)
	virtual void CopyFromMe(MSCustomBarUnit* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum ePointBarType { MainBar, AdditionalBar, TopMainBar, BottomMainBar, SideBar};
	enum eSpacingType  { Equal, EffectiveSpacing};

	void SetBarType(ePointBarType barType) { m_eBarType = barType; };
	void SetSpacingType(eSpacingType spacingType) { m_eSpacingType = spacingType; };
	void SetStartOffset(int offset) { m_StartOffset = offset; };
	void SetEndOffset(int offset) { m_EndOffset = offset; };
	void SetCustomLocationP0(long rebarLineID1, long rebarLineID2);
	void SetCustomLocationP1(long rebarLineID1, long rebarLineID2);

	ePointBarType GetBarType()			 { return m_eBarType; };
	eSpacingType GetSpacingType() { return m_eSpacingType; };
	int GetStartOffset() { return m_StartOffset; };
	int GetEndOffset() { return m_EndOffset; };
	MSCustomLocation GetCustomLocationP0() { return m_LocationP0; };
	MSCustomLocation GetCustomLocationP1() { return m_LocationP1; };
	
	bool HasRebarLineID(long rebarLineID);
	GM2DPolyline* GetProfile();
	GM2DLineVector Get2DLineVector();

	//******************************** string static function
	static vector<CString> GetBarTypeNameList(MSCustomSection::eSectionType eType);
	static vector<CString> GetSpacingNameList();

	static int GetInputPointCount(ePointBarType barType);
	static CString GetDefaultPointRebar(MSCustomPointBarUnit::ePointBarType barType);
	//********************************

	long GetEdgeIndex();
protected:
	ePointBarType m_eBarType;
	eSpacingType m_eSpacingType;
	int m_StartOffset;
	int m_EndOffset;

	MSCustomLocation m_LocationP0;
	MSCustomLocation m_LocationP1;

};

