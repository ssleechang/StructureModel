#include "stdafx.h"
#include "UserState.h"
#include "..\MSObject.h"
#include "CmdProcessHelper.h"
#include "..\MSConnection.h"
UserState::UserState(void)
{
	Previous = NULL;
	Next = NULL;
	ID = 0;
	
	_isDeleteHistoryObjects = false;
}


UserState::~UserState(void)
{
	Dispose();
}


void UserState::Dispose()
{
	_AddCommandList.clear();
	_DeleteCommandList.clear();
	_ModifyCommandList.clear();

	for (auto iter = _mapHistoryObjects.begin(); iter != _mapHistoryObjects.end(); iter++)
	{
		if (iter->second->IsDeleted() || _isDeleteHistoryObjects)
		{
			HistoryManager::_mapHistoryObjectForClear[ iter->second ] = true;
			//delete iter->second;
		}
	}
	_mapHistoryObjects.clear();

	

	//if (NULL != Next)//by LSS, TrimState에서 while 루프로 지운다. 재귀적으로 작성할경우, state가 수천개가 넘어가게 되면 stack overflow가 발생한다.
	//	delete Next;
}

void UserState::Redo()
{
	

	_cmdProcessor.ReDo();

}

void UserState::Undo()
{	
	_cmdProcessor.UnDo();

}

void UserState::AddCommand(CmdAbstract* command)
{
	if (CmdProcessHelper::GetHistoryManager()->IsInUndoRedo()) return;

	eCommandType currentCommandType = command->GetCommandType();

	switch (currentCommandType)
	{
	case eCommandType::Add:
		AddCommandTypeAdd(command);
		break;
	case eCommandType::Modify:
		AddCommandTypeModify(command);
		break;
	case eCommandType::Delete:
		AddCommandTypeDelete(command);
		break;
	case eCommandType::DeleteMulti:
		AddCommandTypeDeleteArray(command);
		break;
	case eCommandType::AddMulti:
		AddCommandTypeAddArray(command);
		break;
	}
	_cmdProcessor.AddCommand(command);
}

void UserState::DelCommand(CmdAbstract* command)
{
	if (CmdProcessHelper::GetHistoryManager()->IsInUndoRedo()) return;

	eCommandType currentCommandType = command->GetCommandType();

	switch (currentCommandType)
	{
	case eCommandType::Delete:
		DelCommandTypeDelete(command);
		break;
	}
	_cmdProcessor.DelCommand(command);
}

UserState* UserState::FindUserState(int stateID, bool isNext)
{
	if (true == isNext)
	{
		if (NULL == Next)
			return NULL;

		UserState* next = Next;
		while (next != NULL)
		{
			if (next->ID == stateID)
				return next;

			next = next->GetNext();
		}
	}
	else
	{
		if (NULL == Previous)
			return NULL;
		
		UserState* previous = Previous;
		while (previous != NULL)
		{
			if (previous->ID == stateID)
				return previous;

			previous = previous->GetPrevious();
		}		
	}
	return NULL;
}

void UserState::AddCommandTypeAdd(CmdAbstract* command)
{
	CommandElement currentData = std::make_tuple((CmdProcess::CommandUpdateElementType)command->GetElementType(), command->GetObjectID(), command->GetFloorID(), command->GetCompoGType(), command->GetOwnerCompoGID());
	_AddCommandList.push_back(currentData);
}

void UserState::AddCommandTypeDelete(CmdAbstract* command)
{
	CommandElement currentData = std::make_tuple(command->GetElementType(), command->GetObjectID(), command->GetFloorID(), command->GetCompoGType(), command->GetOwnerCompoGID());

	CommandListIter SamePairIter = FindPairFromAddList(&currentData);
	if (SamePairIter != _AddCommandList.end())//존재하는 경우
	{
		_AddCommandList.erase(SamePairIter);
	}
	else
	{
		_DeleteCommandList.push_back(currentData);
	}
}

void UserState::DelCommandTypeDelete(CmdAbstract* command)
{
	CommandElement currentData = std::make_tuple(command->GetElementType(), command->GetObjectID(), command->GetFloorID(),command->GetCompoGType(), command->GetOwnerCompoGID());

	CommandListIter SamePairIter = FindPairFromDelList(&currentData);
	if (SamePairIter != _DeleteCommandList.end())//존재하는 경우
	{
		_DeleteCommandList.erase(SamePairIter);
	}
}

