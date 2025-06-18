#pragma once
#include "CmdAbstract.h"

class StructureModel_CLASS CmdDeleteAbstract : public CmdAbstract
{
public:
	CmdDeleteAbstract()
	{
		_CommandStatus = Done;
		_CommandType = Delete;
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
