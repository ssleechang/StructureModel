#include "stdafx.h"
#include "HistoryManager.h"
#include "CmdAbstract.h"
#include "../MSFloor.h"
#include "../MSCompoG.h"
#include "../MSConnection.h"
#include <iostream>
#include <algorithm>

map<MSHistoryObject*, bool> HistoryManager::_mapHistoryObjectForClear;
HistoryManager::HistoryManager(void)
{
	_HistoryState = eHistoryState::None;
	_IsInvoked = false;
	_currentState = NULL;
	_nonHistoryManager = NULL;
	_IsInUndoRedo = false;
	
	NoteState(0);//Make Initial UserState
}


HistoryManager::~HistoryManager(void)
{
}

//Document Level에서의 NoteState와 다르다.
//Model HistoryManager의 NoteState/RolltoState는 Command와 대응한다.
//반면 Document Level의 NoteState는 Tool Level이다.
void HistoryManager::NoteState(int stateID)
{

	_HistoryState = eHistoryState::Notified;

	UserState* newState = new UserState();
	newState->ID = stateID;

	if ( NULL != _currentState)
	{
		_currentState->ClearDeletedHistoryObjectMap();

		if ( NULL != _currentState->GetNext())
		{
			UserState* oldCurrentState = _currentState;
			TrimState(_currentState->GetNext());
			_currentState = oldCurrentState;
		}
		_currentState->SetNext(newState);
		newState->SetPrevious(_currentState);
	}
	if ( NULL != _currentState)
		std::cout<<"HistoryManager NoteStated  " <<_currentState->ID << " -> " <<newState->ID<<std::endl;
	_currentState = newState;
	_ArrangeChangedHistoryList.push_back(_currentState->ID);
	_IsInvoked = false;

	

}

//Document Level에서의 Undo/Redo와 다르다.
//Model HistoryManager의 RolltoState는 Command와 대응한다.
//반면 Document Level의 Undo/Redo는 Tool Level이다.

//Roll할때마다 쌓아둔다. 그리고 Invoked Updated됐을땔 기준으로 기준을 짜르는건데. 그러니까...Set된 상태에서 finished mark를 찍고 그 이후에 다시 쌓이는건 새로운 container에 담는다.
bool HistoryManager::RollToState(int stateID)
{
	if ( NULL == _currentState)
		return false;
	
	
	UserState* findState;
	findState = _currentState->FindUserState(stateID, true);//next쪽으로 검색
	
	if ( NULL != findState)//Redoed
	{
		_HistoryState = eHistoryState::Redoed;
		while(_currentState != findState)
		{
			_currentState = _currentState->GetNext();
			_IsInUndoRedo = true;
			_currentState->Redo();
			_currentState->ClearDeletedHistoryObjectMap();
			_IsInUndoRedo = false;
			_ArrangeChangedHistoryList.push_back(_currentState->ID);
			//ArrangeCommandContainer();
		}
		return true;
	}
		
	findState = _currentState->FindUserState(stateID, false);//Previous쪽으로 검색
	if ( NULL != findState)//Undoed
	{
		_HistoryState = eHistoryState::Undoed;
		while(_currentState != findState)
		{
			_IsInUndoRedo = true;
			_currentState->ClearDeletedHistoryObjectMap();
			_currentState->Undo();
			_currentState->ClearDeletedHistoryObjectMap();
			_IsInUndoRedo = false;
			//ArrangeCommandContainer();
			_ArrangeChangedHistoryList.push_back(_currentState->ID);
			_currentState = _currentState->GetPrevious();
		}
		return true;
	}

	return false;
}

UserState* HistoryManager::GetUserState(int stateID)
{
	UserState* findState;
	if (_currentState->ID == stateID)
		return _currentState;

	findState = _currentState->FindUserState(stateID, true);
	if ( NULL != findState)
		return findState;

	findState = _currentState->FindUserState(stateID, false);
	if ( NULL != findState)
		return findState;
	return NULL;
}

void HistoryManager::RemoveDuplicate(vector<int>& list)
{
	vector<int> copy;
	copy.swap(list);
	list.reserve(copy.size());
	std::map<int, int> mapCheck;
	for(int i=0 ; i<copy.size() ; ++i)
	{
		int data = copy[i];
		if(mapCheck.find(data) == mapCheck.end()) // not exist
		{
			list.push_back(data);
			mapCheck.insert(make_pair(data, 0));
		}
	}
}

void HistoryManager::AppendCmdContainerAndClearChangedHistList()
{
	_IsInvoked = true;
	AppendCommandContainer();
	//FixCommandContainer();
	_ArrangeChangedHistoryList.clear();
}

