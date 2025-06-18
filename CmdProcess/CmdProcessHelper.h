#pragma once

#include "HistoryManager.h"

class StructureModel_CLASS CmdProcessHelper
{
public:
	CmdProcessHelper(void);
	~CmdProcessHelper(void);

	//static HistoryManager* GetHistoryManager()
	static HistoryManager* GetHistoryManager();
	static void SetHistoryManager(HistoryManager* historyManager);
	static NonHistoryManager* GetNonHistoryManager();
	static void SetNonHistoryManager(NonHistoryManager* historyManager);

private :
	static HistoryManager* _HistoryManager;
	static NonHistoryManager* _NonHistoryManager;
};

//HistoryManager *CmdProcessHelper::_HistoryManager;