void UserState::AddCommandTypeDeleteArray(CmdAbstract* command)
{
	vector<long> id_list = command->GetObjectIDArray();
	vector<long> gType_list = command->GetObjectGTypeArray();
	for (int i = 0; i < id_list.size(); ++i)
	{
		long ID = id_list[i];
		long gType = gType_list[i];

		CommandElement currentData = std::make_tuple(command->GetElementType(), ID, command->GetFloorID(), gType, command->GetOwnerCompoGID());

		CommandListIter SamePairIter = FindPairFromAddList(&currentData);
		if (SamePairIter != _AddCommandList.end())//존재하는 경우
		{
			_AddCommandList.erase(SamePairIter);
		}
		else
		{
			_DeleteCommandList.push_back(currentData);
		}
	}
}

void UserState::AddCommandTypeAddArray(CmdAbstract* command)
{
	vector<long> id_list = command->GetObjectIDArray();
	vector<long> gType_list = command->GetObjectGTypeArray();
	for (int i = 0; i < id_list.size(); ++i)
	{
		long ID = id_list[i];
		long gType = gType_list[i];
		CommandElement currentData = std::make_tuple(command->GetElementType(), ID, command->GetFloorID(), gType, command->GetOwnerCompoGID());
		_AddCommandList.push_back(currentData);
	}
}

void UserState::AddCommandTypeModify(CmdAbstract* command)
{
	//_ModifyCommandList.push_back(currentData);
	bool bGeometryModified = true;
	bool bDummyAlteration = false;

	CmdModifyAbstract* modifyCmd = dynamic_cast<CmdModifyAbstract*>(command);
	if (modifyCmd != NULL)
	{
		bGeometryModified = modifyCmd->IsGeometryModified();
		bDummyAlteration = modifyCmd->IsDummyAlteration();
	}


	//ModifyElement element = std::make_tuple((CmdProcess::CommandUpdateElementType)command->GetElementType(), command->GetOldValue(), command->GetObjectID(), command->GetFloorID(), command->GetOwnerCompoGID(), bGeometryModified);
	//ModifyElement element = std::make_tuple((CmdProcess::CommandUpdateElementType)command->GetElementType(), command->GetOldValue(), command->GetObjectID(), command->GetOwnerCompoGID(), bGeometryModified, bDummyAlteration);
	ModifyElement element = std::make_tuple((CmdProcess::CommandUpdateElementType)command->GetElementType(), command->GetOldValue(), command->GetObjectID(), command->GetFloorID(), command->GetOwnerCompoGID(), bGeometryModified, bDummyAlteration);
	_ModifyCommandList.push_back(element);
}

UserState::CommandListIter UserState::FindPairFromAddList(CommandElement* compareData)
{
	return FindPairFromCommandList(compareData, _AddCommandList);
}

UserState::CommandListIter UserState::FindPairFromDelList(CommandElement* compareData)
{
	return FindPairFromCommandList(compareData, _DeleteCommandList);
}


UserState::CommandListIter UserState::FindPairFromCommandList(CommandElement* compareData, CommandContainer& cmdContainer)
{
	CommandListIter iterLoop;

	CmdProcess::CommandUpdateElementType ModelType;
	int ID;
	int ContainerFloorID;
	int OwnerID;
	int CompoGType;

	CmdProcess::CommandUpdateElementType CompareModelType;
	int CompareID;
	int CompareContainerFloorID;
	int CompareOwnerID;
	int CompareCompoGType;

	std::tie(CompareModelType, CompareID, CompareContainerFloorID, CompareCompoGType, CompareOwnerID) = *compareData;

	for (iterLoop = cmdContainer.begin(); iterLoop != cmdContainer.end(); ++iterLoop)
	{
		CommandElement* currentElement = &(*iterLoop);
		std::tie(ModelType, ID, ContainerFloorID, CompoGType, OwnerID) = *currentElement;
		if (CompareModelType == ModelType && CompareID == ID && CompareContainerFloorID == ContainerFloorID && CompareOwnerID == OwnerID)
			return iterLoop;
	}
	return cmdContainer.end();
}

