#include "StdAfx.h"
#include "CmdModifyModel.h"
#include "MSObject.h"
#include "MSCompoG.h"
#include "MSCompoM.h"
#include "MSRebarBuilding.h"
#include "MSLevelZone.h"
#include "MSConstructionZone.h"
#include "MSLineLevelDifG.h"
#include "MSFloor.h"
#include "MSWell.h"
#include "MSConnection.h"
#include "MSMarkM.h"
#include "MSMarkG.h"
#include "MSPolylineObject.h"
#include "MSWallEndRebarLine.h"
#include "MSSlabG.h"
#include "MSElement.h"
#include "..\RADFCore_CLI\StatusReporterWrapper.h"
#include "..\RADFCore_CLI\LoggerWrapper.h"
//
//
//CmdModifyModel::CmdModifyModel(void)
//{
//}
//
//
//CmdModifyModel::~CmdModifyModel(void)
//{
//}

//CmdModifyModelObject::CmdModifyModelObject( MSObject* modelObject, CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long FloorID , bool isGeometryModified)
//CmdModifyModelObject::CmdModifyModelObject( MSObject* modelObject , CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long FloorID , bool isGeometryModified, bool isDummyAlteration)
CmdModifyModelObject::CmdModifyModelObject( MSObject* modelObject , CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long FloorID , bool isGeometryModified, bool isDummyAlteration)
{
	_ModelObject = modelObject;
	_OriginBuffer.Create(128);
	_AfterBuffer.Create(128);

	_CommandStatus = Done;
	_CommandType = Modify;
	//Type, Floor, ElementType
	SetElementType(elementType);
	SetObjectID(ModelObjectID);
	SetFloorID(FloorID);

	MSElement* pElement = dynamic_cast<MSElement*>(modelObject);
	if (pElement != nullptr)
	{
		MSElement* pOwnerElement = pElement->GetOwnerCompoG();
		if (pOwnerElement != nullptr)
			SetOwnerCompoGID(pOwnerElement->GetID());
	}

	m_bIsGeometryModified = isGeometryModified;
	m_bResetConnections = false;
	m_bSkipTrimOld = m_bSkipTrimNew = false;
	m_bIsCheckModelSkip = false;
	m_bDummyAlteration = isDummyAlteration;
}

//CmdModifyModelObject::CmdModifyModelObject( MSObject* modelObject , CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long OwnerCompoGID, long FloorID, bool isGeometryModified, bool isCheckModelSkip)
//CmdModifyModelObject::CmdModifyModelObject( MSObject* modelObject , CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long OwnerCompoGID, long FloorID, bool isGeometryModified, bool isDummyAlteration)
CmdModifyModelObject::CmdModifyModelObject( MSObject* modelObject , CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long OwnerCompoGID, long FloorID, bool isGeometryModified, bool isCheckModelSkip, bool isDummyAlteration)
{
	if(elementType == CmdProcess::CommandUpdateElementType::CompoG)
	{
		MSCompoG* compoG = (MSCompoG*)modelObject;
		if(compoG->GetType() == MSElement::msLineLevelDifG)
		{
			MSLineLevelDifG* pLevelDifG = (MSLineLevelDifG*)compoG;
			_ModelObject = pLevelDifG->mp_LeftSlab;
			ModelObjectID = pLevelDifG->mp_LeftSlab->m_ID;
		}
		else
			_ModelObject = modelObject;
	}
	else
	{
		MSElement* element = dynamic_cast<MSElement*>(modelObject);
		if(element != NULL && element->GetType() == MSElement::msWell)
		{
			MSWell* well = dynamic_cast<MSWell*>(element);
			if (well != nullptr)
			{
				if (well->m_pOwner != nullptr)
					OwnerCompoGID = well->m_pOwner->m_ID;

				_ModelObject = modelObject;
				ModelObjectID = well->m_ID;
				elementType = CmdProcess::CommandUpdateElementType::Well;
			}
		}
		else
			_ModelObject = modelObject;
	}
	_OriginBuffer.Create(128);
	_AfterBuffer.Create(128);

	_CommandStatus = Done;
	_CommandType = Modify;
	//Type, Floor, ElementType
	SetElementType(elementType);
	SetObjectID(ModelObjectID);
	SetFloorID(FloorID);
	SetOwnerCompoGID(OwnerCompoGID);

	m_bIsGeometryModified = isGeometryModified;
	m_bResetConnections = false;
	m_bSkipTrimOld = m_bSkipTrimNew = false;
	m_bIsCheckModelSkip = isCheckModelSkip;
	m_bDummyAlteration = isDummyAlteration;
}

