#pragma once
#include <vector>
#include "CmdAbstract.h"
#include "NonHistoryManager.h"
class StructureModel_CLASS CmdProcessor
{
public:
	CmdProcessor();
	virtual ~CmdProcessor();
	
	void ClearStack();
	void AddCommand(CmdAbstract* command);
	void DelCommand(CmdAbstract* command);
	void UnDo();
	void ReDo();
	
private:
	typedef vector<CmdAbstract*> CommandContainer;
	typedef CommandContainer::iterator CommandIterator;
	CommandContainer _commandList;
	
};

