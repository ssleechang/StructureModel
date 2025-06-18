#pragma once
#include "MSObject.h"
#include "StructureModel/MSElement.h"

enum class SuperMemberType
{
	COLUMN = 0,
	BWALL,
	RWALL,
	LOWWALL,
	BEAM,
	SLAB,
	PIT,
	ISOFOOT,
	WALLFOOT,
	STAIR,
	HAUNCH,
	STEELCOLUMN = 100,
	STEELBEAM,
	SRCCOLUMN = 200,
	SRCBEAM,
	NOT = 999,
};

enum class SubMemberType
{
	RECTCOLUMN = 0,
	CIRCOLUMN,
	BUTTRESS,
	CUSTOMCOLUMN,
	BWALL = 20,
	CUSTOMWALLCOLUMN,
	RWALL = 40,
	LOWWALL = 60,
	BEAM = 80,
	PCBEAM,
	CUSTOMBEAM,
	DECKBEAM,
	SLABONBEAM = 100,
	FLATSLAB,
	SOGSLAB,
	MATSLAB,
	CIRCULARRAMP,
	REBAREMBEDDEDDECK,
	GENERALDECK,
	PCSLAB,
	DROPPANEL,
	PIT = 120,
	ISOFOOT = 140,
	WALLFOOT = 160,
	STAIRSTEP = 180,
	STAIRLANDING,
	STAIRSTRINGER,
	STAIRRISER,
	STAIRSOFFIT,
	HAUNCH = 190,
	SRCRECTCOLUMN = 200,
	SRCCIRCOLUMN,
	SRCBEAM = 220,
	NOT = 999,
};

enum class FormWorkPosition
{
	SIDE = 0,
	SIDEIN,
	SIDEOUT,
	CLOSE,
	TOP,
	BOTTOM,
	BACKFILL,
	SLOPE,
	STARTEND,
	ENDEND,
};

class StructureModel_CLASS FormWorkTemplate : public MSObject
{
public:

	FormWorkTemplate(void);
	~FormWorkTemplate(void);
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(FormWorkTemplate)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	

	struct FormWorkSettingData
	{
		FormWorkSettingData(SuperMemberType iSuperMember, SubMemberType iMember, FormWorkPosition iPosition, CString iForm) :
			superMember(iSuperMember), subMember(iMember), formPosition(iPosition), form(iForm) {}
			
		SuperMemberType superMember;
		SubMemberType subMember;
		FormWorkPosition formPosition;
		CString form;
	};

	long m_nID;
	CString m_sName;

	void InitSettingData();

	vector<FormWorkSettingData*> AllFormWorkSettingData;
};