void CmdModifyModelObject::StartEdit()
{
	if(m_bDummyAlteration)
		return;

	_ModelObject->Dump(_OriginBuffer);

	/////////////////////////////////////////////////////////////////////////////
	MSCompoG* compoG = dynamic_cast<MSCompoG*>(_ModelObject);
	if (compoG != NULL) m_bSkipTrimOld = compoG->GetRigidMember();
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	MSElement* element = dynamic_cast<MSElement*>(_ModelObject);
	if (element != NULL)
	{
		CString log;
		log.Format(L"ELEMENT MODIFY :(OLD) %s", element->ToString());
		LoggerWrapper::Log(log);
	}
	/////////////////////////////////////////////////////////////////////////////
}

void CmdModifyModelObject::EndEdit()
{
	if(m_bDummyAlteration)
	{
		RegisterCommand();
		return;
	}

	// Check Data
	if (m_bIsCheckModelSkip == false && !CheckData())
	{
		_OriginBuffer.Init(); _ModelObject->Recovery(_OriginBuffer);
		return;
	}
	_ModelObject->Dump(_AfterBuffer);
	/////////////////////////////////////////////////////////////////////////////
	MSElement* element = dynamic_cast<MSElement*>(_ModelObject);
	if (element != NULL)
	{
		CString log;
		log.Format(L"ELEMENT MODIFY :(NEW) %s", element->ToString());
		LoggerWrapper::Log(log);
	}
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	MSCompoG* compoG = dynamic_cast<MSCompoG*>(_ModelObject);
	if (compoG != NULL) m_bSkipTrimNew = compoG->GetRigidMember();
	if (m_bSkipTrimOld != m_bSkipTrimNew) m_bResetConnections = true;
	if (compoG != nullptr && m_bIsGeometryModified)
		compoG->DelCompoGZLevelInFloor();
	/////////////////////////////////////////////////////////////////////////////

	if(GetElementType() == CmdProcess::CommandUpdateElementType::LevelZone)
	{
		MSLevelZone* zone = (MSLevelZone*)_ModelObject;
		MSFloor* floor = dynamic_cast<MSFloor*>(zone->mp_Owner);
		if (floor != NULL) floor->Modify(zone);
	}

	if (m_bIsGeometryModified)
		TrimAgain();

	RegisterCommand();
}

void CmdModifyModelObject::EndEdit_Force()
{
	if (m_bDummyAlteration)
	{		
		RegisterCommand();
		return;
	}

	// Check Data
	/*
	if (!CheckData())
	{
		// do nothing
	}
	*/
	_ModelObject->Dump(_AfterBuffer);

	/////////////////////////////////////////////////////////////////////////////
	MSElement* element = dynamic_cast<MSElement*>(_ModelObject);
	if (element != NULL)
	{
		CString log;
		log.Format(L"ELEMENT MODIFY :(NEW) %s", element->ToString());
		LoggerWrapper::Log(log);
	}
	/////////////////////////////////////////////////////////////////////////////

	MSCompoG* compoG = dynamic_cast<MSCompoG*>(_ModelObject);
	if (compoG != nullptr && m_bIsGeometryModified)
		compoG->DelCompoGZLevelInFloor();

	if(GetElementType() == CmdProcess::CommandUpdateElementType::LevelZone)
	{
		MSLevelZone* zone = (MSLevelZone*)_ModelObject;
		MSFloor* floor = dynamic_cast<MSFloor*>(zone->mp_Owner);
		if (floor != NULL) floor->Modify(zone);
	}

	if (m_bIsGeometryModified)
		TrimAgain();

	RegisterCommand();
}

