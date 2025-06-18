#include "StdAfx.h"
#include "MSBarSet.h"
#include "MSRebarShape.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"

//#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSBarSet, MSObject, VERSIONABLE_SCHEMA | 1)
long MSBarSet::ms_LastID = 0;
MSBarSet::MSBarSet(void)
{
	mp_Bldg = NULL;
	mp_Owner = NULL;
	m_nRootType = 0;
	m_nRootType = -1;
	m_nBarType = SD40;
	m_nConsZoneID = 0;
}

MSBarSet::~MSBarSet(void)
{
	DeleteShapeArr();
}

long MSBarSet::GetID()
{
    if(m_ID == 0) // 
    {
        m_ID = ms_LastID + 1;
        ms_LastID = m_ID;
        return m_ID;
    }

    return m_ID;
}
void MSBarSet::DeleteShapeArr()
{
	vector<MSRebarShape*>::iterator it;
	for(it=ma_Shape.begin(); it != ma_Shape.end(); it++)
		delete *it;
	ma_Shape.clear();
}

long MSBarSet::GetNumBars()
{
	return ma_Loc.size();
}

void MSBarSet::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	//long nOwnerID = 0;
	//long nFloorNum = 0;
	//if(ar.IsStoring())
	//{
	//	ar << m_Name;
	//	ar << (int)ma_Shape.size();
	//	vector<MSRebarShape *>::iterator itShape;
	//	for(itShape=ma_Shape.begin(); itShape != ma_Shape.end(); itShape++ )
	//		(*itShape)->Serialize(ar);

	//	ar << (int)ma_Loc.size();
	//	vector<GMVector>::iterator itLoc;
	//	for(itLoc=ma_Loc.begin(); itLoc != ma_Loc.end(); itLoc++ )
	//		(*itLoc).Serialize(ar);

	//	if(mp_Owner)
	//	{
	//		nOwnerID = mp_Owner->m_ID;
	//		MSContainer* pMemberOwner = mp_Owner->GetOwner();
	//		if(pMemberOwner)
	//		{
	//			if(pMemberOwner->m_nType==MSContainer::msFloor)
	//				nFloorNum = ((MSFloor*)pMemberOwner)->m_nFloorNum;
	//		}
	//	}
	//	ar << nOwnerID;
	//	ar << nFloorNum;

	//	ar << (int)ma_CompoG.size();
	//	vector<MSCompoG *>::iterator itCompoG;
	//	for(itCompoG=ma_CompoG.begin(); itCompoG != ma_CompoG.end(); itCompoG++ )
	//	{
	//		nFloorNum = 0;
	//		if(*itCompoG==NULL)	continue;
	//		if((*itCompoG)->GetOwner()->m_nType==MSContainer::msFloor)
	//		{
	//			MSFloor* pOwner = (MSFloor*)(*itCompoG)->GetOwner();
	//			nFloorNum = pOwner->m_nFloorNum;
	//		}
	//		ar << nFloorNum;
	//		ar << (*itCompoG)->m_ID;
	//	}
	//	ar << m_nRootType;
	//	ar << m_nRootIndex;
	//	ar << m_nRootUnitIndex;
	//	ar << m_nFloorNum;

	//	ar << m_nConsZoneID;
	//}
	//else
	//{
	//	ar >> m_Name;
	//	int NumObject;
	//	ar >> NumObject;
	//	for(int i=0; i< NumObject; i++ )
	//	{
	//		MSRebarShape* pRebarShape = new MSRebarShape;
	//		pRebarShape->Serialize(ar);
	//		ma_Shape.push_back(pRebarShape);
	//	}

	//	ar >> NumObject;
	//	for(int i=0; i< NumObject; i++ )
	//	{
	//		GMVector Loc;
	//		Loc.Serialize(ar);
	//		ma_Loc.push_back(Loc);
	//	}
	//	ar >> nOwnerID;
	//	ar >> nFloorNum;
	//	if(nFloorNum==0)
	//		mp_Owner = mp_Bldg->GetCompoG(nOwnerID);
	//	else
	//	{
	//		MSFloor* pFloor = mp_Bldg->GetFloor(nFloorNum);
	//		mp_Owner = pFloor->GetCompoG(nOwnerID);
	//	}

	//	ar >> NumObject;
	//	long nCompoGID;
	//	for(int i=0; i< NumObject; i++ )
	//	{
	//		ar >> nFloorNum;
	//		ar >> nCompoGID;
	//		MSCompoG* pCompoG = NULL;
	//		if(nFloorNum==0)
	//			pCompoG = mp_Bldg->GetCompoG(nCompoGID);
	//		else
	//		{
	//			MSFloor* pFloor = mp_Bldg->GetFloor(nFloorNum);
	//			pCompoG = pFloor->GetCompoG(nCompoGID);
	//		}
	//		if(pCompoG==NULL)	ASSERT(0);
	//		ma_CompoG.push_back(pCompoG);
	//	}
	//	ar >> m_nRootType;
	//	ar >> m_nRootIndex;
	//	ar >> m_nRootUnitIndex;
	//	ar >> m_nFloorNum;
	//	SetLastID(m_ID);

	//	if(MSVersionInfo::GetCurrentVersion() >= 20130111)
	//	{
	//		ar >> m_nConsZoneID;
	//	}
	//}
}

