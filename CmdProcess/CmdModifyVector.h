#pragma once
//#include <vector>
#include "../eCollectionModifyType.h"
#include "CmdAbstract.h"


template <class T>
class CmdModifyVector : public CmdAbstract
{
private:
	typedef std::vector<T> Container;
//	typedef std::vector<T>::iterator Iter;
	//typedef Container::iterator Iter;
	
public:
	CmdModifyVector(Container* container, eCollectionModifyType::eCollectionModifyType modifyType, T value)
	{
		_Container = container;
		_CollectionModifyType = modifyType;
		_Value = value;
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
		_Container->push_back(_Value);
	}

	void Delete()
	{
		std::vector<T>::iterator Iter;
		for (Iter = _Container->begin(); Iter != _Container->end(); ++Iter)
		{
			if (_Value->m_ID == (*Iter)->m_ID)
			{
				_Container->erase(Iter);
				return;
			}
		}
	}
private:
	Container* _Container;
	T _Value;
	eCollectionModifyType::eCollectionModifyType _CollectionModifyType;
};