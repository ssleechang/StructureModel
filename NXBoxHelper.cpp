#include "stdafx.h"
#include "NXBoxHelper.h"
#include "CogReader.h"
#include "NXBoxHelper.h"
#include "NXBoxColumnHelper.h"
#include "NXBoxBeamHelper.h"
#include "NXBoxWallHelper.h"
#include "NXBoxRWallHelper.h"
#include "NXBoxSlabHelper.h"
#include "NXBoxPitHelper.h"
#include "NXBoxLowWallHelper.h"
#include "NXBoxIosFootHelper.h"
#include "NXBoxWallFootHelper.h"


/////////////////////////////////////// NXBoxData //////////////////////////////////////
NXBoxData::NXBoxData()
{

}

NXBoxData::~NXBoxData()
{

}

/////////////////////////////////////// NXBoxHelper //////////////////////////////////////
NXBoxHelper::NXBoxHelper(CogReader* pReader)
{
	mp_Reader = pReader;
	mp_Data = nullptr;
	m_IsMaked = true;
}


NXBoxHelper::~NXBoxHelper()
{
}

MSBaseBuilding* NXBoxHelper::GetBuilding()
{
	return mp_Reader->GetBuilding();
}

MSFloor* NXBoxHelper::GetFloor()
{
	return mp_Reader->GetFloor();
}

NXBoxData* NXBoxHelper::CreateNXBoxData(int memberType)
{
	switch (memberType)
	{
	case DG_BEAM:			return new NXBoxBeamData();
	case DG_COLUMN:	return new NXBoxColumnData();
	case DG_WALL:			return new NXBoxWallData();
	case DG_RWALL:		return new NXBoxRWallData();
	case DG_LOWWALL:	return new NXBoxLowWallData();
	case DG_SLAB:			return new NXBoxSlabData();
	case DG_PIT:				return new NXBoxPitData();
	case DG_ISOFOOT:		return new NXBoxIsoFootData();
	case DG_WFOOT:		return new NXBoxWallFootData();

	}
	return nullptr;
}

MSCompoM* NXBoxHelper::FindCompoM(CString name, long msCompoMType)
{
	return mp_Reader->GetCompoM(name, msCompoMType);
}

XErrorObject*  NXBoxHelper::CreateErrorObject(long figureID, int ErrorCode, int Level, CString GroupName, CString Message)
{
	vector<long> FigureIDs;	FigureIDs.push_back(figureID);
	return mp_Reader->AddErrorObject(FigureIDs, ErrorCode, Level, GroupName, Message);
}

