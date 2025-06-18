#include "stdafx.h"
#include "CmdBridge.h"
#include "HistoryManager.h"

CmdBridge::CmdBridge()
	: CmdAbstract()
{
	
}


CmdBridge::~CmdBridge(void)
{
	if ( NULL == _BridgeDelegate)
		return;
	_BridgeDelegate->Dispose();
}


void CmdBridge::Doit()
{
	//여기 내가 원하는 코드를 init하면 됩니다.
	if ( NULL == _BridgeDelegate)
		return;
	_BridgeDelegate->ReDo();
}
void CmdBridge::Undo()
{
	if ( NULL == _BridgeDelegate)
		return;
	_BridgeDelegate->UnDo();
}


void CmdBridge::SetCmdBridge(ICmdBridge* BridgeDelegate)
{
	_BridgeDelegate = BridgeDelegate;
}