MSHistoryObject* UserState::GetHistoryObject(CString& historyID)
{
	//UndoRedo상황이라면 이전에 생성된 것중 가장 가까운 것을 찾아와 리턴하고, 일반상황이라면 새로운 것을 만들어 내보낼 수 있도록 NULL을 리턴한다.
	//단, 이 UserState에서 SetDeleted된 것이라면 그것을 리턴해 준다.(Delete->New 형식의 Modify에 대응) 
	MSHistoryObject* historyObject = NULL;
	if (CmdProcessHelper::GetHistoryManager()->IsInUndoRedo())
	{
		historyObject = FindHistoryObject(historyID);
		
	}
	else
	{	
		auto iter = _mapHistoryObjectsDeleted.find(historyID);
		if (iter != _mapHistoryObjectsDeleted.end())
		{	
			historyObject = iter->second;
		}
	}

	if (historyObject != NULL)
		historyObject->SetDeleted(false);// Deleted = false로 세팅한후 부활시킨다.

	return historyObject;
		
}


void UserState::AddHistoryObject(MSHistoryObject* hObject)
{
	CString historyID = hObject->GetHistoryID();

	auto iter = _mapHistoryObjects.find(historyID);
	if (iter != _mapHistoryObjects.end())
	{
		if ( (*MSConnection::IGNORE_HISTORY()) )
			delete iter->second;
		else
		{
			ASSERT(false);
			return;
		}
	}

	_mapHistoryObjects[historyID] = hObject;	
}

bool UserState::RemoveHistoryObject(MSHistoryObject* hObject)
{
	bool isFind = false;
	if (hObject == nullptr)
		return isFind;


	map<CString, MSHistoryObject*>::iterator it;
	for (it = _mapHistoryObjects.begin(); it != _mapHistoryObjects.end(); it++)
	{
		if (it->second == hObject)
		{
			isFind = true;
			break;
		}
	}
	if (isFind)
		_mapHistoryObjects.erase(it);

	return isFind;
}

void UserState::RegisterHistoryObjectAsDeleted(MSHistoryObject* pHistoryObject)
{
	CString historyID = pHistoryObject->GetHistoryID();
	_mapHistoryObjectsDeleted[historyID] = pHistoryObject;
	
	if (FindHistoryObject(historyID) == NULL)//history상에 등록되지 않은 객체 등록 (history clear등으로 발생)
	{
		UserState* first_US = GetFirstUserState();
		first_US->AddHistoryObject(pHistoryObject);
	}
}

void UserState::ClearDeletedHistoryObjectMap()
{	
	_mapHistoryObjectsDeleted.clear();
}

MSHistoryObject* UserState::FindHistoryObjectHere(CString& historyID)
{
	auto iter = _mapHistoryObjects.find(historyID);
	if (iter != _mapHistoryObjects.end())
	{
		return iter->second;
	}
	else
		return NULL;
}

MSHistoryObject* UserState::FindHistoryObject(CString& historyID)
{
	
//TODO : 이 방식은 관리와 구현이 간단하지만 성능문제가 있을 수 있다. 만약 그렇다면, HistoryManager에 HO에 대한 캐시를 만들어서 속도를 높일 수 있을 것이다.
//또 다른 방식으로는 생성시 등록하지 말고, 삭제시 등록할 수가 있다. 그렇게 되면 이전으로 거슬러 올라가 찾을 필요가 없다.
//이 방식의 문제점은 생성후(US1) 다음 스테이트(US2)에서 삭제했다가 undo한 후, 다른 State(US3)를 생성해 진행할 경우, US2를 트림하게 되는데 이때는 등록된 객체를 삭제하지 않는다.
//따라서 진짜 삭제 시점을 잡기 어렵다는 것이다. 생성된 US가 Trim될 때가 진짜 지워야 할 시점인데 이 시점을 알기가 어렵다.
	
	UserState* prevState = this;
	//현재에 없다면 이전 UserState에 생성된 것이므로 과거에서 찾는다.
	while (prevState != NULL)
	{

		MSHistoryObject* hO = prevState->FindHistoryObjectHere(historyID);
		if (hO != NULL)
			return hO;
		
		prevState = prevState->GetPrevious();
	}
	
	return NULL;
	
}

UserState* UserState::GetFirstUserState()
{
	UserState* first_US = this;
	while (first_US->Previous != NULL)
	{
		first_US = first_US->Previous;
	}
	return first_US;
}
