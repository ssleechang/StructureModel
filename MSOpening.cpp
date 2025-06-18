#include "StdAfx.h"
#include "MSOpening.h"
#include "MSPolylineObject.h"
#include "EFS_Buffer.h"
#include "GMLib/MSVersionInfo.h"
#include "MSCompoG.h"
#include "MSCompoGUtil.h"

IMPLEMENT_SERIAL(MSOpening, MSElement, VERSIONABLE_SCHEMA | 1)
//long MSOpening::ms_LastID = 0;

////////////////////////////////////////////////////////////////////////
// NOT USED anymore - use MSOpeningG
////////////////////////////////////////////////////////////////////////
MSOpening::MSOpening(void)
{
	m_pOwner = NULL;
	m_eOpeningType = MSOpening::General;
    mp_PolylineInfo = new MSPolylineObject();
	mp_PolylineInfo->SetOwner(this);
}

MSOpening::~MSOpening(void)
{
	if(mp_PolylineInfo != NULL)
		delete mp_PolylineInfo;
	mp_PolylineInfo = NULL;
}

MSObject* MSOpening::Copy(bool bNewID /* = true*/)
{
	MSOpening* pNewObj;

	pNewObj = new MSOpening();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSOpening::CopyFromMe(MSOpening* pSource, bool bNewID /* = true*/)
{
	MSElement::CopyFromMe(pSource, bNewID);

	if(pSource)
	{
		m_pOwner = pSource->m_pOwner;
		m_eOpeningType = pSource->GetOpeningType();
		ma_MadeByCompoGID = pSource->ma_MadeByCompoGID;
		//mp_PolylineInfo->CopyFromMe(pSource->mp_PolylineInfo);
	}
}

void MSOpening::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	int type = 0;
	if(ar.IsStoring())
	{
		type = (int)m_eOpeningType;
		ar << type;
		mp_PolylineInfo->Serialize(ar);
		long NumID = ma_MadeByCompoGID.size();
		ar << NumID;
		for(long i = 0 ; i < NumID ; i++)
			ar << ma_MadeByCompoGID[i];
	}
	else
	{
		if(IsMVAddToBHQ())
		{
			ar >> type;
			m_eOpeningType = (eOpeningType)type;
		}
		mp_PolylineInfo->Serialize(ar);
		mp_PolylineInfo->SetOwner(this);
		if(MSVersionInfo::GetCurrentVersion() >= 20140807)
		{
			long NumID = 0;
			ar >> NumID;
			for(long i = 0 ; i < NumID ; i++)
			{
				long nCompoGID;
				ar >> nCompoGID;
				ma_MadeByCompoGID.push_back(nCompoGID);
			}	
		}
	}
}

void MSOpening::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	int type = (int)m_eOpeningType;
	buffer << type;

	mp_PolylineInfo->Dump(buffer);

	long NumID = ma_MadeByCompoGID.size();
	buffer << NumID;
	for(long i = 0 ; i < NumID ; i++)
		buffer << ma_MadeByCompoGID[i];
}

void MSOpening::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	int type = 0;
	buffer >> type;
	m_eOpeningType = (eOpeningType)type;
	mp_PolylineInfo->Recovery(buffer);
	mp_PolylineInfo->SetOwner(this);

	long NumID = 0;
	buffer >> NumID;
	for(long i = 0 ; i < NumID ; i++)
	{
		long nCompoGID;
		buffer >> nCompoGID;
		ma_MadeByCompoGID.push_back(nCompoGID);
	}
}

MSElement* MSOpening::GetOwnerCompoG()
{
	MSPolylineObject* pOwner = dynamic_cast<MSPolylineObject*>(m_pOwner);
	if (pOwner != nullptr)
		return pOwner->GetOwner();

	return m_pOwner;
}

void MSOpening::SetOwner(MSElement* pElement)
{
	m_pOwner = pElement;
}

