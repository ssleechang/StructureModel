#pragma once
//#include <vector>
#include "../eCollectionModifyType.h"
#include "CmdAbstract.h"


template <class T>
class CmdNewModifyVariable : public CmdAbstract
{
	
public:
	static void Set(T* address, T value)
	{
		CmdNewModifyVariable<T>* cmd = new CmdNewModifyVariable<T>(address, value);
		cmd->Doit();
	}

	CmdNewModifyVariable(T* address, T value)
	{
		CmdAbstract::SetElementType(CmdProcess::CommandUpdateElementType::Collection);
		CmdAbstract::SetObjectID(-1);
		CmdAbstract::SetFloorID(-1);

		_Address = address;
		_Value = value;

		RegisterCommand();
	}

	void Doit()
	{
		_CommandStatus = Done;

		_CommandType = eCommandType::CollectionAdd;
		_Value_Before = *_Address;
		(*_Address) = _Value;
	}

	void Undo()
	{
		_CommandStatus = Undone;
		(*_Address) = _Value_Before;
	}

	void SetTimeStamp(CTime timeStamp)
	{
		return;
	}

private:
	T* _Address;
	T _Value_Before;
	T _Value;
};