#pragma once
//#include "Singleton.h"
#include <atltime.h>
#include "UserState.h"

class CmdAbstract;
class MSCompoG;
class MSConnection;
class MSHistoryObject;
class StructureModel_CLASS HistoryManager
{
public:
	HistoryManager(void);
	~HistoryManager(void);

	void NoteState(int stateID);

	bool RollToState(int stateID);

	void AddCommand(CmdAbstract* command);
	void DelCommand(CmdAbstract* command);

	void Clear(bool bClearHistoryObjects);

	bool IsInUndoRedo(){
		return _IsInUndoRedo;
	}
	void SetIsInUndoRedo(bool isInUndoRedo){ _IsInUndoRedo = isInUndoRedo; }

	void InvokeUpdated()
	{
		_IsInvoked = true;
		//if ( eHistoryState::Notified == _HistoryState)
		ArrangeCommandContainer();
		_ArrangeChangedHistoryList.clear();
	}

	void TimeStamp()
	{
		_StampTime = CTime::GetCurrentTime();
	}

	UserState::CommandContainer* GetAddCommandList();
	UserState::CommandContainer* GetDeleteCommandList();
	UserState::ModifyCommadContainer* GetModifyCommandList();

	void SetNonHistoryManager(NonHistoryManager* nonHistoryManager)
	{
		_nonHistoryManager = nonHistoryManager;
	}
	
	//Get MSHistoryObject ....
	
	
	void AddHistoryObject(MSHistoryObject* hObject);
	bool RemoveHistoryObject(MSHistoryObject* hObject);
	MSHistoryObject* GetHistoryObject(CString& historyID);
	void RegisterHistoryObjectAsDeleted(MSHistoryObject* pHistoryObject);
	static map<MSHistoryObject*, bool> _mapHistoryObjectForClear;

	void AppendCmdContainerAndClearChangedHistList();

private:
	void TrimState(UserState* state);
	
	UserState* GetUserState(int stateID);

	void ArrangeCommandContainer();
	void AppendCommandContainer();
	/// <summary>
	/// ModifySetContainer 정리 (CompoG 업데이트만 처리)
	/// </summary>
	void FixCommandContainer();

	void RemoveDuplicate(vector<int>& list);
	
private:
	enum eHistoryState
	{
		None,
		Notified,
		Undoed,
		Redoed,
	};
	CTime _StampTime;
	std::vector<int> _ArrangeChangedHistoryList;

	eHistoryState _HistoryState;
	UserState::CommandContainer _AddCommandSetContainer;
	UserState::CommandContainer _DeleteCommandSetContainer;
	UserState::ModifyCommadContainer _ModifyCommandSetContainer;

	UserState* _currentState;
	NonHistoryManager* _nonHistoryManager; // Undo/Redo 지원않하는 변경사항의 관리자. 
	bool _IsInvoked;
	bool _IsInUndoRedo;
};
