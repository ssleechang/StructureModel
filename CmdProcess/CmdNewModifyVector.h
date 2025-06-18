#pragma once
//#include <vector>
#include "../eCollectionModifyType.h"
#include "CmdAbstract.h"


template <class T>
class CmdNewModifyVector : public CmdAbstract
{
private:
	typedef std::vector<T> Container;
	int nOperationType;
	
public:
	static void PushBack(Container* container, T value)
	{
		CmdNewModifyVector<T>* cmd = new CmdNewModifyVector<T>(container);
		cmd->SetInputPushBack(value);
		cmd->Doit();
	}

	static void RemoveAll(Container* container)
	{
		CmdNewModifyVector<T>* cmd = new CmdNewModifyVector<T>(container);
		cmd->SetInputRemoveAll();
		cmd->Doit();
	}

	static void Erase(Container* container, int idx)
	{
		CmdNewModifyVector<T>* cmd = new CmdNewModifyVector<T>(container);
		cmd->SetInputErase(idx);
		cmd->Doit();
	}

	static void SetAt(Container* container, int idx, T value)
	{
		CmdNewModifyVector<T>* cmd = new CmdNewModifyVector<T>(container);
		cmd->SetInputSetAt(idx, value);
		cmd->Doit();
	}

	static void InsertAt(Container* container, int idx, T value)
	{
		CmdNewModifyVector<T>* cmd = new CmdNewModifyVector<T>(container);
		cmd->SetInputInsert(idx, value);
		cmd->Doit();
	}

public:
	CmdNewModifyVector(Container* container)
	{
		CmdAbstract::SetElementType(CmdProcess::CommandUpdateElementType::Collection);
		CmdAbstract::SetObjectID(-1);
		CmdAbstract::SetFloorID(-1);

		_Container = container;

		RegisterCommand();
	}

	void SetInputPushBack(T value)
	{
		nOperationType = 0;
		_Value = value;
	}

	void SetInputRemoveAll()
	{
		nOperationType = 1;
	}

	void SetInputErase(int idx)
	{
		nOperationType = 2;
		_Erase_Idx = idx;
	}

	void SetInputSetAt(int idx, T value)
	{
		nOperationType = 3;
		_Erase_Idx = idx;
		_Value = value;
	}

	void SetInputInsert(int idx, T value)
	{
		nOperationType = 4;
		_Erase_Idx = idx;
		_Value = value;
	}

	void Doit()
	{
		_CommandStatus = Done;
		if (nOperationType == 0) // Pushback
		{
			_CommandType = eCommandType::CollectionAdd;
			_Container->push_back(_Value);
		}
		else if (nOperationType == 1) // RemoveAll
		{
			_Container->swap(_Copy);
			_Container->clear();
		}
		else if (nOperationType == 2) // Erase
		{
			_Value = (*_Container)[_Erase_Idx];
			_Container->erase(_Container->begin()+_Erase_Idx);
		}
		else if (nOperationType == 3) // SetAt
		{
			_Value_Before = (*_Container)[_Erase_Idx];
			(*_Container)[_Erase_Idx] = _Value;
		}
		else if (nOperationType == 4) // InsertAt
		{
			_Container->insert(_Container->begin()+_Erase_Idx, _Value);
		}
		
	}

	void Undo()
	{
		_CommandStatus = Undone;
		if (nOperationType == 0)
		{
			for ( int i=(int)_Container->size()-1 ; i>=0 ; --i)
			{
				if( (*_Container)[i] == _Value)
				{
					_Container->erase(_Container->begin()+i);
					break;
				}
			}
		}
		else if (nOperationType == 1) // RemoveAll
		{
			_Container->swap(_Copy);
		}
		else if (nOperationType == 2) // Erase
		{
			_Container->insert(_Container->begin() + _Erase_Idx, _Value);
		}
		else if (nOperationType == 3) // SetAt
		{
			(*_Container)[_Erase_Idx] = _Value_Before;
		}
		else if (nOperationType == 4) // InsertAt
		{
			_Container->erase(_Container->begin()+_Erase_Idx);
		}
	}

	void SetTimeStamp(CTime timeStamp)
	{
		return;
	}

private:
	//void Add()
	//{
	//	_Container->push_back(_Value);
	//}

	//void Delete()
	//{
	//	std::vector<T>::iterator Iter;
	//	for (Iter = _Container->begin(); Iter != _Container->end(); ++Iter)
	//	{
	//		if (_Value->m_ID == (*Iter)->m_ID)
	//		{
	//			_Container->erase(Iter);
	//			return;
	//		}
	//	}
	//}
private:
	Container* _Container;
	T _Value;
	T _Value_Before;

	Container _Copy;

	int _Erase_Idx;
};