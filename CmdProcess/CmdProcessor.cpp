#include "StdAfx.h"
#include "CmdProcessor.h"
#include <iostream>

CmdProcessor::CmdProcessor(void)
{
}

CmdProcessor::~CmdProcessor(void)
{
	ClearStack();
}

void CmdProcessor::ClearStack()
{
	for ( CommandIterator IterBegin = _commandList.begin(); IterBegin != _commandList.end(); ++IterBegin)
	{
		CmdAbstract* currentCommand = *IterBegin;
		if ( NULL == currentCommand)
			continue;
		delete currentCommand;

	}

	_commandList.clear();
}

void CmdProcessor::UnDo()
{
	for ( int i=_commandList.size()-1; i>=0; i--)
	{
		CmdAbstract* currentCommand = _commandList[i];
		currentCommand->Undo();
	}
}

void CmdProcessor::ReDo()
{
	for ( CommandIterator IterBegin = _commandList.begin(); IterBegin != _commandList.end(); ++IterBegin)
	{
		CmdAbstract* currentCommand = *IterBegin;
		if ( NULL != currentCommand)
		{
			currentCommand->Redo();
			/*
			if(IS_USE_NATIVE_CMD)
				currentCommand->Redo();
			else
				currentCommand->Doit();
			*/
				
		}
	}
}

void CmdProcessor::AddCommand(CmdAbstract* command)
{
	for (CommandIterator IterBegin = _commandList.begin(); IterBegin != _commandList.end(); ++IterBegin)
	{
		CmdAbstract* currentCommand = *IterBegin;
		if (currentCommand == command)
			ASSERT(FALSE);

	}
	_commandList.push_back(command);
}

void CmdProcessor::DelCommand(CmdAbstract* command)
{
	for (int i = _commandList.size() - 1; i >= 0; --i)
	{
		if (_commandList[i] == command)
			_commandList.erase(_commandList.begin() + i);
	}
}
