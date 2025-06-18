#pragma once
#include "CmdAbstract.h"
#include "ICmdBridge.h"

typedef void (*PtrFunc)();

//C++ Native에서 CLI호출할 경우 사용
//ICmdBridge를 상속하는 클래스 생성 후 SetCmdBridge에 세팅한후 사용
//현재 사용 안함
class StructureModel_CLASS CmdBridge : public CmdAbstract
{
	
public:
	CmdBridge();
	~CmdBridge(void);

	virtual void Doit();
	virtual void Undo();

	void SetCmdBridge(ICmdBridge* BridgeDelegate);

	//void SetDoFunction(PtrFunc doFunction);

	//void SetUndoFunction(PtrFunc undoFunction);

private:
	PtrFunc _FunctionDoIt;
	PtrFunc _FunctionUndo;
	ICmdBridge* _BridgeDelegate;
};


struct FuncWrapper
{
public:
	PtrFunc Doit();
	PtrFunc Undo();
};