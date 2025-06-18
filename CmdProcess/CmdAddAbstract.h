#pragma once
#include "CmdAbstract.h"
class StructureModel_CLASS CmdAddAbstract : public CmdAbstract
{
public:
	CmdAddAbstract()
	{
		_CommandStatus = Done;
		_CommandType = Add;
	}
	
	virtual void Doit()
	{
		_CommandStatus = Done;
	}

	virtual void Undo()
	{
		_CommandStatus = Undone;
	}

	virtual void SetTimeStamp(CTime timeStamp){}
};
