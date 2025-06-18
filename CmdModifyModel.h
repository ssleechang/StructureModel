#pragma once
#include "CmdProcess/CmdAbstract.h"
#include "EFS_Buffer.h"

class MSObject;
class StructureModel_CLASS CmdModifyModelObject : public CmdModifyAbstract
{
public:
	CmdModifyModelObject(MSObject* modelObject, CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long FloorID, bool isGeometryModified = true, bool isDummyAlteration = false);
	CmdModifyModelObject(MSObject* modelObject, CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long OwnerCompoGID, long FloorID, bool isGeometryModified = true, bool isCheckModelSkip = false, bool isDummyAlteration = false);

	~CmdModifyModelObject(void)
	{
		if ( Undone !=_CommandStatus )
			return;
	}

	void StartEdit();
	void EndEdit();
	void EndEdit_Force();
	void EndEdit_SkipTrimAgain();

	void Doit();

	void Undo();

	bool CheckData();
	

	virtual MSObject* GetOldValue()
	{
		return _ModelObject;
	}

	virtual void SetTimeStamp(CTime timeStamp);

	

private:
	EFS::EFS_Buffer _OriginBuffer;
	EFS::EFS_Buffer _AfterBuffer;

	MSObject* _ModelObject;
	
	void TrimAgain();

	bool m_bResetConnections;
	bool m_bSkipTrimOld;
	bool m_bSkipTrimNew;
	bool m_bIsCheckModelSkip;
};

class StructureModel_CLASS CmdModifyModelObjectControl
{
public:
	CmdModifyModelObjectControl();
	void StartEdit(MSObject* msObject, CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long FloorID , bool isGeometryModified = true, bool isDummyAlteration = false);
	//void StartEdit(MSObject* msObject, CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long OwnerCompoGID, long FloorID, bool isGeometryModified = true, bool isCheckModelSkip = false);
	//void StartEdit(MSObject* msObject, CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long OwnerCompoGID, long FloorID , bool isGeometryModified = true, bool isDummyAlteration = false);
	void StartEdit(MSObject* msObject, CmdProcess::CommandUpdateElementType elementType, long ModelObjectID, long OwnerCompoGID, long FloorID , bool isGeometryModified = true, bool isCheckModelSkip = false, bool isDummyAlteration = false);

	void EndEdit();
	void EndEdit_Force();
	void EndEdit_SkipTrimAgain();

private:
	CmdModifyModelObject* modifyModelObject;
};