#pragma once
#pragma warning(disable : 4482)// 열거형 비표준확장, 정규화된 이름에...
//#include "../MSObject.h"
#include <vector>
#include "UpdateElementType.h"
#define IS_USE_NATIVE_CMD false // mylee tesing

//Clear History/Status Trim 시에 Newed상태일 경우 Floor 등에서 유지하고 있어야 하는 상황이므로 자기가 가진 객체를 지우지 않는다.
enum eCommandStatus
{
	None,
	Undone,
	Done, // after do/redo.
};

enum eCommandType
{
	Add,   //객체( CompoG,CompoM 등)의 New가 일어날경우, Collection관리는 자동으로.
	Modify,
	Delete,//객체의 Delete가 일어날경우, Collection관리는 자동으로.
	DeleteMulti,
	AddMulti,
	
	CollectionAdd,//단순 Collection( 기본데이터 타입, 포인터 등)에 대한 Add
	CollectionDelete,//단순 Collection( 기본데이터 타입, 포인터 등)에 대한 Delete
};


class CmdProcessor;
class MSObject;
class StructureModel_CLASS CmdAbstract
{
public:

	CmdAbstract();
	virtual ~CmdAbstract(void){};
	eCommandType GetCommandType()
	{
		return _CommandType;
	}

	virtual MSObject* GetOldValue()
	{
		return NULL;
	}

	void SetElementType(CmdProcess::CommandUpdateElementType elementType);
	CmdProcess::CommandUpdateElementType  GetElementType();

	void SetObjectID(long ID);
	long GetObjectID();
	
	void SetObjectIDArray(vector<long> ID);
	vector<long> GetObjectIDArray();

	void SetObjectGTypeArray(vector<long> ID);
	vector<long> GetObjectGTypeArray();

	void RegisterCommand();
	void UnRegisterCommand();

	virtual void Doit()=0;
	virtual void Undo()=0;
	virtual void Redo() { Doit(); };

	virtual void SetTimeStamp(CTime timeStamp) = 0;

	void SetFloorID(long ID)
	{
		_FloorContainerID = ID;
	}
	long GetFloorID()
	{
		return _FloorContainerID;
	}

	void SetCompoGType(long type)
	{
		_CompoGType = type;
	}
	long GetCompoGType()
	{
		return _CompoGType;
	}

	void SetOwnerCompoGID(long ID)
	{
		_OwnerCompoGID = ID;
	}
	long GetOwnerCompoGID()
	{
		return _OwnerCompoGID;
	}

protected:
	eCommandStatus _CommandStatus;
	eCommandType _CommandType;
	CmdProcess::CommandUpdateElementType _CommandElementType;
	long _ObjectID;
	long _FloorContainerID;
	long _CompoGType;
	long _OwnerCompoGID;
	vector<long> _ObjectIDArray;
	vector<long> _ObjectGTypeArray;
};

class StructureModel_CLASS CmdModifyAbstract  : public CmdAbstract
{
public:

	CmdModifyAbstract() { m_bIsGeometryModified = true; m_bDummyAlteration = false; };
	virtual ~CmdModifyAbstract(void){};
	bool IsGeometryModified()
	{
		return m_bIsGeometryModified;
	}
	bool IsDummyAlteration()
	{
		return m_bDummyAlteration;
	}

protected:
	bool m_bIsGeometryModified;
	bool m_bDummyAlteration;
};