bool MSBarSet::Add(MSRebarShape* pShape)
{
	long nSize = ma_Shape.size();
	ma_Shape.push_back(pShape);
	return true;
}
bool MSBarSet::Add(GMVector Loc)
{
	ma_Loc.push_back(Loc);
	return true;
}
bool MSBarSet::Add(MSCompoG* pCompoG)
{
	ma_CompoG.push_back(pCompoG);
	return true;
}

void MSBarSet::SetOwner( MSCompoG* pNewOwner )
{
	mp_Owner = pNewOwner;
}

MSCompoG* MSBarSet::GetOwner()
{
	return	mp_Owner;
}

double MSBarSet::GetTotalLength()
{
	double dTotalLength = 0.;
	vector <MSRebarShape*>::iterator it;
	for(it=ma_Shape.begin(); it != ma_Shape.end(); it++)
	{
		MSRebarShape* pRebarShape = *it;
		long NumLoc = GetNumBars();
		dTotalLength += pRebarShape->GetTotalLength()*NumLoc;
	}
	return dTotalLength;
}


double MSBarSet::GetDia()
{
	if(ma_Shape.size()==0)
		return 0.0;

//	return ma_Shape[0]->m_dDia;
	vector<MSRebarShape*>::iterator it = ma_Shape.begin();
	if(it == ma_Shape.end())
		return 0.0;
	return	(*it)->m_dDia;

}
// Barset은 하나의 Dia만을 갖는 것으로 제한되어있으므로 의미없다. 대표 dia값은 첫번째 rebarshape의 dia로 한다.
//void MSBarSet::GetTotalLengthByDia( map<double,double>& mapLengthByDia )
//{
//	vector <MSRebarShape*>::iterator it;
//	long NumLoc = GetNumBars();
//	for(it=ma_Shape.begin(); it != ma_Shape.end(); it++)
//	{
//		MSRebarShape* pRebarShape = *it;
//		
//		map<double,double>::iterator map_it = mapLengthByDia.find(pRebarShape->m_dDia);
//		if(map_it == mapLengthByDia.end())
//		{
//			mapLengthByDia.insert(make_pair(pRebarShape->m_dDia, pRebarShape->GetTotalLength()));
//		}
//		else
//		{
//			map_it->second +=  pRebarShape->GetTotalLength();
//		}
//	}	
//}

MSFloor* MSBarSet::GetFloor()
{
	if(mp_Owner == NULL)
		return NULL;
	if(mp_Owner->GetType() == MSElement::msFloor)//바로 floor에 소속
	{
		MSFloor* pFloor = dynamic_cast<MSFloor*>(mp_Owner);
		return pFloor;
	}
	else if(mp_Owner->GetOwner() != NULL && mp_Owner->GetOwner()->GetType() == MSElement::msFloor )//일반적인 floor에 속한 멤버에 소속
	{
		MSFloor* pFloor = dynamic_cast<MSFloor*>(mp_Owner->GetOwner());
		return pFloor;
	}
	else//기타 빌딩 등에 소속
		return NULL;

}

MSElement::Type_MSCompoG MSBarSet::GetOwnerType()
{
	if(mp_Owner == NULL )
		return (MSElement::Type_MSCompoG)1;//building
	return mp_Owner->GetType();
}

CString MSBarSet::GetName()
{
	return m_Name;
}

// return SD30, SD40, SD50.
CString MSBarSet::GetStrengthName()
{
	CString strDefName = STR_SD40;
	switch(m_nBarType)
	{
	case SD30:	return STR_SD30;
	case SD40:	return STR_SD40;
	case SD50:	return STR_SD50;
	case SD60:	return STR_SD60;
	}
	return strDefName;
}
// D, HD, SHD.
CString MSBarSet::GetBarTypeName()	
{
	CString strDefBarType = STR_BARTYPE_HD;
	switch(m_nBarType)
	{
	case SD30:	return STR_BARTYPE_D;
	case SD40:	return STR_BARTYPE_HD;
	case SD50:	return STR_BARTYPE_SHD;
	case SD60:	return STR_BARTYPE_TD;
	}
	return strDefBarType;

}