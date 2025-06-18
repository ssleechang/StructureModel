#include "StdAfx.h"
#include "MSObject.h"
#include "MSRevision.h"

#include "GMLib\XErrorObject.h"
#include "GMLib\XErrorContainer.h"
#include "GMLib/MSVersionInfo.h"
#include "CmdProcess/CmdProcessHelper.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(MSObject, CObject, VERSIONABLE_SCHEMA | 1)
MSObject::MSObject()
{
	Init();
}

MSObject::~MSObject(void)
{
}

MSObject* MSObject::Copy(bool bNewID)
{
	MSObject* pNewObj;

	pNewObj = new MSObject();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSObject::CopyFromMe(MSObject* pSource, bool bNewID/*=true*/)
{
	// 	if(pSource)
	// 		m_TimeStamp = pSource->m_TimeStamp;
}

void MSObject::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);

	// 	if(ar.IsStoring())
	// 		ar << m_TimeStamp;
	// 	else
	// 		ar >> m_TimeStamp;
}


void MSObject::SetTime()
{
	//	m_TimeStamp = CTime::GetCurrentTime();
}

void MSObject::Init(bool bInConstructor/*=false*/)
{
	SetTime();
}

void MSObject::StartEdit(CmdProcess::CommandUpdateElementType elementType, long ModelID, long FloorID, bool isGeometryModified, bool isDummyAlteration)
{
	//Dump()
	_CmdModifyModelControl.StartEdit(this, elementType, ModelID, FloorID, isGeometryModified, isDummyAlteration);
}

//void MSObject::StartEdit(CmdProcess::CommandUpdateElementType elementType, long ModelID, long OwnerCompoGID, long FloorID, bool isGeometryModified, bool isCheckModelSkip)
void MSObject::StartEdit(CmdProcess::CommandUpdateElementType elementType, long ModelID, long OwnerCompoGID, long FloorID, bool isGeometryModified, bool isCheckModelSkip, bool isDummyAlteration)
{
	//Dump()
	//_CmdModifyModelControl.StartEdit(this, elementType, ModelID, OwnerCompoGID, FloorID, isGeometryModified, isCheckModelSkip);
	//_CmdModifyModelControl.StartEdit(this, elementType, ModelID, OwnerCompoGID, FloorID, isGeometryModified, isDummyAlteration);
	_CmdModifyModelControl.StartEdit(this, elementType, ModelID, OwnerCompoGID, FloorID, isGeometryModified, isCheckModelSkip, isDummyAlteration);
}

void MSObject::EndEdit()
{
	_CmdModifyModelControl.EndEdit();
}

void MSObject::EndEdit_Force()
{
	_CmdModifyModelControl.EndEdit_Force();
}

void MSObject::EndEdit_SkipTrimAgain()
{
	_CmdModifyModelControl.EndEdit_SkipTrimAgain();
}

//코드 통합 이후 BH-Q에 추가해야 되는 변수
bool MSObject::IsMVAddToBHQ()
{
	if (IsBHForBHAMode()
		|| (IsBHQorBHCEMode() && (MSVersionInfo::GetCurrentVersion() >= 20240425 || MSVersionInfo::IsOldTotalVersion())))
		return true;

	return false;
}

//코드 통합 이후 BH-A에 추가해야 되는 변수
bool MSObject::IsMVAddToBHA()
{
	if (IsBHQorBHCEMode()
		|| (IsBHForBHAMode() && (MSVersionInfo::GetCurrentVersion() >= 20240425 || MSVersionInfo::IsOldTotalVersion())))
		return true;

	return false;
}

bool MSObject::IsBHQorBHCEMode()
{
	if (IsBHQMode() || IsBHCEMode())
		return true;

	return false;
}

bool MSObject::IsBHForBHAMode()
{
	if (IsBHFMode() || IsBHAMode())
		return true;

	return false;
}

bool MSObject::IsBHQMode()
{
	if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Rebar)
		return true;

	return false;
}

bool MSObject::IsBHAMode()
{
	if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Finish)
		return true;

	return false;
}

bool MSObject::IsBHFMode()
{
	if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Formwork)
		return true;

	return false;
}

bool MSObject::IsBHCEMode()
{
	if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::CE)
		return true;

	return false;
}

#ifdef _DEBUG

void MSObject::AssertValid() const
{
	CObject::AssertValid();
}

void MSObject::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

template <class T>
void MSObject::DeleteVecArray(vector<T*>* aObject)
{
	while (!aObject->empty())
	{
		delete aObject->back();
		aObject->pop_back();
	}
}


#endif


MSHistoryObject::MSHistoryObject(CString& historyID)
{
	_isDeleted = false;
	_historyID = historyID;
}

MSHistoryObject::~MSHistoryObject()
{

}

void MSHistoryObject::SetDeleted(bool isDeleted)
{
	_isDeleted = isDeleted;
	if (_isDeleted == true)//같은 UserState내에서 Delete -> New될 경우 (Modify시) , Delete된것을 복구시키기 위해 사용한다.
		CmdProcessHelper::GetHistoryManager()->RegisterHistoryObjectAsDeleted(this);
}
