#pragma once


#include "CmdProcessor.h"
#include <vector>
#include <tuple>
#include "..\CmdModifyModel.h"
//#include <iostream>

class MSHistoryObject;
class StructureModel_CLASS UserState
{
	
public:
	UserState();
	~UserState();
	UserState* GetFirstUserState();

	//MSBuildingType, id, containerFloorid
	typedef tuple<CmdProcess::CommandUpdateElementType, int, int, int, int> CommandElement;//element ID, Floor ID, CompoGType, Owner ID (Window/Door 등 Sub CompoG일경우만)
	typedef std::vector<CommandElement> CommandContainer;
	typedef CommandContainer::iterator CommandListIter;
	typedef CommandContainer::reverse_iterator reverseCommandListIter;

	//typedef tuple<CmdProcess::CommandUpdateElementType, MSObject*, int, int, int, bool> ModifyElement; // BH-Q | element ID, Floor ID, Owner ID, isGeometryModified
	//typedef tuple<CmdProcess::CommandUpdateElementType, MSObject*, int, int, bool, bool> ModifyElement; // BH-A | element ID, Floor ID, isGeometryModified, isDummyAlteration
	typedef tuple<CmdProcess::CommandUpdateElementType, MSObject*, int, int, int, bool, bool> ModifyElement; // BH-A | element ID, Floor ID, Owner ID, isGeometryModified, isDummyAlteration
	typedef std::vector<ModifyElement> ModifyCommadContainer;
	typedef ModifyCommadContainer::iterator ModifyCommandListIter;
	typedef ModifyCommadContainer::reverse_iterator reverseModifyCommandListIter;

	void Redo();
	void Undo();

	void AddCommand(CmdAbstract* command);
	void DelCommand(CmdAbstract* command);

	void SetDeleteHistoryObjects(bool isDelete){
		_isDeleteHistoryObjects = isDelete;
	}


	UserState* FindUserState(int stateID, bool isNext);

	CommandContainer* GetAddCommandList()
	{
		return &_AddCommandList;
	}

	CommandContainer* GetDeleteCommandList()
	{
		return &_DeleteCommandList;
	}

	ModifyCommadContainer* GetModifyCommandList()
	{
		return &_ModifyCommandList;
	}

	

private:
	void Dispose();

	void AddCommandTypeAdd(CmdAbstract* command);

	void AddCommandTypeDelete(CmdAbstract* command);
	void DelCommandTypeDelete(CmdAbstract* command);

	void AddCommandTypeDeleteArray(CmdAbstract* command);
	void AddCommandTypeAddArray(CmdAbstract* command);

	void AddCommandTypeModify(CmdAbstract* command);

	CommandListIter FindPairFromAddList(CommandElement* compareData);
	CommandListIter FindPairFromDelList(CommandElement* compareData);
	CommandListIter FindPairFromCommandList(CommandElement* compareData, CommandContainer& cmdContainer);
public:
	
	int ID;
	bool _isDeleteHistoryObjects; 
	UserState *GetNext(){ return Next; }
	UserState *GetPrevious(){ return Previous; }
	void SetNext(UserState * state)
	{ 
		Next = state; 
		ASSERT(this != Next);
		ASSERT(Next == NULL || (Next != NULL && Next != Previous));
		
	}
	void SetPrevious(UserState * state)
	{ 
		Previous = state; 
		ASSERT(this != Previous);
		ASSERT(Previous == NULL || (Previous != NULL && Next != Previous));
	}
	
	void AddHistoryObject(MSHistoryObject* pHistoryObject);
	bool RemoveHistoryObject(MSHistoryObject* pHistoryObject);
	void RegisterHistoryObjectAsDeleted(MSHistoryObject* pHistoryObject);

	

	void ClearDeletedHistoryObjectMap();
	MSHistoryObject* GetHistoryObject(CString& historyID);

	MSHistoryObject* FindHistoryObject(CString& historyID);

private:
	UserState *Previous;
	UserState *Next;
	
	map<CString, MSHistoryObject*> _mapHistoryObjects;
	map<CString, MSHistoryObject*> _mapHistoryObjectsDeleted;

	MSHistoryObject* FindHistoryObjectHere(CString& historyID);
private:
	//1rd compoGType
	//2nd id

	CommandContainer _AddCommandList;
	CommandContainer _DeleteCommandList;
	ModifyCommadContainer _ModifyCommandList;
	
	CmdProcessor _cmdProcessor;
};