void CmdModifyModelObject::EndEdit_SkipTrimAgain()
{
	// Check Data
	if (m_bIsCheckModelSkip == false && !CheckData())
	{
		_OriginBuffer.Init(); _ModelObject->Recovery(_OriginBuffer);
		return;
	}

	_ModelObject->Dump(_AfterBuffer);
	/////////////////////////////////////////////////////////////////////////////
	MSElement* element = dynamic_cast<MSElement*>(_ModelObject);
	if (element != NULL)
	{
		CString log;
		log.Format(L"ELEMENT MODIFY :(NEW) %s", element->ToString());
		LoggerWrapper::Log(log);
	}
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	MSCompoG* compoG = dynamic_cast<MSCompoG*>(_ModelObject);
	if (compoG != NULL) m_bSkipTrimNew = compoG->GetRigidMember();
	if (m_bSkipTrimOld != m_bSkipTrimNew) m_bResetConnections = true;
	/////////////////////////////////////////////////////////////////////////////

	if(GetElementType() == CmdProcess::CommandUpdateElementType::LevelZone)
	{
		MSLevelZone* zone = (MSLevelZone*)_ModelObject;
		MSFloor* floor = dynamic_cast<MSFloor*>(zone->mp_Owner);
		if (floor != NULL) floor->Modify(zone);
	}

	//if (m_bIsGeometryModified)
	//	TrimAgain();

	RegisterCommand();
}

bool CmdModifyModelObject::CheckData()
{
	CmdProcess::CommandUpdateElementType updateType = GetElementType();
	if(updateType == CmdProcess::CommandUpdateElementType::CompoG)
	{
		MSCompoG* compoG = (MSCompoG*)_ModelObject;
		if (!compoG->CheckData()) { return false; }
	}
	else if(updateType == CmdProcess::CommandUpdateElementType::LevelZone)
	{
		MSLevelZone* zone = (MSLevelZone*)_ModelObject;
		if (!zone->CheckData()) { return false; }
	}
	else if (updateType == CmdProcess::CommandUpdateElementType::ConstructionZone)
	{
		MSConstructionZone* zone = (MSConstructionZone*)_ModelObject;
		if (!zone->CheckData(zone->m_ID)) { return false; }
	}
	else if(updateType == CmdProcess::CommandUpdateElementType::Floor)
	{
		MSFloor* floor = (MSFloor*)_ModelObject;
		if (!floor->CheckData()) { return false; }
	}
	else if (updateType == CmdProcess::CommandUpdateElementType::CompoM)
	{
		MSCompoM* compoM = (MSCompoM*)_ModelObject;
		if (!compoM->CheckData()) { return false; }
	}

	return true;
}

void CmdModifyModelObject::Doit()
{
	if (m_bDummyAlteration)
		return;

	/////////////////////////////////////////////////////////////////////////////
	MSElement* element = dynamic_cast<MSElement*>(_ModelObject);
	if (element != NULL)
	{
		CString log;
		log.Format(L"ELEMENT MODIFY REDO:(OLD) %s", element->ToString());
		LoggerWrapper::Log(log);
	}
	/////////////////////////////////////////////////////////////////////////////
	_CommandStatus = Done;
	_AfterBuffer.Init();
	_ModelObject->Recovery(_AfterBuffer);

	/////////////////////////////////////////////////////////////////////////////
	element = dynamic_cast<MSElement*>(_ModelObject);
	if (element != NULL)
	{
		CString log;
		log.Format(L"ELEMENT MODIFY REDO:(NEW) %s", element->ToString());
		LoggerWrapper::Log(log);
	}
	/////////////////////////////////////////////////////////////////////////////

	MSCompoG* compoG = dynamic_cast<MSCompoG*>(_ModelObject);
	if (compoG != nullptr && m_bIsGeometryModified)
		compoG->DelCompoGZLevelInFloor();

	if(GetElementType() == CmdProcess::CommandUpdateElementType::LevelZone)
	{
		MSLevelZone* zone = (MSLevelZone*)_ModelObject;
		MSFloor* floor = dynamic_cast<MSFloor*>(zone->mp_Owner);
		if (floor != NULL) floor->Modify(zone);
	}

	if (m_bIsGeometryModified)
		TrimAgain();
}

void CmdModifyModelObject::Undo()
{
	if (m_bDummyAlteration)	
		return;
	
	/////////////////////////////////////////////////////////////////////////////
	MSElement* element = dynamic_cast<MSElement*>(_ModelObject);
	if (element != NULL)
	{
		CString log;
		log.Format(L"ELEMENT MODIFY UNDO :(OLD) %s", element->ToString());
		LoggerWrapper::Log(log);
	}
	/////////////////////////////////////////////////////////////////////////////

	_CommandStatus = Undone;
	_OriginBuffer.Init();
	_ModelObject->Recovery(_OriginBuffer);

	/////////////////////////////////////////////////////////////////////////////
	element = dynamic_cast<MSElement*>(_ModelObject);
	if (element != NULL)
	{
		CString log;
		log.Format(L"ELEMENT MODIFY UNDO :(NEW) %s", element->ToString());
		LoggerWrapper::Log(log);
	}
	/////////////////////////////////////////////////////////////////////////////

	MSCompoG* compoG = dynamic_cast<MSCompoG*>(_ModelObject);
	if (compoG != nullptr && m_bIsGeometryModified)
		compoG->DelCompoGZLevelInFloor();

	if(GetElementType() == CmdProcess::CommandUpdateElementType::LevelZone)
	{
		MSLevelZone* zone = (MSLevelZone*)_ModelObject;
		MSFloor* floor = dynamic_cast<MSFloor*>(zone->mp_Owner);
		if (floor != NULL) floor->Modify(zone);
	}

	if (m_bIsGeometryModified)
		TrimAgain();
}

