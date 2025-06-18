#pragma once
#include "OldColumnRebarTemplate.h"
#include "OldBeamRebarTemplate.h"
#include "OldBWallRebarTemplate.h"
#include "OldRWallRebarTemplate.h"
#include "OldPitRebarTemplate.h"
#include "OldSlabRebarTemplate.h"
#include "OldIsoFootRebarTemplate.h"
#include "OldWallFootRebarTemplate.h"
#include "..\MSObject.h"

class StructureModel_CLASS OldCommonRebarTemplate : public MSObject
{
public:
	OldCommonRebarTemplate();
	~OldCommonRebarTemplate(void);

	void DataInit();

	OldColumnRebarTemplate* m_ColumnRebarTemplate;
	OldBeamRebarTemplate* m_BeamRebarTemplate;
	OldBWallRebarTemplate* m_BWallRebarTemplate;
	OldRWallRebarTemplate* m_RWallRebarTemplate;
	OldPitRebarTemplate* m_PitRebarTemplate;
	OldSlabRebarTemplate* m_SlabRebarTemplate;
	OldIsoFootRebarTemplate* m_IsoFootRebarTemplate;
	OldWallFootRebarTemplate* m_WallFootRebarTemplate;
};

