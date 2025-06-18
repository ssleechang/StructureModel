#include "stdafx.h"
#include "CmdProcessHelper.h"

HistoryManager* CmdProcessHelper::_HistoryManager;
NonHistoryManager* CmdProcessHelper::_NonHistoryManager;

void CmdProcessHelper::SetHistoryManager(HistoryManager* historyManager)
{
	_HistoryManager = historyManager;
}

void CmdProcessHelper::SetNonHistoryManager(NonHistoryManager* nonHistoryManager)
{
	_NonHistoryManager = nonHistoryManager;
}


HistoryManager* CmdProcessHelper::GetHistoryManager()
{
	return _HistoryManager;
}

NonHistoryManager* CmdProcessHelper::GetNonHistoryManager()
{
	return _NonHistoryManager;
}

CmdProcessHelper::CmdProcessHelper(void)
{
	
}


CmdProcessHelper::~CmdProcessHelper(void)
{
}


