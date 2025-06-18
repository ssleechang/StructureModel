#pragma once
#include <map>
#include "../eCollectionModifyType.h"
#include "CmdAbstract.h"


template <typename KeyT, class ValueT>
class CmdModifyMap : public CmdAbstract
{
private:
	typedef std::map<KeyT, ValueT> Container;

public:
	static void InsertToMap(Container* container, KeyT key, ValueT value)
	{
		CmdModifyMap<KeyT, ValueT>* cmd = new CmdModifyMap<KeyT, ValueT>(container, eCollectionModifyType::CollectionAdd, key, value);
		cmd->Doit();
	}

	static void EraseFromMap(Container* container, KeyT key, ValueT value)
	{
		CmdModifyMap<KeyT, ValueT>* cmd = new CmdModifyMap<KeyT, ValueT>(container, eCollectionModifyType::CollectionDelete, key, value);
		cmd->Doit();
	}

	CmdModifyMap(Container* container, eCollectionModifyType::eCollectionModifyType modifyType, KeyT key, ValueT value)
	{
		if (eCollectionModifyType::CollectionAdd == modifyType)
			_CommandType = eCommandType::CollectionAdd;
		else
			_CommandType = eCommandType::CollectionDelete;

		_Container = container;
		_CollectionModifyType = modifyType;
		_Key = key;
		_Value = value;

		RegisterCommand();
	}

	void Doit()
	{
		CmdAbstract::SetElementType(CmdProcess::CommandUpdateElementType::Collection);
		CmdAbstract::SetObjectID(-1);
		CmdAbstract::SetFloorID(-1);
		_CommandStatus = Done;
		if (eCollectionModifyType::CollectionAdd == _CollectionModifyType)
		{
			_CommandType = eCommandType::CollectionAdd;
			Add();
		}

		else
		{

			_CommandType = eCommandType::CollectionDelete;
			Delete();
		}

	}

	void Undo()
	{
		if (eCollectionModifyType::CollectionAdd == _CollectionModifyType)
			Delete();
		else
			Add();
	}

	void SetTimeStamp(CTime timeStamp)
	{
		return;
	}

private:
	void Add()
	{
		std::pair<KeyT, ValueT> element(_Key, _Value);
		_Container->insert(element);
	}

	void Delete()
	{
		_Container->erase(_Key);
	}
private:
	Container* _Container;
	KeyT _Key;
	ValueT _Value;
	eCollectionModifyType::eCollectionModifyType _CollectionModifyType;
};