void HistoryManager::ArrangeCommandContainer()
{
	_AddCommandSetContainer.clear();
	_ModifyCommandSetContainer.clear();
	_DeleteCommandSetContainer.clear();

	RemoveDuplicate(MSFloor::CompoGIDList_NeedRegen_CurTR());

	MSFloor::CompoGIDList_NeedRegen().clear();
	MSFloor::CompoGIDList_NeedRegen_2DView().clear();
	MSFloor::CompoGIDList_NeedRegen() = MSFloor::CompoGIDList_NeedRegen_CurTR();
	MSFloor::CompoGIDList_NeedRegen_2DView() = MSFloor::CompoGIDList_NeedRegen_2DView_CurTR();
	MSFloor::CompoGIDList_NeedRegen_CurTR().clear();
	MSFloor::CompoGIDList_NeedRegen_2DView_CurTR().clear();
	_nonHistoryManager->UpdateModifyInfo();

	AppendCommandContainer();
}

void HistoryManager::AppendCommandContainer()
{
	std::vector<int>::iterator loopIter;

	for (loopIter = _ArrangeChangedHistoryList.begin(); loopIter != _ArrangeChangedHistoryList.end(); ++loopIter)
	{
		UserState* currentState = GetUserState((*loopIter));
		if (NULL == currentState)
			continue;

		UserState::CommandContainer* currentStateAddContainer = currentState->GetAddCommandList();
		UserState::ModifyCommadContainer* currentStateModifyContainer = currentState->GetModifyCommandList();
		UserState::CommandContainer* currentStateDeleteContainer = currentState->GetDeleteCommandList();
		if (_HistoryState == eHistoryState::Undoed)
		{
			for (UserState::reverseCommandListIter addIter = currentStateAddContainer->rbegin(); addIter != currentStateAddContainer->rend(); ++addIter)
			{
				_AddCommandSetContainer.push_back(*addIter);
			}

			for (UserState::reverseCommandListIter deleteIter = currentStateDeleteContainer->rbegin(); deleteIter != currentStateDeleteContainer->rend(); ++deleteIter)
			{
				_DeleteCommandSetContainer.push_back(*deleteIter);
			}


			for (UserState::reverseModifyCommandListIter modifyIter = currentStateModifyContainer->rbegin(); modifyIter != currentStateModifyContainer->rend(); ++modifyIter)
			{
				_ModifyCommandSetContainer.push_back(*modifyIter);
			}
		}
		else
		{
			for (UserState::CommandListIter addIter = currentStateAddContainer->begin(); addIter != currentStateAddContainer->end(); ++addIter)
			{
				_AddCommandSetContainer.push_back(*addIter);
			}

			for (UserState::CommandListIter deleteIter = currentStateDeleteContainer->begin(); deleteIter != currentStateDeleteContainer->end(); ++deleteIter)
			{
				_DeleteCommandSetContainer.push_back(*deleteIter);
			}


			for (UserState::ModifyCommandListIter modifyIter = currentStateModifyContainer->begin(); modifyIter != currentStateModifyContainer->end(); ++modifyIter)
			{
				_ModifyCommandSetContainer.push_back(*modifyIter);
			}
		}
	}
}

void HistoryManager::FixCommandContainer()
{	
	vector<int> addDelIDList;
	for each (auto commandEle in _AddCommandSetContainer)
	{
		int modelType;
		int modelID;
		int modelContainerFloorID;
		int compoGType;
		int ownerCompoGID;

		std::tie(modelType, modelID, modelContainerFloorID, compoGType, ownerCompoGID) = commandEle;

		if (modelType == CommandUpdateElementType::CompoG)
			addDelIDList.push_back(modelID);
	}

	for each (auto commandEle in _DeleteCommandSetContainer)
	{
		int modelType;
		int modelID;
		int modelContainerFloorID;
		int compoGType;
		int ownerCompoGID;

		std::tie(modelType, modelID, modelContainerFloorID, compoGType, ownerCompoGID) = commandEle;

		if (modelType == CommandUpdateElementType::CompoG)
			addDelIDList.push_back(modelID);
	}

	auto commandEleIt = _ModifyCommandSetContainer.begin();
	while (commandEleIt != _ModifyCommandSetContainer.end())
	{
		int objectType;
		int objectID;
		MSObject* msObject;
		int floorID;
		int ownerCompoGID;
		bool isGeometryModified;
		bool isDummyAlteration;
				
		std::tie(objectType, msObject, objectID, floorID, ownerCompoGID, isGeometryModified, isDummyAlteration) = *commandEleIt;

		if (objectType == CommandUpdateElementType::CompoG)
		{
			auto it = find(addDelIDList.begin(), addDelIDList.end(), objectID);
			if (it != addDelIDList.end())
				commandEleIt = _ModifyCommandSetContainer.erase(commandEleIt);
			else
				commandEleIt++;
		}
		else
			commandEleIt++;
	}

	sort(_ModifyCommandSetContainer.begin(), _ModifyCommandSetContainer.end());
	_ModifyCommandSetContainer.erase(
		unique(_ModifyCommandSetContainer.begin(), _ModifyCommandSetContainer.end()), _ModifyCommandSetContainer.end());
}