void CmdModifyModelObject::TrimAgain()
{
	if(GetElementType() == CmdProcess::CommandUpdateElementType::CompoG)
	{
		if (m_bResetConnections)
		{
			MSCompoG* compoG = (MSCompoG*)_ModelObject;
			for (int i=0 ; i<compoG->ma_Connections.size() ; ++i)
			{
				compoG->ma_Connections[i]->SetFirstTrimOption(MSConnection::TO_DEFAULT);
				compoG->ma_Connections[i]->SetSecondTrimOption(MSConnection::TO_DEFAULT);
			}
		}
		((MSCompoG*)_ModelObject)->ModifyObject();
	}
	else if (GetElementType() == CmdProcess::CommandUpdateElementType::CustomSection)
	{
		// CustomSection의 Profile이 변경되었을 경우에만 IsGeometryModified()가 true이므로, 
		// _OriginBuffer와 _AfterButter를 생성해서 Recovery를 한 후, IsGeometryDifferent를 비교할 필요가 없다.
		if (this->IsGeometryModified())
		{
			MSBaseBuilding* building = _ModelObject->GetBuilding();
			vector<MSCustomM*> customCompoMs = building->GetCompoMsHasCustomSectionID(_ObjectID);
			for (int i = 0; i < customCompoMs.size(); i++)
			{
				MSCustomM* compoM = customCompoMs[i];
				vector<MSCompoG*> list;
				building->GetAllCompoGsWithCompoM(compoM, list);
				for (int i = 0; i < list.size(); ++i)
				{
					list[i]->UpdateVecCenter();
					list[i]->ModifyObject();
				}
			}
		}
	}
	else if(GetElementType() == CmdProcess::CommandUpdateElementType::CompoM)
	{
		MSBaseBuilding* building = ((MSCompoM*)_ModelObject)->mp_Owner;
		MSCompoM* before = building->CreateNewCompoM(((MSCompoM*)_ModelObject)->m_nType);
		MSCompoM* after = building->CreateNewCompoM(((MSCompoM*)_ModelObject)->m_nType);
		_OriginBuffer.Init();
		_AfterBuffer.Init();
		before->Recovery(_OriginBuffer);
		after->Recovery(_AfterBuffer);
		bool trim = false;
		if (before->IsGeometryDifferent(after))
			trim = true;

		MSCompoM* compoM = (MSCompoM*)_ModelObject;
		if (trim)
		{
			if(compoM->GetType() == MSCompoM::msColumnHeaderM)
			{
				vector<MSCompoM*> ColumnMList = building->GetAllColumnMsWidthColumnHeaderMID(compoM->m_ID);
				for (int i = 0; i < ColumnMList.size(); i++)
				{
					vector<MSCompoG*> list;
					building->GetAllCompoGsWithCompoM(ColumnMList[i], list);
					for (int i = 0; i < list.size(); ++i)
					{
						list[i]->UpdateVecCenter();
						list[i]->ModifyObject();
					}
				}
			}
			else
			{
				MSBaseBuilding* pBuilding = compoM->mp_Owner;
				double dPreWidth = before->GetWidth();
				double dCurWidth = after->GetWidth();
				vector<MSCompoG*> list;
				pBuilding->GetAllCompoGsWithCompoM(compoM, list);
				for (int i = 0; i < list.size(); ++i)
				{
					list[i]->UpdateVecCenter();
					list[i]->ModifyObject();
				}
			}
		}
		delete before;
		delete after;
	}
	else if(GetElementType() == CmdProcess::CommandUpdateElementType::MarkM)
	{
		MSMarkM* markM = (MSMarkM*)_ModelObject;
		MSRebarBuilding* building = dynamic_cast<MSRebarBuilding*>(markM->GetBuilding());
		if (building != nullptr)
		{
			vector<MSMarkG*> markGs = building->GetMarkGsfromMID(markM->m_ID);
			for (int i = 0; i < markGs.size(); i++)
			{
				MSFloor::CompoGIDList_NeedRegen_2DView_CurTR().push_back(markGs[i]->m_ID);;
			}
		}
	}
	else if (GetElementType() == CmdProcess::CommandUpdateElementType::Connection)
	{
		MSConnection* pConnection = (MSConnection*)_ModelObject;
		pConnection->UpdateMembers();
		MSCompoG* pSecondCompoG = pConnection->GetSecondCompoG();
		if(pSecondCompoG != nullptr)
			pSecondCompoG->ModifyObject();
		//((MSConnection*)_ModelObject)->GetFirstCompoG()
		//((MSConnection*)_ModelObject)->GetSecondCompoG()
	}
	else if (GetElementType() == CmdProcess::CommandUpdateElementType::WallEndRebarLine)
	{
		MSWallEndRebarLine* pEndRebarLine = (MSWallEndRebarLine*)_ModelObject;
		vector<int> wallGIDs = pEndRebarLine->GetBuilding()->GetAllWallGsWidthRebarLineID(pEndRebarLine->m_ID);
		for (int i = 0; i < wallGIDs.size(); i++)
		{
			MSFloor::CompoGIDList_NeedRegen_2DView_CurTR().push_back(wallGIDs[i]);
		}

	}

}