bool MSOpening::MergeToMe( MSOpening* Other )
{
	if(Other == this)
		return false;
    GM2DPolyline* myPoly = this->GetProfile();
    GM2DPolyline* urPoly = Other->GetProfile();
 
	if(!myPoly->OverlapEx(*urPoly, DTOL_GM, FALSE))
		return false;

    vector<GM2DPolyline*> JoinedPolys;

	long NumOtherCompoG = Other->ma_MadeByCompoGID.size();
	if(NumOtherCompoG > 0)
	{
		myPoly->BoolOperation('A', urPoly, JoinedPolys, DTOL_GM);
		if (JoinedPolys.size() == 0)
			return false;

		//Area크기 순으로 정렬
		if (JoinedPolys.size() > 1)
		{
			sort(JoinedPolys.begin(), JoinedPolys.end(),
				[](GM2DPolyline* a, GM2DPolyline* b)->bool {return a->Area() > b->Area(); }
			);
		}
		
		myPoly->CopyFrom(JoinedPolys[0]);
		return false;
	}
	else
		myPoly->BoolOperation('U', urPoly, JoinedPolys, DTOL_GM);

    int count = JoinedPolys.size();
    if(count != 1)
        return false;

    myPoly->CopyFrom(JoinedPolys[0]);

	for(long i = 0 ; i < Other->ma_MadeByCompoGID.size() ; i++)
	{
		long CompoGID = Other->ma_MadeByCompoGID[i];
		bool HasCompoGID = false;
		for(long j = 0 ; j < ma_MadeByCompoGID.size() ; j++)
		{
			long MyCompoGID = ma_MadeByCompoGID[j];
			if(MyCompoGID == CompoGID)
			{
				HasCompoGID = true;
				break;;
			}
		}
		if(HasCompoGID == false)
			ma_MadeByCompoGID.push_back(CompoGID);
	}

	return true;
}

void MSOpening::RemoveMadeByCompoGIDAll()
{
	ma_MadeByCompoGID.clear();
}

void MSOpening::RemoveMadeByCompoGID(long nCompoGID)
{
	long NumID = ma_MadeByCompoGID.size();
	for(long i = NumID -1 ; i >= 0 ; i--)
	{
		if(ma_MadeByCompoGID[i] == nCompoGID)
			ma_MadeByCompoGID.erase(ma_MadeByCompoGID.begin() + i);
	}
}

bool MSOpening::HasMadeByCompoGID(long nCompoGID)
{
	long NumID = ma_MadeByCompoGID.size();
	for(long i = NumID -1 ; i >= 0 ; i--)
	{
		if(ma_MadeByCompoGID[i] == nCompoGID)
			return true;
	}

	return false;
}

void MSOpening::AddMadeByCompoGID(long nCompoGID)
{
	for (int i = 0; i < ma_MadeByCompoGID.size(); i++)
	{
		if (ma_MadeByCompoGID[i] == nCompoGID)
			return;
	}

	ma_MadeByCompoGID.push_back(nCompoGID);
}

bool MSOpening::IsMadeByCompoG()
{
	return ma_MadeByCompoGID.size() > 0 ? true:false;
}

MSFloor* MSOpening::GetFloor()
{
	MSElement* pElement = GetOwnerCompoG();
	if(pElement == nullptr)
		return nullptr;

	return pElement->GetFloor();
}

bool MSOpening::HasSamePolyMember()
{
	MSCompoG* pOwnerSlabG = dynamic_cast<MSCompoG*>(GetOwnerCompoG());
	if (pOwnerSlabG == nullptr)
		return false;

	GM2DPolyline* pOpeningProfile = GetProfile();

	vector<MSCompoG*> FindSlabGArr = MSCompoGUtil::FindCompoG(GetFloor(), pOpeningProfile, FindType_MSElement::msSlabG, true, true);
	vector<MSCompoG*> FindStairGArr = MSCompoGUtil::FindCompoG(GetFloor(), pOpeningProfile, FindType_MSElement::msStairG, true, true);
	
	for (int iCompoG = 0; iCompoG < FindSlabGArr.size(); iCompoG++)
		if (pOpeningProfile->SamePolyLine(FindSlabGArr[iCompoG]->GetProfile()))
			return true;

	for (int iCompoG = 0; iCompoG < FindStairGArr.size(); iCompoG++)
		if (pOpeningProfile->SamePolyLine(FindStairGArr[iCompoG]->GetProfile()))
			return true;

	return false;
}

bool MSOpening::IsSameOpening(MSOpening* urOpening)
{
	GM2DPolyline* myPoly = this->GetProfile();
	GM2DPolyline* urPoly = urOpening->GetProfile();

	if (myPoly->SamePolyLine(urPoly))
	{
		int myCnt = ma_MadeByCompoGID.size();
		int urCnt = urOpening->ma_MadeByCompoGID.size();
		if (myCnt != urCnt)
			return false;

		for (int i = 0; i < ma_MadeByCompoGID.size() ; i++)
		{
			if (ma_MadeByCompoGID[i] != urOpening->ma_MadeByCompoGID[i])
				return false;
		}

		return true;
	}

	return false;
}