void HistoryManager::AddCommand(CmdAbstract* command)
{
	if ( NULL ==_currentState)
		return;

	command->SetTimeStamp(_StampTime);

	_currentState->AddCommand(command);
}

void HistoryManager::DelCommand(CmdAbstract* command)
{
	if (NULL == _currentState)
		return;

	command->SetTimeStamp(_StampTime);

	_currentState->DelCommand(command);
}

// MSConnection클래스에서 CmdProcessHelper::GetHistoryManager()를 호출하여 _currentState를 사용하는 경우가 발생
// _currentState 상태가 할당이 되어 있거나 null인 상태로 유지 되어야 할 필요성이 있음
// HistoryManager::NoteState()에서 TrimState(_currentState->GetNext()) 호출후 _currentState 값을 oldCurrentState 값으로 변경
// HistoryManager::Clear()에서 TrimState(_currentState) 호출후 _currentState 값을 null로 변경
void HistoryManager::TrimState(UserState* state)
{
	UserState* pNextState = NULL;
	while (NULL != (pNextState = state->GetNext()))
	{
		_currentState = state;
		_currentState->SetPrevious(nullptr);
		delete state;
		state = pNextState;		
	}
	_currentState = state;
	_currentState->SetPrevious(nullptr);
	delete state;
}


UserState::CommandContainer* HistoryManager::GetAddCommandList()
{
	if ( NULL == _currentState)
		return NULL;

	if ( eHistoryState::Undoed == _HistoryState )
		return &_DeleteCommandSetContainer;

	return &_AddCommandSetContainer;
}

UserState::CommandContainer* HistoryManager::GetDeleteCommandList()
{
	if (NULL ==_currentState)
		return NULL;
	
	if ( eHistoryState::Undoed == _HistoryState )
		return &_AddCommandSetContainer;

	return &_DeleteCommandSetContainer;
}
UserState::ModifyCommadContainer* HistoryManager::GetModifyCommandList()
{
	if (NULL ==_currentState)
		return NULL;

	//if ( true == _IsUndoed)
	//	return _currentState->Next->GetModifyCommandList();

	return &_ModifyCommandSetContainer;
}

void HistoryManager::Clear(bool bClearHistoryObjects)
{
	// mylee added
	_AddCommandSetContainer.clear();
	_ModifyCommandSetContainer.clear();
	_DeleteCommandSetContainer.clear();
	_nonHistoryManager->Clear();

	_ArrangeChangedHistoryList.clear();
	// end

	if (_currentState != NULL)
	{

		_currentState->SetDeleteHistoryObjects(bClearHistoryObjects);
		while (NULL != _currentState->GetPrevious())
		{
			_currentState = _currentState->GetPrevious();
			_currentState->SetDeleteHistoryObjects(bClearHistoryObjects);
		}

		TrimState(_currentState);

		// mylee
		for (map<MSHistoryObject*, bool>::iterator itr = _mapHistoryObjectForClear.begin(); itr != _mapHistoryObjectForClear.end(); ++itr)
		{
			try
			{ delete itr->first; }
			catch (const std::exception& ex)
			{
				CString errorMsg;
				errorMsg.Format(L"Standard exception occurred in MSHistoryManager: %S", ex.what());
				StatusReporterWrapper::OutputError(errorMsg);
			}
#ifndef _DEBUG
			catch (...)
			{
				StatusReporterWrapper::OutputError("Unknown Error from MSHistoryManager!");
			}
#endif
		}
		_mapHistoryObjectForClear.clear();

		_currentState = NULL;
	}
}


MSHistoryObject* HistoryManager::GetHistoryObject(CString& historyID)
{
	
	if (_currentState != NULL)
		return _currentState->GetHistoryObject(historyID);
	
	
	return NULL; //Undo/Redo가 아닌 상황에서 HistoryObject 에 대한 요청이 들어오면 NULL을 반환해서 새로 생성하도록 해야 한다.	
}


void HistoryManager::AddHistoryObject(MSHistoryObject* hObject)
{
	CString historyID = hObject->GetHistoryID();

	if (_currentState != NULL)
		_currentState->AddHistoryObject(hObject);
}

bool HistoryManager::RemoveHistoryObject(MSHistoryObject* hObject)
{
	CString historyID = hObject->GetHistoryID();

	if (_currentState != NULL)
		return _currentState->RemoveHistoryObject(hObject);

	return false;
}

void HistoryManager::RegisterHistoryObjectAsDeleted(MSHistoryObject* pHistoryObject)
{
	if (_currentState != NULL)
		_currentState->RegisterHistoryObjectAsDeleted(pHistoryObject);

}
