#include "StdAfx.h"
#include "MSBraceG.h"
#include "MSContainer.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"

IMPLEMENT_SERIAL(MSBraceG, MSLineMemberG,  VERSIONABLE_SCHEMA | 1)
MSBraceG::MSBraceG()
{
	Init(true);
}

MSBraceG::~MSBraceG(void)
{
}

//bool MSBraceG::SetJoints(long BotJointID, long TopJointID)
//{
//	if(mp_Owner==NULL)
//		return FALSE;
//
//	mp_SConn = mp_Owner->GetJoint(BotJointID);
//	if(mp_SConn == NULL)
//		return false;
//
//	MSBaseBuilding* pBldg = NULL;
//	MSContainer* pNextJointOwner = NULL;
//	long nCurFloorNum = 0;
//	if(mp_Owner->GetType()==MSContainer::msFloor)
//	{
//		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
//		pNextJointOwner = pCurFloor->GetUpperFloor();
//		if(pNextJointOwner==NULL)
//			return false;
//		mp_EConn = pNextJointOwner->GetJoint(TopJointID);
//		pBldg = (MSBaseBuilding*)mp_Owner->GetOwner();
//	}
//	else if(mp_Owner->GetType()==MSContainer::msBuilding)
//		pBldg = (MSBaseBuilding*)mp_Owner;
//
//	if(pBldg==NULL)	return false;
//	mp_EConn = pBldg->GetJoint(TopJointID);
//	if(mp_EConn==NULL)	return false;
//
//	return true;
//}


void MSBraceG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Dump(buffer);
	buffer << m_bTensionOnly << m_Direction;
}

void MSBraceG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Recovery(buffer);
	buffer >> m_bTensionOnly >> m_Direction;
}


void MSBraceG::Serialize(CArchive &ar)
{

	MSLineMemberG::Serialize(ar);

	//if(ar.IsStoring())
	//{
	//	ar << m_bTensionOnly;
	//	ar << m_Direction;
	//}
	//else
	//{
	//	ar >> m_bTensionOnly;
	//	ar >> m_Direction;
	//}
}

MSObject* MSBraceG::Copy( bool bNewID /*= true*/ )
{
	MSBraceG* pNewObj;

	pNewObj = new MSBraceG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBraceG::CopyFromMe( MSBraceG* pSource, bool bNewID/*=true*/)
{
	MSLinkMemberG::CopyFromMe(pSource, bNewID);

	if(pSource)
	{
		m_bTensionOnly = pSource->m_bTensionOnly;
	}
}

void MSBraceG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSLineMemberG::Init(bInConstructor);

	m_bTensionOnly = true;
}