void CmdModifyModelObject::SetTimeStamp( CTime timeStamp )
{
	//수정시에도??
}



void CmdModifyModelObjectControl::StartEdit( MSObject* msObject, CmdProcess::CommandUpdateElementType elementType, long ModelID, long FloorID, bool IsGeometryModified, bool isDummyAlteration)
{
	if ( NULL != modifyModelObject)
		return;

	modifyModelObject = new CmdModifyModelObject(msObject, elementType, ModelID, FloorID,IsGeometryModified, isDummyAlteration);
	modifyModelObject->StartEdit();
}

//void CmdModifyModelObjectControl::StartEdit( MSObject* msObject, CmdProcess::CommandUpdateElementType elementType, long ModelID, long OwnerCompoGID, long FloorID, bool IsGeometryModified, bool isCheckModelSkip)
//void CmdModifyModelObjectControl::StartEdit( MSObject* msObject, CmdProcess::CommandUpdateElementType elementType, long ModelID, long OwnerCompoGID, long FloorID, bool IsGeometryModified, bool isDummyAlteration)
void CmdModifyModelObjectControl::StartEdit( MSObject* msObject, CmdProcess::CommandUpdateElementType elementType, long ModelID, long OwnerCompoGID, long FloorID, bool IsGeometryModified, bool isCheckModelSkip, bool isDummyAlteration)
{
	if ( NULL != modifyModelObject)
		return;

	//modifyModelObject = new CmdModifyModelObject(msObject, elementType, ModelID, OwnerCompoGID, FloorID, IsGeometryModified, isCheckModelSkip);
	//modifyModelObject = new CmdModifyModelObject(msObject, elementType, ModelID, OwnerCompoGID, FloorID, IsGeometryModified, isDummyAlteration);
	modifyModelObject = new CmdModifyModelObject(msObject, elementType, ModelID, OwnerCompoGID, FloorID, IsGeometryModified, isCheckModelSkip, isDummyAlteration);
	modifyModelObject->StartEdit();
}

void CmdModifyModelObjectControl::EndEdit()
{
	if ( NULL == modifyModelObject)
		return;

	modifyModelObject->EndEdit();
	
	//delete modifyModelObject;  Undo에서 사용하므로 Delete하면 안된다.
	modifyModelObject = NULL;
}

void CmdModifyModelObjectControl::EndEdit_Force()
{
	if ( NULL == modifyModelObject)
		return;

	modifyModelObject->EndEdit_Force();
	
	//delete modifyModelObject;  Undo에서 사용하므로 Delete하면 안된다.
	modifyModelObject = NULL;
}

void CmdModifyModelObjectControl::EndEdit_SkipTrimAgain()
{
	if ( NULL == modifyModelObject)
		return;

	modifyModelObject->EndEdit_SkipTrimAgain();
	
	//delete modifyModelObject;  Undo에서 사용하므로 Delete하면 안된다.
	modifyModelObject = NULL;
}

CmdModifyModelObjectControl::CmdModifyModelObjectControl()
{
	modifyModelObject = NULL;
}
