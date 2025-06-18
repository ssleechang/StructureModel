#pragma once
#include "ColumnRebarTemplate.h"
#include "BeamRebarTemplate.h"
#include "BWallRebarTemplate.h"
#include "RWallRebarTemplate.h"
#include "PitRebarTemplate.h"
#include "SlabRebarTemplate.h"
#include "IsoFootRebarTemplate.h"
#include "WallFootRebarTemplate.h"
#include "LowWallRebarTemplate.h"
#include "ButtressColumnRebarTemplate.h"
#include "TransferColumnRebarTemplate.h"
#include "TransferBeamRebarTemplate.h"
#include "TransferSlabRebarTemplate.h"
#include "DeckBeamRebarTemplate.h"

class StructureModel_CLASS RebarTemplate
{
public:
	RebarTemplate();
	~RebarTemplate(void);

	void Serialize(CArchive& ar);

	void CopyFromMe(RebarTemplate* pSource_in);

	void DataInit();

	CString m_sName;
	long m_nID;

	BaseRebarTemplate* GetColumnRebarTemplate() { return m_ColumnRebarTemplate; };
	BaseRebarTemplate* GetBWallRebarTemplate() { return m_BWallRebarTemplate; };
	BaseRebarTemplate* GetRWallRebarTemplate() { return m_RWallRebarTemplate; };
	BaseRebarTemplate* GetLowWallRebarTemplate() { return m_LowWallRebarTemplate; };
	BaseRebarTemplate* GetBeamRebarTemplate() { return m_BeamRebarTemplate; };
	BaseRebarTemplate* GetSlabRebarTemplate() { return m_SlabRebarTemplate; };
	BaseRebarTemplate* GetPitRebarTemplate() { return m_PitRebarTemplate; };
	BaseRebarTemplate* GetIsoFootRebarTemplate() { return m_IsoFootRebarTemplate; };
	BaseRebarTemplate* GetWallFootRebarTemplate() { return m_WallFootRebarTemplate; };
	BaseRebarTemplate *GetTransferColumnRebarTemplate() { return m_TransferColumnRebarTemplate; }
	BaseRebarTemplate *GetTransferBeamRebarTemplate() { return m_TransferBeamRebarTemplate; }
	BaseRebarTemplate *GetTransferSlabRebarTemplate() { return m_TransferSlabRebarTemplate; }
	DeckBeamRebarTemplate *GetDeckBeamRebarTemplate() { return m_DeckBeamRebarTemplate; }
private:

	ColumnRebarTemplate* m_ColumnRebarTemplate;
	BWallRebarTemplate* m_BWallRebarTemplate;
	RWallRebarTemplate* m_RWallRebarTemplate;
	LowWallRebarTemplate* m_LowWallRebarTemplate;
	BeamRebarTemplate* m_BeamRebarTemplate;
	SlabRebarTemplate* m_SlabRebarTemplate;
	PitRebarTemplate* m_PitRebarTemplate;
	IsoFootRebarTemplate* m_IsoFootRebarTemplate;
	WallFootRebarTemplate* m_WallFootRebarTemplate;
	TransferColumnRebarTemplate *m_TransferColumnRebarTemplate;
	TransferBeamRebarTemplate *m_TransferBeamRebarTemplate;
	TransferSlabRebarTemplate *m_TransferSlabRebarTemplate;
	DeckBeamRebarTemplate *m_DeckBeamRebarTemplate;
};


