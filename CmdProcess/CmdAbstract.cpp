#include "StdAfx.h"
#include "CmdAbstract.h"
//#include "HistoryManager.h"
#include "CmdProcessHelper.h"

CmdAbstract::CmdAbstract()
{
	//_historyManager = historyManager;
	_ObjectID = 0;
	_FloorContainerID = 0;
	_CompoGType = 0;
	_OwnerCompoGID = 0;

}

void CmdAbstract::SetElementType(CmdProcess::CommandUpdateElementType elementType)
{
	_CommandElementType = elementType;
}

CmdProcess::CommandUpdateElementType  CmdAbstract::GetElementType()
{
	return _CommandElementType;
}

void CmdAbstract::RegisterCommand()
{
	CmdProcessHelper::GetHistoryManager()->AddCommand(this);
}

void CmdAbstract::UnRegisterCommand()
{
	CmdProcessHelper::GetHistoryManager()->DelCommand(this);
}

void CmdAbstract::SetObjectID(long ID)
{
	_ObjectID = ID;
}

long CmdAbstract::GetObjectID()
{
	return _ObjectID;
}

void CmdAbstract::SetObjectIDArray(vector<long> ID)
{
	_ObjectIDArray = ID;
}

vector<long> CmdAbstract::GetObjectIDArray()
{
	return _ObjectIDArray;
}


void CmdAbstract::SetObjectGTypeArray(vector<long> GTypes)
{
	_ObjectGTypeArray = GTypes;
}

vector<long> CmdAbstract::GetObjectGTypeArray()
{
	return _ObjectGTypeArray;
}